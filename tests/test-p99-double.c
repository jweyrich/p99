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
#include "p99_double.h"

long double const a0 = +0.5L;
long double const a1 = P99_DEC_DOUBLE();
long double const a2 = P99_DEC_DOUBLE(,1);
long double const a3 = P99_DEC_DOUBLE(,,9);
long double const a4 = P99_HEX_DOUBLE(,,9,-,7);
long double const a5 = P99_HEX_DOUBLE();
long double const a6 = P99_HEX_DOUBLE(,1);
long double const a7 = P99_HEX_DOUBLE(,,9);
long double const a8 = P99_HEX_DOUBLE(,,8,-,,L);
long double const a9 = P99_HEX_DOUBLE(-,1,8,-,1);

#include "p99_c99.h"
#include "p99_for.h"

int main(int argc, char **argv) {
  P99_IS_EQ_void();
  P99_IS_EQ_void(void);
  P99_IS_EQ_void(void*);
  P99_IS_EQ_void(void());
  P99_IS_EQ_void(void, void);
  P99_TOK_EQ(void, void);
  P99_TOK_EQ(void, int);
  P99_TOK_EQ(int, int);

  P99_IS_DEC_LT(0, 0);
  P99_IS_DEC_LT(0, 1);
  P99_IS_DEC_LT(0, 2);
  P99_IS_DEC_LT(1, 1);
  P99_IS_DEC_LT(1, 0);
  P99_IS_DEC_LT(1, 2);
  P99_IS_DEC_LT(1, 3);

  P99_IS_DEC_GT(0, 0);
  P99_IS_DEC_GT(0, 1);
  P99_IS_DEC_GT(0, 2);
  P99_IS_DEC_GT(1, 1);
  P99_IS_DEC_GT(1, 0);
  P99_IS_DEC_GT(1, 2);
  P99_IS_DEC_GT(1, 3);

  P99_IS_DEC_GE(0, 0);
  P99_IS_DEC_GE(0, 1);
  P99_IS_DEC_GE(0, 2);
  P99_IS_DEC_GE(1, 1);
  P99_IS_DEC_GE(1, 0);
  P99_IS_DEC_GE(1, 2);
  P99_IS_DEC_GE(1, 3);

  P99_IS_DEC_LE(0, 0);
  P99_IS_DEC_LE(0, 1);
  P99_IS_DEC_LE(0, 2);
  P99_IS_DEC_LE(1, 1);
  P99_IS_DEC_LE(1, 0);
  P99_IS_DEC_LE(1, 2);
  P99_IS_DEC_LE(1, 3);

  P99_IF_DEC_LT(7,5)(true)(false);
  P99_IF_DEC_GE(5,5)(true)(false);
  P99_IF_DEC_LE(7,5)(true)(false);
  P99_IF_DEC_GT(5,5)(true)(false);

  P99_IF_DEC_GT(0,0)(true)(false);
  P99_IF_DEC_GE(0,0)(true)(false);
  P99_IF_DEC_LT(0,0)(true)(false);
  P99_IF_DEC_LE(0,0)(true)(false);

  P99_IF_DEC_GT(1,0)(true)(false);

  P99_REVS();
  P99_REVS("a");
  P99_REVS("a", "b");
  P99_REVS("a", "b", "c");
  P99_DEC_ADD(3, 7);
  P99_DEC_MUL(7, 2);
  P99_DEC_MUL(9, 3);
  //P99_DEC_MUL(8, 8);
  P99_DEC_MUL(8, 0);
  P99_DEC_MUL(10, 5);

  P99_DEC_DIV(7, 2);
  P99_DEC_DIV(9, 3);
  P99_DEC_DIV(8, 8);
  P99_DEC_DIV(7, 8);
  P99_DEC_DIV(10, 5);
  P99_DEC_DIV(3, 7);

  P99_DEC_MOD(7, 2);
  P99_DEC_MOD(9, 3);
  P99_DEC_MOD(8, 8);
  P99_DEC_MOD(7, 8);
  P99_DEC_MOD(10, 5);
  P99_DEC_MOD(3, 7);
  P99_SUMS(3, 4, 5, 6);
  P99_IS_EQ_assert(acos);
  P99_IS_EQ_assert(assert);
  P99_IS_EQ_acos(acos);
  P99_IS_EQ_acos(assert);
  //P99_GCC_VERSION;
}
