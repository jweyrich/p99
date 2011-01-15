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
/**
 ** @file
 ** @brief x86 specific code for atomic operations.
 **
 ** Don't include this file directly, but use "orwl_atomic.h",
 ** instead.
 **/

#ifndef ORWL_ATOMIC_x86_H
#define ORWL_ATOMIC_x86_H

#if ATOMIC_SIZE == 8
#define x86_cmpxchg_reg rax
/**
 ** @brief Store a consistent value @a desired in @a object if the
 ** previous value corresponds to @a expected.
 **
 ** Compares @a expected with <code>*object</code>.
 ** If they are equal, stores desired in <code>*object</code>.
 **
 ** @return the value that was initially in <code>*object</code>.
 **
 ** This is a x86 specific function that should not be used directly
 ** in user code. Use ::atomic_compare_exchange_weak instead.
 **
 ** The particularity of the underlying assembler instruction is that
 ** it uses registers EAX or RAX, respectively, as input and output
 ** register. The implementation of the atomic functions for this
 ** architecture all take care to have already the corresponding value
 ** in exactly that register.
 **/
p99_inline
ATOMIC_TYPE cmpxchg(register ATOMIC_TYPE volatile *object, register size_t expected, register size_t desired) {
  register ATOMIC_TYPE ret P99_FIXED_REGISTER(x86_cmpxchg_reg) = expected;
  __asm__ __volatile__("lock cmpxchgq %1, %2"
                       : "=a"(ret)
                       : "r"(desired), "m"(*object), "0"(ret)
                       : "memory");
  return ret;
}
#else
#define x86_cmpxchg_reg eax
p99_inline
ATOMIC_TYPE cmpxchg(register ATOMIC_TYPE volatile *object, register size_t expected, register size_t desired) {
  register ATOMIC_TYPE ret P99_FIXED_REGISTER(x86_cmpxchg_reg) = expected;
  __asm__ __volatile__("lock cmpxchgl %1, %2"
                       : "=a"(ret)
                       : "r"(desired), "m"(*object), "0"(ret)
                       : "memory");
  return ret;
}
#endif

inline
_Bool atomic_compare_exchange_weak(register atomic_size_t volatile *object, register size_t *expected, register size_t desired) {
  register size_t oldval = *expected;
  register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = oldval;
  expect = cmpxchg(object, expect, desired);
  if (expect == oldval) return true;
  else {
    *expected = expect;
    return false;
  }
}

inline
size_t atomic_load(register atomic_size_t volatile *object) {
  return *object;
}

inline
void atomic_store(register atomic_size_t volatile *object, register size_t desired) {
  for (register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = atomic_load(object);;) {
    register size_t oldval = expect;
    expect = cmpxchg(object, expect, desired);
    if (expect == oldval) break;
  }
}


inline
size_t atomic_fetch_add(register atomic_size_t volatile *object, register size_t operand) {
  for (register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = atomic_load(object);;) {
    register size_t desired = expect + operand;
    register size_t oldval = expect;
    expect = cmpxchg(object, expect, desired);
    if (expect == oldval) return oldval;
  }
}

inline
size_t atomic_fetch_sub(register atomic_size_t volatile *object, register size_t operand) {
  for (register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = atomic_load(object);;) {
    register size_t desired = expect - operand;
    register size_t oldval = expect;
    expect = cmpxchg(object, expect, desired);
    if (expect == oldval) return oldval;
  }
}


inline
size_t atomic_fetch_or(register atomic_size_t volatile *object, register size_t operand) {
  for (register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = atomic_load(object);;) {
    register size_t desired = expect | operand;
    register size_t oldval = expect;
    expect = cmpxchg(object, expect, desired);
    if (expect == oldval) return oldval;
  }
}


inline
size_t atomic_fetch_xor(register atomic_size_t volatile *object, register size_t operand) {
  for (register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = atomic_load(object);;) {
    register size_t desired = expect ^ operand;
    register size_t oldval = expect;
    expect = cmpxchg(object, expect, desired);
    if (expect == oldval) return oldval;
  }
}


inline
size_t atomic_fetch_and(register atomic_size_t volatile *object, register size_t operand) {
  for (register size_t expect P99_FIXED_REGISTER(x86_cmpxchg_reg) = atomic_load(object);;) {
    register size_t desired = expect & operand;
    register size_t oldval = expect;
    expect = cmpxchg(object, expect, desired);
    if (expect == oldval) return oldval;
  }
}

#endif
