/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/* (C) copyright  2012 William Morris                                         */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_GENERIC_H_
# define    P99_GENERIC_H_

#include "p99_for.h"

/**
 ** @file
 **/

#ifndef P00_DOXYGEN

#define P00_GENERIC_TYPE(T, EXP) T
#define P00_GENERIC_SIZE_(UI, EXP) char(*)[UI]
#define P00_GENERIC_EXP_(T, EXP) (EXP)
#define P00_GENERIC_LIT_(T, EXP) (EXP){ 0 }

#define P00_GENERIC_SIZE_LIT(EXP, PAIR, I) P00_GENERIC_EXPRESSION(P00_GENERIC_SIZE_, P00_GENERIC_LIT_, EXP, PAIR, I)
#define P00_GENERIC_SIZE(EXP, PAIR, I) P00_GENERIC_EXPRESSION(P00_GENERIC_SIZE_, P00_GENERIC_EXP_, EXP, PAIR, I)
#define P00_GENERIC_EXP(EXP, PAIR, I) P00_GENERIC_EXPRESSION(P00_GENERIC_TYPE, P00_GENERIC_EXP_, EXP, PAIR, I)
#define P00_GENERIC_LIT(EXP, PAIR, I) P00_GENERIC_EXPRESSION(P00_GENERIC_TYPE, P00_GENERIC_LIT_, EXP, PAIR, I)

#if p99_has_extension(c_generic_selections)

#define P00_GENERIC_EXPRESSION(TOP, EOP, EXP, PAIR, I)         \
TOP PAIR: EOP PAIR

#define P00_GENERIC_(N, MOP, EXP, DEF, ...)                    \
p99_extension                                                  \
_Generic                                                       \
((EXP),                                                        \
 P99_IF_EMPTY(DEF)()(default: (DEF),)                          \
 P99_FOR((EXP), N, P00_SEQ, MOP, __VA_ARGS__)                  \
 )

#elif defined(__GNUC__)

#define P00_GENERIC_CLOSE(A,B,C) )

#define P00_GENERIC_EXPRESSION(TOP, EOP, EXP, PAIR, I)         \
__builtin_choose_expr                                          \
(__builtin_types_compatible_p(__typeof__ EXP, TOP PAIR),       \
 EOP PAIR

/* If no default is given, the idea of _Generic is to abort
   compilation. To emulate this behavior we would have to produce an
   expression that is valid as such, but can't be used in any valid
   context. Such an expression doesn't exist, I think, since any valid
   expression can always be used in a void context.

   The idea for that case is to use an extern function that never will
   be defined and that bears a special attribute that inhibits this
   function to be called.  This function is then called through a
   sizeof expression for VLA with side effect, such that it only
   triggers when that particular branch is taken. */

__attribute__((__error__("Invalid choice in type generic expression")))
extern size_t p00_invalid_type_in_generic(char const*);

#define P00_INVALID_TYPE_IN_GENERIC(EXP, STR)                  \
p00_invalid_type_in_generic(__FILE__                           \
                            P99_STRINGIFY(:__LINE__)           \
                            ": invalid type generic choice `"  \
                            P99_STRINGIFY(EXP)                 \
                            "` for "                           \
                            STR)

#define P00_GENERIC_(N, MOP, EXP, DEF, ...)                                \
  P99_FOR((EXP), N, P00_SEQ, MOP, __VA_ARGS__),                            \
    P99_IF_EMPTY(DEF)(P00_INVALID_TYPE_IN_GENERIC(EXP, #__VA_ARGS__))(DEF) \
    P99_FOR(, N, P00_SER, P00_GENERIC_CLOSE, P99_DUPL(N, ))

#endif

#define P00_GENERIC0(MOP, EXP, DEF, ...)                       \
P00_GENERIC_                                                   \
(                                                              \
 P99_NARG(__VA_ARGS__),                                        \
 P00_ROBUST(MOP),                                              \
 P00_ROBUST(EXP),                                              \
 P00_ROBUST(DEF),                                              \
 __VA_ARGS__)

#define P00_GENERIC(N, ...) P99_IF_LT(N, 3)()(P00_GENERIC0(__VA_ARGS__))

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC, 2)
#define P99_GENERIC(...) P00_GENERIC(P99_NARG(__VA_ARGS__), P00_GENERIC_EXP, __VA_ARGS__)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_LIT, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_LIT, 2)
#define P99_GENERIC_LIT(...) P00_GENERIC(P99_NARG(__VA_ARGS__), P00_GENERIC_LIT, __VA_ARGS__)

#define P00_GENERIC_SIZE0(...) P00_GENERIC(P99_NARG(__VA_ARGS__), P00_GENERIC_SIZE, __VA_ARGS__)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_SIZE, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_SIZE, 2)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_SIZE, 3)
#define P99_GENERIC_SIZE(UI, ...) P00_GENERIC_SIZE0((char(*)[(size_t)(UI)]){ 0 }, __VA_ARGS__)

#define P00_GENERIC_SIZE_LIT0(...) P00_GENERIC(P99_NARG(__VA_ARGS__), P00_GENERIC_SIZE_LIT, __VA_ARGS__)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_SIZE_LIT, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_SIZE_LIT, 2)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GENERIC_SIZE_LIT, 3)
#define P99_GENERIC_SIZE_LIT(UI, ...) P00_GENERIC_SIZE_LIT0((char(*)[(size_t)(UI)]){ 0 }, __VA_ARGS__)


#if p99_has_extension(c_generic_selections)

# define P99_TYPED_TERN(COND, YES, NO)                         \
(P99_GENERIC                                                   \
 ((char(*)[1 + !!(COND)]){ 0 },                                \
  (NO),                                                        \
  (char(*)[2], (YES))))

#elif defined(__GNUC__)

# define P99_TYPED_TERN __builtin_choose_expr

#else

#define P99_TYPED_TERN(COND, YES, NO) only_implemented_with_C11_or_gcc


#endif


#define P00_TYPE_CHOICE(YES, T, I) (T, YES)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_CHOICE, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_CHOICE, 1)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TYPE_CHOICE, 2)
#define P99_TYPE_CHOICE(EXP, YES, NO, ...)                                   \
P99_GENERIC                                                                  \
((EXP),                                                                      \
 NO,                                                                         \
 P99_FOR(YES, P99_NARG(__VA_ARGS__), P00_SEQ, P00_TYPE_CHOICE, __VA_ARGS__))

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

#define P00_SIZE_CHOICE(YES, UI, I) (char(*)[(size_t)(UI)], YES)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_SIZE_CHOICE, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_SIZE_CHOICE, 1)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_SIZE_CHOICE, 2)
#define P99_SIZE_CHOICE(UI, YES, NO, ...)                                    \
P99_GENERIC                                                                  \
 ((char(*)[(size_t)(UI)])0,                                                  \
 NO,                                                                         \
 P99_FOR(YES, P99_NARG(__VA_ARGS__), P00_SEQ, P00_SIZE_CHOICE, __VA_ARGS__))


P00_DOCUMENT_PERMITTED_ARGUMENT(P99_SIZE_INDICATOR, 0)
#define P99_SIZE_INDICATOR(UI, ...) P99_SIZE_CHOICE(UI, 1, 0, __VA_ARGS__)


#define P00_DECLARE_INLINE_EXPRESSION1(EXT, BASE, EXP, A)      \
p99_inline                                                     \
P99_BUILTIN_TYPE(EXT) P99_PASTE3(p00_gen_, BASE, EXT)          \
(P99_BUILTIN_TYPE(EXT) A)                                      \
{                                                              \
  return (EXP);                                                \
}

#define P00_DECLARE_INLINE_EXPRESSION2(EXT, BASE, EXP, A, B)   \
p99_inline                                                     \
P99_BUILTIN_TYPE(EXT) P99_PASTE3(p00_gen_, BASE, EXT)          \
(P99_BUILTIN_TYPE(EXT) A, P99_BUILTIN_TYPE(EXT) B)             \
{                                                              \
  return (EXP);                                                \
}

#define P00_DECLARE_INLINE_EXPRESSION3(EXT, BASE, EXP, A, B, C) \
p99_inline                                                      \
P99_BUILTIN_TYPE(EXT) P99_PASTE3(p00_gen_, BASE, EXT)           \
(P99_BUILTIN_TYPE(EXT) A, P99_BUILTIN_TYPE(EXT) B,              \
 P99_BUILTIN_TYPE(EXT) C)                                       \
{                                                               \
  return (EXP);                                                 \
}

#define P00_DECLARE_INLINE_EXPRESSION4(EXT, BASE, EXP, A, B, C, D) \
p99_inline                                                         \
P99_BUILTIN_TYPE(EXT) P99_PASTE3(p00_gen_, BASE, EXT)              \
(P99_BUILTIN_TYPE(EXT) A, P99_BUILTIN_TYPE(EXT) B,                 \
 P99_BUILTIN_TYPE(EXT) C, P99_BUILTIN_TYPE(EXT) D)                 \
{                                                                  \
  return (EXP);                                                    \
}

#define P00_DECLARE_INLINE_EXPRESSION5(EXT, BASE, EXP, A, B, C, D, E) \
p99_inline                                                            \
P99_BUILTIN_TYPE(EXT) P99_PASTE3(p00_gen_, BASE, EXT)                 \
(P99_BUILTIN_TYPE(EXT) A, P99_BUILTIN_TYPE(EXT) B,                    \
 P99_BUILTIN_TYPE(EXT) C, P99_BUILTIN_TYPE(EXT) D,                    \
 P99_BUILTIN_TYPE(EXT) E)                                             \
{                                                                     \
  return (EXP);                                                       \
}

#define P00_DECLARE_INLINE_EXPRESSION6(EXT, BASE, EXP, A, B, C, D, E, F) \
p99_inline                                                               \
P99_BUILTIN_TYPE(EXT) P99_PASTE3(p00_gen_, BASE, EXT)                    \
(P99_BUILTIN_TYPE(EXT) A, P99_BUILTIN_TYPE(EXT) B,                       \
 P99_BUILTIN_TYPE(EXT) C, P99_BUILTIN_TYPE(EXT) D,                       \
 P99_BUILTIN_TYPE(EXT) E, P99_BUILTIN_TYPE(EXT) F)                       \
{                                                                        \
  return (EXP);                                                          \
}

P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_INLINE_EXPRESSION, 0)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_INLINE_EXPRESSION, 1)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_DECLARE_INLINE_EXPRESSION, 2)
#define P99_DECLARE_INLINE_EXPRESSION(...)                                                  \
P99_PASTE2(P00_DECLARE_INLINE_EXPRESSION, P99_MINUS(P99_NARG(__VA_ARGS__), 3))(__VA_ARGS__)

#define P00_DECLARE_INLINE_EXPRESSION_(...) P99_DECLARE_INLINE_EXPRESSION(__VA_ARGS__)
#define P00_DECLARE_INLINE_EXPRESSION(ARGS, EXT, I) P00_DECLARE_INLINE_EXPRESSION_(EXT, P00_ROBUST ARGS)

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_DECLARE_INLINE_EXPRESSIONS, 0)
#define P99_DECLARE_INLINE_EXPRESSIONS(NEPL, ...)                                         \
P99_FOR(NEPL, P99_NARG(__VA_ARGS__), P00_SER, P00_DECLARE_INLINE_EXPRESSION, __VA_ARGS__) \
P99_MACRO_END(P99_DECLARE_INLINE_EXPRESSIONS, __VA_ARGS__)

extern void p00_invalid_function(void*, ...);

#define P00_GEN_EXPR(BASE, EXT, I) (P99_BUILTIN_TYPE(EXT), P99_PASTE3(p00_gen_, BASE, EXT))

P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_GEN_EXPR, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_EXPR, 1)
#define P99_GEN_EXPR(BASE, EXPR, ...)                                                 \
P99_GENERIC(EXPR,                                                                     \
            p00_invalid_function,                                                     \
            P99_FOR(BASE, P99_NARG(__VA_ARGS__), P00_SEQ, P00_GEN_EXPR, __VA_ARGS__))




P99_DECLARE_INLINE_EXPRESSIONS((maximum,
                                (p00_a >= p00_b) ? p00_a : p00_b,
                                p00_a, p00_b),
                               P99_STD_REAL_EXTS
                              );

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_MAX, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_MAX, 1)
#define P99_GEN_MAX(A, B)                                      \
P99_GEN_EXPR(maximum, ((A) >= (B)) ? (A) : (B),                \
             P99_STD_REAL_EXTS                                 \
             )                                                 \
((A), (B))

P99_DECLARE_INLINE_EXPRESSIONS((minimum,
                                (p00_a <= p00_b) ? p00_a : p00_b,
                                p00_a, p00_b),
                               P99_STD_REAL_EXTS
                              );

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_MIN, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_MIN, 1)
#define P99_GEN_MIN(A, B)                                      \
P99_GEN_EXPR(minimum, ((A) <= (B)) ? (A) : (B),                \
             P99_STD_REAL_EXTS                                 \
             )                                                 \
((A), (B))


P99_DECLARE_INLINE_EXPRESSIONS((abs,
                                (p00_a >= 0) ? p00_a : -p00_a,
                                p00_a),
                               P99_STD_REAL_EXTS
                              );

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_ABS, 0)
#define P99_GEN_ABS(A) P99_GEN_EXPR(abs, ((A) >= 0) ? (A) : -(A), P99_STD_REAL_EXTS)(A)

#define p00_gen_sind sin
#define p00_gen_sinf sinf
#define p00_gen_sinld sinl
#define p00_gen_sindc csin
#define p00_gen_sinfc csinf
#define p00_gen_sinldc csinl

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_GEN_SIN, 0)
#define P99_GEN_SIN(A) P99_GEN_EXPR(sin, (A), P99_STD_FLOATING_EXTS)(A)

inline int* p00_generic_test(int * p00_a) {
  double *p00_x;
  switch (P99_GEN_ABS(*p00_a % 3)) {
  case 0:
    return P99_GENERIC(&*p00_a,
                       /* empty default expression */,
                       (int*, p00_a),
                       (double*, p00_x+1),
                       (float*, p00_x+2));
  case 1:
    return P99_GENERIC(&*p00_a,
                       /* another form of empty default */,
                       (double[7], P99_GEN_SIN(((p00_x+0)))),
                       (int*, p00_a),
                       (float*, p00_x+2));
  default:
    return P99_GENERIC(&*p00_a,
                       /* default expression: */ p00_x,
                       (double*, p00_x+0),
                       (float[7], p00_x+1),
                       (int*, p00_a));
  }
}

#else

/**
 ** @addtogroup generic Generic type expressions
 **
 ** C11 provides a new feature to write template-like expressions with
 ** the macro preprocessor, @c _Generic. Here we provide some tools
 ** that emulate this feature by means of gcc specific extensions.
 **
 ** @see P99_GENERIC
 **
 ** @{
 **/

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
 ** As you can see above, the default value can be omitted. If omitted, it
 ** is replaced with some appropriate expression that should usually
 ** give you a syntax error.
 **
 ** If there is a default expression, it is used when none of
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
 ** Here all of the expressions evaluate to a function specifier. If
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
 **   function. If @c a is @c unsigned and @c b is @c
 **   double the result is also @c double.
 **
 ** - the return type of the @c _Generic expression is a
 **   function to two arguments. If it is @c int, e.g, the
 **   type is <code>int ()(int, int)</code>. So the return type
 **   of the function call would be @c int in that case.
 **
 ** - the arguments are promoted and converted to the expected type of
 **   the choosen function
 **
 ** @remark If the compiler is C11 compliant, the ::P99_GENERIC
 ** expression will just be translated to the corresponding @c
 ** _Generic expression.
 **
 ** @remark Otherwise only gcc and compatible compilers are supported.
 **/
#define P99_GENERIC(...)

/**
 ** @brief For each generic choice return a compound literal of the chosen type
 **
 ** The resulting literal is default initialized with @c 0.
 **
 ** This is similar to ::P99_GENERIC except that the second entry in
 ** each pair should be a type.
 **
 ** @code
 ** P99_GENERIC_LIT(a + b,
 **                 uintmax_t,
 **                 (int, unsigned),
 **                 (long, unsigned long),
 **                 (float, double),
 **                 (double, double));
 ** @endcode
 **
 ** @see P99_GENERIC
 **/
#define P99_GENERIC_LIT(...)

/**
 ** @brief Similar to ::P99_GENERIC but the choice is not
 ** according to the type of the expression @a UI but to its unsigned
 ** value.
 **
 ** @param UI must be a compile time integer expression that is cast
 ** to @c size_t and that must not be of value @c 0.
 **
 ** @remark @a UI is only evaluated at compile time
 **
 ** @code
 ** a = P99_GENERIC_SIZE(sizeof(a),
 **                      func_fallback,
 **                      (1, func1),
 **                      (2, func2),
 **                      (4, func4),
 **                      (8, func8))
 **                ((void*)&a);
 ** @endcode
 **
 ** In this example one of the functions @c func1, ..., @c func8 is
 ** chosen according to the size of @c a. This function is then called
 ** with the address of @c a. In case the size of @c a is not
 ** appropriate, a function @c func_fallback is used.
 **
 ** @warning The range of allowed values for @a UI is platform
 ** dependent. The only known fixed guarantee is @c UINT16_MAX, but
 ** the result of a @c sizeof operator should always work.
 **
 ** @see P99_GENERIC
 **/
#define P99_GENERIC_SIZE(UI, ...)

/**
 ** @brief Similar to ::P99_GENERIC_SIZE but returns a compound
 ** literal of the chosen type
 **
 ** @see P99_GENERIC_SIZE
 ** @see P99_GENERIC_LIT
 ** @see P99_GENERIC
 **/
#define P99_GENERIC_SIZE_LIT(UI, ...)

/**
 ** @brief A compile time ternary operator that is analogous to
 ** <code>COND ? YES : NO</code> that keeps the type of the chosen
 ** expression.
 **
 ** When there is full @c _Generic support this is implemented using
 ** that feature. If not, the fallback from gcc for that purpose is
 ** used.
 **/
#define P99_TYPED_TERN(COND, YES, NO)

/**
 ** @addtogroup C11_types Generic identification of families of types or values
 **
 ** @{
 **/

#define P00_TYPE_CHOICE(YES, T, I) (T, YES)

/**
 ** @brief Classify expression @a EXP according to its type and return
 ** @a YES if the type is in the list and @a NO, otherwise.
 **
 ** @a EXP is evaluated only at compile time for its type. @a YES and @a
 ** NO are evaluated at most once.
 **/
#define P99_TYPE_CHOICE(EXP, YES, NO, ...)

/**
 ** @addtogroup type_generic Generic macros that classify expressions
 **
 ** @{
 **/

#define P99_TYPE_UNSIGNED(EXP)      P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_UNSIGNED_TYPES)
#define P99_TYPE_SIGNED(EXP)        P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_SIGNED_TYPES)
#define P99_TYPE_REAL_FLOATING(EXP) P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_REAL_FLOATING_TYPES)
# define P99_TYPE_COMPLEX(EXP)       P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_COMPLEX_TYPES)
#define P99_TYPE_FLOATING(EXP)      P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_FLOATING_TYPES)
#define P99_TYPE_BASIC(EXP)         P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_BASIC_TYPES)
#define P99_TYPE_CHARACTER(EXP)     P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_CHARACTER_TYPES)
#define P99_TYPE_INTEGER(EXP)       P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_INTEGER_TYPES)
#define P99_TYPE_REAL(EXP)          P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_REAL_TYPES)
#define P99_TYPE_ARITHMETIC(EXP)    P99_TYPE_CHOICE((EXP), 1, 0, P99_STD_ARITHMETIC_TYPES)

/**
 ** @}
 **/

/**
 ** @brief Classify expression @a UI according to its value and
 ** return @a YES if the value is in the list and @a NO, otherwise.
 **
 ** @a UI is evaluated only at compile time and the resulting value is cast
 ** to @c size_t. It must not be @c 0.
 **
 ** @a YES and @a NO are evaluated at most once.
 **
 ** @warning The range of allowed values for @a UI is platform
 ** dependent. The only known fixed guarantee is @c UINT16_MAX, but
 ** the result of a @c sizeof operator should always work.
 **/
#define P99_SIZE_CHOICE(UI, YES, NO, ...)

/**
 ** @brief Classify expression @a UI according to its value and
 ** return @c 1 if the value is in the list and @c 0, otherwise.
 **
 ** @a UI is evaluated only at compile time and the resulting value is cast
 ** to @c size_t. It must not be @c 0.
 **
 ** @code
 ** if (P99_SIZE_INDICATOR(sizeof(toto), 1, 2, 4, 8)) {
 **   .. do something easy ..
 ** } else {
 **   .. have some fallback for weirdos ...
 ** }
 ** @endcode
 **
 ** @warning The range of allowed values for @a UI is platform
 ** dependent. The only known fixed guarantee is @c UINT16_MAX, but
 ** the result of a @c sizeof operator should always work.
 **/
#define P99_SIZE_INDICATOR(UI, ...)

/**
 ** @}
 **/

/**
 ** @brief Declare an inline function of basename @a BASE for
 ** expression @a EXP, applied to the builtin type @a EXT.
 **
 ** The motivation for declaring such functions is for expressions that
 ** evaluate their arguments multiple times.
 **
 ** The argument list contains the names of the function parameters as they
 ** should appear inside @a EXP. The name of the declared function has
 ** @a EXT appended to @a BASE. E.g for a function operating on two @c
 ** unsigned values:
 **
 ** @code
 ** P99_DECLARE_INLINE_EXPRESSION(u, my_max, ((a >= b) ? a : b), a, b);
 ** @endcode
 **
 ** This declares an @c inline function with the following prototype:
 **
 ** @code
 ** unsigned p00_gen_my_maxu(unsigned, unsigned);
 ** @endcode
 **
 ** @remark For the moment, the number of arguments that can appear in
 ** the expression is limited to 6, but this could be augmented easily
 ** if there is need for it.
 **
 ** @see P99_BUILTIN_TYPE for the shorthands that can be used for the
 ** types
 **
 ** @see P99_DECLARE_INLINE_EXPRESSIONS to create this type of
 ** functions %for a whole list of %types
 **/
#define P99_DECLARE_INLINE_EXPRESSION(EXT, BASE, EXP, ...)

/**

 ** @brief Declare a whole bunch of @c inline functions of basename @c
 ** BASE for expression @c EXP, applied to the builtin types as given
 ** in the argument list.
 **
 ** The motivation for declaring such functions is for expressions that
 ** evaluate their arguments multiple times. You can use these
 ** functions in a generic expression such as ::P99_GEN_EXPR.
 **
 ** @a NEPL represents the argument list that is passed to
 ** ::P99_DECLARE_INLINE_EXPRESSION for defining the individual
 ** functions.
 **
 ** @code
 ** P99_DECLARE_INLINE_EXPRESSIONS((my_max, ((a >= b) ? a : b), a, b),
 **                                u, ul, i, c, uhh);
 ** @endcode
 **
 ** This declares @c inline functions with the following prototypes:
 **
 ** @code
 ** unsigned p00_gen_my_maxu(unsigned, unsigned);
 ** unsigned long p00_gen_my_maxul(unsigned long, unsigned long);
 ** signed p00_gen_my_maxi(signed, signed);
 ** char p00_gen_my_maxc(char, char);
 ** unsigned char p00_gen_my_maxuhh(unsigned char, unsigned char);
 ** @endcode
 **
 ** @see P99_BUILTIN_TYPE
 ** @see P99_GENERIC
 ** @see P99_GEN_EXPR
 ** @see P99_DECLARE_INLINE_EXPRESSION
 **/
#define P99_DECLARE_INLINE_EXPRESSIONS(NEPL, ...)

extern void p00_invalid_function(void*, ...);

/**
 ** @brief Produce a type generic expression that can be used as if it
 ** were an @c inline function.
 **
 ** The motivation for declaring a macro that uses this is for
 ** expressions that evaluate their arguments multiple times.
 **
 ** E.g to define a macro ::P99_GEN_MAX you can use the following
 **
 ** @code
 ** P99_DECLARE_INLINE_EXPRESSIONS((maximum,
 **                                 (p00_a >= p00_b) ? p00_a : p00_b,
 **                                 p00_a, p00_b),
 **                                b, c, hh, uhh, h, uh, i, u, l, ul, ll, ull,
 **                                d, f, ld
 **                                );
 **
 ** #define P99_GEN_MAX(A, B)                                  \
 ** P99_GEN_EXPR(maximum, ((A) >= (B)) ? (A) : (B),            \
 **              b, c, hh, uhh, h, uh, i, u, l, ul, ll, ull,   \
 **              d, f, ld                                      \
 **              )                                             \
 ** ((A), (B))
 ** @endcode
 **
 ** This first defines 15 inline functions for the different
 ** arithmetic types, you could also just use
 ** ::P99_STD_ARITHMETIC_EXTS to produce that long list. Then the
 ** definition of the macro expands to a type generic expression that
 ** has as @a EXPR as its selection expression (here <code>(A) >= (B)) ?
 ** (A) : (B)</code>).
 **
 ** @remark the sole effect of using exactly the target expression for @a EXPR
 ** is that the type of the selection expression undergoes
 ** exactly the promotion or conversion rules that would be applied to
 ** @a EXPR if it were to be evaluated directly.
 **
 ** So, staying with the example above, we would have
 **
 ** @code
 ** int f(toto t);
 ** double d;
 **
 ** double dd = P99_GEN_MAX(f(t), d);
 ** @endcode
 **
 ** the commonly used type of @c f and @c d for the evaluation of
 ** <code>P99_GEN_MAX(f(t), d)</code> would be @c double. Before
 ** computing the maximum value, the result of @c f(t) is converted to
 ** @c double and then the result of the operation is also @c double.
 **
 ** As you can already see from this simple example, for such an
 ** expression it is crucial that @c f(t) is only evaluated once,
 ** which is the principal goal of ::P99_GEN_EXPR.
 **
 ** Here is another example that shows how simple it is to produce the
 ** type generic math macros that should normally be provided by
 ** "tgmath.h".
 **
 ** @code
 ** #define p00_gen_sind sin
 ** #define p00_gen_sinf sinf
 ** #define p00_gen_sinld sinl
 ** #define p00_gen_sindc csin
 ** #define p00_gen_sinfc csinf
 ** #define p00_gen_sinldc csinl
 **
 ** #define P99_GEN_SIN(A) P99_GEN_EXPR(sin, (A), P99_STD_FLOATING_EXTS)(A)
 ** @endcode
 **/
#define P99_GEN_EXPR(BASE, EXPR, ...)

#define P99_GEN_MAX(A, B)                                      \
P99_GEN_EXPR(maximum, ((A) >= (B)) ? (A) : (B),                \
             P99_STD_REAL_EXTS                                 \
             )                                                 \
((A), (B))

#define P99_GEN_MIN(A, B)                                      \
P99_GEN_EXPR(minimum, ((A) <= (B)) ? (A) : (B),                \
             P99_STD_REAL_EXTS                                 \
             )                                                 \
((A), (B))


#define P99_GEN_ABS(A) P99_GEN_EXPR(abs, ((A) >= 0) ? (A) : -(A), P99_STD_REAL_EXTS)(A)

/**
 ** @brief Type generic macro to compute the sine of @a A.
 **
 ** This is just a little example showing how simply the type generic macros
 ** of "tgmath.h" can be implemented.
 **/
#define P99_GEN_SIN(A) P99_GEN_EXPR(sin, (A), P99_STD_FLOATING_EXTS)(A)

/**
 ** @}
 **/

#endif

#define P00_OVALUES_(X, T, I) (T*, X[0])
#define P00_OVALUES(X, ...) P99_FOR(X, P99_NARG(__VA_ARGS__), P00_SEQ, P00_OVALUES_, __VA_ARGS__)
#define P00_OVALUE_(X, DEF, ...) P99_GENERIC(X, DEF, __VA_ARGS__)

#define P00_OVALUES1_(X, T, I) (T*, X)
#define P00_OVALUES1(X, ...) P99_FOR(X, P99_NARG(__VA_ARGS__), P00_SEQ, P00_OVALUES1_, __VA_ARGS__)
#define P00_OVALUE1_(X, ...) P99_GENERIC(&(X[0]), , P00_OVALUES1(X, __VA_ARGS__))

#define P00_AVALUES_(X, T, I) (T*, *(T(*)[1])X)
#define P00_AVALUES(X, ...) P99_FOR(X, P99_NARG(__VA_ARGS__), P00_SEQ, P00_AVALUES_, __VA_ARGS__)
#define P00_AVALUE_(X, DEF, ...) P99_GENERIC(X, DEF, __VA_ARGS__)


#ifdef P00_DOXYGEN
/**
 ** @brief For a pointer or array expression @a X return the
 ** underlying object.
 **
 ** @pre This requires that @a X is either a pointer or array, or in
 ** other words that <code>(X)[0]</code> is a valid expression.
 **
 ** @pre This requires that the base type of @a X is one of the types
 ** listed in the arguments.
 **
 ** @return If @a X is a pointer to of one of the types listed in the
 ** argument list, <code>X[0]</code> is returned, that is it
 ** is assumed that the pointer points to one element of the
 ** underlying base type. Otherwise @a X is an lvalue of array type
 ** and this object is returned.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_OVALUE, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OVALUE, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OVALUE, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OVALUE, 3)
#define P99_OVALUE(X, ...)

/**
 ** @brief For a pointer or array expression @a X return the
 ** underlying array.
 **
 ** @pre This requires that @a X is either a pointer or array, or in
 ** other words that <code>(X)[0]</code> is a valid expression.
 **
 ** @pre This requires that the base type of @a X is one of the types
 ** listed in the arguments.
 **
 ** @return If @a X is a pointer to of one of the types listed in the
 ** argument list, @c X[0] is reinterpreted as a one element array and
 ** that array is returned. Otherwise @a X is an lvalue of array type
 ** and this object is returned.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_AVALUE, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AVALUE, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AVALUE, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AVALUE, 3)
#define P99_AVALUE(X, ...)

/**
 ** @brief For a pointer or array expression @a X return the size of
 ** the underlying object.
 **
 ** @pre This requires that @a X is either a pointer or array, or in
 ** other words that <code>(X)[0]</code> is a valid expression.
 **
 ** @pre This requires that the base type of @a X is one of the types
 ** listed in the arguments.
 **
 ** @return If @a X is a pointer to of one of the types listed in the
 ** argument list, <code>sizeof (X)[0]</code> is returned, that is it
 ** is assumed that the pointer points to one element of the
 ** underlying base type. Otherwise @a X is an lvalue of array type
 ** and this corresponds to the total size of the array object.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_OBJSIZE, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJSIZE, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJSIZE, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJSIZE, 3)
#define P99_OBJSIZE(X, ...)

/**
 ** @brief For a pointer or array expression @a X return the length of
 ** the underlying array object.
 **
 ** @pre This requires that @a X is either a pointer or array, or in
 ** other words that <code>(X)[0]</code> is a valid expression.
 **
 ** @pre This requires that the base type of @a X is one of the types
 ** listed in the arguments.
 **
 ** @return If @a X is a pointer to of one of the types listed in the
 ** argument list, <code>1</code> is returned, that is it is assumed
 ** that the pointer points to one element of the underlying base
 ** type. Otherwise @a X is an lvalue of array type and this
 ** corresponds to the total number of elements of the array object.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_OBJLEN, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJLEN, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJLEN, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJLEN, 3)
#define P99_OBJLEN(X, ...)
#else
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_OVALUE, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OVALUE, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OVALUE, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OVALUE, 3)
#define P99_OVALUE(X, ...) P00_OVALUE_((X), P00_OVALUE1_((X), __VA_ARGS__), P00_OVALUES((X), __VA_ARGS__))

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_AVALUE, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AVALUE, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AVALUE, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AVALUE, 3)
#define P99_AVALUE(X, ...) P00_AVALUE_((X), P00_OVALUE1_((X), __VA_ARGS__), P00_AVALUES((X), __VA_ARGS__))

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_OBJSIZE, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJSIZE, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJSIZE, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJSIZE, 3)
#define P99_OBJSIZE(X, ...) (sizeof P99_OVALUE(X, __VA_ARGS__))

P00_DOCUMENT_PERMITTED_ARGUMENT(P99_OBJLEN, 0)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJLEN, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJLEN, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_OBJLEN, 3)
#define P99_OBJLEN(X, ...) (P99_OBJSIZE(X, __VA_ARGS__)/(sizeof (X)[0]))
#endif


#endif
