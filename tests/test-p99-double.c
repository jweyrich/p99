/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_double.h"
#include "p99_compiler.h"

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

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

  P99_IS_LT(0, 0);
  P99_IS_LT(0, 1);
  P99_IS_LT(0, 2);
  P99_IS_LT(1, 1);
  P99_IS_LT(1, 0);
  P99_IS_LT(1, 2);
  P99_IS_LT(1, 3);

  P99_IS_GT(0, 0);
  P99_IS_GT(0, 1);
  P99_IS_GT(0, 2);
  P99_IS_GT(1, 1);
  P99_IS_GT(1, 0);
  P99_IS_GT(1, 2);
  P99_IS_GT(1, 3);

  P99_IS_GE(0, 0);
  P99_IS_GE(0, 1);
  P99_IS_GE(0, 2);
  P99_IS_GE(1, 1);
  P99_IS_GE(1, 0);
  P99_IS_GE(1, 2);
  P99_IS_GE(1, 3);

  P99_IS_LE(0, 0);
  P99_IS_LE(0, 1);
  P99_IS_LE(0, 2);
  P99_IS_LE(1, 1);
  P99_IS_LE(1, 0);
  P99_IS_LE(1, 2);
  P99_IS_LE(1, 3);

  P99_IF_LT(7,5)(true)(false);
  P99_IF_GE(5,5)(true)(false);
  P99_IF_LE(7,5)(true)(false);
  P99_IF_GT(5,5)(true)(false);

  P99_IF_GT(0,0)(true)(false);
  P99_IF_GE(0,0)(true)(false);
  P99_IF_LT(0,0)(true)(false);
  P99_IF_LE(0,0)(true)(false);

  P99_IF_GT(1,0)(true)(false);

  P99_REVS();
  P99_REVS("a");
  P99_REVS("a", "b");
  P99_REVS("a", "b", "c");
  P99_ADD(3, 7);
  P99_MUL(7, 2);
  P99_MUL(9, 3);
  //P99_MUL(8, 8);
  P99_MUL(8, 0);
  P99_MUL(10, 5);

  P99_DIV(7, 2);
  P99_DIV(9, 3);
  P99_DIV(8, 8);
  P99_DIV(7, 8);
  P99_DIV(10, 5);
  P99_DIV(3, 7);

  P99_MOD(7, 2);
  P99_MOD(9, 3);
  P99_MOD(8, 8);
  P99_MOD(7, 8);
  P99_MOD(10, 5);
  P99_MOD(3, 7);
  P99_SUMS(3, 4, 5, 6);
  P99_IS_EQ_assert(acos);
  P99_IS_EQ_assert(assert);
  P99_IS_EQ_acos(acos);
  P99_IS_EQ_acos(assert);
  //P99_GCC_VERSION;
}
