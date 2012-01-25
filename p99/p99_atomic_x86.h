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
#ifndef P99_ATOMIC_X86_H
#define P99_ATOMIC_X86_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif

/**
 ** @addtogroup atomic_x86 Atomic operations on i386 and x86_64
 **
 ** This implementation uses the @c xchgl instruction that is
 ** implemented since the early days of i386.
 **
 ** This instruction has an implicit lock prefix and no further OS
 ** support is needed to make it lock-free.
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
uint32_t p00_sync_lock_test_and_set(uint32_t volatile *p00_objp) {
  register uint32_t p00_ret = 1;
  __asm__ __volatile__("xchgl %1, %0"
                       : "=r"(p00_ret)
                       : "m"(*p00_objp), "0"(p00_ret)
                       : "memory");
  return p00_ret;
}

p99_inline
void p00_sync_lock_release(uint32_t volatile *p00_objp) {
  __asm__ __volatile__("movl $0, %0"
                       :
                       : "m"(*p00_objp)
                       : "memory");
}

p99_inline
void p00_mfence(void) {
  __asm__ __volatile__("mfence":::"memory");
}



/**
 ** @}
 **/

#endif
