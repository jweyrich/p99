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
#include "p99_choice.h"
#include "p99_c99.h"

P99_CHOICE_FUNCTION(unsigned, toto, -1, 4, 5, 6, 7, 8, 1, 2, 3, 4, 99, 3, 88 );

#define P99__FIXED(TYPE, NAME, VAL)                            \
TYPE P99_PASTE3(NAME, _fixed, VAL)(void) {                     \
  return NAME(VAL);                                            \
}                                                              \
P99_MACRO_END(_fixed)

P99__FIXED(unsigned, toto, 0);
P99__FIXED(unsigned, toto, 5);
P99__FIXED(unsigned, toto, 10);
P99__FIXED(unsigned, toto, 10000);

unsigned toto_toto(size_t x) {
  return toto(x);
}

P99__FIXED(unsigned, p99_unique_bit_32, 0);
P99__FIXED(unsigned, p99_unique_bit_32, 1);
P99__FIXED(unsigned, p99_unique_bit_32, 2);
P99__FIXED(unsigned, p99_unique_bit_32, 4);
P99__FIXED(unsigned, p99_unique_bit_32, 8);
P99__FIXED(unsigned, p99_unique_bit_32, 16);
P99__FIXED(unsigned, p99_unique_bit_32, 32);
P99__FIXED(unsigned, p99_unique_bit_32, 64);
P99__FIXED(unsigned, p99_unique_bit_32, 128);
P99__FIXED(unsigned, p99_unique_bit_32, 1024);

unsigned p99__unique_bit_hash_6_(size_t x) {
  return p99__unique_bit_hash_6(x);
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
