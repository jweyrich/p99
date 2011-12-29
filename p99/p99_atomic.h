/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2011 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_ATOMIC_H
#define P99_ATOMIC_H 1

#include "p99_enum.h"

#ifndef __GNUC__
# error "P99 atomic needs gcc specific features"
#endif

/**
 ** @addtogroup atomic
 ** @{
 **/

/**
 ** @addtogroup atomic_macros
 ** @{
 **/

#define ATOMIC_BOOL_LOCK_FREE 2
#define ATOMIC_CHAR_LOCK_FREE 2
//#define ATOMIC_CHAR16_T_LOCK_FREE 2
//#define ATOMIC_CHAR32_T_LOCK_FREE 2
#define ATOMIC_WCHAR_T_LOCK_FREE 2
#define ATOMIC_SHORT_LOCK_FREE 2
#define ATOMIC_INT_LOCK_FREE 2
#define ATOMIC_LONG_LOCK_FREE 2
#define ATOMIC_LLONG_LOCK_FREE 2
#define ATOMIC_POINTER_LOCK_FREE 0

/**
 ** @memberof atomic_flag
 **/
#define ATOMIC_FLAG_INIT P99_ENC_INIT(0)

/**
 ** @memberof atomic_int
 **/
#define ATOMIC_VAR_INIT(V) P99_ENC_INIT(V)

/**
 ** @}
 **/


/**
 ** @addtogroup atomic_types
 ** @{
 **/

P99_ENC_DECLARE(int, atomic_flag);

/**
 ** @brief Atomic access to a value of type <code>int</code>
 **
 ** See the "public member function" section of this description for
 ** the operations that support this type
 **/
P99_ENC_DECLARE(int, atomic_int);

/**
 ** @brief Atomic access to a value of type <code>bool</code>
 ** @see atomic_load
 ** @see atomic_store
 ** @see atomic_compare_exchange_weak
 ** @see ATOMIC_VAR_INIT
 ** @see atomic_init
 **/
P99_ENC_DECLARE(_Bool, atomic_bool);

#define P00_DECLARE_ATOMIC(T, NAME)                                 \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
P99_ENC_DECLARE(T, NAME)

P00_DECLARE_ATOMIC(char, atomic_char);
P00_DECLARE_ATOMIC(signed char, atomic_schar);
P00_DECLARE_ATOMIC(unsigned char, atomic_uchar);
P00_DECLARE_ATOMIC(short, atomic_short);
P00_DECLARE_ATOMIC(unsigned short, atomic_ushort);
P00_DECLARE_ATOMIC(unsigned int, atomic_uint);
P00_DECLARE_ATOMIC(long, atomic_long);
P00_DECLARE_ATOMIC(unsigned long, atomic_ulong);
P00_DECLARE_ATOMIC(long long, atomic_llong);
P00_DECLARE_ATOMIC(unsigned long long, atomic_ullong);
//P00_DECLARE_ATOMIC(char16_t, atomic_char16_t);
//P00_DECLARE_ATOMIC(char32_t, atomic_char32_t);
P00_DECLARE_ATOMIC(wchar_t, atomic_wchar_t);
P00_DECLARE_ATOMIC(int_least8_t, atomic_int_least8_t);
P00_DECLARE_ATOMIC(uint_least8_t, atomic_uint_least8_t);
P00_DECLARE_ATOMIC(int_least16_t, atomic_int_least16_t);
P00_DECLARE_ATOMIC(uint_least16_t, atomic_uint_least16_t);
P00_DECLARE_ATOMIC(int_least32_t, atomic_int_least32_t);
P00_DECLARE_ATOMIC(uint_least32_t, atomic_uint_least32_t);
P00_DECLARE_ATOMIC(int_least64_t, atomic_int_least64_t);
P00_DECLARE_ATOMIC(uint_least64_t, atomic_uint_least64_t);
P00_DECLARE_ATOMIC(int_fast8_t, atomic_int_fast8_t);
P00_DECLARE_ATOMIC(uint_fast8_t, atomic_uint_fast8_t);
P00_DECLARE_ATOMIC(int_fast16_t, atomic_int_fast16_t);
P00_DECLARE_ATOMIC(uint_fast16_t, atomic_uint_fast16_t);
P00_DECLARE_ATOMIC(int_fast32_t, atomic_int_fast32_t);
P00_DECLARE_ATOMIC(uint_fast32_t, atomic_uint_fast32_t);
P00_DECLARE_ATOMIC(int_fast64_t, atomic_int_fast64_t);
P00_DECLARE_ATOMIC(uint_fast64_t, atomic_uint_fast64_t);
P00_DECLARE_ATOMIC(intptr_t, atomic_intptr_t);
P00_DECLARE_ATOMIC(uintptr_t, atomic_uintptr_t);
P00_DECLARE_ATOMIC(size_t, atomic_size_t);
P00_DECLARE_ATOMIC(ptrdiff_t, atomic_ptrdiff_t);
P00_DECLARE_ATOMIC(intmax_t, atomic_intmax_t);
P00_DECLARE_ATOMIC(uintmax_t, atomic_uintmax_t);

/**
 ** @}
 **/

/**
 ** @memberof atomic_flag
 **/
p99_inline
_Bool atomic_flag_test_and_set(volatile atomic_flag *object) {
  return __sync_val_compare_and_swap(&P99_ENCP(object), 0, 1);
}

/**
 ** @memberof atomic_flag
 **/
p99_inline
void atomic_flag_clear(volatile atomic_flag *object) {
  __sync_val_compare_and_swap(&P99_ENCP(object), 1, 0);
}

p99_inline
_Bool p00_is_lock_free(volatile void * objp, size_t size) {
  switch (size * CHAR_BIT) {
#ifdef UINT8_MAX
  case 8: return true;
#endif
#ifdef UINT16_MAX
  case 16: return true;
#endif
#ifdef UINT24_MAX
  case 24: return true;
#endif
#ifdef UINT32_MAX
  case 32: return true;
#endif
#ifdef UINT48_MAX
  case 48: return true;
#endif
#ifdef UINT64_MAX
  case 64: return true;
#endif
#ifdef UINT96_MAX
  case 96: return true;
#endif
#ifdef UINT128_MAX
  case 128: return true;
#endif
  default: return false;
  }
}

/**
 ** @memberof atomic_int
 **/
#define atomic_is_lock_free(OBJECT, DESIRED) p00_is_lock_free(&P99_ENCP(OBJECT), sizeof(P99_ENCP(OBJECT)))


/**
 ** @memberof atomic_int
 **/
#define atomic_init(OBJECT, VAL) ((void)(P99_ENCP(OBJECT) = (VAL)))

#define P00_STORE(N)                                                    \
do { /* empty */ }                                                      \
 while(!__sync_bool_compare_and_swap                                    \
       ((uint ## N ## _t volatile*)objp,                                \
        __sync_val_compare_and_swap((uint ## N ## _t volatile*)objp, 0, 0), \
        (uint ## N ## _t)desired))

p99_inline
void p00_store(volatile void * objp, size_t size, uintmax_t desired) {
  switch (size * CHAR_BIT) {
#ifdef UINT8_MAX
  case 8: P00_STORE(8); break;
#endif
#ifdef UINT16_MAX
  case 16: P00_STORE(16); break;
#endif
#ifdef UINT24_MAX
  case 24: P00_STORE(24); break;
#endif
#ifdef UINT32_MAX
  case 32: P00_STORE(32); break;
#endif
#ifdef UINT48_MAX
  case 48: P00_STORE(48); break;
#endif
#ifdef UINT64_MAX
  case 64: P00_STORE(64); break;
#endif
#ifdef UINT96_MAX
  case 96: P00_STORE(96); break;
#endif
#ifdef UINT128_MAX
  case 128: P00_STORE(128); break;
#endif
  }
}

/**
 ** @memberof atomic_int
 **/
#define atomic_store(OBJECT, DESIRED) p00_store(&P99_ENCP(OBJECT), sizeof(P99_ENCP(OBJECT)), (DESIRED))

/**
 ** @memberof atomic_int
 **/
#define atomic_load(OBJECT) __sync_val_compare_and_swap(&P99_ENCP(OBJECT), 0, 0)

p99_inline
_Bool p00_compare_exchange(uintmax_t val, uintmax_t expected, size_t size, void* expectedP) {
  _Bool ret = (val == expected);
  if (!ret) {
    switch (size * CHAR_BIT) {
#ifdef UINT8_MAX
    case 8: *(uint8_t*)expectedP = (uint8_t)val; break;
#endif
#ifdef UINT16_MAX
    case 16: *(uint16_t*)expectedP = (uint16_t)val; break;
#endif
#ifdef UINT24_MAX
    case 24: *(uint24_t*)expectedP = (uint24_t)val; break;
#endif
#ifdef UINT32_MAX
    case 32: *(uint32_t*)expectedP = (uint32_t)val; break;
#endif
#ifdef UINT48_MAX
    case 48: *(uint48_t*)expectedP = (uint48_t)val; break;
#endif
#ifdef UINT64_MAX
    case 64: *(uint64_t*)expectedP = (uint64_t)val; break;
#endif
#ifdef UINT96_MAX
    case 96: *(uint96_t*)expectedP = (uint96_t)val; break;
#endif
#ifdef UINT128_MAX
    case 128: *(uint128_t*)expectedP = (uint128_t)val; break;
#endif
    }
  }
  return ret;
}

/**
 ** @memberof atomic_int
 **/
#define atomic_compare_exchange_weak(OBJECT, EXPECTED, DESIRED)         \
p00_compare_exchange                                                    \
(__sync_val_compare_and_swap(&P99_ENCP(OBJECT), *(EXPECTED), (uintmax_t)(DESIRED)), \
 *(EXPECTED),                                                           \
 sizeof(*(EXPECTED)),                                                   \
 (EXPECTED))


/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_add(OBJECT, OPERAND) __sync_fetch_and_add(&P99_ENCP(OBJECT), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_sub(OBJECT, OPERAND) __sync_fetch_and_sub(&P99_ENCP(OBJECT), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_or(OBJECT, OPERAND) __sync_fetch_and_or(&P99_ENCP(OBJECT), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_and(OBJECT, OPERAND) __sync_fetch_and_and(&P99_ENCP(OBJECT), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_xor(OBJECT, OPERAND) __sync_fetch_and_xor(&P99_ENCP(OBJECT), OPERAND)


/**
 ** @addtogroup atomic_enum
 ** @{
 **/

P99_DECLARE_ENUM(memory_order,
                 memory_order_relaxed,
                 memory_order_consume,
                 memory_order_acquire,
                 memory_order_release,
                 memory_order_acq_rel,
                 memory_order_seq_cst
                 );


/**
 ** @}
 **/

/**
 ** @addtogroup fences Memory Fences
 **
 ** This is only rudimentary support for fences. Basically all fences
 ** but with argument ::memory_order_relaxed perform a full memory
 ** barrier.
 ** @{
 **/

p99_inline
void atomic_thread_fence(memory_order order) {
  switch (order) {
  case memory_order_relaxed: break;
  default: __sync_synchronize(); break;
  }
}

/**
 ** @remark In the current implementation a signal fence and a thread
 ** fence are the same full memory barrier.
 **/
#define atomic_signal_fence atomic_thread_fence

/**
 ** @}
 **/

/**
 ** @}
 **/

#endif
