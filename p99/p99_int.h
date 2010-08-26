/*
** p99_int.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Jul 26 09:03:21 2010 Jens Gustedt
** Last update Mon Jul 26 09:03:21 2010 Jens Gustedt
*/

#ifndef   	P99_INT_H_
# define   	P99_INT_H_

/**
 ** @file
 ** @brief Macros handling integer types.
 **/

#include "p99_c99.h"
#include "p99_if.h"

/**
 ** @brief Apply the type macro @a MACRO to an unsigned type that is
 ** compatible with type @a T.
 **
 ** The returning expression is of type @c uintmax_t
 **/
#define P99_TO_UNSIGNED(T, MACRO)               \
((uintmax_t)                                    \
 (sizeof(T) < sizeof(signed)                    \
  ? (sizeof(T) == 1u                            \
     ? MACRO(unsigned char)                     \
     : MACRO(unsigned short))                   \
  : (sizeof(T) < sizeof(unsigned long)          \
     ? MACRO(unsigned)                          \
     : (sizeof(T) < sizeof(unsigned long long)  \
        ? MACRO(unsigned long)                  \
        : MACRO(unsigned long long)))))

/**
 ** @brief Convert -1 to type @a T
 **
 ** If @a T is a signed type a representation of the value -1 should
 ** always exist, since the value 1 also exists.
 **
 ** If @a T is an unsigned type, the converted value is guaranteed to
 ** hold the maximum value that is representable in this type. This is
 ** guaranteed to work regardless of the platform's representation of
 ** negative values: integer conversion @b to an unsigned type is
 ** always defined to be modulo 2^N where N is the precision of the
 ** target type.
 **/
#define P99_M1(T) ((T)-1)

/**
 ** @brief Return an unsigned version of P99_M1.
 **
 ** The returning expression is of type @c uintmax_t
 **/
#define P99_M1U(T) P99_TO_UNSIGNED(T, P99_M1)

#define P99__DOCUMENT_SIGNED(X) /*! @brief Cast the @c int value @c X to type @a T */
#define P99__DOCUMENT_UNSIGNED(X)                                       \
/*! @brief Cast the @c int value @c X to the unsigned type corresponding to @a T */

P99__DOCUMENT_SIGNED(0)
#define P99_0(T) ((T)0)

P99__DOCUMENT_UNSIGNED(0)
#define P99_0U(T) P99_TO_UNSIGNED(T, P99_0)

P99__DOCUMENT_SIGNED(1)
#define P99_1(T) ((T)1)

P99__DOCUMENT_UNSIGNED(1)
#define P99_1U(T)  P99_TO_UNSIGNED(T, P99_1)

P99__DOCUMENT_SIGNED(2)
#define P99_2(T) ((T)2)

P99__DOCUMENT_UNSIGNED(2)
#define P99_2U(T)  P99_TO_UNSIGNED(T, P99_2)

P99__DOCUMENT_SIGNED(3)
#define P99_3(T) ((T)3)

P99__DOCUMENT_UNSIGNED(3)
#define P99_3U(T)  P99_TO_UNSIGNED(T, P99_3)

/**
 ** @brief The maximum representable value of the unsigned type
 ** corresponding to @a T
 **
 ** The returning expression is of type @c uintmax_t
 **/
#define P99_UT_MAX(T) (P99_M1U(T))

/**
 ** @brief Half of the maximum representable value of the unsigned type
 ** corresponding to @a T
 **
 ** This is the maximum representable value of the signed type
 ** corresponding to @a T.
 **
 ** The returning expression is of type @c uintmax_t.
 **
 ** @see P99_TMAX
 **/
#define P99_UT_MAX1(T) (P99_UT_MAX(T)/2u)
#define P99_UT_HIGH(T) (P99_UT_MAX1(T) + P99_1U(T))

/**
 ** @brief The negative of the half of the maximum representable value
 ** of the unsigned type corresponding to @a T
 **
 ** This is generally not the minimum representable value of the signed type
 ** corresponding to @a T, it might deviate by one from that value.
 **
 ** The returning expression is of type @a T.
 **
 ** @see P99_TMIN
 **/
#define P99_ST_MIN1(T) (-(T)P99_UT_MAX1(T))

/**
 ** @brief Determine if @a T is an unsigned or signed integral type
 **
 ** This works as follows:
 ** - If @a T is signed then -1 in that type is always less than 0 in
 **    that type
 ** - If @a T is unsigned then conversion of -1 to that type gives the
 **    maximally encodable value for that type. This is always greater
 **    than 0 in that type,
 ** - If @a T is @c _Bool or equivalent, -1 converted to it results in
 **    1 and 0 is also mapped to 0. Thus it is detected as unsigned.
 **
 ** @see P99_SIGNED for a similar macro that takes an expression as an
 ** argument
 **/
#define P99_ISSIGNED(T) (P99_M1(T) < P99_0(T))

/**
 ** @brief Compute expression @a A and apply integer promotion rules
 ** under the constraint of expression @a B.
 **
 ** Expression @a A is evaluated exactly once. Expression @a B is only
 ** used for its type and never evaluated.
 **
 ** The result can always be determined at compile time, if @a A can
 ** be determined then, regardless of the actual value of @a B.  But
 ** it is only a constant expression in the strict sense of C99 if
 ** both @a A and @a B are so.
 **
 ** @warning Due to the integer promotion rules the resulting
 ** expression has a width that is at least that of @c int, even if @a
 ** A and @a B have smaller width than that.
 **/
#define P99_SIGN_PROMOTE(A, B) (1 ? (A) : (B))

/**
 ** @brief Determine if @a EXPR has an unsigned or signed integral type
 **
 ** Note that this macro does not evaluate @a EXPR but uses it only to
 ** determine its signedness. There are no side effects.
 **
 ** The result can always be determined at compile time, but it is not
 ** a constant expression in the strict sense of C99 if @a EXPR is not
 ** so.
 **
 ** Beware, the result of this expression is not the sign of @a EXPR,
 ** but whether or not the type of it could hold a signed value.
 **
 ** This works as follows:
 ** - The condition of the conditional subexpression is always @c 1. So
 **    that expression always evaluates to @c (intmax_t)-1 and @a
 **    (EXPR) is never evaluated.
 ** - The type of the conditional expression is signed if @a EXPR
 **    has a designated type that is signed, or if @a EXPR is an
 **    integer constant that fits into @c intmax_t.
 ** - The type of the conditional expression is unsigned if @a EXPR
 **    has a designated type that is unsigned, or if @a EXPR is an
 **    integer constant that does not fit into @c intmax_t but into @c
 **    uintmax_t.
 ** - The width of the conditional expression is the maximal width of
 **    an integer on the platform, namely that of @c intmax_t and @c
 **    uintmax_t.
 ** - The result of that conditional expression, i.e either @c
 **     (intmax_t)-1 or @c (uintmax_t)-1, is now compared against @c
 **     (intmax_t)0.
 **
 ** @see P99_ISSIGNED for a similar macro that takes a type as an argument
 **/
#define P99_SIGNED(EXPR) (P99_SIGN_PROMOTE(-INTMAX_C(1), EXPR) < INTMAX_C(0))


inline
intmax_t p99__abs_signed(intmax_t a) {
  return (a < INTMAX_C(0)) ? -a : a;
}

/**
 ** @brief Compute the absolute value of an integral expression.
 **
 ** @return The return type of this macro is @c intmax_t if @a EXPR
 **    has a designated type that is signed, or if @a EXPR is an
 **    integer constant that fits into @c intmax_t. Otherwise it is @c
 **    uintmax_t.
 **
 ** @a EXPR is guaranteed to be evaluated exactly once.
 **/
#define P99_ABS(EXPR) (P99_SIGNED(EXPR) ? p99__abs_signed(EXPR) : (EXPR))

/**
 ** @brief C99 allows for exactly three different possibilities to
 ** encode negative values of integer types.
 **
 ** @see ::P99_SIGNED_REPRESENTATION
 **/
typedef enum {
  p99_signed_representation_invalid = 0,
  p99_signed_representation_magnitude = 1,
  p99_signed_representation_ones = 2,
  p99_signed_representation_twos = 3,
} p99_signed_representation;

/**
 ** @brief C99 allows for exactly three different possibilities to
 ** encode negative values of integer types.
 **
 ** The representation can be found by looking at the two least
 ** significant bits of -1 represented in @a T.
 ** @see ::p99_signed_representation
 **/
#define P99_SIGNED_REPRESENTATION(T) ((p99_signed_representation)(P99_M1(T) & P99_3(T)))


/**
 ** @brief If the sign representation of a type is two's complement
 ** the type has no @em negative zero and can thus represent one more
 ** value.
 **/
#define P99_2COMPLEMENT(T) ((T)(P99_SIGNED_REPRESENTATION(T) == p99_signed_representation_twos))

/**
 ** @brief Give the maximum representable value of type @a T
 **/
#define P99_TMAX(T) ((T)(P99_ISSIGNED(T) ? P99_UT_MAX1(T) : P99_UT_MAX(T)))

/**
 ** @brief Give the minimum representable value of type @a T
 **/
#define P99_TMIN(T) ((T)(P99_ISSIGNED(T) ? (P99_ST_MIN1(T) - P99_2COMPLEMENT(T)) : P99_0(T)))

/**
 ** @brief The precision, i.e the number of significant bits of integral type
 ** @a T.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning this is not necessarily the width of @a T
 ** @see P99_TWIDTH
 ** @see P99_TPADDING
 **/
#define P99_TPREC(T) (P99_HIGH2_1(P99_TMAX(T)))

/**
 ** @brief The width of integral type @a T.
 **
 ** This is the precision plus eventually a sign bit, if @a T is signed.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning These are not necessarily all bits that are @em used by
 ** the type, there might be padding bits.
 ** @see P99_TPREC
 ** @see P99_TPADDING
 **/
#define P99_TWIDTH(T) (P99_TPREC(T) + P99_ISSIGNED(T))

/**
 ** @brief The padding bits of integral type @a T.
 **
 ** These are the bits that are not used for the numerical
 ** representation of the values. On most architectures and for most
 ** types this will be 0. But for e.g for @c _Bool this will be at
 ** least 7.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @see P99_TWIDTH
 ** @see P99_TPREC
 **/
#define P99_TPADDING(T) ((sizeof(T)*CHAR_BIT) - P99_TWIDTH(T))

#define P99__LVAL(T, ...) ((T)__VA_ARGS__)

/**
 ** @brief Define an lvalue of type @c T, where @c T is the first
 ** parameter in the variable parameter list.
 **
 ** If only @c T is given as a parameter, @c { 0 } is used to
 ** initialize the lvalue. Otherwise the remaining parameters are used
 ** for initialization.
 **/
#define P99_LVAL(...) P99_IF_DEC_LE(P99_NARG(__VA_ARGS__),1)(P99__LVAL(__VA_ARGS__, { 0 }))(P99__LVAL(__VA_ARGS__))


#define P99_CHOOSE5(xT, cc, cs, ci, cl, cll)    \
((sizeof(xT) < sizeof(int))                     \
 ? ((sizeof(xT) < sizeof(short))                \
    ? cc                                        \
    : cs)                                       \
 : ((sizeof(xT) <= sizeof(long))                \
    ? ((sizeof(xT) == sizeof(int))              \
       ? ci                                     \
       : cl)                                    \
    : cll))

#define P99_PRI(xT, F, ...)                     \
P99_CHOOSE5(xT,                                 \
        "%" #__VA_ARGS__ "hh" #F,               \
        "%" #__VA_ARGS__ "h" #F,                \
        "%" #__VA_ARGS__ "" #F,                 \
        "%" #__VA_ARGS__ "l" #F,                \
        "%" #__VA_ARGS__ "ll" #F)

/**
 ** @brief Promote integer expression @a x to the width of @c
 ** uintmax_t but keep signedness if possible.
 **/
#define P99__J(x) (0 ? P99_0(uintmax_t) : (x))

#endif 	    /* !P99_INT_H_ */
