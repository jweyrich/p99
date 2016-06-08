/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved, 2016 Jens Gustedt, INRIA, France                      */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_uf.h"

static
uint64_t position(size_t n, uint64_t i, uint64_t j) {
  return i*n + j;
}

/**
 ** @brief Print a UF data structure that is supposed be placed on a
 ** n×n grid.
 **
 ** The root of a region is printed as +N, where N is the size of the
 ** region. All other elements print just what they know is the next
 ** upward in the UF chain to the root.
 **/
static
void printBoard(size_t n, p99_uf const* uf) {
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      int64_t region = uf->tab[position(n, i, j)];
      if (region < 0)
        printf("+%.02" PRId64 "|", -region);
      else
        printf("%.03" PRIX64 "|", region);
    }
    printf("\n");
  }
  for (size_t j = 0; j < n; ++j) {
    fputs("----", stdout);
  }
  fputc('\n', stdout);
}


int main(void) {
  size_t n = 20;
  p99_uf* uf = p99_uf_alloc(n*n);
  // Link pairs in each row. The starting position of a pair is odd or
  // even, determined by the parity of the row.
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i%2; j < n-1; j += 2) {
      p99_uf_union(uf, position(n, i, j), position(n, i, j+1));
    }
  }
  printBoard(n, uf);
  // Link pairs in each column. The result of all this should be
  // diagonals of width two.
  for (size_t i = 0; i < n-1; ++i) {
    for (size_t j = !(i%2); j < n; j += 2) {
      p99_uf_union(uf, position(n, i+1, j), position(n, i, j));
    }
  }
  printBoard(n, uf);
  p99_uf_flatten(uf, 0, uf->size);
  printBoard(n, uf);
  p99_uf_free(uf);
}
