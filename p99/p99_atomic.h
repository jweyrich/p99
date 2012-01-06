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
 **
 ** Most of the macros defined here just serve the purpose of
 ** detecting the lock-free property for specific integer data
 ** types. Names are used as they are foreseen in the standard, e.g
 ** ::ATOMIC_INT_LOCK_FREE that handles the case for @c int and @c
 ** unsigned types.
 **
 ** This information can be provided externally. If it isn't, some
 ** heuristic is used to detect these features. Currently the only
 ** mechanism that is implemented is the one of gcc.
 **
 ** From this information then is constructed a list of atomic integer
 ** types, ::P99_ATOMIC_LOCK_FREE_TYPES, that is used inside the
 ** macros for type generic programming.
 **
 ** The only type/functions that is always necessary is ::atomic_flag
 ** that is used for all other atomic types.
 **
 ** @{
 **/

#ifndef ATOMIC_BOOL_LOCK_FREE
# ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define ATOMIC_BOOL_LOCK_FREE 2
# else
#define ATOMIC_BOOL_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_CHAR_LOCK_FREE
# ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define ATOMIC_CHAR_LOCK_FREE 2
# else
#define ATOMIC_CHAR_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_SHORT_LOCK_FREE
# ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define ATOMIC_SHORT_LOCK_FREE 2
# else
#define ATOMIC_SHORT_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_INT_LOCK_FREE
# ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define ATOMIC_INT_LOCK_FREE 2
# else
#define ATOMIC_INT_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_LONG_LOCK_FREE
#  if ULONG_MAX == UINT_MAX
#   define ATOMIC_LONG_LOCK_FREE ATOMIC_INT_LOCK_FREE
#  elif __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 && (ULONG_MAX <= UINT64_C(18446744073709551615))
#   define ATOMIC_LONG_LOCK_FREE 2
#  else
#   define ATOMIC_LONG_LOCK_FREE 0
#  endif
#endif
#ifndef ATOMIC_LLONG_LOCK_FREE
#  if ULLONG_MAX == UINT_MAX
#   define ATOMIC_LLONG_LOCK_FREE ATOMIC_INT_LOCK_FREE
#  elif ULONG_MAX == ULONG_MAX
#   define ATOMIC_LLONG_LOCK_FREE ATOMIC_LONG_LOCK_FREE
#  elif __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 && (ULLONG_MAX <= UINT64_C(18446744073709551615))
#   define ATOMIC_LLONG_LOCK_FREE 2
#  else
#   define ATOMIC_LLONG_LOCK_FREE 0
#  endif
#endif
#ifndef ATOMIC_POINTER_LOCK_FREE
# ifdef __GNUC__
#define ATOMIC_POINTER_LOCK_FREE 0
# else
#define ATOMIC_POINTER_LOCK_FREE 0
# endif
#endif
#if UINT_LEAST16_MAX == UCHAR_MAX
# define ATOMIC_CHAR16_T_LOCK_FREE ATOMIC_CHAR_LOCK_FREE
#elif UINT_LEAST16_MAX == UINT_MAX
# define ATOMIC_CHAR16_T_LOCK_FREE ATOMIC_SHORT_LOCK_FREE
#elif UINT_LEAST16_MAX == ULONG_MAX
# define ATOMIC_CHAR16_T_LOCK_FREE ATOMIC_INT_LOCK_FREE
#else
# define ATOMIC_CHAR16_T_LOCK_FREE ATOMIC_LONG_LOCK_FREE
#endif
#if UINT_LEAST32_MAX == UCHAR_MAX
# define ATOMIC_CHAR32_T_LOCK_FREE ATOMIC_CHAR_LOCK_FREE
#elif UINT_LEAST32_MAX == UINT_MAX
# define ATOMIC_CHAR32_T_LOCK_FREE ATOMIC_SHORT_LOCK_FREE
#elif UINT_LEAST32_MAX == ULONG_MAX
# define ATOMIC_CHAR32_T_LOCK_FREE ATOMIC_INT_LOCK_FREE
#else
# define ATOMIC_CHAR32_T_LOCK_FREE ATOMIC_LONG_LOCK_FREE
#endif
#if WCHAR_MAX == UCHAR_MAX
# define ATOMIC_WCHAR_LOCK_FREE ATOMIC_CHAR_LOCK_FREE
#elif WCHAR_MAX == UINT_MAX
# define ATOMIC_WCHAR_LOCK_FREE ATOMIC_SHORT_LOCK_FREE
#elif WCHAR_MAX == ULONG_MAX
# define ATOMIC_WCHAR_LOCK_FREE ATOMIC_INT_LOCK_FREE
#else
# define ATOMIC_WCHAR_LOCK_FREE ATOMIC_LONG_LOCK_FREE
#endif

/**
 ** @memberof atomic_flag
 **/
#define ATOMIC_FLAG_INIT P99_ENC_INIT(0)

/**
 ** @memberof atomic_int
 **/
#define ATOMIC_VAR_INIT(V) { .p00_xval = { .p00_type_member = (V), }, }

/**
 ** @brief A list of types that are supposed to have lock-free atomic
 ** operations.
 **
 ** This list is constructed from the knowledge given via the
 ** lock-free macros for the standard types.
 **
 ** @see ATOMIC_BOOL_LOCK_FREE
 ** @see ATOMIC_CHAR_LOCK_FREE
 ** @see ATOMIC_SHORT_LOCK_FREE
 ** @see ATOMIC_INT_LOCK_FREE
 ** @see ATOMIC_LONG_LOCK_FREE
 ** @see ATOMIC_LLONG_LOCK_FREE
 **/
#ifndef P99_ATOMIC_LOCK_FREE_TYPES
# define P99_ATOMIC_LOCK_FREE_TYPES P00_ATOMIC_LOCK_FREE_TYPES5_

#if ATOMIC_BOOL_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES0 _Bool
#endif
#if ATOMIC_CHAR_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES1 char, signed char, unsigned char
#endif
#if ATOMIC_SHORT_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES2 short, unsigned short
#endif
#if ATOMIC_INT_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES3 int, unsigned int
#endif
#if ATOMIC_LONG_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES4 long, unsigned long
#endif
#if ATOMIC_LLONG_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES5 long long, unsigned long long
#endif

#ifdef P00_ATOMIC_LOCK_FREE_TYPES0
# ifdef P00_ATOMIC_LOCK_FREE_TYPES1
#  define P00_ATOMIC_LOCK_FREE_TYPES1_ P00_ATOMIC_LOCK_FREE_TYPES0, P00_ATOMIC_LOCK_FREE_TYPES1
# else
#  define P00_ATOMIC_LOCK_FREE_TYPES1_ P00_ATOMIC_LOCK_FREE_TYPES0
# endif
#else
# ifdef P00_ATOMIC_LOCK_FREE_TYPES1
#  define P00_ATOMIC_LOCK_FREE_TYPES1_ P00_ATOMIC_LOCK_FREE_TYPES1
# endif
#endif

#ifdef P00_ATOMIC_LOCK_FREE_TYPES1_
# ifdef P00_ATOMIC_LOCK_FREE_TYPES2
#  define P00_ATOMIC_LOCK_FREE_TYPES2_ P00_ATOMIC_LOCK_FREE_TYPES1_, P00_ATOMIC_LOCK_FREE_TYPES2
# else
#  define P00_ATOMIC_LOCK_FREE_TYPES2_ P00_ATOMIC_LOCK_FREE_TYPES1_
# endif
#else
# ifdef P00_ATOMIC_LOCK_FREE_TYPES2
#  define P00_ATOMIC_LOCK_FREE_TYPES2_ P00_ATOMIC_LOCK_FREE_TYPES2
# endif
#endif

#ifdef P00_ATOMIC_LOCK_FREE_TYPES2_
# ifdef P00_ATOMIC_LOCK_FREE_TYPES3
#  define P00_ATOMIC_LOCK_FREE_TYPES3_ P00_ATOMIC_LOCK_FREE_TYPES2_, P00_ATOMIC_LOCK_FREE_TYPES3
# else
#  define P00_ATOMIC_LOCK_FREE_TYPES3_ P00_ATOMIC_LOCK_FREE_TYPES2_
# endif
#else
# ifdef P00_ATOMIC_LOCK_FREE_TYPES3
#  define P00_ATOMIC_LOCK_FREE_TYPES3_ P00_ATOMIC_LOCK_FREE_TYPES3
# endif
#endif

#ifdef P00_ATOMIC_LOCK_FREE_TYPES3_
# ifdef P00_ATOMIC_LOCK_FREE_TYPES4
#  define P00_ATOMIC_LOCK_FREE_TYPES4_ P00_ATOMIC_LOCK_FREE_TYPES3_, P00_ATOMIC_LOCK_FREE_TYPES4
# else
#  define P00_ATOMIC_LOCK_FREE_TYPES4_ P00_ATOMIC_LOCK_FREE_TYPES3_
# endif
#else
# ifdef P00_ATOMIC_LOCK_FREE_TYPES4
#  define P00_ATOMIC_LOCK_FREE_TYPES4_ P00_ATOMIC_LOCK_FREE_TYPES4
# endif
#endif

#ifdef P00_ATOMIC_LOCK_FREE_TYPES4_
# ifdef P00_ATOMIC_LOCK_FREE_TYPES5
#  define P00_ATOMIC_LOCK_FREE_TYPES5_ P00_ATOMIC_LOCK_FREE_TYPES4_, P00_ATOMIC_LOCK_FREE_TYPES5
# else
#  define P00_ATOMIC_LOCK_FREE_TYPES5_ P00_ATOMIC_LOCK_FREE_TYPES4_
# endif
#else
# ifdef P00_ATOMIC_LOCK_FREE_TYPES5
#  define P00_ATOMIC_LOCK_FREE_TYPES5_ P00_ATOMIC_LOCK_FREE_TYPES5
# endif
#endif

_Pragma(P99_STRINGIFY(message P99_STRINGIFY(detected atomic types are P99_ATOMIC_LOCK_FREE_TYPES)))
#endif

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

P99_ENC_DECLARE(int volatile, atomic_flag);

#define P00_AT(OBJP) ((OBJP)->p00_xval.p00_type_member)
#define P00_AI(OBJP) ((OBJP)->p00_xval.p00_integer_member)

#define P99_DECLARE_ATOMIC_LOCK_FREE(T, NAME)                                                                                    \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
union NAME {                                                                                                           \
  atomic_flag p00_lock;                                                                                                \
  union {                                                                                                              \
    T p00_integer_member;                                                                                              \
    T p00_type_member;                                                                                                 \
  } p00_xval;                                                                                                          \
};                                                                                                                     \
typedef union NAME NAME

#define P99_ATOMIC(T)                                                            \
struct {                                                                         \
  atomic_flag p00_lock;                                                          \
  union {                                                                        \
    /* should be an integer type that can be cast to pointers without warning */ \
    uintptr_t p00_integer_member;                                                \
    T p00_type_member;                                                           \
  } p00_xval;                                                                    \
}

#define P99_DECLARE_ATOMIC(T, NAME)                                                                                    \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
typedef P99_ATOMIC(T) NAME

#define P00_DECLARE_ATOMIC_CHOICE(MAC, T, NAME) \
P99_IF_EQ_2(MAC)                                \
(P99_DECLARE_ATOMIC_LOCK_FREE(T, NAME))         \
(P99_DECLARE_ATOMIC(T, NAME))


#define P00_DECLARE_ATOMIC_INHERIT(T, NAME)     \
typedef                                         \
__typeof__                                      \
(                                               \
 P99_GENERIC_LITERAL                            \
 ((T)0, ,                                       \
  (_Bool, atomic_bool),                         \
  (char, atomic_char),                          \
  (signed char, atomic_schar),                  \
  (unsigned char, atomic_uchar),                \
  (short int, atomic_short),                    \
  (unsigned short int, atomic_ushort),          \
  (int, atomic_int),                            \
  (unsigned int, atomic_uint),                  \
  (long int, atomic_long),                      \
  (unsigned long int, atomic_ulong),            \
  (long long int, atomic_llong),                \
  (unsigned long long int, atomic_ullong)       \
  )                                             \
 ) NAME


/**
 ** @brief Atomic access to a value of type <code>int</code>
 **
 ** See the "public member function" section of this description for
 ** the operations that support this type
 **/
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_INT_LOCK_FREE, int, atomic_int);

/**
 ** @brief Atomic access to a value of type <code>bool</code>
 ** @see atomic_load
 ** @see atomic_store
 ** @see atomic_compare_exchange_weak
 ** @see ATOMIC_VAR_INIT
 ** @see atomic_init
 **/
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_BOOL_LOCK_FREE, _Bool, atomic_bool);


P00_DECLARE_ATOMIC_CHOICE(ATOMIC_CHAR_LOCK_FREE, char, atomic_char);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_CHAR_LOCK_FREE, signed char, atomic_schar);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_CHAR_LOCK_FREE, unsigned char, atomic_uchar);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_SHORT_LOCK_FREE, short, atomic_short);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_SHORT_LOCK_FREE, unsigned short, atomic_ushort);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_INT_LOCK_FREE, unsigned int, atomic_uint);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_LONG_LOCK_FREE, long, atomic_long);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_LONG_LOCK_FREE, unsigned long, atomic_ulong);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_LLONG_LOCK_FREE, long long, atomic_llong);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_LONG_LOCK_FREE, unsigned long long, atomic_ullong);
P00_DECLARE_ATOMIC_INHERIT(char16_t, atomic_char16_t);
P00_DECLARE_ATOMIC_INHERIT(char32_t, atomic_char32_t);
P00_DECLARE_ATOMIC_INHERIT(wchar_t, atomic_wchar_t);
P00_DECLARE_ATOMIC_INHERIT(int_least8_t, atomic_int_least8_t);
P00_DECLARE_ATOMIC_INHERIT(uint_least8_t, atomic_uint_least8_t);
P00_DECLARE_ATOMIC_INHERIT(int_least16_t, atomic_int_least16_t);
P00_DECLARE_ATOMIC_INHERIT(uint_least16_t, atomic_uint_least16_t);
P00_DECLARE_ATOMIC_INHERIT(int_least32_t, atomic_int_least32_t);
P00_DECLARE_ATOMIC_INHERIT(uint_least32_t, atomic_uint_least32_t);
P00_DECLARE_ATOMIC_INHERIT(int_least64_t, atomic_int_least64_t);
P00_DECLARE_ATOMIC_INHERIT(uint_least64_t, atomic_uint_least64_t);
P00_DECLARE_ATOMIC_INHERIT(int_fast8_t, atomic_int_fast8_t);
P00_DECLARE_ATOMIC_INHERIT(uint_fast8_t, atomic_uint_fast8_t);
P00_DECLARE_ATOMIC_INHERIT(int_fast16_t, atomic_int_fast16_t);
P00_DECLARE_ATOMIC_INHERIT(uint_fast16_t, atomic_uint_fast16_t);
P00_DECLARE_ATOMIC_INHERIT(int_fast32_t, atomic_int_fast32_t);
P00_DECLARE_ATOMIC_INHERIT(uint_fast32_t, atomic_uint_fast32_t);
P00_DECLARE_ATOMIC_INHERIT(int_fast64_t, atomic_int_fast64_t);
P00_DECLARE_ATOMIC_INHERIT(uint_fast64_t, atomic_uint_fast64_t);
P00_DECLARE_ATOMIC_INHERIT(intptr_t, atomic_intptr_t);
P00_DECLARE_ATOMIC_INHERIT(uintptr_t, atomic_uintptr_t);
P00_DECLARE_ATOMIC_INHERIT(size_t, atomic_size_t);
P00_DECLARE_ATOMIC_INHERIT(ptrdiff_t, atomic_ptrdiff_t);
P00_DECLARE_ATOMIC_INHERIT(intmax_t, atomic_intmax_t);
P00_DECLARE_ATOMIC_INHERIT(uintmax_t, atomic_uintmax_t);

P99_DECLARE_ATOMIC(float, atomic_float);
P99_DECLARE_ATOMIC(double, atomic_double);
P99_DECLARE_ATOMIC(long double, atomic_ldouble);


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
 ** @memberof atomic_flag
 **/
#define atomic_flag_test_and_set_explicit(OBJP, ORDER)                  \
({                                                                      \
  _Bool ret;                                                            \
  memory_order order = (ORDER);                                         \
  switch (order) {                                                      \
    /* This case doesn't require any guarantee. */                      \
  case memory_order_relaxed:                                            \
    ret = P99_ENCP(OBJP);                                               \
    P99_ENCP(OBJP) = 1;                                                 \
    break;                                                              \
    /* For these three the acquire semantics are not sufficient. */     \
  case memory_order_release: ;                                          \
  case memory_order_acq_rel: ;                                          \
  case memory_order_seq_cst:                                            \
    atomic_thread_fence(order);                                         \
    ret = __sync_lock_test_and_set(&P99_ENCP(OBJP), 1);                 \
    break;                                                              \
  default:                                                              \
    ret = __sync_lock_test_and_set(&P99_ENCP(OBJP), 1);                 \
    break;                                                              \
  }                                                                     \
  ret;                                                                  \
 })

/**
 ** @memberof atomic_flag
 **/
#define atomic_flag_test_and_set(OBJP) atomic_flag_test_and_set_explicit((OBJP), memory_order_seq_cst)

/**
 ** @memberof atomic_flag
 **/
#define atomic_flag_clear_explicit(OBJP, ORDER)                         \
(void)({                                                                \
    memory_order order = (ORDER);                                       \
    switch(order) {                                                     \
      /* This case doesn't require any guarantee. */                    \
    case memory_order_relaxed:                                          \
      P99_ENCP(OBJP) = 0;                                               \
      break;                                                            \
    /* For these three the release semantics are not sufficient. */     \
    case memory_order_acquire: ;                                        \
    case memory_order_acq_rel: ;                                        \
    case memory_order_seq_cst:                                          \
      __sync_lock_release(&P99_ENCP(OBJP));                             \
      atomic_thread_fence(order);                                       \
      break;                                                            \
    default:                                                            \
      __sync_lock_release(&P99_ENCP(OBJP));                             \
      break;                                                            \
    }                                                                   \
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
#define atomic_store(OBJP, DESIRED)                                                         \
({                                                                                          \
  __typeof__(OBJP) p00_objp = (OBJP);                                                       \
  __typeof__(DESIRED) p00_des = (DESIRED);                                                  \
  if (!atomic_is_lock_free(p00_objp)) {                                                     \
    atomic_flag_lock(&p00_objp->p00_lock);                                                  \
    P00_AT(p00_objp) = p00_des;                                                             \
    atomic_flag_unlock(&p00_objp->p00_lock);                                                \
  }                                                                                         \
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)()                                        \
    (else {                                                                                 \
      __typeof__(P00_AI(p00_objp)) p00_desi =                                               \
        P99_TYPE_CHOICE(P00_AT(p00_objp), p00_des, 0, P99_ATOMIC_LOCK_FREE_TYPES);           \
      __typeof__(P00_AI(p00_objp)) p00_prei = 0;                                            \
      __typeof__(P00_AI(p00_objp)) p00_vali = 0;                                            \
      P99_TYPE_CHOICE                                                                       \
        (P00_AT(p00_objp),                                                                  \
         ({                                                                                 \
           for (;;) {                                                                       \
             p00_vali = __sync_val_compare_and_swap(&P00_AI(p00_objp), p00_prei, p00_desi); \
             if (p00_vali == p00_prei) break;                                               \
             p00_prei = p00_vali;                                                           \
           }                                                                                \
         }),                                                                                \
         (void)0,                                                                           \
         P99_ATOMIC_LOCK_FREE_TYPES);                                                        \
    })                                                                                      \
 })

/**
 ** @memberof atomic_int
 **/
#define atomic_load(OBJP)                                                     \
({                                                                            \
  __typeof__(OBJP) p00_objp = (OBJP);                                         \
  __typeof__(P00_AT(p00_objp)) p00_ret;                                       \
  if (!atomic_is_lock_free(p00_objp)) {                                       \
    atomic_flag_lock(&p00_objp->p00_lock);                                    \
    p00_ret = P00_AT(p00_objp);                                               \
    atomic_flag_unlock(&p00_objp->p00_lock);                                  \
  }                                                                           \
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)                              \
    (else p00_ret = P00_AT(p00_objp);)                                  \
    (else {                                                                   \
      p00_ret =                                                               \
        P99_TYPE_CHOICE(p00_ret,                                              \
                        __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0), \
                        P00_AT(p00_objp),                                     \
                        P99_ATOMIC_LOCK_FREE_TYPES);                           \
    })                                                                        \
  /* cast to the same type to be sure that the result is an lvalue */         \
  (__typeof__(P00_AT(p00_objp)))p00_ret;                                      \
 })

/**
 ** @memberof atomic_int
 **/
#define atomic_compare_exchange_weak(OBJP, EXPECTED, DESIRED)                      \
({                                                                                 \
  _Bool p00_ret;                                                                   \
  __typeof__(OBJP) p00_objp = (OBJP);                                              \
  __typeof__(EXPECTED) p00_exp = (EXPECTED);                                       \
  __typeof__(DESIRED) p00_des = (DESIRED);                                         \
  if (!atomic_is_lock_free(p00_objp)) {                                            \
    atomic_flag_lock(&p00_objp->p00_lock);                                         \
    p00_ret = !memcmp(p00_exp, (void*)&P00_AT(p00_objp), sizeof P00_AT(p00_objp)); \
    /* Both, *EXPECTED and DESIRED must be assignment compatible                   \
       with the base type */                                                       \
    if (!p00_ret) *p00_exp = P00_AT(p00_objp);                                     \
    else         P00_AT(p00_objp) = p00_des;                                       \
    atomic_flag_unlock(&p00_objp->p00_lock);                                       \
  }                                                                                \
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)                              \
    (else p00_ret = P00_AT(p00_objp);)                                  \
    (else {                                                                        \
      __typeof__(P00_AI(p00_objp)) p00_desi =                                      \
        P99_TYPE_CHOICE(P00_AT(p00_objp), p00_des, 0, P99_ATOMIC_LOCK_FREE_TYPES);  \
      __typeof__(P00_AI(p00_objp))* p00_expi =                                     \
        P99_TYPE_CHOICE(P00_AT(p00_objp), p00_exp, 0, P99_ATOMIC_LOCK_FREE_TYPES);  \
      __typeof__(P00_AI(p00_objp)) p00_vali =                                      \
        P99_TYPE_CHOICE                                                            \
        (P00_AT(p00_objp),                                                         \
         __sync_val_compare_and_swap(&P00_AI(p00_objp), *p00_expi, p00_desi),      \
         0,                                                                        \
         P99_ATOMIC_LOCK_FREE_TYPES);                                               \
      p00_ret = (*p00_expi == p00_vali);                                           \
      if (!p00_ret) *p00_expi = p00_vali;                                          \
    })                                                                             \
  p00_ret;                                                                         \
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
  }                                                                     \
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)                              \
    (else p00_ret = P00_AT(p00_objp);)                                  \
    (else {                                                             \
      __typeof__(P00_AI(p00_objp)) p00_opi =                            \
        P99_TYPE_CHOICE(p00_ret, p00_op, 0, P99_ATOMIC_LOCK_FREE_TYPES); \
      p00_ret =                                                         \
        P99_TYPE_CHOICE(p00_ret,                                        \
                        BUILTIN(&P00_AI(p00_objp), p00_opi),            \
                        P00_AT(p00_objp),                               \
                        P99_ATOMIC_LOCK_FREE_TYPES);                     \
    })                                                                  \
    p00_ret;                                                            \
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
 ** @}
 **/

#endif
