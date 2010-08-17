/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	P99_IF_H_
# define   	P99_IF_H_

/**
 ** @file
 ** @brief preprocessor conditionals to use inside macros
 **/

#include "p99_logical.h"

/**
 ** @addtogroup preprocessor_conditionals Preprocessor conditionals
 **
 ** @brief This provides different types of if-else clauses for the preprocessor.
 **
 ** For the underlying logical and arithmetical operations
 ** @see preprocessor_logic
 ** @see preprocessor_arithmetic
 ** @{
 **/

#define P99__IGNORE(...)
#define P99__IDENT(...) __VA_ARGS__
#define P99__SKIP_
#define P99__CLAUSE1(...) __VA_ARGS__ P99__IGNORE
#define P99__CLAUSE2(...) P99__IDENT

#define P99___IF_CLAUSE(A,B,C,...) C
#define P99__IF_CLAUSE(EXP) P99___IF_CLAUSE(EXP, P99__CLAUSE1, P99__CLAUSE2, ~)
#define P99__IF_NOT_CLAUSE(EXP) P99___IF_CLAUSE(EXP, P99__CLAUSE2, P99__CLAUSE1, ~)

/**
 ** @brief Test two words @a A and @a B if they are equal.
 **
 ** @a A and @a B must be just one word, i.e composed of
 ** alpha-numerical characters and underscores.
 **
 ** For such a test to work properly a corresponding macro @c P99__IS_A_EQ_A
 ** must exist for all @a A such that @a B may be the same word as @a
 ** A. E.g for the test
 **
 ** @code
 ** P99_IF_EQ(0, X)(expand_when_equal)(expand_when_unequal)
 ** @endcode
 **
 ** the macro ::P99__IF_0_EQ_0 must exist. (Which it does in that case).
 **
 ** @see P99_IF_DEC_EQ for equality of not too large decimal numbers
 **/
#define P99_IF_EQ(A, B) P99__IF_CLAUSE(P99_PASTE4(P99__IS_,A,_EQ_,B)())

/**
 ** @brief Test two words @a A and @a B if they are unequal.
 **
 ** @see P99_IF_EQ
 **/
#define P99_IF_NE(A, B) P99__IF_NOT_CLAUSE(P99_PASTE4(P99__IS_,A,_EQ_,B)())

/**
 ** @brief Test two decimal numbers @a A and @a B if they are equal.
 **/
#define P99_IF_DEC_EQ(A, B) P99_IF_EQ_1(P99_IS_DEC_EQ(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if they are unequal.
 **/
#define P99_IF_DEC_NE(A, B) P99_IF_EQ_0(P99_IS_DEC_EQ(A,B))

#define P99_IF_GE_0(A) P99__IF_CLAUSE(P99_PASTE4(P99__IS_,A,_GE_,0)())
#define P99_IF_LT_0(A) P99__IF_NOT_CLAUSE(P99_PASTE4(P99__IS_,A,_GE_,0)())

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is greater
 ** or equal to @a B.
 **/
#define P99_IF_DEC_GE(A, B) P99_IF_EQ_1(P99_IS_DEC_GE(A, B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is less
 ** or equal to @a B.
 **/
#define P99_IF_DEC_LE(A, B) P99_IF_EQ_1(P99_IS_DEC_LE(A, B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** less than @a B.
 **/
#define P99_IF_DEC_LT(A, B) P99_IF_EQ_1(P99_IS_DEC_LT(A, B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** greater than @a B.
 **/
#define P99_IF_DEC_GT(A, B) P99_IF_EQ_1(P99_IS_DEC_GT(A, B))

/**
 ** @brief Test if token N is the token 0.
 **/
#define P99_IF_EQ_0(N) P99_IF_EQ(0, N)

/**
 ** @brief Test if token N is the token 1.
 **/
#define P99_IF_EQ_1(N) P99_IF_EQ(1, N)

/**
 ** @brief Test if token N is the token 2.
 **/
#define P99_IF_EQ_2(N) P99_IF_EQ(2, N)

/**
 ** @brief Test if token N is the token 3.
 **/
#define P99_IF_EQ_3(N) P99_IF_EQ(3, N)

/**
 ** @brief Test if token N is the token 4.
 **/
#define P99_IF_EQ_4(N) P99_IF_EQ(4, N)

#define P99_IF_EMPTY(...) P99_IF_EQ_1(P99_IS_EMPTY(__VA_ARGS__))

/**
 ** @brief A preprocessor control structure
 **
 ** Use this as follows
 ** @code
 ** P99_IF_ELSE(some_expression)(tokens_A)(tokens_B)
 ** @endcode
 ** This expands to tokens_B if the list expanded to the token 0 and
 ** to tokens_A in any other case.
 **
 ** Observe the parenthesis around tokens_A and tokens_B.
 **/
#define P99_IF_ELSE(...) P99_IF_EQ_1(P99_IS_EQ_0(__VA_ARGS__))

#define P99_IF_void(...) P99_IF_EQ_1(P99_IS_EQ_void(__VA_ARGS__))

#define P99_IF_VOID(...) P99_IF_EQ_1(P99_IS_VOID(__VA_ARGS__))


/**
 ** @}
 **/

#endif 	    /* !P99_IF_H_ */
