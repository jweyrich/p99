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
#ifndef P99_ATOMIC_GCC_ATOMIC_H
#define P99_ATOMIC_GCC_ATOMIC_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif

#include "p99_args.h"

/**
 ** @file
 **
 ** @brief Implement some of the atomic operations as fallback to gcc'
 ** @c __atomic builtins.
 **/


#define P00_ATOMIC_EXCHANGE_DECLARE(T, N)                             \
p99_inline                                                            \
T P99_PASTE(p00_atomic_exchange_, N)(T volatile * p00_p, T p00_des) { \
  T p00_ret = *p00_p;                                                 \
  for (;;) {                                                          \
    T p00_val = __sync_val_compare_and_swap(p00_p, p00_ret, p00_des); \
    if (P99_LIKELY(p00_val == p00_ret)) break;                        \
    p00_ret = p00_val;                                                \
  }                                                                   \
  return p00_ret;                                                     \
}                                                                     \
P99_MACRO_END(P00_ATOMIC_EXCHANGE_DECLARE)

P00_ATOMIC_EXCHANGE_DECLARE(uint8_t, 1);
P00_ATOMIC_EXCHANGE_DECLARE(uint16_t, 2);
P00_ATOMIC_EXCHANGE_DECLARE(uint32_t, 4);
P00_ATOMIC_EXCHANGE_DECLARE(uint64_t, 8);

#define p00_atomic_clear(OBJ, ORD)              \
p99_extension ({                                \
  p00_atomic_flag* p00_obj = (OBJ);             \
  __atomic_clear(p00_obj, (ORD));               \
 })

#define p00_atomic_test_and_set(OBJ, ORD)       \
p99_extension ({                                \
  p00_atomic_flag* p00_obj = (OBJ);             \
  __atomic_test_and_set(p00_obj, (ORD));        \
 })

#define p00_mfence(...)                         \
 P99_IF_EMPTY(__VA_ARGS__)                      \
 (__atomic_thread_fence(__ATOMIC_SEQ_CST))      \
 (__atomic_thread_fence(__VA_ARGS__))

#define p00_sync_lock_release(...)                      \
  P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                   \
  (p00_atomic_clear(__VA_ARGS__, __ATOMIC_SEQ_CST))     \
  (p00_atomic_clear(__VA_ARGS__))

#define p00_sync_lock_test_and_set(...)                         \
  P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                           \
  (p00_atomic_test_and_set(__VA_ARGS__, __ATOMIC_SEQ_CST))      \
  (p00_atomic_test_and_set(__VA_ARGS__))

#define p00_atomic_compare_exchange_n_(PTR, EXP, DES, WEAK, SUC, FAI, ...) \
p99_extension ({                                                        \
    __typeof__(*(PTR)) volatile* p00_ptr2 = (PTR);                      \
    __typeof__(*(PTR)) volatile* p00_exp2 = (EXP);                      \
    __typeof__(*(PTR)) p00_des2 = (DES);                                \
    __atomic_compare_exchange_n(p00_ptr2, (void*)p00_exp2, p00_des2, (WEAK), (SUC), (FAI)); \
  })

#define p00_atomic_compare_exchange_n(...)                              \
  P99_IF_EQ(P99_NARG(__VA_ARGS__), 3)                                   \
  (p00_atomic_compare_exchange_n_(__VA_ARGS__, 0, memory_order_seq_cst, memory_order_seq_cst)) \
  (p00_atomic_compare_exchange_n_(__VA_ARGS__, memory_order_seq_cst, memory_order_seq_cst, ))

#endif
