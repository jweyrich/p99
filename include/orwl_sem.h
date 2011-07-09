/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
/*
** orwl_sem.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri Nov 19 16:26:43 2010 Jens Gustedt
** Last update Fri Nov 19 16:26:43 2010 Jens Gustedt
*/

#ifndef   	ORWL_SEM_H_
# define   	ORWL_SEM_H_

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#endif
#ifndef POSIX_SEMAPHORES
# warning "No value for POSIX_SEMAPHORES found, assuming 200112L"
# define POSIX_SEMAPHORES 200112L
#endif

#include "p99_c99.h"
#include "p99_type.h"
#include "p99_int.h"
#include <pthread.h>

P99_DECLARE_STRUCT(orwl_sem);

/**
 ** @struct orwl_sem "orwl_sem.h"
 ** @brief A semaphore implementation close to POSIX unnamed semaphores
 **
 ** This implementation here deviates from the POSIX analogs in the
 ** following points:
 ** - orwl_sem_getvalue will never return a negative value, so a
 **   return value of @c 0 indicates that there @em might be waiters.
 ** - None of the functions will return EINTR. In case that one of the
 **   underlying primitives encounters such an error condition, it
 **   will simply be relaunched.
 ** - They may only be shared between threads and not between
 **   different processes.
 **
 **
 ** If POSIX unnamed semaphores are available this is just a wrapper
 ** around @c sem_t. Otherwise a stub implementation using only @c
 ** pthread_mutex_t and @c pthread_cond_t is provided. The stub will
 ** only be used if ::SEM_PROBLEM is defined at compile time.
 **/

/**
 ** @def ORWL_SEM_VALUE_MAX
 ** @brief Maximum value that an ::orwl_sem may carry before an overflow
 ** occurs.
 **
 ** Analogous to @c SEM_VALUE_MAX.
 **/

/** @brief Destroy the semaphore **/
/** @memberof orwl_sem **/
inline int    orwl_sem_destroy(orwl_sem *);
/** @brief Get the value of the semaphore a one point before
    returning from this functions **/
/** @memberof orwl_sem **/
inline int    orwl_sem_getvalue(orwl_sem *, int *);
/** @brief Iniitalize the semaphore **/
/** @memberof orwl_sem **/
inline int    orwl_sem_init(orwl_sem *, unsigned int);
/** @brief Increment the semaphore **/
/** @memberof orwl_sem **/
inline int    orwl_sem_post(orwl_sem *);
/** @brief Test and decrement the semaphore **/
/** @memberof orwl_sem **/
inline int    orwl_sem_trywait(orwl_sem *);
/** @brief Wait for a token to become available on the semaphore **/
/** @memberof orwl_sem **/
inline int    orwl_sem_wait(orwl_sem *);

/** @memberof orwl_sem **/
/** @see sem_assert **/
inline unsigned orwl_sem_assert(orwl_sem* s);

#ifndef DOXYGEN
#if POSIX_SEMAPHORES < 199800L

#define ORWL_SEM_VALUE_MAX INT_MAX

struct orwl_sem { unsigned val; pthread_mutex_t mut; pthread_cond_t cond; };

inline int orwl_sem_destroy(orwl_sem *s) {
  if (!s) return EINVAL;
  int ret = pthread_cond_destroy(&s->cond);
  if (ret) return ret;
  ret = pthread_mutex_destroy(&s->mut);
  if (ret) return ret;
  s->val = 0;
  return 0;
}

inline int orwl_sem_getvalue(orwl_sem *s, int *p) {
  if (!s || !p) return EINVAL;
  pthread_mutex_t *mut = &s->mut;
  int ret = pthread_mutex_lock(mut);
  if (ret) return ret;
  *p = s->val;
  ret = pthread_mutex_unlock(mut);
  return ret;
}

inline int orwl_sem_init(orwl_sem *s, unsigned int val) {
  if (!s || val > ORWL_SEM_VALUE_MAX) return EINVAL;
  *s = P99_LVAL(orwl_sem const, .val = val);
  pthread_mutex_init(&s->mut, 0);
  pthread_cond_init(&s->cond, 0),
  return 0;
}

inline int orwl_sem_post(orwl_sem *s) {
  if (!s) return EINVAL;
  pthread_mutex_t *mut = &s->mut;
  int ret = pthread_mutex_lock(mut);
  if (ret) return ret;
  if (s->val == ORWL_SEM_VALUE_MAX) ret = EOVERFLOW;
  else {
    ret = pthread_cond_signal(&s->cond);
    if (!ret) ++(s->val);
  }
  /* If this unlock fails things are not recoverable. */
  int ret2 = pthread_mutex_unlock(mut);
  assert(!ret2);
  return ret;
}

inline int orwl_sem_trywait(orwl_sem *s) {
  if (!s) return EINVAL;
  pthread_mutex_t *mut = &s->mut;
  int ret = pthread_mutex_lock(mut);
  if (ret) return ret;
  if (s->val) --(s->val);
  else ret = EAGAIN;
  /* If this unlock fails things are not recoverable. */
  int ret2 = pthread_mutex_unlock(mut);
  assert(!ret2);
  return ret;
}

inline int orwl_sem_wait(orwl_sem *s) {
  if (!s) return EINVAL;
  pthread_mutex_t *mut = &s->mut;
  int ret = pthread_mutex_lock(mut);
  if (ret) return ret;
  while (!ret && !s->val)
    ret = pthread_cond_wait(&s->cond, mut);
  if (!ret) --(s->val);
  /* unlock should never fail, here. Otherwise somebody has
     reinitialized the mutex while we were waiting */
  int ret2 = pthread_mutex_unlock(mut);
  assert(!ret2);
  return ret;
}


inline unsigned orwl_sem_assert(orwl_sem* s) {
  int ret = 0;
  int err = orwl_sem_getvalue(s, &ret);
  return err ? UINT_MAX : ret;
}

#else

#include "semaphore_nointr.h"

#define ORWL_SEM_VALUE_MAX SEM_VALUE_MAX

struct orwl_sem { sem_t sem; };

inline int    orwl_sem_destroy(orwl_sem *s)                { return sem_destroy(&s->sem); }
inline int    orwl_sem_getvalue(orwl_sem *s, int *p)       { int ret = sem_getvalue(&s->sem, p); if (*p < 0) *p = 0; return ret; }
inline int    orwl_sem_init(orwl_sem *s, unsigned int val) { return sem_init(&s->sem, 0, val); }
inline int    orwl_sem_post(orwl_sem *s)                   { return sem_post(&s->sem); }
inline int    orwl_sem_trywait(orwl_sem *s)                { return sem_trywait_nointr(&s->sem); }
inline int    orwl_sem_wait(orwl_sem *s)                   { return sem_wait_nointr(&s->sem); }

inline unsigned orwl_sem_assert(orwl_sem* s)                  { return sem_assert(&s->sem); }


#endif      /* POSIX_SEMAPHORES */
#endif      /* !DOXYGEN     */
#endif 	    /* !ORWL_SEM_H_ */
