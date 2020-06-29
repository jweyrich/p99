/* This may look like nonsense, but it really is -*- mode: C; coding: utf-8 -*- */
/*                                                                              */
/* Except for parts copied from previous work and as explicitly stated below,   */
/* the author and copyright holder for this work is                             */
/* (C) copyright  2016 Jens Gustedt, INRIA, France                              */
/*                                                                              */
/* This file is free software; it is part of the P99 project.                   */
/*                                                                              */
/* Licensed under the Apache License, Version 2.0 (the "License");              */
/* you may not use this file except in compliance with the License.             */
/* You may obtain a copy of the License at                                      */
/*                                                                              */
/*     http://www.apache.org/licenses/LICENSE-2.0                               */
/*                                                                              */
/* Unless required by applicable law or agreed to in writing, software          */
/* distributed under the License is distributed on an "AS IS" BASIS,            */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     */
/* See the License for the specific language governing permissions and          */
/* limitations under the License.                                               */
/*                                                                              */
#ifndef P99_CM_H
#define P99_CM_H

#include "p99_futex.h"
#include "p99_enum.h"

/**
 ** @addtogroup futex
 ** @{
 **/

/**
 ** @brief A simple condition-mutex data structure.
 **
 ** This combines elements of standard mtx_t and cnd_t into one simple
 ** lock. Namely it implements three basic control interfaces
 ** ::p99_cm_lock, ::p99_cm_unlock and ::p99_cm_wait.
 **
 ** There is no separate interface that would correspond to
 ** ::cnd_signal or ::cnd_broadcast, because the wake up functionality
 ** is already integrated into ::p99_cm_unlock.
 **
 ** This is meant to be simple or even simplistic:
 **
 **  - There are no variants of this that would correspond to
 **    recursive or timed locks.
 **
 **  - None of the blocking states can be canceled.
 **
 **  - Ownership of the lock is not tracked.
 **    - If a thread calls ::p99_cm_unlock while another thread
 **      effectively holds the lock and relies on the fact that he is
 **      alone in a critical section, bad things can happen.
 **    - If the thread that was holding the lock has exited, ::p99_cm_unlock can be
 **      used to unblock the condition-mutex.
 **
 ** @see p99_cm_lock
 ** @see p99_cm_unlock
 ** @see p99_cm_trylock
 ** @see p99_cm_wait
 ** @see P99_CM_EXCLUDE
 **/
P99_DECLARE_STRUCT(p99_cm);

struct p99_cm {
  p99_futex p00_c;
  p99_futex p00_m;
  size_t waiters;
};

p99_inline
bool p00_cm_islocked(unsigned p00_act) {
  return p00_act & 1u;
}

p99_inline
bool p00_cm_isntlocked(unsigned p00_act) {
  return ~p00_act & 1u;
}

/**
 ** @brief Initialize a cm
 ** @related p99_cm
 **/
p99_inline
p99_cm* p99_cm_init(p99_cm* p00_cm) {
  if (p00_cm) {
    *p00_cm = (p99_cm) { .waiters = 0, };
    p99_futex_init(&p00_cm->p00_c, 0u);
    p99_futex_init(&p00_cm->p00_m, 0u);
  }
  return p00_cm;
}

/**
 ** @brief destroy a cm
 ** @related p99_cm
 **/
p99_inline
void p99_cm_destroy(p99_cm* p00_cm) {
  p99_futex_destroy(&p00_cm->p00_c);
  p99_futex_destroy(&p00_cm->p00_m);
}

/**
 ** @brief Initialize an ::p99_cm object.
 **/
# define P99_CM_INITIALIZER P99_FUTEX_INITIALIZER(p99_cm_unlocked)

enum { p00_cm_unlocked, p00_cm_locked, };

/* This is only for internal use. Other than @c cnd_signal this
   supposes that the corresponding mutex part of the condition-mutex
   is locked. */
p99_inline
void p00_cm_signal(p99_cm volatile* p00_cm) {
  if (p00_cm->waiters) {
    /* In most of the cases the waiter will already be in the kernel
       wait when we arrive here: this thread has acquired the lock,
       done whatever processing and now is releasing the
       lock. Nevertheless there is a small window, where we may have
       doubled a waiter thread. Therefore we force a busy wait if
       necessary, until we have woken up someone. */
    p99_futex_wakeup(&p00_cm->p00_c, 1u, 1u);
    --p00_cm->waiters;
  }
}

p99_inline
void p00_cm_unlock(p99_cm volatile* p00_cm) {
  p99_futex_exchange(&p00_cm->p00_m, p00_cm_unlocked, p00_cm_unlocked, 1u, 0u, 1u);
}

/**
 ** @brief Unconditionally unlock @a p00_cm and wake up a waiter, if
 ** any.
 **
 ** This has a similar effect as would have a combination ::cnd_signal
 ** and ::mtx_unlock for a standard mutex. That is, one thread, if
 ** any, that is either waiting in ::p99_cm_lock or ::p99_cm_wait is
 ** woken up and acquires the lock.
 **
 ** @related p99_cm
 **/
p99_inline
void p99_cm_unlock(p99_cm volatile* p00_cm) {
  p00_cm_signal(p00_cm);
  p00_cm_unlock(p00_cm);
}

/**
 ** @brief Acquire @a p00_cm.
 **
 ** If @a p00_cm is currently held this function is is blocking.
 **
 ** @warning If @a p00_cm is already held by the same thread calling
 ** this function results in a deadlock of the thread.
 **
 ** @related p99_cm
 **/
p99_inline
void p99_cm_lock(p99_cm volatile* p00_cm) {
  P99_FUTEX_COMPARE_EXCHANGE(&p00_cm->p00_m, p00_act,
                             // wait while locked by another thread
                             p00_act == p00_cm_unlocked,
                             p00_cm_locked,
                             // never wake up anyone
                             0u, 0u);
}

/**
 ** @brief Acquire @a p00_cm.
 **
 ** If @a is currently held this function is is blocking.
 **
 ** @related p99_cm
 **/
p99_inline
bool p99_cm_trylock(p99_cm volatile* p00_cm) {
  return p99_futex_exchange(&p00_cm->p00_m, p00_cm_locked, 0u, 0u, 0u, 0u);
}

/**
 ** @brief Wait until another thread successfully locks and unlocks @a
 ** p00_cm.
 **
 ** @related p99_cm
 **/
P00_FUTEX_INLINE(p99_cm_wait)
void p99_cm_wait(p99_cm volatile* p00_cm) {
  ++p00_cm->waiters;
  p00_cm_unlock(p00_cm);
  p99_futex_wait(&p00_cm->p00_c);
  p99_cm_lock(p00_cm);
}

/**
 ** @brief Protect the following block or statement as a critical
 ** section of the program by using @a CMP as a condition-mutex.
 **
 ** @param CMP is an expression that evaluates to a pointer to
 ** ::p99_cm. The cm is used as a condition-mutex, so this is an
 ** inactive wait.
 **
 ** @remark @a CMP is only evaluated once at the beginning, so it
 ** would be safe to change it in the depending block or statement.
 **
 ** @warning Such a section should not contain preliminary exits such
 ** as @c goto, @c break, @c return, @c longjmp, or ::P99_UNWIND etc.
 **
 ** @see P99_MUTUAL_EXCLUDE that uses a standard ::mtx_t
 **/
#define P99_CM_EXCLUDE(CMP)   P00_CM_EXCLUDE(CMP, P99_UNIQ(cm))

#define P00_CM_EXCLUDE(CMP, ID)                                \
P00_BLK_START                                                  \
P00_BLK_DECL(register p99_cm volatile*const, ID, (CMP))        \
P00_BLK_BEFAFT(p99_cm_lock(ID),                                \
               p99_cm_unlock(ID))                              \
P00_BLK_END



/**
 ** @}
 **/


#endif
