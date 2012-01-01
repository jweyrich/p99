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
#ifndef     P99_GENERIC_H_
# define    P99_GENERIC_H_

#include "p99_for.h"

/**
 ** @file
 **/

#define P00_GENERIC_TYPE(T, EXP) T
#define P00_GENERIC_EXP(T, EXP) EXP

#if __STDC_VERSION__ >= 201012L

#define P00_GENERIC_BASE(EXP, PAIR, I)          \
P00_GENERIC_TYPE PAIR: (P00_GENERIC_EXP PAIR)

#define P00_GENERIC(N, EXP, DEF, ...)                           \
_Generic                                                        \
((EXP),                                                         \
 P99_IF_EMPTY(DEF)()(default: (DEF),)                           \
 P99_FOR((EXP), N, P00_SEQ, P00_GENERIC_BASE, __VA_ARGS__)      \
 )

#elif defined(__GNUC__)

#define P00_GENERIC_CLOSE(A,B,C) )

#define P00_GENERIC_BASE(EXP, PAIR, I)                                  \
__builtin_choose_expr                                                   \
(__builtin_types_compatible_p(__typeof__ EXP, P00_GENERIC_TYPE PAIR),   \
 (P00_GENERIC_EXP PAIR)

#define P00_GENERIC(N, EXP, DEF, ...)                                   \
  P99_FOR((EXP), N, P00_SEQ, P00_GENERIC_BASE, __VA_ARGS__),            \
    P99_IF_EMPTY(DEF)(&(const volatile struct { int p00_v; }){ .p00_v = 0 })(DEF) \
    P99_FOR(, N, P00_SER, P00_GENERIC_CLOSE, P99_DUPL(N, ))


#endif

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
 ** Here all the expressions evaluate to a function pointer. If
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
 ** - the return type of the @c _Generic expression is a pointer of
 **   function to two arguments. If it would be for @c int, e.g, the
 **   type would be <code>int (*)(int, int)</code>. So the return type
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
 ** @addtogroup C11
 **/
#define P99_GENERIC(EXP, DEF, ...) P00_GENERIC(P99_NARG(__VA_ARGS__), EXP, DEF, __VA_ARGS__)

#define P00_TYPE_EXPRESSION_(_0, T, I) (T, 1)

#define P00_TYPE_EXPRESSION(EXP, ...)                                   \
P99_GENERIC                                                             \
((EXP),                                                                 \
 0,                                                                     \
 P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEQ, P00_TYPE_EXPRESSION_, __VA_ARGS__))

#define P99_TYPE_INTEGER(EXP)                   \
P00_TYPE_EXPRESSION                             \
((EXP),                                         \
 _Bool,                                         \
 char,                                          \
 signed char,                                   \
 unsigned char,                                 \
 signed short,                                  \
 unsigned short,                                \
 signed,                                        \
 unsigned,                                      \
 signed long,                                   \
 unsigned long,                                 \
 signed long long,                              \
 unsigned long long)

#if CHAR_MIN < 0
/* char is signed */
#define P99_TYPE_UNSIGNED(EXP)                  \
P00_TYPE_EXPRESSION                             \
((EXP),                                         \
 _Bool,                                         \
 unsigned char,                                 \
 unsigned short,                                \
 unsigned,                                      \
 unsigned long,                                 \
 unsigned long long)

#define P99_TYPE_SIGNED(EXP)                    \
P00_TYPE_EXPRESSION                             \
((EXP),                                         \
 char,                                          \
 signed char,                                   \
 signed short,                                  \
 signed,                                        \
 signed long,                                   \
 signed long long)
#else
/* char is signed */
#define P99_TYPE_UNSIGNED(EXP)                  \
P00_TYPE_EXPRESSION                             \
((EXP),                                         \
 _Bool,                                         \
 char,                                          \
 unsigned char,                                 \
 unsigned short,                                \
 unsigned,                                      \
 unsigned long,                                 \
 unsigned long long)

#define P99_TYPE_SIGNED(EXP)                    \
P00_TYPE_EXPRESSION                             \
((EXP),                                         \
 signed char,                                   \
 signed short,                                  \
 signed,                                        \
 signed long,                                   \
 signed long long)
#endif

#define P99_TYPE_FLOATING(EXP)                  \
P00_TYPE_EXPRESSION                             \
((EXP),                                         \
 float,                                         \
 double,                                        \
 long double)



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
