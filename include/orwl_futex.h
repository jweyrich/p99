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
#ifndef ORWL_FUTEX_H
#define ORWL_FUTEX_H

#include "orwl_once.h"
#include "orwl_atomic.h"
#include "p99_posix_default.h"

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
 ** @see ORWL_FUTEX_WAIT
 ** @see orwl_futex_wake
 ** @see orwl_futex_wait_once
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
               *timeout, /*!< a time out for wait, unused by ORWL, defaults to
                            0. */
               int *uaddr2, /*!< unused by ORWL, defaults to 0 */
               int val3     /*!< unused by ORWL, defaults to 0 */
              ) {
  return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}

#define orwl_futex(...) P99_CALL_DEFARG(orwl_futex, 6, __VA_ARGS__)
#define orwl_futex_defarg_3() ((void*)0)
#define orwl_futex_defarg_4() ((void*)0)
#define orwl_futex_defarg_5() 0

/**
 ** @brief Wait until a wake up event is received on address @a uaddr.
 **
 ** This will return under three different circumstances:
 ** - the value @c (*uaddr) is initially not equal to @a val
 ** - a wake up event is triggered by some other task or process
 ** - an interrupt is received.
 **
 ** This later type of event is called a "spurious wakeup".
 **
 ** In any of these cases it is up to the application to check for the
 ** condition that it wanted to be satisfied while waiting.
 **
 ** @warning Generally this is not the function that you'd want to use
 ** directly in an application.
 ** @see ORWL_FUTEX_WAIT for a macro that is better suited for an
 ** application usage.
 ** @see orwl_futex_wake
 ** @see orwl_futex_signal
 ** @see orwl_futex_broadcast
 **/
inline
int orwl_futex_wait_once(int* uaddr, int val) {
  int ret = orwl_futex(uaddr, FUTEX_WAIT, val);
  if (P99_UNLIKELY(ret < 0)) {
    ret = errno;
    errno = 0;
  }
  // Allow for different val or spurious wake ups
  switch (ret) {
  case EWOULDBLOCK: ;
  case EINTR: ret = 0;
  }
  return ret;
}

/**
 ** @brief Wait until the value to which @a ADDR points to fulfills
 ** expression @a EXPECTED.
 **
 ** @param ADDR is a valid address that is interpreted as pointing to
 ** an @c int object.
 **
 ** @param NAME must be an identifier. It serves to declare an
 ** internal variable
 ** that will hold @c (*ADDR) and which should be used in expression
 ** @a EXPECTED.
 **
 ** @param EXPECTED is a expression that evaluates @a NAME and that
 ** should not have side effects.
 **
 ** If it is not fulfilled initially, this macro blocks execution of
 ** the calling thread until the thread is woken up @em and @a
 ** EXPECTED is fulfilled. That is if each occurence of @a NAME in @a
 ** EXPECTED is replaced by the current value of @c (*ADDR) and then
 ** the evaluation of the resulting expression returns a truth value.
 **
 ** It is the responsibility of the application to issue the
 ** corresponding calls to wake up potential waiters whenever the
 ** value of @c (*ADDR) is changed.
 **
 ** Take the following example of a counter implementation:
 ** @code
 ** enum { why = 42 };
 ** int count = 0;
 **
 ** // some thread waiting for the count to become more than why.
 ** ORWL_FUTEX_WAIT(&count, x, x > why);
 **
 ** // some thread augmenting the count. wake up potential
 ** // waiters.
 ** atomic_fetch_add(&count, 1);
 ** orwl_futex_broadcast(&count);
 **
 ** // some thread reducing the count. no need to wake up potential
 ** // waiters.
 ** atomic_fetch_sub(&count, 1);
 ** @endcode
 **
 ** @warning It is probably not a good idea to have the evaluation of
 ** a variable (other than @a NAME) inside expression @a EXPRESSION
 ** that is subject to changes.
 ** @see orwl_futex_wake
 ** @see orwl_futex_signal
 ** @see orwl_futex_broadcast
 **/
#define ORWL_FUTEX_WAIT(ADDR, NAME, EXPECTED)                  \
do {                                                           \
  register int volatile*const p = (int volatile*)(ADDR);       \
  for (;;) {                                                   \
    register int NAME = *p;                                    \
    if (EXPECTED) break;                                       \
    register int ret = orwl_futex_wait_once((int*)p, NAME);    \
    if (P99_UNLIKELY(ret)) {                                   \
      assert(!ret);                                            \
    }                                                          \
  }                                                            \
 } while (false)

/**
 ** @brief Wakeup waiters for address @a uaddr.
 **
 ** If there are @a wakeup waiters, as much waiters are woke up. If
 ** there are less, all waiters are woken up.
 ** @see ORWL_FUTEX_WAIT
 ** @see orwl_futex_signal
 ** @see orwl_futex_broadcast
 **/
inline
int orwl_futex_wake(int* uaddr, int wakeup) {
  int ret = orwl_futex(uaddr, FUTEX_WAKE, wakeup);
  return ret;
}

/**
 ** @brief Wakeup one waiter for address @a uaddr, if there is any.
 ** @see ORWL_FUTEX_WAIT
 ** @see orwl_futex_wake
 ** @see orwl_futex_broadcast
 **/
inline
int orwl_futex_signal(int* uaddr) {
  return orwl_futex_wake(uaddr, 1);
}

/**
 ** @brief Wakeup all waiters for address @a uaddr, if there are
 ** any.
 ** @see ORWL_FUTEX_WAIT
 ** @see orwl_futex_wake
 ** @see orwl_futex_signal
 **/
inline
int orwl_futex_broadcast(int* uaddr) {
  return orwl_futex_wake(uaddr, INT_MAX);
}

#endif

#if (defined(ATOMIC_OPS) && ATOMIC_OPS > 0) || (defined(__GNUC__) && (!defined(GNUC_NO_SYNC) || defined(GNUC_SYNC_REPLACE)))
# define HAVE_ATOMIC
#endif


#endif
