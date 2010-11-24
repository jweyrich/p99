/*
** orwl_barrier.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Nov 24 23:35:18 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#if !defined(POSIX_BARRIERS) || (POSIX_BARRIERS <= 0)

#include "orwl_posix.h"
#include "orwl_once.h"

int orwl_barrier_destroy(orwl_barrier* barrier) {
  int ret = 0;
  MUTUAL_EXCLUDE(barrier->mut) {
    if (barrier->inside) ret = EBUSY;
    else barrier->outside = 0;
  }
  if (ret) return ret;
  ret = pthread_mutex_destroy(&barrier->mut);
  if (ret) return ret;
  ret = pthread_cond_destroy(&barrier->enter);
  if (ret) return ret;
  ret = pthread_cond_destroy(&barrier->leave);
  return ret;
}

int orwl_barrier_init(orwl_barrier* barrier, unsigned count) {
  int ret = pthread_mutex_init(&barrier->mut, NULL);
  if (ret) return ret;
  ret = pthread_cond_init(&barrier->enter, NULL);
  if (ret) return ret;
  ret = pthread_cond_init(&barrier->leave, NULL);
  if (ret) return ret;
  barrier->outside = count;
  barrier->inside = 0;
  return ret;
}

int orwl_barrier_wait(orwl_barrier* barrier) {
  int ret = 0;
  MUTUAL_EXCLUDE(barrier->mut) {
    --(barrier->outside);
    if (!barrier->outside) {
      /* this is the last thread that enters the barrier */
      pthread_cond_broadcast(&barrier->enter);
    } else {
      pthread_cond_wait(&barrier->enter, &barrier->mut);
    }
    ++(barrier->inside);
  }
  MUTUAL_EXCLUDE(barrier->mut) {
    --(barrier->inside);
    if (!barrier->inside) {
      /* this is the last thread that leaves the barrier */
      pthread_cond_broadcast(&barrier->leave);
      /* this one will have a head start before the others, so he
         should do any specific task, if necessary. */
      ret = PTHREAD_BARRIER_SERIAL_THREAD;
    } else {
      pthread_cond_wait(&barrier->leave, &barrier->mut);
    }
    ++(barrier->outside);
  }
  return ret;
}

#endif /* POSIX_BARRIER */
