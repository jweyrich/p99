/*
** orwl_barrier.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Nov 24 23:17:01 2010 Jens Gustedt
** Last update Wed Nov 24 23:17:01 2010 Jens Gustedt
*/

/**
 ** @file
 ** @brief Provide a fallback implementation for barriers.
 **/

#ifndef   	ORWL_BARRIER_H_
# define   	ORWL_BARRIER_H_

#if defined(POSIX_BARRIERS) && (POSIX_BARRIERS > 0)

#include <pthread.h>

typedef pthread_barrier_t orwl_barrier;

static_inline
int orwl_barrier_destroy(orwl_barrier* barrier) {
  return pthread_barrier_destroy(barrier);
}

static_inline
int orwl_barrier_init(orwl_barrier* barrier, unsigned count) {
  return pthread_barrier_init(barrier, NULL, count);
}

static_inline
int orwl_barrier_wait(orwl_barrier* barrier) {
  return pthread_barrier_wait(barrier);
}

#else

P99_DECLARE_STRUCT(orwl_barrier);

/**
 ** @brief a simple replacement type for @c pthread_barrier_t in case
 ** that it is not available.
 **/
struct orwl_barrier {
  pthread_mutex_t mut;
  pthread_cond_t enter;
  pthread_cond_t leave;
  unsigned outside;
  unsigned inside;
};

#ifndef PTHREAD_BARRIER_SERIAL_THREAD
enum { PTHREAD_BARRIER_SERIAL_THREAD = INT_MAX };
#endif

int orwl_barrier_destroy(orwl_barrier* barrier);
int orwl_barrier_init(orwl_barrier* barrier, unsigned count);
int orwl_barrier_wait(orwl_barrier* barrier);

#endif


#endif 	    /* !ORWL_BARRIER_H_ */
