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
#include "p99_rand.h"


int main(int argc, char * argv[]) {
  unsigned n = argc <= 1 ? 20 : strtoul(argv[1], 0, 0);
  for (unsigned i = 0; i < n; ++i)
    printf("0x%016" PRIx64 "\n", p99_rand());
  p99_rand_init();
  for (unsigned i = 0; i < n; ++i)
    printf("%.15f\n", p99_drand());
}
