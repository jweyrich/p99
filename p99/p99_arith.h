/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                            */
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

p99_inline intmax_t p99_arith_min(intmax_t p00_a, intmax_t p00_b) {
#if UINTMAX_MAX > INTMAX_MAX
  p00_a -= p00_b;
  return (p00_a & P00_ARITH_INTMAX_SHIFT(p00_a)) + p00_b;
#else
  return p00_a < p00_b ? p00_a : p00_b;
#endif
}

p99_inline intmax_t p99_arith_max(intmax_t p00_a, intmax_t p00_b) {
#if UINTMAX_MAX > INTMAX_MAX
  p00_a -= p00_b;
  return (p00_a & ~P00_ARITH_INTMAX_SHIFT(p00_a)) + p00_b;
#else
  return p00_a < p00_b ? p00_b : p00_a;
#endif
}

p99_inline uintmax_t p99_arith_prev_pow2(uintmax_t p00_a) {
  /* Any decent compiler will unroll this loop */
  for(uintmax_t p00_s = 1; p00_s <= P99_EWIDTH(p00_a) >> 1; p00_s <<= 1) {
    p00_a |= p00_a >> p00_s;
  }
  return p00_a - (p00_a >> 1);
}

p99_inline uintmax_t p99_arith_next_pow2(uintmax_t p00_a) {
  --p00_a;
  /* Any decent compiler will unroll this loop */
  for(uintmax_t p00_s = 1; p00_s <= P99_EWIDTH(p00_a) >> 1; p00_s <<= 1) {
    p00_a |= p00_a >> p00_s;
  }
  return p00_a + 1;
}

#if P99_UINTMAX_WIDTH >= 64
# define P00_UINTMAX_WIDTH_HBIT 7
#else
# define P00_UINTMAX_WIDTH_HBIT 6
#endif

/**
 ** @brief Computes the floored base-2 logarithm.
 **/
p99_inline uintmax_t p99_arith_log2(uintmax_t p00_a) {
  uintmax_t p00_l = 0;

  /* Any decent compiler will unroll this loop */
  for(int i = P00_UINTMAX_WIDTH_HBIT - 2; i >= 0; --i) {
    uintmax_t p00_s = UINTMAX_C(1) << i;
    uintmax_t p00_m = ((UINTMAX_C(1) << p00_s) - 1) << p00_s;
    uintmax_t p00_t = (p00_a & p00_m) != 0;

    p00_l  |= p00_t * p00_s;
    p00_a   >>= p00_t * p00_s;
  }
  return p00_l;
}

#endif      /* !P99_ARITH_H_ */
