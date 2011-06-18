/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2010-2011 Jens Gustedt, INRIA, France                      */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
/**
 ** @file p99_type.h
 */

#ifndef   	P99_TYPE_H_
# define   	P99_TYPE_H_

#include "p99_paste.h"

/**
 ** @addtogroup types Type facilities
 ** @brief Macros that help you handle different kind of types,
 ** forward declaring @c struct of @c union, and handle @c enum.
 ** @{
 **/


/**
 ** @brief forward declaration of a @c struct @a NAME
 **
 ** This combines a @c struct forward declaration and a @c typedef
 ** that should allow you to use @c struct @a NAME and just @a NAME
 ** interchangeably.
 **
 ** This declaration should also be suitable to be included into a C++
 ** source.
 **/
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_STRUCT, 0)
#define P99_DECLARE_STRUCT(NAME)                               \
/*! @copydoc struct NAME */                                    \
typedef struct NAME NAME


/**
 ** @brief forward declaration of a @c union @a NAME
 **
 ** This combines a @c union forward declaration and a @c typedef
 ** that should allow you to use @c union @a NAME and just @a NAME
 ** interchangeably.
 **
 ** This declaration should also be suitable to be included into a C++
 ** source.
 **/
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_UNION, 0)
#define P99_DECLARE_UNION(NAME)                                \
/*! @copydoc union NAME */                                     \
typedef union NAME NAME

/* For each one word integer type have a signed and unsigned variant. */
#define P00_ONE_TOK(T, NAME)                                                    \
/*! @brief a `one token' abreviation for <code>T</code> */                      \
/*! These type names are needed to have a valid naming scheme for functions. */ \
typedef T NAME

/**
 ** @brief Declare derived pointer types with endings "_ptr" and "_cptr"
 **/
P00_DOCUMENT_TYPE_IDENTIFIER_ARGUMENT(P99_POINTER_TYPE, 0)
#define P99_POINTER_TYPE(T)                                    \
/*! @brief a pointer to @c T */                                \
typedef T *P99_PASTE2(T, _ptr);                                \
/*! @brief a @c const pointer to @c T */                       \
typedef T const*P99_PASTE2(T, _cptr)

/**
 ** @brief Declare trivial init and destroy functions for type @a T.
 **/
P00_DOCUMENT_TYPE_IDENTIFIER_ARGUMENT(P99_PLAIN_TYPE, 0)
#define P99_PLAIN_TYPE(T)                                      \
/*! @brief initialize the object that @a id points to by 0. */ \
p99_inline T* P99_PASTE2(T, _init)(T *id) {                    \
  if (id) *id = (T){ 0 };                                      \
  return id;                                                   \
}                                                              \
/*! @brief destroy the object that @a id points to. */         \
p99_inline void P99_PASTE2(T, _destroy)(T*  id) {              \
  /* empty */                                                  \
  (void)id;                                                    \
}

/**
 ** @brief Declare trivial init and destroy functions for type @a T
 ** and its pointer derivatives.
 **/
P00_DOCUMENT_TYPE_IDENTIFIER_ARGUMENT(P99_DERIVED_TYPES, 0)
#define P99_DERIVED_TYPES(T)                                   \
P99_PLAIN_TYPE(T);                                             \
P99_PLAIN_TYPE(P99_PASTE2(T, _cptr));                          \
P99_PLAIN_TYPE(P99_PASTE2(T, _ptr))

/** @}
 **/

#endif 	    /* !P99_TYPE_H_ */
