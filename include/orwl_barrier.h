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

inline
int orwl_barrier_destroy(orwl_barrier* barrier) {
  return pthread_barrier_destroy(barrier);
}

inline
int orwl_barrier_init(orwl_barrier* barrier, unsigned count) {
  return pthread_barrier_init(barrier, NULL, count);
}

inline
int orwl_barrier_wait(orwl_barrier* barrier) {
  return pthread_barrier_wait(barrier);
}

#else

P99_DECLARE_STRUCT(orwl_barrier);

/** @struct orwl_barrier
 ** @brief a simple fallback type for @c pthread_barrier_t
 **
 ** In case that @c pthread_barrier_t is not available a homebrew
 ** replacement is used.
 **/
#ifdef DOXYGEN
struct orwl_barrier;
#else
struct orwl_barrier {
  pthread_mutex_t mut;
  pthread_cond_t enter;
  pthread_cond_t leave;
  unsigned outside;
  unsigned inside;
};
#endif

#ifndef PTHREAD_BARRIER_SERIAL_THREAD
enum { PTHREAD_BARRIER_SERIAL_THREAD = INT_MAX };
#endif

/** @memberof orwl_barrier **/
int orwl_barrier_destroy(orwl_barrier* barrier);
/** @memberof orwl_barrier **/
int orwl_barrier_init(orwl_barrier* barrier, unsigned count);
/** @memberof orwl_barrier **/
int orwl_barrier_wait(orwl_barrier* barrier);

#endif


#endif 	    /* !ORWL_BARRIER_H_ */
