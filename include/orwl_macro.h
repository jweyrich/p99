/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef     ORWL_MACRO_H_
# define    ORWL_MACRO_H_

#include "p99_block.h"
#include "p99_defarg.h"
#include "orwl_document.h"

#define O_RWL_INV(N) P99_PASTE2(o_rwl_variable_argument_list_must_be_divisible_by_, N)

#define O_RWL_MODARG_(_X) P99_PASTE2(P00_NARG_,  _X)

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
 ** @brief Declare the types that are going to be used with a
 ** ::P99_LENGTH_VA_ARG or ::LEN_MODARG parameter list.
 **/
#define VA_TYPES(NAME, ...)   P99_TYPEDEFS(P99_PASTE2(NAME, _mod_type_), __VA_ARGS__)

#define O_RWL_VA_MODARG(AP, NAME, M, ...) va_arg(AP, P99_PASTE3(NAME, _mod_type_, M))

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
#define VA_MODARG(AP, ...) O_RWL_VA_MODARG(AP, __VA_ARGS__, 0, ~)

#define O_RWL_CAS1(NAME, X, N) P99_LVAL(P99_PASTE2(NAME, _mod_type_0), X)
#define O_RWL_CAS2(NAME, X, N) P99_LVAL(P99_PASTE3(NAME, _mod_type_, P99_MOD(N, 2)), X)
#define O_RWL_CAS3(NAME, X, N) P99_LVAL(P99_PASTE3(NAME, _mod_type_, P99_MOD(N, 3)), X)
#define O_RWL_CAS4(NAME, X, N) P99_LVAL(P99_PASTE3(NAME, _mod_type_, P99_MOD(N, 4)), X )
#define O_RWL_CAS5(NAME, X, N) P99_LVAL(P99_PASTE3(NAME, _mod_type_, P99_MOD(N, 5)), X )
#define O_RWL_MODARG_LIST(NAME, F, N, ...) P99_FOR(NAME, N, P00_SEQ, F, __VA_ARGS__)

#define LEN_MODARG(NAME, M, ...) O_RWL_MODARG_(M)(__VA_ARGS__), O_RWL_MODARG_LIST(NAME, P99_PASTE2(O_RWL_CAS, M), P99_NARG(__VA_ARGS__), __VA_ARGS__)

#endif      /* !ORWL_MACRO_H_ */
