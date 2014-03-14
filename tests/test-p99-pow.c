/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2011-2012 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_checkargs.h"
#include "p99_for.h"
#include "p99_map.h"
#include "p99_new.h"
#include "p99_c99_default.h"
#include "p99_atomic.h"

#ifdef _OPENMP
# include <omp.h>
#else
/* If this is not compiled with OpenMP support, just ignore the two
   functions that we borrowed from them. */
#define omp_get_wtime(...) 0.0;
#define omp_set_num_threads(...) P99_NOP
#endif

/* Print a 2D double array, the (more or less) hidden function. */
void
printFunc(P99_AARG(double const, A, 2)) {
  size_t const na0 = P99_ALEN(*A, 0);
  size_t const na1 = P99_ALEN(*A, 1);
  P99_DO(size_t, i, 0, na0) {
    P99_DO(size_t, j, 0, na1) {
      printf("%g\t", (*A)[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  fflush(0);
}

P99_CA_WRAP_DECLARE(printFunc, void, (P99_AARG(double const, A, 2)), (P99_ANAME(A, 2)), (), (2));

/* Print a 2D array, the user interface */
#define print(ARR) P99_CA_CALL(printFunc, (), (2), P99_ACALL(ARR, 2, double const))

/* Check a 2D double array if it is all 0.0, the (more or less) hidden function. */
/* The array is called C. The lengths are not accessible through a
   name, but only trough the ::P99_ALEN macro. */
inline
bool
checkZeroFunc(P99_AARG(double const, C, 2)) {
  atomic_flag ret = ATOMIC_FLAG_INIT;
  P99_PARALLEL_DO(size_t, i, 0, P99_ALEN(*C, 0)) {
    P99_PARALLEL_DO(size_t, j, 0, P99_ALEN(*C, 1)) {
      if ((*C)[i][j] != 0.0) atomic_flag_test_and_set_explicit(&ret, memory_order_release);
    }
  }
  return !atomic_flag_test_and_set_explicit(&ret, memory_order_acquire);
}

P99_CA_WRAP_DECLARE(checkZeroFunc, bool, (P99_AARG(double const, C, 2)), (P99_ANAME(C, 2)), (), (2));


/* The user interface. It receives just one argument the pointer to
   the matrix. */
#define checkZero(VC) P99_CA_CALL(checkZeroFunc, (), (2), P99_ACALL(VC, 2, double const))

/* Compute the dot-product of two double vectors, the (more or less) hidden function. */
/* The two vectors are called A and B. Their length are not accessible
   through a name, but only trough the ::P99_ALEN macro. */
/* The parameter ret serves as an accumulator. */
inline
double
dotproductFunc(double ret,
               P99_AARG(double const, A, 1),
               P99_AARG(double const, B, 1)) {
  assert(P99_ALEN(A, 1) == P99_ALEN(B, 1));
  P99_DO(size_t, i, 0, P99_ALEN(A, 1))
  ret += (*A)[i] * (*B)[i];
  return ret;
}

P99_CA_WRAP_DECLARE(dotproductFunc,
                    double,
                    (double ret, P99_AARG(double const, A, 1), P99_AARG(double const, B, 1)),
                    (ret, P99_ANAME(A, 1), P99_ANAME(B, 1)), (), (2, 4));

/* A helper macro that puts the accumulator argument in front. */
#define dotproduct1(VA, VB, CAR)                                                                             \
P99_CA_CALL(dotproductFunc, (), (2, 4), CAR, P99_ACALL(VA, 1, double const), P99_ACALL(VB, 1, double const))

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
transposeFunc(P99_AARG(double const, B, 2)) {
  size_t const nb0 = P99_ALEN(*B, 0);
  size_t const nb1 = P99_ALEN(*B, 1);
  P99_AREF(double, A, nb1, nb0) = P99_ALLOC(*A);
  P99_PARALLEL_DO(size_t, j, 0, nb0) {
    /* sequentialize access to the rows of B */
    P99_AREF(double const, BV, nb1) = &(*B)[j];
    P99_PARALLEL_DO(size_t, i, 0, nb1) {
      (*A)[i][j] = (*BV)[i];
    }
  }
  return A;
}

P99_CA_WRAP_DECLARE(transposeFunc, void*, (P99_AARG(double const, B, 2)), (P99_ANAME(B, 2)), (), (2));

/* The user interface. */
#define transpose(VB) ((double const(*)[P99_ALEN(*VB, 1)][P99_ALEN(*VB, 1)])P99_CA_CALL(transposeFunc, (), (2), P99_ACALL(VB, 2, double const)))

/* Use constants that represent the sizes of blocks for a
   decomposition of the matrices used by the mult function.
   The total amount of elements used in the inner part of the
   iteration will be (istep + jstep)*kstep for A and B, and
   istep*jstep for C. */
enum {
  /* A block should be such that several such submatrices will
     reasonably fit into the cache on a decent machine. */
  blocksize = (1u << 21)/sizeof(double),
  /* istep * jstep should be such that the load into cache of any of
     the submatrices is associated to a large number of operations. */
  istep = (1u << 5),
  jstep = (1u << 5),
  /* The k-dimension of the matrices is accessed sequentially, so
     this part should be longer to give the automatic prefetcher an
     occasion to kick in. */
  kstep = (blocksize - istep*jstep)/(istep + jstep)
};

/* Compute the product of two 2D matrices. The (more or less) hidden
   function. */
/* The input matrices are called A and B. */
/* The matrix C receives the result. */
/* This function is not declared as inline. It will be way to big that
   this would make sense. */
void
multFunc(P99_AARG(double const, A, 2),
         P99_AARG(double const, B, 2),
         P99_AREF(double, C, P99_ALEN(*A, 0), P99_ALEN(*B, 1)));

P99_CA_WRAP_DECLARE(multFunc,
                    void,
                    (P99_AARG(double const, A, 2),
                     P99_AARG(double const, B, 2),
                     P99_AREF(double, C, P99_ALEN(*A, 0), P99_ALEN(*B, 1))),
                    (P99_ANAME(A, 2), P99_ANAME(B, 2), C),
                    (),
                    (2, 5, 6));

/* The user interface. It receives just the three pointers to
   the matrix as arguments. */
#define mult(ARR, BRR, CRR)                                    \
P99_CA_CALL(multFunc,                                          \
            (),                                                \
            (2, 5, 6),                                         \
            P99_ACALL(ARR, 2, double const),                   \
            P99_ACALL(BRR, 2, double const),                   \
            CRR)

/* All above would typically be written in a header (.h) file ***/
/****************************************************************/
/*************** end of interface section ***********************/
/****************************************************************/
/* From here on are only things that concern the implementation */
/* This starts with instantiations of for the inline function. **/

P99_INSTANTIATE(bool, checkZeroFunc,
                P99_AARG(double const, C, 2));

P99_INSTANTIATE(double, dotproductFunc,
                double,
                P99_AARG(double const, A, 1),
                P99_AARG(double const, B, 1));

P99_INSTANTIATE(void*, transposeFunc,
                P99_AARG(double const, B, 2));


P99_INSTANTIATE(void, multFunc,
                P99_AARG(double const, A, 2),
                P99_AARG(double const, B, 2),
                P99_AREF(double, C, P99_ALEN(*A, 0), P99_ALEN(*B, 1)));


P99_CA_WRAP_DEFINE(printFunc, void, (P99_AARG(double const, A, 2)), (P99_ANAME(A, 2)), (), (2));
P99_CA_WRAP_DEFINE(checkZeroFunc, bool, (P99_AARG(double const, C, 2)), (P99_ANAME(C, 2)), (), (2));
P99_CA_WRAP_DEFINE(dotproductFunc,
                   double,
                   (double ret, P99_AARG(double const, A, 1), P99_AARG(double const, B, 1)),
                   (ret, P99_ANAME(A, 1), P99_ANAME(B, 1)), (), (2, 4));
P99_CA_WRAP_DEFINE(multFunc,
                   void,
                   (P99_AARG(double const, A, 2),
                    P99_AARG(double const, B, 2),
                    P99_AREF(double, C, P99_ALEN(*A, 0), P99_ALEN(*B, 1))),
                   (P99_ANAME(A, 2), P99_ANAME(B, 2), C),
                   (),
                   (2, 5, 6));

P99_CA_WRAP_DEFINE(transposeFunc,
                   void*,
                   (P99_AARG(double const, B, 2)),
                   (P99_ANAME(B, 2), buf),
                   (),
                   (2));


/* Matrix multiplication has three nested for loops. One for each
   dimension and on for the dotproduct between the rows of A and the
   columns of B. We split all these loops in two. One with a longer
   step count, the values istep, jstep and kstep from above, and an
   inner one that then goes through the individual elements.

   For convenience we realize the inner three loops with step size 1
   as a macro. This will help optimization where all the three
   parameters ILEN, JLEN and KLEN correspond to compile time
   constants, namely istep, jstep and kstep, respectively.

   ILEN, JLEN and KLEN are the length of the steps in the 3
   dimensions. Generally they will be equal to istep, jstep and kstep,
   respectively, but will be less than that for the last iteration. */
#define INNER_CASE(C, A, B, ISTART, ILEN, JSTART, JLEN, KSTART, KLEN) \
/* The local type that handles a partial line. Two of these will be   \
   created below and then be used for the dot product. */             \
typedef double const kLineFrag[KLEN];                                 \
/* The local type for the partial line in the result matrix C. */     \
typedef double cLine[JLEN];                                           \
/* None of the loops are parallelized at this level. All threads      \
   should have enough work to do such that they may reuse their       \
   caches well. */                                                    \
P99_DO(size_t, i0, ISTART, ILEN) {                                    \
  register kLineFrag* AF = (kLineFrag*)&((*A)[i0][KSTART]);           \
  register cLine*restrict CL = (cLine*)&(*C)[i0][JSTART];             \
  P99_DO(size_t, j0, JSTART, JLEN) {                                  \
    register kLineFrag* BF = (kLineFrag*)&((*B)[j0][KSTART]);         \
    register double*restrict c = &(*CL)[j0-JSTART];                   \
    *c = dotproduct(AF, BF, *c);                                      \
  }                                                                   \
}

/* The implementation of the function itself. */
void
multFunc(P99_AARG(double const, A, 2),
         P99_AARG(double const, B, 2),
         P99_AREF(double, C, P99_ALEN(*A, 0), P99_ALEN(*B, 1))) {
  size_t const na0 = P99_ALEN(*A, 0);
  size_t const na1 = P99_ALEN(*A, 1);
  size_t const nb0 = P99_ALEN(*B, 0);
  size_t const nb1 = P99_ALEN(*B, 1);
  /* check that the dimensions of the matrices fit */
  assert(na1 == nb0);

  double times0 = omp_get_wtime();
  /* transpose B to have its columns consecutive in memory */
  double const P99_ARRAY(*BS, nb1, nb0) = transpose(B);
  assert(checkZero(C));
  double times1 = omp_get_wtime();

  /* The loop for the dot product cannot be easily parallelized
     because it accumulates the result. If we would want to do this,
     we would need to use reduction for the entries of C. */
  P99_DO(size_t, k, 0, na1, kstep) {
    /* The loops in the 2 dimensions can be parallelized because every
       entry of the result is computed independently. Organize a
       sufficiently large block for every individual thread such that
       it is able to reuse cached values repeatedly. For the general
       execution a thread works on

       (istep + jstep)*kstep

       elements for matrices A and B, C must not necessarily be
       cached. On these it performs istep*jstep*kstep operations, so

       istep*jstep / (istep + jstep)

       or about istep/2 operations per entry. */
    P99_PARALLEL_DO(size_t, i, 0, na0, istep) {
      P99_PARALLEL_DO(size_t, j, 0, nb1, jstep) {
        register size_t const kMax = (na1 - k);
        register size_t const iMax = (na0 - i);
        register size_t const jMax = (nb1 - j);
        if (P99_LIKELY(iMax >= istep)) {
          if (P99_LIKELY(jMax >= jstep)) {
            if (P99_LIKELY(kMax >= kstep)) {
              /* This is the principal case for which we want to
                 optimize as much as possible. */
              INNER_CASE(C, A, BS, i, istep, j, jstep, k, kstep);
            } else {
              INNER_CASE(C, A, BS, i, istep, j, jstep, k, kMax);
            }
          } else {
            if (kMax >= kstep) {
              INNER_CASE(C, A, BS, i, istep, j, jMax, k, kstep);
            } else {
              INNER_CASE(C, A, BS, i, istep, j, jMax, k, kMax);
            }
          }
        } else {
          if (jMax >= jstep) {
            if (kMax >= kstep) {
              INNER_CASE(C, A, BS, i, iMax, j, jstep, k, kstep);
            } else {
              INNER_CASE(C, A, BS, i, iMax, j, jstep, k, kMax);
            }
          } else {
            if (kMax >= kstep) {
              INNER_CASE(C, A, BS, i, iMax, j, jMax, k, kstep);
            } else {
              INNER_CASE(C, A, BS, i, iMax, j, jMax, k, kMax);
            }
          }
        }
      }
    }
  }
  double times2 = omp_get_wtime();
  printf("%s time:\t%g\n", "setup", times1 - times0);
  printf("%s time:\t%g\n", "mult", times2 - times1);
  free((void*)BS);
}

int main(int argc, char*argv[]) {
#if __STDC_WANT_LIB_EXT1__
  set_constraint_handler_s(abort_handler_s);
#endif

  if (argc <= 3) {
    fprintf(stderr, "Usage: %s n k m p e\n", argv[0]);
    fputs("   for matrices C[n][m] = A[n][k] * B[k][m]\n", stderr);
    fputs("   with p OpenMP processors\n", stderr);
    fputs("   error generation e: 0 - no error\n", stderr);
    fputs("                       1 - size error in C\n", stderr);
    fputs("                       2 - null pointer for C\n", stderr);
    return EXIT_SUCCESS;
  }
  size_t p = 1;
  P99_UNUSED(p);
  if (argc > 4)
    p = strtoul(argv[4], NULL, 0);
  omp_set_num_threads(p);

  size_t err = 0;
  if (argc > 5)
    err = strtoul(argv[5], NULL, 0);

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
  mult(AP, BP, CP);
  print(CP);

  /* Now demonstrate the use of the matrix multiplication code with
     matrices that have dynamic length. */
  register const size_t n = strtoul(argv[1]);
  register const size_t m = strtoul(argv[2]);
  register const size_t k = strtoul(argv[3]);
  printf("matrix length are n=%zu, m=%zu, k=%zu\n", n, m, k);
  printf("step   length are n=%d, m=%d, k=%d\n", istep, jstep, kstep);

  /* Allocate the three matrices. The matrix dimensions are only
     specified on the left side. The allocation expression only uses
     the base type and sizeof information to do the allocation.

     Also, each of n, m, k occurs exactly once, so the other
     dimensions are enforced. */
  P99_AREF(double, AR, n, /*          */ k /*          */) = P99_ALLOC(*AR);
  P99_AREF(double, BR, P99_ALEN(*AR, 1), m /*          */) = P99_ALLOC(*BR);

  /* Test the error checks by perturbing matrix C:
     - if err is 1 we add one to the size so a size mismatch should be detected
     - if err is 2 we don't alloc but set to 0, so a null pointer should be detected */
  P99_AREF(double, CR, P99_ALEN(*AR, 0)
           /*   */ + (err == 1 ? 1 : 0),
           /*                         */ P99_ALEN(*BR, 1)) = (err == 2 ? 0 : P99_ALLOC(*CR, P99_LVAL(double)));

  /* Initialize the two matrices to some arbitrary values */
  /* This replaces the following nested loops: */
  /* for (size_t j = 0; j < n; ++j) */
  /*   for (size_t i = 0; i < k; ++i) */
  size_t const D[2] = { P99_ALEN(*AR, 0), P99_ALEN(*AR, 1) };
  P99_FORALL(D, j, i) (*AR)[j][i] = i*j;
  /* This replaces the following nested loops: */
  /* for (size_t i = 0; i < k; ++i) */
  /*   for (size_t j = 0; j < m; ++j) */
  size_t const E[2] = { P99_ALEN(*BR, 0), P99_ALEN(*BR, 1)};
  P99_FORALL(E, i, j) (*BR)[i][j] = j + i;

  /* run the multiplication itself */
  mult(AR, BR, CR);
  free(AR);
  free(BR);
  free(CR);
}

/* follow just some arbitrary compile tests */

enum { A = 3 };

size_t A0 = P99_IPOW(0, A);
size_t A1 = P99_IPOW(1, A);
size_t A2 = P99_IPOW(2, A);
size_t A3 = P99_IPOW(3, A);
size_t A4 = P99_IPOW(4, A);

