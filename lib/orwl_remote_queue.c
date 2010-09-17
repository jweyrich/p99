/*
** orwl_remote_queue.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Apr  6 15:30:01 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_remote_queue.h"
#include "orwl_socket.h"
#include "orwl_server.h"
#include "orwl_auth_sock_functions.h"

orwl_mirror *orwl_mirror_init(orwl_mirror *rq, orwl_endpoint h, orwl_endpoint t);

void orwl_mirror_destroy(orwl_mirror *rq);

DEFINE_NEW_DELETE(orwl_mirror);

orwl_handle *orwl_handle_init(orwl_handle *rh);
void orwl_handle_destroy(orwl_handle *rh);

DEFINE_NEW_DELETE(orwl_handle);

orwl_state orwl_acquire(orwl_handle* rh);
orwl_state orwl_test(orwl_handle* rh);

orwl_state orwl_write_request(orwl_mirror *rq, orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
  if (rq && rh)
  MUTUAL_EXCLUDE(rq->mut) {
    if (!rh->wh) {
      // insert two wh in the local queue
      rh->wh = P99_NEW(orwl_wh);
      orwl_wh* cli_wh = P99_NEW(orwl_wh);
      state = orwl_wq_request(&rq->local, &cli_wh, 1, &rh->wh, 1);
      if (state == orwl_requested) {
        assert(!rh->rq);
        /* Send the insertion request with the id of cli_wh to the other
           side. As result retrieve the ID on the other side that is to be
           released when we release here. */
        rh->svrID = orwl_rpc(&rq->there, seed, auth_sock_write_request,
                             rq->there.index,
                             (uintptr_t)cli_wh,
                             port2host(&rq->here.port)
                             );
        if (rh->svrID) {
          /* Link us to rq */
          rh->rq = rq;
        } else {
          report(1, "bad things happen: unable to get a write insertion request from the other side.");
          /* something went wrong */
          state = orwl_invalid;
        }
      } else {
        /* roll back */
        orwl_wh_delete(rh->wh); rh->wh = NULL;
        orwl_wh_delete(cli_wh);
        state = orwl_invalid;
      }
    }
  }
  return state;
}

orwl_state orwl_read_request(orwl_mirror *rq, orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
  if (rq && rh)
  MUTUAL_EXCLUDE(rq->mut) {
    if (!rh->wh) {
      orwl_wh* cli_wh = P99_NEW(orwl_wh);
      orwl_wh* wh_inc = NULL;
      /* first try to piggyback the latest wh in the local list */
      state = orwl_wq_request(&rq->local, &wh_inc, 1);
      assert(!wh_inc || wh_inc->svrID);
      /* Otherwise, the dummy handle is loaded with two tokens, one
         for the remote event of acquisition of the lock. The other is
         used here locally to ensure that cli_wh is not freed before
         we have finished our work, here. */
      state = orwl_wq_request(&rq->local, &cli_wh, 2);
      /* Send the insertion request to the other side. This consists
         of sending cli_wh, for the case that this is will be
         considered a new request, and the svrID that was memorized on
         wh_inc, if any. As result retrieve the ID on the other side
         that is to be released when we release here. */
      rh->svrID = orwl_rpc(&rq->there, seed, auth_sock_read_request,
                           rq->there.index,
                           (uintptr_t)cli_wh,
                           wh_inc ? wh_inc->svrID : 0,
                           port2host(&rq->here.port)
                           );
      if (!rh->svrID) {
        state = orwl_invalid;
        report(1, "bad things happen: unable to get a read insertion request from the other side.");
        orwl_wh_acquire(cli_wh, 2);
        orwl_wh_release(cli_wh);
        orwl_wh_delete(cli_wh);
      } else {
        /* Link us to rq */
        rh->rq = rq;
        if (state == orwl_requested
            && wh_inc
            && wh_inc->svrID == rh->svrID) {
          report(0, "piggybacked on srvID %" PRIx64, rh->svrID);
          /* remote and local queue have still the same last element, a
             corresponding inclusive pair at the tail of the their
             list. */
          rh->wh = wh_inc;
          /* Clean up the queue by removing the dummy handle that we
             inserted. */
          assert(&rq->local == cli_wh->location);
          MUTUAL_EXCLUDE(rq->local.mut) {
            assert(wh_inc->location == &rq->local);
            assert(wh_inc->next == cli_wh);
            assert(wh_inc->location->tail == cli_wh);
            wh_inc->next = NULL;
            wh_inc->location->tail = wh_inc;
            cli_wh->tokens = 0;
            cli_wh->location = NULL;
            orwl_wh_delete(cli_wh);
          }
        } else {
          // A new handle has to be inserted in the local queue
          orwl_wh* wh = P99_NEW(orwl_wh);
          wh->svrID = rh->svrID;
          report(0, "new handle %p for remote %" PRIx64, (void*)wh, rh->svrID);
          assert(&rq->local == cli_wh->location);
          // The handle on the remote side might have been acquired
          // before we even woke up. In that case we may already have
          // received the information and will have to release and delete
          // cli_wh.
          bool last_cli = false;
          bool last_inc = false;
          MUTUAL_EXCLUDE(rq->local.mut) {
            // If we added a token to an existing handle but now we
            // see that this corresponds to a different priority
            // (because remotely there was something in between) we
            // have to unload this again.
            if (wh_inc) {
              orwl_wh_unload(wh_inc, 1);
              last_inc = (wh_inc->tokens == 0);
            }
            assert(rq->local.tail == cli_wh);
            orwl_wq_request_locked(&rq->local, wh, 1);
            // Finally have rh point on wh and mark wh as being
            // inclusive.
            rh->wh = wh;
            orwl_wh_unload(cli_wh, 1);
            last_cli = (cli_wh->tokens == 0);
          }
          if (last_inc) {
            assert(wh_inc);
            state = orwl_wh_release(wh_inc);
            orwl_wh_delete(wh_inc);
          }
          if (last_cli) {
            state = orwl_wh_release(cli_wh);
            orwl_wh_delete(cli_wh);
          }
        }
      }
    }
  }
  return state;
}

orwl_state orwl_release(orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_valid;
  assert(rh);
  assert(rh->wh);
  /* To be able to re-initialize rh in the middle of the procedure,
     store all necessary data on the stack.*/
  orwl_wh* wh = rh->wh;
  orwl_mirror* rq = rh->rq;
  uint64_t svrID = rh->svrID;
  orwl_endpoint there = rq->there;
  orwl_wq* wq = wh->location;
  /* If we are the last that holds this handle we have to prepare the
     buffer for the return message while we hold the inner lock. */
  uint64_t* mess = NULL;
  size_t len = 2;
  /* Detect if we are the last user of this handle */
  bool last = false;
  pthread_mutex_lock(&rq->mut);
  MUTUAL_EXCLUDE(wq->mut) {
    assert(wq == &rq->local);
    assert(wq->head == wh);
    last = (wh->tokens == 1);
    if (last) {
      bool inclusive = wh->svrID;
      size_t extend = (inclusive ? 0 : wq->data_len);
      report(false, "adding extend of length %zu?", extend);
      len += extend;
      mess = uint64_t_vnew(len);
      mess[0] = ORWL_OBJID(auth_sock_release);
      mess[1] = svrID;
      if (extend) {
        report(false, "adding suplement of length %zu", extend);
        memcpy(&mess[2], wq->data, extend * sizeof(uint64_t));
      }
    } else {
      orwl_handle_init(rh);
      pthread_mutex_unlock(&rq->mut);
    }
    orwl_wh_unload(wh);
  }
  if (last) {
    state = orwl_wh_release(wh);
    orwl_handle_init(rh);
    pthread_mutex_unlock(&rq->mut);
    orwl_send(&there, seed, len, mess);
    /* We should be the last to have a reference to this handle so
       we may destroy it. */
    orwl_wh_delete(wh);
    uint64_t_vdelete(mess);
  }
  return state;
}

typedef orwl_handle orwl_handle_cancel;

orwl_handle_cancel* orwl_handle_cancel_init(orwl_handle_cancel* H) {
  if (!H) return NULL;
  return orwl_handle_init(H);
}

void orwl_handle_cancel_destroy(orwl_handle_cancel* H) {
  orwl_handle_destroy(H);
}

DECLARE_NEW_DELETE(orwl_handle_cancel);
DEFINE_NEW_DELETE(orwl_handle_cancel);

DECLARE_THREAD(orwl_handle_cancel);


DEFINE_THREAD(orwl_handle_cancel) {
  orwl_acquire(Arg);
  orwl_release(Arg);
  /* We should be the last to have a reference to this handle so this
     may be automatically destroyed when we exit this thread. */
}

orwl_state orwl_cancel(orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_valid;
  if (!rh || !rh->wh) return orwl_valid;
  orwl_handle_cancel* rhcp = memcpy(P99_NEW(orwl_handle_cancel), rh, sizeof(*rh));
  orwl_handle_cancel_create(rhcp, NULL);
  orwl_handle_destroy(rh);
  return state;
}

void orwl_map(orwl_handle* rh, uint64_t** data, size_t* data_len);
void orwl_resize(orwl_handle* rh, size_t data_len);

DEFINE_ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_release, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle);


DEFINE_ORWL_TYPE_DYNAMIC(orwl_handle,
                         ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle),
                         ORWL_REGISTER_ALIAS(orwl_release, orwl_handle),
                         ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle)
                         );
