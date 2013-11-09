/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2010-2012 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
/**
 ** @file
 ** @brief Test the behavior of compound literals
 **
 ** Compound literals may lead to the propagation of stack pointers
 ** outside there scope of validity. This little program tests the
 ** behavior of your compiler with respect to these, in particular if
 ** it detects the misbehavior.
 **
 ** All my compilers with just -Wall for the moment only detect
 ** ::myLessBuggyFunction, i.e an explicit temporary that is returned
 ** from a function.
 **
 ** Compound literal proliferation in myBuggyFunction is only detected
 ** by clang with the --analyze option.
 **
 ** valgrind detects this misshavior at run time
 **/

#include "p99.h"
#include "p99_for.h"
#include "p99_if.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

enum { len = 9 };


char const* myVerryBuggyFunction(void) {
  return memset((char[len + 1]) { 0 }, 'a', len);
}

char const* myStillBuggyFunction(void) {
  char tmp[len + 1] = { 0 };
  return memset(tmp, 'a', len);
}

char const* myLessBuggyFunction(void) {
  char const tmp[len + 1] = { "aaaaaaaaa" };
  return tmp;
}

char const* myBuggyFunction(void) {
  return (char const[len + 1]) { 0 };
}

struct toto {
  int a;
};

struct tutu {
  int b;
  struct toto c;
};


P99_CONSTANT(int, O1, 1);
P99_CONSTANT(unsigned, O2, 2);
P99_CONSTANT(signed, O3, 3);
P99_CONSTANT(struct tutu, tutu0);
P99_CONSTANT(struct tutu, tutu1, { .b = 7, });

inline
struct tutu * tutu_init(struct tutu* x) {
  if (x) *x = tutu0;
  return x;
}

P99_INSTANTIATE(struct tutu *, tutu_init, struct tutu*);


int main(int argc, char*argv[]) {
  printf("return %u 'a's: %s\n", len, myVerryBuggyFunction());
  printf("return %u 'a's: %s\n", len, myStillBuggyFunction());
  printf("return %u 'a's: %s\n", len, myLessBuggyFunction());
  printf("return two temporaries? %p ?= %p\n",
         (void*)myBuggyFunction(),
         (void*)myBuggyFunction());
  int * ip = P99_INIT;
  int * ipp[2] = P99_INIT;
  int (*ippp)[2] = P99_LVAL(int (*)[2]);
  struct toto j = P99_INIT;
  struct toto jj[4][5] = P99_INIT;
  struct toto (*jjj)[4][5] = &P99_LVAL(struct toto[4][5]);
  struct toto k = P99_DESIGNATED(j, .a);
  struct tutu L = P99_INIT;
  struct tutu *M = &P99_LCOPY(struct tutu, L, .b, .c);
  double A[4] = { 1, 2, 3, 4 };
  double *B = P99_LCOPY(double[4], A, [2]);
  P99_UNUSED(ip, ipp, ippp, jj, jjj, k, M, B);
  return P99_LVAL(struct toto[3][4])[0][0].a;
}
