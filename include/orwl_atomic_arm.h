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
 ** @brief ARM specific code for atomic operations.
 **
 ** Don't include this file directly, but use "orwl_atomic.h",
 ** instead.
 **/

#ifndef ORWL_ATOMIC_ARM_H
#define ORWL_ATOMIC_ARM_H

p99_inline
ATOMIC_TYPE arm_ldrex(ATOMIC_TYPE volatile*ptr) {
  ATOMIC_TYPE ret;
  __asm__ volatile ("ldrex %0,[%1]\t@ load exclusive\n"
                    : "=&r" (ret)
                    : "r" (ptr)
                    : "cc", "memory"
                    );
  return ret;
}

p99_inline
_Bool arm_strex(ATOMIC_TYPE volatile*ptr, ATOMIC_TYPE val) {
  ATOMIC_TYPE error;
  __asm__ volatile ("strex %0,%1,[%2]\t@ store exclusive\n"
                    : "=&r" (error)
                    : "r" (val), "r" (ptr)
                    : "cc", "memory"
                    );
  return !error;
}

inline
size_t atomic_load(atomic_size_t volatile *object) {
  size_t val = arm_ldrex(object);
  /* Cancel */
  arm_strex(object, val);
  return val;
}

inline
void atomic_store(atomic_size_t volatile *object, size_t desired) {
  do {
    (void)arm_ldrex(object);
  } while(!arm_strex(object, desired));
}


/**
 ** @brief Store a consistent value @a desired in @a object if the
 ** previous value corresponds to @a expected.
 **
 ** If the operation fails, the actual value is not changed and that
 ** value is returned in @a expected.
 **/
inline
_Bool atomic_compare_exchange_weak(atomic_size_t volatile *object, size_t *expected, size_t desired) {
  size_t oldval = arm_ldrex(object);
  if (oldval == *expected) {
    if (arm_strex(object, desired)) return true;
  }
  else {
    /* Cancel */
    arm_strex(object, oldval);
    *expected = oldval;
  }
  return false;
}

inline
size_t atomic_fetch_add(atomic_size_t volatile *object, size_t operand) {
  for (;;) {
    size_t oldval = arm_ldrex(object);
    size_t newval = oldval + operand;
    if (arm_strex(object, newval)) return oldval;
  }
}

/**
 ** @brief Substract value @a operand from the value in @a object and
 ** return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret - operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_sub(atomic_size_t volatile *object, size_t operand) {
  for (;;) {
    size_t oldval = arm_ldrex(object);
    size_t newval = oldval - operand;
    if (arm_strex(object, newval)) return oldval;
  }
}


/**
 ** @brief Bitwise or value @a operand with the value in @a object and
 ** return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret | operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_or(atomic_size_t volatile *object, size_t operand) {
  for (;;) {
    size_t oldval = arm_ldrex(object);
    size_t newval = oldval | operand;
    if (arm_strex(object, newval)) return oldval;
  }
}


/**
 ** @brief Bitwise exclusive or value @a operand with the value in @a
 ** object and return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret ^ operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_xor(atomic_size_t volatile *object, size_t operand) {
  for (;;) {
    size_t oldval = arm_ldrex(object);
    size_t newval = oldval ^ operand;
    if (arm_strex(object, newval)) return oldval;
  }
}


/**
 ** @brief Bitwise and value @a operand with the value in @a
 ** object and return the previous value.
 **
 ** This is the atomic equivalent to
 ** @code
 ** size_t ret = *object;
 ** *object = ret & operand;
 ** return ret;
 ** @endcode
 **/
inline
size_t atomic_fetch_and(atomic_size_t volatile *object, size_t operand) {
  for (;;) {
    size_t oldval = arm_ldrex(object);
    size_t newval = oldval & operand;
    if (arm_strex(object, newval)) return oldval;
  }
}

#endif
