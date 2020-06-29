/* This may look like nonsense, but it really is -*- mode: C; coding: utf-8 -*- */
/*                                                                              */
/* Except for parts copied from previous work and as explicitly stated below,   */
/* the authors and copyright holders for this work are as follows:              */
/* (C) copyright  2012-2014 Jens Gustedt, INRIA, France                         */
/* (C) copyright  2012 William Morris                                           */
/*                                                                              */
/* This file is free software; it is part of the P99 project.                   */
/*                                                                              */
/* Licensed under the Apache License, Version 2.0 (the "License");              */
/* you may not use this file except in compliance with the License.             */
/* You may obtain a copy of the License at                                      */
/*                                                                              */
/*     http://www.apache.org/licenses/LICENSE-2.0                               */
/*                                                                              */
/* Unless required by applicable law or agreed to in writing, software          */
/* distributed under the License is distributed on an "AS IS" BASIS,            */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     */
/* See the License for the specific language governing permissions and          */
/* limitations under the License.                                               */
/*                                                                              */
#ifndef P99_ATOMIC_X86_H
#define P99_ATOMIC_X86_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif


p99_inline
uint8_t p00_atomic_exchange_1(uint8_t volatile* p00_objp, uint8_t p00_ret) {
  __asm__ __volatile__("xchgb %1, %b0"
                       : "=r"(p00_ret)
                       : "m"(*p00_objp), "0"(p00_ret)
                       : "memory");
  return p00_ret;
}

p99_inline
uint16_t p00_atomic_exchange_2(uint16_t volatile* p00_objp, uint16_t p00_ret) {
  __asm__ __volatile__("xchgw %1, %w0"
                       : "=r"(p00_ret)
                       : "m"(*p00_objp), "0"(p00_ret)
                       : "memory");
  return p00_ret;
}

p99_inline
uint32_t p00_atomic_exchange_4(uint32_t volatile* p00_objp, uint32_t p00_ret) {
  __asm__ __volatile__("xchgl %1, %k0"
                       : "=r"(p00_ret)
                       : "m"(*p00_objp), "0"(p00_ret)
                       : "memory");
  return p00_ret;
}

#if defined(__x86_64__) || defined(P00_DOXYGEN)
# if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) || defined(P00_DOXYGEN)
p99_inline
uint64_t p00_atomic_exchange_8(uint64_t volatile* p00_objp, uint64_t p00_ret) {
  __asm__ __volatile__("xchgq %1, %0"
                       : "=r"(p00_ret)
                       : "m"(*p00_objp), "0"(p00_ret)
                       : "memory");
  return p00_ret;
}
# endif
#else
#undef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
#endif

/**
 ** @addtogroup atomic_x86 Atomic operations on i386 and x86_64
 **
 ** This implementation uses the @c xchgl instruction that has been
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
void p00_sync_lock_release_internal(uint32_t volatile *p00_objp) {
  __asm__ __volatile__("movl $0, %0"
                       :
                       : "m"(*p00_objp)
                       : "memory");
}

p99_inline
void p00_mfence_internal(memory_order p00_ord) {
  __asm__ __volatile__("mfence":::"memory");
}

#define p00_mfence(...)                                        \
P99_IF_EMPTY(__VA_ARGS__)                                      \
 (p00_mfence_internal(memory_order_seq_cst))                   \
 (p00_mfence_internal(__VA_ARGS__))

#define p00_sync_lock_release(...)                              \
  P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                           \
  (p00_sync_lock_release_internal(__VA_ARGS__))                 \
  (p00_sync_lock_release_internal(P99_ALLBUTLAST(__VA_ARGS__)))
#define p00_sync_lock_test_and_set(...)                        \
  P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                          \
  (p00_atomic_exchange_4(__VA_ARGS__, 1))                      \
  (p00_atomic_exchange_4(P99_ALLBUTLAST(__VA_ARGS__), 1))



/**
 ** @}
 **/

#endif
