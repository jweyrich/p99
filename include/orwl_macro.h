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

#include "orwl_inline.h"
#include "orwl_document.h"
#include "p99_defarg.h"

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
 ** @see VA_ARGS
 **/

/**
 ** @def LEN_ARG
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It prefixes the
 ** list of arguments by an integer constant containing the length of
 ** the list.
 ** @see VA_ARGS
 **/


/**
 ** @brief Helper macro to declare a variable length parameter list.
 **
 ** Inside the declared function @a X will of @c size_t and should
 ** hold the actual length of the list. It can be used as the argument
 ** to @c va_start.
 **
 ** Wrap your function into a macro that uses LEN_ARG. If used through
 ** that macro, the correct value for @a X will always be provided at
 ** compile time. Declare such a function as this:
 ** @code
 ** unsigned FSYMB(toto)(unsigned a, VA_ARGS(number));
 ** #define toto(A, ...) FSYMB(toto)(A, LEN_ARG(toto, __VA_ARGS__))
 ** VA_TYPES(toto, unsigned);
 ** @endcode
 **
 ** In the definition of the function you then may use the @c va_start
 ** etc from stdarg.h to tread the argument list.
 ** @code
 ** unsigned FSYMB(toto)((unsigned a, VA_ARGS(number)) {
 **     unsigned ret = 0;
 **     va_list ap;
 **     va_start(ap, number);
 **     for (size_t i = 0; i < number; ++i) {
 **       ret += VA_MODARG(ap, toto);
 **     }
 **     va_end(ap);
 **     return ret % a;
 ** }
 ** @endcode
 ** In this toy example @c toto can be used as
 ** @code
 ** unsigned magic = toto(3, 1, 3, 5, 7);
 ** @endcode
 ** which will result in converting 1, 3, 5, 7 (the variable
 ** arguments) to @c unsigned, computing their sum, i.e 16u, and
 ** compute that value mod 3u (the fixed argument @a a). So @a magic
 ** should hold the value 1u thereafter.
 **
 ** @param X is the name of the `length' parameter that you want to
 ** use in the definition of the function. As in the example above it
 ** should be then used as the second argument to @c va_start and as a
 ** loop boudary when you actual handle the argument list. @a X is
 ** implicitly declared to have type @c size_t.
 **
 ** @see LEN_ARG
 ** @see LEN_MODARG
 ** @see VA_TYPES
 ** @see FSYMB
 **/
#define VA_ARGS(X) size_t X /*!< the number of arguments that follow */, ...

#define P99__FSYMB(NAME) P99_PASTE5(NAME, _f, sy, mb, _)

/** @brief Mangle @a NAME 
 **
 ** This should only be used in declaration and definition of the
 ** function that is hidden behind the macro @a NAME.
 **/
#define FSYMB(NAME) P99__FSYMB(NAME)


/**
 ** @define expand to a comma token
 **
 ** This sometimes needed for trick macros such as
 ** ::P99_CALL_DEFARG_EVEN_EMPTY to ensure that a macro parameter
 ** inside a recursion is not empty.
 **/
#define P99__COMMA_ ,

/**
 ** @brief A meta-macro to protect a dependent block or statement by a
 ** statement that is executed before and one after.
 **
 ** Preliminary exit of the block is possible with @c break.
 **
 ** @warning @c continue and @c return inside the dependent block will
 ** not execute @a AFTER, so be careful.
 **/
#define BLOCK(BEFORE, AFTER)                                            \
for (int _one1_ = 1;                                                    \
     /* be sure to execute BEFORE only at the first evaluation */       \
     (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                      \
     /* run AFTER exactly once */                                       \
     ((void)(AFTER), _one1_ = 0))                                       \
  /* Ensure that a `break' will still execute AFTER */                  \
  for (; _one1_; _one1_ = 0)


#define INVARIANT(EXPR) BLOCK(assert("" && (EXPR)), assert("" && (EXPR)))

#define SAVE_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)                     \
for (int _one1_ = 1; _one1_; _one1_ = 0)                                \
  for (T NAME = (INITIAL);                                              \
       /* be sure to execute BEFORE only at the first evaluation */     \
       (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                    \
       /* run AFTER exactly once */                                     \
       ((void)(AFTER), _one1_ = 0))                                     \
    /* Ensure that a `break' will still execute AFTER */                \
    for (; _one1_; _one1_ = 0)


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
#define P99__J(x) (0 ? TNULL(uintmax_t) : (x))

/**
 ** @brief Promote integer expression @a x to the width of @c
 ** size_t but keep signedness if possible.
 **/
#define P99__Z(x) (0 ? TNULL(size_t) : (x))


/**
 ** @brief Declare the types that are going to be used with a
 ** ::LEN_ARG or ::LEN_MODARG parameter list.
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
 ** @see VA_ARGS
 **/
#define VA_MODARG(AP, ...) P99__VA_MODARG(AP, __VA_ARGS__, 0, ~)

#define P99__CAS1(NAME, X, N) (P99_PASTE2(NAME, _mod_type_0){ X }
#define P99__CAS2(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 2))){ X }
#define P99__CAS3(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 3))){ X }
#define P99__CAS4(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 4))){ X }
#define P99__CAS5(NAME, X, N) (P99_PASTE3(NAME, _mod_type_, P99_DEC_MOD(N, 5))){ X }
#define P99__MODARG_LIST(NAME, F, N, ...) P99_FOR(NAME, N, P99__SEQ, F, __VA_ARGS__)

#define LEN_MODARG(NAME, M, ...) P99__MODARG_(M)(__VA_ARGS__), P99__MODARG_LIST(NAME, P99_PASTE2(P99__CAS, M), P99_NARG(__VA_ARGS__), __VA_ARGS__)
#define LEN_ARG(NAME, ...) P99__MODARG_(1)(__VA_ARGS__), P99__MODARG_LIST(NAME, P99__CAS1, P99_NARG(__VA_ARGS__), __VA_ARGS__)


/**
 ** @def branch_expect
 ** @brief Provide a compiler hint concerning the likelihood of a
 ** certain value in an expression @a EXP.
 **
 ** With this you indicate that in the overwhelming number of cases
 ** the integer expression @a EXP will evaluate to @a VAL.
 **
 ** Currently this is only implemented for gcc.
 **/
#if (defined(__GNUC__) && (__GNUC__ > 3))
#define branch_expect(EXP, VAL) __builtin_expect((EXP), (VAL))
#else
#define branch_expect(EXP, VAL) (EXP)
#endif


#endif 	    /* !ORWL_MACRO_H_ */
