/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_ATOMIC_ARM_H
#define P99_ATOMIC_ARM_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif

/**
 ** @addtogroup atomic_arm Atomic operations on ARM
 **
 ** This implementation uses the @c ldrex/strex instruction pair that is
 ** implemented since arm v6.
 **
 ** @c ldrex/strex is a load-link and store instruction pair. This means
 ** a strex will only succeed, if nobody else has touched "object"
 ** since the load.
 **
 ** This operation is not lock-free by itself, so it doesn't guarantee
 ** the requirements of the standard for operations on ::atomic_flag
 ** when used by its own. The OS must support this operation by
 ** canceling all pending @c ldrex operations when (de)scheduling a
 ** thread or entering or leaving a signal handler.
 **
 ** @remark This is only used if the gcc extensions for atomic lock
 ** free operations is not implemented directly by the compiler.
 **
 ** @remark Uses the @c __asm__ extension of gcc for inline assembler.
 **
 ** @ingroup atomic_stub
 **
 ** @{
 **/

p99_inline
uint32_t p00_arm_ldrex(uint32_t volatile*ptr) {
  uint32_t ret;
  __asm__ volatile ("ldrex %0,[%1]\t@ load exclusive\n"
                    : "=&r" (ret)
                    : "r" (ptr)
                    : "cc", "memory"
                   );
  return ret;
}

p99_inline
_Bool p00_arm_strex(uint32_t volatile*ptr, uint32_t val) {
  uint32_t ret;
  __asm__ volatile ("strex %0,%1,[%2]\t@ store exclusive\n"
                    : "=&r" (ret)
                    : "r" (val), "r" (ptr)
                    : "cc", "memory"
                   );
  return ret;
}

p99_inline
uint32_t p00_sync_lock_test_and_set(uint32_t volatile *object) {
  for (;;) {
    uint32_t ret = p00_arm_ldrex(object);
    /* Even if the result has been a 1 in ret, We must imperatively
       put a strex after the ldex since otherwise we would block other
       threads when they try to access this. On the other hand even if
       the strex doesn't succeed but ret is already set, we are also
       done. */
    if (!p00_arm_strex(object, 1) || ret) return ret;
  }
}

p99_inline
void p00_sync_lock_release(uint32_t volatile *object) {
  __sync_lock_release(object);
}

#elif defined(__x86_64__) || defined(__i386__)

p99_inline
uint32_t p00_sync_lock_test_and_set(uint32_t volatile *object) {
  register uint32_t ret P99_FIXED_REGISTER(eax) = 1;
  __asm__ __volatile__("xchgl %2, %0"
                       : "=a"(ret)
                       : "r"(ret), "m"(*object)
                       : "memory");
  return ret;
}

p99_inline
void p00_sync_lock_release(uint32_t volatile *object) {
  register uint32_t val = 0;
  __asm__ __volatile__("movl %0, %1"
                       :
                       : "r"(val), "m"(*object)
                       : "memory");
}

p99_inline
void p00_mfence(void) {
  __asm__ __volatile__("dmb":::"memory");
}

/**
 ** @}
 **/

#endif
