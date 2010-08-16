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

#include "orwl_posix.h"
#include "p99_id.h"
#include "orwl_thread.h"
#include "orwl_enum.h"


DECLARE_STRUCT(rand48_t);

uint64_t orwl_mix(uint64_t a, uint64_t b);

uint64_t orwl_challenge(uint64_t a);

/**
 ** @brief Get the micro seconds since epoch.
 **/
inline
uint64_t useconds(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  uint64_t ret = t.tv_sec;
  ret *= 1000000;
  ret += t.tv_usec;
  return ret;
}


/**
 ** @brief A seed class that is compatible with the @c rand48 family
 ** of pseudo random generators
 **
 ** @see seed_get
 **/
struct rand48_t {
  unsigned short x[3];
};

#define RAND48_T_INITIALIZER { { useconds(), getpid(), pthread_self() } }

#ifndef DOXYGEN
inline
P99_PROTOTYPE(rand48_t *, rand48_t_init, rand48_t*, unsigned short, unsigned short, unsigned short);
#define rand48_t_init(...) P99_CALL_DEFARG(rand48_t_init, 4, __VA_ARGS__)
P99_DECLARE_DEFARG(rand48_t_init, , useconds(), getpid(), pthread_self());
#endif

DOCUMENT_INIT(rand48_t)
P99_DEFARG_DOCU(rand48_t)
inline
rand48_t *rand48_t_init(rand48_t *seed,     /*!< [out] the object to iniialize */
                        unsigned short x0,  /*!< [in] defaults to a time value */
                        unsigned short x1,  /*!< [in] defaults to the process id */
                        unsigned short x2   /*!< [in] defaults to the thread id */
                        ) {
  if (!seed) return NULL;
  seed->x[0] = x0;
  seed->x[1] = x1;
  seed->x[2] = x2;
  return seed;
}

inline
void rand48_t_destroy(rand48_t* seed){
  /* empty */
}

DECLARE_NEW_DELETE(rand48_t);

/**
 ** @brief Give access to a seed variable that is specific to each
 ** thread.
 **/
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
