/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_proc_symbols.h"
#include "orwl_server.h"
#include "orwl_wait_queue.h"
#include "p99_id.h"
#include "orwl_timing.h"

DEFINE_ORWL_PROC_FUNC(orwl_proc_insert_peer, uint64_t port) {
  ORWL_PROC_READ(Arg, orwl_proc_insert_peer, uint64_t port);
  if (Arg->fd != -1) {
    orwl_host *h = P99_NEW(orwl_host);
    /* mes and orwl_addr is already in host order */
    h->ep.addr = orwl_proc_getpeer(Arg);
    h->ep.port.p = htons((uint16_t)port);
    orwl_host_connect(h, &Arg->srv->host);
  }
}

DEFINE_ORWL_PROC_FUNC(orwl_proc_insert_host, uint64_t addr, uint64_t port) {
  ORWL_PROC_READ(Arg, orwl_proc_insert_host, uint64_t addr, uint64_t port);
  orwl_host *h = P99_NEW(orwl_host);
  /* mes is already in host order */
  orwl_addr_init(&h->ep.addr, addr);
  orwl_port_init(&h->ep.port, port);
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_ORWL_PROC_FUNC(orwl_proc_do_nothing, void) {
  /* special care for bogus warning given by icc */
  (void)Arg;
}

/* Once the lock on the server side is achieved, acknowledge the
   requesting side and push the associated data */
static
orwl_state orwl_proc_push(orwl_server *srv, orwl_endpoint *ep,
                          orwl_wh *srv_wh, uint64_t whID,
                          bool withdata) {
  /* Wait until the lock on wh is obtained. */
  orwl_state state = orwl_wh_acquire(srv_wh);
  if (state == orwl_acquired) {
    uint64_t* mess = 0;
    size_t len = 2;
    /* Send a request to the other side to remove the remote wh ID
       and to transfer the data, if any. */
    orwl_wq* wq = srv_wh->location;
    assert(wq);
    MUTUAL_EXCLUDE(wq->mut) {
      ORWL_TIMER(copy_data_push_server) {
	size_t extend = (withdata ? wq->data_len : 0);
	len += extend;
	mess = uint64_t_vnew(len);
	mess[0] = ORWL_OBJID(orwl_proc_release);
	mess[1] = whID;
	if (extend) {
	  report(false, "adding suplement of length %zu", extend);
	  memcpy(&mess[2], wq->data, extend * sizeof(uint64_t));
	}
      }
    }
    ORWL_TIMER(send_push_server)
      orwl_send(srv, ep, seed_get(), len, mess);
    uint64_t_vdelete(mess);
  }
  return state;
}

DEFINE_ORWL_PROC_FUNC(orwl_proc_write_request, uint64_t wqPOS, uint64_t whID, uint64_t port) {
  ORWL_TIMER(total_write_request_server) {
    ORWL_PROC_READ(Arg, orwl_proc_write_request, uint64_t wqPOS, uint64_t whID, uint64_t port);
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
	orwl_endpoint ep = { .addr = orwl_proc_getpeer(Arg), .port = host2port(port) };
	/* Acknowledge the creation of the wh and send back its id. */
	Arg->ret = (uintptr_t)srv_wh;
	orwl_proc_untie_caller(Arg);
	/* Wait until the lock on wh is obtained. */
	ORWL_TIMER(push_write_request_server)
	  state = orwl_proc_push(Arg->srv, &ep, srv_wh, whID, true);
      } else {
	orwl_wh_delete(srv_wh);
      }
    }
  }
}

DEFINE_ORWL_PROC_FUNC(orwl_proc_read_request, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port) {
  ORWL_TIMER(total_read_request_server) {
    orwl_state state = orwl_invalid;
    /* Extract wq and the remote handle IDs from Arg */
    ORWL_PROC_READ(Arg, orwl_proc_read_request, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port);
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
      MUTUAL_EXCLUDE(srv_wq->mut) {
        state = orwl_wq_request_locked(srv_wq, &srv_wh, 2);
        if (srv_wh) {
          if (svrID) {
            piggyback = (svrID == (uintptr_t)srv_wh);
          }
        } else {
          srv_wh = P99_NEW(orwl_wh);
          /* mark it as being inclusive */
          srv_wh->svrID = (uintptr_t)srv_wh;
          report(0, "inclusive request (%p) 0x%jx 0x%jx", (void*)srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
          state = orwl_wq_request_locked(srv_wq, &srv_wh, 2);
        }
      }
      if (state != orwl_requested) {
	if (!piggyback) orwl_wh_delete(srv_wh);
      } else {
	orwl_endpoint ep = { .addr = orwl_proc_getpeer(Arg), .port = host2port(port) };
	/* Acknowledge the creation of the wh and send back its ID. */
	report(0, "inclusive request (%p) 0x%jx 0x%jx, detaching", (void*)srv_wh, (uintmax_t)svrID, (uintmax_t)cliID);
	Arg->ret = (uintptr_t)srv_wh;
	/* If now the local handle is `requested' we only have to wait if
	   we establish a new pair of client-server handles. */
	if (piggyback) {
	  report(0, "unloading server handle %p for existing pair", (void*)srv_wh);
	  bool last = false;
          last = !orwl_wh_unload(srv_wh, 2);
	  if (last) {
	    state = orwl_wh_release(srv_wh);
	    orwl_wh_delete(srv_wh);
	  }
	} else {
          orwl_proc_untie_caller(Arg);
	  // wait until the lock on wh is obtained
	  ORWL_TIMER(push_read_request_server)
	    state = orwl_proc_push(Arg->srv, &ep, srv_wh, cliID, true);
	}
      }
    }
  }
}

/* this is executed first on the client when the lock is acquired and */
/* then on the server when the lock is released. */
DEFINE_ORWL_PROC_FUNC(orwl_proc_release, uintptr_t whID) {
  ORWL_TIMER(total_release_server) {
    ORWL_PROC_READ(Arg, orwl_proc_release, uintptr_t whID);
    orwl_state ret = orwl_valid;
    // extract the wh for Arg
    assert(whID);
    orwl_wh* wh = (void*)whID;
    orwl_wq* wq = wh->location;
    assert(wq);
    bool last = false;
    MUTUAL_EXCLUDE(wq->mut) {
      last = !orwl_wh_unload(wh);
      if (!last) orwl_count_inc(&wh->finalists);
      size_t len = Arg->len;
      if (len) ORWL_TIMER(copy_data_release_server) {
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
    orwl_proc_untie_caller(Arg);
    if (Arg->det) {
      orwl_thread_cntrl_wait_for_caller(Arg->det);
      orwl_thread_cntrl_delete(Arg->det);
      Arg->det = 0;
    }
    if (last) {
      orwl_count_wait(&wh->finalists);
      ret = orwl_wh_release(wh);
      orwl_wh_delete(wh);
    } else {
      orwl_count_dec(&wh->finalists);
    }
    Arg->ret = ret;
  }
}

DEFINE_ORWL_PROC_FUNC(orwl_proc_check_initialization, uint64_t id) {
  ORWL_PROC_READ(Arg, orwl_proc_check_initialization, uint64_t id);
  bool finished = false;
  while (!finished) {
    pthread_rwlock_rdlock(&Arg->srv->lock);
    finished = Arg->srv->id_initialized[id];
    pthread_rwlock_unlock(&Arg->srv->lock);
    sleepfor(0.1);
  }
  //orwl_proc_untie_caller(Arg);
}

DEFINE_ORWL_TYPE_DYNAMIC(orwl_proc,
                         ORWL_REGISTER_ALIAS(orwl_proc_insert_peer, orwl_proc),
                         ORWL_REGISTER_ALIAS(orwl_proc_insert_host, orwl_proc),
                         ORWL_REGISTER_ALIAS(orwl_proc_do_nothing, orwl_proc),
                         ORWL_REGISTER_ALIAS(orwl_proc_write_request, orwl_proc),
                         ORWL_REGISTER_ALIAS(orwl_proc_read_request, orwl_proc),
                         ORWL_REGISTER_ALIAS(orwl_proc_release, orwl_proc),
			 ORWL_REGISTER_ALIAS(orwl_proc_check_initialization, orwl_proc)
                         );

