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
#include "p99_choice.h"
#include "p99_c99.h"
#include "p99_compiler.h"

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

P99_CHOICE_FUNCTION(unsigned, toto, -1, 4, 5, 6, 7, 8, 1, 2, 3, 4, 99, 3, 88 );

#define P00_FIXED(TYPE, NAME, VAL)                             \
TYPE P99_PASTE3(NAME, _fixed, VAL)(void) {                     \
  return NAME(VAL);                                            \
}                                                              \
P99_MACRO_END(_fixed)

static P00_FIXED(unsigned, toto, 0);
static P00_FIXED(unsigned, toto, 5);
static P00_FIXED(unsigned, toto, 10);
static P00_FIXED(unsigned, toto, 10000);

static
unsigned toto_toto(size_t x) {
  return toto(x);
}

static P00_FIXED(unsigned, p99_unique_bit_32, 0);
static P00_FIXED(unsigned, p99_unique_bit_32, 1);
static P00_FIXED(unsigned, p99_unique_bit_32, 2);
static P00_FIXED(unsigned, p99_unique_bit_32, 4);
static P00_FIXED(unsigned, p99_unique_bit_32, 8);
static P00_FIXED(unsigned, p99_unique_bit_32, 16);
static P00_FIXED(unsigned, p99_unique_bit_32, 32);
static P00_FIXED(unsigned, p99_unique_bit_32, 64);
static P00_FIXED(unsigned, p99_unique_bit_32, 128);
static P00_FIXED(unsigned, p99_unique_bit_32, 1024);

static
unsigned p00_unique_bit_hash_6_(size_t x) {
  return p00_unique_bit_hash_6(x);
}


#define SAYIT(WIDTH)                                                   \
for (unsigned i = 0; i < 64; ++i) {                                    \
  uintmax_t val = (UINTMAX_C(1) << i);                                 \
  printf("%u bit value for %ju is %u\n",                               \
         WIDTH, val, P99_PASTE2(p99_unique_bit_checked_, WIDTH)(val)); \
 }

int main(int argc, char** argv) {

  SAYIT(8);
  SAYIT(16);
  SAYIT(32);
  SAYIT(64);

}
