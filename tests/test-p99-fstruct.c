/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2013 Jens Gustedt, INRIA, France                     */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include <stdio.h>

#include "p99.h"

typedef struct {
  struct {
    double b;
    int    a;
  } x;
  char   f[];
} S;

int main(void) {
  printf("size -> %zu\n", P99_FSIZEOF(S, f, 10));
  return 0;
}
