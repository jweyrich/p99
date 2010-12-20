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

#if defined(__GNUC__) && !defined(GNUC_NO_SYNC)

p99_instantiate
size_t atomic_load(atomic_size_t volatile *object);

p99_instantiate
void atomic_store(atomic_size_t volatile *object, size_t desired);

p99_instantiate
_Bool atomic_compare_exchange_weak(atomic_size_t volatile *object, size_t *expected, size_t desired);

p99_instantiate
size_t atomic_fetch_add(atomic_size_t volatile *object, size_t operand);

p99_instantiate
size_t atomic_fetch_sub(atomic_size_t volatile *object, size_t operand);

p99_instantiate
size_t atomic_fetch_or(atomic_size_t volatile *object, size_t operand);

p99_instantiate
size_t atomic_fetch_xor(atomic_size_t volatile *object, size_t operand);

p99_instantiate
size_t atomic_fetch_and(atomic_size_t volatile *object, size_t operand);

#endif
