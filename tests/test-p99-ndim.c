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
#include "p99_id.h"
#include "p99_args.h"
#include "p99_map.h"

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

int main(int argc, char** argv) {
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
}
