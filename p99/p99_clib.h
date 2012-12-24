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
#include "p99_callback.h"

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

/* In both cases this is guaranteed to do the correct
   initialization. */
P99_WEAK(p00_cb)
p99_callback_stack p00_at_quick_exit;

/**
 ** @brief registers the function pointed to by func, to be called
 ** without arguments should ::quick_exit be called.
 **/
p99_inline
int at_quick_exit(void (*p00_void_func)(void)) {
  int ret = 0;
  p00_cb_el *el = P99_NEW(p00_cb_el, p00_void_func);
  ret = !el;
  if (P99_LIKELY(!ret)) p99_callback_push(&p00_at_quick_exit, el);
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
  p99_callback_stack_call(&p00_at_quick_exit);
  _Exit(status);
}

P99_SETJMP_INLINE(p00_run_at_thrd_exit)
void p00_run_at_thrd_exit(void * li) {
  p99_callback_stack_call(li);
}

P99_TSS_DECLARE_LOCAL(p99_callback_stack, p00_at_thrd_exit, p00_run_at_thrd_exit);
#define P00_AT_THRD_EXIT P99_TSS_LOCAL(p00_at_thrd_exit)

/**
 ** @brief Add @a p00_void_func to the functions that are called on exit of
 ** the current thread.
 **
 ** This is an extension of the C11 thread functions and works
 ** analogously to the functions ::at_exit and ::at_quick exit.
 **/
p99_inline
int at_thrd_exit(void (*p00_void_func)(void)) {
  int ret = 0;
  p00_cb_el *el = P99_NEW(p00_cb_el, p00_void_func);
  ret = !el;
  if (P99_LIKELY(!ret)) p99_callback_push(&P00_AT_THRD_EXIT, el);
  return ret;
}

/**
 ** @}
 **/

#  endif /* XOPEN */
# endif /* STDC < 2011 */
#endif
