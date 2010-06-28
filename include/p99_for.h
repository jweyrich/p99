/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	P99_FOR_H_
# define   	P99_FOR_H_

#include "orwl_inline.h"
#include "p99_logical.h"



/** @addtogroup list_processing
 ** @{
 **/

/**
 ** @brief Obtain the last element of a list.
 **/
#define LAST(...) CHS(DEC_PRED(P99__NARG(__VA_ARGS__)), __VA_ARGS__,)

/**
 ** @brief Obtain all elements but the last of a list.
 **/
#define ALLBUTLAST(...) PASTE2(P99__PRE,DEC_PRED(P99__NARG(__VA_ARGS__)))(__VA_ARGS__,)

#define P99__FOR0(NAME, OP, FUNC, ...)
#define P99__FOR1(NAME, OP, FUNC, ...) FUNC(NAME, P99__PRE1(__VA_ARGS__,), 0)

/**
 ** @brief A preprocessor pseudo iterator.
 **
 ** @param NAME a name or other data that is visible to all iterations
 ** @param N the number of times the iteration is to be done
 ** @param OP an operation that is performed to glue the different
 **        results of the individual iterations together
 ** @param FUNC a function that is applied in each iteration
 **
 ** In each iteration, @a FUNC will be called as @c FUNC(NAME, X, I).
 ** Here @a NAME is the same as the argument to ::FOR. @c I is the
 ** ordinal number of this iteration step, starting from 0. @c X is
 ** the @c Ith argument from the remaining argument list of this
 ** invocation of ::FOR.
 **
 ** @a OP is called for iterations 1 to @a N - 1 to glue the results
 ** of the @a N iterations together. It is called in the form @a
 ** OP(NAME, I, REC, RES), where @a NAME and @c I are as before. @c
 ** REC is the result of all iterations with index less than @c I, @c
 ** RES is the result of the current iteration.
 **
 ** @code
 ** #define P99__SEP(NAME, I, REC, RES) REC; RES
 ** #define P99__VASSIGN(NAME, X, I) X = (NAME)[I]
 **
 ** FOR(A, 2, P99__SEQ, P99__ACCESSOR, toto, tutu);
 ** @endcode
 **
 ** Will result in
 ** @code
 ** toto = A[0]; tutu[1];
 ** @endcode
 **
 ** To understand the associativity of the @a OP argument 
 ** @code
 ** #define P99__SUM(NAME, I, REC, RES) ((REC) + (RES))
 ** #define P99__IDT(NAME, X, I) X
 **
 ** FOR(A, 2, P99__SUM, P99__IDT, a, b, c)
 ** @endcode
 ** Will result in
 ** @code
 ** ((((a) + (b))) + (c))
 ** @endcode
 **/
#define FOR(NAME, N, OP, FUNC, ...) PASTE2(P99__FOR, N)(NAME, OP, FUNC, __VA_ARGS__)

#define P99__IGN(NAME, X, I)
#define P99__IDT(NAME, X, I) X
#define P99__POS(NAME, X, I) I
#define P99__NAM(NAME, X, I) NAME

#define P99__SUM(NAME, I, X, Y) ((X) + (Y))
#define P99__PROD(NAME, I, X, Y) ((X) * (Y))
#define P99__QUOT(NAME, I, X, Y) ((X) / (Y))
#define P99__XOR(NAME, I, X, Y) ((X) ^ (Y))
#define P99__BOR(NAME, I, X, Y) ((X) | (Y))
#define P99__BAND(NAME, I, X, Y) ((X) & (Y))
#define P99__OR(NAME, I, X, Y) ((X) || (Y))
#define P99__AND(NAME, I, X, Y) ((X) && (Y))

#define P99__SEQ(NAME, I, REC, X) REC, X
#define P99__SEP(NAME, I, REC, X) REC; X
#define P99__SER(NAME, I, REC, X) REC X
#define P99__REV(NAME, I, REC, X) X, REC

/**
 ** @brief Compute the right associative operation @a OP of all the arguments.
 **
 ** Here @a OP should receive four arguments <code>NAME, X, N,
 ** REC</code> out of which @c NAME and @c N are ignored and @c X and
 ** @c REC should be interpreted as the left and right hand of the
 ** operator action, respectively.
 **
 ** @a M is the length of the list that follows it.
 **/
#define BIGOP(OP, M, ...) FOR( , M, OP, P99__IDT, __VA_ARGS__,)

/**
 ** @brief Compute the right associative sum of all the arguments.
 **/
#define SUMS(...) BIGOP(P99__SUM, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative product of all the arguments.
 **/
#define PRODS(...) BIGOP(P99__PROD, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative quotient of all the arguments.
 **/
#define QUOTS(...) BIGOP(P99__QUOT, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative bitwise exclusive or of all the arguments.
 **/
#define XORS(...) BIGOP(P99__XOR, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative bitwise or of all the arguments.
 **/
#define BORS(...) BIGOP(P99__BOR, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative bitwise and of all the arguments.
 **/
#define BANDS(...) BIGOP(P99__BAND, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative logical or of all the arguments.
 **/
#define ORS(...) BIGOP(P99__OR, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative logical and of all the arguments.
 **/
#define ANDS(...) BIGOP(P99__AND, (NARG(__VA_ARGS__),__VA_ARGS__)

/**
 ** @brief Revert the argument list
 **/
#define REVS(...) P99__REVS(NARG(__VA_ARGS__),__VA_ARGS__)

#define P99__REVS(N, ...) PASTE2(P99__REVS_, IS_DEC_LT(N, 2))(N, __VA_ARGS__)

#define P99__REVS_0(N, ...) P99__REVS_(N,__VA_ARGS__)
#define P99__REVS_1(N, ...) __VA_ARGS__

/* the general case for an argument list of at least two elements */
#define P99__REVS_(N, ...) FOR(,N, P99__REV, P99__IDT, __VA_ARGS__)

#define P99__UNP(NAME, X, I) P99__IDENT NAME

/**
 ** @brief Construct a list that repeats the argument list @a N times
 **
 ** The corner cases should work as expected:
 ** - If @a N is 0 the empty list is produced.
 ** - If the argument list is empty, the empty list is produced.
 ** - If the argument list has just one element which is empty, the
 **   result will just be a sequence of @a N - 1 commas.
 **/
#define DUPL(...) PASTE2(P99__DUPL_, IS_DEC_LT(NARG(__VA_ARGS__), 2))(__VA_ARGS__)

#define P99__DUPL_0(...) P99__DUPL__(__VA_ARGS__)
#define P99__DUPL_1(...)

#define P99__DUPL__(N, ...) FOR((__VA_ARGS__), N, P99__SEQ, P99__UNP, P99__ALL_ZEROES)

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

/** @}
 **/


#endif 	    /* !P99_FOR_H_ */
