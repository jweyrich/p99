/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_QSORT_H_
#define P99_QSORT_H_
#include "p99_rand.h"

/** @file
 **
 ** @brief An implementation of a sorting routine
 **/

/**
 ** @addtogroup sorting
 ** @{
 **/

P99_DECLARE_STRUCT(p00_qsort);

struct p00_qsort {
  size_t bot;
  size_t top;
};

#define P00_QSWAP_MEMPCP(P, A, B, S)            \
do {                                            \
  void * p00Ma = (P)+(A);                       \
  void * p00Mb = (P)+(B);                       \
  size_t p00Ms = (S);                           \
  /* if (p00Ma != p00Mb) */ {                   \
    memcpy(&p00_tmp, p00Ma, p00Ms);             \
    memcpy(p00Ma, p00Mb, p00Ms);                \
    memcpy(p00Mb, &p00_tmp, p00Ms);             \
  }                                             \
} while (false)

#define P00_QSWAP_ASSIGN(P, A, B, S)            \
do {                                            \
  p00_tmp = (P)[A];                             \
  (P)[A] = (P)[B];                              \
  (P)[B] = p00_tmp;                             \
 } while (false)

#define P00_QCOMP(A, B) p00_comp(&p00_B[A], &p00_B[B], p00_ctx)

#define P00_QPUSH(P, B, T)                      \
do {                                            \
  register p00_qsort *const p00Mp = ++(P);      \
  p00Mp->bot = (B);                             \
  p00Mp->top = (T);                             \
 } while (false)

#define P00_QTOP(P, B, T)                       \
  register size_t const B = (P)->bot;           \
  register size_t const T = (P)->top

#define P00_QEMPTY(S, P) ((S) == (P))

#define P00_ILOG2_INNER(_0, XI, _2)              \
{                                               \
  P99_CONSTANT(int, p00_r, (1 << (XI)));        \
  if (p00_i >= (UINTMAX_C(1) << p00_r)) {       \
    p00_n += p00_r;                             \
    p00_i >>= p00_r;                            \
  }                                             \
}

p99_inline
signed p99_ilog2(uintmax_t p00_i) {
  if (!p00_i) return -1;
  register unsigned p00_n = 0;
#if (UINTMAX_MAX/2 + 1) > (1ULL << 63)
  P99_FOR(, 7, P00_SER, P00_ILOG2_INNER, 6, 5, 4, 3, 2, 1, 0);
#else
  P99_FOR(, 6, P00_SER, P00_ILOG2_INNER, 5, 4, 3, 2, 1, 0);
#endif
  return p00_n;
}


#define P00_QSORT_BODY(SWAP)                                            \
if (p00_n > RSIZE_MAX || p00_s > RSIZE_MAX) return ERANGE;              \
if (p00_n && (!p00_base || !p00_comp)) return EINVAL;                   \
do {                                                                    \
  register p99_seed *const seed = p99_seed_get();                       \
  /* Initialize a stack of states */                                    \
  P99_CONSTANT(int, p00_tail, 20);                                      \
  /* provide a reasonable stack of states */                            \
  p00_qsort p00_a[(p99_ilog2(p00_n) + 1)*p00_tail];                     \
  p00_qsort* p00_p = p00_a;                                             \
  P00_QPUSH(p00_p, 0, p00_n);                                           \
  while(!P00_QEMPTY(p00_a, p00_p)) {                                    \
    /* read the values from the stack */                                \
    P00_QTOP(p00_p, p00_bot, p00_top);                                  \
    register size_t const p00_len = p00_top - p00_bot;                  \
                                                                        \
    switch (p00_len) {                                                  \
      /* special cases all fall through. */                             \
    case 2:                                                             \
      if (P00_QCOMP(p00_bot, p00_bot + 1) > 0)                          \
        SWAP(p00_B, p00_bot, p00_bot + 1, p00_s);                       \
    case 1: ;                                                           \
    case 0:                                                             \
      --p00_p;                                                          \
      break;                                                            \
    default:                                                            \
      /* move the pivot to the bottom element */                        \
      {                                                                 \
        size_t p00_c = p99_rand(seed) % p00_len;                        \
        if (p00_c) SWAP(p00_B, p00_bot, p00_bot + p00_c, p00_s);        \
      }                                                                 \
                                                                        \
      /* The running variables. The fact that p00_b is > p00_bot,       \
         guarantees progress. */                                        \
      register size_t p00_b = p00_bot + 1;                              \
      register size_t p00_t = p00_top;                                  \
                                                                        \
      while (P99_LIKELY(p00_b < p00_t)) {                               \
        /* Try to find two misplaced elements. */                       \
        while (!(P00_QCOMP(p00_bot, p00_b) < 0)) {                      \
          ++p00_b;                                                      \
          if (P99_UNLIKELY(p00_b == p00_t)) goto P00_RECURSE;           \
        }                                                               \
        while (P00_QCOMP(p00_bot, p00_t - 1) < 0){                      \
          --p00_t;                                                      \
          if (P99_UNLIKELY(p00_b == p00_t)) goto P00_RECURSE;           \
        }                                                               \
                                                                        \
        /* Two distinct misplaced elements are found. */                \
        --p00_t;                                                        \
        SWAP(p00_B, p00_b, p00_t, p00_s);                               \
        ++p00_b;                                                        \
      }                                                                 \
    P00_RECURSE:                                                        \
      if ((p00_b - p00_bot) > 1) {                                      \
        /* The pivot is always a maximal element in the lower part */   \
        --p00_b;                                                        \
        SWAP(p00_B, p00_bot, p00_b, p00_s);                             \
        /* Move all elements that compare equal adjacent */             \
        register size_t const p00_bb = p00_b;                           \
        for (register size_t p00_c = p00_bot;;) {                       \
          if (P00_QCOMP(p00_bb, p00_c) <= 0) {                          \
            --p00_b;                                                    \
            if (P99_UNLIKELY(p00_c == p00_b)) break;                    \
            SWAP(p00_B, p00_c, p00_b, p00_s);                           \
          } else {                                                      \
            ++p00_c;                                                    \
            if (P99_UNLIKELY(p00_c == p00_b)) break;                    \
          }                                                             \
        }                                                               \
      }                                                                 \
      /* Register the "recursive" calls into the two halfs. Most */     \
      /* recursive calls are actually close to the bottom, so we */     \
      /* rarely have to do the check if they are small enough.   */     \
      if (P99_UNLIKELY(p00_len >= p00_tail)) {                          \
        size_t p00Mmax = p00_len*0.99;                                  \
        /* If the split isn't good enough we just try again     */      \
        if (P99_UNLIKELY(((p00_b - p00_bot) >= p00Mmax)                 \
                         ||                                             \
                         ((p00_top - p00_t) >= p00Mmax)))               \
          continue;                                                     \
      }                                                                 \
      --p00_p;                                                          \
      if ((p00_b - p00_bot) > 1) P00_QPUSH(p00_p, p00_bot, p00_b);      \
      if ((p00_top - p00_t) > 1) P00_QPUSH(p00_p, p00_t, p00_top);      \
    }                                                                   \
  }                                                                     \
  return 0;                                                             \
 } while(false)


P99_WEAK(p99_qsort_generic)
errno_t p99_qsort_generic(void *p00_base,
                rsize_t p00_n,
                rsize_t p00_s,
                int (*p00_comp)(const void *, const void *, void *),
                void *p00_ctx) {
  typedef unsigned char p00_el[p00_s];
  register p00_el *const p00_B = p00_base;
  p00_el p00_tmp;
  P00_QSORT_BODY(P00_QSWAP_MEMPCP);
}


#define P00_QSORT_DECLARE(T)                                            \
p99_inline                                                              \
errno_t P99_PASTE2(p99_qsort_, T)(void *p00_base,                       \
                    rsize_t p00_n,                                      \
                    rsize_t p00_s,                                      \
                    int (*p00_comp)(const void *, const void *, void *), \
                    void *p00_ctx) {                                    \
  register T *const p00_B = p00_base;                                   \
  _Alignas(sizeof(max_align_t)) T p00_tmp;                              \
  P00_QSORT_BODY(P00_QSWAP_ASSIGN);                                     \
}                                                                       \
P99_MACRO_END(p99_qsort_, T)

P00_QSORT_DECLARE(_Bool);
P00_QSORT_DECLARE(schar);
P00_QSORT_DECLARE(uchar);
P00_QSORT_DECLARE(char);
P00_QSORT_DECLARE(short);
P00_QSORT_DECLARE(ushort);
P00_QSORT_DECLARE(signed);
P00_QSORT_DECLARE(unsigned);
P00_QSORT_DECLARE(long);
P00_QSORT_DECLARE(ulong);
P00_QSORT_DECLARE(llong);
P00_QSORT_DECLARE(ullong);
P00_QSORT_DECLARE(float);
P00_QSORT_DECLARE(double);
P00_QSORT_DECLARE(ldouble);
#ifndef __STDC_NO_COMPLEX__
P00_QSORT_DECLARE(cfloat);
P00_QSORT_DECLARE(cdouble);
P00_QSORT_DECLARE(cldouble);
#endif

P00_QSORT_DECLARE(void_ptr);

P00_QSORT_DECLARE(size_t);
P00_QSORT_DECLARE(ptrdiff_t);
P00_QSORT_DECLARE(intptr_t);
P00_QSORT_DECLARE(uintptr_t);

#ifdef UINT8_MAX
P00_QSORT_DECLARE(uint8_t);
#endif
#ifdef UINT16_MAX
P00_QSORT_DECLARE(uint16_t);
#endif
#ifdef UINT32_MAX
P00_QSORT_DECLARE(uint32_t);
#endif
#ifdef UINT64_MAX
P00_QSORT_DECLARE(uint64_t);
#endif
#ifdef UINT128_MAX
P00_QSORT_DECLARE(uint128_t);
#else
# ifdef P99X_UINT128_MAX
P00_QSORT_DECLARE(p99x_uint128);
# endif
#endif

p99_inline
errno_t p99_qsort_s(void *p00_base,
                    rsize_t p00_n,
                    rsize_t p00_s,
                    int (*p00_comp)(const void *, const void *, void *),
                    void *p00_ctx) {
  switch (p00_s) {
#ifdef UINT8_MAX
  case sizeof(uint8_t): return p99_qsort_uint8_t(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
#endif
#ifdef UINT16_MAX
  case sizeof(uint16_t): return p99_qsort_uint16_t(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
#endif
#ifdef UINT32_MAX
  case sizeof(uint32_t): return p99_qsort_uint32_t(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
#endif
#ifdef UINT64_MAX
  case sizeof(uint64_t): return p99_qsort_uint64_t(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
#endif
#ifdef UINT128_MAX
  case sizeof(uint128_t): return p99_qsort_uint128_t(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
#else
# ifdef P99X_UINT128_MAX
  case sizeof(p99x_uint128): return p99_qsort_p99x_uint128(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
# endif
#endif
  default: return p99_qsort_generic(p00_base, p00_n, p00_s, p00_comp, p00_ctx);
  }
}

#ifdef __STDC_NO_COMPLEX__
#define qsort_s(B, N, S, CMP, CTX)                                      \
  P99_CONSTRAINT_TRIGGER(                                               \
  P99_GENERIC(&((B)[0]),                                                \
            p99_qsort_s,                                                \
            (void_ptr*, p99_qsort_void_ptr),                            \
            /* */                                                       \
            (float*, p99_qsort_float),                                  \
            (double*, p99_qsort_double),                                \
            (double*, p99_qsort_cdouble),                               \
            /* */                                                       \
            (_Bool*, p99_qsort__Bool),                                  \
            (char*, p99_qsort_char),                                    \
            (uchar*, p99_qsort_uchar),                                  \
            (schar*, p99_qsort_schar),                                  \
            /* */                                                       \
            (ushort*, p99_qsort_ushort),                                \
            (short*, p99_qsort_short),                                  \
            /* */                                                       \
            (unsigned*, p99_qsort_unsigned),                            \
            (signed*, p99_qsort_signed),                                \
            /* */                                                       \
            (long*, p99_qsort_long),                                    \
            (ulong*, p99_qsort_ulong),                                  \
            /* */                                                       \
            (llong*, p99_qsort_llong),                                  \
            (ullong*, p99_qsort_ullong)                                 \
              )((B), (N), (S), (CMP), (CTX)),                           \
  "qsort_s runtime constraint violation")
#else
#define qsort_s(B, N, S, CMP, CTX)                                      \
  P99_CONSTRAINT_TRIGGER(                                               \
  P99_GENERIC(&((B)[0]),                                                \
            p99_qsort_s,                                                \
            (void_ptr*, p99_qsort_void_ptr),                            \
            /* */                                                       \
            (float*, p99_qsort_float),                                  \
            (double*, p99_qsort_double),                                \
            (ldouble*, p99_qsort_ldouble),                              \
            /* */                                                       \
            (cfloat*, p99_qsort_cfloat),                                \
            (cdouble*, p99_qsort_cdouble),                              \
            (cldouble*, p99_qsort_cldouble),                            \
            /* */                                                       \
            (_Bool*, p99_qsort__Bool),                                  \
            (char*, p99_qsort_char),                                    \
            (uchar*, p99_qsort_uchar),                                  \
            (schar*, p99_qsort_schar),                                  \
            /* */                                                       \
            (ushort*, p99_qsort_ushort),                                \
            (short*, p99_qsort_short),                                  \
            /* */                                                       \
            (unsigned*, p99_qsort_unsigned),                            \
            (signed*, p99_qsort_signed),                                \
            /* */                                                       \
            (long*, p99_qsort_long),                                    \
            (ulong*, p99_qsort_ulong),                                  \
            /* */                                                       \
            (llong*, p99_qsort_llong),                                  \
            (ullong*, p99_qsort_ullong)                                 \
              )((B), (N), (S), (CMP), (CTX)),                           \
  "qsort_s runtime constraint violation")
#endif

p99_inline
rsize_t p99_mismatch(void *p00_base,
                     rsize_t p00_n,
                     rsize_t p00_s,
                     int (*p00_comp)(const void *, const void *, void *),
                     void *p00_ctx) {
  typedef unsigned char p00_el[p00_s];
  p00_el *const p00_B = p00_base;
  for (register rsize_t p00_r = 1; p00_r < p00_n; ++p00_r) {
    if (p00_comp(&p00_B[p00_r - 1], &p00_B[p00_r], p00_ctx) > 0)
      return p00_r;
  }
  return p00_n;
}

p99_inline
bool p99_is_sorted(void *p00_base,
                      rsize_t p00_n,
                      rsize_t p00_s,
                      int (*p00_comp)(const void *, const void *, void *),
                      void *p00_ctx) {
  return !(p99_mismatch(p00_base, p00_n, p00_s, p00_comp, p00_ctx) < p00_n);
}


/**
 ** @}
 **/

#endif
