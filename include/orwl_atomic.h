/*
** orwl_atomic.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Dec 19 00:34:52 2010 Jens Gustedt
** Last update Sun Dec 19 00:34:52 2010 Jens Gustedt
*/

#ifndef   	ORWL_ATOMIC_H_
# define   	ORWL_ATOMIC_H_

#include "orwl_int.h"

/**
 ** @file
 ** @brief Replacement for some functionalities in "stdatomic.h"
 **
 ** The next C standard will contain interfaces for atomic
 ** operations. These will help to speed up things e.g on start up of
 ** threads.
 **
 ** For the moment we don't even have a compiler/library that
 ** implements this directly, but there are extensions that provide
 ** similar interfaces. So we wrap these to provide some of the
 ** interfaces.
 **
 ** The standard prescribes that the operations on signed types should
 ** be done as if the representation would be two's complement. In
 ** essence this means that we may do all operations on unsigned types
 ** and transpose them to signed types if necessary.
 **
 ** Our main use for these operations are counting. So the only atomic
 ** type that is used is ::atomic_size_t: we don't use the idea that
 ** the functions in "stdatomic.h" are <em>type generic</em>. This
 ** avoids a lot of difficulties when implementing "overloaded"
 ** functions with macros.
 **/


#ifdef ATOMIC_OPS
# include <stdatomic.h>
#else

/**
 ** @brief The only atomic type that the wrappers use.
 **
 ** If you need a signed type use this one here and then do an
 ** appropriate conversion to @c ptrdiff_t.
 **/
typedef size_t atomic_size_t;

/**
 ** @brief Return a consistent value of @a object.
 **/
inline
size_t atomic_load(atomic_size_t volatile *object);

/**
 ** @brief Store a consistent value @a desired in @a object by erasing
 ** any possible other contents.
 **/
inline
void atomic_store(atomic_size_t volatile *object, size_t desired);

/**
 ** @brief Store a consistent value @a desired in @a object if the
 ** previous value corresponds to @a expected.
 **
 ** If the operation fails, the actual value is not changed and that
 ** value is returned in @a expected.
 **/
inline
_Bool atomic_compare_exchange_weak(atomic_size_t volatile *object, size_t *expected, size_t desired);


/**
 ** @brief Add value @a operand to the value in @a object and return
 ** the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret + operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_add(atomic_size_t volatile *object, size_t operand);

/**
 ** @brief Substract value @a operand from the value in @a object and
 ** return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret - operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_sub(atomic_size_t volatile *object, size_t operand);

/**
 ** @brief Bitwise or value @a operand with the value in @a object and
 ** return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret | operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_or(atomic_size_t volatile *object, size_t operand);

/**
 ** @brief Bitwise exclusive or value @a operand with the value in @a
 ** object and return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret ^ operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_xor(atomic_size_t volatile *object, size_t operand);

/**
 ** @brief Bitwise and value @a operand with the value in @a
 ** object and return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret & operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_and(atomic_size_t volatile *object, size_t operand);

#ifdef __GNUC__
#ifndef GNUC_NO_SYNC

inline
_Bool atomic_compare_exchange_weak(atomic_size_t volatile *object, size_t *expected, size_t desired) {
  size_t val = __sync_val_compare_and_swap(object, *expected, desired);
  _Bool ret = (val == *expected);
  if (!ret) *expected = val;
  return ret;
}

inline
void atomic_store(atomic_size_t volatile *object, size_t desired){
  for (size_t expected = desired;;) {
    size_t val = __sync_val_compare_and_swap(object, expected, desired);
    if (val == expected) break;
  }
}


inline
size_t atomic_fetch_add(atomic_size_t volatile *object, size_t operand) {
  return __sync_fetch_and_add(object, operand);
}

inline
size_t atomic_fetch_sub(atomic_size_t volatile *object, size_t operand) {
  return __sync_fetch_and_sub(object, operand);
}

inline
size_t atomic_fetch_or(atomic_size_t volatile *object, size_t operand) {
  return __sync_fetch_and_or(object, operand);
}

inline
size_t atomic_fetch_xor(atomic_size_t volatile *object, size_t operand) {
  return __sync_fetch_and_xor(object, operand);
}

inline
size_t atomic_fetch_and(atomic_size_t volatile *object, size_t operand) {
  return __sync_fetch_and_and(object, operand);
}

inline
size_t atomic_load(atomic_size_t volatile *object) {
  return atomic_fetch_xor(object, 0);
}

#else
#ifdef __arm__
#include "orwl_atomic_arm.h"
#else
#if defined(__x86_64__) || defined(__i386__)
#include "orwl_atomic_x86.h"
#endif
#endif
#endif
#endif
#endif
#endif 	    /* !ORWL_ATOMIC_H_ */
