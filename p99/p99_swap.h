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
/*
** p99_swap.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Thu Oct 21 11:40:09 2010 Jens Gustedt
** Last update Thu Oct 21 11:40:09 2010 Jens Gustedt
*/

#ifndef   	P99_SWAP_H_
# define   	P99_SWAP_H_

#include "p99_c99.h"
#include "p99_int.h"

p99_inline
void p00_swap2(void* p0, void* p1, size_t pSize, void* t0, void* t1) {
  memcpy(t0, p0, pSize);
  memcpy(t1, p1, pSize);
  memcpy(p1, t0, pSize);
  memcpy(p0, t1, pSize);
}

/**
 ** @brief Swap the contents of the arguments.
 **
 ** This uses two temporary object, allocated on the heap.
 ** @see P99_SWAP for a macro that is generally better adapted, since
 ** all allocation is done on the stack.
 ** @see p99_swap1
 **/
p99_inline
void p99_swap2(void* p0, void* p1, size_t pSize) {
  char* t0 = malloc(2 * pSize);
  char* t1 = t0 + pSize;
  p00_swap2(p0, p1, pSize, t0, t1);
  free(t0);
}

p99_inline
void p00_swap1(void* p0, void* p1, size_t pSize, void* t0) {
  memcpy(t0, p0, pSize);
  memcpy(p0, p1, pSize);
  memcpy(p1, t0, pSize);
}

/**
 ** @brief Swap the contents of the arguments.
 **
 ** This uses one temporary object, allocated on the heap.
 ** @see P99_SWAP for a macro that is generally better adapted, since
 ** all allocation is done on the stack.
 ** @see p99_swap2
 **/
p99_inline
void p99_swap1(void* p0, void* p1, size_t pSize) {
  void* t0 = malloc(pSize);
  p00_swap1(p0, p1, pSize, t0);
  free(t0);
}

#define P00_SWAP2(_0, _1)                                       \
p00_swap2(                                                      \
          /* check if the two are assignment compatible */      \
          P99_SIGN_PROMOTE(&(_0), ((_0) = (_1), NULL)),         \
          P99_SIGN_PROMOTE(&(_1), ((_1) = (_0), NULL)),         \
          sizeof(_0),                                           \
          /* only works if sizeof(_0) >= sizeof(_1) */          \
          (char[sizeof(_0)]){                                   \
            [(intmax_t)sizeof(_0) - sizeof(_1)] = 0,            \
              },                                                \
          /* only works if sizeof(_0) <= sizeof(_1) */          \
          (char[sizeof(_0)]){                                   \
            [(intmax_t)sizeof(_1) - sizeof(_0)] = 0,            \
              })

#define P00_SWAP1(_0, _1)                                       \
p00_swap1(                                                      \
          /* check if the two are assignment compatible */      \
          P99_SIGN_PROMOTE(&(_0), ((_0) = (_1), NULL)),         \
          P99_SIGN_PROMOTE(&(_1), ((_1) = (_0), NULL)),         \
          sizeof(_0),                                           \
          /* only works if sizeof(_0) <= sizeof(_1) */          \
          (char[sizeof(_0)]){                                   \
            [(intmax_t)sizeof(_1) - sizeof(_0)] = 0,            \
              })


/**
 ** @brief Swap the contents of the arguments.
 **
 ** @warning Both arguments must be lvalue expressions @b and it must
 ** be permitted to take their addresses. So variables that have the
 ** @c register storage class will not work with this macro.
 **
 ** @warning The types of the arguments are only checked that they
 ** have the same size @em and that the two lvalues are assignment
 ** compatible. If both are of the same size @em and are assignment
 ** compatible @em and have different semantics, you are in
 ** trouble. This may e.g happen with a @c double and a @c uint64_t.
 **
 ** This macro has the following properties:
 **  - It evaluates each of @a _0 and @a _1 only once.
 **  - It has a compile time check for the correct sizes.
 **  - It has no naming issue with a hidden variable.
 **  - The size of the temporary variable is computed at compile
 **    time, so the compound literal is not a dynamic array.
 **
 ** @remark This implementation is a follow up of a <a
 ** href="http://stackoverflow.com/questions/3982348/implement-generic-swap-macro-in-c/3983089#3983089">
 ** discussion on stackoverflow</a>.
 **
 ** @remark The concrete implementation separates into two different
 ** cases: @c P00_SWAP2(_0, _1), for small sizes, uses two
 ** temporaries in the hope that the loads into registers can be done
 ** simultaneously. The other one, @c P00_SWAP1(_0, _1), uses only one
 ** temporary but is then forced to do serialized copy operations.
 **/
#define P99_SWAP(_0, _1) ((sizeof(_0) > sizeof(uintmax_t)) ? P00_SWAP1(_0, _1) : P00_SWAP2(_0, _1))


#endif 	    /* !P99_SWAP_H_ */