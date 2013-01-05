/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2013 Pierre-Nicolas Clauss                                  */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_ARITH_H_
# define    P99_ARITH_H_

/**
 ** @file
 ** @brief Branchless arithmetic and bit twiddling snippets.
 **/

#include "p99_int.h"

#define P00_HAS_ARITH_SHIFT (((-1) >> 1) == (-1))

#if P00_HAS_ARITH_SHIFT
# define P00_ARITH_SHIFT(X, N) ((X) >> (N))
#else
# define P00_ARITH_SHIFT(X, N) P99_IF_ELSE(P99_SIGNED(X))(((X) >> (N)) | ~((((X) & (1 << (P99_EWIDTH(X) - (N)))) << 1) - 1))((X) >> (N))
#endif      /* !P00_HAS_ARITH_SHIFT */

static p99_inline size_t p00_arith_abs(ssize_t a) {
#ifndef P99_2COMPLEMENT
   register ssize_t mask = P00_ARITH_SHIFT(a, P99_EWIDTH(a) - 1);

   return (size_t)((a ^ mask) - mask);
#else
   return (size_t)(a < 0 ? -a : a);
#endif      /* !P99_2COMPLEMENT */
}
#define p99_arith_abs(X) (P99_SIGNED(X) ? p00_arith_abs(X) : (size_t)(X))

static p99_inline ssize_t p99_arith_min(ssize_t a, ssize_t b) {
#ifdef P99_2COMPLEMENT
   a = a - b;
   return (a & P00_ARITH_SHIFT(a, P99_EWIDTH(a) - 1)) + b;
#else
   return a < b ? a : b;
#endif      /* !P99_2COMPLEMENT */
}

static p99_inline ssize_t p99_arith_max(ssize_t a, ssize_t b) {
#ifdef P99_2COMPLEMENT
   a = a - b;
   return (a & ~P00_ARITH_SHIFT(a, P99_EWIDTH(a) - 1)) + b;
#else
   return a < b ? b : a;
#endif      /* !P99_2COMPLEMENT */
}

static p99_inline size_t p99_arith_prev_pow2(size_t a) {
   /* Any decent compiler will unroll this loop */
   for(size_t shift = 1; shift <= P99_EWIDTH(a) >> 1; shift <<= 1) {
      a |= a >> shift;
   }
   return a - (a >> 1);
}

static p99_inline size_t p99_arith_next_pow2(size_t a) {
   --a;
   /* Any decent compiler will unroll this loop */
   for(size_t shift = 1; shift <= P99_EWIDTH(a) >> 1; shift <<= 1) {
      a |= a >> shift;
   }
   return a + 1;
}

/**
 ** @brief Computes the floored base-2 logarithm.
 **/
static p99_inline size_t p99_arith_log2(size_t a) {
   size_t log = 0;

   /* Any decent compiler will unroll this loop */
   for(size_t shift = 1 << sizeof(a), mask  = (1 << (P99_EWIDTH(a) >> 1)) - 1; shift > 1; shift >>= 1, mask >>= shift) {
      if(a > mask) {
         a   >>= shift;
         log  |= shift;
      }
   }
   return log | (a >> 1);
}

#endif      /* !P99_ARITH_H_ */
