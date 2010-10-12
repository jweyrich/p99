/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	P99_FOR_H_
# define   	P99_FOR_H_

/**
 ** @file
 ** @brief A prepprocessor for loop implementation and some derived
 ** list handling macros.
 ** @see P99_FOR
 **/

#include "p99_logical.h"



/** @addtogroup preprocessor_for Preprocessor FOR loops
 ** @{
 **/

#define P00_FOR0(NAME, OP, FUNC, ...)
#define P00_FOR1(NAME, OP, FUNC, ...) FUNC(NAME, P00_PRE1(__VA_ARGS__,), 0)

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
 ** Here @a NAME is the same as the argument to ::P99_FOR. @c I is the
 ** ordinal number of this iteration step, starting from 0. @c X is
 ** the @c Ith argument from the remaining argument list of this
 ** invocation of ::P99_FOR.
 **
 ** @a OP is called for iterations 1 to @a N - 1 to glue the results
 ** of the @a N iterations together. It is called in the form @a
 ** OP(NAME, I, REC, RES), where @a NAME and @c I are as before. @c
 ** REC is the result of all iterations with index less than @c I, @c
 ** RES is the result of the current iteration.
 **
 ** @code
 ** #define P00_SEP(NAME, I, REC, RES) REC; RES
 ** #define P00_VASSIGN(NAME, X, I) X = (NAME)[I]
 **
 ** P99_FOR(A, 2, P00_SEQ, P00_VASSIGN, toto, tutu);
 ** @endcode
 **
 ** Will result in
 ** @code
 ** toto = (A)[0]; tutu = (A)[1];
 ** @endcode
 **
 ** To understand the associativity of the @a OP argument 
 ** @code
 ** #define P00_SUM(NAME, I, REC, RES) ((REC) + (RES))
 ** #define P00_IDT(NAME, X, I) X
 **
 ** P99_FOR(A, 2, P00_SUM, P00_IDT, a, b, c)
 ** @endcode
 ** Will result in
 ** @code
 ** ((((a) + (b))) + (c))
 ** @endcode
 **/
#define P99_FOR(NAME, N, OP, FUNC, ...) P99_PASTE2(P00_FOR, N)(NAME, OP, FUNC, __VA_ARGS__)

#define P00_IGN(NAME, X, I)
#define P00_IDT(NAME, X, I) X
#define P00_POS(NAME, X, I) I
#define P00_NAM(NAME, X, I) NAME

#define P00_SUM(NAME, I, X, Y) ((X) + (Y))
#define P00_PROD(NAME, I, X, Y) ((X) * (Y))
#define P00_QUOT(NAME, I, X, Y) ((X) / (Y))
#define P00_XOR(NAME, I, X, Y) ((X) ^ (Y))
#define P00_BOR(NAME, I, X, Y) ((X) | (Y))
#define P00_BAND(NAME, I, X, Y) ((X) & (Y))
#define P00_OR(NAME, I, X, Y) ((X) || (Y))
#define P00_AND(NAME, I, X, Y) ((X) && (Y))

#define P00_SEQ(NAME, I, REC, X) REC, X
#define P00_SEP(NAME, I, REC, X) REC; X
#define P00_SER(NAME, I, REC, X) REC X
#define P00_REV(NAME, I, REC, X) X, REC

#define P00_FUNC(NAME, I, REC, X) NAME(REC, X)

/**
 ** @brief Realize the right associative operation @a OP of all the arguments.
 **
 ** Here @a OP should receive four arguments <code>NAME, X, N,
 ** REC</code> out of which @c NAME and @c N are ignored and @c X and
 ** @c REC should be interpreted as the left and right hand of the
 ** operator action, respectively.
 **
 ** @a M is the length of the list that follows it.
 **/
#define P99_BIGOP(OP, M, ...) P99_FOR( , M, OP, P00_IDT, __VA_ARGS__)

/**
 ** @brief Realize the right associative call of binary function @a FUNC of all the arguments.
 **
 ** Here @a FUNC should be a function of two arguments.
 **
 ** @a M is the length of the list that follows it.
 **/
#define P99_BIGFUNC(FUNC, M, ...) P99_FOR(FUNC, M, P00_FUNC, P00_IDT, __VA_ARGS__)

/**
 ** @}
 **/



/** @addtogroup statement_lists
 ** @{
 **/

/**
 ** @brief Realize the right associative sum of all the arguments.
 **/
#define P99_SUMS(...) P99_BIGOP(P00_SUM, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative product of all the arguments.
 **/
#define P99_PRODS(...) P99_BIGOP(P00_PROD, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative quotient of all the arguments.
 **/
#define P99_QUOTS(...) P99_BIGOP(P00_QUOT, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative bitwise exclusive or of all the arguments.
 **/
#define P99_XORS(...) P99_BIGOP(P00_XOR, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative bitwise or of all the arguments.
 **/
#define P99_BORS(...) P99_BIGOP(P00_BOR, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative bitwise and of all the arguments.
 **/
#define P99_BANDS(...) P99_BIGOP(P00_BAND, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative logical or of all the arguments.
 **/
#define P99_ORS(...) P99_BIGOP(P00_OR, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative logical and of all the arguments.
 **/
#define P99_ANDS(...) P99_BIGOP(P00_AND, P99_NARG(__VA_ARGS__),__VA_ARGS__)

/**
 ** @}
 **/


/**
 ** @ingroup preprocessor_for
 ** @brief Revert the argument list
 **/
#define P99_REVS(...) P00_REVS(P99_NARG(__VA_ARGS__),__VA_ARGS__)

#define P00_REVS(N, ...) P99_PASTE2(P00_REVS_, P99_IS_DEC_LT(N, 2))(N, __VA_ARGS__)

#define P00_REVS_0(N, ...) P00_REVS_(N,__VA_ARGS__)
#define P00_REVS_1(N, ...) __VA_ARGS__

/* the general case for an argument list of at least two elements */
#define P00_REVS_(N, ...) P99_FOR(,N, P00_REV, P00_IDT, __VA_ARGS__)

/**
 ** @ingroup preprocessor_arithmetic
 ** @brief Generate the quotient of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning Both arguments must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define P99_DEC_DIV(A, B) P99_CHS(A, P99_FOR(B, 32, P00_SEQ, P00_IDI, P00_ALL_ONES()))

#define P00_IDI(B, X, I) P99_DUPL(B, I)


#define P00_CDIM_OP(NAME, I, REC, X) (X + ((NAME)[I] * REC))
#define P00_CDIM_FUNC(NAME, X, I) (X)
#define P00_CDIM(N, NAME, ...) P99_FOR(NAME, N, P00_CDIM_OP, P00_CDIM_FUNC, __VA_ARGS__)

/**
 ** @ingroup preprocessor_arithmetic
 ** @brief Compute an absolute index in a multidimensional array with
 ** the same function as C would do that.
 **
 ** If we have @c N arguments after @a NAME, @a NAME must be an array
 ** of type
 ** @code
 ** size_t NAME[N];
 ** @endcode
 ** that hold the side lengths of an @c N dimensional cube. E.g
 ** @code
 ** size_t const D[] = { 3, 4, 7 };
 ** @endcode
 ** corresponds to a cube of side length 3 in dimension 0, 4 in
 ** dimension 1 and 7 in dimension 2, with in total 84 elements.
 ** With that, we get
 ** @code
 ** P99_CDIM(D, 1, 2, 3)  =>  ((3) + ((D)[2] * ((2) + ((D)[1] * (1)))))
 ** P99_CDIM(D, k0, k1, k2)  =>  ((k2) + ((D)[2] * ((k1) + ((D)[1] * (k0)))))
 ** @endcode
 **/
#define P99_CDIM(NAME, ...) P00_CDIM(P99_NARG(__VA_ARGS__), NAME, __VA_ARGS__)

#define P00_FORALL_OP(NAME, I, REC, X) REC X
#define P00_FORALL_FUNC(NAME, X, I) for (register unsigned X = 0; X < (NAME)[I]; ++X)
#define P00_FORALL(N, NAME, ...) P99_FOR(NAME, N, P00_FORALL_OP, P00_FORALL_FUNC, __VA_ARGS__)

/**
 ** @ingroup preprocessor_blocks
 ** @brief A multi-index @c for loop
 **
 ** Given the names of @c N identifiers after @a NAME, @a NAME must
 ** correspond to an array of lengths with dimension at least @c N.
 ** @code
 ** size_t NAME[N];
 ** @endcode
 ** For @c N = 3 this could e.g be
 ** @code
 ** size_t const D[] = { 3, 4, 7 };
 ** @endcode
 ** Then, the macro in the following
 ** @code
 ** P99_FORALL(D, i0, i1, i2) {
 **   A[i0][i1][i2] *= B[i0][i1][i2]
 ** }
 ** @endcode
 ** would expand to something similar to
 ** @code
 ** for (register unsigned i0 = 0; i0 < D[0]; ++i0)
 **   for (register unsigned i1 = 0; i1 < D[1]; ++i1)
 **     for (register unsigned i2 = 0; i2 < D[2]; ++i2) {
 **        A[i0][i1][i2] *= B[i0][i1][i2]
 **     }
 ** @endcode
 **
 ** @see P99_PARALLEL_FORALL for a variant that uses OpenMp to parallelize the loop.
 ** @see P99_CDIM for a macro that computes the absolute position of a
 **   index N-tuple in a multi-dimensional array.
 **/
#define P99_FORALL(NAME, ...) P00_FORALL(P99_NARG(__VA_ARGS__), NAME, __VA_ARGS__)

#define P00_PARALLEL_FORALL_FUNC(NAME, X, I) P99_PARALLEL_FOR (register unsigned X = 0; X < (NAME)[I]; ++X)
#define P00_PARALLEL_FORALL(N, NAME, ...) P99_FOR(NAME, N, P00_FORALL_OP, P00_PARALLEL_FORALL_FUNC, __VA_ARGS__)

/**
 ** @ingroup preprocessor_blocks
 ** @brief A multi-index @c for loop who's dependent statement or
 ** block may be executed out of order.
 **
 ** This has the same semantics as ::P99_FORALL only that it assumes
 ** the independence of the of each execution of the statement from
 ** the others.
 **
 ** @see P99_FORALL for a variant that doesn't need that assumption,
 ** i.e where the statements should be executed sequentially in order.
 ** @see P99_PARALLEL_FOR for a simple parallel replacement of @c for
 ** @see P99_CDIM for a macro that computes the absolute position of a
 **   index N-tuple in a multi-dimensional array.
 **/
#define P99_PARALLEL_FORALL(NAME, ...) P00_PARALLEL_FORALL(P99_NARG(__VA_ARGS__), NAME, __VA_ARGS__)


#endif 	    /* !P99_FOR_H_ */
