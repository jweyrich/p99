/*
** orwl_rand.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Mar 22 23:07:05 2010 Jens Gustedt
** Last update Mon Mar 22 23:07:05 2010 Jens Gustedt
*/

#ifndef   	ORWL_RAND_H_
# define   	ORWL_RAND_H_

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "orwl_int.h"
#include "orwl_thread.h"

uint64_t orwl_mix(uint64_t a, uint64_t b);

uint64_t orwl_challenge(uint64_t a);

inline
uint64_t useconds(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  uint64_t ret = t.tv_sec;
  ret *= 1000000;
  ret += t.tv_usec;
  return ret;
}


struct rand48_t {
  unsigned short x[3];
};

#ifndef __cplusplus
typedef struct rand48_t rand48_t;
#endif

#define RAND48_T_INITIALIZER { { useconds(), getpid(), pthread_self() } }

inline
rand48_t *rand48_t_init(rand48_t *seed, unsigned short x0, unsigned short x1, unsigned short x2) {
  seed->x[0] = x0;
  seed->x[1] = x1;
  seed->x[2] = x2;
  return seed;
}

inline
PROTOTYPE(rand48_t *, rand48_t_init, rand48_t*, unsigned short, unsigned short, unsigned short);
#define rand48_t_init(...) CALL_WITH_DEFAULTS(rand48_t_init, 4, __VA_ARGS__)
DECLARE_DEFARG(rand48_t_init, , useconds(), getpid(), pthread_self());

inline
void rand48_t_destroy(rand48_t *seed){
  /* empty */
}

DECLARE_NEW_DELETE(rand48_t);

/** @brief Give access to a seed variable that is specific to each
    thread. */
DECLARE_THREAD_VAR(rand48_t, seed_get);


inline
uint32_t orwl_rand(rand48_t *xsubi) {
  return jrand48(xsubi->x);
}

inline
double orwl_drand(rand48_t *xsubi) {
  return erand48(xsubi->x);
}

inline
uint64_t orwl_rand64(rand48_t *xsubi) {
  double ret = ldexp(orwl_drand(xsubi), 48);
  return (uint64_t)ret;
}


#endif 	    /* !ORWL_RAND_H_ */
