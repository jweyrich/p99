/* This may look like nonsense, but it really is -*- C -*-                   */
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

#include "orwl_once.h"
#include "orwl_atomic.h"
#include "p99_posix_default.h"

P99_DECLARE_STRUCT(orwl_count);

#if (defined(__linux__) && !defined(NO_FUTEX)) || defined(DOXYGEN)
#define HAVE_FUTEX 1

# include <linux/futex.h>
# include <sys/syscall.h>

/**
 ** @brief Wrap the linux system call @c futex.
 **
 ** @c syscall is a va_arg function, so we would never be sure
 ** of the type conversions that could take place. Clearly specify the
 ** interface.
 **
 ** @warning This is probably nothing that should be called directly.
 ** @see orwl_futex_wake
 ** @see orwl_futex_wait
 ** @see orwl_futex_signal
 ** @see orwl_futex_broadcast
 **/
P99_DEFARG_DOCU(orwl_futex)
inline
int orwl_futex(int *uaddr, /*!< the base address to be used */
               int op,     /*!< the operation that is to be performed */
               int val,    /*!< the value that a wait operation
                             expects when going into wait, or the
                             number of tasks to wake up */
               const struct timespec
               *timeout, /*!< a time out for wait, not yet defaults to
                            0. */
               int *uaddr2, /*!< unused, defaults to 0 */
               int val3     /*!< unused, defaults to 0 */
               ) {
  return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}

#define orwl_futex(...) P99_CALL_DEFARG(orwl_futex, 6, __VA_ARGS__)
#define orwl_futex_defarg_3() ((void*)0)
#define orwl_futex_defarg_4() 0
#define orwl_futex_defarg_5() 0

/**
 ** @brief Wait until the value to which @a uaddr points to is equal
 ** to @a expected.
 **/
inline
int orwl_futex_wait(int* uaddr, int expected) {
  for (;;) {
    int val = *uaddr;
    if (val == expected) return 0;
    if (orwl_futex(uaddr, FUTEX_WAIT, val) < 0) {
      switch (errno) {
      case EWOULDBLOCK: continue;
      case EINTR: continue;
      }
      int ret = errno;
      errno = 0;
      return ret;
    }
  }
}

/**
 ** @brief Wakeup waiters for address @a uaddr.
 **
 ** If there are @a wakeup waiters, as much waiters are woke up. If
 ** there are less, all waiters are woken up.
 **/
inline
int orwl_futex_wake(int* uaddr, int wakeup) {
  int ret = orwl_futex(uaddr, FUTEX_WAKE, wakeup);
  return ret;
}

/**
 ** @brief Wakeup one waiter for address @a uaddr, if there is any.
 **/
inline
int orwl_futex_signal(int* uaddr) {
  return orwl_futex_wake(uaddr, 1);
}

/**
 ** @brief Wakeup all waiters for address @a uaddr, if there are
 ** any.
 **/
inline
int orwl_futex_broadcast(int* uaddr) {
  return orwl_futex_wake(uaddr, INT_MAX);
}

#endif

#if (defined(ATOMIC_OPS) && ATOMIC_OPS > 0) || (defined(__GNUC__) && (!defined(GNUC_NO_SYNC) || defined(GNUC_SYNC_REPLACE)))
# define HAVE_ATOMIC
#endif


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
# define ORWL_COUNT_INITIALIZER                                \
{                                                              \
  .overl = { .large = 0 },                                     \
    .mut = PTHREAD_MUTEX_INITIALIZER,                          \
       .cnd = PTHREAD_COND_INITIALIZER                         \
}
#else
/**
 ** @brief Initialize an ::orwl_count object.
 **/
# define ORWL_COUNT_INITIALIZER                                \
{                                                              \
  .overl = { .large = 0 },                                     \
}
#endif

/**
 ** @brief Account the @c pthread_rwlock_t @a COUNT during execution
 ** of a dependent block or statement.
 **/
P99_BLOCK_DOCUMENT
#define ORWL_ACCOUNT(COUNT) P99_PROTECTED_BLOCK(orwl_count_inc(&COUNT), orwl_count_dec(&COUNT))

/**
 ** @brief increment the counter @a counter atomically.
 ** @return the value of the counter before the increment.
 ** @memberof orwl_count
 **/
inline size_t orwl_count_inc(orwl_count* counter);
/**
 ** @brief decrement the counter @a counter atomically.
 **
 ** Also signals eventual waiters if the counter falls to 0.
 ** @return the value of the counter after the decrement.
 ** @memberof orwl_count
 **/
inline size_t orwl_count_dec(orwl_count* counter);
/**
 ** @brief wait until the counter @a counter falls to 0.
 **
 ** @return 0 if the waiting was successful, an error indication otherwise.
 **
 ** @memberof orwl_count
 **/
inline int orwl_count_wait(orwl_count* counter);

#ifdef HAVE_ATOMIC

inline
size_t orwl_count_inc(orwl_count* counter) {
  size_t ret = atomic_fetch_add(&counter->overl.large, (size_t)1);
  return ret;
}

#ifndef HAVE_FUTEX

inline
size_t orwl_count_dec(orwl_count* counter) {
  /* if we are the last notify the waiters */
  size_t val = atomic_fetch_sub(&counter->overl.large, (size_t)1);
  if (P99_UNLIKELY(val == 1))
    MUTUAL_EXCLUDE(counter->mut)
      pthread_cond_broadcast(&counter->cnd);
  return val - 1;
}

inline
int orwl_count_wait(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    while (atomic_load(&counter->overl.large)) pthread_cond_wait(&counter->cnd, &counter->mut);
  }
  return 0;
}

# else
inline
size_t orwl_count_dec(orwl_count* counter) {
  /* if we are the last notify the waiters */
  register int val = atomic_fetch_sub(&counter->overl.large, (size_t)1);
  if (P99_UNLIKELY(val == 1))
    orwl_futex_broadcast((int*)&(counter->overl.narrow));
  return val - 1;
}

inline
int orwl_count_wait(orwl_count* counter) {
  int ret = 0;
  while (true) {
    if (!atomic_load(&counter->overl.large)) break;
    /* futexes unfortunately only work on int */
    /* we chose the least significant one to be sure to capture an
       intermittent change of the value */
    ret = orwl_futex_wait((int*)&(counter->overl.narrow), 0);
    if (ret) break;
    /* Check if really the whole counter is 0, so we iterate. */
  }
  return ret;
}
#endif
#else

inline
size_t orwl_count_inc(orwl_count* counter) {
  size_t ret = 0;
  MUTUAL_EXCLUDE(counter->mut) {
    ret = counter->overl.large;
    counter->overl.large = ret + 1;
  }
  return ret;
}

inline
size_t orwl_count_dec(orwl_count* counter) {
  size_t ret = 0;
  MUTUAL_EXCLUDE(counter->mut) {
    ret = counter->overl.large;
    counter->overl.large = ret - 1;
    if (!(counter->overl.large)) pthread_cond_broadcast(&counter->cnd);
  }
  return ret;
}

inline
int orwl_count_wait(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    while (counter->overl.large) pthread_cond_wait(&counter->cnd, &counter->mut);
  }
  return 0;
}
#endif


#endif
