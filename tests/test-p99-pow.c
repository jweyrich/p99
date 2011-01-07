/*
** test-p99-pow.c
**
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
**
** Started on  Mon Dec 27 22:04:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

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
print(P99_AARG(double, A, 2, a)) {
  P99_DO(size_t, i, 0, a0) {
    P99_DO(size_t, j, 0, a1) {
      printf("%g\t", (*A)[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  fflush(stdout);
}

#define print(ARR) print(P99_ACALL(ARR, 2))

extern inline
double
dotproduct(P99_AARG(register double, A),
           P99_AARG(register double, B));

inline
double
dotproduct(P99_AARG(register double, A),
           P99_AARG(register double, B)) {
  register double ret = 0.0;
  P99_DO(size_t, i, 0, P99_ALEN(A, 1))
    ret += (*A)[i] * (*B)[i];
  return ret;
}

#define dotproduct(VA, VB) dotproduct(P99_ACALL(VA), P99_ACALL(VB))

extern inline
void
transpose(P99_AARG(double, A, 2),
          P99_AARG(double, B, 2));

inline
void
transpose(P99_AARG(double, A, 2),
          P99_AARG(double, B, 2)) {
  P99_DO(size_t, j, 0, P99_ALEN(*B)) {
    /* sequentialize access to the rows of B */
    P99_AREF(double, BV, P99_ALEN(A, 1)) = &(*B)[j];
    P99_DO(size_t, i, 0, P99_ALEN(A, 1)) {
      (*A)[i][j] = (*BV)[i];
    }
  }
}

#define transpose(VA, VB) transpose(P99_ACALL(VA, 2), P99_ACALL(VB, 2))

extern inline
void
mult(P99_AARG(double, C, 2),
     P99_AARG(double, A, 2),
     P99_AARG(double, B, 2));

inline
void
mult(P99_AARG(double, C, 2),
     P99_AARG(double, A, 2),
     P99_AARG(double, B, 2)) {
  /* check that the dimensions of the matrices fit */
  assert(P99_ALEN(C, 1) == P99_ALEN(A, 1));
  assert(P99_ALEN(C, 2) == P99_ALEN(B, 2));
  assert(P99_ALEN(A, 2) == P99_ALEN(B, 1));

  /* transpose B to have its columns consecutive in memory */
  P99_AREF(double, BS, P99_ALEN(B, 2), P99_ALEN(B, 1)) = malloc(sizeof(*B));
  transpose(BS, B);

  P99_PARALLEL_DO(size_t, i, 0, P99_ALEN(A, 1)) {
    /* Obtain the current row of A */
    P99_AREF(double, AV, P99_ALEN(A, 2)) = &(*A)[i];
    P99_DO(size_t, j, 0, P99_ALEN(BS, 1)) {
      /* Obtain the current row of BS, which is actually the current
         column of B */
      (*C)[i][j] = dotproduct(AV, &(*BS)[j]);
    }
  }
  free(BS);
}

#define mult(CRR, ARR, BRR) mult(P99_ACALL(CRR, 2), P99_ACALL(ARR, 2), P99_ACALL(BRR, 2))


int main(int argc, char*argv[]) {
  double P99_ARRAY(A, 3, 2) = {
    {2, 3 },
    {1, 1 },
    {-1, 0}
  };
  P99_AREF(double, AP, 3, 2) = &A;
  print(AP);
  double P99_ARRAY(B, 2, 4) = {
    {2, 3, -1, 0},
    {1, 1, 2, 1},
  };
  P99_AREF(double, BP, 2, 4) = &B;
  print(BP);
  double P99_ARRAY(C, 3, 4) = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };
  P99_AREF(double, CP, 3, 4) = &C;
  mult(CP, AP, BP);
  print(CP);
}
