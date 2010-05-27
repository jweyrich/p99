/*
** orwl-wait_queue.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri Feb 19 11:52:37 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_wait_queue.h"

#include "orwl_thread.h"

DEFINE_ENUM(orwl_state);

DEFINE_DEFARG(orwl_wh_test, , 0);
DEFINE_DEFARG(orwl_wh_acquire, , 1);
DEFINE_DEFARG(orwl_wh_load, , 1);
DEFINE_DEFARG(orwl_wh_unload, , 1);

static pthread_mutexattr_t smattr = INIT2;

DEFINE_ONCE(orwl_wq) {
  pthread_mutexattr_init(&smattr);
}

orwl_wq* orwl_wq_init(orwl_wq *wq,
                                const pthread_mutexattr_t *attr) {
  INIT_ONCE(orwl_wq);
  if (!attr) attr = &smattr;
  pthread_mutex_init(&wq->mut, attr);
  wq->head = NULL;
  wq->tail = NULL;
  wq->data = NULL;
  wq->data_len = 0;
  wq->clock = 1;
  return wq;
}

DEFINE_DEFARG(orwl_wq_init, , NULL);

void orwl_wq_destroy(orwl_wq *wq) {
  assert(!wq->head);
  assert(!wq->tail);
  if (wq->data) free(wq->data);
  pthread_mutex_destroy(&wq->mut);
  wq->head = TGARB(orwl_wh*);
  wq->tail = TGARB(orwl_wh*);
  wq->data = TGARB(void*);
  wq->clock = TONES(uint64_t);
  wq->data_len = TONES(size_t);
}

DEFINE_NEW_DELETE(orwl_wq);

static pthread_condattr_t scattr = INIT2;

DEFINE_ONCE(orwl_wh) {
  pthread_condattr_init(&scattr);
}

orwl_wh* orwl_wh_init(orwl_wh *wh,
                  const pthread_condattr_t *attr) {
  INIT_ONCE(orwl_wh);
  if (!attr) attr = &scattr;
  pthread_cond_init(&wh->cond, attr);
  wh->location = NULL;
  wh->next = NULL;
  wh->tokens = 0;
  wh->priority = 0;
  wh->svrID = 0;
  return wh;
}

DEFINE_DEFARG(orwl_wh_init, , NULL);

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  pthread_cond_destroy(&wh->cond);
  wh->location = TGARB(orwl_wq*);
  wh->next = TGARB(orwl_wh*);
  wh->priority = TONES(int64_t);
}

DEFINE_NEW_DELETE(orwl_wh);

int orwl_wh_valid(orwl_wh *wh);
int orwl_wh_idle(orwl_wh *wh);
/* This supposes that wq != NULL */
int orwl_wq_valid(orwl_wq *wq);
/* This supposes that wq != NULL */
int orwl_wq_idle(orwl_wq *wq);

/* This supposes that the corresponding wq != NULL */
uint64_t orwl_wh_load(orwl_wh *wh, uint64_t howmuch);
/* This supposes that the corresponding wq != NULL */
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

orwl_state FSYMB(orwl_wq_request)(orwl_wq *wq, VA_ARGS(number)) {
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
          VA_MODARG(ap, orwl_wq_request, 1);
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
        uint64_t howmuch = (hm > TNULL(int64_t)) ? hm : -hm;
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
              assert(hm >= TNULL(int64_t));
              /* if the wh is NULL, take this as a request to add to the
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
            wh->location = NULL;
            if (!wh->next) wq->tail = NULL;
            wq->head = wh->next;
            wh->next = NULL;
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

void orwl_wh_map(orwl_wh* wh, uint64_t** datap, size_t* data_len) {
  if (datap && data_len) {
    if (orwl_wh_acquire(wh) == orwl_acquired) {
      *datap = wh->location->data;
      *data_len = wh->location->data_len;
    } else {
      *datap = NULL;
      *data_len = 0;
    }
  }
}

void orwl_wh_resize(orwl_wh* wh, size_t len) {
  if (orwl_wh_acquire(wh) == orwl_acquired) {
    size_t data_len = wh->location->data_len;
    /* realloc is allowed to return something non-NULL if len is
       0. Avoid that. */
    if (len) {
      size_t const blen = len * sizeof(uint64_t);
      uint64_t* data = (uint64_t*)realloc(wh->location->data, blen);
      if (data) {
        size_t const data_blen = data_len * sizeof(uint64_t);
        if (data_blen < blen)
          memset(&data[data_len], 0, blen - data_blen);
        if (wh->location->data != data)
          wh->location->data = data;
        wh->location->data_len = len;
      } else {
        /* realloc failed, don't do anything */
      }
    } else {
      if (wh->location->data) free(wh->location->data);
      wh->location->data = NULL;
      wh->location->data_len = 0;
    }
  }
}




void orwl_state_destroy(orwl_state *el);
DEFINE_DEFARG(orwl_state_init, , orwl_invalid);
orwl_state* orwl_state_init (orwl_state *el, orwl_state val);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_state, {0});

DEFINE_ORWL_TYPE_DYNAMIC(orwl_wq, {0});

DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_acquire, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_release, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_load, orwl_wh);
DEFINE_ORWL_REGISTER_ALIAS(orwl_wh_unload, orwl_wh);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_wh,
                         ORWL_REGISTER_ALIAS(orwl_wh_acquire),
                         ORWL_REGISTER_ALIAS(orwl_wh_release),
                         ORWL_REGISTER_ALIAS(orwl_wh_load),
                         ORWL_REGISTER_ALIAS(orwl_wh_unload)
                         );
