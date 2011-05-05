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
#include "orwl_auth_sock_functions.h"
#include "orwl_server.h"
#include "orwl_wait_queue.h"
#include "p99_id.h"

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_peer, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_peer, uint64_t port);
  orwl_host *h = P99_NEW(orwl_host);
  /* mes and addr_t is already in host order */
  h->ep.addr = getpeer(Arg);
  h->ep.port.p = htons((uint16_t)port);
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_host, uint64_t addr, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_host, uint64_t addr, uint64_t port);
  orwl_host *h = P99_NEW(orwl_host);
  /* mes is already in host order */
  addr_t_init(&h->ep.addr, addr);
  port_t_init(&h->ep.port, port);
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_do_nothing, void) {
  /* special care for bogus warning given by icc */
  (void)Arg;
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
    orwl_wh *srv_wh = P99_NEW(orwl_wh);
    orwl_state state = orwl_wq_request(srv_wq, &srv_wh, 2);
    if (state == orwl_requested) {
      /* mes is already in host order */
      orwl_endpoint ep = { .addr = getpeer(Arg), .port = host2port(port) };
      /* Acknowledge the creation of the wh and send back its id. */
      Arg->ret = (uintptr_t)srv_wh;
      auth_sock_close(Arg);
      /* Wait until the lock on wh is obtained. */
      state = orwl_wh_acquire(srv_wh);
      /* Send a request to the other side to remove the remote wh ID
         and to transfer the data, if any. */
      orwl_wq* wq = srv_wh->location;
      assert(wq);
      MUTUAL_EXCLUDE(wq->mut) {
        size_t extend = wq->data_len;
        size_t len = 2 + extend;
        uint64_t* mess = uint64_t_vnew(len);
        mess[0] = ORWL_OBJID(auth_sock_release);
        mess[1] = whID;
        if (extend) {
          report(false, "adding suplement of length %zu", extend);
          memcpy(&mess[2], wq->data, extend * sizeof(uint64_t));
        }
        orwl_send(&ep, seed_get(), len, mess);
        uint64_t_vdelete(mess);
      }
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
    report(0, "inclusive request 0x%jx 0x%jx", (uintmax_t)svrID, (uintmax_t)cliID);
    /* First check if a previously inserted inclusive handle can be
       re-used. Always request two tokens, one for this function here
       when it acquires below, the other one to block until the remote
       issues a release */
    bool piggyback = false;
    orwl_wh *srv_wh = 0;
    report(0, "inclusive request (%p) 0x%jx 0x%jx", (void*)srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
    state = orwl_wq_request(srv_wq, &srv_wh, 2);
    if (srv_wh) {
      if (svrID) {
        piggyback = (svrID == (uintptr_t)srv_wh);
      }
    } else {
      srv_wh = P99_NEW(orwl_wh);
      /* mark it as being inclusive */
      srv_wh->svrID = (uintptr_t)srv_wh;
      report(0, "inclusive request (%p) 0x%jx 0x%jx", (void*)srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
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
        report(0, "unloading server handle %p for existing pair", (void*)srv_wh);
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
        report(0, "waiting to acquire server handle %p 0x%jx (0x%jX)", (void*)srv_wh, (uintmax_t)cliID, (uintmax_t)svrID);
        // wait until the lock on wh is obtained
        state = orwl_wh_acquire(srv_wh);
        report(0, "acquired server handle %p (0x%jX)", (void*)srv_wh, (uintmax_t)svrID);
        /* Send a request to the other side to remove the remote wh ID
           and to transfer the data, if any. */
        orwl_wq* wq = srv_wh->location;
        assert(wq);
        MUTUAL_EXCLUDE(wq->mut) {
          size_t extend = wq->data_len;
          size_t len = 2 + extend;
          uint64_t* mess = uint64_t_vnew(len);
          mess[0] = ORWL_OBJID(auth_sock_release);
          mess[1] = cliID;
          if (extend) {
            report(false, "adding suplement of length %zu", extend);
            memcpy(&mess[2], wq->data, extend * sizeof(uint64_t));
          }
          orwl_send(&ep, seed_get(), len, mess);
          uint64_t_vdelete(mess);
        }
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
    size_t len = Arg->len;
    if (len) {
      report(false, "found suplementary message of length %zu", len);
      Arg->len = 0;
      size_t data_len;
      uint64_t* data = orwl_wq_map_locked(wq, &data_len);
      if (data_len != len) {
        orwl_wq_resize_locked(wq, len);
        data = orwl_wq_map_locked(wq, &data_len);
      }
      memcpy(data, Arg->mes, len * sizeof(uint64_t));
      report(false, "copied suplementary message of length %zu", len);
    }
  }
  if (last) {
    ret = orwl_wh_release(wh);
    orwl_wh_delete(wh);
  }
  Arg->ret = ret;
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_check_initialization, uint64_t id) {
  AUTH_SOCK_READ(Arg, auth_sock_check_initialization, uint64_t id);
  bool finished = false;
  while (!finished) {
    pthread_rwlock_rdlock(&Arg->srv->lock);
    finished = Arg->srv->id_initialized[id];
    pthread_rwlock_unlock(&Arg->srv->lock);
    sleepfor(0.1);
  }
  auth_sock_close(Arg);
}

DEFINE_ORWL_TYPE_DYNAMIC(auth_sock,
                         ORWL_REGISTER_ALIAS(auth_sock_insert_peer, auth_sock),
                         ORWL_REGISTER_ALIAS(auth_sock_insert_host, auth_sock),
                         ORWL_REGISTER_ALIAS(auth_sock_do_nothing, auth_sock),
                         ORWL_REGISTER_ALIAS(auth_sock_write_request, auth_sock),
                         ORWL_REGISTER_ALIAS(auth_sock_read_request, auth_sock),
                         ORWL_REGISTER_ALIAS(auth_sock_release, auth_sock),
			 ORWL_REGISTER_ALIAS(auth_sock_check_initialization, auth_sock)
                         );

