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

#include "orwl_inline.h"
#include "p99_args.h"


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



#endif 	    /* !P99_LOGICAL_H_ */
