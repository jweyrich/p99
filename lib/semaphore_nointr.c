/* This may look like nonsense, but it really is -*- C -*-                   */
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
#include "semaphore_nointr.h"

int sem_wait_nointr(sem_t *sem);
int sem_trywait_nointr(sem_t *sem);
#if _XOPEN_SOURCE >= 600
int sem_timedwait_nointr(sem_t *sem, const struct timespec *abs_timeout);
#endif
int sem_getvalue0(sem_t* p, unsigned* val);
unsigned sem_assert(sem_t* p);
