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
#ifndef ORWL_COUNT_H
#define ORWL_COUNT_H

#include "orwl_futex.h"
#include "orwl_thread.h"

P99_DECLARE_STRUCT(orwl_count);

/**
 ** @brief A counter that allows atomic increment and decrement and a
 ** wait for the value 0.
 **
 ** This is implemented more or less efficiently according to the
 ** features that the platform provides:
 ** - one that uses atomic operations and futexes for signaling
 ** - one that uses atomic operations and a mutex/condition pair for signaling
 ** - one that mutexes all access and uses a condition variable for
 **   signaling.
 **/
struct orwl_count {
  /** @brief An atomic @c size_t that make up the counter itself.
   **
   ** Its address might be used as a @c int* for signaling through a
   ** futex call if this is supported.
   **/
  volatile union {
    atomic_size_t large;
    int narrow;
  }
    overl;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
  /** @brief A mutex that is used in the non-futex implementations */
  pthread_mutex_t mut;
  /** @brief A conditional variable that is used in the non-futex
   ** implementations */
  pthread_cond_t cnd;
#endif
};



#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
# define ORWL_COUNT_INITIALIZER(INITIAL)                       \
{                                                              \
  .overl = { .large = INITIAL },                               \
    .mut = PTHREAD_MUTEX_INITIALIZER,                          \
       .cnd = PTHREAD_COND_INITIALIZER                         \
}
#else
/**
 ** @brief Initialize an ::orwl_count object.
 **/
# define ORWL_COUNT_INITIALIZER(INITIAL)                       \
{                                                              \
  .overl = { .large = INITIAL },                               \
}
#endif

inline
orwl_count* orwl_count_init(orwl_count* c, size_t initial) {
  if (c) {
    c->overl.large = initial;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
    pthread_mutex_init(&c->mut);
    pthread_cond_init(&c->cnd);
#endif
  }
  return c;
}

#define orwl_count_init(...) P99_CALL_DEFARG(orwl_count_init, 2, __VA_ARGS__)
#define orwl_count_init_defarg_1() P99_0(size_t)

inline
void orwl_count_destroy(orwl_count* c) {
  if (c) {
    c->overl.large = 0;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
    pthread_mutex_destroy(&c->mut);
    pthread_cond_destroy(&c->cnd);
#endif
  }
}

/**
 ** @brief Account the @c pthread_rwlock_t @a COUNT during execution
 ** of a dependent block or statement.
 **/
P99_BLOCK_DOCUMENT
#define ORWL_ACCOUNT(COUNT) P99_PROTECTED_BLOCK(orwl_count_inc(&COUNT), orwl_count_dec(&COUNT))

/**
 ** @brief increment the counter @a counter atomically by @a howmuch.
 ** @return the value of the counter before the increment.
 ** @remark @a howmuch defaults to 1 if omitted.
 ** @memberof orwl_count
 **/
inline size_t orwl_count_inc(orwl_count* counter, size_t howmuch);

#define orwl_count_inc(...) P99_CALL_DEFARG(orwl_count_inc, 2, __VA_ARGS__)
#define orwl_count_inc_defarg_1() ((size_t)1)

/**
 ** @brief Obtain the value of counter @a counter atomically.
 ** @memberof orwl_count
 **/
inline size_t orwl_count_value(orwl_count* counter);


/**
 ** @brief decrement the counter @a counter atomically by @a howmuch.
 **
 ** Also signals eventual waiters if the counter falls to 0.
 ** @return the value of the counter after the decrement.
 ** @remark @a howmuch defaults to 1 if omitted.
 ** @memberof orwl_count
 **/
inline size_t orwl_count_dec(orwl_count* counter, size_t howmuch);

#define orwl_count_dec(...) P99_CALL_DEFARG(orwl_count_dec, 2, __VA_ARGS__)
#define orwl_count_dec_defarg_1() ((size_t)1)

/**
 ** @brief wait until the counter @a counter falls to 0.
 ** @memberof orwl_count
 **/
inline void orwl_count_wait(orwl_count* counter);

#ifdef HAVE_ATOMIC

inline
size_t orwl_count_inc(orwl_count* counter, size_t howmuch) {
  size_t ret = atomic_fetch_add(&counter->overl.large, howmuch);
  return ret;
}

inline
size_t orwl_count_value(orwl_count* counter) {
  size_t ret = atomic_load(&counter->overl.large);
  return ret;
}

#ifndef HAVE_FUTEX

inline
size_t orwl_count_dec(orwl_count* counter, size_t howmuch) {
  /* if we are the last notify the waiters */
  size_t val = atomic_fetch_sub(&counter->overl.large, howmuch);
  if (P99_UNLIKELY(val <= howmuch))
    MUTUAL_EXCLUDE(counter->mut)
      pthread_cond_broadcast(&counter->cnd);
  return val - howmuch;
}

inline
void orwl_count_wait(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    while (atomic_load(&counter->overl.large)) pthread_cond_wait(&counter->cnd, &counter->mut);
  }
}

# else
inline
size_t orwl_count_dec(orwl_count* counter, size_t howmuch) {
  /* if we are the last notify the waiters */
  register int val = atomic_fetch_sub(&counter->overl.large, howmuch);
  if (P99_UNLIKELY(val <= howmuch))
    orwl_futex_broadcast((int*)&(counter->overl.narrow));
  return val - howmuch;
}

inline
void orwl_count_wait(orwl_count* counter) {
  while (true) {
    if (!atomic_load(&counter->overl.large)) break;
    /* futexes unfortunately only work on int */
    /* we chose the least significant one to be sure to capture an
       intermittent change of the value */
    ORWL_FUTEX_WAIT(&(counter->overl.narrow), x, !x);
    /* Check if really the whole counter is 0, so we iterate. */
  }
}
#endif
#else

inline
size_t orwl_count_inc(orwl_count* counter, size_t howmuch) {
  size_t ret = 0;
  MUTUAL_EXCLUDE(counter->mut) {
    ret = counter->overl.large;
    counter->overl.large = ret + howmuch;
  }
  return ret;
}

inline
size_t orwl_count_value(orwl_count* counter) {
  size_t ret = 0;
  MUTUAL_EXCLUDE(counter->mut) {
    ret = counter->overl.large;
  }
  return ret;
}

inline
size_t orwl_count_dec(orwl_count* counter, size_t howmuch) {
  size_t ret = 0;
  MUTUAL_EXCLUDE(counter->mut) {
    ret = counter->overl.large;
    counter->overl.large = ret - howmuch;
    if (!(counter->overl.large)) pthread_cond_broadcast(&counter->cnd);
  }
  return ret;
}

inline
void orwl_count_wait(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    while (counter->overl.large) pthread_cond_wait(&counter->cnd, &counter->mut);
  }
}
#endif

/**
 ** @brief A restriction of ::orwl_count to set flag and notify
 ** other threads that an event has occurred.
 **
 ** @see orwl_notifier_set
 ** @see orwl_notifier_block
 ** @see orwl_notifier_verify
 **/
typedef orwl_count orwl_notifier;

#define ORWL_NOTIFIER_INITIALIZER ORWL_COUNT_INITIALIZER(1)

/**
 ** @memberof orwl_notifier
 **/
inline
orwl_notifier* orwl_notifier_init(orwl_notifier* notifier) {
  orwl_count_init(notifier, 1);
  return notifier;
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_destroy(orwl_notifier* notifier) {
  orwl_count_destroy(notifier);
}

/**
 ** @brief Verify if the notification flag has been set. Non blocking. 
 ** @memberof orwl_notifier
 **/
inline
bool orwl_notifier_verify(orwl_notifier* notifier) {
  return !orwl_count_value(notifier);
}

/**
 ** @brief Set the notifier flag and wake up all potential waiters. 
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_set(orwl_notifier* notifier) {
  if (!orwl_notifier_verify(notifier)) {
    if (orwl_count_dec(notifier, 1)) {
      report(true, "duplicated notifier set for %p", (void*)notifier);
    }
  }
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_unset(orwl_notifier* notifier) {
  if (orwl_notifier_verify(notifier)) {
    if (orwl_count_inc(notifier, 1)) {
      report(true, "duplicated notifier unset for %p", (void*)notifier);
    }
  }
}

/**
 ** @brief Block the notification flag has been set.
 **
 ** Non blocking if the flag is already set.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_block(orwl_notifier* notifier) {
  orwl_count_wait(notifier);
}

#endif
