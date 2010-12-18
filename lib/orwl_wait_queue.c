/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
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

DEFINE_ENUM(orwl_state);

DEFINE_ONCE(orwl_wq) {
  INIT_ONCE(orwl_thread);
}

orwl_wq* orwl_wq_init(orwl_wq *wq,
                                const pthread_mutexattr_t *attr) {
  INIT_ONCE(orwl_wq);
  if (!wq) return 0;
  *wq = (orwl_wq const)ORWL_WQ_INITIALIZER;
  if (attr) pthread_mutex_init(&wq->mut, attr);
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
  *wq = P99_LVAL(orwl_wq const,
                 .head = TGARB(orwl_wh*),
                 .tail = TGARB(orwl_wh*),
                 .data = TGARB(void*),
                 .clock = P99_TMAX(uint64_t),
                 .data_len = P99_TMAX(size_t),
                 );
}

DEFINE_NEW_DELETE(orwl_wq);

DEFINE_ONCE(orwl_wh) {
  INIT_ONCE(orwl_thread);
}

orwl_wh* orwl_wh_init(orwl_wh *wh,
                  const pthread_condattr_t *attr) {
  INIT_ONCE(orwl_wh);
  if (!wh) return 0;
  *wh = P99_LVAL(orwl_wh const,
                 .cond = PTHREAD_COND_INITIALIZER,
                 .location = 0,
                 .next = 0,
                 .tokens = 0,
                 .priority = 0,
                 .svrID = 0,
                 );
  if (attr) pthread_cond_init(&wh->cond, attr);
  return wh;
}

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  pthread_cond_destroy(&wh->cond);
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
uint64_t orwl_wh_load(orwl_wh *wh, uint64_t howmuch);
/* This supposes that the corresponding wq != 0 */
uint64_t orwl_wh_unload(orwl_wh *wh, uint64_t howmuch);

void orwl_wq_request_locked(orwl_wq *wq, orwl_wh *wh, uint64_t howmuch) {
  wh->location = wq;
  wh->priority = wq->clock;
  orwl_wh_load(wh, howmuch);
  if (orwl_wq_idle(wq)) wq->head = wh;
  else wq->tail->next = wh;
  wq->tail = wh;
  ++wq->clock;
}

orwl_state P99_FSYMB(orwl_wq_request)(orwl_wq *wq, P99_VA_ARGS(number)) {
  orwl_state ret = orwl_invalid;
  if (wq && orwl_wq_valid(wq)) {
    MUTUAL_EXCLUDE(wq->mut) {
      /* Check (and wait eventually) that all wh are idle */
      for (bool idle = false; !idle; ) {
        va_list ap;
        va_start(ap, number);
        idle = true;
        for (size_t i = 0; i < number; ++i) {
          orwl_wh **wh = VA_MODARG(ap, orwl_wq_request, 0);
          (void)VA_MODARG(ap, orwl_wq_request, 1);
          if (*wh && !orwl_wh_idle(*wh)) {
            idle = false;
            pthread_cond_wait(&(*wh)->cond, &wq->mut);
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
            orwl_wq_request_locked(wq, *wh, howmuch);
          } else {
            report(false, "request for augmenting an inclusive lock %p", (void*)wq->tail);
            if (number == 1
                && !orwl_wq_idle(wq)
                && wq->tail
                && wq->tail->svrID) {
              report(false, "request for augmenting an inclusive lock %p, succes", (void*)wq->tail);
              assert(hm >= P99_0(int64_t));
              /* if the wh is a null pointer, take this as a request to add to the
                 last handle if it exists */
              orwl_wh_load(wq->tail, howmuch);
              *wh = wq->tail;
            } else {
              report(false, "request for augmenting an inclusive lock %p (%jX), failed",
                     (void*)wq->tail, wq->tail->svrID);
              ret = orwl_invalid;
            }
          }
        }
      }
      va_end(ap);
    }
  }
  return ret;
}

orwl_state orwl_wh_acquire_locked(orwl_wh *wh, orwl_wq *wq) {
  orwl_state ret = orwl_invalid;
  if (orwl_wq_valid(wq) && wq->head) {
    /* We are on the fast path */
    if (wq->head == wh) ret = orwl_acquired;
    /* We are on the slow path */
    else {
      uint64_t loaded;
    RETRY:
      loaded = orwl_wh_load(wh, 1);
      pthread_cond_wait(&wh->cond, &wq->mut);
      orwl_wh_unload(wh, loaded);
      /* Check everything again, somebody might have destroyed
         our wq */
      if (orwl_wq_valid(wq)) {
        if (wq->head == wh)
          ret = orwl_acquired;
        else goto RETRY;
      }
    }
  }
  return ret;
}

orwl_state orwl_wh_acquire(orwl_wh *wh, uint64_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq) {
      MUTUAL_EXCLUDE(wq->mut) {
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
    if (wq) MUTUAL_EXCLUDE(wq->mut) {
        if (orwl_wq_valid(wq) && wq->head)
          ret = wq->head == wh ? orwl_acquired : orwl_requested;
        if (ret == orwl_acquired) orwl_wh_unload(wh, howmuch);
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
    if (wq) {
      MUTUAL_EXCLUDE(wq->mut) {
        while (orwl_wq_valid(wq)) {
          if (wq->head == wh && !wh->tokens) {
            wh->location = 0;
            if (!wh->next) wq->tail = 0;
            wq->head = wh->next;
            wh->next = 0;
            ret = orwl_valid;
            /* Unlock potential acquirers */
            if (wq->head) pthread_cond_broadcast(&wq->head->cond);
            /* Unlock potential requesters */
            pthread_cond_broadcast(&wh->cond);
            break;
          } else pthread_cond_wait(&wh->cond, &wq->mut);
        }
      }
    } else {
      if (!wh->next) ret = orwl_valid;
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
  size_t data_len = wq->data_len;
  /* realloc is allowed to return something non-0 if len is
     0. Avoid that. */
  if (len) {
    size_t const blen = len * sizeof(uint64_t);
    uint64_t* data = (uint64_t*)realloc(wq->data, blen);
    if (data) {
      size_t const data_blen = data_len * sizeof(uint64_t);
      if (data_blen < blen)
        memset(&data[data_len], 0, blen - data_blen);
      if (wq->data != data)
        wq->data = data;
      wq->data_len = len;
    } else {
      /* realloc failed, don't do anything */
      report(true, "adding suplement of length %zu failed", len);
    }
  } else {
    if (wq->data) free(wq->data);
    wq->data = 0;
    wq->data_len = 0;
  }
}

void orwl_wh_resize(orwl_wh* wh, size_t len) {
  if (orwl_wh_acquire(wh, 0) == orwl_acquired) {
    orwl_wq *wq = wh->location;
    assert(wq);
    MUTUAL_EXCLUDE(wq->mut)
      orwl_wq_resize_locked(wq, len);
  }
}

void orwl_state_destroy(orwl_state *el);
orwl_state* orwl_state_init (orwl_state *el, orwl_state val);

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
