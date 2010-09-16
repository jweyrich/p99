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
#include "p99_classification.h"
#include "p99_c99.h"
#include "p99_for.h"

int main(int argc, char **argv) {
  P99_INTEGER_NORMALIZE(long int) aa;
  P99_INTEGER_NORMALIZE(long long int volatile) bb;
  P99_INTEGER_NORMALIZE(const volatile unsigned long long int) cc;
  // P99_INTEGER_NORMALIZE(unsigned long signed long int) ccc;
  P99_INTEGER_NORMALIZE(long long signed) dd;
  P99_INTEGER_NORMALIZE(long) ee;
  P99_INTEGER_NORMALIZE(wchar_t) ff;
  P99_INTEGER_NORMALIZE() *gg;
  // P99_VOID_NORMALIZE(volatile waide) *ggg;
  P99_VOID_NORMALIZE(void const) *ggg;
  P99_INTEGER_NORMALIZE(unsigned long) hh;
  P99_INTEGER_NORMALIZE(long unsigned) ii;
  P99_INTEGER_NORMALIZE(signed) jj;
  P99_INTEGER_NORMALIZE(const int) kk;
  P99_INTEGER_NORMALIZE(unsigned int) ll;
  P99_INTEGER_NORMALIZE(unsigned) mm;
  P99_INTEGER_NORMALIZE(char) oo;
  P99_INTEGER_NORMALIZE(volatile const unsigned char)*const pp;
  P99_INTEGER_NORMALIZE(signed char) qq;
  P99_INTEGER_NORMALIZE(unsigned short) rr;
  P99_INTEGER_NORMALIZE(short) ss;
  // P99_INTEGER_NORMALIZE(short long) tt;
  // P99_INTEGER_NORMALIZE(long char) uu;
  P99_FLOAT_NORMALIZE(long double) vv;
  P99_FLOAT_NORMALIZE(double _Complex long) ww;
  P99_FLOAT_NORMALIZE(const volatile float) xx;
  // P99_FLOAT_NORMALIZE(const volatile long float) yy;
  P99_BOOL_NORMALIZE(const volatile _Bool) zz0;
  P99_BOOL_NORMALIZE(const volatile bool) zz1;
  P99_BUILTIN_TYPE(UINT32_C()) au32;
  P99_BUILTIN_TYPE(UINT64_C()) au64;
  P99_BUILTIN_TYPE(INT32_C()) ai32;
  P99_BUILTIN_TYPE(INT64_C()) ai64;
}
