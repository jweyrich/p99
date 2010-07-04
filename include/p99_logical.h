/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	P99_LOGICAL_H_
# define   	P99_LOGICAL_H_

#include "p99_args.h"
#include "p99_list.h"


/**
 ** @brief Determine if the two tokens @a _0 and @a _2 are equal
 **
 ** For a specific token X to work with this, a macro @c
 ** P99__IS_X_EQ_X must have been declared and expand to a comma.
 **/
#define IS_EQ(_0, _1) HAS_COMMA(PASTE4(P99__IS_, _0, _EQ_, _1)())


/**
 ** @brief Do a evaluation of the argument.
 **
 ** The result is the token 0 (seen here as `false') if the argument
 ** was empty or if it evaluated to the token 0. Otherwise returns the
 ** token 1.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_EVAL(_0) P99__EVAL_0(IS_EMPTY(_0), IS_EQ_0(_0))

#define P99__EVAL_0(_0, _1) PASTE3(P99__EVAL_, _0, _1)

#define P99__EVAL_00 0
#define P99__EVAL_10 1
#define P99__EVAL_01 0
/* should never happen */
#define P99__EVAL_11 WEIRD_EVALUATION_ERROR

/**
 ** @brief Do a logical negation of the argument.
 **
 ** The result is the token 1 (seen here as `true') if the argument
 ** was empty or if it evaluated to the token 0. Otherwise returns the
 ** token 0.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_NOT(A)  P99__NOT_0(IS_EMPTY(_0), IS_EQ_0(_0))

#define P99__NOT_0(_0, _1) PASTE3(P99__NOT_, _0, _1)

#define P99__NOT_00 1
#define P99__NOT_10 0
#define P99__NOT_01 1
/* should never happen */
#define P99__NOT_11 WEIRD_NEGATION_ERROR

/**
 ** @brief Do a logical exclusive or of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_XOR(A, B) IS_EQ(LOGIC_NOT(A), LOGIC_EVAL(B))


/**
 ** @brief Do a logical inclusive or of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_OR(A, B) P99__LOGIC_OR(LOGIC_EVAL(A), LOGIC_EVAL(B))


#define P99__LOGIC_OR(_0, _1) PASTE3(P99__LOGIC_OR_, _0, _1)

#define P99__LOGIC_OR_00 0
#define P99__LOGIC_OR_10 1
#define P99__LOGIC_OR_01 1
#define P99__LOGIC_OR_11 1

/**
 ** @brief Do a logical and of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_AND(A, B) P99__LOGIC_AND(LOGIC_EVAL(A), LOGIC_EVAL(B))


#define P99__LOGIC_AND(_0, _1) PASTE3(P99__LOGIC_AND_, _0, _1)

#define P99__LOGIC_AND_00 0
#define P99__LOGIC_AND_10 0
#define P99__LOGIC_AND_01 0
#define P99__LOGIC_AND_11 1



/**
 ** @def IS_EQ_void(...)
 ** @brief Test if the argument consists of exactly the token @c void.
 **
 ** @see IS_VOID for a macro that test whether or not its argument is
 ** empty @b or if it consists of the word @c void.
 **/


/**
 ** @brief Test whether or not its argument is empty @b or if it
 ** consists of the word @c void.
 **
 ** @see IF_void for a macro that tests if the argument is exactly the
 ** word @c void.
 **/
#define IS_VOID(...) P99__IS_VOID(IS_EMPTY(__VA_ARGS__), IS_EQ_void(__VA_ARGS__))

#define P99__IS_VOID(_0, _1) PASTE3(P99__IS_VOID_, _0, _1)

#define P99__IS_VOID_00 0
#define P99__IS_VOID_01 1
#define P99__IS_VOID_10 1
/* should not happen */
#define P99__IS_VOID_11 WEIRD_VOID_ARG_ERROR

/** @brief a decimal less than operator **/
#define IS_DEC_LT(_0, _1)  P99__DEC_LT( IS_EQ_0(_1), CHS(_0, SELS(_1, P99__ALL_ONES()), P99__ALL_ZEROES()))
#define P99__DEC_LT(_0, _1)  PASTE2(P99__DEC_LT_, _0)(_1)

#define P99__DEC_LT_0(_0) _0
#define P99__DEC_LT_1(_0) 0

/** @brief a decimal greater or equal operator **/
#define IS_DEC_GE(_0, _1)  P99__DEC_GE( IS_EQ_0(_1), CHS(_0, SELS(_1, P99__ALL_ZEROES()), P99__ALL_ONES()))
#define P99__DEC_GE(_0, _1)  PASTE2(P99__DEC_GE_, _0)(_1)

#define P99__DEC_GE_0(_0) _0
#define P99__DEC_GE_1(_0) 1

/** @brief a decimal greater than operator **/
#define IS_DEC_GT(_0, _1)  IS_DEC_LT(_1, _0)
/** @brief a decimal less or equal operator **/
#define IS_DEC_LE(_0, _1)  IS_DEC_GE(_1, _0)

/** @brief a decimal equal operator **/
#define IS_DEC_EQ(_0, _1)  IS_EQ(_0, _1)

/** @brief a decimal unequal operator **/
#define IS_DEC_NE(_0, _1)  LOGIC_NOT(IS_EQ(_0, _1))

/** @brief add two decimal numbers **/
#define DEC_ADD(_0, _1)                         \
P99__DEC_ADD(_0, _1,                            \
             IS_EQ_0(_0),                       \
             IS_EQ_0(_1),                       \
             NARG(                              \
                  SELS(_0, P99__ALL_ZEROES()),  \
                  SELS(_1, P99__ALL_ZEROES())))


#define P99__DEC_ADD(_0, _1, _2, _3, _4) PASTE3(P99__DEC_ADD_, _2, _3)(_0, _1, _4)

#define P99__DEC_ADD_00(_0, _1, _2) _2
#define P99__DEC_ADD_01(_0, _1, _2) _0
#define P99__DEC_ADD_10(_0, _1, _2) _1
#define P99__DEC_ADD_11(_0, _1, _2) 0


/**
 ** @brief substract two decimal numbers
 **
 ** If the result is negative, a token of the form @c minus_NNN is
 ** returned where @c NNN would be the result of
 ** @code
 ** DEC_MINUS(_1, _0)
 ** @endcode
 **/
#define DEC_MINUS(_0, _1) P99__DEC_MINUS(_0, _1, IS_EQ(_0, _1), IS_EQ_0(_0), IS_EQ_0(_1))
#define P99__DEC_MINUS(_0, _1, _2, _3, _4) PASTE4(P99__DEC_MINUS_, _2, _3, _4)(_0, _1)

#define P99__DEC_MINUS_000(_0, _1) P99__DEC_MINUS_(_0, _1, IS_DEC_LT(_0, _1))
/* if one of _0 or _1 is 0, there is not much to do */
#define P99__DEC_MINUS_001(_0, _1) _0
#define P99__DEC_MINUS_010(_0, _1) PASTE2(minus_, _1)
/* should not occur */
#define P99__DEC_MINUS_011(_0, _1) P99_WEIRD_MINUS_ARG_ERROR
/* whenever _0 and _1 are equal there is nothing to do */
#define P99__DEC_MINUS_100(_0, _1) 0
#define P99__DEC_MINUS_101(_0, _1) 0
#define P99__DEC_MINUS_110(_0, _1) 0
#define P99__DEC_MINUS_111(_0, _1) 0

#define P99__DEC_MINUS_(_0, _1, _2) PASTE2(P99__DEC_MINUS_, _2)(_0, _1)

#define P99__DEC_MINUS_0(_0, _1) P99__DEC_MINUS__(_0, _1)
#define P99__DEC_MINUS_1(_0, _1) PASTE2(minus_, P99__DEC_MINUS__(_1, _0))



/* The general case both are non-zero and _0 is strictly greater than _1 */
#define P99__DEC_MINUS__(_0, _1) NARG(SKP(_1, SELS(_0, P99__ALL_ZEROES())))


/**
 ** @brief Macro that expands to the predecessor of decimal constant
 ** @a N
 **/
#define DEC_PRED(N) P99__DEC_PRED(N)
#define P99__DEC_PRED(N) P99___DEC_PRED(P99__DEC_PRED_ , N)
#define P99___DEC_PRED(P, N) P ## N

#define P99__DEC_PRED_0 minus_1

/**
 ** @brief evaluate the result of one of the arithmetic preprocessor 
 **/
#define DEC_EVAL(EDEC) PASTE2(P99__dec_eval_, EDEC)


/**
 ** @brief Generate the product of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning The result must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define DEC_MUL(A, B) PASTE3(DEC_MUL_, IS_EQ_0(A), IS_EQ_0(B))(A, B)

#define DEC_MUL_00(A, B) NARG(DUPL(A, SELS(B, P99__ALL_ONES())))
#define DEC_MUL_01(A, B) 0
#define DEC_MUL_10(A, B) 0
#define DEC_MUL_11(A, B) 0


/**
 ** @brief Generate the modulus of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning Both arguments must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define DEC_MOD(A, B) P99__DEC_MOD(A, DUPL(32, SELS(B, P99__ASCENDING())))
#define P99__DEC_MOD(A, ...) CHS(A, __VA_ARGS__)


/**
 ** @brief Obtain the last element of a list.
 **/
#define LAST(...) CHS(DEC_PRED(P99__NARG(__VA_ARGS__)), __VA_ARGS__,)

/**
 ** @brief Obtain all elements but the last of a list.
 **/
#define ALLBUTLAST(...) PASTE2(P99__PRE,DEC_PRED(P99__NARG(__VA_ARGS__)))(__VA_ARGS__,)

#define P99___PASTE(F, N, ...) F ## N(__VA_ARGS__)
#define P99__PASTE(N, ...) P99___PASTE(PASTE, N, __VA_ARGS__)


/** @addtogroup list_processing List processing macros
 ** @brief We provide here a series of macros that take a list of
 ** arguments of arbitrary length and that transform each element in
 ** that list in some way.
 ** @{
 **/


/**
 ** @brief A left-to-right associative paste operator.
 **
 ** This macro avoids the ambiguity of the @c ## preprocessor operator
 ** which has no well defined associativity. With this macro here
 ** something like
 ** @code
 ** PASTE(0.1E, -, 1)
 ** @endcode
 ** is guaranteed to produce the token @c 0.1E-1, whereas the
 ** seemingly equivalent
 ** @code
 ** ETSAP(0.1E, -, 1)
 ** @endcode
 ** is not valid: the intermediate operation to paste tokens `-' and
 ** `1' would result in an invalid token and is thus rejected.
 **
 ** This macro does the evaluation of the arguments first and
 ** then proceeds at the concatenation of the results.
 ** @pre the argumentlist should not be empty.
 **/
#define PASTE(...) P99__PASTE(P99__NARG(__VA_ARGS__), __VA_ARGS__)

/** @}
 **/


#endif 	    /* !P99_LOGICAL_H_ */
