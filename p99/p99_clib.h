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
#ifndef P99_CLIB_H
#define P99_CLIB_H 1

#include "p99_for.h"
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
#if defined(ATOMIC_FLAG_INIT) || defined(P00_DOXYGEN)
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
typedef struct p00_aqe_list p00_aqe_list;
typedef void p00_aqe_func(void);

struct p00_aqe_el {
  p00_aqe_el * next;
  p00_aqe_func * func;
};

p99_inline
p00_aqe_el* p00_aqe_el_init(p00_aqe_el * obj, p00_aqe_el * next, p00_aqe_func* func) {
  if (obj) {
    *obj = (p00_aqe_el){ .next = next, .func = func };
  }
  return obj;
}

struct p00_aqe_list {
#if p99_has_feature(quick_exit_thread_safe)
  atomic_flag cat;
#endif
  p00_aqe_el * head;
};

P99_WEAK(p00_aqe)
p00_aqe_list p00_at_quick_exit = { .cat = ATOMIC_FLAG_INIT, .head = 0 };

p99_inline
int at_quick_exit(void (*func)(void)) {
  int ret = 0;
#if p99_has_feature(quick_exit_thread_safe)
  P99_SPIN_EXCLUDE(&p00_at_quick_exit.cat) {
#endif
    p00_aqe_el *el = P99_NEW(p00_aqe_el, p00_at_quick_exit.head, func);
    ret = !el;
    if (!ret) p00_at_quick_exit.head = el;
#if p99_has_feature(quick_exit_thread_safe)
  }
#endif
  return ret;
}

p99_inline
_Noreturn void quick_exit(int status) {
  for (;;) {
    p00_aqe_func * func = 0;
#if p99_has_feature(quick_exit_thread_safe)
    P99_SPIN_EXCLUDE(&p00_at_quick_exit.cat) {
#endif
      p00_aqe_el *el = p00_at_quick_exit.head;
      if (el) {
        func = el->func;
        p00_at_quick_exit.head = el->next;
        free(el);
      }
#if p99_has_feature(quick_exit_thread_safe)
    }
#endif
    if (!func) break;
    func();
  }
  _Exit(status);
}

/**
 ** @}
 **/

#  endif /* XOPEN */
# endif /* STDC < 2011 */
#endif
