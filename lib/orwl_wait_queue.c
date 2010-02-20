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
#include <string.h>
#include "orwl_wait_queue.h"

struct _orwl_wq {
  pthread_mutex_t mut;
  orwl_wh *head;
  orwl_wh *tail;
};


struct _orwl_wh {
  pthread_cond_t cond;
  orwl_wq *location;
  orwl_wh *next;
  uint64_t waiters;
};

static orwl_wh *const orwl_wh_garb = ((orwl_wh*)(~(uintptr_t)0));
static orwl_wq *const orwl_wq_garb = ((orwl_wq*)(~(uintptr_t)0));

static pthread_mutexattr_t *smattr_p = NULL;
static pthread_mutex_t smut = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutexattr_t smattr = {};

void orwl_wq_init(orwl_wq *wq,
                  const pthread_mutexattr_t *attr) {
  if (!smattr_p) {
    pthread_mutex_lock(&smut);
    if (!smattr_p) {
      pthread_mutexattr_init(&smattr);
      smattr_p = &smattr;
    }
    pthread_mutex_unlock(&smut);
  }
  if (!attr) attr = smattr_p;
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

#define DEFINE_NEW(T, ...)                      \
T *T ## _new(void) {                            \
  T *ret = (T*)malloc(sizeof(T));               \
  if (ret) T ## _init(ret, __VA_ARGS__);        \
  return ret;                                   \
}

#define DEFINE_DELETE(T)                        \
void T ## _delete(T *el) {                      \
  if (el) {                                     \
    T ## _destroy(el);                          \
    free(el);                                   \
  }                                             \
}

#define DEFINE_VNEW(T)                          \
T *const*T ## _vnew(size_t n) {                 \
  size_t N = (n + 1)*sizeof(T*);                \
  T **ret = malloc(N);                          \
  if (ret) {                                    \
    size_t i;                                   \
    memset(ret, 0, N);                          \
    assert(!ret[n]);                            \
    for (i = 0; i < n; ++i) {                   \
      ret[i] = T ## _new();                     \
      /* roll back if allocation failed */      \
      if (!ret[i]) {                            \
        T ## _vdelete(ret);                     \
        ret = NULL;                             \
        break;                                  \
      }                                         \
    }                                           \
  }                                             \
  return ret;                                   \
}

#define DEFINE_VDELETE(T)                       \
void T ## _vdelete(T *const*vec) {              \
  if (vec) {                                    \
    T *v;                                       \
    for (v = *vec; v; ++v)                      \
      T ## _delete(v);                          \
    free((T**)vec);                             \
  }                                             \
}

#define DEFINE_NEW_DELETE(T, ...)               \
DEFINE_NEW(T, __VA_ARGS__);                     \
DEFINE_VNEW(T);                                 \
DEFINE_DELETE(T);                               \
DEFINE_VDELETE(T)

DEFINE_NEW_DELETE(orwl_wq, NULL);


static pthread_condattr_t *scattr_p = NULL;
static pthread_condattr_t scattr = { };

void orwl_wh_init(orwl_wh *wh,
                  const pthread_condattr_t *attr) {
  if (!scattr_p) {
    //static pthread_mutex_t smut = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&smut);
    if (!scattr_p) {
      pthread_condattr_init(&scattr);
      scattr_p = &scattr;
    }
    pthread_mutex_unlock(&smut);
  }
  if (!attr) attr = scattr_p;
  pthread_cond_init(&wh->cond, attr);
  wh->location = NULL;
  wh->next = NULL;
  wh->waiters = 0;
}

void orwl_wh_destroy(orwl_wh *wh) {
  assert(!wh->location);
  assert(!wh->next);
  pthread_cond_destroy(&wh->cond);
  wh->location = orwl_wq_garb;
  wh->next = orwl_wh_garb;
}

DEFINE_NEW_DELETE(orwl_wh, NULL);

static
int orwl_wh_valid(orwl_wh *wh) {
  return wh
    && wh->location != orwl_wq_garb
    && wh->next != orwl_wh_garb;
}

static
int orwl_wh_idle(orwl_wh *wh) {
  return wh && !wh->location && !wh->next;
}

/* This supposes that wq != NULL */
static
int orwl_wq_valid(orwl_wq *wq) {
  return wq->head != orwl_wh_garb
    && wq->tail != orwl_wh_garb;
}

/* This supposes that wq != NULL */
static
int orwl_wq_idle(orwl_wq *wq) {
  return !wq->head && !wq->tail;
}

orwl_state orwl_wait_request(orwl_wh *wh, orwl_wq *wq) {
  orwl_state ret = orwl_invalid;
  if (wq && orwl_wq_valid(wq) && orwl_wh_idle(wh)) {
    pthread_mutex_lock(&wq->mut);
    wh->location = wq;
    if (orwl_wq_idle(wq)) wq->head = wh;
    else wq->tail->next = wh;
    wq->tail = wh;
    ret = orwl_requested;
    pthread_mutex_unlock(&wq->mut);
  }
  return ret;
}

orwl_state orwl_wait_acquire(orwl_wh *wh) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq) {
      pthread_mutex_lock(&wq->mut);
      if (orwl_wq_valid(wq) && wq->head) {
        /* We are on the fast path */
        if (wq->head == wh) ret = orwl_acquired;
        /* We are on the slow path */
        else {
          ++wh->waiters;
          pthread_cond_wait(&wh->cond, &wq->mut);
          --wh->waiters;
          /* Check everything again, somebody might have destroyed
             our wq */
          if (orwl_wq_valid(wq) && wq->head == wh)
            ret = orwl_acquired;
        }
      }
      pthread_mutex_unlock(&wq->mut);
    }
  }
  return ret;
}

orwl_state orwl_wait_test(orwl_wh *wh) {
  orwl_state ret = orwl_invalid;
  if (orwl_wh_valid(wh)) {
    orwl_wq *wq = wh->location;
    if (wq) {
      pthread_mutex_lock(&wq->mut);
      if (orwl_wq_valid(wq)
          && wq->head) {
        ret = wq->head == wh ? orwl_acquired : orwl_requested;
      }
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
      if (orwl_wq_valid(wq)
          && wq->head == wh
          && !wh->waiters) {
        if (wh->next)
          pthread_cond_broadcast(&wh->next->cond);
        else
          wq->tail = NULL;
        wq->head = wh->next;
        wh->location = NULL;
        wh->next = NULL;
        ret = orwl_valid;
      }
      pthread_mutex_unlock(&wq->mut);
    } else {
      if (!wh->next) ret = orwl_valid;
    }
  }
  return ret;
}
