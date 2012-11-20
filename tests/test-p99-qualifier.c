/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2012 Jens Gustedt, INRIA, France                     */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_atomic.h"

#define DETECT_TYPE(T, X) P99_GENERIC((X), false, (T, true))
#define DETECT_QVAL(T, X) P99_GENERIC(P99_QVALUE(X), false, (T, true))
#define DETECT_SVAL(T, X) P99_GENERIC(P99_SVALUE(X), false, (T, true))
#define DETECT_TVAL(T, X) P99_GENERIC(P00_RVALUE(X), false, (T, true))


typedef struct tester tester;

struct tester {
  double a;
};

#define SAYIT(MAC)                                                                                                     \
  printf(#MAC ":\t%d for double lvalue\n", MAC(double const, (double const){ 0 }));                                    \
  printf(#MAC ":\t%d for double rvalue\n", MAC(double const, (double const)0));                                        \
  printf(#MAC ":\t%d for int lvalue\n", MAC(int const, (int const){ 0 }));                                             \
  printf(#MAC ":\t%d for int rvalue\n", MAC(int const, (int const)0));                                                 \
  printf(#MAC ":\t%d for int lvalue conversion\n", MAC(int const, +((int const){ 0 })));                               \
  printf(#MAC ":\t%d for int rvalue, no conversion\n", MAC(int const, +((int const)0)));                               \
  printf(#MAC ":\t%d for short lvalue, potential promotion\n", MAC(short const, (short const){ 0 }));                  \
  printf(#MAC ":\t%d for short rvalue, potential promotion\n", MAC(short const, (short const)0));                      \
  printf(#MAC ":\t%d for short lvalue conversion and potential promotion\n", MAC(short const, +((short const){ 0 }))); \
  printf(#MAC ":\t%d for short rvalue and potential promotion\n", MAC(short const, +((short const)0)));                \
  printf(#MAC ":\t%d for pointer lvalue\n", MAC(int const*const, (int const*const){ 0 }));                             \
  printf(#MAC ":\t%d for pointer rvalue\n", MAC(int const*const, (int const*const)0));                                 \
  printf(#MAC ":\t%d for void pointer lvalue\n", MAC(void const*const, (void const*const){ 0 }));                      \
  printf(#MAC ":\t%d for void pointer rvalue\n", MAC(void const*const, (void const*const)0));                          \
  printf(#MAC ":\t%d for pointer lvalue, restrict\n", MAC(int const*restrict, (int const*restrict){ 0 }));             \
  printf(#MAC ":\t%d for pointer rvalue, restrict\n", MAC(int const*restrict, (int const*restrict)0));                 \
  printf(#MAC ":\t%d for array, potential array-to-pointer conversion\n", MAC(int const[2], (int const[2]){ 0 }))


#define SAYIT_STRUCT(MAC)                                                                                    \
printf(#MAC ":\t%d for struct lvalue\n", MAC(tester const, (tester const){ .a = 0 }));                       \
printf(#MAC ":\t%d for struct rvalue\n", MAC(tester const, ((tester){ .a = 0 } = (tester const){ .a = 0 })))

int main(void) {
  printf("P99 compiler:\t%40s\n", P99_COMPILER_VERSION);
  SAYIT_STRUCT(DETECT_TYPE);
  SAYIT(DETECT_TYPE);
  SAYIT(DETECT_QVAL);
  SAYIT(DETECT_SVAL);
  SAYIT(DETECT_TVAL);
}
