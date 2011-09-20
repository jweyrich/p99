/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/* all rights reserved,  2011 Matias E. Vara, INRIA, France                  */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_wait_queue.h"
#include "orwl_socket.h"
#include "orwl_atomic.h"
#include "orwl_timing.h"

P99_DEFINE_ENUM(orwl_state);

DEFINE_ONCE(orwl_wq, orwl_thread) {
}

orwl_wq* orwl_wq_init(orwl_wq *wq,
                      const pthread_mutexattr_t *attr) {
  if (!wq) return 0;
  *wq = (orwl_wq const)ORWL_WQ_INITIALIZER;
  pthread_mutex_init(&wq->mut, attr);
  pthread_cond_init(&wq->cond, 0);
  return wq;
}

#define ORWL__WQ_CHECK(EL)                                     \
do {                                                           \
  assert((EL) != TGARB(orwl_wh*));                             \
  assert(!(EL));                                               \
 } while(0)

/**
   @todo Ensure that wq->head and wq->tail are empty when   
            destroying a wq  
**/
void orwl_wq_destroy(orwl_wq *wq) {
  //ORWL__WQ_CHECK(wq->head);
  //ORWL__WQ_CHECK(wq->tail);
  if (wq->data.data) wq->data.data = wq->borrowed ? 0 : realloc(wq->data.data, 0);
  pthread_mutex_destroy(&wq->mut);
  pthread_cond_destroy(&wq->cond);
  *wq = P99_LVAL(orwl_wq const,
                 .head = TGARB(orwl_wh*),
                 .tail = TGARB(orwl_wh*),
  .data = {
    .len = P99_TMAX(size_t),
    .data = TGARB(void*)
  },
  .clock = P99_TMAX(uint64_t),
                );
}

DEFINE_NEW_DELETE(orwl_wq);

DEFINE_ONCE(orwl_wh,
            orwl_thread) {
}

DEFINE_ATOMIC_OPS(orwl_wh_ptr);


orwl_wh* orwl_wh_init(orwl_wh *wh,
                      size_t tok) {
  if (!wh) return 0;
  *wh =  (orwl_wh const)ORWL_WH_INITIALIZER;
  orwl_count_init(&wh->tokens, tok);
  orwl_notifier_init(&wh->acq);
  return wh;
}

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  orwl_count_destroy(&wh->tokens);
  orwl_notifier_destroy(&wh->acq);
  *wh = P99_LVAL(orwl_wh const,
                 .location = TGARB(orwl_wq*),
                 .next = TGARB(orwl_wh*),
                 .priority = P99_TMAX(int64_t),
                );
}

DEFINE_NEW_DELETE(orwl_wh);

P99_INSTANTIATE(int, orwl_wh_valid, orwl_wh*);
P99_INSTANTIATE(int, orwl_wh_idle, orwl_wh*);
/* This supposes that wq != 0 */
P99_INSTANTIATE(int, orwl_wq_valid, orwl_wq*);
/* This supposes that wq != 0 */
P99_INSTANTIATE(int, orwl_wq_idle, orwl_wq*);

/* This supposes that the corresponding wq != 0 */
P99_INSTANTIATE(uint64_t, orwl_wh_load_conditionally, orwl_wh *wh, uint64_t howmuch);
/* This supposes that the corresponding wq != 0 */
P99_INSTANTIATE(uint64_t, orwl_wh_unload, orwl_wh *wh, uint64_t howmuch);

void orwl_wq_request_append(orwl_wq *wq, orwl_wh *wh) {
  wh->location = wq;
  wh->priority = wq->clock;
  if (orwl_wq_idle(wq)) {
    wq->head = wh;
    wq->tail = wh;
    assert(!orwl_notifier_verify(&wh->acq));
    orwl_notifier_set(&wh->acq);
  } else {
    orwl_wh *wq_tail = wq->tail;
    wq_tail->next = wh;
    wq->tail = wh;
  }
  ++wq->clock;
}

orwl_state orwl_wq_request(orwl_wq *wq, orwl_wh *wh) {
  orwl_state ret = orwl_invalid;
  if (wq && wh)
    MUTUAL_EXCLUDE(wq->mut) {
    ret = orwl_wq_request_locked(wq, wh);
  }
  return ret;
}

orwl_state orwl_wq_request_locked(orwl_wq *wq, orwl_wh *wh) {
  orwl_state ret = orwl_invalid;
  if (orwl_wq_valid(wq) && wh) {
    orwl_wq_request_append(wq, wh);
    ret = orwl_requested;
  }
  return ret;
}

orwl_state orwl_wq_try_request(orwl_wq *wq, orwl_wh **wh, uint64_t hm) {
  orwl_state ret = orwl_invalid;
  if (orwl_wq_valid(wq) && wh) {
    uint64_t howmuch = (hm > P99_0(int64_t)) ? hm : -hm;
    assert(!*wh);
    if (!orwl_wq_idle(wq)) {
      /* if the wh is a null pointer, take this as a request to add to the
         last handle if it exists */
      orwl_wh *wq_tail = wq->tail;
      if (wq_tail
          // detect if this last element is a read handle
          && wq_tail->svrID) {
        assert(hm >= P99_0(int64_t));
        // Detect if it is still in use or just being released.
        // Because we hold the lock we can be the only one that is
        // issuing a request, but another one could just be releasing.
        if (orwl_wh_load_conditionally(wq_tail, howmuch)) {
          *wh = wq_tail;
          ret = orwl_requested;
        } else {
          // remove this message, once this code stabilizes
          report(1, "wait handle %p in the middle of release, please try again", (void*)wq_tail);
          ret = orwl_again;
        }
      }
    }
  }
  return ret;
}

orwl_state orwl_wq_request2(orwl_wq *wq,
                            orwl_wh *wh0,
                            orwl_wh *wh1) {
  orwl_state ret = orwl_invalid;
  if (wq)
    MUTUAL_EXCLUDE(wq->mut) {
    if (orwl_wq_valid(wq)) {
      ret = orwl_requested;
      assert(wh0);
      orwl_wq_request_append(wq, wh0);
      assert(wh1);
      orwl_wq_request_append(wq, wh1);
    }
  }
  return ret;
}

orwl_state orwl_wh_acquire(orwl_wh *wh, uint64_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_notifier_block(&wh->acq);
    ret = orwl_acquired;
    orwl_wh_unload(wh, howmuch);
  }
  return ret;
}

orwl_state orwl_wh_test(orwl_wh *wh, uint64_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    ret = (orwl_notifier_verify(&wh->acq)) ? orwl_acquired : orwl_requested;
    orwl_wh_unload(wh, howmuch);
  }
  return ret;
}

orwl_state orwl_wh_release(orwl_wh *wh) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq && orwl_wq_valid(wq)) {
      ORWL_TIMER(release_wh_acquire)
      ret = orwl_wh_acquire(wh, 0);
      switch (ret) {
      default:
        report(true, "Inconsistency in queue %p when releasing %p, state is %s",
               (void*)wq, (void*)wh, orwl_state_getname(ret));
      case orwl_valid:
        break;
      case orwl_acquired:
        orwl_count_wait(&wh->tokens);
        MUTUAL_EXCLUDE(wq->mut) {
          orwl_wh  *wh_next = wh->next;
          wh->location = 0;
          wh->next = 0;
          if (!wh_next) {
            assert(wq->tail == wh);
            wq->head = 0;
            wq->tail = 0;
          } else {
            assert(!orwl_notifier_verify(&wh_next->acq));
            wq->head = wh_next;
            /* Unlock potential acquirers */
            orwl_notifier_set(&wh_next->acq);
          }
          ret = orwl_valid;
        }
      }
    }
  }
  return ret;
}

uint64_t* orwl_wq_map_locked(orwl_wq* wq, size_t* data_len) {
  if (data_len) *data_len = wq->data.len;
  return wq->data.len ? wq->data.data : 0;
}

void orwl_wq_link(orwl_wq *wq,       /*!< the locked queue to act on */
                  orwl_buffer data,    /*!< data buffer that is provided
                                      from elsewhere */
                  bool borrowed      /*!< whether this location here
                                       is responsible for the data */
                 ) {
  orwl_wq_resize_locked(wq, 0);
  wq->data = data;
  wq->borrowed = borrowed;
}

uint64_t* orwl_wh_map(orwl_wh* wh, size_t* data_len) {
  uint64_t* ret = 0;
  orwl_state state;
  ORWL_TIMER(map_wh_acquire)
  state = orwl_wh_acquire(wh, 0);
  if (state == orwl_acquired) {
    orwl_wq *wq = wh->location;
    assert(wq);
    MUTUAL_EXCLUDE(wq->mut)
    ret = orwl_wq_map_locked(wq, data_len);
  } else {
    if (data_len) *data_len = 0;
  }
  return ret;
}

void orwl_wq_resize_locked(orwl_wq* wq, size_t len) {
  if (wq->borrowed) {
    wq->data = P99_LVAL(orwl_buffer);
    wq->borrowed = false;
  }
  if (P99_UNLIKELY((!len && !wq->data.len) || (len == wq->data.len))) return;
  size_t const blen      =          len * sizeof(wq->data.data[0]);
  size_t const data_blen = wq->data.len * sizeof(wq->data.data[0]);
  /* zero out memory that is returned to the system */
  if (data_blen > blen)
    memset(&wq->data.data[len], 0, data_blen - blen);
  // Some systems may return a valid pointer even if the length is
  // 0. Keep track of it.
  orwl_buffer data = {
    .data = realloc(wq->data.data, blen),
    .len = len
  };
  /* zero out newly allocated memory */
  if (data_blen < blen) {
    if (P99_LIKELY(data.data))
      memset(&data.data[wq->data.len], 0, blen - data_blen);
    /* We wanted to enlarge the buffer but we ran out of virtual
       address space. There is not much that we can do about it: keep
       the data that we still possess, cross fingers and return. */
    else return;
  }
  if (P99_LIKELY(data.data || !len))
    wq->data = data;
  else {
    /* There is just one case that may bring us here and no decent
       implementation of realloc should ever do that, but who
       knows. We wanted to shrink the buffer, and realloc failed
       instead of just returning the original pointer. So we do just
       that, keep the pointer and adjust the length. We already
       destroyed the data that we have beyond the new bound,
       anyhow. */
    assert(data_blen > blen);
    wq->data.len = len;
  }
}

void orwl_wh_resize(orwl_wh* wh, size_t len) {
  orwl_state state;
  ORWL_TIMER(resize_wh_acquire)
  state = orwl_wh_acquire(wh, 0);
  if (state == orwl_acquired) {
    orwl_wq *wq = wh->location;
    assert(wq);
    MUTUAL_EXCLUDE(wq->mut)
    orwl_wq_resize_locked(wq, len);
  }
}

P99_INSTANTIATE(void, orwl_state_destroy, orwl_state*);
P99_INSTANTIATE(orwl_state*, orwl_state_init, orwl_state *, orwl_state);

DEFINE_NEW_DELETE(orwl_state);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_state, {0});

DEFINE_ORWL_TYPE_DYNAMIC(orwl_wq, {0});

DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_acquire, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_release, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_load_conditionally, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_unload, orwl_wh);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_wh,
                         ORWL_REGISTER_ALIAS(orwl_wh_acquire, orwl_wh),
                         ORWL_REGISTER_ALIAS(orwl_wh_release, orwl_wh),
                         ORWL_REGISTER_ALIAS(orwl_wh_load_conditionally, orwl_wh),
                         ORWL_REGISTER_ALIAS(orwl_wh_unload, orwl_wh)
                        );
