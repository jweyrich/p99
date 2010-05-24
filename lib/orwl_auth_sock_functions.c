/*
** orwl_auth_sock_functions.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat May 15 23:23:34 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_auth_sock_functions.h"

#include "orwl_server.h"
#include "orwl_wait_queue.h"

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_peer, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_peer, uint64_t port);
  orwl_host *h = NEW(orwl_host);
  /* mes and addr_t is already in host order */
  h->ep.addr = getpeer(Arg);
  h->ep.port.p = htons(port);
  report(1, "inserting peer %s", orwl_endpoint_print(&h->ep));
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_host, uint64_t addr, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_host, uint64_t addr, uint64_t port);
  orwl_host *h = NEW(orwl_host);
  /* mes is already in host order */
  addr_t_init(&h->ep.addr, addr);
  port_t_init(&h->ep.port, port);
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_do_nothing, void) {
  /* empty */
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_write_request, uint64_t wqPOS, uint64_t whID, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_write_request, uint64_t wqPOS, uint64_t whID, uint64_t port);
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


DEFINE_AUTH_SOCK_FUNC(auth_sock_read_request, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port) {
  orwl_state state = orwl_invalid;
  /* Extract wq and the remote handle IDs from Arg */
  AUTH_SOCK_READ(Arg, auth_sock_read_request, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port);
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
    report(1, "inclusive request (%p) 0x%jx 0x%jx", (void*)srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
    state = orwl_wq_request(srv_wq, &srv_wh, 2);
    if (srv_wh) {
      if (svrID) {
        piggyback = (svrID == (uintptr_t)srv_wh);
      }
    } else {
      srv_wh = NEW(orwl_wh);
      /* mark it as being inclusive */
      srv_wh->svrID = (uintptr_t)srv_wh;
      report(1, "inclusive request (%p) 0x%jx 0x%jx", (void*)srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
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
      if (piggyback) {
        report(1, "unloading server handle %p for existing pair", (void*)srv_wh);
        bool last = false;
        MUTUAL_EXCLUDE(srv_wq->mut) {
          orwl_wh_unload(srv_wh, 2);
          last = (srv_wh->tokens == 0);
        }
        if (last) {
          state = orwl_wh_release(srv_wh);
          orwl_wh_delete(srv_wh);
        }
      } else {
        report(1, "waiting to acquire server handle %p 0x%jx (0x%jX)", (void*)srv_wh, cliID, (uintmax_t)svrID);
        // wait until the lock on wh is obtained
        state = orwl_wh_acquire(srv_wh);
        report(1, "acquired server handle %p (0x%jX)", (void*)srv_wh, (uintmax_t)svrID);
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
  orwl_state ret = orwl_valid;
  // extract the wh for Arg
  assert(whID);
  orwl_wh* wh = (void*)whID;
  orwl_wq* wq = wh->location;
  assert(wq);
  bool last = false;
  MUTUAL_EXCLUDE(wq->mut) {
    orwl_wh_unload(wh);
    last = (wh->tokens == 0);
  }
  if (last) {
    ret = orwl_wh_release(wh);
    orwl_wh_delete(wh);
  }
  Arg->ret = ret;
}

DEFINE_ORWL_TYPE_DYNAMIC(auth_sock,
                         ORWL_REGISTER_ALIAS(auth_sock_insert_peer),
                         ORWL_REGISTER_ALIAS(auth_sock_insert_host),
                         ORWL_REGISTER_ALIAS(auth_sock_do_nothing),
                         ORWL_REGISTER_ALIAS(auth_sock_write_request),
                         ORWL_REGISTER_ALIAS(auth_sock_read_request),
                         ORWL_REGISTER_ALIAS(auth_sock_release)
                         );

