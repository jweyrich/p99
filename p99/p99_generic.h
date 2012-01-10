/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef     P99_GENERIC_H_
# define    P99_GENERIC_H_

#include "p99_for.h"

/**
 ** @file
 **/

/**
 ** @addtogroup C11_keywords
 **
 ** @{
 **/

#define P00_GENERIC_TYPE(T, EXP) T
#define P00_GENERIC_EXP_(T, EXP) (EXP)
#define P00_GENERIC_LIT_(T, EXP) (EXP){ 0 }

#define P00_GENERIC_EXP(EXP, PAIR, I) P00_GENERIC_EXPRESSION(P00_GENERIC_EXP_, EXP, PAIR, I)
#define P00_GENERIC_LIT(EXP, PAIR, I) P00_GENERIC_EXPRESSION(P00_GENERIC_LIT_, EXP, PAIR, I)

#if __has_extension(c_generic_selections) || (__STDC_VERSION__ >= 201012L)

#define P00_GENERIC_EXPRESSION(OP EXP, PAIR, I)                         \
P00_GENERIC_TYPE PAIR: OP PAIR

#define P00_GENERIC_(N, MOP, EXP, DEF, ...)                      \
_Generic                                                       \
((EXP),                                                        \
 P99_IF_EMPTY(DEF)()(default: (DEF),)                          \
 P99_FOR((EXP), N, P00_SEQ, MOP, __VA_ARGS__)      \
 )

#elif defined(__GNUC__)

#define P00_GENERIC_CLOSE(A,B,C) )

#define P00_GENERIC_EXPRESSION(OP, EXP, PAIR, I)                      \
__builtin_choose_expr                                                 \
(__builtin_types_compatible_p(__typeof__ EXP, P00_GENERIC_TYPE PAIR), \
 OP PAIR

#define P00_GENERIC_(N, MOP, EXP, DEF, ...)                               \
  P99_FOR((EXP), N, P00_SEQ, MOP, __VA_ARGS__),                       \
    P99_IF_EMPTY(DEF)(&(const volatile struct { int p00_v; }){ .p00_v = 0 })(DEF) \
    P99_FOR(, N, P00_SER, P00_GENERIC_CLOSE, P99_DUPL(N, ))

#endif

#define P00_GENERIC0(MOP, EXP, DEF, ...)        \
P00_GENERIC_                                    \
(                                               \
 P99_NARG(__VA_ARGS__),                         \
 P00_ROBUST(MOP),                               \
 P00_ROBUST(EXP),                               \
 P00_ROBUST(DEF),                               \
 __VA_ARGS__)

#define P00_GENERIC(N, ...) P99_IF_LT(N, 4)()(P00_GENERIC0(__VA_ARGS__))




/**
 ** @brief Type generic expression in anticipation of C11 @c _Generic
 **
 ** This macro expects a syntax that is similar to that of C11's @c
 ** _Generic keyword. The syntax that we support is as follows
 **
 ** @code
 ** P99_GENERIC(a,
 **             , // empty default expression
 **             (int*, a),
 **             (double*, x));
 ** @endcode
 **
 ** That is an expression @a EXP, followed by a default value @a DEF,
 ** followed by a list of type value pairs. So here this is an
 ** expression that depending on the type of @c a will have a type of
 ** @c int* or @c double* that will be set to @c a or @c x,
 ** respectively.
 **
 ** In C11 syntax, the above would be coded with some kind of "label"
 ** syntax:
 **
 ** @code
 ** _Generic(a,
 **          int*: a,
 **          double*: x);
 ** @endcode
 **
 ** As you can see above, the default value can be omitted. If so, it
 ** is replaced with some appropriate expression that should usually
 ** give you a syntax error.
 **
 ** If there is a default expression, that one is used when none of
 ** the types matches:
 **
 ** @code
 ** a = P99_GENERIC(a + b,
 **                 max_uintmax,
 **                 (int, max_int),
 **                 (long, max_long),
 **                 (long long, max_llong),
 **                 (float, max_float),
 **                 (double, max_double))(a, b);
 ** @endcode
 **
 ** In C11 syntax
 ** @code
 ** a = _Generic(a + b,
 **              default: max_uintmax,
 **              int: max_int,
 **              long: max_long,
 **              long long: max_llong,
 **              float: max_float,
 **              double: max_double)(a, b);
 ** @endcode
 **
 ** Here all the expressions evaluate to a function specifier. If
 ** <code>a + b</code> is @c int, ... or @c double the
 ** appropriate maximum function is choosen for that type. If none of
 ** these matches, the one for @c uintmax_t is choosen. The
 ** corresponding function is then evaluated with @c a and @c b as
 ** arguments.
 **
 ** - because the choice expression is <code>a + b</code> its type is
 **   the promoted common type of @c a and @c b. E.g for all types
 **   that are narrower than @c int, e.g @c short, normally @c int
 **   will be the type of the expression and @c max_int will be the
 **   function. If @c a would be @c unsigned and @c b would be @c
 **   double the result would be @c double, too.
 **
 ** - the return type of the @c _Generic expression is a
 **   function to two arguments. If it would be for @c int, e.g, the
 **   type would be <code>int ()(int, int)</code>. So the return type
 **   of the function call would be @c int in that case.
 **
 ** - the arguments are promoted and converted to the expected type of
 **   the choosen function
 **
 ** @remark If the compiler is C11 complying, the ::P99_GENERIC
 ** expression will just be translated to the corresponding @c
 ** _Generic expression.
 **
 ** @remark Otherwise only gcc and compatible compilers are supported.
 **/
#define P99_GENERIC(...) P00_GENERIC(P99_NARG(__VA_ARGS__), P00_GENERIC_EXP, __VA_ARGS__)

#define P99_GENERIC_LITERAL(...) P00_GENERIC(P99_NARG(__VA_ARGS__), P00_GENERIC_LIT, __VA_ARGS__)

/**
 ** @}
 **/

/**
 ** @addtogroup C11_types Generic identification of families of types
 **
 ** @{
 **/

#define P00_TYPE_CHOICE(VAL, T, I) (T, VAL)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_CHOICE, 0)
P00_DOCUMENT_MACRO_ARGUMENT(P99_TYPE_CHOICE, 1)
#define P99_TYPE_CHOICE(EXP, MATCH, DEF, ...)                                  \
P99_GENERIC                                                                    \
((EXP),                                                                        \
 DEF,                                                                          \
 P99_FOR(MATCH, P99_NARG(__VA_ARGS__), P00_SEQ, P00_TYPE_CHOICE, __VA_ARGS__))

/**
 ** @addtogroup type_generic Generic macros that classify expressions
 **/

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_UNSIGNED, 0)
#define P99_TYPE_UNSIGNED(EXP)      P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_UNSIGNED_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_SIGNED, 0)
#define P99_TYPE_SIGNED(EXP)        P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_SIGNED_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_REAL_FLOATING, 0)
#define P99_TYPE_REAL_FLOATING(EXP) P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_REAL_FLOATING_TYPES)
#ifndef __STDC_NO_COMPLEX__
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_COMPLEX, 0)
# define P99_TYPE_COMPLEX(EXP)       P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_COMPLEX_TYPES)
#endif
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_FLOATING, 0)
#define P99_TYPE_FLOATING(EXP)      P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_FLOATING_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_BASIC, 0)
#define P99_TYPE_BASIC(EXP)         P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_BASIC_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_CHARACTER, 0)
#define P99_TYPE_CHARACTER(EXP)     P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_CHARACTER_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_INTEGER, 0)
#define P99_TYPE_INTEGER(EXP)       P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_INTEGER_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_REAL, 0)
#define P99_TYPE_REAL(EXP)          P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_REAL_TYPES)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_ARITHMETIC, 0)
#define P99_TYPE_ARITHMETIC(EXP)    P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_ARITHMETIC_TYPES)

/**
 ** @}
 **/

/**
 ** @}
 **/

inline int* p00_generic_test(int * a) {
  double *x;
  switch (*a % 3) {
  case 0:
    return P99_GENERIC(&*a,
                       /* empty default expression */,
                       (int*, a),
                       (double*, x+1),
                       (float*, x+2));
  case 1:
    return P99_GENERIC(&*a,
                       /* another form of empty default */,
                       (double[], x+0),
                       (int*, a),
                       (float*, x+2));
  default:
    return P99_GENERIC(&*a,
                       /* default expression: */ x,
                       (double*, x+0),
                       (float[7], x+1),
                       (int*, a));
  }
}

#endif
