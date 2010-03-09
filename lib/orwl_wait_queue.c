/*
** orwl-wait_queue.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri Feb 19 11:52:37 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "orwl_wait_queue.h"

DEFINE_ENUM(orwl_state);

define_default_arg(orwl_wh_test, 1, uintptr_t);
define_default_arg(orwl_wh_acquire, 1, uintptr_t);
define_default_arg(orwl_wh_load, 1, uintptr_t);
define_default_arg(orwl_wh_unload, 1, uintptr_t);

static pthread_mutexattr_t smattr = { { 0 } };

DEFINE_ONCE(orwl_wq) {
  pthread_mutexattr_init(&smattr);
}

void FUNC_DEFAULT(orwl_wq_init)(orwl_wq *wq,
                                const pthread_mutexattr_t *attr) {
  INIT_ONCE(orwl_wq);
  if (!attr) attr = &smattr;
  pthread_mutex_init(&wq->mut, attr);
  wq->head = NULL;
  wq->tail = NULL;
  wq->clock = 0;
}

define_default_arg(orwl_wq_init, 1, const pthread_mutexattr_t *);

void orwl_wq_destroy(orwl_wq *wq) {
  assert(!wq->head);
  assert(!wq->tail);
  pthread_mutex_destroy(&wq->mut);
  wq->head = orwl_wh_garb;
  wq->tail = orwl_wh_garb;
  wq->clock = ~(uintptr_t)0;
}

DEFINE_NEW_DELETE(orwl_wq);

static pthread_condattr_t scattr = { { 0 } };

DEFINE_ONCE(orwl_wh) {
  pthread_condattr_init(&scattr);
}

void FUNC_DEFAULT(orwl_wh_init)(orwl_wh *wh,
                  const pthread_condattr_t *attr) {
  INIT_ONCE(orwl_wh);
  if (!attr) attr = &scattr;
  pthread_cond_init(&wh->cond, attr);
  wh->location = NULL;
  wh->next = NULL;
  wh->tokens = 0;
  wh->priority = 0;
}

define_default_arg(orwl_wh_init, 1, const pthread_condattr_t *);

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  pthread_cond_destroy(&wh->cond);
  wh->location = orwl_wq_garb;
  wh->next = orwl_wh_garb;
  wh->priority = ~(uintptr_t)0;
}

DEFINE_NEW_DELETE(orwl_wh);

int orwl_wh_valid(orwl_wh *wh);
int orwl_wh_idle(orwl_wh *wh);
/* This supposes that wq != NULL */
int orwl_wq_valid(orwl_wq *wq);
/* This supposes that wq != NULL */
int orwl_wq_idle(orwl_wq *wq);

/* This supposes that the corresponding wq != NULL */
void FUNC_DEFAULT(orwl_wh_load)(orwl_wh *wh, uintptr_t howmuch);
/* This supposes that the corresponding wq != NULL */
void FUNC_DEFAULT(orwl_wh_unload)(orwl_wh *wh, uintptr_t howmuch);

orwl_state FUNC_DEFAULT(orwl_wq_request)(orwl_wq *wq, VA_ARGS(number)) {
  orwl_state ret = orwl_invalid;
  if (wq && orwl_wq_valid(wq)) {
    pthread_mutex_lock(&wq->mut);
    /* Check (and wait eventually) that all wh are idle */
    for (bool idle = false; !idle; ) {
      va_list ap;
      va_start(ap, number);
      idle = true;
      for (size_t i = 0; i < number; ++i) {
        orwl_wh *wh = va_arg(ap, orwl_wh*);
        va_arg(ap, uintptr_t);
        if (!orwl_wh_idle(wh)) {
          idle = false;
          pthread_cond_wait(&wh->cond, &wq->mut);
        }
      }
      va_end(ap);
    }
    /* Now insert them */
    va_list ap;
    va_start(ap, number);
    for (size_t i = 0; i < number; ++i) {
      orwl_wh *wh = va_arg(ap, orwl_wh*);
      size_t howmuch = va_arg(ap, uintptr_t);
      wh->location = wq;
      orwl_wh_load(wh, howmuch);
      if (orwl_wq_idle(wq)) wq->head = wh;
      else wq->tail->next = wh;
      wq->tail = wh;
      ++wq->clock;
      wh->priority = wq->clock;
    }
    va_end(ap);
    ret = orwl_requested;
    pthread_mutex_unlock(&wq->mut);
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
    RETRY:
      orwl_wh_load(wh, 1);
      pthread_cond_wait(&wh->cond, &wq->mut);
      orwl_wh_unload(wh, 1);
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

orwl_state FUNC_DEFAULT(orwl_wh_acquire)(orwl_wh *wh, uintptr_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq) {
      pthread_mutex_lock(&wq->mut);
      ret = orwl_wh_acquire_locked(wh, wq);
      if (ret == orwl_acquired)
        orwl_wh_unload(wh, howmuch);
      pthread_mutex_unlock(&wq->mut);
    }
  }
  return ret;
}

orwl_state FUNC_DEFAULT(orwl_wh_test)(orwl_wh *wh, uintptr_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq) {
      pthread_mutex_lock(&wq->mut);
      if (orwl_wq_valid(wq)
          && wq->head) {
        ret = wq->head == wh ? orwl_acquired : orwl_requested;
      }
      if (ret == orwl_acquired) orwl_wh_unload(wh, howmuch);
      pthread_mutex_unlock(&wq->mut);
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
      pthread_mutex_lock(&wq->mut);
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
      pthread_mutex_unlock(&wq->mut);
    } else {
      if (!wh->next) ret = orwl_valid;
    }
  }
  return ret;
}

void orwl_state_destroy(orwl_state *el);
define_default_arg(orwl_state_init, 1, orwl_state);
void FUNC_DEFAULT(orwl_state_init) (orwl_state *el, orwl_state val);

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
