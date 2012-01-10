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

/**
 ** @addtogroup atomic C11 atomic operations
 **
 ** This is a stub implementation of C11 atomic types and operations.
 **
 ** This uses gcc extensions
 ** - all those that are needed for ::P99_GENERIC
 ** - @c __typeof__ to declare @c typedef or local variables of a specific type
 ** - block expressions with <code>({ ... })</code>
 **
 ** We also use @c __sync_lock_test_and_set and other similar builtins
 ** if they are available. If not, @c __sync_lock_test_and_set and @c
 ** __sync_lock_release are implemented in assembler (for 4 byte
 ** integers) and all other operations are synthesized with an
 ** ::atomic_flag that is used as a spinlock.
 **
 ** @{
 **/


#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)

p99_inline
uint32_t p00_sync_lock_test_and_set(uint32_t volatile *object) {
  return __sync_lock_test_and_set(object, 1);
}

p99_inline
void p00_sync_lock_release(uint32_t volatile *object) {
  __sync_lock_release(object);
}

#elif defined(__arm__)

/*
  ldrex/strex is a load-link and store instruction pair. This means a
  strex will only succeed, if nobody else has touched "object" since
  the load.

  This operation is not lock-free by itself, so by itself it doesn't
  guarantee the requirements of the standard for operations on
  atomic_flag. The OS must support this operation by canceling all
  pending ldrex operations when (de)scheduling a thread or entering or
  leaving a signal handler.
*/

p99_inline
uint32_t p00_arm_ldrex(uint32_t volatile*ptr) {
  uint32_t ret;
  __asm__ volatile ("ldrex %0,[%1]\t@ load exclusive\n"
                    : "=&r" (ret)
                    : "r" (ptr)
                    : "cc", "memory"
                    );
  return ret;
}

p99_inline
_Bool p00_arm_strex(uint32_t volatile*ptr, uint32_t val) {
  uint32_t ret;
  __asm__ volatile ("strex %0,%1,[%2]\t@ store exclusive\n"
                    : "=&r" (ret)
                    : "r" (val), "r" (ptr)
                    : "cc", "memory"
                    );
  return ret;
}

p99_inline
uint32_t p00_sync_lock_test_and_set(uint32_t volatile *object) {
  for (;;) {
    uint32_t ret = p00_arm_ldrex(object);
    /* Even if the result has been a 1 in ret, We must imperatively
       put a strex after the ldex since otherwise we would block other
       threads when they try to access this. On the other hand even if
       the strex doesn't succeed but ret is already set, we are also
       done. */
    if (!p00_arm_strex(object, 1) || ret) return ret;
  }
}

p99_inline
void p00_sync_lock_release(uint32_t volatile *object) {
  __sync_lock_release(object);
}

#elif defined(__x86_64__) || defined(__i386__)

p99_inline
uint32_t p00_sync_lock_test_and_set(uint32_t volatile *object) {
  register uint32_t ret P99_FIXED_REGISTER(eax) = 1;
  __asm__ __volatile__("xchgl %2, %0"
                       : "=a"(ret)
                       : "r"(ret), "m"(*object)
                       : "memory");
  return ret;
}

p99_inline
void p00_sync_lock_release(uint32_t volatile *object) {
  register uint32_t val = 0;
  __asm__ __volatile__("movl %0, %1"
                       :
                       : "r"(val), "m"(*object)
                       : "memory");
}


#endif

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
# if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) && (UINT_MAX <= UINT32_MAX)
#define ATOMIC_BOOL_LOCK_FREE 2
# else
#define ATOMIC_BOOL_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_CHAR_LOCK_FREE
# if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) && (UCHAR_MAX <= UINT32_MAX)
#define ATOMIC_CHAR_LOCK_FREE 2
# else
#define ATOMIC_CHAR_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_SHORT_LOCK_FREE
# if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) && (USHORT_MAX <= UINT32_MAX)
#define ATOMIC_SHORT_LOCK_FREE 2
# else
#define ATOMIC_SHORT_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_INT_LOCK_FREE
# if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) && (UINT_MAX <= UINT32_MAX)
#define ATOMIC_INT_LOCK_FREE 2
# else
#define ATOMIC_INT_LOCK_FREE 0
# endif
#endif
#ifndef ATOMIC_LONG_LOCK_FREE
#  if ULONG_MAX == UINT_MAX
#   define ATOMIC_LONG_LOCK_FREE ATOMIC_INT_LOCK_FREE
#  elif defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) && (ULONG_MAX <= UINT64_MAX)
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
#  elif defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) && (ULLONG_MAX <= UINT64_MAX)
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
#if UINTPTR_MAX == UCHAR_MAX
# define ATOMIC_INTPTR_LOCK_FREE ATOMIC_CHAR_LOCK_FREE
#elif UINTPTR_MAX == UINT_MAX
# define ATOMIC_INTPTR_LOCK_FREE ATOMIC_SHORT_LOCK_FREE
#elif UINTPTR_MAX == ULONG_MAX
# define ATOMIC_INTPTR_LOCK_FREE ATOMIC_INT_LOCK_FREE
#else
# define ATOMIC_INTPTR_LOCK_FREE ATOMIC_LONG_LOCK_FREE
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
 ** @brief Initialize a variable of type ::atomic_flag
 ** @memberof atomic_flag
 **/
#define ATOMIC_FLAG_INIT P99_ENC_INIT(0)

/**
 ** @brief Initialize a variable of an atomic type.
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
# define P99_ATOMIC_LOCK_FREE_TYPES P00_ATOMIC_LOCK_FREE_TYPES6_

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
#if ATOMIC_INTPTR_LOCK_FREE == 2
# define P00_ATOMIC_LOCK_FREE_TYPES6 char*, signed char*, unsigned char*
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

#ifdef P00_ATOMIC_LOCK_FREE_TYPES5_
# ifdef P00_ATOMIC_LOCK_FREE_TYPES6
#  define P00_ATOMIC_LOCK_FREE_TYPES6_ P00_ATOMIC_LOCK_FREE_TYPES5_, P00_ATOMIC_LOCK_FREE_TYPES6
# else
#  define P00_ATOMIC_LOCK_FREE_TYPES6_ P00_ATOMIC_LOCK_FREE_TYPES5_
# endif
#else
# ifdef P00_ATOMIC_LOCK_FREE_TYPES6
#  define P00_ATOMIC_LOCK_FREE_TYPES6_ P00_ATOMIC_LOCK_FREE_TYPES6
# endif
#endif
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

/**
 ** @brief The "minimal" type for which atomic operations must be
 ** defined.
 **
 ** This only has very basic operations that can set and clear the
 ** flag. In particular it has no operation that returns the state of
 ** the flag and that would not modify it eventually. So this is a
 ** kind of Heisenberg flag, you can't measure it without modifying
 ** it. This can be used as a spinlock to force the flag to be set,
 ** perform a desired operation and then clear the flag, again.
 **
 ** There are extensions ::atomic_flag_lock, ::atomic_flag_trylock and
 ** ::atomic_flag_unlock that perform these spinlock type operations
 ** with only the necessary memory consistency.
 **/
P99_ENC_DECLARE(uint32_t volatile, atomic_flag);

#define P00_AT(OBJP) ((OBJP)->p00_xval.p00_type_member)
#define P00_AI(OBJP) ((OBJP)->p00_xval.p00_integer_member)

/**
 ** @brief declare an atomic type that will have lock-free operations
 **
 ** Normally it shouldn't be necessary to use this macro in user code
 ** for integral data types; the lock-free types should be detected
 ** automatically.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_ATOMIC_LOCK_FREE, 0)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_ATOMIC_LOCK_FREE, 1)
#ifdef P00_DOXYGEN
# define P99_DECLARE_ATOMIC_LOCK_FREE(T, NAME)                                                                         \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
typedef union NAME NAME
#else
#define P99_DECLARE_ATOMIC_LOCK_FREE(T, NAME)                                                                          \
union NAME {                                                                                                           \
  atomic_flag p00_lock;                                                                                                \
  union {                                                                                                              \
    T p00_integer_member;                                                                                              \
    T p00_type_member;                                                                                                 \
  } p00_xval;                                                                                                          \
};                                                                                                                     \
typedef union NAME NAME
#endif

P00_DOCUMENT_TYPE_ARGUMENT(P00_ATOMIC_STRUCT, 0)
#define P00_ATOMIC_STRUCT(T, NAME)                                               \
struct NAME {                                                                    \
  atomic_flag p00_lock;                                                          \
  union {                                                                        \
    /* should be an integer type that can be cast to pointers without warning */ \
    uintptr_t p00_integer_member;                                                \
    T p00_type_member;                                                           \
  } p00_xval;                                                                    \
}

/**
 ** @brief declare an atomic type that will use lock operations to
 ** guarantee atomicity
 **
 ** Normally it shouldn't be necessary to use this macro in user code
 ** for integral data types; the lock-free types should be detected
 ** automatically.
 **
 ** This is used per default for integral types that are detected not
 ** to support atomic operations directly and for floating point
 ** types.
 **
 ** Use this macro to protect your own particular data type.
 **
 ** @param T base type for the atomic type
 **
 ** @param NAME the name of the newly derived type. Usually for type
 ** @c foo you should use something like atomic_foo.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_ATOMIC, 0)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_ATOMIC_LOCK_FREE, 1)
#ifdef P00_DOXYGEN
#define P99_DECLARE_ATOMIC(T, NAME)
#else
#define P99_DECLARE_ATOMIC(...)                                         \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
P99_IF_EQ_1(P99_NARG(__VA_ARGS__))                                      \
(P00_DECLARE_ATOMIC(__VA_ARGS__, P99_PASTE2(atomic_, __VA_ARGS__)))     \
(P00_DECLARE_ATOMIC(__VA_ARGS__))
#endif

#define P00_DECLARE_ATOMIC(T, ...)                                                                                    \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
typedef P00_ATOMIC_STRUCT(T, __VA_ARGS__) __VA_ARGS__

#define P00_DECLARE_ATOMIC_CHOICE(MAC, T, NAME) \
P99_IF_EQ_2(MAC)                                \
(P99_DECLARE_ATOMIC_LOCK_FREE(T, NAME))         \
(P99_DECLARE_ATOMIC(T, NAME))


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
 **
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

P00_DECLARE_ATOMIC_CHOICE(ATOMIC_CHAR_LOCK_FREE, char*, atomic_char_p);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_CHAR_LOCK_FREE, signed char*, atomic_schar_p);
P00_DECLARE_ATOMIC_CHOICE(ATOMIC_CHAR_LOCK_FREE, unsigned char*, atomic_uchar_p);

P99_DECLARE_ATOMIC(float, atomic_float);
P99_DECLARE_ATOMIC(double, atomic_double);
P99_DECLARE_ATOMIC(long double, atomic_ldouble);

#ifndef __STDC_NO_COMPLEX__
P99_DECLARE_ATOMIC(float _Complex, atomic_cfloat);
P99_DECLARE_ATOMIC(double _Complex, atomic_cdouble);
P99_DECLARE_ATOMIC(long double _Complex, atomic_cldouble);
#define P00_ATOMIC_TYPES                         \
  (_Bool, atomic_bool*),                         \
  (char, atomic_char*),                          \
  (signed char, atomic_schar*),                  \
  (unsigned char, atomic_uchar*),                \
  (short int, atomic_short*),                    \
  (unsigned short int, atomic_ushort*),          \
  (int, atomic_int*),                            \
  (unsigned int, atomic_uint*),                  \
  (long int, atomic_long*),                      \
  (unsigned long int, atomic_ulong*),            \
  (long long int, atomic_llong*),                \
  (unsigned long long int, atomic_ullong*),      \
  (float, atomic_float*),                        \
  (double, atomic_double*),                      \
  (long double, atomic_ldouble*),                \
  (float _Complex, atomic_cfloat*),              \
  (double _Complex, atomic_cdouble*),            \
  (long double _Complex, atomic_cldouble*)
#else
#define P00_ATOMIC_TYPES                         \
  (_Bool, atomic_bool*),                         \
  (char, atomic_char*),                          \
  (signed char, atomic_schar*),                  \
  (unsigned char, atomic_uchar*),                \
  (short int, atomic_short*),                    \
  (unsigned short int, atomic_ushort*),          \
  (int, atomic_int*),                            \
  (unsigned int, atomic_uint*),                  \
  (long int, atomic_long*),                      \
  (unsigned long int, atomic_ulong*),            \
  (long long int, atomic_llong*),                \
  (unsigned long long int, atomic_ullong*),      \
  (float, atomic_float*),                        \
  (double, atomic_double*),                      \
  (long double, atomic_ldouble*)
#endif

#define P99_ATOMIC_INHERIT(T) (*P99_GENERIC_LITERAL((T)0, (struct P99_PASTE2(atomic_, T)*){ 0 }, P00_ATOMIC_TYPES))

/**
 ** @brief refer to an atomic type of base type T
 **
 ** @param T a @b type name
 **
 ** This implements only part of the @c _Atomic keyword of C11. First
 ** it implements only the part described in 6.7.2.4, atomic type
 ** specifiers. I does not cover type qualifiers.
 **
 ** Second for data types that are not predefined integer or
 ** floatingpoint types, the underlying realization of the atomic type
 ** must have been previously declared with ::P99_DECLARE_ATOMIC.
 **
 ** Besides the atomic integer types that are listed in the standard
 ** we provide 6 atomic types for all real and complex floating point
 ** types, as long as they exist.
 **
 ** For the standard predefined integer types there are different
 ** atomic types as they are listed in the standard. The atomic types
 ** for the floating point types (as there are no names listed in the
 ** standard) are chosen arbitrarily with an @c atomic_ prefix. Don't
 ** rely on a particular naming. Whenever ::_Atomic is called with a
 ** typename @a T that resolves to one of the predefined arithmetic
 ** types (without @c enum and pointers) the same type as for the
 ** original type results: e.g if @c unsigned is 32 bit wide (and @c
 ** long is 64)
 **
 ** @code
 ** _Atomic(uint32_t) a;
 ** _Atomic(unsigned) b;
 ** @endcode
 **
 ** is equivalent to
 **
 ** @code
 ** atomic_uint a;
 ** atomic_uint b;
 ** @endcode
 **
 ** @remark These types don't work with the usual operators such as @
 ** +=, and a variable of such an atomic type doesn't evaluate to an
 ** rvalue of the base type.
 **
 ** @remark These variables are guaranteed to be in a valid state when
 ** they are @c 0 initialized by the default initializer, either
 ** explicitly or when declared with static storage location. The
 ** initial value of the base type corresponds to its @c 0 initialized
 ** value.
 **
 ** @see ATOMIC_VAR_INIT for an initializer macro
 ** @see atomic_init for a initializer function
 ** @see atomic_load to return an rvalue of the underlying base type @a T
 ** @see atomic_store to store a value that is compatible with @a T
 ** @see atomic_compare_exchange_weak to store a value conditionally
 **
 ** If the underlying operations are defined for @a T the following
 ** generic functions (macros) can be used with an atomic type:
 **
 ** @see atomic_fetch_add to add a value
 ** @see atomic_fetch_sub to subtract a value
 ** @see atomic_fetch_or or a value
 ** @see atomic_fetch_and and a value
 ** @see atomic_fetch_xor exclusive or a value
 **
 ** @warning Don't assign atomic variables through the @c =
 ** operator. This will most probably not do what you expect:
 ** - This would copy state information from the right side to the
 **   left side.
 ** - Neither the read access on the right side nor the write access
 **   on the left side would be atomic.
 **
 ** @warning Don't use atomic variables as function arguments.
 ** - If you are only interested in the value evaluate by ::atomic_load.
 ** - Otherwise pass it through a pointer to atomic type.
 **
 ** @ingroup C11_keywords
 **/
#ifdef P00_DOXYGEN
# define _Atomic(T) struct P99_PASTE2(atomic_, T)
#else
# define _Atomic(T) __typeof__(P99_ATOMIC_INHERIT(T))
#endif

typedef _Atomic(char16_t) atomic_char16_t;
typedef _Atomic(char32_t) atomic_char32_t;
typedef _Atomic(wchar_t) atomic_wchar_t;
typedef _Atomic(int_least8_t) atomic_int_least8_t;
typedef _Atomic(uint_least8_t) atomic_uint_least8_t;
typedef _Atomic(int_least16_t) atomic_int_least16_t;
typedef _Atomic(uint_least16_t) atomic_uint_least16_t;
typedef _Atomic(int_least32_t) atomic_int_least32_t;
typedef _Atomic(uint_least32_t) atomic_uint_least32_t;
typedef _Atomic(int_least64_t) atomic_int_least64_t;
typedef _Atomic(uint_least64_t) atomic_uint_least64_t;
typedef _Atomic(int_fast8_t) atomic_int_fast8_t;
typedef _Atomic(uint_fast8_t) atomic_uint_fast8_t;
typedef _Atomic(int_fast16_t) atomic_int_fast16_t;
typedef _Atomic(uint_fast16_t) atomic_uint_fast16_t;
typedef _Atomic(int_fast32_t) atomic_int_fast32_t;
typedef _Atomic(uint_fast32_t) atomic_uint_fast32_t;
typedef _Atomic(int_fast64_t) atomic_int_fast64_t;
typedef _Atomic(uint_fast64_t) atomic_uint_fast64_t;
typedef _Atomic(intptr_t) atomic_intptr_t;
typedef _Atomic(uintptr_t) atomic_uintptr_t;
typedef _Atomic(size_t) atomic_size_t;
typedef _Atomic(ptrdiff_t) atomic_ptrdiff_t;
typedef _Atomic(intmax_t) atomic_intmax_t;
typedef _Atomic(uintmax_t) atomic_uintmax_t;

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
 ** @brief Unconditionally set @a *object to @c true and return the
 ** previous value
 **
 ** @memberof atomic_flag
 **
 ** @param object the object that will be set
 **
 ** @param order fine tune the set operation for a specific memory
 ** order. The current implementation only uses this to enforce a
 ** complete memory barrier where necessary.
 **/
p99_inline
_Bool atomic_flag_test_and_set_explicit(volatile atomic_flag *object, memory_order order) {
  _Bool ret;
  switch (order) {
    /* This case doesn't require any guarantee. */
  case memory_order_relaxed:
    ret = P99_ENCP(object);
    P99_ENCP(object) = 1;
    break;
    /* For these three the acquire semantics are not sufficient. */
  case memory_order_release: ;
  case memory_order_acq_rel: ;
  case memory_order_seq_cst:
    atomic_thread_fence(order);
    ret = p00_sync_lock_test_and_set(&P99_ENCP(object));
    break;
  default:
    ret = p00_sync_lock_test_and_set(&P99_ENCP(object));
    break;
  }
  return ret;
}

/**
 ** @brief Unconditionally set @a *object to @c true and return the
 ** previous value
 **
 ** @memberof atomic_flag
 **/
p99_inline
_Bool atomic_flag_test_and_set(volatile atomic_flag *object) {
  return atomic_flag_test_and_set_explicit(object, memory_order_seq_cst);
}

/**
 ** @brief Unconditionally set @a *object to @c false
 **
 ** @memberof atomic_flag
 **
 ** @param object the object that will be set
 **
 ** @param order fine tune the set operation for a specific memory
 ** order. The current implementation only uses this to enforce a
 ** complete memory barrier where necessary.
 **/
p99_inline
void atomic_flag_clear_explicit(volatile atomic_flag *object, memory_order order) {
  switch(order) {
    /* This case doesn't require any guarantee. */
  case memory_order_relaxed:
    P99_ENCP(object) = 0;
    break;
    /* For these three the release semantics are not sufficient. */
  case memory_order_acquire: ;
  case memory_order_acq_rel: ;
  case memory_order_seq_cst:
    __sync_lock_release(&P99_ENCP(object));
    atomic_thread_fence(order);
    break;
  default:
    __sync_lock_release(&P99_ENCP(object));
    break;
  }
}

/**
 ** @brief Unconditionally set @a *object to @c false
 **
 ** @memberof atomic_flag
 **/
p99_inline
void atomic_flag_clear(volatile atomic_flag *object) {
  atomic_flag_clear_explicit(object, memory_order_seq_cst);
}

/**
 ** @brief extension: spin on @a object setting the flag until the state before was "clear"
 **
 ** This interprets an ::atomic_flag as a spinlock. State "clear"
 ** means unlocked and state "set" means locked.
 **
 ** This operation only guarantees acquire consistency.
 **
 ** @memberof atomic_flag
 **/
p99_inline
void atomic_flag_lock(volatile atomic_flag *object) {
  while (atomic_flag_test_and_set_explicit(object, memory_order_acquire));
}

/**
 ** @brief extension: set the flag and return true if we are the first
 ** to do so
 **
 ** This interprets an ::atomic_flag as a spinlock. State "clear"
 ** means unlocked and state "set" means locked.
 **
 ** This operation only guarantees acquire consistency.
 **
 ** @memberof atomic_flag
 **/
p99_inline
_Bool atomic_flag_trylock(volatile atomic_flag *object) {
  return !atomic_flag_test_and_set_explicit(object, memory_order_acquire);
}

/**
 ** @brief extension: clear the flag unconditionally
 **
 ** This operation only guarantees release consistency.
 **
 ** @memberof atomic_flag
 **/
p99_inline
void atomic_flag_unlock(volatile atomic_flag *object) {
  atomic_flag_clear_explicit(object, memory_order_release);
}

/**
 ** @brief return true if @a OBJP points to a lock-free object
 **
 ** In this implementation this is a constant integer expression and
 ** will be determined at compile time.
 **
 ** @memberof atomic_int
 **/
#define atomic_is_lock_free(OBJP)                                       \
P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)                                \
(0)                                                                     \
(P99_TYPE_CHOICE(P00_AT(OBJP), 1, 0, P99_ATOMIC_LOCK_FREE_TYPES))

/**
 ** @brief Initialize the object behind @a OBJP with value @a VAL
 **
 ** @a VAL and the base type of @a OBJP must be assignment compatible.
 **
 ** @warning this should only be used in a context that is known to
 ** have no race condition
 **
 ** @memberof atomic_int
 **/
#define atomic_init(OBJP, VAL) ((void)(P00_AT(OBJP) = (VAL)))

/**
 ** @brief Store @a VAL into the object behind @a OBJP.
 **
 ** @a VAL and the base type of @a OBJP must be assignment compatible.
 **
 ** @remark this can be used in a context that is known to have a race
 ** condition
 **
 ** @memberof atomic_int
 **/
#define atomic_store(OBJP, DESIRED)                                                         \
({                                                                                          \
  __typeof__(*OBJP) volatile* p00_objp = OBJP;                                              \
  __typeof__(P00_AT(p00_objp)) p00_des = (DESIRED);                                         \
  if (!atomic_is_lock_free(p00_objp)) {                                                     \
    atomic_flag_lock(&p00_objp->p00_lock);                                                  \
    P00_AT(p00_objp) = p00_des;                                                             \
    atomic_flag_unlock(&p00_objp->p00_lock);                                                \
  }                                                                                         \
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)()                                                \
    (else {                                                                                 \
      __typeof__(P00_AI(p00_objp)) p00_desi =                                               \
        P99_TYPE_CHOICE(P00_AT(p00_objp), p00_des, 0, P99_ATOMIC_LOCK_FREE_TYPES);          \
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
         P99_ATOMIC_LOCK_FREE_TYPES);                                                       \
    })                                                                                      \
 })

/**
 ** @brief Return the value of the object behind @a OBJP.
 **
 ** @remark this can be used in a context that is known to have a race
 ** condition
 **
 ** @memberof atomic_int
 **/
#define atomic_load(OBJP)                                                     \
({                                                                            \
  __typeof__(*OBJP) volatile* p00_objp = OBJP;                                \
  __typeof__(P00_AT(p00_objp)) p00_ret;                                       \
  if (!atomic_is_lock_free(p00_objp)) {                                       \
    atomic_flag_lock(&p00_objp->p00_lock);                                    \
    p00_ret = P00_AT(p00_objp);                                               \
    atomic_flag_unlock(&p00_objp->p00_lock);                                  \
  }                                                                           \
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)                                    \
    (else p00_ret = P00_AT(p00_objp);)                                        \
    (else {                                                                   \
      p00_ret =                                                               \
        P99_TYPE_CHOICE(p00_ret,                                              \
                        __sync_val_compare_and_swap(&P00_AI(p00_objp), 0, 0), \
                        P00_AT(p00_objp),                                     \
                        P99_ATOMIC_LOCK_FREE_TYPES);                          \
    })                                                                        \
  /* cast to the same type to be sure that the result is an lvalue */         \
  (__typeof__(P00_AT(p00_objp)))p00_ret;                                      \
 })

/**
 ** @brief Atomically compare @a *OBJP to @a *EXPECTED and set it to
 ** @a DESIRED if they are equal.
 **
 ** @return true if both values agree and @c false otherwise.  If the
 ** two values are not the same, the current value is returned in @a
 ** *EXPECTED, additionally.
 **
 ** The base type of @a OBJP and @a *EXPECTED must be compatible, that
 ** is they must be the same if all qualifiers are taken out.
 **
 ** @a DESIRED must be assignment compatible with the base type of @a
 ** OBJP.
 **
 ** @memberof atomic_int
 **/
#define atomic_compare_exchange_weak(OBJP, EXPECTED, DESIRED)                      \
({                                                                                 \
  _Bool p00_ret;                                                                   \
  __typeof__(*OBJP) volatile* p00_objp = OBJP;                                     \
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
  P99_IF_EMPTY(P99_ATOMIC_LOCK_FREE_TYPES)                                         \
    (else p00_ret = P00_AT(p00_objp);)                                             \
    (else {                                                                        \
      __typeof__(P00_AI(p00_objp)) p00_desi =                                      \
        P99_TYPE_CHOICE(P00_AT(p00_objp), p00_des, 0, P99_ATOMIC_LOCK_FREE_TYPES); \
      __typeof__(P00_AI(p00_objp))* p00_expi =                                     \
        P99_TYPE_CHOICE(P00_AT(p00_objp), p00_exp, 0, P99_ATOMIC_LOCK_FREE_TYPES); \
      __typeof__(P00_AI(p00_objp)) p00_vali =                                      \
        P99_TYPE_CHOICE                                                            \
        (P00_AT(p00_objp),                                                         \
         __sync_val_compare_and_swap(&P00_AI(p00_objp), *p00_expi, p00_desi),      \
         0,                                                                        \
         P99_ATOMIC_LOCK_FREE_TYPES);                                              \
      p00_ret = (*p00_expi == p00_vali);                                           \
      if (!p00_ret) *p00_expi = p00_vali;                                          \
    })                                                                             \
  p00_ret;                                                                         \
 })

#define P00_FETCH_OP(OBJP, OPERAND, BUILTIN, OPERATOR)                  \
({                                                                      \
  __typeof__(*OBJP) volatile* p00_objp = OBJP;                          \
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
        P99_TYPE_CHOICE(p00_ret, p00_op, 0, P99_ATOMIC_LOCK_FREE_TYPES);\
      p00_ret =                                                         \
        P99_TYPE_CHOICE(p00_ret,                                        \
                        BUILTIN(&P00_AI(p00_objp), p00_opi),            \
                        P00_AT(p00_objp),                               \
                        P99_ATOMIC_LOCK_FREE_TYPES);                    \
    })                                                                  \
    p00_ret;                                                            \
 })

/**
 ** @brief Atomically add @a OPERAND to @a *OBJP.
 **
 ** @return the current value hidden in @a OBJP before the operation.
 **
 ** @a OPERAND must be assignment compatible with the base type of @a
 ** OBJP.
 **
 ** The base type of @a OBJP must have an operator @c +=.
 **
 ** @memberof atomic_int
 **/
#define atomic_fetch_add(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_add, +=)


/**
 ** @brief Atomically subtract @a OPERAND from @a *OBJP.
 **
 ** @return the current value hidden in @a OBJP before the operation.
 **
 ** @a OPERAND must be assignment compatible with the base type of @a
 ** OBJP.
 **
 ** The base type of @a OBJP must have an operator @c -=.
 **
 ** @memberof atomic_int
 **/
#define atomic_fetch_sub(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_sub, -=)

/**
 ** @brief Atomically do a bitwise or operation between @a OPERAND and
 ** @a *OBJP.
 **
 ** @return the current value hidden in @a OBJP before the operation.
 **
 ** @a OPERAND must be assignment compatible with the base type of @a
 ** OBJP.
 **
 ** The base type of @a OBJP must have an operator @c |=.
 **
 ** @memberof atomic_int
 **/
#define atomic_fetch_or(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_or, |=)

/**
 ** @brief Atomically do a bitwise and operation between @a OPERAND
 ** and @a *OBJP.
 **
 ** @return the current value hidden in @a OBJP before the operation.
 **
 ** @a OPERAND must be assignment compatible with the base type of @a
 ** OBJP.
 **
 ** The base type of @a OBJP must have an operator @c &=.
 **
 ** @memberof atomic_int
 **/
#define atomic_fetch_and(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_and, &=)

/**
 ** @brief Atomically do a bitwise xor operation between @a OPERAND
 ** and @a *OBJP.
 **
 ** @return the current value hidden in @a OBJP before the operation.
 **
 ** @a OPERAND must be assignment compatible with the base type of @a
 ** OBJP.
 **
 ** The base type of @a OBJP must have an operator @c ^=.
 **
 ** @see atomic_int
 ** @see _Atomic
 **/
#define atomic_fetch_xor(OBJP, OPERAND) P00_FETCH_OP((OBJP), (OPERAND), __sync_fetch_and_xor, ^=)


/**
 ** @}
 **/

#endif
