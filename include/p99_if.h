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

#include "orwl_inline.h"
#include "p99_logical.h"


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
 ** IF_EQ(0, X)(expand_when_equal)(expand_when_unequal)
 ** @endcode
 **
 ** the macro ::P99__IF_0_EQ_0 must exist. (Which it does in that case).
 **
 ** @see IF_DEC_EQ for equality of not too large decimal numbers
 **/
#define IF_EQ(A, B) P99__IF_CLAUSE(PASTE(P99__IS_,A,_EQ_,B)())

/**
 ** @brief Test two words @a A and @a B if they are unequal.
 **
 ** @see IF_EQ
 **/
#define IF_NE(A, B) P99__IF_NOT_CLAUSE(PASTE(P99__IS_,A,_EQ_,B)())

/**
 ** @brief Test two decimal numbers @a A and @a B if they are equal.
 **/
#define IF_DEC_EQ(A, B) IF_EQ_1(IS_DEC_EQ(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if they are unequal.
 **/
#define IF_DEC_NE(A, B) IF_EQ_0(IS_DEC_EQ(A,B))

#define IF_GE_0(A) P99__IF_CLAUSE(PASTE(P99__IS_,A,_GE_,0)())
#define IF_LT_0(A) P99__IF_NOT_CLAUSE(PASTE(P99__IS_,A,_GE_,0)())

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is greater
 ** or equal to @a B.
 **/
#define IF_DEC_GE(A, B) IF_EQ_1(IS_DEC_GE(A, B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is less
 ** or equal to @a B.
 **/
#define IF_DEC_LE(A, B) IF_EQ_1(IS_DEC_LE(A, B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** less than @a B.
 **/
#define IF_DEC_LT(A, B) IF_EQ_1(IS_DEC_LT(A, B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** greater than @a B.
 **/
#define IF_DEC_GT(A, B) IF_EQ_1(IS_DEC_GT(A, B))

/**
 ** @brief Test if token N is the token 0.
 **/
#define IF_EQ_0(N) IF_EQ(0, N)

/**
 ** @brief Test if token N is the token 1.
 **/
#define IF_EQ_1(N) IF_EQ(1, N)

/**
 ** @brief Test if token N is the token 2.
 **/
#define IF_EQ_2(N) IF_EQ(2, N)

/**
 ** @brief Test if token N is the token 3.
 **/
#define IF_EQ_3(N) IF_EQ(3, N)

/**
 ** @brief Test if token N is the token 4.
 **/
#define IF_EQ_4(N) IF_EQ(4, N)

#define IF_EMPTY(...) IF_EQ_1(IS_EMPTY(__VA_ARGS__))

/**
 ** @brief A preprocessor control structure
 **
 ** Use this as follows
 ** @code
 ** IF_ELSE(some_expression)(tokens_A)(tokens_B)
 ** @endcode
 ** This expands to tokens_B if the list expanded to the token 0 and
 ** to tokens_A in any other case.
 **
 ** Observe the parenthesis around tokens_A and tokens_B.
 **/
#define IF_ELSE(...) IF_EQ_1(IS_EQ_0(__VA_ARGS__))

#define IF_void(...) IF_EQ_1(IS_EQ_void(__VA_ARGS__))

#define IF_VOID(...) IF_EQ_1(IS_VOID(__VA_ARGS__))

#endif 	    /* !P99_IF_H_ */
