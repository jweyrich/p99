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
#ifndef   	P99_LIST_H_
# define   	P99_LIST_H_

/**
 ** @file
 ** @brief Basis macros to process the parameter list of a variadic
 ** macro.
 ** @see basic_list_operations
 **/

#include "p99_generated.h"

#define P00_PRE0(...)
#define P00_PRE1(_0, ...) _0

/** @addtogroup list_processing List processing macros
 ** @brief We provide here a series of macros that take a list of
 ** arguments of arbitrary length and that transform each element in
 ** that list in some way.
 **/

/** @addtogroup basic_list_operations Basic list processing macros
 ** 
 ** @{
 **/

/**
 ** Cut the argument list at position @a N
 **/
#define P99_SELS(N, ...) P99_PASTE2(P00_PRE, N)(__VA_ARGS__)

#define P00_SKP0(...) __VA_ARGS__
#define P00_SKP1(_0, ...) __VA_ARGS__


/**
 ** @brief Skip @a N elements in the remaining argument list.
 **/
#define P99_SKP(N, ...) P99_PASTE2(P00_SKP, N)(__VA_ARGS__)

/**
 ** @brief Get the sublist of length @a L starting at the @a
 ** N<sup>th</sup> element in the remaining argument list.
 **
 ** Counting of elements starts at 0.
 **/
#define P99_SUB(N, L, ...) P00_SUB(L, P99_SKP(N, __VA_ARGS__))
#define P00_SUB(L, ...) P99_SELS(L, __VA_ARGS__)

/**
 ** @brief Choose the @a N<sup>th</sup> element in the remaining argument
 ** list.
 **
 ** Counting of elements starts at 0.
 **/
#define P99_CHS(N, ...) P99_SUB(N, 1, __VA_ARGS__)

#ifndef P00_DOXYGEN
#define P99_DUPL0(...)
#define P99_DUPL1(...) __VA_ARGS__
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
#define P99_DUPL(...) P99_PASTE2(P00_DUPL_, P99_IS_DEC_LT(P99_NARG(__VA_ARGS__), 2))(__VA_ARGS__)

#define P00_DUPL_0(...) P00_DUPL(__VA_ARGS__)
#define P00_DUPL_1(...)


#define P00_DUPL(N, ...) P99_DUPL ## N(__VA_ARGS__)


/** @}
 **/


#endif 	    /* !P99_LIST_H_ */
