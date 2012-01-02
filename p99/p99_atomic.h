/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2011-2012 Jens Gustedt, INRIA, France                      */
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
#include "p99_generic.h"

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
 ** @addtogroup atomic_types
 ** @{
 **/

P99_ENC_DECLARE(int, atomic_flag);

#define P00_AT(OBJP) ((OBJP)->p00_xval.p00_type_member)
#define P00_AI(OBJP) ((OBJP)->p00_xval.p00_integer_member)

#define P00_DECLARE_INT_STORE(T, NAME)                         \
p99_inline                                                     \
void P99_PASTE2(NAME, _store)(NAME * objp, T desired) {        \
  do { /* empty */ }                                           \
  while(!__sync_bool_compare_and_swap                          \
        (&P00_AT(objp),                                        \
         __sync_val_compare_and_swap(&P00_AT(objp), 0, 0),     \
         desired));                                            \
}


#define P00_DECLARE_INT_CMP_EXCHG_WEAK(T, NAME)                                                  \
p99_inline                                                                                       \
_Bool P99_PASTE2(NAME, _compare_exchange_weak)(T * objp, T * expected, size_t size, T desired) { \
  T val = __sync_val_compare_and_swap(objp, *expected, desired);                                 \
  _Bool ret = (*expected == val);                                                                \
  if (!ret) *expected = val;                                                                     \
  return ret;                                                                                    \
}


#define P00_DECLARE_ATOMIC(T, NAME)                                                                                    \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
union NAME;                                                                                                            \
typedef union NAME NAME;                                                                                               \
union NAME {                                                                                                           \
  atomic_flag p00_lock;                                                                                                \
  union {                                                                                                              \
    T p00_integer_member;                                                                                              \
    T p00_type_member;                                                                                                 \
  } p00_xval;                                                                                                          \
};                                                                                                                     \
P00_DECLARE_INT_STORE(T, NAME);                                                                                        \
P00_DECLARE_INT_CMP_EXCHG_WEAK(T, NAME)

/**
 ** @brief Atomic access to a value of type <code>int</code>
 **
 ** See the "public member function" section of this description for
 ** the operations that support this type
 **/
P00_DECLARE_ATOMIC(int, atomic_int);

/**
 ** @brief Atomic access to a value of type <code>bool</code>
 ** @see atomic_load
 ** @see atomic_store
 ** @see atomic_compare_exchange_weak
 ** @see ATOMIC_VAR_INIT
 ** @see atomic_init
 **/
P00_DECLARE_ATOMIC(_Bool, atomic_bool);


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

#define P99_DECLARE_ATOMIC(T, NAME)                                                                                    \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
struct NAME {                                                                                                          \
  atomic_flag p00_lock;                                                                                                \
  union {                                                                                                              \
    int p00_integer_member;                                                                                            \
    T p00_type_member;                                                                                                 \
  } p00_xval;                                                                                                          \
};                                                                                                                     \
P99_DECLARE_STRUCT(NAME)

p99_inline
void p00_atomic_store_ignore(void * objp, ...) {
  /* just ignore all */
}

p99_inline
_Bool p00_atomic_compare_exchange_weak_generic(void * objp, void * expected, size_t size, ...) {
  return !memcmp(objp, expected, size);
}

P99_DECLARE_ATOMIC(float, atomic_float);
P99_DECLARE_ATOMIC(double, atomic_double);


/**
 ** @}
 **/

/**
 ** @memberof atomic_flag
 **/
#define atomic_flag_test_and_set_explicit(OBJP, ORDER)                                        \
({                                                                                            \
  _Bool ret;                                                                                  \
  switch (ORDER) {                                                                            \
  case memory_order_relaxed: ;                                                                \
  case memory_order_acquire: ret = __sync_lock_test_and_set(&P99_ENCP(OBJP), 1); break;       \
  default:                   ret = __sync_val_compare_and_swap(&P99_ENCP(OBJP), 0, 1); break; \
  }                                                                                           \
  ret;                                                                                        \
 })

/**
 ** @memberof atomic_flag
 **/
#define atomic_flag_test_and_set(OBJP) atomic_flag_test_and_set_explicit((OBJP), memory_order_seq_cst)

/**
 ** @memberof atomic_flag
 **/
#define atomic_flag_clear_explicit(OBJP, ORDER)                                         \
(void)({                                                                                \
  switch(ORDER) {                                                                       \
  case memory_order_relaxed: ;                                                          \
  case memory_order_release: __sync_lock_release(&P99_ENCP(OBJP)); break;               \
  default:                   __sync_val_compare_and_swap(&P99_ENCP(OBJP), 1, 0); break; \
  }                                                                                     \
 })

/**
 ** @memberof atomic_flag
 **/
#define atomic_flag_clear(OBJP) atomic_flag_clear_explicit((OBJP), memory_order_seq_cst)

/**
 ** @brief extension: spin on @a object setting the flag until the state before was "clear"
 **
 ** This interprets an ::atomic_flag as a spinlock. State "clear"
 ** means unlocked and state "set" means locked.
 **
 ** @memberof atomic_flag
 **/
#define atomic_flag_lock(OBJP)                                                       \
(void)({ while (atomic_flag_test_and_set_explicit((OBJP), memory_order_acquire)); })

/**
 ** @brief extension: set the flag and return true if we are the first
 ** to do so
 **
 ** This interprets an ::atomic_flag as a spinlock. State "clear"
 ** means unlocked and state "set" means locked.
 **
 ** @memberof atomic_flag
 **/
#define atomic_flag_trylock(OBJP) !atomic_flag_test_and_set_explicit((OBJP), memory_order_acquire)

#define atomic_flag_unlock(OBJP) atomic_flag_clear_explicit((OBJP), memory_order_release)

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
#define atomic_is_lock_free(OBJP, DESIRED) p00_is_lock_free(&P00_AT(OBJP), sizeof(P00_AT(OBJP)))


/**
 ** @memberof atomic_int
 **/
#define atomic_init(OBJP, VAL) ((void)(P00_AT(OBJP) = (VAL)))

#define p00_store(OBJP, size, desired)                         \
P99_GENERIC                                                    \
(P00_AT(OBJP),                                                 \
 (                                                             \
  ({ __typeof__(OBJP) o = (OBJP);                              \
    atomic_flag_lock(&o->p00_lock);                            \
    (void)(P00_AT(o) = desired);                               \
    atomic_flag_unlock(&o->p00_lock);                          \
  }),                                                          \
  p00_atomic_store_ignore),                                    \
 (_Bool,              atomic_bool_store),                      \
 (char,               atomic_char_store),                      \
 (signed char,        atomic_schar_store),                     \
 (unsigned char,      atomic_uchar_store),                     \
 (signed short,       atomic_short_store),                     \
 (unsigned short,     atomic_ushort_store),                    \
 (signed,             atomic_int_store),                       \
 (unsigned,           atomic_uint_store),                      \
 (signed long,        atomic_long_store),                      \
 (unsigned long,      atomic_ulong_store),                     \
 (signed long long,   atomic_llong_store),                     \
 (unsigned long long, atomic_ullong_store))(OBJP, desired)

/**
 ** @memberof atomic_int
 **/
#define atomic_store(OBJP, DESIRED) p00_store((OBJP), sizeof(P00_AT(OBJP)), (DESIRED))

#define P00_LOAD(OBJP)                                         \
  ({ __typeof__(OBJP) o = (OBJP);                              \
    atomic_flag_lock(&o->p00_lock);                            \
   __typeof__(P00_AT(o)) ret = P00_AT(o);                      \
   atomic_flag_unlock(&o->p00_lock);                           \
   (__typeof__(P00_AT(o)))ret;                                 \
  })

/**
 ** @memberof atomic_int
 **/
#define atomic_load(OBJP)                                                \
P99_GENERIC                                                              \
(P00_AT(OBJP),                                                           \
 P00_LOAD(OBJP),                                                         \
 (_Bool,              __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (char,               __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (signed char,        __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (unsigned char,      __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (signed short,       __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (unsigned short,     __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (signed,             __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (unsigned,           __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (signed long,        __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (unsigned long,      __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (signed long long,   __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)), \
 (unsigned long long, __sync_val_compare_and_swap(&P00_AI(OBJP), 0, 0)))

/**
 ** @memberof atomic_int
 **/
#define atomic_compare_exchange_weak(OBJP, EXPECTED, DESIRED)                          \
({                                                                                     \
  __typeof__(OBJP) o = (OBJP);                                                         \
  __typeof__(EXPECTED) p00_exp = (EXPECTED);                                           \
  __typeof__(EXPECTED) p00_des = (DESIRED);                                            \
  if (!P99_TYPE_INTEGER(P00_AT(o))) atomic_flag_lock(&o->p00_lock);                    \
  _Bool p00_ret = P99_GENERIC                                                          \
    (P00_AT(o),                                                                        \
     p00_atomic_compare_exchange_weak_generic,                                         \
     (_Bool,              atomic_bool_compare_exchange_weak),                          \
     (char,               atomic_char_compare_exchange_weak),                          \
     (signed char,        atomic_schar_compare_exchange_weak),                         \
     (unsigned char,      atomic_uchar_compare_exchange_weak),                         \
     (signed short,       atomic_short_compare_exchange_weak),                         \
     (unsigned short,     atomic_ushort_compare_exchange_weak),                        \
     (signed,             atomic_int_compare_exchange_weak),                           \
     (unsigned,           atomic_uint_compare_exchange_weak),                          \
     (signed long,        atomic_long_compare_exchange_weak),                          \
     (unsigned long,      atomic_ulong_compare_exchange_weak),                         \
     (signed long long,   atomic_llong_compare_exchange_weak),                         \
     (unsigned long long, atomic_ullong_compare_exchange_weak))                        \
    (&P00_AT(o), p00_exp, sizeof P00_AT(o), p00_des);                                  \
  if (!P99_TYPE_INTEGER(P00_AT(o))) {                                                  \
    /* Both, *EXPECTED and DESIRED must be assignment compatible with the base type */ \
    if (p00_ret) *p00_exp = P00_AT(o);                                                 \
    else         P00_AT(o) = p00_des;                                                  \
    atomic_flag_unlock(&o->p00_lock);                                                  \
  }                                                                                    \
  p00_ret;                                                                             \
 })




/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_add(OBJP, OPERAND) __sync_fetch_and_add(&P00_AT(OBJP), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_sub(OBJP, OPERAND) __sync_fetch_and_sub(&P00_AT(OBJP), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_or(OBJP, OPERAND) __sync_fetch_and_or(&P00_AT(OBJP), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_and(OBJP, OPERAND) __sync_fetch_and_and(&P00_AT(OBJP), OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_xor(OBJP, OPERAND) __sync_fetch_and_xor(&P00_AT(OBJP), OPERAND)


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
