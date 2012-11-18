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
#ifndef P99_ATOMIC_GCC_H
#define P99_ATOMIC_GCC_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif

/**
 ** @file
 **
 ** @brief Implement some of the atomic operations as fallback to gcc'
 ** builtins.
 **/


#define P00_ATOMIC_FETCH_AND_STORE_DECLARE(T, N)                             \
p99_inline                                                                   \
T P99_PASTE(p00_atomic_fetch_and_store_, N)(T volatile * p00_p, T p00_des) { \
  T p00_ret = *p00_p;                                                        \
  for (;;) {                                                                 \
    T p00_val = __sync_val_compare_and_swap(p00_p, p00_ret, p00_des);        \
    if (P99_LIKELY(p00_val == p00_ret)) break;                               \
    p00_ret = p00_val;                                                       \
  }                                                                          \
  return p00_ret;                                                            \
}                                                                            \
P99_MACRO_END(P00_ATOMIC_FETCH_AND_STORE_DECLARE)

P00_ATOMIC_FETCH_AND_STORE_DECLARE(uint8_t, 1);
P00_ATOMIC_FETCH_AND_STORE_DECLARE(uint16_t, 2);
P00_ATOMIC_FETCH_AND_STORE_DECLARE(uint32_t, 4);
P00_ATOMIC_FETCH_AND_STORE_DECLARE(uint64_t, 8);

p99_inline void p00_mfence(void) {  __sync_synchronize(); }

# define p00_sync_lock_release(X) __sync_lock_release((X))
# define p00_sync_lock_test_and_set(X) __sync_lock_test_and_set((X), 1)


#endif
