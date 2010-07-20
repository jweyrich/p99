/*
** semaphore_nointr.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri Jul  9 18:18:19 2010 Jens Gustedt
** Last update Fri Jul  9 18:18:19 2010 Jens Gustedt
*/

#ifndef   	SEMAPHORE_NOINTR_H_
# define   	SEMAPHORE_NOINTR_H_

#include <semaphore.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include "p99_compiler.h"



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
