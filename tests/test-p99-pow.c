/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "p99_for.h"
#include "p99_map.h"
#include "p99_map.h"
#include "p99_c99_default.h"

enum { A = 3 };

size_t A0 = P99_IPOW(0, A);
size_t A1 = P99_IPOW(1, A);
size_t A2 = P99_IPOW(2, A);
size_t A3 = P99_IPOW(3, A);
size_t A4 = P99_IPOW(4, A);

void
printFunc(P99_AARG(double const, A, 2, a)) {
  P99_DO(size_t, i, 0, a0) {
    P99_DO(size_t, j, 0, a1) {
      printf("%g\t", (*A)[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  fflush(stdout);
}

#define print(ARR) printFunc(P99_ACALL(ARR, 2, double const))

extern inline
double
dotproductFunc(P99_AARG(register double const, A, 1),
               P99_AARG(register double const, B, 1));

inline
double
dotproductFunc(P99_AARG(register double const, A),
           P99_AARG(register double const, B)) {
  register double ret = 0.0;
  P99_DO(size_t, i, 0, P99_ALEN(A, 1))
    ret += (*A)[i] * (*B)[i];
  return ret;
}

#define dotproduct(VA, VB) dotproductFunc(P99_ACALL(VA, 1, double const), P99_ACALL(VB, 1, double const))

extern inline
void
transposeFunc(P99_AARG(double, A, 2),
          P99_AARG(double const, B, 2));

inline
void
transposeFunc(P99_AARG(double, A, 2),
          P99_AARG(double const, B, 2)) {
  P99_DO(size_t, j, 0, P99_ALEN(*B)) {
    /* sequentialize access to the rows of B */
    P99_AREF(double const, BV, P99_ALEN(A, 1)) = &(*B)[j];
    P99_DO(size_t, i, 0, P99_ALEN(A, 1)) {
      (*A)[i][j] = (*BV)[i];
    }
  }
}

#define transpose(VA, VB) transposeFunc(P99_ACALL(VA, 2), P99_ACALL(VB, 2, double const))

extern inline
void
multFunc(P99_AARG(double, C, 2),
     P99_AARG(double const, A, 2),
     P99_AARG(double const, B, 2));

inline
void
multFunc(P99_AARG(double, C, 2),
     P99_AARG(double const, A, 2),
     P99_AARG(double const, B, 2)) {
  /* check that the dimensions of the matrices fit */
  assert(P99_ALEN(C, 1) == P99_ALEN(A, 1));
  assert(P99_ALEN(C, 2) == P99_ALEN(B, 2));
  assert(P99_ALEN(A, 2) == P99_ALEN(B, 1));

  /* transpose B to have its columns consecutive in memory */
  P99_AREF(double, BS, P99_ALEN(B, 2), P99_ALEN(B, 1)) = malloc(sizeof(*B));
  transpose(BS, B);

  P99_PARALLEL_DO(size_t, i, 0, P99_ALEN(A, 1)) {
    /* Obtain the current row of A */
    P99_AREF(double const, AV, P99_ALEN(A, 2)) = &(*A)[i];
    P99_DO(size_t, j, 0, P99_ALEN(BS, 1)) {
      /* Obtain the current row of BS, which is actually the current
         column of B */
      (*C)[i][j] = dotproduct(AV, &(*BS)[j]);
    }
  }
  free(BS);
}

#define mult(CRR, ARR, BRR) multFunc(P99_ACALL(CRR, 2, double), P99_ACALL(ARR, 2, double const), P99_ACALL(BRR, 2, double const))


int main(int argc, char*argv[]) {
  double const P99_ARRAY(A, 3, 2) = {
    {2, 3 },
    {1, 1 },
    {-1, 0}
  };
  P99_AREF(double const, AP, 3, 2) = &A;
  print(AP);
  double const P99_ARRAY(B, 2, 4) = {
    {2, 3, -1, 0},
    {1, 1, 2, 1},
  };
  P99_AREF(double const, BP, 2, 4) = &B;
  print(BP);
  double P99_ARRAY(C, 3, 4) = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };
  P99_AREF(double, CP, 3, 4) = &C;
  mult(CP, AP, BP);
  print(CP);
  /* captured by the checks inside the macro: */
  // struct {double a[2];} * p; P99_ALEN(p);
  /* not captured:                            */
  // char * q; P99_ALEN(q);
}
