/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef ORWL_NOTIFIER_H
#define ORWL_NOTIFIER_H

#include "orwl_count.h"

P99_DECLARE_STRUCT(orwl_notifier);

/**
 ** @brief Set a flag and notify other threads that an event has
 ** occurred.
 **
 ** @see orwl_notifier_set
 ** @see orwl_notifier_block
 ** @see orwl_notifier_verify
 **/
struct orwl_notifier {
  atomic_size_t val;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
  /** @brief A mutex that is used in the non-futex implementations */
  pthread_mutex_t mut;
  /** @brief A conditional variable that is used in the non-futex
   ** implementations */
  pthread_cond_t cnd;
#endif
};

/**
 ** @memberof orwl_notifier
 **/
inline
orwl_notifier* orwl_notifier_init(orwl_notifier* notifier);

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_destroy(orwl_notifier* notifier);

/**
 ** @brief Verify if the notification flag has been set. Non blocking.
 ** @memberof orwl_notifier
 **/
inline
bool orwl_notifier_verify(orwl_notifier* notifier);

/**
 ** @brief Set the notifier flag and wake up all potential waiters.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_set(orwl_notifier* notifier);

/**
 ** @brief Reset the notifier flag.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_unset(orwl_notifier* notifier);

/**
 ** @brief Block until the notification flag has been set.
 **
 ** Non blocking if the flag is already set.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_block(orwl_notifier* notifier);


#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
# define ORWL_NOTIFIER_INITIALIZER                             \
{                                                              \
  .val = 0,                                                    \
    .mut = PTHREAD_MUTEX_INITIALIZER,                          \
       .cnd = PTHREAD_COND_INITIALIZER                         \
}
#else
/**
 ** @brief Initialize an ::orwl_notifier object.
 **/
# define ORWL_NOTIFIER_INITIALIZER                             \
{                                                              \
  .val = 0,                                                    \
}
#endif

/**
 ** @memberof orwl_notifier
 **/
inline
orwl_notifier* orwl_notifier_init(orwl_notifier* c) {
  if (c) {
    c->val = 0;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
    pthread_mutex_init(&c->mut);
    pthread_cond_init(&c->cnd);
#endif
  }
  return c;
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_destroy(orwl_notifier* c) {
  if (c) {
    c->val = 1;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
    pthread_mutex_destroy(&c->mut);
    pthread_cond_destroy(&c->cnd);
#endif
  }
}

#if defined(HAVE_ATOMIC)

/**
 ** @brief Verify if the notification flag has been set. Non blocking.
 ** @memberof orwl_notifier
 **/
inline
bool orwl_notifier_verify(orwl_notifier* notifier) {
  assert(notifier);
  return atomic_load(&notifier->val);
}

/**
 ** @brief Set the notifier flag and wake up all potential waiters.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_set(orwl_notifier* notifier) {
  atomic_store(&notifier->val, 1);
#if defined(HAVE_FUTEX)
  orwl_futex_broadcast((int*)&notifier->val);
#else
  pthread_cond_broadcast(&notifier->cnd);
#endif
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_unset(orwl_notifier* notifier) {
  atomic_store(&notifier->val, 0);
}

/**
 ** @brief Block until the notification flag has been set.
 **
 ** Non blocking if the flag is already set.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_block(orwl_notifier* notifier) {
  while (!orwl_notifier_verify(notifier)) {
#if defined(HAVE_FUTEX)
    ORWL_FUTEX_WAIT(&notifier->val, x, x);
#else
    MUTUAL_EXCLUDE(notifier->mut)
      pthread_cond_wait(&notifier->cnd, &notifier->mut);
#endif
  }
}

#else

/**
 ** @brief Verify if the notification flag has been set. Non blocking.
 ** @memberof orwl_notifier
 **/
inline
bool orwl_notifier_verify(orwl_notifier* notifier) {
  bool ret = false;
  MUTUAL_EXCLUDE(notifier->mut)
    ret = notifier->val;
  return ret;
}

/**
 ** @brief Set the notifier flag and wake up all potential waiters.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_set(orwl_notifier* notifier) {
  MUTUAL_EXCLUDE(notifier->mut) {
    notifier->val = 1;
    pthread_cond_broadcast(&notifier->cnd);
  }
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_unset(orwl_notifier* notifier) {
  MUTUAL_EXCLUDE(notifier->mut)
    notifier->val = 0;
}

/**
 ** @brief Block until the notification flag has been set.
 **
 ** Non blocking if the flag is already set.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_block(orwl_notifier* notifier) {
  MUTUAL_EXCLUDE(notifier->mut)
    while (!notifier->val)
      pthread_cond_wait(&notifier->cnd, &notifier->mut);
}

#endif

DECLARE_NEW_DELETE(orwl_notifier);

#endif
