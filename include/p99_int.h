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
 ** @brief Determine if @a T is an unsigned or signed type
 **
 ** This works as follows:
 ** - If @a T is signed then -1 in that type is always less than 0 in
 **    that type
 ** - If @a T is unsigned then conversion of -1 to that type gives the
 **    maximally encodable value for that type. This is always greater
 **    than 0 in that type,
 ** - If @a T is @c _Bool or equivalent, -1 converted to it results in
 **    1 and 0 is also mapped to 0. Thus it is detected as unsigned.
 **/
#define P99_ISSIGNED(T) (P99_M1(T) < P99_0(T))

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

#endif 	    /* !P99_INT_H_ */
