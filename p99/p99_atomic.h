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
#define ATOMIC_VAR_INIT(V) { .p00_xval = { .p00_type_member = (V), }, }

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
_Bool P99_PASTE2(NAME, _compare_exchange_weak)(T volatile* objp, T * expected, T desired) {      \
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

#define P99_ATOMIC(T)                                                   \
struct {                                                                \
  atomic_flag p00_lock;                                                 \
  union {                                                               \
    /* should be an integer type that can be cast to pointers without warning */ \
    uintptr_t p00_integer_member;                                       \
    T p00_type_member;                                                  \
  } p00_xval;                                                           \
}

#define P99_DECLARE_ATOMIC(T, NAME)                                                                                    \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
typedef P99_ATOMIC(T) NAME

p99_inline
uintptr_t p00_atomic_ignore(void volatile* objp, ...) {
  /* just ignore all */
  return 0;
}

P99_DECLARE_ATOMIC(float, atomic_float);
P99_DECLARE_ATOMIC(double, atomic_double);
P99_DECLARE_ATOMIC(long double, atomic_ldouble);


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

/**
 ** @memberof atomic_int
 **/
#define atomic_is_lock_free(OBJP) !offsetof(__typeof__(*(OBJP)), p00_xval)

/**
 ** @memberof atomic_int
 **/
#define atomic_init(OBJP, VAL) ((void)(P00_AT(OBJP) = (VAL)))

/**
 ** @memberof atomic_int
 **/
#define atomic_store(OBJP, DESIRED)                     \
({                                                      \
  __typeof__(OBJP) p00_objp = (OBJP);                   \
  __typeof__(DESIRED) p00_des = (DESIRED);              \
  P99_GENERIC                                           \
    (P00_AT(p00_objp),                                  \
     (                                                  \
      ({                                                \
        atomic_flag_lock(&p00_objp->p00_lock);          \
        P00_AT(p00_objp) = p00_des;                     \
        atomic_flag_unlock(&p00_objp->p00_lock);        \
      }),                                               \
      p00_atomic_ignore),                               \
     (_Bool,              atomic_bool_store),           \
     (char,               atomic_char_store),           \
     (signed char,        atomic_schar_store),          \
     (unsigned char,      atomic_uchar_store),          \
     (signed short,       atomic_short_store),          \
     (unsigned short,     atomic_ushort_store),         \
     (signed,             atomic_int_store),            \
     (unsigned,           atomic_uint_store),           \
     (signed long,        atomic_long_store),           \
     (unsigned long,      atomic_ulong_store),          \
     (signed long long,   atomic_llong_store),          \
     (unsigned long long, atomic_ullong_store))         \
    (p00_objp, p00_des);                                \
 })

/**
 ** @memberof atomic_int
 **/
#define atomic_load(OBJP)                                               \
({                                                                      \
  __typeof__(OBJP) p00_objp = (OBJP);                                   \
  __typeof__(P00_AT(p00_objp)) p00_ret;                                 \
  /* cast to the same type to be sure that the result is an lvalue */   \
  (__typeof__(P00_AT(p00_objp)))                                        \
    P99_GENERIC                                                         \
    (p00_ret,                                                           \
     ({                                                                 \
       atomic_flag_lock(&p00_objp->p00_lock);                           \
       p00_ret = P00_AT(p00_objp);                                      \
       atomic_flag_unlock(&p00_objp->p00_lock);                         \
       p00_ret;                                                         \
     }),                                                                \
     (_Bool,              __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (char,               __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (signed char,        __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (unsigned char,      __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (signed short,       __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (unsigned short,     __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (signed,             __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (unsigned,           __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (signed long,        __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (unsigned long,      __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (signed long long,   __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0)), \
     (unsigned long long, __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0))); \
 })

/**
 ** @memberof atomic_int
 **/
#define atomic_compare_exchange_weak(OBJP, EXPECTED, DESIRED)           \
({                                                                      \
  _Bool p00_ret;                                                        \
  __typeof__(OBJP) p00_objp = (OBJP);                                   \
  __typeof__(EXPECTED) p00_exp = (EXPECTED);                            \
  __typeof__(DESIRED) p00_des = (DESIRED);                              \
  if (!atomic_is_lock_free(p00_objp)) {                                 \
    atomic_flag_lock(&p00_objp->p00_lock);                              \
    p00_ret = !memcmp(p00_exp, (void*)&P00_AT(p00_objp), sizeof P00_AT(p00_objp)); \
    /* Both, *EXPECTED and DESIRED must be assignment compatible        \
       with the base type */                                            \
    if (!p00_ret) *p00_exp = P00_AT(p00_objp);                          \
    else         P00_AT(p00_objp) = p00_des;                            \
    atomic_flag_unlock(&p00_objp->p00_lock);                            \
  } else {                                                              \
    __typeof__(P00_AI(p00_objp)) p00_desi =                             \
      P99_GENERIC                                                       \
      (P00_AT(p00_objp),                                                \
       0,                                                               \
       (_Bool,              p00_des),                                   \
       (char,               p00_des),                                   \
       (signed char,        p00_des),                                   \
       (unsigned char,      p00_des),                                   \
       (signed short,       p00_des),                                   \
       (unsigned short,     p00_des),                                   \
       (signed,             p00_des),                                   \
       (unsigned,           p00_des),                                   \
       (signed long,        p00_des),                                   \
       (unsigned long,      p00_des),                                   \
       (signed long long,   p00_des),                                   \
       (unsigned long long, p00_des));                                  \
    __typeof__(P00_AI(p00_objp))* p00_expi =                            \
      P99_GENERIC                                                       \
      (P00_AT(p00_objp),                                                \
       0,                                                               \
       (_Bool,              p00_exp),                                   \
       (char,               p00_exp),                                   \
       (signed char,        p00_exp),                                   \
       (unsigned char,      p00_exp),                                   \
       (signed short,       p00_exp),                                   \
       (unsigned short,     p00_exp),                                   \
       (signed,             p00_exp),                                   \
       (unsigned,           p00_exp),                                   \
       (signed long,        p00_exp),                                   \
       (unsigned long,      p00_exp),                                   \
       (signed long long,   p00_exp),                                   \
       (unsigned long long, p00_exp));                                  \
    p00_ret = P99_GENERIC                                               \
      (P00_AT(p00_objp),                                                \
       p00_atomic_ignore,                                               \
       (_Bool,              atomic_bool_compare_exchange_weak),         \
       (char,               atomic_char_compare_exchange_weak),         \
       (signed char,        atomic_schar_compare_exchange_weak),        \
       (unsigned char,      atomic_uchar_compare_exchange_weak),        \
       (signed short,       atomic_short_compare_exchange_weak),        \
       (unsigned short,     atomic_ushort_compare_exchange_weak),       \
       (signed,             atomic_int_compare_exchange_weak),          \
       (unsigned,           atomic_uint_compare_exchange_weak),         \
       (signed long,        atomic_long_compare_exchange_weak),         \
       (unsigned long,      atomic_ulong_compare_exchange_weak),        \
       (signed long long,   atomic_llong_compare_exchange_weak),        \
       (unsigned long long, atomic_ullong_compare_exchange_weak))       \
      (&P00_AI(p00_objp), p00_expi, p00_desi);                          \
  }                                                                     \
  p00_ret;                                                              \
 })

/**
 ** @memberof atomic_int
 **/
#define P00_FETCH_OP(OBJP, OPERAND, BUILTIN, OPERATOR)                  \
({                                                                      \
  __typeof__(OBJP) p00_objp = OBJP;                                     \
  __typeof__(P00_AT(p00_objp)) p00_ret;                                 \
  __typeof__(OPERAND) p00_op = OPERAND;                                 \
  if (!atomic_is_lock_free(p00_objp)) {                                 \
    atomic_flag_lock(&p00_objp->p00_lock);                              \
    p00_ret = P00_AT(p00_objp);                                         \
    P00_AT(p00_objp) OPERATOR p00_op;                                   \
    atomic_flag_unlock(&p00_objp->p00_lock);                            \
  } else {                                                              \
    __typeof__(P00_AI(p00_objp)) p00_opi =                              \
      P99_GENERIC                                                       \
      (p00_ret,                                                         \
       0,                                                               \
       (_Bool,              p00_op),                                    \
       (char,               p00_op),                                    \
       (signed char,        p00_op),                                    \
       (unsigned char,      p00_op),                                    \
       (signed short,       p00_op),                                    \
       (unsigned short,     p00_op),                                    \
       (signed,             p00_op),                                    \
       (unsigned,           p00_op),                                    \
       (signed long,        p00_op),                                    \
       (unsigned long,      p00_op),                                    \
       (signed long long,   p00_op),                                    \
       (unsigned long long, p00_op));                                   \
    p00_ret =                                                           \
      P99_GENERIC                                                       \
      (p00_ret,                                                         \
       P00_AT(p00_objp),                                                \
       (_Bool,              BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (char,               BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (signed char,        BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (unsigned char,      BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (signed short,       BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (unsigned short,     BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (signed,             BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (unsigned,           BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (signed long,        BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (unsigned long,      BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (signed long long,   BUILTIN(&P00_AI(p00_objp), p00_opi)),       \
       (unsigned long long, BUILTIN(&P00_AI(p00_objp), p00_opi)));      \
  }                                                                     \
  p00_ret;                                                              \
 })

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_add(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_add, +=)


/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_sub(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_sub, -=)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_or(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_or, |=)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_and(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_and, &=)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_xor(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_xor, ^=)


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
