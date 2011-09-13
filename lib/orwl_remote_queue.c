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
#include "orwl_remote_queue.h"
#include "orwl_socket.h"
#include "orwl_server.h"
#include "orwl_proc_symbols.h"

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
    if (rq && rh && !rh->wh)
      MUTUAL_EXCLUDE(rq->mut) {

      // insert two wh in the local queue
      orwl_wh* wh = P99_NEW(orwl_wh);
      orwl_wh* cli_wh = P99_NEW(orwl_wh);
      state = orwl_wq_request2(&rq->local, cli_wh, 1, wh, 1);
      assert(state == orwl_requested);
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
      assert(rh->svrID && (rh->svrID != ORWL_SEND_ERROR));

      /* Link us to the queue and the handle */
      rh->wh = wh;
      rh->rq = rq;
    }
  }
  return state;
}

orwl_state orwl_read_request(orwl_mirror *rq, orwl_handle* rh, rand48_t *seed) {
  orwl_state state = orwl_invalid;
  ORWL_TIMER(total_read_request) {
    orwl_wh* cli_wh = 0;
    orwl_wh* wh_inc = 0;
    if (rq && rh && !rh->wh)
      MUTUAL_EXCLUDE(rq->mut) {
      cli_wh = P99_NEW(orwl_wh);
      MUTUAL_EXCLUDE(rq->local.mut) {
        /* first try to piggyback the latest wh in the local list */
        state = orwl_wq_request_locked(&rq->local, &wh_inc, 1);
        assert(!wh_inc || wh_inc->svrID);
        assert(!wh_inc || (state == orwl_requested));

        /* Otherwise, the dummy handle is loaded with two tokens, one
           for the remote event of acquisition of the lock. The other is
           used here locally to ensure that cli_wh is not freed before
           we have finished our work, here. */
        state = orwl_wq_request_locked(&rq->local, &cli_wh, 2);
        assert(state == orwl_requested);

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
        assert(rh->svrID);
        assert(&rq->local == cli_wh->location);
        assert(rq->local.tail == cli_wh);


        /* Link us to rq */
        rh->rq = rq;
        if (wh_inc && wh_inc->svrID == rh->svrID) {
          /* remote and local queue have still the same last element, a
             corresponding inclusive pair at the tail of the their
             list. */
          rh->wh = wh_inc;

          /* Clean up the queue by removing the dummy handle that we
             inserted. */
          assert(wh_inc->location == &rq->local);
          assert(wh_inc->next == cli_wh);

          wh_inc->next = 0;
          rq->local.tail = wh_inc;
          wh_inc = 0;

          cli_wh->location = 0;
          orwl_wh_delete(cli_wh);
          cli_wh = 0;
        } else {
          // A new handle has to be inserted in the local queue
          rh->wh = P99_NEW(orwl_wh);
          rh->wh->svrID = rh->svrID;
          orwl_wq_request_append(&rq->local, rh->wh, 1);
        }
      }
    }

    // If we added a token to an existing handle but now we
    // see that this corresponds to a different priority
    // (because remotely there was something in between) we
    // have to unload this again.
    if (wh_inc && !orwl_wh_unload(wh_inc, 1)) {
      assert(wh_inc);
      orwl_wh_release(wh_inc);
      orwl_wh_delete(wh_inc);
    }

    // The handle on the remote side might have been acquired
    // before we even woke up. In that case we may already have
    // received the information and will have to release and delete
    // cli_wh.
    if (cli_wh && !orwl_wh_unload(cli_wh, 1)) {
      orwl_wh_release(cli_wh);
      orwl_wh_delete(cli_wh);
    }
    state = orwl_requested;
  }
  return state;
}

void orwl_push(orwl_server *srv, orwl_endpoint const*ep,
               orwl_wq *wq, uint64_t whID,
               bool withdata, bool keep) {
  /*
    Send a request to the other side
    - to remove the remote wh ID and to transfer the data, if any,
      or,
    - pass over a pointer of the data.
  */
  assert(wq);
  uint64_t buffer[orwl_push_header] = {
    [0] = ORWL_OBJID(orwl_proc_release),
    [1] = whID,
    [2] = ((withdata ? orwl_push_withdata : 0)
           |(keep ? orwl_push_keep : 0)),
  };
  enum { push_buffers = 2 };
  orwl_buffer mess[push_buffers] = {
    ORWL_BUFFER_INITIALIZER(orwl_push_header, buffer),
    ORWL_BUFFER_INITIALIZER(0, 0),
  };
  MUTUAL_EXCLUDE(wq->mut) {
    if (withdata) {
      mess[1].data = orwl_wq_map_locked(wq, &mess[1].len);
      buffer[3] = mess[1].len;
      if (mess[1].data) {
        /* first check if this will be remote */
        if(!srv || !orwl_endpoint_similar(&srv->host.ep, ep)) {
        } else if (!keep) {
          // Just delete the local trace of the buffer, not the buffer itself
          wq->data = P99_LVAL(orwl_buffer);
        }
      }
    }
    ORWL_TIMER(send_push_server)
    orwl_send(srv, ep, seed_get(), push_buffers, mess);
    if (!keep) orwl_wq_resize_locked(wq, 0);
  }
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
    bool withdata = !wh->svrID;

    /* Detect if we are the last user of this handle */
    if (!orwl_wh_unload(wh)) {
      orwl_wh_load(wh);
      ORWL_TIMER(push_release)
      orwl_push(srv, &there, wq, svrID, withdata, false);
      orwl_wh_unload(wh);

      /* We were the last to have a reference to this handle so we may
         destroy it. */
      state = orwl_wh_release(wh);
      orwl_wh_delete(wh);
    }
    orwl_handle_init(rh);
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
  orwl_handle_cancel* rhcp = P99_NEW(orwl_handle_cancel);
  *rhcp = *rh;
  orwl_handle_cancel_create_detached(rhcp);
  orwl_handle_destroy(rh);
  return state;
}

P99_INSTANTIATE(uint64_t*, orwl_map, orwl_handle*, size_t*);
P99_INSTANTIATE(uint64_t const*, orwl_mapro, orwl_handle*, size_t*);
P99_INSTANTIATE(void, orwl_resize, orwl_handle*, size_t);

P99_INSTANTIATE(void*, orwl_write_map, orwl_handle*, size_t*);
P99_INSTANTIATE(void const*, orwl_read_map, orwl_handle*, size_t*);
P99_INSTANTIATE(void, orwl_truncate, orwl_handle*, size_t);

P99_DEFINE_DEFARG(orwl_scale_state_init, , P99_0(orwl_mirror*), P99_0(size_t), P99_0(orwl_thread_cntrl*));
DEFINE_NEW_DELETE(orwl_scale_state);
P99_INSTANTIATE(orwl_scale_state*, orwl_scale_state_init, orwl_scale_state*, orwl_mirror*, size_t, orwl_thread_cntrl*);
P99_INSTANTIATE(void, orwl_scale_state_destroy, orwl_scale_state*);

DEFINE_THREAD(orwl_scale_state) {
  ORWL_TIMER(total_scale) {
    orwl_handle first = ORWL_HANDLE_INITIALIZER;
    orwl_write_request(Arg->rq, &first);
    orwl_thread_cntrl_freeze(Arg->det);
    orwl_thread_cntrl_wait_for_caller(Arg->det);
    orwl_acquire(&first);
    orwl_truncate(&first, Arg->data_len);
    orwl_release(&first);
  }
}

DEFINE_ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_release, orwl_handle);
DEFINE_ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle);


DEFINE_ORWL_TYPE_DYNAMIC(orwl_handle,
                         ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle),
                         ORWL_REGISTER_ALIAS(orwl_release, orwl_handle),
                         ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle)
                        );
