/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2012, 2014 Jens Gustedt, INRIA, France                      */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_ATOMIC_GCC_SYNC_H
#define P99_ATOMIC_GCC_SYNC_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif

#include "p99_args.h"

/**
 ** @file
 **
 ** @brief Implement some of the atomic operations as fallback to gcc'
 ** @c __sync builtins.
 **/

#define p00_atomic_exchange_n_(PTR, DES, ORD, ...)                      \
p99_extension ({                                                        \
    register __typeof__(*(PTR)) volatile* p00_ptr = (PTR);              \
    register __typeof__(*(PTR)) p00_des = (DES);                        \
    register __typeof__(*(PTR)) p00_ret = p00_des;                      \
    for (;;) {                                                          \
      register __typeof__(*(PTR)) p00_val = __sync_val_compare_and_swap(p00_ptr, p00_ret, p00_des); \
      if (P99_LIKELY(p00_val == p00_ret)) break;                        \
      p00_ret = p00_val;                                                \
    }                                                                   \
    p00_ret = p00_ret;                                                  \
  })

#define p00_atomic_exchange_n(...) p00_atomic_exchange_n_(__VA_ARGS__,,)

#define p00_mfence(...) __sync_synchronize()

#define p00_sync_lock_release_(OBJ, ORD, ...)                           \
p99_extension ({                                                        \
  p00_atomic_flag* p00_obj = (OBJ);                                     \
  /* __sync_lock_release only has release consistency */                \
  /* the fence must come before so nothing can be reordered after */    \
  switch (ORD) {                                                        \
  case memory_order_consume: ;                                          \
  case memory_order_acquire: ;                                          \
  case memory_order_acq_rel: ;                                          \
  case memory_order_seq_cst: ;                                          \
    p00_mfence();                                                       \
  }                                                                     \
  __sync_lock_release(p00_obj);                                         \
 })

#define p00_sync_lock_test_and_set_(OBJ, ORD, ...)                      \
p99_extension ({                                                        \
  p00_atomic_flag* p00_obj = (OBJ);                                     \
  /* __sync_lock_test_and_set only has acquire consistency */           \
  int ret = __sync_lock_test_and_set(p00_obj, 1);                       \
  /* the fence must come after so nothing can be reordered before */    \
  switch (ORD) {                                                        \
  case memory_order_consume: ;                                          \
  case memory_order_release: ;                                          \
  case memory_order_acq_rel: ;                                          \
  case memory_order_seq_cst: ;                                          \
    p00_mfence();                                                       \
  }                                                                     \
  ret = ret;                                                            \
 })

#define p00_sync_lock_release(...) p00_sync_lock_release_(__VA_ARGS__, memory_order_seq_cst, )
#define p00_sync_lock_test_and_set(...) p00_sync_lock_test_and_set_(__VA_ARGS__, memory_order_seq_cst, )

#define p00_atomic_compare_exchange_n_(PTR, EXP, DES, WEAK, SUC, FAI, ...) \
p99_extension ({                                                        \
    __typeof__(*PTR) volatile* p00_ptr2 = (PTR);                         \
    __typeof__(EXP) p00_exp2 = (EXP);                                    \
    __typeof__(DES) const p00_des2 = (DES);                              \
    __typeof__(*PTR) p00_val2 = __sync_val_compare_and_swap(p00_ptr2, *p00_exp2, p00_des2); \
    _Bool p00_ret2 = (*p00_exp2 == p00_val2);                              \
    if (!p00_ret2) *p00_exp2 = p00_val2;                                   \
    p00_ret2 = p00_ret2;                                                  \
  })

#define p00_atomic_compare_exchange_n(...)                              \
  P99_IF_EQ(P99_NARG(__VA_ARGS__), 3)                                   \
  (p00_atomic_compare_exchange_n_(__VA_ARGS__, 0, memory_order_seq_cst, memory_order_seq_cst)) \
  (p00_atomic_compare_exchange_n_(__VA_ARGS__, memory_order_seq_cst, memory_order_seq_cst, ))


#endif
