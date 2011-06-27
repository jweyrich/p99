/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_remote_queue.h"
#include "orwl_socket.h"
#include "orwl_server.h"
#include "orwl_proc_symbols.h"
#include "orwl_timing.h"

DEFINE_ONCE(orwl_mirror, orwl_wq, orwl_rand, orwl_handle) {
  // empty
}

DEFINE_ONCE(orwl_handle, orwl_mirror, orwl_wh) {
  // empty
}

P99_INSTANTIATE(orwl_mirror*, orwl_mirror_init, orwl_mirror*, orwl_server*, orwl_endpoint);
P99_INSTANTIATE(void, orwl_mirror_destroy, orwl_mirror*);

DEFINE_NEW_DELETE(orwl_mirror);

void orwl_mirror_connect(orwl_mirror *rq, orwl_server* srv, orwl_endpoint endp) {
  orwl_mirror_init(rq, srv, endp);
  /* wait until the other side is up. */
  /* ep.port is already in host order */
  while (orwl_rpc(srv, &endp, seed_get(), orwl_proc_insert_peer, port2host(&srv->host.ep.port))
         == P99_TMAX(uint64_t)) {
    if (!orwl_alive(srv)) break;
    sleepfor(0.02);
  }
}

P99_INSTANTIATE(orwl_handle*, orwl_handle_init, orwl_handle*);
P99_INSTANTIATE(void, orwl_handle_destroy, orwl_handle*);

DEFINE_NEW_DELETE(orwl_handle);

P99_INSTANTIATE(bool, orwl_inclusive, orwl_handle*);
P99_INSTANTIATE(orwl_state, orwl_acquire, orwl_handle*);
P99_INSTANTIATE(orwl_state, orwl_test, orwl_handle*);

orwl_state orwl_write_request(orwl_mirror *rq, orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
  ORWL_TIMER(total_write_request) {
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
        ORWL_TIMER(rpc_write_request)
        rh->svrID = orwl_rpc(rq->srv, &rq->there, seed, orwl_proc_write_request,
                             rq->there.index,
                             (uintptr_t)cli_wh,
                             port2host(&rq->srv->host.ep.port)
                             );
        if (rh->svrID && (rh->svrID != ORWL_SEND_ERROR)) {
          /* Link us to rq */
          rh->rq = rq;
        } else {
          report(1, "bad things happen: unable to get a write insertion request from the other side.");
          /* something went wrong */
          state = orwl_invalid;
        }
      } else {
        /* roll back */
        orwl_wh_delete(rh->wh); rh->wh = 0;
        orwl_wh_delete(cli_wh);
        state = orwl_invalid;
      }
    }
  }
  }
  return state;
}

orwl_state orwl_read_request(orwl_mirror *rq, orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
  ORWL_TIMER(total_read_request) {
  if (rq && rh)
  MUTUAL_EXCLUDE(rq->mut) {
    if (!rh->wh) {
      orwl_wh* cli_wh = P99_NEW(orwl_wh);
      orwl_wh* wh_inc = 0;
      /* first try to piggyback the latest wh in the local list */
      state = orwl_wq_request(&rq->local, &wh_inc, 1);
      assert(!wh_inc || wh_inc->svrID);
      assert(!wh_inc || (state == orwl_requested));
      /* Otherwise, the dummy handle is loaded with two tokens, one
         for the remote event of acquisition of the lock. The other is
         used here locally to ensure that cli_wh is not freed before
         we have finished our work, here. */
      state = orwl_wq_request(&rq->local, &cli_wh, 2);
      if (state != orwl_requested)
        report(1, "orwl_wh %p could not be requested, state is %s",
               (void*)cli_wh,
               orwl_state_getname(state));
      /* Send the insertion request to the other side. This consists
         of sending cli_wh, for the case that this is will be
         considered a new request, and the svrID that was memorized on
         wh_inc, if any. As result retrieve the ID on the other side
         that is to be released when we release here. */
      ORWL_TIMER(rpc_read_request)
      rh->svrID = orwl_rpc(rq->srv, &rq->there, seed, orwl_proc_read_request,
                           rq->there.index,
                           (uintptr_t)cli_wh,
                           wh_inc ? wh_inc->svrID : 0,
                           port2host(&rq->srv->host.ep.port)
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
            wh_inc->next = 0;
            wh_inc->location->tail = wh_inc;
            cli_wh->location = 0;
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
            if (wh_inc) MUTUAL_EXCLUDE(wh_inc->mut) {
              last_inc = !orwl_wh_unload(wh_inc, 1);
            }
            assert(rq->local.tail == cli_wh);
            orwl_wq_request_append(&rq->local, wh, 1);
            // Finally have rh point on wh and mark wh as being
            // inclusive.
            rh->wh = wh;
            MUTUAL_EXCLUDE(cli_wh->mut) {
              last_cli = !orwl_wh_unload(cli_wh, 1);
            }
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
          state = orwl_requested;
        }
      }
    }
  }
  }
  return state;
}

orwl_state orwl_release(orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_valid;
  ORWL_TIMER(total_release) {
  assert(rh);
  assert(rh->wh);
  /* To be able to re-initialize rh in the middle of the procedure,
     store all necessary data on the stack.*/
  orwl_wh* const wh = rh->wh;
  orwl_mirror*const rq = rh->rq;
  uint64_t const svrID = rh->svrID;
  orwl_server *srv = rq->srv;
  orwl_endpoint const there = rq->there;
  orwl_wq*const wq = wh->location;
  /* If we are the last that holds this handle we have to prepare the
     buffer for the return message while we hold the inner lock. */
  uint64_t* mess = 0;
  size_t len = 2;
  /* Detect if we are the last user of this handle */
  bool last = false;
  pthread_mutex_lock(&rq->mut);
  MUTUAL_EXCLUDE(wq->mut)
  MUTUAL_EXCLUDE(wh->mut) {
    assert(wq == &rq->local);
    assert(wq->head == wh);
    last = !orwl_wh_unload(wh);
    if (last) {
      bool inclusive = wh->svrID;
      size_t extend = (inclusive ? 0 : wq->data_len);
      report(false, "adding extend of length %zu?", extend);
      len += extend;
      mess = uint64_t_vnew(len);
      mess[0] = ORWL_OBJID(orwl_proc_release);
      mess[1] = svrID;
      if (extend) {
        report(false, "adding suplement of length %zu", extend);
        ORWL_TIMER(copy_data_release)
        memcpy(&mess[2], wq->data, extend * sizeof(uint64_t));
      }
    } else {
      orwl_count_inc(&wh->finalists);
      orwl_handle_init(rh);
      pthread_mutex_unlock(&rq->mut);
    }
  }
  if (last) {
    orwl_count_wait(&wh->finalists);
    state = orwl_wh_release(wh);
    orwl_handle_init(rh);
    pthread_mutex_unlock(&rq->mut);
    ORWL_TIMER(send_data_release)
    orwl_send(srv, &there, seed, len, mess);
    /* We should be the last to have a reference to this handle so
       we may destroy it. */
    orwl_wh_delete(wh);
    uint64_t_vdelete(mess);
  } else {
    orwl_count_dec(&wh->finalists);
  }
  }
  return state;
}

typedef orwl_handle orwl_handle_cancel;

inline
orwl_handle_cancel* orwl_handle_cancel_init(orwl_handle_cancel* H) {
  if (!H) return 0;
  return orwl_handle_init(H);
}

P99_INSTANTIATE(orwl_handle_cancel*, orwl_handle_cancel_init, orwl_handle_cancel*);

inline
void orwl_handle_cancel_destroy(orwl_handle_cancel* H) {
  orwl_handle_destroy(H);
}

P99_INSTANTIATE(void, orwl_handle_cancel_destroy, orwl_handle_cancel*);

DECLARE_NEW_DELETE(orwl_handle_cancel);
DEFINE_NEW_DELETE(orwl_handle_cancel);

inline
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
  orwl_handle_cancel_create(rhcp, P99_0(pthread_t*));
  orwl_handle_destroy(rh);
  return state;
}

P99_INSTANTIATE(uint64_t*, orwl_map, orwl_handle*, size_t*);
P99_INSTANTIATE(uint64_t const*, orwl_mapro, orwl_handle*, size_t*);
P99_INSTANTIATE(void, orwl_resize, orwl_handle*, size_t);

P99_INSTANTIATE(void*, orwl_write_map, orwl_handle*, size_t*);
P99_INSTANTIATE(void const*, orwl_read_map, orwl_handle*, size_t*);
P99_INSTANTIATE(void, orwl_truncate, orwl_handle*, size_t);
P99_INSTANTIATE(void, orwl_scale, orwl_mirror*, size_t);

DEFINE_ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_release, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle);


DEFINE_ORWL_TYPE_DYNAMIC(orwl_handle,
                         ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle),
                         ORWL_REGISTER_ALIAS(orwl_release, orwl_handle),
                         ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle)
                         );
