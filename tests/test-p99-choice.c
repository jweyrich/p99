/*
** test-prepro.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Jul 19 10:00:06 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/


#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include <stddef.h>
#include <stdio.h>

#include "p99_choice.h"

P99_CHOICE_FUNCTION(unsigned, toto, -1, 4, 5, 6, 7, 8, 1, 2, 3, 4, 99, 3, 88 );

#define P99__FIXED(TYPE, NAME, VAL)             \
TYPE P99_PASTE3(NAME, _fixed, VAL)(void) {      \
  return NAME(VAL);                             \
}                                               \
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


#define SAYIT(WIDTH)                                                    \
for (unsigned i = 0; i < 64; ++i) {                                     \
  uintmax_t val = (UINTMAX_C(1) << i);                                  \
  printf("%u bit value for %ju is %u\n",                                \
         WIDTH, val, P99_PASTE2(p99_unique_bit_checked_, WIDTH)(val));  \
 }

int main(int argc, char** argv) {

  SAYIT(8);
  SAYIT(16);
  SAYIT(32);
  SAYIT(64);

}
