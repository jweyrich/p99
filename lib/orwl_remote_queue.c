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

orwl_rq *orwl_rq_init(orwl_rq *rq, orwl_endpoint h, orwl_endpoint t, uint64_t id);
define_defarg(orwl_rq_init, 3, uint64_t);
define_defarg(orwl_rq_init, 2, orwl_endpoint);
define_defarg(orwl_rq_init, 1, orwl_endpoint);
void orwl_rq_destroy(orwl_rq *rq);

DEFINE_NEW_DELETE(orwl_rq);

orwl_rh *orwl_rh_init(orwl_rh *rh);
void orwl_rh_destroy(orwl_rh *rh);

DEFINE_NEW_DELETE(orwl_rh);

orwl_state orwl_acquire(orwl_rh* rh, size_t token);
define_defarg(orwl_acquire, 1, uintptr_t);


orwl_state orwl_test(orwl_rh* rh, size_t token);
define_defarg(orwl_test, 1, uintptr_t);

orwl_state orwl_request(orwl_rq *rq, orwl_rh* rh, size_t token, rand48_t *seed) {
  // insert two wh in the local queue
  assert(!rh->wh);
  rh->wh = NEW(orwl_wh);
  orwl_wh* cli_wh = NEW(orwl_wh);
  orwl_state state = orwl_invalid;
  MUTUAL_EXCLUDE(rq->mut) {
    state = orwl_wq_request(&rq->local, cli_wh, 1, rh->wh, token);
    if (state == orwl_requested) {
      assert(!rh->rq);
      /* Send the insertion request with the id of cli_wh to the other
         side. As result retrieve the ID on the other side that is to be
         released when we release here. */
      rh->rID = orwl_rpc(&rq->there, seed, auth_sock_request,
                         rq->ID,
                         (uintptr_t)cli_wh,
                         rq->here.port.p
                         );
      if (rh->rID) {
        /* Link us to rq */
        rh->rq = rq;
      } else {
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
  return state;
}

orwl_state orwl_release(orwl_rh* rh, rand48_t *seed) {
  orwl_state ret = orwl_wh_release(rh->wh);
  if (ret == orwl_valid) {
    orwl_rpc(&rh->rq->there, seed, auth_sock_release, rh->rID);
    orwl_wh_delete(rh->wh);
    orwl_rh_init(rh);
  }
  return ret;
}

void auth_sock_request(auth_sock *Arg) {
  // extract wq and the remote wh ID from Arg
  orwl_wq *srv_wq = (orwl_wq*)(void*)(uintptr_t)Arg->mes[1];
  // create a wh and insert it in wq
  orwl_wh *srv_wh = NEW(orwl_wh);
  // request two tokens, one for this function here when it acquires
  // below, the other one to block until the remote issues a release
  orwl_state state = orwl_wq_request(srv_wq, srv_wh, 2);
  if (state == orwl_requested) {
    // mes is already in host order
    orwl_endpoint ep = { .addr = getpeer(Arg), .port = { .p = Arg->mes[3] } };
    uint64_t id = Arg->mes[2];
    // acknowledge the creation of the wh and send back its id
    Arg->ret = (uintptr_t)srv_wh;
    auth_sock_close(Arg);
    // Unfortunately we don't know anybody who could borrow us some
    // randomness, here. So do this after the ack has been send to the
    // other side and we prepare for waiting a while, anyhow.
    rand48_t seed = RAND48_T_INITIALIZER;
    // wait until the lock on wh is obtained
    state = orwl_wh_acquire(srv_wh);
    // send a request to the other side to remove the remote wh ID
    orwl_rpc(&ep, &seed, auth_sock_release, id);
  } else {
    // tell other side about the error
    Arg->ret = 0;
    orwl_wh_delete(srv_wh);
  }
}

/* this is executed first on the client when the lock is acquired and */
/* then on the server when the lock is released. */
void auth_sock_release(auth_sock *Arg) {
  // extract the wh for Arg
  orwl_wh* wh = (void*)(uintptr_t)Arg->mes[1];
  // acquire and release wh
  // * in fact this must already been acquired when we come here,
  // * the acquire only serve to take out the last token.
  // * replace by a test!
  orwl_state state = orwl_wh_acquire(wh);
  if (state == orwl_acquired) {
    state = orwl_wh_release(wh);
  }
  // delete wh and return
  Arg->ret = state;
  orwl_wh_delete(wh);
}

