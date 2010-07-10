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
#include <errno.h>
#include "orwl_inline.h"



/**
 ** @brief An interrupt safe wrapper for @c sem_wait.
 **
 ** The POSIX function may be interrupted if e.g there is delivery of IO.
 ** This function here catches the case of an interrupt and retries
 ** until success or until another error condition occurs.
 **/
static_inline
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
static_inline
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
static_inline
int sem_timedwait_nointr(sem_t *sem, const struct timespec *abs_timeout) {
  while (sem_timedwait(sem, abs_timeout))
    if (errno == EINTR) errno = 0;
    else return -1;
  return 0;
}


#endif 	    /* !SEMAPHORE_NOINTR_H_ */
