/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_ENUM_H_
# define   	ORWL_ENUM_H_

#include "orwl_once.h"

/**
 ** @brief Declare a simple enumeration type.
 **
 ** This macro only works for enumeration types that have no
 ** assignments to the constants. To define an enumeration type @c
 ** color Use it in something as
 ** @code
 ** DECLARE_ENUM(color, red, green, blue);
 ** @endcode
 **
 ** As additional benefits you obtain:
 **
 ** - constants @c color_min and @c color_max that in this example
 **   here would correspond to @c red and @c blue, respectively
 ** - a function @c color_getname that returns a string containing the
 **   name of its argument.
 **
 ** To have this functional, you have to put an line
 ** @code
 ** DEFINE_ENUM(color);
 ** @endcode
 **
 ** in one of your object files.
 **
 ** Unfortunately you can't use doxygen like comments inside the
 ** argument list to document the enumeration constants. You have to
 ** document the constants separately. For the above example you would
 ** do
 ** @code
 **   /// @var color red
 **   /// the most redish color of all colors
 ** @endcode
 ** Observe the bizarre naming convention, here. Although in C (and
 ** C++) `red' is declared in the same scope as `enum color' you have
 ** to prefix it with `color' such that the documentation lands inside
 ** the one for `color'.
 **/
#define DECLARE_ENUM(T, ...)                                                                                   \
/*! @see DECLARE_ENUM was used for the declaration of this enumeration type */                                 \
/*! @see T ## _getname for access to the names of the constants as strings */                                  \
typedef enum { __VA_ARGS__ ,                                                                                   \
               /*! upper bound of the @ref T constants */                                                      \
               P99_PASTE2(T, _amount),                                                                         \
               /*! the largest @ref T constant */                                                              \
               P99_PASTE2(T, _max) = ((size_t)(P99_PASTE2(T, _amount)) - 1u),                                  \
               /*! the smallest @ref T constant */                                                             \
               P99_PASTE2(T, _min) = 0                                                                         \
} T;                                                                                                           \
 extern char const* P99_PASTE3(p00_, T, _names)[P99_PASTE2(T, _amount)];                                       \
DECLARE_ONCE(T);                                                                                               \
 /*! @brief Get a string with the name of constant @a x of type @ref T */                                      \
char const* P99_PASTE2(T, _getname)(T x);                                                                      \
static char P99_PASTE3(p00_, T, _concat)[] =  # __VA_ARGS__;                                                   \
enum P99_PASTE2(p00_decl_enum_, T) { P99_PASTE3(p00_, T, _concat_len) = sizeof(P99_PASTE3(p00_, T, _concat)) }


/**
 ** @brief Define the necessary symbols for a simple enumeration type.
 **
 ** Use this with DECLARE_ENUM(), which see.
 **/
#define DEFINE_ENUM(T)                                                                                  \
char const* P99_PASTE2(T, _getname)(T x) {                                                              \
  unsigned pos = x;                                                                                     \
  INIT_ONCE(T);                                                                                         \
  return (pos < P99_PASTE2(T, _amount)) ? P99_PASTE3(p00_, T, _names)[pos] : "((" #T ")unknown value)"; \
}                                                                                                       \
/* Ensure that the table is generated in this object file */                                            \
char const* P99_PASTE3(p00_, T, _names)[P99_PASTE2(T, _amount)] = { 0 };                                \
DEFINE_ONCE(T) {                                                                                        \
  char *head = P99_PASTE3(p00_, T, _concat);                                                            \
  for (T i = P99_PASTE2(T, _min); i < P99_PASTE2(T, _max); ++i) {                                       \
    P99_PASTE3(p00_, T, _names)[i] = head;                                                              \
    head = index(head, ',');                                                                            \
    for (; *head == ',' || *head == ' '; ++head)                                                        \
      *head = '\0';                                                                                     \
  }                                                                                                     \
  P99_PASTE3(p00_, T, _names)[P99_PASTE2(T, _max)] = head;                                              \
}                                                                                                       \
P99_MACRO_END(define_enum_, T)


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
#define DECLARE_STRUCT(NAME) typedef struct NAME NAME


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
#define DECLARE_UNION(NAME) typedef union NAME NAME

#endif 	    /* !ORWL_ENUM_H_ */
