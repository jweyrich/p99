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

#if UINTPTR_MAX == UINT32_MAX
typedef uint64_t p00_lifo_uint;
#else
# if defined(UINT128_MAX)
typedef uint128_t p00_lifo_uint;
# else
typedef p99x_uint128 p00_lifo_uint;
# endif
#endif

P99_CONSTANT(int, p00_lifo_bits, sizeof(p00_lifo_uint)*CHAR_BIT);
P99_CONSTANT(int, p00_lifo_shift, p00_lifo_bits/2);

#if defined(P99_DECLARE_ATOMIC) || P00_DOXYGEN
P99_DECLARE_ATOMIC(p00_lifo_uint);

# define P99_LIFO(T) P99_PASTE2(p00_lifo_, T)
# define P99_LIFO_DECLARE(T)                                            \
typedef struct P99_PASTE2(p00_lifo_, T) P99_PASTE2(p00_lifo_, T);       \
_Alignas(sizeof(p00_lifo_uint)) struct P99_PASTE2(p00_lifo_, T) {       \
  _Atomic(p00_lifo_uint) p00_val;                                       \
  _Atomic(uintptr_t) p00_tic;                                           \
  T p00_dum; /* we only need this for its type */                       \
}

# define P99_LIFO_INITIALIZER(VAL) {                    \
    .p00_val = ATOMIC_VAR_INIT((uintptr_t)(void*)VAL),  \
      .p00_tic = ATOMIC_VAR_INIT(UINTPTR_C(1)),         \
}

p99_inline
void * p00_lifo_i2p(uintptr_t v) {
  return (void*)v;
}

p99_inline
p00_lifo_uint p00_lifo_p2i(void * p, p00_lifo_uint t) {
  return (t<<p00_lifo_shift)|(uintptr_t)p;
}

p99_inline
bool p00_lifo_cmpxchg(_Atomic(p00_lifo_uint)* p00_p, p00_lifo_uint* p00_prev, p00_lifo_uint p00_new) {
  P99_MARK("wide cmpxchg start");
  bool ret = atomic_compare_exchange_weak(p00_p, p00_prev, p00_new);
  P99_MARK("wide cmpxchg end");
  return ret;
}


/**
 ** @brief Return a pointer to the top element of an atomic LIFO @a L
 ** @see P99_LIFO_CLEAR
 ** @see P99_LIFO_POP
 ** @see P99_LIFO_PUSH
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_TOP, 0)
#define P99_LIFO_TOP(L)                                                 \
({                                                                      \
  register const P99_MACRO_VAR(p00_l, (L));                             \
  /* be sure that the result can not be used as an lvalue */            \
  register const __typeof__(p00_l->p00_dum) p00_r = p00_lifo_i2p(atomic_load(&p00_l->p00_val));    \
  p00_r;                                                                \
 })


/**
 ** @brief Push element @a EL into an atomic LIFO @a L
 ** @see P99_LIFO_CLEAR
 ** @see P99_LIFO_POP
 ** @see P99_LIFO_TOP
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_PUSH, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_PUSH, 1)
#define P99_LIFO_PUSH(L, EL)                                            \
p99_extension                                                           \
({                                                                      \
  register const P99_MACRO_VAR(p00_el, (EL));                           \
  register const P99_MACRO_VAR(p00_l, (L));                             \
  register const P99_MACRO_VAR(p00_p, &p00_l->p00_val);                 \
  register const uintptr_t p00_tic = atomic_fetch_add(&p00_l->p00_tic, 1); \
  p00_lifo_uint p00_prev = atomic_load(p00_p);                          \
  do {                                                                  \
    p00_el->p99_lifo = p00_lifo_i2p(p00_prev);                          \
  } while (!p00_lifo_cmpxchg(p00_p, &p00_prev, p00_lifo_p2i(p00_el, p00_tic))); \
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
 ** P99_FIFO_DECLARE(myData_ptr);
 **
 ** struct myData {
 **   ...
 **   myData_ptr p99_lifo;
 **   ...
 ** };
 ** extern P99_LIFO(myData_ptr) head;
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
 ** @see P99_LIFO
 ** @see P99_LIFO_DECLARE
 ** @see P99_LIFO_PUSH
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_POP, 0)
#define P99_LIFO_POP(L)                                                 \
p99_extension                                                           \
({                                                                      \
  register const P99_MACRO_VAR(p00_l, (L));                             \
  register const P99_MACRO_VAR(p00_p, &p00_l->p00_val);                 \
  register const uintptr_t p00_tic = atomic_fetch_add(&p00_l->p00_tic, 1); \
  p00_lifo_uint p00_el = atomic_load(p00_p);                            \
  /* be sure that the result can not be used as an lvalue */            \
  register __typeof__(p00_l->p00_dum) p00_r = p00_lifo_i2p(p00_el);     \
  for (; p00_r; p00_r = p00_lifo_i2p(p00_el)) {                         \
    if (p00_lifo_cmpxchg(p00_p, &p00_el, p00_lifo_p2i(p00_r->p99_lifo, p00_tic))) \
      break;                                                            \
  }                                                                     \
  if (p00_r) p00_r->p99_lifo = 0;                                       \
  p00_r;                                                                \
})

#define P00_LIFO_REVERT(L)                                     \
p99_extension                                                  \
({                                                             \
  register P99_MACRO_VAR(p00_h, (L));                          \
  register P99_MACRO_VAR(p00_t, P99_PROMOTE_0(p00_h));         \
  while (p00_h) {                                              \
    register P99_MACRO_VAR(p00_n, p00_h->p99_lifo);            \
    p00_h->p99_lifo = p00_t;                                   \
    p00_h = p00_n;                                             \
  }                                                            \
  /* make sure that the result can not be used as an lvalue */ \
  register const __typeof__(p00_t = p00_t) p00_r = p00_t;      \
  p00_r;                                                       \
})

/**
 ** @brief Atomically clear an atomic LIFO @a L and return a pointer
 ** to the start of the list that it previously contained
 **
 ** @see P99_LIFO
 ** @see P99_LIFO_DECLARE
 ** @see P99_LIFO_PUSH
 ** @see P99_LIFO_TOP
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_LIFO_CLEAR, 0)
#define P99_LIFO_CLEAR(L)                                               \
p99_extension                                                           \
({                                                                      \
  register const P99_MACRO_VAR(p00_l, (L));                             \
  register const P99_MACRO_VAR(p00_p, &p00_l->p00_val);                 \
  p00_lifo_uint p00_el = atomic_load(p00_p);                            \
  while ((uintptr_t)p00_el && !atomic_compare_exchange_weak(p00_p, &p00_el, (p00_lifo_uint)0)); \
  /* be sure that the result can not be used as an lvalue */            \
  register const __typeof__(p00_l->p00_dum) p00_r = p00_lifo_i2p(p00_el);     \
  p00_r;                                                                \
})

#else

/* A fall back implementation for the case that there are no atomic
   operations available */

# define P99_LIFO(T) P99_PASTE2(p00_lifo_, T)
# define P99_LIFO_DECLARE(T) typedef T P99_LIFO(T)
# define P99_LIFO_INITIALIZER(VAL) ((void*)VAL)

#define P99_LIFO_TOP(L)  (*(L))

#define P99_LIFO_PUSH(L, EL)                                   \
p99_extension                                                  \
({                                                             \
  P99_MACRO_VAR(p00_l, (L));                                   \
  P99_MACRO_VAR(p00_el, (EL));                                 \
  p00_el->p99_lifo = *p00_l;                                   \
  *p00_l = p00_el;                                             \
})

#define P99_LIFO_POP(L)                                        \
p99_extension                                                  \
({                                                             \
  P99_MACRO_VAR(p00_l, (L));                                   \
  P99_MACRO_VAR(p00_el, *p00_l);                               \
  *p00_l = p00_el->p99_lifo;                                   \
  if (p00_el) p00_el->p99_lifo = 0;                            \
  /* be sure that the result can not be used as an lvalue */   \
  register __typeof__(p00_el = p00_el) p00_r = p00_el;         \
  p00_r;                                                       \
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
#define P99_LIFO_CLEAR(L)                                      \
({                                                             \
  P99_MACRO_VAR(p00_l, (L));                                   \
  register P99_MACRO_VAR(p00_ret, *p00_l);                     \
  *p00_l = 0;                                                  \
  p00_ret;                                                     \
})

#endif

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
