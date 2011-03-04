/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	SEMAPHORE_NOINTR_H_
# define   	SEMAPHORE_NOINTR_H_

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#elif _XOPEN_SOURCE < 600
# error "Need at least XOPEN specification 6 to compile this file"
#endif

#ifndef POSIX_SEMAPHORES
# warning "No value for POSIX_SEMAPHORES found, assuming 200112L"
# define POSIX_SEMAPHORES 200112L
#endif
#include "p99_c99.h"
#include <semaphore.h>

/**
 ** @brief An interrupt safe wrapper for @c sem_wait.
 **
 ** The POSIX function may be interrupted if e.g there is delivery of IO.
 ** This function here catches the case of an interrupt and retries
 ** until success or until another error condition occurs.
 **/
inline
int sem_wait_nointr(sem_t *sem) {
  while (sem_wait(sem))
    if (errno == EINTR) errno = 0;
    else return -1;
  return 0;
}

/**
 ** @brief An interrupt safe wrapper for @c sem_trywait.
 **
 ** The POSIX function may be interrupted if e.g there is delivery of IO.
 ** This function here catches the case of an interrupt and retries
 ** until success or until another error condition occurs.
 **/
inline
int sem_trywait_nointr(sem_t *sem) {
  while (sem_trywait(sem))
    if (errno == EINTR) errno = 0;
    else return -1;
  return 0;
}

#if (defined(POSIX_TIMEOUTS) && (POSIX_TIMEOUTS >= 200112L)) || defined(DOXYGEN)

/**
 ** @brief An interrupt safe wrapper for @c sem_timedwait.
 **
 ** The POSIX function may be interrupted if e.g there is delivery of IO.
 ** This function here catches the case of an interrupt and retries
 ** until success or until another error condition occurs.
 **/
inline
int sem_timedwait_nointr(sem_t *sem, const struct timespec *abs_timeout) {
  while (sem_timedwait(sem, abs_timeout))
    if (errno == EINTR) errno = 0;
    else return -1;
  return 0;
}

#endif /* POSIX_TIMEOUTS */

/**
 ** @brief Return the value of the semaphore object in @a val if it is
 ** positive or 0 otherwise.
 **
 ** POSIX allows to return negative values if there are waiters.
 **/
inline
int sem_getvalue0(sem_t* p, unsigned* val) {
  int value;
  int ret = sem_getvalue(p, &value);
  *val = (ret || value < 0) ? 0u : value;
  return ret;
}

/**
 ** @brief Return the value of a sem_t in a form that is suitable for assertions.
 **
 ** If @a p points to a valid semaphore a value in the range from @c 0
 ** ... @c INT_MAX will be returned. Otherwise a value outside that
 ** range is returned. So in particular it might be used in forms as
 ** @code
 ** assert(sem_assert(&mySem) < 3u);
 ** @endcode
 ** This will always work, even if there are waiters for the semaphore
 ** since this case is mapped to a return value of @c 0.
 **/
inline
unsigned sem_assert(sem_t* p) {
  int value;
  int err = sem_getvalue(p, &value);
  unsigned ret = (err
                  ? (unsigned)INT_MIN
                  : ((value < 0)
                     ? 0u
                     : (unsigned)value
                     )
                  );
  return ret;
}



#endif 	    /* !SEMAPHORE_NOINTR_H_ */
