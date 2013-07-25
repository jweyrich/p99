/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_RP_H
#define P99_RP_H 1

#include "p99_enum.h"
#include "p99_generic.h"

/* Additions by C11 */
# if __STDC_VERSION__ < 201100L
#  include "p99_atomic.h"
# endif

#ifdef P99_TP_NEED_INTEGER
#if UINTPTR_MAX == UINT32_MAX
typedef uint64_t p00_tp_state;
#else
# if defined(UINT128_MAX)
typedef uint128_t p00_tp_state;
# else
typedef p99x_uint128 p00_tp_state;
# endif
#endif

P99_CONSTANT(int, p00_tp_bits, sizeof(p00_tp_state)*CHAR_BIT);
P99_CONSTANT(int, p00_tp_shift, p00_tp_bits/2);

p99_inline
p00_tp_state p00_tp_p2i(void * p, uintptr_t t) {
  return (((p00_tp_state)t)<<p00_tp_shift)|(uintptr_t)p;
}

#define P00_TP_STATE_INITIALIZER(VAL) { (((p00_tp_state)VAL)<<p00_tp_shift)|(UINTPTR_C(1)) }


p99_inline
void * p00_tp_i2p(p00_tp_state v) {
  return (void*)(uintptr_t)v;
}

p99_inline
uintptr_t p00_tp_i2i(p00_tp_state v) {
  return v >> p00_tp_shift;
}

#else
P99_DECLARE_STRUCT(p00_tp_state);

struct p00_tp_state {
  uintptr_t p00_tag;
  void* p00_val;
};

#define P00_TP_STATE_INITIALIZER(VAL) { .p00_tag = 1, .p00_val = (VAL), }

p99_inline
p00_tp_state p00_tp_p2i(void * p00_val, uintptr_t p00_tag) {
  return (p00_tp_state) { .p00_tag = p00_tag, .p00_val = p00_val, };
}

p99_inline
void * p00_tp_i2p(p00_tp_state p00_sta) {
  return p00_sta.p00_val;
}

p99_inline
uintptr_t p00_tp_i2i(p00_tp_state p00_sta) {
  return p00_sta.p00_tag;
}
#endif

P99_DECLARE_ATOMIC(p00_tp_state);

P99_DECLARE_STRUCT(p99_tp);
P99_DECLARE_STRUCT(p99_tp_state);

p99_inline
p00_tp_state* p00_tp_state_init(p00_tp_state* el, void * p) {
  if (el) {
    *el = (p00_tp_state)P00_TP_STATE_INITIALIZER(p);
  }
  return el;
}

/**
 ** @brief A tagged pointer for atomic storage.
 **
 ** The idea of this data structure is to store a unique transaction
 ** ID together with a pointer such that different usages of the same
 ** pointer can be distinguished.
 **
 ** @see p99_tp_state
 **/
struct p99_tp {
  _Atomic(p00_tp_state) p00_val;
  _Atomic(uintptr_t) p00_tic;
};

/**
 ** @brief A local view of a tagged pointer for atomic storage.
 **
 ** This is to be used together with ::p99_tp.
 **
 ** @code
 ** p99_tp tp = P99_TP_INITIALIZER(0);
 ** ...
 ** void * fut = &(int){ 37 };
 ** p99_tp_state st = p99_tp_state_initializer(&tp, fut);
 ** void * val = p99_tp_state_get(&st);
 ** // do something with the current value, then
 ** // commit the new value "fut" if possible
 ** if (!p99_tp_state_commit(&st)) abort();
 ** @endcode
 **
 ** @see p99_tp_state_get
 ** @see p99_tp_state_set
 ** @see p99_tp_state_commit
 ** @see p99_tp_state_initializer
 **/
struct p99_tp_state {
  p00_tp_state p00_val;
  p00_tp_state p00_next;
  p99_tp* p00_tp;
};

# define P00_TP_INITIALIZER(VAL) {                             \
    .p00_val = ATOMIC_VAR_INIT(P00_TP_STATE_INITIALIZER(VAL)), \
      .p00_tic = ATOMIC_VAR_INIT(UINTPTR_C(1)),                \
}

p99_inline
p00_tp_state p00_tp_get(p99_tp* p00_tp) {
  return atomic_load(&p00_tp->p00_val);
}

/**
 ** @brief Load the value of @a p00_tp into the state variable and
 ** prepare it to commit value @a p00_p later.
 **/
p99_inline
p99_tp_state p99_tp_state_initializer(p99_tp* p00_tp, void* p00_p) {
  uintptr_t p00_tic = atomic_fetch_add(&p00_tp->p00_tic, UINTPTR_C(1));
  return (p99_tp_state) {
    .p00_val = p00_tp_get(p00_tp),
     .p00_next = p00_tp_p2i(p00_p, p00_tic),
      .p00_tp = p00_tp,
  };
}

p99_inline
void * p99_tp_state_get(p99_tp_state* p00_state) {
  return p00_tp_i2p(p00_state->p00_val);
}

p99_inline
void * p99_tp_get(p99_tp* p00_tp) {
  return p00_tp_i2p(p00_tp_get(p00_tp));
}

p99_inline
void p99_tp_state_set(p99_tp_state* p00_state, void* p00_p) {
  p00_state->p00_next = p00_tp_p2i(p00_p, p00_tp_i2i(p00_state->p00_next));
}

p99_inline
bool p00_tp_cmpxchg(_Atomic(p00_tp_state)* p00_p, p00_tp_state* p00_prev, p00_tp_state p00_new) {
  P99_MARK("wide cmpxchg start");
  bool ret = atomic_compare_exchange_weak(p00_p, p00_prev, p00_new);
  P99_MARK("wide cmpxchg end");
  return ret;
}

p99_inline
bool p99_tp_state_commit(p99_tp_state* p00_state) {
  return p00_tp_cmpxchg(&p00_state->p00_tp->p00_val, &p00_state->p00_val, p00_state->p00_next);
}


# define P99_TP(T) P99_PASTE2(p00_tp_, T)
# define P99_TP_STATE(T) P99_PASTE2(p00_tp_state_, T)
# define P99_TP_DECLARE(T)                                       \
typedef union P99_TP(T) P99_TP(T);                               \
typedef union P99_TP_STATE(T) P99_TP_STATE(T);                   \
union P99_TP(T) {                                                \
  p99_tp p00_tp;                                                 \
  T p00_dum;                /* we only need this for its type */ \
  P99_TP_STATE(T)* p00_mud; /* we only need this for its type */ \
  max_align_t p00_align;    /* ensure maximal alignment       */ \
};                                                               \
union P99_TP_STATE(T) {                                          \
  p99_tp_state p00_st;                                           \
  T p00_dum;             /* we only need this for its type */    \
  P99_TP(T)* p00_mud;    /* we only need this for its type */    \
  max_align_t p00_align; /* ensure maximal alignment       */    \
}

# define P99_TP_TYPE(TP) __typeof__(*(TP)->p00_dum)
# define P99_TP_TYPE_STATE(TP) __typeof__(*(TP)->p00_mud)
# define P99_TP_STATE_TYPE(TPS) __typeof__(*(TPS)->p00_dum)


# define P99_TP_INITIALIZER(VAL) { .p00_tp = P00_TP_INITIALIZER(VAL), }

p99_inline
void* p00_tp_init(p99_tp* p00_el, void* p00_val) {
  if (p00_el) {
    atomic_init(&p00_el->p00_val, (p00_tp_state){ .p00_val = p00_val });
    atomic_init(&p00_el->p00_tic, 1);
  }
  return p00_el;
}

# define p99_tp_init(EL, VAL) (p00_tp_init(&(EL)->p00_tp, (VAL)), (EL))

/**
 ** @brief Load the value of @a TP into the state variable and
 ** prepare it to commit value @a P later.
 **/
#define P99_TP_STATE_INITIALIZER(TP, P)                           \
p99_extension ({                                                  \
    P99_MACRO_VAR(p00_tp, (TP));                                  \
    /* ensure that P is assignment compatible to the */           \
    /* base type, and that the return can't be used as lvalue */  \
    register P99_TP_TYPE(p00_tp)* const p00_p = (P);              \
    register P99_TP_TYPE_STATE(p00_tp) const p00_r = {            \
      .p00_st = p99_tp_state_initializer(&p00_tp->p00_tp, p00_p), \
    };                                                            \
    p00_r;                                                        \
})

#define P99_TP_GET(TP)                                             \
p99_extension ({                                                   \
    P99_MACRO_VAR(p00_tp, (TP));                                   \
    /* ensure that pointer that is returned is converted to the */ \
    /* base type, and that the return can't be used as lvalue */   \
    register P99_TP_TYPE(p00_tp)* const p00_r                      \
      = p99_tp_get(&p00_tp->p00_tp);                               \
    p00_r;                                                         \
})

#define P99_TP_STATE_GET(TPS)                                      \
p99_extension ({                                                   \
    P99_MACRO_VAR(p00_tps, (TPS));                                 \
    /* ensure that pointer that is returned is converted to the */ \
    /* base type, and that the return can't be used as lvalue */   \
    register P99_TP_STATE_TYPE(p00_tps)* const p00_r               \
      = p99_tp_state_get(&p00_tps->p00_st);                        \
    p00_r;                                                         \
})

#define P99_TP_STATE_SET(TPS, P)                                 \
p99_extension ({                                                 \
    P99_MACRO_VAR(p00_tps, (TPS));                               \
    /* ensure that P is assignment compatible to the */          \
    /* base type, and that the return can't be used as lvalue */ \
    register P99_TP_STATE_TYPE(p00_tps)* p00_p = (P);            \
    p99_tp_state_set(&p00_tps->p00_st, p00_p);                   \
})

#define P99_TP_STATE_COMMIT(TPS) p99_tp_state_commit(&(TPS)->p00_st)


#endif
