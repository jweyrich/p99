/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef     ORWL_RAND_H_
# define    ORWL_RAND_H_

#include "orwl_thread.h"
#include P99_ADVANCE_ID
#include "p99_enum.h"

DECLARE_ONCE(orwl_rand);


P99_DECLARE_STRUCT(rand48_t);

uint64_t orwl_mix(uint64_t a, uint64_t b);

uint64_t orwl_challenge(uint64_t a);

/**
 ** @brief A seed class that is compatible with the @c rand48 family
 ** of pseudo random generators
 **
 ** @see seed_get
 **/
struct rand48_t {
  unsigned short x[3];
};

#define RAND48_T_INITIALIZER { { (unsigned short)useconds(), (unsigned short)getpid(), (unsigned short)(uintptr_t)pthread_self() } }

#ifndef DOXYGEN
inline
P99_PROTOTYPE(rand48_t *, rand48_t_init, rand48_t*, unsigned short, unsigned short, unsigned short);
#define rand48_t_init(...) P99_CALL_DEFARG(rand48_t_init, 4, __VA_ARGS__)
#define rand48_t_init_defarg_1() ((unsigned short)useconds())
#define rand48_t_init_defarg_2() ((unsigned short)getpid())
#define rand48_t_init_defarg_3() ((unsigned short)(uintptr_t)pthread_self())
#endif

DOCUMENT_INIT(rand48_t)
P99_DEFARG_DOCU(rand48_t)
inline
rand48_t *rand48_t_init(rand48_t *seed,     /*!< [out] the object to iniialize */
                        unsigned short x0,  /*!< [in] defaults to a time value */
                        unsigned short x1,  /*!< [in] defaults to the process id */
                        unsigned short x2   /*!< [in] defaults to the thread id */
                       ) {
  if (seed) {
    *seed = P99_LVAL(rand48_t,
    .x = {
      [0] = x0,
      [1] = x1,
      [2] = x2,
    }
                    );
  }
  return seed;
}

DOCUMENT_DESTROY(rand48_t)
inline
void rand48_t_destroy(rand48_t* seed) {
  /* special care for bogus warning given by icc */
  (void)seed;
}

DECLARE_NEW_DELETE(rand48_t);

/**
 ** @brief Give access to a seed variable that is specific to each
 ** thread.
 **/
DECLARE_THREAD_VAR(rand48_t, seed_get);


/**
 ** @memberof rand48_t
 **/
inline
uint32_t orwl_rand(rand48_t *xsubi) {
  return jrand48(xsubi->x);
}

/**
 ** @memberof rand48_t
 **/
inline
double orwl_drand(rand48_t *xsubi) {
  return erand48(xsubi->x);
}

/**
 ** @memberof rand48_t
 **/
inline
uint64_t orwl_rand64(rand48_t *xsubi) {
  double ret = ldexp(orwl_drand(xsubi), 48);
  return (uint64_t)ret;
}


#endif      /* !ORWL_RAND_H_ */
