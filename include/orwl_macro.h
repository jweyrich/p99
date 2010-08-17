/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	ORWL_MACRO_H_
# define   	ORWL_MACRO_H_

#include "p99_block.h"
#include "p99_defarg.h"
#include "orwl_document.h"

#define P99__DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP, ...)       \
  P99_IF_EMPTY(SIGN)(+)(SIGN)P99__SKIP_ P99_PASTE(              \
  P99_IF_EMPTY(INT)(0)(INT),                                    \
  .,                                                            \
  P99_IF_EMPTY(FRAC)(0)(FRAC),                                  \
  E,                                                            \
  P99_IF_EMPTY(ESIGN)(+)(ESIGN),                                \
  P99_IF_EMPTY(EXP)(0)(EXP),                                    \
  __VA_ARGS__)

#ifdef DOXYGEN
/** 
 ** @brief construct a @c double constant in decimal representation.
 **
 ** @param SIGN the sign of the constant, (+)
 ** @param INT the integer part, (0)
 ** @param FRAC the fractional part, (0)
 ** @param ESIGN sign of the exponent, (+)
 ** @param EXP exponent in decimal, (0)
 **
 ** So if all parameters are empty such as
 ** @code
 ** P99_DEC_DOUBLE()
 ** DEC(,,,,)
 ** @endcode
 ** this produces @c +0.0E+0
 **/
#define P99_DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP)
#else
#define P99_DEC_DOUBLE(...)                     \
  P99_IF_DEC_GE(P99_NARG(__VA_ARGS__), 6)       \
  (P99__DEC_DOUBLE(__VA_ARGS__))                \
  (P99__DEC_DOUBLE(__VA_ARGS__,,,,,))
#endif


#define P99__HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP, ...)      \
  P99_IF_EMPTY(SIGN)(+)(SIGN)P99__SKIP_ P99_PASTE(                      \
  0x,                                                                   \
  P99_IF_EMPTY(HEXINT)(0)(HEXINT),                                      \
  .,                                                                    \
  P99_IF_EMPTY(HEXFRAC)(0)(HEXFRAC),                                    \
  P,                                                                    \
  P99_IF_EMPTY(ESIGN)(+)(ESIGN),                                        \
  P99_IF_EMPTY(BINEXP)(0)(BINEXP),                                      \
  __VA_ARGS__)

#ifdef DOXYGEN
/**
 ** @brief construct a @c double constant in hexadecimal representation.
 **
 ** @see P99_DEC_DOUBLE for the analog with decimal representation
 **/
#define HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP)
#else
#define HEX_DOUBLE(...)                         \
  P99_IF_DEC_GE(P99_NARG(__VA_ARGS__), 6)       \
  (P99__HEX_DOUBLE(__VA_ARGS__))                \
  (P99__HEX_DOUBLE(__VA_ARGS__,,,,,))
#endif

#define P99__INV(N) P99_PASTE2(P99__variable_argument_list_must_be_divisible_by_, N)

#define P99__MODARG_(_X) P99_PASTE2(P99__NARG_,  _X)

/**
 ** @def LEN_MODARG
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It replaces @a X by
 ** the number of X-tuples in the following list of arguments.
 ** @see P99_VA_ARGS
 **/

/**
 ** @define expand to a comma token
 **
 ** This sometimes needed for trick macros such as
 ** ::P99_CALL_DEFARG_EVEN_EMPTY to ensure that a macro parameter
 ** inside a recursion is not empty.
 **/
#define P99__COMMA_ ,

#define INVARIANT(EXPR) P99_PROTECTED_BLOCK(assert("" && (EXPR)), assert("" && (EXPR)))

#define ARRAYSIZE(A) (sizeof(A)/sizeof(A[0]))

#define ARRAY2SIZE(A) A, ARRAY2SIZE(A)

#define CHOOSE5(xT, cc, cs, ci, cl, cll)        \
((sizeof(xT) < sizeof(int))                     \
 ? ((sizeof(xT) < sizeof(short))                \
    ? cc                                        \
    : cs)                                       \
 : ((sizeof(xT) <= sizeof(long))                \
    ? ((sizeof(xT) == sizeof(int))              \
       ? ci                                     \
       : cl)                                    \
    : cll))

#define PRI(xT, F, ...)                         \
CHOOSE5(xT,                                     \
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
 ** @brief Promote integer expression @a x to the width of @c
 ** size_t but keep signedness if possible.
 **/
#define P99__Z(x) (0 ? P99_0(size_t) : (x))


/**
 ** @brief Declare the types that are going to be used with a
 ** ::P99_LENGTH_VA_ARG or ::LEN_MODARG parameter list.
 **/
#define VA_TYPES(NAME, ...)   P99_TYPEDEFS(P99_PASTE2(NAME, _mod_type_), __VA_ARGS__)

#define P99__VA_MODARG(AP, NAME, M, ...) va_arg(AP, P99_PASTE3(NAME, _mod_type_, M))

/**
 ** @brief Obtain the next argument in the variable argument list of
 ** function @a NAME.
 **
 ** This takes three arguments, @a AP, @c NAME and  @c R.
 ** @a AP is the @c va_list that has been defined in the function.
 **
 ** @c NAME is the name of the function
 **
 ** @c R should be the position modulo the module M that was given to
 ** ::LEN_MODARG. @c R defaults to 0 if omitted.
 ** @see P99_VA_ARGS
 **/
#define VA_MODARG(AP, ...) P99__VA_MODARG(AP, __VA_ARGS__, 0, ~)

#define P99__CAS1(NAME, X, N) (P99_PASTE2(NAME, _mod_type_0){ X }
#define P99__CAS2(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 2))){ X }
#define P99__CAS3(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 3))){ X }
#define P99__CAS4(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 4))){ X }
#define P99__CAS5(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 5))){ X }
#define P99__MODARG_LIST(NAME, F, N, ...) P99_FOR(NAME, N, P99__SEQ, F, __VA_ARGS__)

#define LEN_MODARG(NAME, M, ...) P99__MODARG_(M)(__VA_ARGS__), P99__MODARG_LIST(NAME, P99_PASTE2(P99__CAS, M), P99_NARG(__VA_ARGS__), __VA_ARGS__)

#endif 	    /* !ORWL_MACRO_H_ */
