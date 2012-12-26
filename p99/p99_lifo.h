/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_LIFO_H
#define P99_LIFO_H 1

#include "p99_enum.h"
#include "p99_generic.h"

/* Additions by C11 */
# if __STDC_VERSION__ < 201100L
#  include "p99_atomic.h"
# endif


/**
 ** @addtogroup atomic C11 atomic operations
 ** @{
 **/

/**
 ** @brief Return a pointer to the top element of an atomic LIFO @a L
 ** @see P99_LIFO_CLEAR
 ** @see P99_LIFO_POP
 ** @see P99_LIFO_PUSH
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_TOP, 0)
#define P99_LIFO_TOP(L)  atomic_load(L)

/**
 ** @brief Push element @a EL into an atomic LIFO @a L
 ** @see P99_LIFO_CLEAR
 ** @see P99_LIFO_POP
 ** @see P99_LIFO_TOP
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_PUSH, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_PUSH, 1)
#define P99_LIFO_PUSH(L, EL)                                   \
p99_extension                                                  \
({                                                             \
  P99_MACRO_VAR(p00_l, (L));                                   \
  P99_MACRO_VAR(p00_el, (EL));                                 \
  p00_el->p99_lifo = atomic_exchange(p00_l, p00_el);           \
 })

/**
 ** @brief Pop the top element from an atomic LIFO @a L
 **
 ** This implements a generic interface to an atomic LIFO (Last In -
 ** First Out) data structure. To use it you just have do some
 ** preparatory declarations and add a @c p99_lifo field to your data
 ** structure:
 **
 ** @code
 ** P99_DECLARE_STRUCT(myData);
 ** P99_POINTER_TYPE(myData);
 ** P99_DECLARE_ATOMIC(myData_ptr);
 **
 ** struct myData {
 **   ...
 **   myData_ptr p99_lifo;
 **   ...
 ** };
 ** extern _Atomic(myData_ptr) head;
 ** @endcode
 **
 ** Now @c head can be used as the head of a LIFO:
 **
 ** @code
 ** myData_ptr el = P99_NEW(myData, \/\* your initializer arguments \*\/);
 ** P99_LIFO_PUSH(&head, el);
 ** ...
 ** for (myData_ptr el = P99_LIFO_POP(&head);
 **      el;
 **      el = P99_LIFO_POP(&head)) {
 **        // do something with el and then
 **        free(el);
 ** }
 ** @endcode
 **
 ** @see P99_LIFO_CLEAR
 ** @see P99_LIFO_PUSH
 ** @see P99_LIFO_TOP
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_POP, 0)
#define P99_LIFO_POP(L)                                                                            \
p99_extension                                                                                      \
({                                                                                                 \
  P99_MACRO_VAR(p00_l, (L));                                                                       \
  __typeof__(P99_LIFO_TOP(p00_l)) p00_el = P99_LIFO_TOP(p00_l);                                    \
  if (P99_LIKELY(p00_el)) {                                                                        \
    while (P99_UNLIKELY(!atomic_compare_exchange_weak(p00_l, &p00_el, p00_el->p99_lifo))) P99_NOP; \
    p00_el->p99_lifo = 0;                                                                          \
  }                                                                                                \
  /* be sure that the result can not be used as an lvalue */                                       \
  register __typeof__(p00_el = p00_el) p00_r = p00_el;                                             \
  p00_r;                                                                                           \
 })

/**
 ** @brief Atomically clear an atomic LIFO @a L and return a pointer
 ** to the start of the list that it previously contained
 **
 ** @see P99_LIFO_POP
 ** @see P99_LIFO_PUSH
 ** @see P99_LIFO_TOP
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_CLEAR, 0)
#define P99_LIFO_CLEAR(L) atomic_exchange(L, 0)

P00_DOCUMENT_TYPE_ARGUMENT(P99_LIFO_TABULATE, 0)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_LIFO_TABULATE, 1)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_TABULATE, 2)
#define P99_LIFO_TABULATE(TYPE, TAB, L)                        \
size_t P99_FILEID(TAB, _cnt) = 0;                              \
TYPE * P99_FILEID(TAB, _head) = P99_LIFO_CLEAR(L);             \
for (TYPE * p00_e = P99_FILEID(TAB, _head);                    \
     p00_e;                                                    \
     p00_e = p00_e->p99_lifo)                                  \
  ++P99_FILEID(TAB, _cnt);                                     \
TYPE * TAB[P99_FILEID(TAB, _cnt)];                             \
for (TYPE ** p00_t = &(TAB[0]),                                \
       * p00_e = P99_FILEID(TAB, _head);                       \
     p00_e;                                                    \
     p00_e = p00_e->p99_lifo,                                  \
       ++p00_t)                                                \
  *p00_t = p00_e

/**
 ** @}
 **/

#endif
