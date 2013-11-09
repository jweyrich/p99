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
#include "p99_classification.h"
#include "p99_c99.h"
#include "p99_for.h"
#include "p99_int.h"

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

int main(int argc, char **argv) {
  P99_INTEGER_NORMALIZE(long int) aa = P99_INIT;
  P99_INTEGER_NORMALIZE(long long int volatile) bb = P99_INIT;
  P99_INTEGER_NORMALIZE(const volatile unsigned long long int) cc = P99_INIT;
  // P99_INTEGER_NORMALIZE(unsigned long signed long int) ccc = P99_INIT;
  P99_INTEGER_NORMALIZE(long long signed) dd = P99_INIT;
  P99_INTEGER_NORMALIZE(long) ee = P99_INIT;
  P99_INTEGER_NORMALIZE(wchar_t) ff = P99_INIT;
  P99_INTEGER_NORMALIZE() *gg = P99_INIT;
  // P99_VOID_NORMALIZE(volatile waide) *ggg = P99_INIT;
  P99_VOID_NORMALIZE(void const) *ggg = P99_INIT;
  P99_INTEGER_NORMALIZE(unsigned long) hh = P99_INIT;
  P99_INTEGER_NORMALIZE(long unsigned) ii = P99_INIT;
  P99_INTEGER_NORMALIZE(signed) jj = P99_INIT;
  P99_INTEGER_NORMALIZE(const int) kk = P99_INIT;
  P99_INTEGER_NORMALIZE(unsigned int) ll = P99_INIT;
  P99_INTEGER_NORMALIZE(unsigned) mm = P99_INIT;
  P99_INTEGER_NORMALIZE(char) oo = P99_INIT;
  P99_INTEGER_NORMALIZE(volatile const unsigned char)*const pp = P99_INIT;
  P99_INTEGER_NORMALIZE(signed char) qq = P99_INIT;
  P99_INTEGER_NORMALIZE(unsigned short) rr = P99_INIT;
  P99_INTEGER_NORMALIZE(short) ss = P99_INIT;
  // P99_INTEGER_NORMALIZE(short long) tt = P99_INIT;
  // P99_INTEGER_NORMALIZE(long char) uu = P99_INIT;
  P99_FLOAT_NORMALIZE(long double) vv = P99_INIT;
  P99_FLOAT_NORMALIZE(double _Complex long) ww = P99_INIT;
  P99_FLOAT_NORMALIZE(const volatile float) xx = P99_INIT;
  // P99_FLOAT_NORMALIZE(const volatile long float) yy = P99_INIT;
  P99_BOOL_NORMALIZE(const volatile _Bool) zz0 = P99_INIT;
  P99_BOOL_NORMALIZE(const volatile bool) zz1 = P99_INIT;
  P99_BUILTIN_TYPE(P99_REMOVE_PAREN(UINT32_C())) au32 = P99_INIT;
  P99_BUILTIN_TYPE(P99_REMOVE_PAREN(UINT64_C())) au64 = P99_INIT;
  P99_BUILTIN_TYPE(P99_REMOVE_PAREN(INT32_C())) ai32 = P99_INIT;
  P99_BUILTIN_TYPE(P99_REMOVE_PAREN(INT64_C())) ai64 = P99_INIT;
  P99_BUILTIN_TYPE(P99_REMOVE_PAREN(ULL)) ai65 = P99_INIT;
  P99_BUILTIN_TYPE(P99_REMOVE_PAREN((ULL))) ai66 = P99_INIT;
}
