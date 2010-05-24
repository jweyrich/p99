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
DEFINE_DEFARG(orwl_mirror_init, , (orwl_endpoint){{0}}, (orwl_endpoint){{0}});

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
      rh->wh = NEW(orwl_wh);
      orwl_wh* cli_wh = NEW(orwl_wh);
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
      orwl_wh* cli_wh = NEW(orwl_wh);
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
          report(1, "piggybacked on srvID %" PRIx64, rh->svrID);
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
          orwl_wh* wh = NEW(orwl_wh);
          report(1, "new handle %p for remote %" PRIx64, (void*)wh, rh->svrID);
          assert(&rq->local == cli_wh->location);
          // The handle on the remote side might have been acquired
          // before we even woke up. In that case we may already have
          // received the information and will have to release and delete
          // cli_wh.
          bool last = false;
          MUTUAL_EXCLUDE(rq->local.mut) {
            assert(rq->local.tail == cli_wh);
            orwl_wq_request_locked(&rq->local, wh, 1);
            // Finally have rh point on wh and mark wh as being
            // inclusive.
            wh->svrID = rh->svrID;
            rh->wh = wh;
            orwl_wh_unload(cli_wh, 1);
            last = (cli_wh->tokens == 0);
          }
          if (last) {
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
  /* Detect if we are the last user of this handle */
  bool last = false;
  pthread_mutex_lock(&rq->mut);
  MUTUAL_EXCLUDE(wq->mut) {
    assert(wq == &rq->local);
    assert(wq->head == wh);
    last = (wh->tokens == 1);
    if (!last) {
      orwl_handle_init(rh);
      pthread_mutex_unlock(&rq->mut);
    }
    orwl_wh_unload(wh);
  }
  if (last) {
    state = orwl_wh_release(wh);
    orwl_handle_init(rh);
    pthread_mutex_unlock(&rq->mut);
    orwl_rpc(&there, seed, auth_sock_release, svrID);
    /* We should be the last to have a reference to this handle so
       we may destroy it. */
    orwl_wh_delete(wh);
  }
  return state;
}

typedef orwl_handle _orwl_handle_cancel;

_orwl_handle_cancel* _orwl_handle_cancel_init(_orwl_handle_cancel* H) {
  return orwl_handle_init(H);
}

void _orwl_handle_cancel_destroy(_orwl_handle_cancel* H) {
  orwl_handle_destroy(H);
}

DECLARE_NEW_DELETE(_orwl_handle_cancel);
DEFINE_NEW_DELETE(_orwl_handle_cancel);

DECLARE_THREAD(_orwl_handle_cancel);


DEFINE_THREAD(_orwl_handle_cancel) {
  orwl_acquire(Arg);
  orwl_release(Arg);
  /* We should be the last to have a reference to this handle so this
     may be automatically destroyed when we exit this thread. */
}

orwl_state orwl_cancel(orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_valid;
  if (!rh || !rh->wh) return orwl_valid;
  _orwl_handle_cancel* rhcp = memcpy(NEW(_orwl_handle_cancel), rh, sizeof(*rh));
  _orwl_handle_cancel_create(rhcp, NULL);
  orwl_handle_destroy(rh);
  return state;
}



DEFINE_DEFARG(orwl_write_request, , , seed_get());
DEFINE_DEFARG(orwl_read_request, , , seed_get());
DEFINE_DEFARG(orwl_release, , seed_get());
DEFINE_DEFARG(orwl_cancel, , seed_get());

DEFINE_ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_release, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle);


DEFINE_ORWL_TYPE_DYNAMIC(orwl_handle,
                         ORWL_REGISTER_ALIAS(orwl_acquire),
                         ORWL_REGISTER_ALIAS(orwl_release),
                         ORWL_REGISTER_ALIAS(orwl_cancel)
                         );
