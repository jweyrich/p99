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
#include <string.h>
#include "orwl_wait_queue.h"

DEFINE_ENUM(orwl_state);


static pthread_mutexattr_t smattr = { { 0 } };

#define report(F, ...) fprintf(stderr, "%ju: " F, (uintmax_t)pthread_self(), __VA_ARGS__)

DEFINE_ONCE(orwl_wq) {
  pthread_mutexattr_init(&smattr);
}

void orwl_wq_init(orwl_wq *wq,
                  const pthread_mutexattr_t *attr) {
  INIT_ONCE(orwl_wq);
  if (!attr) attr = &smattr;
  pthread_mutex_init(&wq->mut, attr);
  wq->head = NULL;
  wq->tail = NULL;
}

void orwl_wq_destroy(orwl_wq *wq) {
  assert(!wq->head);
  assert(!wq->tail);
  pthread_mutex_destroy(&wq->mut);
  wq->head = orwl_wh_garb;
  wq->tail = orwl_wh_garb;
}

DEFINE_NEW_DELETE(orwl_wq);

static pthread_condattr_t scattr = { { 0 } };

DEFINE_ONCE(orwl_wh) {
  pthread_condattr_init(&scattr);
}

void orwl_wh_init(orwl_wh *wh,
                  const pthread_condattr_t *attr) {
  INIT_ONCE(orwl_wh);
  if (!attr) attr = &scattr;
  pthread_cond_init(&wh->cond, attr);
  wh->location = NULL;
  wh->next = NULL;
  wh->tokens = 0;
}

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  pthread_cond_destroy(&wh->cond);
  wh->location = orwl_wq_garb;
  wh->next = orwl_wh_garb;
}

DEFINE_NEW_DELETE(orwl_wh);

int orwl_wh_valid(orwl_wh *wh);
int orwl_wh_idle(orwl_wh *wh);
/* This supposes that wq != NULL */
int orwl_wq_valid(orwl_wq *wq);
/* This supposes that wq != NULL */
int orwl_wq_idle(orwl_wq *wq);

orwl_state orwl_wait_request(orwl_wh *wh, orwl_wq *wq, uintptr_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (wq && orwl_wq_valid(wq)) {
    pthread_mutex_lock(&wq->mut);
    while (!orwl_wh_idle(wh)) pthread_cond_wait(&wh->cond, &wq->mut);
    wh->location = wq;
    orwl_wh_load(wh, howmuch);
    if (orwl_wq_idle(wq)) wq->head = wh;
    else wq->tail->next = wh;
    wq->tail = wh;
    ret = orwl_requested;
    pthread_mutex_unlock(&wq->mut);
  }
  return ret;
}

orwl_state orwl_wait_acquire_locked(orwl_wh *wh, orwl_wq *wq) {
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

orwl_state orwl_wait_acquire(orwl_wh *wh, uintptr_t howmuch) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq) {
      pthread_mutex_lock(&wq->mut);
      ret = orwl_wait_acquire_locked(wh, wq);
      if (ret == orwl_acquired)
        orwl_wh_unload(wh, howmuch);
      pthread_mutex_unlock(&wq->mut);
    }
  }
  return ret;
}

orwl_state orwl_wait_test(orwl_wh *wh, uintptr_t howmuch) {
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

orwl_state orwl_wait_release(orwl_wh *wh) {
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
          /* Unlock potential requesters */
          pthread_cond_broadcast(&wh->cond);
          /* Unlock potential acquirers */
          if (wq->head) pthread_cond_broadcast(&wq->head->cond);
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
