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
#include "p99_checkargs.h"
#include "p99_id.h"
#include "p99_args.h"
#include "p99_map.h"
#include "p99_defarg.h"
#include "p99_c99.h"


enum { d0 = 2, d1 = 3, d2 = 2, d3 = 3 };

unsigned const D[4] = { d0, d1, d2, d3 };

unsigned const A[d0][d1][d2][d3]
= {
  [0] = {
    [0] = {
      [0] = {
        [0] = 1,
        [1] = 2,
        [2] = 3,
      },
      [1] = {
        [0] = 4,
        [1] = 5,
        [2] = 6,
      },
    },
    [1] = {
      [0] = {
        [0] = 7,
        [1] = 8,
        [2] = 9,
      },
      [1] = {
        [0] = 0xA,
        [1] = 0xB,
        [2] = 0xC,
      },
    },
    [2] = {
      [0] = {
        [0] = 0xD,
        [1] = 0xE,
        [2] = 0xF,
      },
      [1] = {
        [0] = 0x10,
        [1] = 0x11,
        [2] = 0x12,
      },
    },
  },
  [1] = {
    [0] = {
      [0] = {
        [0] = 0x13,
        [1] = 0x14,
        [2] = 0x15,
      },
      [1] = {
        [0] = 0x16,
        [1] = 0x17,
        [2] = 0x18,
      },
    },
    [1] = {
      [0] = {
        [0] = 0x19,
        [1] = 0x1A,
        [2] = 0x1B,
      },
      [1] = {
        [0] = 0x1C,
        [1] = 0x1D,
        [2] = 0x1E,
      },
    },
    [2] = {
      [0] = {
        [0] = 0x1F,
        [1] = 0x20,
        [2] = 0x21,
      },
      [1] = {
        [0] = 0x22,
        [1] = 0x23,
        [2] = 0x24,
      },
    },
  },
};

inline
unsigned char sum(size_t n, unsigned char A[n]) {
  unsigned char ret = 0;
  for (size_t i = 0; i < n; ++i) ret += A[i];
  return ret;
}

P99_INSTANTIATE(unsigned char, sum, size_t n, unsigned char A[n]);


P99_CA_WRAP_DECLARE(sum, unsigned char, (size_t n, unsigned char A[n]), (n, A), (1));
P99_CA_WRAP_DEFINE(sum, unsigned char, (size_t n, unsigned char A[n]), (n, A), (1));

#define sum(...) P99_CA_CALL(sum, (1), (1), __VA_ARGS__)

inline
unsigned char sump(size_t n, unsigned char *A) {
  unsigned char ret = 0;
  for (size_t i = 0; i < n; ++i) ret += A[i];
  return ret;
}

P99_INSTANTIATE(unsigned char, sump, size_t n, unsigned char *A);

P99_CA_WRAP_DECLARE(sump, unsigned char, (size_t n, unsigned char *A), (n, A), (), (1));
P99_CA_WRAP_DEFINE(sump, unsigned char, (size_t n, unsigned char *A), (n, A), (), (1));

#define sump(...) P99_CA_CALL(sump, (), (1), __VA_ARGS__)

int main(int argc, char** argv) {
#if __STDC_WANT_LIB_EXT1__
  set_constraint_handler_s(abort_handler_s);
#endif
  unsigned const* a = &(A[0][0][0][0]);
  P99_PARALLEL_FORALL(D, i0, i1, i2, i3)
  printf("ooo: compare indexing: %u %u\n",
         a[ P99_CDIM(D, i0, i1, i2, i3)],
         A[i0][i1][i2][i3]
        );
  P99_DO(unsigned, i,  0, argc)
  printf("ino: %u\n", i);
  P99_PARALLEL_DO(unsigned, i,  0, argc)
  printf("ooo: %u\n", i);
  __asm__("nop");
  unsigned char B[argc];
  printf("va_arg sum: %hhu\n", sum(argc, B));
  __asm__("nop");
  size_t const fail8 = (argc == 2 ? 8 : 7);
  size_t const fail9 = (argc == 3 ? 10 : 9);
  {
    unsigned char C[] = { 1 };
    printf("fixed sump: %hhu\n", sump(1, C));
    unsigned char D[] = { 1, 2 };
    printf("fixed sump: %hhu\n", sump(2, D));
    unsigned char E[] = { 1, 2, 3 };
    printf("fixed sump: %hhu\n", sump(3, E));
    unsigned char F[] = { 1, 2, 3, 4 };
    printf("fixed sump: %hhu\n", sump(4, F));
    unsigned char G[] = { 1, 2, 3, 4, 5 };
    printf("fixed sump: %hhu\n", sump(5, G));
    unsigned char H[] = { 1, 2, 3, 4, 5, 6 };
    printf("fixed sump: %hhu\n", sump(6, H));
    unsigned char J[] = { 1, 2, 3, 4, 5, 6, 7 };
    printf("fixed sump: %hhu\n", sump(fail8, J));
    unsigned char K[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    printf("fixed sump: %hhu\n", sump(8, K));
    unsigned char L[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    printf("fixed sump: %hhu\n", sump(fail9, L));
  }
  {
    unsigned char C[] = { 1 };
    printf("fixed sum: %hhu\n", sum(1, C));
    unsigned char D[] = { 1, 2 };
    printf("fixed sum: %hhu\n", sum(2, D));
    unsigned char E[] = { 1, 2, 3 };
    printf("fixed sum: %hhu\n", sum(3, E));
    unsigned char F[] = { 1, 2, 3, 4 };
    printf("fixed sum: %hhu\n", sum(4, F));
    unsigned char G[] = { 1, 2, 3, 4, 5 };
    printf("fixed sum: %hhu\n", sum(5, G));
    unsigned char H[] = { 1, 2, 3, 4, 5, 6 };
    printf("fixed sum: %hhu\n", sum(6, H));
    unsigned char J[] = { 1, 2, 3, 4, 5, 6, 7 };
    printf("fixed sum: %hhu\n", sum(fail8, J));
    unsigned char K[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    printf("fixed sum: %hhu\n", sum(8, K));
    unsigned char L[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    printf("fixed sum: %hhu\n", sum(fail9, L));
  }
}
