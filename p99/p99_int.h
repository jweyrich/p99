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
 ** @addtogroup integers Macros to handle integer type expressions
 **
 ** The macros here help to integers more easily, e.g to detect if a
 ** type or expression is signed, what the maximum and minimum values
 ** are etc.
 **
 ** @{
 */

#define P99__DOCUMENT_C2(WIDTH) /*! @brief Combine two integer expressions of half the width into one of width WIDTH*/

#ifdef UINT16_MAX
P99__DOCUMENT_C2(16)
#define P99X_UINT16_C2(H, L) ((((uint16_t)(uint8_t)H) << 8u) | (uint16_t)(uint8_t)L)
#endif
#ifdef INT16_MAX
P99__DOCUMENT_C2(16)
#define P99X_INT16_C2(H, L) ((((int16_t)(int8_t)H) << 8u) | (int16_t)(int8_t)L)
#endif
#ifdef UINT32_MAX
P99__DOCUMENT_C2(32)
#define P99X_UINT32_C2(H, L) ((((uint32_t)(uint16_t)H) << 16u) | (uint32_t)(uint16_t)L)
#endif
#ifdef INT32_MAX
P99__DOCUMENT_C2(32)
#define P99X_INT32_C2(H, L) ((((int32_t)(int16_t)H) << 16u) | (int32_t)(int16_t)L)
#endif
#ifdef UINT64_MAX
P99__DOCUMENT_C2(64)
#define P99X_UINT64_C2(H, L) ((((uint64_t)(uint32_t)H) << 32u) | (uint64_t)(uint32_t)L)
#endif
#ifdef INT64_MAX
P99__DOCUMENT_C2(64)
#define P99X_INT64_C2(H, L) ((((int64_t)(int32_t)H) << 32u) | (int64_t)(int32_t)L)
#endif

#ifdef p99x_uint128
P99__DOCUMENT_C2(128)
#define P99X_UINT128_C2(H, L) ((((p99x_uint128)H) << 64u) | (p99x_uint128)L)
#define P99X_UINT128_0 P99X_UINT128_C2(0x0000000000000000, 0x0000000000000000)
#define P99X_UINT128_MAX P99X_UINT128_C2(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF)
#endif

#ifdef p99x_int128
P99__DOCUMENT_C2(128)
#define P99X_INT128_C2(H, L) ((((p99x_int128)H) << 64u) | (p99x_int128)L)
#define P99X_INT128_MAX P99X_INT128_C2(0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF)
#define P99X_INT128_MIN P99X_INT128_C2(0x8000000000000000, 0x0000000000000000)
#endif

#ifdef DOXYGEN
/**
 ** @brief An unsigned integer type of maximal width that extends the
 ** C99 specifications.
 **
 ** Typically this might be of width 128 where the whole tool chain,
 ** in particular the preprocessor only supports 64.
 **
 ** This @c typedef does not necessary exist. It can be checked for with a
 ** macro of the same name.
 ** @see p99x_uint128
 **/
typedef uintmax_t p99x_uintmax;

/**
 ** @brief A signed integer type of maximal width that extends the
 ** C99 specifications.
 **
 ** Typically this might be of width 128 where the whole tool chain,
 ** in particular the preprocessor only supports 64.
 **
 ** This @c typedef does not necessary exist. It can be checked for with a
 ** macro of the same name.
 ** @see p99x_int128
 **/
typedef intmax_t p99x_intmax;

/**
 ** @brief An unsigned integer type of width 128 that extends the
 ** C99 specifications.
 **
 ** Typically this might be defined when the whole tool chain,
 ** in particular the preprocessor only supports 64 bits.
 **
 ** This @c typedef does not necessary exist. It can be checked for with a
 ** macro of the same name.
 ** @see p99x_uintmax
 **/
typedef uintmax_t p99x_uint128;

/**
 ** @brief A signed integer type of width 128 that extends the
 ** C99 specifications.
 **
 ** Typically this might be defined when the whole tool chain,
 ** in particular the preprocessor only supports 64 bits.
 **
 ** This @c typedef does not necessary exist. It can be checked for with a
 ** macro of the same name.
 ** @see p99x_intmax
 **/
typedef intmax_t p99x_intmax;
#endif

#ifdef p99x_uintmax

#define P99X__SIGN_PROMOTE(EXPR)                                        \
((p99x_uintmax)P99_SIGN_PROMOTE(P99_UE_MAX(EXPR), (p99x_uintmax)UINTMAX_MAX))

#define P99X__SHIFT(EXPR)                                               \
((P99_SIGN_PROMOTE(P99_UE_MAX(EXPR), UINTMAX_MAX) > (p99x_uintmax)UINTMAX_MAX) \
 ? 64u                                                                  \
 : 0u)
#endif

#ifndef P99_HIGH2
# if P99_UINTMAX_WIDTH == 64
#  define P99_HIGH2(X)                          \
((((X) & P99__B0) ? P99__S0 : 0u)               \
 | (((X) & P99__B1) ? P99__S1 : 0u)             \
 | (((X) & P99__B2) ? P99__S2 : 0u)             \
 | (((X) & P99__B3) ? P99__S3 : 0u)             \
 | (((X) & P99__B4) ? P99__S4 : 0u)             \
 | (((X) & P99__B5) ? P99__S5 : 0u))
# endif
#endif
#ifndef P99_HIGH2
# if P99_UINTMAX_WIDTH <= 128
#  define P99_HIGH2(X)                          \
((((X) & P99__B0) ? P99__S0 : 0u)               \
 | (((X) & P99__B1) ? P99__S1 : 0u)             \
 | (((X) & P99__B2) ? P99__S2 : 0u)             \
 | (((X) & P99__B3) ? P99__S3 : 0u)             \
 | (((X) & P99__B4) ? P99__S4 : 0u)             \
 | (((X) & P99__B5) ? P99__S5 : 0u)             \
 | (((X) & P99__B6) ? P99__S6 : 0u))
# endif
#endif

#ifndef P99X__SHIFT
#define P99X__SHIFT(EXPR) 0
#endif

#ifndef P99X__SIGN_PROMOTE
#define P99X__SIGN_PROMOTE(EXPR) 0
#endif

#define P99_HIGH2_1(X) ((X) == P99_UINTMAX_MAX ? P99_UINTMAX_WIDTH : (P99_HIGH2((X) + UINTMAX_C(1))))

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
#define P99__DOCUMENT_UNSIGNED(X) /*! @brief Cast the @c int value @c X to the unsigned type corresponding to @a T */

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
#define P99__ST_MIN1(T) (-(T)P99_UT_MAX1(T))

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


#define P99__SEE_PROMOTE /*! @see P99_SIGN_PROMOTE for rules about integer sign promotion */

#define P99__DOCUMENT_PROMOTE(X) /*! @brief Promote the integer value @c X under the constraint of expression @a EXPR */

P99__DOCUMENT_PROMOTE(0)
P99__SEE_PROMOTE
#define P99_PROMOTE_0(EXPR) P99_SIGN_PROMOTE(0, (EXPR))

P99__DOCUMENT_PROMOTE(1)
P99__SEE_PROMOTE
#define P99_PROMOTE_1(EXPR) P99_SIGN_PROMOTE(1, (EXPR))

P99__DOCUMENT_PROMOTE(2)
P99__SEE_PROMOTE
#define P99_PROMOTE_2(EXPR) P99_SIGN_PROMOTE(2, (EXPR))

P99__DOCUMENT_PROMOTE(3)
P99__SEE_PROMOTE
#define P99_PROMOTE_3(EXPR) P99_SIGN_PROMOTE(3, (EXPR))

P99__DOCUMENT_PROMOTE(0U)
P99__SEE_PROMOTE
#define P99_PROMOTE_0U(EXPR) P99_SIGN_PROMOTE(0U, (EXPR))

P99__DOCUMENT_PROMOTE(1u)
P99__SEE_PROMOTE
#define P99_PROMOTE_1U(EXPR) P99_SIGN_PROMOTE(1U, (EXPR))

P99__DOCUMENT_PROMOTE(2u)
P99__SEE_PROMOTE
#define P99_PROMOTE_2U(EXPR) P99_SIGN_PROMOTE(2U, (EXPR))


P99__DOCUMENT_PROMOTE(-1)
P99__SEE_PROMOTE
#define P99_PROMOTE_M1(EXPR) P99_SIGN_PROMOTE(-1, (EXPR))

/**
 ** @brief Promote the integer value @c -1 under the constraint of the
 ** unsigned promoted expression @a EXPR
 ** */
P99__SEE_PROMOTE
#define P99_PROMOTE_M1U(EXPR) P99_SIGN_PROMOTE(P99_PROMOTE_M1(EXPR), P99_PROMOTE_0U(EXPR))

/**
 ** @brief The maximum representable value of the unsigned type
 ** promoted with respect to expression @a EXPR.
 **
 ** The returning expression is of unsigned integer type.
 **/
P99__SEE_PROMOTE
#define P99_UE_MAX(EXPR) (P99_PROMOTE_M1U(EXPR))

/**
 ** @brief Half of the maximum representable value of the unsigned type
 ** promoted with respect to expression @a EXPR
 **
 ** This is the maximum representable value of the signed type
 ** promoted with respect to @a EXPR
 **
 ** The returning expression is of unsigned integer type.
 **
 ** @see P99_TMAX
 **/
P99__SEE_PROMOTE
#define P99_UE_MAX1(EXPR) (P99_UE_MAX(EXPR)/P99_PROMOTE_2U(EXPR))


/**
 ** @brief The width of the integral type of expression @a EXPR.
 **
 ** This is the precision plus eventually a sign bit, if the type is
 ** signed.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning These are not necessarily all bits that are @em used by
 ** the type, there might be padding bits.
 ** @see P99_TWIDTH
 ** @see P99_EPREC
 ** @see P99_EPADDING
 **/
P99__SEE_PROMOTE
#define P99_EWIDTH(EXPR)                                                \
  (P99X__SHIFT(EXPR)                                                    \
 ? (P99_HIGH2_1((uintmax_t)(P99_UE_MAX(EXPR)>>P99X__SHIFT(EXPR))) + P99X__SHIFT(EXPR)) \
 : P99_HIGH2_1(P99_UE_MAX(EXPR))                                        \
 )

/**
 ** @brief The precision, i.e the number of significant bits the
 ** integral type of expression @a EXPR.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning this is not necessarily the width of @a T
 ** @see P99_TPREC
 ** @see P99_EWIDTH
 ** @see P99_EPADDING
 **/
P99__SEE_PROMOTE
#define P99_EPREC(EXPR) (P99_EWIDTH(EXPR) - P99_SIGNED(EXPR))

/**
 ** @brief The padding bits of the integral type of expression @a EXPR.
 **
 ** These are the bits that are not used for the numerical
 ** representation of the values. On most architectures and for most
 ** types this will be 0. But for e.g for @c _Bool this will be at
 ** least 7.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @see P99_TPADDING
 ** @see P99_EWIDTH
 ** @see P99_EPREC
 **/
P99__SEE_PROMOTE
#define P99_EPADDING(EXPR) (sizeof(P99_PROMOTE_0(EXPR))*CHAR_BIT - P99_EWIDTH(EXPR))

#define P99_SE_MAX(EXPR)                        \
((((P99_PROMOTE_1(EXPR)                         \
    << (P99_EWIDTH(EXPR) - 2U))                 \
   - P99_PROMOTE_1(EXPR))                       \
  << 1U)                                        \
 + P99_PROMOTE_1(EXPR))


/**
 ** @brief The negative of the half of the maximum representable value
 ** of the promoted integral type of expression @a EXPR
 **
 ** This is generally not the minimum representable value of the signed type
 ** corresponding to @a EXPR, it might deviate by one from that value.
 **
 ** The returning expression is of type @a T.
 **
 ** @see P99_TMIN
 **/
#define P99__SE_MIN1(EXPR) (-P99_SE_MAX(EXPR))

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
 ** @see P99_ISSIGNED for a similar macro that takes a type as an argument
 **/
P99__SEE_PROMOTE
#define P99_SIGNED(EXPR) (P99_PROMOTE_M1(EXPR) < P99_PROMOTE_0(EXPR))


inline
uintmax_t p99__abs_signed(intmax_t a) {
  uintmax_t aa = a;
  /* The minus is taken on the unsigned value, so it gives the
     correct result with -INTMAX_MAX, namely INTMAX_MAX, which might
     not be representable for two's complement representation. */
  return (a < INTMAX_C(0)) ? -aa : a;
}

/**
 ** @brief Compute the absolute value of an integral expression.
 **
 ** @return The return type of this macro is @c uintmax_t. It must be
 ** so, since if we have a two's complement representation the value
 ** @c -INTMAX_MIN is not representable in @c intmax_t.
 **
 ** @a EXPR is guaranteed to be evaluated exactly once.
 **/
#define P99_ABS(EXPR) (P99_SIGNED(EXPR) ? p99__abs_signed(EXPR) : (EXPR))

#if defined(p99x_uintmax) && defined(p99x_intmax)
inline
p99x_uintmax p99x__abs_signed(p99x_intmax a) {
  p99x_uintmax aa = a;
  /* The minus is taken on the unsigned value, so it gives the
     correct result with -INTMAX_MAX, namely INTMAX_MAX, which might
     not be representable for two's complement representation. */
  return (a < (p99x_intmax)0) ? -aa : a;
}
#define P99X_ABS(EXPR) (P99_SIGNED(EXPR) ? p99x__abs_signed(EXPR) : (EXPR))
#endif

#ifndef P99X_ABS
/**
 ** @brief Compute the absolute value of an integral expression, using
 ** integers with extended width.
 **
 ** @return The return type of this macro is p99x_uintmax. It must be
 ** so, since if we have a two's complement representation the minimum
 ** signed value of p99x_intmax is not representable in p99x_intmax.
 **
 ** @a EXPR is guaranteed to be evaluated exactly once.
 **/
# define P99X_ABS P99__ABS
#endif

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
#define P99_TMAX(T)                                                     \
((T)                                                                    \
 (P99X__SHIFT((T)0)                                                     \
  ? (P99_ISSIGNED(T) ? P99X__SIGN_PROMOTE((T)-1)/2u : P99_UE_MAX((T)0)) \
  : (P99_ISSIGNED(T) ? P99_UT_MAX1(T) : P99_UT_MAX(T))))

/**
 ** @brief Give the minimum representable value of type @a T
 **/
#define P99_TMIN(T)                                                     \
((T)                                                                    \
 (P99X__SHIFT((T)0)                                                     \
  ? (P99_ISSIGNED(T) ? (-(P99X__SIGN_PROMOTE((T)-1)/2u)) - P99_2COMPLEMENT(T) : (T)0) \
  : (P99_ISSIGNED(T) ? (P99__ST_MIN1(T) - P99_2COMPLEMENT(T)) : P99_0(T))))

//#define P99_TMIN(T) ((T)(P99_ISSIGNED(T) ? (P99__ST_MIN1(T) - P99_2COMPLEMENT(T)) : P99_0(T)))


/**
 ** @brief C99 allows for exactly three different possibilities to
 ** encode negative values of integer types.
 **
 ** The representation can be found by looking at the two least
 ** significant bits of -1 represented in the type of the expression.
 ** @see ::p99_signed_representation
 **/
#define P99_E_REPRESENTATION(EXPR) ((p99_signed_representation)(P99_PROMOTE_M1(EXPR) & P99_PROMOTE_3(EXPR)))


/**
 ** @brief If the sign representation of a type is two's complement
 ** the type has no @em negative zero and can thus represent one more
 ** value.
 **/
#define P99_E_2COMPLEMENT(EXPR)                                         \
P99_SIGN_PROMOTE(P99_E_REPRESENTATION(EXPR) == p99_signed_representation_twos, (EXPR))

/**
 ** @brief Give the maximum representable value of the type of
 ** expression @a EXPR
 **/
#define P99_EMAX(EXPR) (P99_SIGNED(EXPR) ? P99_SE_MAX(EXPR) : P99_PROMOTE_M1(EXPR))

/**
 ** @brief Give the minimum representable value of the type of the
 ** expression @a EXPR
 **/
#define P99_EMIN(EXPR) (P99_SIGNED(EXPR) ? (P99__SE_MIN1(EXPR) - P99_E_2COMPLEMENT(EXPR)) : P99_PROMOTE_0(EXPR))

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
#define P99_TPREC(T)                            \
(P99X__SHIFT((T)-1)                             \
 ? P99_EPREC((T)-1)                             \
 : P99_HIGH2_1(P99_TMAX(T)))

  //(P99_HIGH2_1(P99_TMAX(T)))

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

/**
 ** @}
 **/


#endif 	    /* !P99_INT_H_ */
