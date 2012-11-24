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
#include "p99_qsort.h"

inline
int uComp0(void const*a, void const*b) {
  register uint64_t const*const A = a;
  register uint64_t const*const B = b;
  {
    register uint64_t const a = *A;
    register uint64_t const b = *B;
    return (a < b) ? -1 : (a > b);
  }
}

inline
int uComp(void const*a, void const*b, void*c) {
  if (c) {
    register uint64_t const*const A = a;
    register uint64_t const*const B = b;
    register uint64_t const*const C = c;
    {
      register uint64_t const c = *C;
      register uint64_t const a = *A % c;
      register uint64_t const b = *B % c;
      return (a < b) ? -1 : (a > b);
    }
  } else
    return uComp0(a, b);
}

inline
int dComp(void const*a, void const*b, void*context) {
  register double const*const A = a;
  register double const*const B = b;
  {
    register double const a = *A;
    register double const b = *B;
    return (a < b) ? -1 : (a > b);
  }
}

P99_INSTANTIATE(int, dComp, void const*, void const*, void*);
P99_INSTANTIATE(int, uComp, void const*, void const*, void*);
P99_INSTANTIATE(int, uComp0, void const*, void const*);

int main(int argc, char * argv[]) {
  size_t n = argc <= 1 ? 20 : strtoul(argv[1], 0, 0);
  uint64_t m = (argc <= 2 ? 0 : strtoul(argv[2], 0, 0));

  uint64_t * uA= P99_MALLOC(uint64_t[n]);
  for (unsigned i = 0; i < n; ++i)
    uA[i] = p99_rand();
  qsort_s(uA, n, sizeof uA[0], uComp, m ? &m : 0);
  for (unsigned i = 0; i < n; ++i)
    printf("0x%016" PRIx64 "\n", uA[i]);
  free(uA);

  double * dA= P99_MALLOC(double[n]);
  for (unsigned i = 0; i < n; ++i)
    dA[i] = p99_drand();
  qsort_s(dA, n, sizeof dA[0], dComp, 0);
  for (unsigned i = 0; i < n; ++i)
    printf("%.15f\n", dA[i]);
  free(dA);
}
