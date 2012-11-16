/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/* (C) copyright  2012 William Morris                                         */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_CLIB_H
#define P99_CLIB_H 1

#include "p99_new.h"

/* Additions by C11 */
# if __STDC_VERSION__ < 201100L

# include "p99_atomic.h"

/* Provide aligned allocation. */

# if (_XOPEN_SOURCE >= 600) || defined(P00_DOXYGEN)

/**
 ** @addtogroup C11_library C11 additions to the C library
 **
 ** @{
 **/

#define p00_has_feature_aligned_alloc 1
#define p00_has_extension_aligned_alloc 1
#define p00_has_feature_quick_exit 1
#define p00_has_extension_quick_exit 1
#if defined(P99_LIFO_POP) || defined(P00_DOXYGEN)
# define p00_has_feature_quick_exit_thread_safe 1
# define p00_has_extension_quick_exit_thread_safe 1
#endif

/**
 ** @brief allocation with a chosen alignment
 **
 ** @remark this implementation relies on the presence of @c posix_memalign from POSIX
 **/
p99_inline
void *aligned_alloc(size_t p00_alignment, size_t p00_size) {
  void * p00_ret = 0;
  int err = posix_memalign(&p00_ret, p00_alignment, p00_size);
  /* If there was an error and a fake pointer has been returned, free
     this pointer and set it to 0. This is the only way to return an
     error for this C11 interface. */
  if (err && p00_ret) {
    free(p00_ret);
    p00_ret = 0;
  }
  return p00_ret;
}

typedef struct p00_aqe_el p00_aqe_el;
P99_POINTER_TYPE(p00_aqe_el);
#ifndef P00_DOXYGEN
# if p99_has_feature(quick_exit_thread_safe)
P99_DECLARE_ATOMIC(p00_aqe_el_ptr);
typedef _Atomic(p00_aqe_el_ptr) p00_aqe_list;
# else
typedef p00_aqe_el_ptr p00_aqe_list;
# endif
#endif

typedef void p00_aqe_func(void);

struct p00_aqe_el {
  p00_aqe_el_ptr p99_lifo;
  p00_aqe_func * p00_func;
};


p99_inline
p00_aqe_el* p00_aqe_el_init(p00_aqe_el * p00_obj, p00_aqe_func* p00_func) {
  if (p00_obj) {
    *p00_obj = (p00_aqe_el) { .p00_func = p00_func };
  }
  return p00_obj;
}

p99_inline
p00_aqe_el* p00_at_quick_exit_top(p00_aqe_list* p00_l) {
#if p99_has_feature(quick_exit_thread_safe)
  return P99_LIFO_TOP(p00_l);
#else
  return *p00_l;
#endif
}

p99_inline
void p00_at_quick_exit_push(p00_aqe_list* p00_l, p00_aqe_el* p00_el) {
#if p99_has_feature(quick_exit_thread_safe)
  P99_LIFO_PUSH(p00_l, p00_el);
#else
  p00_el->p99_lifo = *p00_l;
  *p00_l = p00_el;
#endif
}

p99_inline
p00_aqe_el* p00_at_quick_exit_pop(p00_aqe_list* p00_l) {
#if p99_has_feature(quick_exit_thread_safe)
  return P99_LIFO_POP(p00_l);
#else
  p00_aqe_el *p00_el = p00_at_quick_exit_top(p00_l);
  if (p00_el) {
    *p00_l = p00_el->p99_lifo;
    p00_el->p99_lifo = 0;
  }
  return p00_el;
#endif
}

/* In both cases this is guaranteed to do the correct
   initialization. */
P99_WEAK(p00_aqe)
p00_aqe_list p00_at_quick_exit;

/**
 ** @brief registers the function pointed to by func, to be called
 ** without arguments should ::quick_exit be called.
 **/
p99_inline
int at_quick_exit(void (*p00_func)(void)) {
  int ret = 0;
  p00_aqe_el *el = P99_NEW(p00_aqe_el, p00_func);
  ret = !el;
  if (P99_LIKELY(!ret)) p00_at_quick_exit_push(&p00_at_quick_exit, el);
  return ret;
}

/**
 ** @brief causes normal program termination to occur.
 **
 ** No functions registered by the @c atexit function or signal
 ** handlers registered by the signal function are called.
 **
 ** @see at_quick_exit
 **/
p99_inline
_Noreturn void quick_exit(int status) {
  for (;;) {
    p00_aqe_el *el = p00_at_quick_exit_pop(&p00_at_quick_exit);
    if (P99_UNLIKELY(!el)) break;
    p00_aqe_func * p00_func = el->p00_func;
    free(el);
    p00_func();
  }
  _Exit(status);
}

P99_SETJMP_INLINE(p00_run_at_thrd_exit)
void p00_run_at_thrd_exit(void * li) {
  p00_aqe_list* list = li;
  for (;;) {
    p00_aqe_el *el = p00_at_quick_exit_pop(list);
    if (P99_UNLIKELY(!el)) break;
    p00_aqe_func * p00_func = el->p00_func;
    free(el);
    p00_func();
  }
}

P99_TSS_DECLARE_LOCAL(p00_aqe_list, p00_at_thrd_exit, p00_run_at_thrd_exit);
#define P00_AT_THRD_EXIT P99_TSS_LOCAL(p00_at_thrd_exit)

/**
 ** @brief Add @a p00_func to the functions that are called on exit of
 ** the current thread.
 **
 ** This is an extension of the C11 thread functions and works
 ** analogously to the functions ::at_exit and ::at_quick exit.
 **/
p99_inline
int at_thrd_exit(void (*p00_func)(void)) {
  int ret = 0;
  p00_aqe_el *el = P99_NEW(p00_aqe_el, p00_func);
  ret = !el;
  if (P99_LIKELY(!ret)) p00_at_quick_exit_push(&P00_AT_THRD_EXIT, el);
  return ret;
}

/**
 ** @}
 **/

#  endif /* XOPEN */
# endif /* STDC < 2011 */
#endif
