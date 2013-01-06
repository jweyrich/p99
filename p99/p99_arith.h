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

#define P00_UINTMAX_HIGHBIT(X) ((((uintmax_t)+(X)) & (UINTMAX_MAX ^ INTMAX_MAX)) >> (P99_UINTMAX_WIDTH - 1U))

/* The wonders of unsigned types make it act as if it were a signed
   number in 2complement representation. Unsigned types always have
   logical shift so we negate the result to simulate an arithmetic
   shift in that special case.

   To get back to intmax_t even more wonders occur. The result of
   P99_UINTMAX_HIGHBIT is 0 or 1 and so it fits into intmax_t. That
   converted value then can be negated to obtain 0 or -1 as a
   result. */
#define P00_ARITH_INTMAX_SHIFT(X) (-((intmax_t)+P00_UINTMAX_HIGHBIT(X)))

p99_inline uintmax_t p00_arith_abs(uintmax_t p00_a) {
#if UINTMAX_MAX > INTMAX_MAX
  register uintmax_t p00_m = P00_ARITH_INTMAX_SHIFT(p00_a);
  return (p00_m ^ p00_a) - p00_m;
#else
  /* On such a perverted architecture the case of INTMAX_MIN might
     raise a application defined signal. */
  return (p00_a < 0) ? -p00_a : p00_a;
#endif
}
#define p99_arith_abs(X) (P99_SIGNED(X) ? p00_arith_abs(X) : P99_RVAL(uintmax_t, (X)))

p99_inline intmax_t p99_arith_min(intmax_t a, intmax_t b) {
#if UINTMAX_MAX > INTMAX_MAX
   a -= b;
   return (a & P00_ARITH_INTMAX_SHIFT(a)) + b;
#else
   return a < b ? a : b;
#endif
}

p99_inline intmax_t p99_arith_max(intmax_t a, intmax_t b) {
#if UINTMAX_MAX > INTMAX_MAX
   a -= b;
   return (a & ~P00_ARITH_INTMAX_SHIFT(a)) + b;
#else
   return a < b ? b : a;
#endif
}

p99_inline uintmax_t p99_arith_prev_pow2(uintmax_t a) {
   /* Any decent compiler will unroll this loop */
   for(uintmax_t shift = 1; shift <= P99_EWIDTH(a) >> 1; shift <<= 1) {
      a |= a >> shift;
   }
   return a - (a >> 1);
}

p99_inline uintmax_t p99_arith_next_pow2(uintmax_t a) {
   --a;
   /* Any decent compiler will unroll this loop */
   for(uintmax_t shift = 1; shift <= P99_EWIDTH(a) >> 1; shift <<= 1) {
      a |= a >> shift;
   }
   return a + 1;
}

/**
 ** @brief Computes the floored base-2 logarithm.
 **/
static p99_inline uintmax_t p99_arith_log2(uintmax_t a) {
   uintmax_t log = 0;

   /* Any decent compiler will unroll this loop */
   for(int i = ffs(P99_EWIDTH(a)) - 2; i >= 0; --i) {
      uintmax_t shift = P99_PROMOTE_1(shift) << i;
      uintmax_t mask = ((P99_PROMOTE_1(mask) << shift) - 1) << shift;
      uintmax_t test = (a & mask) != 0;

      log  |= test * shift;
      a   >>= test * shift;
   }
   return log;
}

#endif      /* !P99_ARITH_H_ */
