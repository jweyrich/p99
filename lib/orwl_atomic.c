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
/*
** orwl_atomic.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Dec 19 01:04:47 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_atomic.h"
#include "p99_defarg.h"

#if defined(__GNUC__)

P99_INSTANTIATE(size_t, atomic_load, atomic_size_t volatile*);

P99_INSTANTIATE(void, atomic_store, atomic_size_t volatile *object, size_t desired);

P99_INSTANTIATE(_Bool, atomic_compare_exchange_weak, atomic_size_t volatile *object, size_t *expected, size_t desired);

P99_INSTANTIATE(size_t, atomic_fetch_add, atomic_size_t volatile *object, size_t operand);

P99_INSTANTIATE(size_t, atomic_fetch_sub, atomic_size_t volatile *object, size_t operand);

P99_INSTANTIATE(size_t, atomic_fetch_or, atomic_size_t volatile *object, size_t operand);

P99_INSTANTIATE(size_t, atomic_fetch_xor, atomic_size_t volatile *object, size_t operand);

P99_INSTANTIATE(size_t, atomic_fetch_and, atomic_size_t volatile *object, size_t operand);

DEFINE_ATOMIC_OPS(atomic_float);
DEFINE_ATOMIC_OP(atomic_float, add, +=);
DEFINE_ATOMIC_OP(atomic_float, minus, -=);
DEFINE_ATOMIC_OP(atomic_float, mult, *=);
DEFINE_ATOMIC_OP(atomic_float, div, /=);

#endif
