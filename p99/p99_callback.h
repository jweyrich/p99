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
#ifndef P99_CALLBACK_H
#define P99_CALLBACK_H 1

/* Additions by C11 */
# if __STDC_VERSION__ < 201100L
#  include "p99_atomic.h"
# endif

/**
 ** @addtogroup callbacks simple callbacks with or without @c void* argument
 **
 ** @{
 **/

#if defined(P99_LIFO_POP) || defined(P00_DOXYGEN)
# define p00_has_feature_callback_thread_safe 1
# define p00_has_extension_callback_thread_safe 1
#endif

typedef struct p00_cb_el p00_cb_el;
P99_POINTER_TYPE(p00_cb_el);
#ifndef P00_DOXYGEN
# if p99_has_feature(callback_thread_safe)
P99_DECLARE_ATOMIC(p00_cb_el_ptr);
typedef _Atomic(p00_cb_el_ptr) p99_callback_stack;
# else
typedef p00_cb_el_ptr p99_callback_stack;
# endif
#endif

typedef void p99_callback_void_func(void);
typedef void p99_callback_voidptr_func(void*);

struct p00_cb_el {
  p00_cb_el_ptr p99_lifo;
  p99_callback_voidptr_func * p00_voidptr_func;
  union {
    p99_callback_void_func * p00_void_func;
    void* p00_arg;
  };
};


p99_inline
p00_cb_el* p00_cb_el_init(p00_cb_el * p00_obj,
                          p99_callback_voidptr_func* p00_voidptr_func,
                          p99_callback_void_func* p00_void_func,
                          void* p00_arg
                         ) {
  if (p00_obj) {
    if (p00_voidptr_func)
      *p00_obj = (p00_cb_el) {
      .p00_voidptr_func = p00_voidptr_func,
       .p00_arg = p00_arg,
    };
    else
      *p00_obj = (p00_cb_el) { .p00_void_func = p00_void_func };
  }
  return p00_obj;
}

#define p00_cb_el_init_1(FUNC)                                                \
P99_GENERIC((&*FUNC),                                                         \
            ,                                                                 \
            (p99_callback_voidptr_func*, (p99_callback_voidptr_func*)(FUNC)), \
            (p99_callback_void_func*, (p99_callback_voidptr_func*)0)          \
            )


#define p00_cb_el_init_2(FUNC)                                          \
P99_GENERIC((&*FUNC),                                                   \
            ,                                                           \
            (p99_callback_void_func*, (p99_callback_void_func*)(FUNC)), \
            (p99_callback_voidptr_func*, (p99_callback_void_func*)0)    \
            )

#define p00_cb_el_init_(OBJ, FUNC, ARG)                                    \
p00_cb_el_init((OBJ), p00_cb_el_init_1(FUNC), p00_cb_el_init_2(FUNC), ARG)


#define p00_cb_el_init(...)                                    \
P99_IF_LT(P99_NARG(__VA_ARGS__), 3)                            \
(p00_cb_el_init_(__VA_ARGS__, 0))                              \
(p00_cb_el_init_(__VA_ARGS__))

p99_inline
p00_cb_el* p00_callback_top(p99_callback_stack* p00_l) {
#if p99_has_feature(callback_thread_safe)
  return P99_LIFO_TOP(p00_l);
#else
  return *p00_l;
#endif
}

p99_inline
p00_cb_el* p00_callback_push(p99_callback_stack* p00_l, p00_cb_el* p00_el) {
  if (p00_el) {
#if p99_has_feature(callback_thread_safe)
    P99_LIFO_PUSH(p00_l, p00_el);
#else
    p00_el->p99_lifo = *p00_l;
    *p00_l = p00_el;
#endif
  }
  return p00_el;
}

p99_inline
p00_cb_el* p00_callback_pop(p99_callback_stack* p00_l) {
#if p99_has_feature(callback_thread_safe)
  return P99_LIFO_POP(p00_l);
#else
  p00_cb_el *p00_el = p00_callback_top(p00_l);
  if (p00_el) {
    *p00_l = p00_el->p99_lifo;
    p00_el->p99_lifo = 0;
  }
  return p00_el;
#endif
}

#define P99_CALLBACK_PUSH(STCK, ...) p00_callback_push((STCK), P99_NEW(p00_cb_el, __VA_ARGS__))


p99_inline
void p99_callback(p99_callback_stack* stck) {
  for (;;) {
    p00_cb_el *el = p00_callback_pop(stck);
    if (P99_UNLIKELY(!el)) break;
    p99_callback_voidptr_func * p00_voidptr_func = el->p00_voidptr_func;
    p99_callback_void_func * p00_void_func = el->p00_void_func;
    void * p00_arg = el->p00_arg;
    free(el);
    if (p00_voidptr_func) p00_voidptr_func(p00_arg);
    else p00_void_func();
  }
}


/**
 ** @}
 **/

#endif
