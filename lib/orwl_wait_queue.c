/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
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

void orwl_wq_destroy(orwl_wq *wq) {
  ORWL__WQ_CHECK(wq->head);
  ORWL__WQ_CHECK(wq->tail);
  if (wq->data) free(wq->data);
  pthread_mutex_destroy(&wq->mut);
  pthread_cond_destroy(&wq->cond);
  *wq = P99_LVAL(orwl_wq const,
                 .head = TGARB(orwl_wh*),
                 .tail = TGARB(orwl_wh*),
                 .data = TGARB(void*),
                 .clock = P99_TMAX(uint64_t),
                 .data_len = P99_TMAX(size_t),
                 );
}

DEFINE_NEW_DELETE(orwl_wq);

DEFINE_ONCE(orwl_wh,
            orwl_thread) {
}

DEFINE_ATOMIC_OPS(orwl_wh_ptr);


orwl_wh* orwl_wh_init(orwl_wh *wh,
                  const pthread_condattr_t *attr) {
  if (!wh) return 0;
  *wh =  (orwl_wh const)ORWL_WH_INITIALIZER;
  pthread_cond_init(&wh->cond, attr);
  pthread_mutex_init(&wh->mut, 0);
  return wh;
}

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  pthread_cond_destroy(&wh->cond);
  pthread_mutex_destroy(&wh->mut);
  *wh = P99_LVAL(orwl_wh const,
                 .location = TGARB(orwl_wq*),
                 .next = TGARB(orwl_wh*),
                 .priority = P99_TMAX(int64_t),
                 );
}

DEFINE_NEW_DELETE(orwl_wh);

int orwl_wh_valid(orwl_wh *wh);
int orwl_wh_idle(orwl_wh *wh);
/* This supposes that wq != 0 */
int orwl_wq_valid(orwl_wq *wq);
/* This supposes that wq != 0 */
int orwl_wq_idle(orwl_wq *wq);

/* This supposes that the corresponding wq != 0 */
P99_INSTANTIATE(uint64_t, orwl_wh_load, orwl_wh *wh, uint64_t howmuch);
/* This supposes that the corresponding wq != 0 */
P99_INSTANTIATE(uint64_t, orwl_wh_unload, orwl_wh *wh, uint64_t howmuch);

void orwl_wq_request_locked(orwl_wq *wq, orwl_wh *wh, uint64_t howmuch) {
  wh->location = wq;
  wh->priority = wq->clock;
  orwl_wh_load(wh, howmuch);
  if (orwl_wq_idle(wq)) {
    wq->head = wh;
    wq->tail = wh;
  } else {
    orwl_wh *wq_tail = wq->tail;
    MUTUAL_EXCLUDE(wq_tail->mut) {
      wq_tail->next = wh;
      wq->tail = wh;
    }
  }
  ++wq->clock;
}

orwl_state P99_FSYMB(orwl_wq_request)(orwl_wq *wq, P99_VA_ARGS(number)) {
  orwl_state ret = orwl_invalid;
  if (wq)
    MUTUAL_EXCLUDE(wq->mut)
      if (orwl_wq_valid(wq)) {
        /* Check (and wait eventually) that all wh are idle */
        for (bool idle = false; !idle; ) {
          va_list ap;
          va_start(ap, number);
          idle = true;
          for (size_t i = 0; i < number; ++i) {
            orwl_wh **wh = VA_MODARG(ap, orwl_wq_request, 0);
            (void)VA_MODARG(ap, orwl_wq_request, 1);
            if (*wh) {
              orwl_wh *whp = *wh;
              MUTUAL_EXCLUDE(whp->mut) {
                if (!orwl_wh_idle(whp)) {
                  idle = false;
                }
              }
              if (!idle) {
                report(true, "wait handle seems to be busy: %p", (void*)*wh);
                pthread_cond_wait(&wq->cond, &wq->mut);
                break;
              }
            }
          }
          va_end(ap);
        }
        ret = orwl_requested;
        /* Now insert them */
        va_list ap;
        va_start(ap, number);
        for (size_t i = 0; i < number; ++i) {
          orwl_wh **wh = VA_MODARG(ap, orwl_wq_request, 0);
          int64_t hm = VA_MODARG(ap, orwl_wq_request, 1);
          uint64_t howmuch = (hm > P99_0(int64_t)) ? hm : -hm;
          if (wh) {
            if (*wh) {
              orwl_wh *whp = *wh;
              MUTUAL_EXCLUDE(whp->mut)
                orwl_wq_request_locked(wq, whp, howmuch);
            } else {
              /* if the wh is a null pointer, take this as a request to add to the
                 last handle if it exists */
              orwl_wh *wq_tail = wq->tail;
              report(false, "request for augmenting an inclusive lock %p", (void*)wq_tail);
              if (number == 1
                  && !orwl_wq_idle(wq)
                  && wq_tail) {
                MUTUAL_EXCLUDE(wq_tail->mut) {
                  if (wq_tail->svrID) {
                    report(false, "request for augmenting an inclusive lock %p, succes", (void*)wq_tail);
                    assert(hm >= P99_0(int64_t));
                    orwl_wh_load(wq_tail, howmuch);
                    *wh = wq_tail;
                  } else {
                    report(false, "request for augmenting an inclusive lock %p (%jX), failed",
                           (void*)wq_tail, wq_tail ? wq_tail->svrID : P99_0(size_t));
                    ret = orwl_invalid;
                  }
                }
              } else {
                report(false, "request for augmenting an inclusive lock %p (%jX), failed",
                       (void*)wq_tail, wq_tail ? wq_tail->svrID : P99_0(size_t));
                ret = orwl_invalid;
                break;
              }
            }
          }
        }
        va_end(ap);
      }
  return ret;
}

orwl_state orwl_wh_acquire_locked(orwl_wh *wh, orwl_wq *wq) {
  orwl_state ret = orwl_invalid;
  orwl_wh* wq_head = atomic_load_orwl_wh_ptr(&wq->head);
  if (wq_head) {
    /* We are on the fast path */
    if (wq_head == wh) ret = orwl_acquired;
    /* We are on the slow path */
    else while(true) {
        orwl_wh_load(wh);
        /* We wait on the local orwl_wh mutex */
        ORWL_TIMER(wait_on_cond_acquire)
        pthread_cond_wait(&wh->cond, &wh->mut);
        orwl_wh_unload(wh);
        if  (wq != wh->location) break;
        /* Check everything again, somebody might have destroyed
           our wq */
        if (orwl_wq_valid(wq)) {
          if (atomic_load_orwl_wh_ptr(&wq->head) == wh) {
            ret = orwl_acquired;
            pthread_cond_broadcast(&wh->cond);
            break;
          }
        }
      }
  }
  return ret;
}

orwl_state orwl_wh_acquire(orwl_wh *wh, uint64_t howmuch) {
  orwl_state ret = orwl_invalid;
  /* using the mutex in orwl_wh */
  MUTUAL_EXCLUDE(wh->mut) {
    if (orwl_wh_valid(wh)) {
      orwl_wq *wq = wh->location;
      /* orwl_wq_valid uses atomic operations internally */
      if (wq && orwl_wq_valid(wq)) {
        ret = orwl_wh_acquire_locked(wh, wq);
        if (ret == orwl_acquired)
          orwl_wh_unload(wh, howmuch);
      }
    }
  }
  return ret;
}

orwl_state orwl_wh_test(orwl_wh *wh, uint64_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    orwl_wh* wq_head = atomic_load_orwl_wh_ptr(&wq->head);
    if (wq) MUTUAL_EXCLUDE(wh->mut) {
        /* orwl_wq_valid uses atomic operations internaly */
        if (orwl_wq_valid(wq) && wq_head)
        ret = (wq_head == wh) ? orwl_acquired : orwl_requested;
        if (ret == orwl_acquired)
          orwl_wh_unload(wh, howmuch);
      } else {
      if (!wh->next) ret = orwl_valid;
    }
  }
  return ret;
}

orwl_state orwl_wh_release(orwl_wh *wh) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq && orwl_wq_valid(wq)) {
      ret = orwl_wh_acquire(wh, 0);
      switch (ret) {
      default:
        report(true, "Inconsistency in queue %p when releasing %p, state is %s",
               (void*)wq, (void*)wh, orwl_state_getname(ret));
      case orwl_valid:
        break;
      case orwl_acquired:
        orwl_count_wait(&wh->tokens);
        MUTUAL_EXCLUDE(wq->mut)
          MUTUAL_EXCLUDE(wh->mut) {
          orwl_wh  *wh_next = wh->next;
          wh->location = 0;
          wh->next = 0;
          if (wh_next) pthread_mutex_lock(&wh_next->mut);
          else {
            if (wq->tail == wh) wq->tail = 0;
            else report(true, "Inconsistency in queue %p when releasing %p, different tail %p",
                        (void*)wq, (void*)wh, (void*)wq->tail);
          }
          wq->head = wh_next;
          /* Unlock potential requesters */
          pthread_cond_broadcast(&wq->cond);
          pthread_cond_broadcast(&wh->cond);
          /* Unlock potential acquirers */
          /* wake up the first one in the queue */
          if (wh_next) {
            pthread_cond_broadcast(&wh_next->cond);
            pthread_mutex_unlock(&wh_next->mut);
          }
          ret = orwl_valid;
        }
      }
    }
  }
  return ret;
}

uint64_t* orwl_wq_map_locked(orwl_wq* wq, size_t* data_len) {
  if (data_len) *data_len = wq->data_len;
  return wq->data;
}

uint64_t* orwl_wh_map(orwl_wh* wh, size_t* data_len) {
  uint64_t* ret = 0;
  if (orwl_wh_acquire(wh, 0) == orwl_acquired) {
    MUTUAL_EXCLUDE(wh->mut) {
      orwl_wq *wq = wh->location;
      assert(wq);
      MUTUAL_EXCLUDE(wq->mut)
        ret = orwl_wq_map_locked(wq, data_len);
    }
  } else {
    if (data_len) *data_len = 0;
  }
  return ret;
}

void orwl_wq_resize_locked(orwl_wq* wq, size_t len) {
  size_t data_len = wq->data_len;
  size_t const blen = len * sizeof(uint64_t);
  uint64_t* data = realloc(wq->data, blen);
  /* realloc is allowed to return something non-0 if len is
     0. Avoid that. */
  if (len) {
    if (P99_LIKELY(data)) {
      size_t const data_blen = data_len * sizeof(uint64_t);
      if (data_blen < blen)
        memset(&data[data_len], 0, blen - data_blen);
      wq->data = data;
    } else {
      /* realloc failed, don't do anything */
      report(true, "adding suplement of length %zu failed", len);
      return;
    }
  } else {
    wq->data = 0;
  }
  wq->data_len = len;
}

void orwl_wh_resize(orwl_wh* wh, size_t len) {
  if (orwl_wh_acquire(wh, 0) == orwl_acquired) {
    MUTUAL_EXCLUDE(wh->mut) {
      orwl_wq *wq = wh->location;
      assert(wq);
      MUTUAL_EXCLUDE(wq->mut)
        orwl_wq_resize_locked(wq, len);
    }
  }
}

P99_INSTANTIATE(void, orwl_state_destroy, orwl_state*);
P99_INSTANTIATE(orwl_state*, orwl_state_init, orwl_state *, orwl_state);

DEFINE_NEW_DELETE(orwl_state);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_state, {0});

DEFINE_ORWL_TYPE_DYNAMIC(orwl_wq, {0});

DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_acquire, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_release, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_load, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_unload, orwl_wh);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_wh,
                         ORWL_REGISTER_ALIAS(orwl_wh_acquire, orwl_wh),
                         ORWL_REGISTER_ALIAS(orwl_wh_release, orwl_wh),
                         ORWL_REGISTER_ALIAS(orwl_wh_load, orwl_wh),
                         ORWL_REGISTER_ALIAS(orwl_wh_unload, orwl_wh)
                         );
