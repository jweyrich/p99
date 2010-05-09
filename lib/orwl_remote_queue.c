/*
** orwl_remote_queue.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Apr  6 15:30:01 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdio.h>
#include "orwl_remote_queue.h"
#include "orwl_server.h"

orwl_rq *orwl_rq_init(orwl_rq *rq, orwl_endpoint h, orwl_endpoint t);
DEFINE_DEFARG(orwl_rq_init, , (orwl_endpoint){{0}}, (orwl_endpoint){{0}});

void orwl_rq_destroy(orwl_rq *rq);

DEFINE_NEW_DELETE(orwl_rq);

orwl_rh *orwl_rh_init(orwl_rh *rh);
void orwl_rh_destroy(orwl_rh *rh);

DEFINE_NEW_DELETE(orwl_rh);

orwl_state orwl_acquire(orwl_rh* rh);
orwl_state orwl_test(orwl_rh* rh);

orwl_state orwl_request_excl(orwl_rq *rq, orwl_rh* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
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
        rh->svrID = orwl_rpc(&rq->there, seed, auth_sock_request_excl,
                             rq->there.index,
                             (uintptr_t)cli_wh,
                             port2host(&rq->here.port)
                             );
        if (rh->svrID) {
          /* Link us to rq */
          rh->rq = rq;
        } else {
          report(1, "bad things happen: unable to get an insertion request from the other side.");
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

orwl_state orwl_request_incl(orwl_rq *rq, orwl_rh* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
  MUTUAL_EXCLUDE(rq->mut) {
    if (!rh->wh) {
      orwl_wh* cli_wh = NEW(orwl_wh);
      orwl_wh* wh_inc = NULL;
      /* first try to piggyback the latest wh in the local list */
      state = orwl_wq_request(&rq->local, &wh_inc, 1);
      /* the dummy handle is loaded with two tokens, one for the remote
         event of acquisition of the lock. The other is used here locally
         to ensure that cli_wh is not freed before we have finished our
         work, here. */
      state = orwl_wq_request(&rq->local, &cli_wh, 2);
      /* Send the insertion request with the two ids of the handles to
         the other side. As result retrieve the ID on the other side
         that is to be released when we release here. */
      rh->svrID = orwl_rpc(&rq->there, seed, auth_sock_request_incl,
                           rq->there.index,
                           (uintptr_t)cli_wh,
                           wh_inc ? wh_inc->svrID : 0,
                           port2host(&rq->here.port)
                           );
      if (!rh->svrID) {
        state = orwl_invalid;
        report(1, "bad things happen: unable to get an insertion request from the other side.");
        orwl_wh_acquire(cli_wh, 2);
        orwl_wh_release(cli_wh);
        orwl_wh_delete(cli_wh);
      } else {
        /* Link us to rq */
        rh->rq = rq;
        if (state == orwl_requested
            && wh_inc
            && wh_inc->svrID == rh->svrID) {
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
          assert(&rq->local == cli_wh->location);
          MUTUAL_EXCLUDE(rq->local.mut) {
            assert(rq->local.tail == cli_wh);
            // The handle on the remote side might have been acquired
            // before we even woke up. In that case we may already have
            // received the information and have released and deleted
            // cli_wh.
            orwl_wq_request_locked(&rq->local, wh, 1);
            // Finally have rh point on wh and mark wh as being
            // inclusive.
            wh->svrID = rh->svrID;
            rh->wh = wh;
            orwl_wh_unload(cli_wh, 1);
          }
        }
      }
    }
  }
  return state;
}

orwl_state orwl_release(orwl_rh* rh, rand48_t *seed) {
  orwl_state state = orwl_valid;
  assert(rh);
  assert(rh->wh);
  /* To be able to re-initialize rh in the middle of the procedure,
     tore all necessary data on the stack.*/
  orwl_wh* wh = rh->wh;
  orwl_rq* rq = rh->rq;
  uint64_t svrID = rh->svrID;
  orwl_endpoint there = rq->there;
  orwl_wq* wq = wh->location;
  /* Detect if we are the last user of this handle */
  uint64_t remain = 0;
  pthread_mutex_lock(&rq->mut);
  MUTUAL_EXCLUDE(wq->mut) {
    assert(wq == &rq->local);
    assert(wq->head == wh);
    remain = wh->tokens;
    if (remain != 1) {
      orwl_rh_init(rh);
      pthread_mutex_unlock(&rq->mut);
    }
    orwl_wh_unload(wh);
  }
  if (remain == 1) {
    state = orwl_wh_release(wh);
    orwl_rh_init(rh);
    pthread_mutex_unlock(&rq->mut);
    orwl_rpc(&there, seed, auth_sock_release, svrID);
    /* We should be the last to have a reference to this handle so
       we may destroy it. */
    orwl_wh_delete(wh);
  }
  return state;
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_request_excl, uint64_t wqPOS, uint64_t whID, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_request_excl, uint64_t wqPOS, uint64_t whID, uint64_t port);
  Arg->ret = 0;
  if (wqPOS < Arg->srv->max_queues) {
    /* extract wq and the remote wh ID */
    orwl_wq *srv_wq = &Arg->srv->wqs[wqPOS];
    /* Create a handle and insert it in the queue.  Request two tokens,
       one for this function here when it acquires below, the other one to
       block until the remote issues a release. */
    orwl_wh *srv_wh = NEW(orwl_wh);
    orwl_state state = orwl_wq_request(srv_wq, &srv_wh, 2);
    if (state == orwl_requested) {
      /* mes is already in host order */
      orwl_endpoint ep = { .addr = getpeer(Arg), .port = host2port(port) };
      /* Acknowledge the creation of the wh and send back its id. */
      Arg->ret = (uintptr_t)srv_wh;
      auth_sock_close(Arg);
      /* Wait until the lock on wh is obtained. */
      state = orwl_wh_acquire(srv_wh);
      /* Send a request to the other side to remove the remote wh ID. */
      orwl_rpc(&ep, seed_get(), auth_sock_release, whID);
    } else {
      orwl_wh_delete(srv_wh);
    }
  }
}


DEFINE_AUTH_SOCK_FUNC(auth_sock_request_incl, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port) {
  orwl_state state = orwl_invalid;
  /* Extract wq and the remote handle IDs from Arg */
  AUTH_SOCK_READ(Arg, auth_sock_request_incl, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port);
  Arg->ret = 0;
  if (wqPOS < Arg->srv->max_queues) {
    /* extract wq and the remote wh ID */
    orwl_wq *srv_wq = &Arg->srv->wqs[wqPOS];
    report(1, "inclusive request 0x%jx 0x%jx", (uintmax_t)svrID, (uintmax_t)cliID);
    /* First check if a previously inserted inclusive handle can be
       re-used. Always request two tokens, one for this function here
       when it acquires below, the other one to block until the remote
       issues a release */
    bool piggyback = false;
    orwl_wh *srv_wh = NULL;
    if (svrID) {
      report(1, "inclusive request (%p) 0x%jx 0x%jx", srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
      state = orwl_wq_request(srv_wq, &srv_wh, 2);
      piggyback = (svrID == (uintptr_t)srv_wh);
    }
    if (!piggyback) {
      srv_wh = NEW(orwl_wh);
      /* mark it as being inclusive */
      srv_wh->svrID = (uintptr_t)srv_wh;
      report(1, "inclusive request (%p) 0x%jx 0x%jx", srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
      state = orwl_wq_request(srv_wq, &srv_wh, 2);
    }
    if (state != orwl_requested) {
      if (!piggyback) orwl_wh_delete(srv_wh);
      auth_sock_close(Arg);
    } else {
      orwl_endpoint ep = { .addr = getpeer(Arg), .port = host2port(port) };
      /* Acknowledge the creation of the wh and send back its ID. */
      Arg->ret = (uintptr_t)srv_wh;
      auth_sock_close(Arg);
      /* If now the local handle is `requested' we only have to wait if
         we establish a new pair of client-server handles. */
      if (svrID == (uintptr_t)srv_wh) {
        assert(piggyback);
        report(1, "unloading server handle %p for existing pair", srv_wh);
        MUTUAL_EXCLUDE(srv_wq->mut) {
          orwl_wh_unload(srv_wh, 2);
        }
      } else {
        report(1, "waiting to acquire server handle %p 0x%jx (0x%jX)", srv_wh, cliID, (uintmax_t)svrID);
        // wait until the lock on wh is obtained
        state = orwl_wh_acquire(srv_wh);
        report(1, "acquired server handle %p (0x%jX)", srv_wh, (uintmax_t)svrID);
        // send a request to the other side to remove the remote wh ID
        orwl_rpc(&ep, seed_get(), auth_sock_release, cliID);
      }
    }
  }
}

/* this is executed first on the client when the lock is acquired and */
/* then on the server when the lock is released. */
DEFINE_AUTH_SOCK_FUNC(auth_sock_release, uintptr_t whID) {
  AUTH_SOCK_READ(Arg, auth_sock_release, uintptr_t whID);
  // extract the wh for Arg
  assert(whID);
  orwl_wh* wh = (void*)whID;
  orwl_wq* wq = wh->location;
  assert(wq);
  uint64_t remain = 0;
  MUTUAL_EXCLUDE(wq->mut) {
    remain = wh->tokens;
    assert(remain);
    orwl_wh_unload(wh);
  }
  // delete wh and return
  orwl_state state = orwl_valid;
  if (remain == 1) {
    state = orwl_wh_release(wh);
    orwl_wh_delete(wh);
  }
  Arg->ret = state;
}


DEFINE_DEFARG(orwl_request_excl, , , seed_get());
DEFINE_DEFARG(orwl_request_incl, , , seed_get());
DEFINE_DEFARG(orwl_release, , seed_get());
