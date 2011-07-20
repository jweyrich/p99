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

#include "orwl_atomic.h"
#include "p99_defarg.h"

#if defined(__GNUC__)

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
#define ORWL_MANG(NAME) ORWL_MANG_(NAME, 64)
#define ORWL_AT ORWL_AT_(64)
#define ORWL_RT ORWL_RT_(64)
#include "orwl_atomic_gcc_sync_inttypes_instantiate.h"
#undef ORWL_RT
#undef ORWL_AT
#undef ORWL_MANG
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define ORWL_MANG(NAME) ORWL_MANG_(NAME, 32)
#define ORWL_AT ORWL_AT_(32)
#define ORWL_RT ORWL_RT_(32)
#include "orwl_atomic_gcc_sync_inttypes_instantiate.h"
#undef ORWL_RT
#undef ORWL_AT
#undef ORWL_MANG
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
#define ORWL_MANG(NAME) ORWL_MANG_(NAME, 16)
#define ORWL_AT ORWL_AT_(16)
#define ORWL_RT ORWL_RT_(16)
#include "orwl_atomic_gcc_sync_inttypes_instantiate.h"
#undef ORWL_RT
#undef ORWL_AT
#undef ORWL_MANG
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
#define ORWL_MANG(NAME) ORWL_MANG_(NAME, 8)
#define ORWL_AT ORWL_AT_(8)
#define ORWL_RT ORWL_RT_(8)
#include "orwl_atomic_gcc_sync_inttypes_instantiate.h"
#undef ORWL_RT
#undef ORWL_AT
#undef ORWL_MANG
#endif

#ifndef atomic_load
P99_INSTANTIATE(size_t, atomic_load, atomic_size_t volatile*);
#endif

#ifndef atomic_store
P99_INSTANTIATE(void, atomic_store, atomic_size_t volatile *object, size_t desired);
#endif

#ifndef atomic_compare_exchange_weak
P99_INSTANTIATE(_Bool, atomic_compare_exchange_weak, atomic_size_t volatile *object, size_t *expected, size_t desired);
#endif

#ifndef atomic_fetch_add
P99_INSTANTIATE(size_t, atomic_fetch_add, atomic_size_t volatile *object, size_t operand);
#endif

#ifndef atomic_fetch_sub
P99_INSTANTIATE(size_t, atomic_fetch_sub, atomic_size_t volatile *object, size_t operand);
#endif

#ifndef atomic_fetch_or
P99_INSTANTIATE(size_t, atomic_fetch_or, atomic_size_t volatile *object, size_t operand);
#endif

#ifndef atomic_fetch_xor
P99_INSTANTIATE(size_t, atomic_fetch_xor, atomic_size_t volatile *object, size_t operand);
#endif

#ifndef atomic_fetch_and
P99_INSTANTIATE(size_t, atomic_fetch_and, atomic_size_t volatile *object, size_t operand);
#endif

DEFINE_ATOMIC_OPS(atomic_float);
DEFINE_ATOMIC_OP(atomic_float, add, +=);
DEFINE_ATOMIC_OP(atomic_float, minus, -=);
DEFINE_ATOMIC_OP(atomic_float, mult, *=);
DEFINE_ATOMIC_OP(atomic_float, div, /=);

#endif
