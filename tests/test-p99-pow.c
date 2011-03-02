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
#include <omp.h>

enum { A = 3 };

size_t A0 = P99_IPOW(0, A);
size_t A1 = P99_IPOW(1, A);
size_t A2 = P99_IPOW(2, A);
size_t A3 = P99_IPOW(3, A);
size_t A4 = P99_IPOW(4, A);

/* Print a 2D double array, the (more or less) hidden function. */
/* The array is called A, the length in the two dimensions are called
   na0 and na1, respectively. */
void
printFunc(P99_AARG(double const, A, 2, na)) {
  P99_DO(size_t, i, 0, na0) {
    P99_DO(size_t, j, 0, na1) {
      printf("%g\t", (*A)[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  fflush(0);
}

/* Print a 2D array, the user interface */
#define print(ARR) printFunc(P99_ACALL(ARR, 2, double const))

/* Zero out a 2D double array, the (more or less) hidden function. */
/* The array is called C. The lengths are not accessible through a
   name, but only trough the ::P99_ALEN macro. */
inline
void
zeroOutFunc(P99_AARG(double, C, 2)) {
  P99_PARALLEL_DO(size_t, i, 0, P99_ALEN(*C, 0)) {
    P99_PARALLEL_DO(size_t, j, 0, P99_ALEN(*C, 1)) {
      (*C)[i][j] = 0.0;
    }
  }
}

/* The user interface. It receives just one argument the pointer to
   the matrix. */
#define zeroOut(VC) zeroOutFunc(P99_ACALL(VC, 2))

/* Compute the dot-product of two double vectors, the (more or less) hidden function. */
/* The two vectors are called A and B. Their length are not accessible
   through a name, but only trough the ::P99_ALEN macro. */
/* The parameter ret serves as an accumulator. */
inline
double
dotproductFunc(double ret,
               P99_AARG(double const, A, 1),
               P99_AARG(double const, B, 1)) {
  P99_DO(size_t, i, 0, P99_ALEN(A, 1))
    ret += (*A)[i] * (*B)[i];
  return ret;
}

/* A helper macro that puts the accumulator argument in front. */
#define dotproduct1(VA, VB, CAR) dotproductFunc(CAR, P99_ACALL(VA, 1, double const), P99_ACALL(VB, 1, double const))
/* A helper macro that translates a va_arg list into the five
   arguments for dotproductFunc. */
#define dotproduct0(...) dotproduct1(__VA_ARGS__)

/* The user interface. It receives three arguments of which the last
   is optional. */
#define dotproduct(...) dotproduct0(P99_CALL_DEFARG_LIST(dotproduct, 3, __VA_ARGS__))
/* Define the optional argument the accumulator to default to 0.0 */
#define dotproduct_defarg_2() 0.0

/* Compute the transpose of a 2D matrix, the (more or less) hidden function. */
/* The input matrix is called B, the length in the two dimensions are
   called nb0 and nb1, respectively. */
/* The parameter buf provides the memory where the transpose should be
   stored. It is also returned. */
inline
void*
transposeFunc(P99_AARG(double const, B, 2, nb), void* buf) {
  typedef double P99_ARRAY(transposed, nb1, nb0);
  transposed *A = buf ? buf : malloc(sizeof(*A));
  P99_PARALLEL_DO(size_t, j, 0, nb0) {
    /* sequentialize access to the rows of B */
    P99_AREF(double const, BV, nb1) = &(*B)[j];
    P99_PARALLEL_DO(size_t, i, 0, nb1) {
      (*A)[i][j] = (*BV)[i];
    }
  }
  return A;
}

/* A helper macro that is to be called with 2 arguments. */
#define transpose2(VB, buff) transposeFunc(P99_ACALL(VB, 2, double const), buff)
/* A helper macro that is to be called with 1 arguments and adds a
   call to malloc to allocate the necessary space. */
#define transpose1(VB) transpose2(VB, malloc(sizeof(*VB)))

/* The user interface. It receives two arguments of which the second
   is optional. That second argument is given as a call to malloc to
   allocate the space to store the transposed matrix. */
#define transpose(...) P99_IF_LT(P99_NARG(__VA_ARGS__),2)(transpose1(__VA_ARGS__))(transpose2(__VA_ARGS__))


/* Use constants that represent the sizes of blocks for a
   decomposition of the matrices. */
enum {
  /* A block should be such that several such submatrices will
     reasonably fit into the cache on a decent machine. */
  blocksize = (1u << 21)/sizeof(double),
  /* istep * jstep should be such that the load into cache of any of
     the submatrices is associated to a large number of operations. */
  istep = (1u << 4),
  jstep = (1u << 4),
  /* The k-dimension of the matrices is accessed sequentially, so
     this part should be longer to give the automatic prefetcher an
     occasion to kick in. */
  kstep = blocksize/(istep*jstep)
};

inline
void
multFunc(P99_AARG(double, C, 2, nc),
         P99_AARG(double const, A, 2, na),
         P99_AARG(double const, B, 2, nb)) {
  /* check that the dimensions of the matrices fit */
  assert(nc0 == na0);
  assert(nc1 == nb1);
  assert(na1 == nb0);

  double times0 = omp_get_wtime();
  /* transpose B to have its columns consecutive in memory */
  double const P99_ARRAY(*BS, nb1, nb0) = transpose(B);
  zeroOut(C);
  double times1 = omp_get_wtime();

  /* This loop cannot be parallelized because otherwise we would need
     to reduce the entries of C */
  P99_DO(size_t, k, 0, na1, kstep) {
    P99_PARALLEL_DO(size_t, i, 0, na0, istep) {
      P99_PARALLEL_DO(size_t, j, 0, nb1, jstep) {
#define INNER_CASE(IINCR, JINCR, KINCR)                                 \
        typedef double const kLineFrag[KINCR];                          \
        typedef double cLine[JINCR];                                    \
        P99_DO(size_t, i0, i, IINCR) {                                  \
          register kLineFrag* AF = (kLineFrag*)&((*A)[i0][k]);          \
          register cLine*restrict CL = (cLine*)&(*C)[i0][j];            \
          P99_DO(size_t, j0, j, JINCR) {                                \
            register kLineFrag* BF = (kLineFrag*)&((*BS)[j0][k]);       \
            register double*restrict c = &(*CL)[j0-j];                  \
            *c = dotproduct(AF, BF, *c);                                \
          }                                                             \
        }
        register size_t const kMax = (na1 - k);
        register size_t const iMax = (na0 - i);
        register size_t const jMax = (nb1 - j);
        if (P99_LIKELY(iMax >= istep)) {
          if (P99_LIKELY(jMax >= jstep)) {
            if (P99_LIKELY(kMax >= kstep)) {
              INNER_CASE(istep, jstep, kstep);
            } else {
              INNER_CASE(istep, jstep, kMax);
            }
          } else {
            if (kMax >= kstep) {
              INNER_CASE(istep, jMax, kstep);
            } else {
              INNER_CASE(istep, jMax, kMax);
            }
          }
        } else {
          if (jMax >= jstep) {
            if (kMax >= kstep) {
              INNER_CASE(iMax, jstep, kstep);
            } else {
              INNER_CASE(iMax, jstep, kMax);
            }
          } else {
            if (kMax >= kstep) {
              INNER_CASE(iMax, jMax, kstep);
            } else {
              INNER_CASE(iMax, jMax, kMax);
            }
          }
        }
      }
    }
  }
  double times2 = omp_get_wtime();
  printf("%s time:\t%g\n", "setup", times1 - times0);
  printf("%s time:\t%g\n", "mult", times2 - times1);
  free(BS);
}

#define mult(CRR, ARR, BRR) multFunc(P99_ACALL(CRR, 2, double), P99_ACALL(ARR, 2, double const), P99_ACALL(BRR, 2, double const))

P99_INSTANTIATE(void, zeroOutFunc,
                P99_AARG(double, C, 2));

P99_INSTANTIATE(double, dotproductFunc,
                double,
                P99_AARG(double const, A, 1),
                P99_AARG(double const, B, 1));

P99_INSTANTIATE(void*, transposeFunc,
                P99_AARG(double const, B, 2),
                void*);


P99_INSTANTIATE(void, multFunc,
                P99_AARG(double, C, 2),
                P99_AARG(double const, A, 2),
                P99_AARG(double const, B, 2));

int main(int argc, char*argv[]) {
  size_t p = 1;
  if (argc > 4)
    p = strtoul(argv[4], NULL, 0);
  omp_set_num_threads(p);

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
  dotproduct(&(*BP)[0], &(*BP)[1]);
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
  size_t n = strtoul(argv[1]);
  size_t m = strtoul(argv[2]);
  size_t k = strtoul(argv[3]);
  P99_AREF(double, AR, n, k) = malloc(sizeof(*AR));
  P99_AREF(double, BR, k, m) = malloc(sizeof(*BR));
  /* for (size_t j = 0; j < n; ++j) */
  /*   for (size_t i = 0; i < k; ++i) */
  size_t const D[2] = {n, k};
  P99_FORALL(D, j, i)
    (*AR)[j][i] = i*j;
  /* for (size_t i = 0; i < k; ++i) */
  /*   for (size_t j = 0; j < m; ++j) */
  size_t const E[2] = {k, m};
  P99_FORALL(E, i, j)
    (*BR)[i][j] = j + i;
  P99_AREF(double, CR, n, m) = malloc(sizeof(*CR));
  mult(CR, AR, BR);
  free(AR);
  free(BR);
  free(CR);
}
