/*
** p99_list.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Jun 27 17:43:03 2010 Jens Gustedt
** Last update Sun Jun 27 17:43:03 2010 Jens Gustedt
*/

#ifndef   	P99_LIST_H_
# define   	P99_LIST_H_

#include "p99_generated.h"

#define P99__PRE0(...)
#define P99__PRE1(_0, ...) _0

/** @addtogroup list_processing List processing macros
 ** @brief We provide here a series of macros that take a list of
 ** arguments of arbitrary length and that transform each element in
 ** that list in some way.
 ** @{
 **/

/**
 ** Cut the argument list at position @a N
 **/
#define SELS(N, ...) PASTE2(P99__PRE, N)(__VA_ARGS__)

#define P99__SKP0(...) __VA_ARGS__
#define P99__SKP1(_0, ...) __VA_ARGS__


/**
 ** @brief Skip @a N elements in the remaining argument list.
 **/
#define SKP(N, ...) PASTE2(P99__SKP, N)(__VA_ARGS__)

/**
 ** @brief Get the sublist of length @a L starting at the @a
 ** N<sup>th</sup> element in the remaining argument list.
 **
 ** Counting of elements starts at 0.
 **/
#define SUB(N, L, ...) P99__SUB(L, SKP(N, __VA_ARGS__))
#define P99__SUB(L, ...) SELS(L, __VA_ARGS__)

/**
 ** @brief Choose the @a N<sup>th</sup> element in the remaining argument
 ** list.
 **
 ** Counting of elements starts at 0.
 **/
#define CHS(N, ...) SUB(N, 1, __VA_ARGS__)


#ifndef DOXYGEN
#define REP0(...)
#define REP1(X) X
#endif
/**
 ** @brief Concatenate the token @a X @a N times.
 **/
#define REP(N, X) P99__REP(N, X)
#define P99__REP(N, X) REP ## N(X)

#ifndef DOXYGEN
#define DUPL0(...)
#define DUPL1(...) __VA_ARGS__
#endif

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

#define P99__DUPL_0(...) P99__DUPL(__VA_ARGS__)
#define P99__DUPL_1(...)


#define P99__DUPL(N, ...) DUPL ## N(__VA_ARGS__)


/** @}
 **/


#endif 	    /* !P99_LIST_H_ */
