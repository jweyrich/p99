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
typedef uint64_t p00_tp_glue;
#else
# if defined(UINT128_MAX)
typedef uint128_t p00_tp_glue;
# else
typedef p99x_uint128 p00_tp_glue;
# endif
#endif

P99_CONSTANT(int, p00_tp_bits, sizeof(p00_tp_glue)*CHAR_BIT);
P99_CONSTANT(int, p00_tp_shift, p00_tp_bits/2);

p99_inline
p00_tp_glue p00_tp_p2i(void * p, uintptr_t t) {
  return (((p00_tp_glue)t)<<p00_tp_shift)|(uintptr_t)p;
}

#define P00_TP_GLUE_INITIALIZER(VAL, TIC) { (((p00_tp_glue)VAL)<<p00_tp_shift)|((uintptr_t){(TIC)}) }


p99_inline
void * p00_tp_i2p(p00_tp_glue v) {
  return (void*)(uintptr_t)v;
}

p99_inline
uintptr_t p00_tp_i2i(p00_tp_glue v) {
  return v >> p00_tp_shift;
}

#else
P99_DECLARE_STRUCT(p00_tp_glue);

struct p00_tp_glue {
  uintptr_t p00_tag;
  void* p00_val;
};

#define P00_TP_GLUE_INITIALIZER(VAL, TIC) { .p00_tag = (TIC), .p00_val = (VAL), }

p99_inline
p00_tp_glue p00_tp_p2i(void * p00_val, uintptr_t p00_tag) {
  return (p00_tp_glue) { .p00_tag = p00_tag, .p00_val = p00_val, };
}

p99_inline
void * p00_tp_i2p(p00_tp_glue p00_sta) {
  return p00_sta.p00_val;
}

p99_inline
uintptr_t p00_tp_i2i(p00_tp_glue p00_sta) {
  return p00_sta.p00_tag;
}
#endif

P99_DECLARE_ATOMIC(p00_tp_glue);

P99_DECLARE_STRUCT(p99_tp);
P99_DECLARE_STRUCT(p99_tp_state);


/* The tag part of a p99_tp ideally would be unique during the whole
   run of a program instance. Since we might not have more than 32 bit
   for this, this will not be possible in general, but we try to get
   as close as possible to that.

   The problem should generally only arise for several accesses to the
   same memory that are close in time. Particularly problematic are
   such usages that go across free and re-malloc boundaries, because
   two instances of p99_tp that are realized on the same address could
   look the same if by coincidence their tags are the same.

   Therefore we compose each tag from two parts. One is a thread
   specific counter (p00_tp_tick), and the other is a global counter
   (p00_tp_tack) that is only consulted, when the thread counter
   wraps.

   If uintptr_t is 64 bit wide this should be safe. Each of the type
   of counters only would wrap after 4 billion increments. In
   particular we only could have a duplicate tag after 4 billion
   events that consist in the creation of a new thread or in
   p00_tp_tick wrapping for certain threads.

   */
P99_DECLARE_THREAD_LOCAL(uintptr_t, p00_tp_tick);
P99_WEAK(p00_tp_tack) _Atomic(uintptr_t) p00_tp_tack;

p99_inline
uintptr_t p00_tp_tick_get(void) {
  enum { p00_bits = sizeof(uintptr_t)*CHAR_BIT/2, };
  register uintptr_t const p00_mask = (UINTPTR_MAX >> p00_bits);
  register uintptr_t*const p00_ret = &P99_THREAD_LOCAL(p00_tp_tick);
  if (P99_UNLIKELY(!(*p00_ret & p00_mask))) {
    uintptr_t p00_tack = 0;
    while (!p00_tack) {
      p00_tack = atomic_fetch_add(&p00_tp_tack, 1u);
      p00_tack &= p00_mask;
    }
    *p00_ret = (p00_tack << p00_bits);
  }
  return ++(*p00_ret);
}

p99_inline
p00_tp_glue* p00_tp_glue_init(p00_tp_glue* el, void * p) {
  if (el) {
    *el = (p00_tp_glue)P00_TP_GLUE_INITIALIZER(p, p00_tp_tick_get());
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
  _Atomic(p00_tp_glue) p00_val;
  void*const p00_init;
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
  p00_tp_glue p00_val;
  p00_tp_glue p00_next;
  p99_tp* p00_tp;
};

# define P00_TP_INITIALIZER(VAL) {                              \
    .p00_init = (VAL),                                                \
}

p99_inline
bool p00_tp_cmpxchg(_Atomic(p00_tp_glue)* p00_p, p00_tp_glue* p00_prev, p00_tp_glue p00_new) {
  P99_MARK("wide cmpxchg start");
  bool ret = atomic_compare_exchange_weak(p00_p, p00_prev, p00_new);
  P99_MARK("wide cmpxchg end");
  return ret;
}

p99_inline
p00_tp_glue p00_tp_get(p99_tp* p00_tp) {
  register p00_tp_glue p00_ret = atomic_load(&p00_tp->p00_val);
  if (P99_UNLIKELY(!p00_tp_i2i(p00_ret))) {
    /* Only store it in addressable memory if we can't avoid it. */
    p00_tp_glue p00_ter = p00_ret;
    register p00_tp_glue p00_rep = P00_TP_GLUE_INITIALIZER(p00_tp->p00_init, p00_tp_tick_get());
    if (p00_tp_cmpxchg(&p00_tp->p00_val, &p00_ter, p00_rep))
      p00_ret = p00_rep;
    else
      p00_ret = p00_ter;
  }
  return p00_ret;
}

/**
 ** @brief Load the value of @a p00_tp into the state variable and
 ** prepare it to commit value @a p00_p later.
 **/
p99_inline
p99_tp_state p99_tp_state_initializer(p99_tp* p00_tp, void* p00_p) {
  return (p99_tp_state) {
    .p00_val = p00_tp_get(p00_tp),
     .p00_next = P00_TP_GLUE_INITIALIZER(p00_p, p00_tp_tick_get()),
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
bool p99_tp_state_commit(p99_tp_state* p00_state) {
  return p00_tp_cmpxchg(&p00_state->p00_tp->p00_val, &p00_state->p00_val, p00_state->p00_next);
}

p99_inline
bool p99_tp_state_check(p99_tp_state* p00_state) {
  return p00_tp_cmpxchg(&p00_state->p00_tp->p00_val, &p00_state->p00_val, p00_state->p00_val);
}

# define P99_TP(T) P99_PASTE2(p00_tp_, T)
# define P99_TP_STATE(T) P99_PASTE2(p00_tp_glue_, T)

P00_DOCUMENT_TYPE_ARGUMENT(P99_TP_DECLARE, 0)
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
    atomic_init(&p00_el->p00_val, p00_tp_p2i(p00_val, p00_tp_tick_get()));
  }
  return p00_el;
}

# define p99_tp_init(EL, VAL) (p00_tp_init(&(EL)->p00_tp, (VAL)), (EL))

/**
 ** @brief Load the value of @a TP into the state variable.
 **
 ** @see P99_TP_STATE_COMMIT to commit value @a P, later
 **
 ** @see P99_TP_STATE_CHECK to check if the such initialized state is
 ** still consistent with @a TP
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

/**
 ** @brief Unconditionally return the value of @a TP.
 **/
#define P99_TP_GET(TP)                                             \
p99_extension ({                                                   \
    P99_MACRO_VAR(p00_tp, (TP));                                   \
    /* ensure that pointer that is returned is converted to the */ \
    /* base type, and that the return can't be used as lvalue */   \
    register P99_TP_TYPE(p00_tp)* const p00_r                      \
      = p99_tp_get(&p00_tp->p00_tp);                               \
    p00_r;                                                         \
})

/**
 ** @brief Return the value the underlying tagged pointer of @a TPS as
 ** it was found when initializing @a TPS.
 **
 ** @see P99_TP_STATE_CHECK to know if that returned value is still
 ** valid.
 **/
#define P99_TP_STATE_GET(TPS)                                      \
p99_extension ({                                                   \
    P99_MACRO_VAR(p00_tps, (TPS));                                 \
    /* ensure that pointer that is returned is converted to the */ \
    /* base type, and that the return can't be used as lvalue */   \
    register P99_TP_STATE_TYPE(p00_tps)* const p00_r               \
      = p99_tp_state_get(&p00_tps->p00_st);                        \
    p00_r;                                                         \
})


/**
 ** @brief Change the value to which the tagged pointer will be
 ** updated.
 **/
#define P99_TP_STATE_SET(TPS, P)                                 \
p99_extension ({                                                 \
    P99_MACRO_VAR(p00_tps, (TPS));                               \
    /* ensure that P is assignment compatible to the */          \
    /* base type, and that the return can't be used as lvalue */ \
    register P99_TP_STATE_TYPE(p00_tps)* p00_p = (P);            \
    p99_tp_state_set(&p00_tps->p00_st, p00_p);                   \
})

/**
 ** @brief Commit the registered value change to the underlying tagged
 ** pointer.
 **
 ** @return @c true if the commit was succesful, @c false otherwise.
 **/
#define P99_TP_STATE_COMMIT(TPS) p99_tp_state_commit(&(TPS)->p00_st)

/**
 ** @brief Check if the registered value is still consistent with the
 ** underlying tagged pointer.
 **
 ** @return @c true if the value is still consistent, @c false
 ** otherwise.
 **/
#define P99_TP_STATE_CHECK(TPS) p99_tp_state_check(&(TPS)->p00_st)


#endif