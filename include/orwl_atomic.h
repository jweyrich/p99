/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
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


#if defined(ATOMIC_OPS) && ATOMIC_OPS > 0
# include <stdatomic.h>
#else

/**
 ** @brief The only atomic type that the wrappers use.
 **
 ** If you need a signed type use this one here and then do an
 ** appropriate conversion to @c ptrdiff_t.
 **/
typedef ATOMIC_TYPE atomic_size_t;

#ifndef ATOMIC_FLOAT
# if ATOMIC_SIZE >= 8
#  define ATOMIC_FLOAT double
# else
#  define ATOMIC_FLOAT float
# endif
#endif

/**
 ** @brief An atomic floating point type
 **
 ** Usually this should correspond to @c double but for narrow
 ** architectures it could be just @c float.
 **/
typedef ATOMIC_FLOAT atomic_float;

#ifdef __GNUC__
#ifndef GNUC_NO_SYNC
#include "orwl_atomic_gcc_sync.h"
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

#ifndef atomic_load
/**
 ** @brief Return a consistent value of @a object.
 **/
inline
size_t atomic_load(atomic_size_t volatile *object);
#endif

#ifndef atomic_store
/**
 ** @brief Store a consistent value @a desired in @a object by erasing
 ** any possible other contents.
 **/
inline
void atomic_store(atomic_size_t volatile *object, size_t desired);
#endif

#ifndef atomic_compare_exchange_weak
/**
 ** @brief Store a consistent value @a desired in @a object if the
 ** previous value corresponds to @a expected.
 **
 ** If the operation fails, the actual value is not changed and that
 ** value is returned in @a expected.
 ** @return @c true when value has successfully been stored, @c false
 ** otherwise.
 **/
inline
_Bool atomic_compare_exchange_weak(atomic_size_t volatile *object, size_t *restrict expected, size_t desired);
#endif

#ifndef atomic_fetch_add
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
#endif

#ifndef atomic_fetch_sub
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
#endif

#ifndef atomic_fetch_or
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
#endif

#ifndef atomic_fetch_xor
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
#endif

#ifndef atomic_fetch_and
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
#endif

/**
 ** @brief Declare atomic operations for type @a T, different from @c
 ** size_t.
 **
 ** This assumes that the size of @a T is less than or equal to that
 ** of @c size_t.
 **
 ** Three @c inline functions are declared by this macro. They
 ** implement the analogs of ::atomic_load, ::atomic_store and
 ** ::atomic_compare_exchange_weak for the given type. The naming
 ** convention is to just append the type name to the function name,
 ** e.g @c atomic_load_toto for a type named toto.
 **
 ** @see DEFINE_ATOMIC_OPS for a macro that ensures that the
 ** corresponding functions are really emitted.
 ** @see DECLARE_ATOMIC_OP to define a concrete operation on a
 ** specific type.
 **/
P00_DOCUMENT_TYPE_IDENTIFIER_ARGUMENT(DECLARE_ATOMIC_OPS, 0)
#define DECLARE_ATOMIC_OPS(T)                                              \
P99_COMPILETIME_ASSERT(sizeof(T) <= sizeof(atomic_size_t), atomic_size_t); \
/*! @brief Load a variable of type T atomically. */                        \
/*! @remark This supposes that T is of the same size as @c size_t */       \
/*! @see atomic_load */                                                    \
/*! @see T */                                                              \
inline                                                                     \
T P99_PASTE2(atomic_load_, T)(T volatile* object) {                        \
  union both { T t; atomic_size_t a; } ret;                                \
  union both volatile *o = (union both volatile *)object;                  \
  ret.a = atomic_load(&o->a);                                              \
  return ret.t;                                                            \
}                                                                          \
/*! @brief Store a variable of type T atomically. */                       \
/*! @remark This supposes that T is of the same size as @c size_t */       \
/*! @see atomic_store */                                                   \
/*! @see T */                                                              \
inline                                                                     \
void P99_PASTE2(atomic_store_, T)(T volatile *object, T desired) {         \
  union both { T t; atomic_size_t a; } des = { .t = desired };             \
  union both volatile *o = (union both volatile *)object;                  \
  atomic_store(&o->a, des.a);                                              \
}                                                                          \
/*! @brief Comare and swap for type T. */                                  \
/*! @remark This supposes that T is of the same size as @c size_t */       \
/*! @see atomic_compare_exchange_weak */                                   \
/*! @see T */                                                              \
inline                                                                     \
_Bool P99_PASTE2(atomic_compare_exchange_weak_, T)(T volatile *object,     \
                                                   T *restrict expected,   \
                                                   T desired) {            \
  register union both { T t; atomic_size_t a; } des = { .t = desired };    \
  register union both volatile *o = (union both volatile *)object;         \
  register union both *restrict e = (union both*restrict)expected;         \
  return atomic_compare_exchange_weak(&o->a, &e->a, des.a);                \
}

/**
 ** @brief Macro to instantiate the functions declared through ::DECLARE_ATOMIC_OPS.
 **/
#define DEFINE_ATOMIC_OPS(T)                                                        \
P99_INSTANTIATE(T, P99_PASTE2(atomic_load_, T), T volatile* object);                \
P99_INSTANTIATE(void, P99_PASTE2(atomic_store_, T), T volatile *object, T desired); \
P99_INSTANTIATE(_Bool,                                                              \
                P99_PASTE2(atomic_compare_exchange_weak_, T),                       \
                T volatile *object,                                                 \
                T *restrict expected,                                               \
                T desired)

DECLARE_ATOMIC_OPS(atomic_float);

/**
 ** @brief Declare an atomic operation on type @a T.
 **
 ** The naming convention here is "atomic_fetch_" followed by the type
 ** name @a T, followed by an underscore character, followed by @a
 ** NAME, e.g ::atomic_fetch_double_add.
 **/
P00_DOCUMENT_TYPE_IDENTIFIER_ARGUMENT(DECLARE_ATOMIC_OP, 0)
#define DECLARE_ATOMIC_OP(T, NAME, OP)                                                      \
/*! @brief Perform an atomic NAME of @a operand to the object to which @a object points. */ \
inline                                                                                      \
T P99_PASTE4(atomic_fetch_, T, _, NAME)(register T volatile *object, register T operand) {  \
  T expect = *object;                                                                       \
  for (register T oldval = expect;; oldval = expect) {                                      \
    register T desired = expect;                                                            \
    desired OP operand;                                                                     \
    if (P99_PASTE2(atomic_compare_exchange_weak_, T)(object, &expect, desired))             \
      return oldval;                                                                        \
  }                                                                                         \
}                                                                                           \
P99_MACRO_END(DECLARE_ATOMIC_OP, T, NAME)

#define DEFINE_ATOMIC_OP(T, NAME, OP)                                              \
P99_INSTANTIATE(T, P99_PASTE4(atomic_fetch_, T, _, NAME), T volatile *, T operand)


DECLARE_ATOMIC_OP(atomic_float, add, +=);
DECLARE_ATOMIC_OP(atomic_float, minus, -=);
DECLARE_ATOMIC_OP(atomic_float, mult, *=);
DECLARE_ATOMIC_OP(atomic_float, div, /=);


#endif 	    /* !ORWL_ATOMIC_H_ */
