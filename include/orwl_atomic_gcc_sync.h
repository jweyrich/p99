
/**
 ** @file An implementation of the atomic generic functions on top of
 ** gcc's __sync builtins.
 **
 ** The idea is to generate inline functions that are specific to a
 ** particular size (and width) and select the correct one via a
 ** macro. This only works if the platform implements the exact width
 ** integer types.
 **/

#ifndef ORWL_ATOMIC_GCC_SYNC_H
#define ORWL_ATOMIC_GCC_SYNC_H

#define ORWL_MANG_(NAME, BITS) P99_PASTE3(NAME, _, BITS)
#define ORWL_AT_(BITS) P99_PASTE3(uint, BITS, _t)
#define ORWL_RT_(BITS) P99_PASTE3(int, BITS, _t)

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
#undef  ORWL_SYNC_MAX
#define ORWL_SYNC_MAX 8
#define ORWL_SYNC_1 8
#include "orwl_atomic_gcc_sync_inttypes.h"
#else
#define ORWL_SYNC_1 ORWL_SYNC_MAX
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
#undef  ORWL_SYNC_MAX
#define ORWL_SYNC_MAX 16
#define ORWL_SYNC_2 16
#include "orwl_atomic_gcc_sync_inttypes.h"
#else
#define ORWL_SYNC_2 ORWL_SYNC_MAX
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#undef  ORWL_SYNC_MAX
#define ORWL_SYNC_MAX 32
#define ORWL_SYNC_4 32
#include "orwl_atomic_gcc_sync_inttypes.h"
#else
#define ORWL_SYNC_4 ORWL_SYNC_MAX
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
#undef  ORWL_SYNC_MAX
#define ORWL_SYNC_MAX 64
#define ORWL_SYNC_8 64
#include "orwl_atomic_gcc_sync_inttypes.h"
#else
#define ORWL_SYNC_8 ORWL_SYNC_MAX
#endif

#define atomic_compare_exchange_weak(OBJECT, EXPECTED, DESIRED)         \
(sizeof(*(OBJECT)) == 1                                                 \
 ? P99_PASTE2(atomic_compare_exchange_weak_, ORWL_SYNC_1)(OBJECT, EXPECTED, DESIRED)            \
 : (sizeof(*(OBJECT)) == 2                                              \
    ? P99_PASTE2(atomic_compare_exchange_weak_, ORWL_SYNC_2)(OBJECT, EXPECTED, DESIRED)        \
    : (sizeof(*(OBJECT)) == 4                                           \
       ? P99_PASTE2(atomic_compare_exchange_weak_, ORWL_SYNC_4)(OBJECT, EXPECTED, DESIRED)     \
       : P99_PASTE2(atomic_compare_exchange_weak_, ORWL_SYNC_8)(OBJECT, EXPECTED, DESIRED))))

#define atomic_store(OBJECT, DESIRED)                                   \
(sizeof(*(OBJECT)) == 1                                                 \
 ? P99_PASTE2(atomic_store_, ORWL_SYNC_1)(OBJECT, DESIRED)              \
 : (sizeof(*(OBJECT)) == 2                                              \
    ? P99_PASTE2(atomic_store_, ORWL_SYNC_2)(OBJECT, DESIRED)           \
    : (sizeof(*(OBJECT)) == 4                                           \
       ? P99_PASTE2(atomic_store_, ORWL_SYNC_4)(OBJECT, DESIRED)        \
       : P99_PASTE2(atomic_store_, ORWL_SYNC_8)(OBJECT, DESIRED))))

#define atomic_load(OBJECT)                                     \
P99_SIGN_PROMOTE                                                \
((sizeof(*(OBJECT)) == 1                                        \
  ? P99_PASTE2(atomic_load_, ORWL_SYNC_1)(OBJECT)               \
  : (sizeof(*(OBJECT)) == 2                                     \
     ? P99_PASTE2(atomic_load_, ORWL_SYNC_2)(OBJECT)            \
     : (sizeof(*(OBJECT)) == 4                                  \
        ? P99_PASTE2(atomic_load_, ORWL_SYNC_4)(OBJECT)         \
        : P99_PASTE2(atomic_load_, ORWL_SYNC_8)(OBJECT)))),     \
 *(OBJECT))

#define atomic_fetch_add(OBJECT, OPERAND)                               \
P99_SIGN_PROMOTE                                                        \
((sizeof(*(OBJECT)) == 1                                                \
  ? P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_1)(OBJECT, OPERAND)         \
  : (sizeof(*(OBJECT)) == 2                                             \
     ? P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_2)(OBJECT, OPERAND)      \
     : (sizeof(*(OBJECT)) == 4                                          \
        ? P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_4)(OBJECT, OPERAND)   \
        : P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_8)(OBJECT, OPERAND)))), \
 *(OBJECT))

#define atomic_fetch_add(OBJECT, OPERAND)                               \
P99_SIGN_PROMOTE                                                        \
((sizeof(*(OBJECT)) == 1                                                \
  ? P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_1)(OBJECT, OPERAND)         \
  : (sizeof(*(OBJECT)) == 2                                             \
     ? P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_2)(OBJECT, OPERAND)      \
     : (sizeof(*(OBJECT)) == 4                                          \
        ? P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_4)(OBJECT, OPERAND)   \
        : P99_PASTE2(atomic_fetch_add_, ORWL_SYNC_8)(OBJECT, OPERAND)))), \
 *(OBJECT))

#define atomic_fetch_sub(OBJECT, OPERAND)                               \
P99_SIGN_PROMOTE                                                        \
((sizeof(*(OBJECT)) == 1                                                \
  ? P99_PASTE2(atomic_fetch_sub_, ORWL_SYNC_1)(OBJECT, OPERAND)         \
  : (sizeof(*(OBJECT)) == 2                                             \
     ? P99_PASTE2(atomic_fetch_sub_, ORWL_SYNC_2)(OBJECT, OPERAND)      \
     : (sizeof(*(OBJECT)) == 4                                          \
        ? P99_PASTE2(atomic_fetch_sub_, ORWL_SYNC_4)(OBJECT, OPERAND)   \
        : P99_PASTE2(atomic_fetch_sub_, ORWL_SYNC_8)(OBJECT, OPERAND)))), \
 *(OBJECT))

#define atomic_fetch_or(OBJECT, OPERAND)                                \
P99_SIGN_PROMOTE                                                        \
((sizeof(*(OBJECT)) == 1                                                \
  ? P99_PASTE2(atomic_fetch_or_, ORWL_SYNC_1)(OBJECT, OPERAND)          \
  : (sizeof(*(OBJECT)) == 2                                             \
     ? P99_PASTE2(atomic_fetch_or_, ORWL_SYNC_2)(OBJECT, OPERAND)       \
     : (sizeof(*(OBJECT)) == 4                                          \
        ? P99_PASTE2(atomic_fetch_or_, ORWL_SYNC_4)(OBJECT, OPERAND)    \
        : P99_PASTE2(atomic_fetch_or_, ORWL_SYNC_8)(OBJECT, OPERAND)))), \
 *(OBJECT))

#define atomic_fetch_xor(OBJECT, OPERAND)                               \
P99_SIGN_PROMOTE                                                        \
((sizeof(*(OBJECT)) == 1                                                \
  ? P99_PASTE2(atomic_fetch_xor_, ORWL_SYNC_1)(OBJECT, OPERAND)         \
  : (sizeof(*(OBJECT)) == 2                                             \
     ? P99_PASTE2(atomic_fetch_xor_, ORWL_SYNC_2)(OBJECT, OPERAND)      \
     : (sizeof(*(OBJECT)) == 4                                          \
        ? P99_PASTE2(atomic_fetch_xor_, ORWL_SYNC_4)(OBJECT, OPERAND)   \
        : P99_PASTE2(atomic_fetch_xor_, ORWL_SYNC_8)(OBJECT, OPERAND)))), \
 *(OBJECT))

#define atomic_fetch_and(OBJECT, OPERAND)                               \
P99_SIGN_PROMOTE                                                        \
((sizeof(*(OBJECT)) == 1                                                \
  ? P99_PASTE2(atomic_fetch_and_, ORWL_SYNC_1)(OBJECT, OPERAND)         \
  : (sizeof(*(OBJECT)) == 2                                             \
     ? P99_PASTE2(atomic_fetch_and_, ORWL_SYNC_2)(OBJECT, OPERAND)      \
     : (sizeof(*(OBJECT)) == 4                                          \
        ? P99_PASTE2(atomic_fetch_and_, ORWL_SYNC_4)(OBJECT, OPERAND)   \
        : P99_PASTE2(atomic_fetch_and_, ORWL_SYNC_8)(OBJECT, OPERAND)))), \
 *(OBJECT))

#endif
