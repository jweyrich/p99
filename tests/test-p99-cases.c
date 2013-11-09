/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                     */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
/*
** test_p99_cases.c
**
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
**
** Started on  Wed Jan 19 08:44:00 2011 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "p99_for.h"

typedef struct {
  size_t index;
  double* aPos;
} pair;

#define ASIZE 10

double A[] = { P99_DUPL(ASIZE, 1.7) };

#define INIT_PAIR(I) [I] = {                                   \
  .index = I,                                                  \
  .aPos = &A[I],                                               \
  }

pair P[] = { P99_REPEAT(INIT_PAIR, ASIZE) };

int main(int argc, char*argv[]) {
  if (argc < 2) return -2;
  switch (argv[1][0]) {
    /* This line could produce a warning */
    P99_CASERANGE('\0', 0): return -1;
    P99_CASERANGE('0', 10): return 0;
    P99_CASERANGE('A', 25): --argc;
    P99_CASERANGE('\01', 0): return -1;
    P99_CASERANGE('a', 25, oioi): return P[2].index;
  default: return 3;
  }
}
