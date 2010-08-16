/*
** semaphore_nointr.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri Jul  9 18:23:06 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "semaphore_nointr.h"

int sem_wait_nointr(sem_t *sem);
int sem_trywait_nointr(sem_t *sem);
#if _XOPEN_SOURCE >= 600
int sem_timedwait_nointr(sem_t *sem, const struct timespec *abs_timeout);
#endif
int sem_getvalue0(sem_t* p, unsigned* val);
unsigned sem_assert(sem_t* p);
