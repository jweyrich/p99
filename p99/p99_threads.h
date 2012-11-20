/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2011-2012 Jens Gustedt, INRIA, France                       */
/* (C) copyright  2012 William Morris                                         */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_THREADS_H
#define P99_THREADS_H 1

#include "p99_try.h"


/**
 ** @addtogroup threads C11 thread emulation on top of POSIX threads
 **
 ** This is a relatively straightforward implementation of the C11
 ** thread model on top of POSIX threads. The main difficulty this presents
 ** is that the thread entry function signature differs between the
 ** two. C11 thread returns an <code>int</code> whereas POSIX returns
 ** a <code>void*</code>.
 **
 ** You can find the thread management interfaces through the
 ** documentation of the type ::thrd_t.
 **
 ** @remark In addition to POSIX threads this implementation needs
 ** some C11 atomic operations for initialization via ::call_once and
 ** status communication.
 **
 ** @{
 **/

/**
 ** @addtogroup thread_macros
 ** @{
 **/

/**
 ** @brief expands to a value that can be used to initialize an object
 ** of type ::p99_once_flag
 **
 ** A characteristic of this implementation is that initialization is
 ** equivalent to the default initialization by @c 0, meaning in
 ** particular that a ::p99_once_flag that is of static or
 ** thread-local storage duration is by default correctly initialized.
 ** This property is subsequently used in the fallback
 ** implementation of thread local storage.
 **
 ** @see p99_once_flag
 **/
#define P99_ONCE_FLAG_INIT { { 0 } }

#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
# warning "definition of PTHREAD_DESTRUCTOR_ITERATIONS is missing"
/**
 ** @brief expands to an integer constant expression representing the
 ** maximum number of times that destructors will be called when a
 ** thread terminates
 ** @see tss_t
 **/
# define TSS_DTOR_ITERATIONS 1
#else
# define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS
#endif

/**
 ** @}
 **/


/**
 ** @addtogroup thread_types
 ** @{
 **/

/**
 ** @brief complete object type that holds an identifier for a
 ** condition variable
 **
 ** @remark This type is just a wrapper around a POSIX @c pthread_cond_t.
 **/
P99_ENC_DECLARE(pthread_cond_t, cnd_t);

typedef struct p00_thrd p00_thrd;
/**
 ** @brief complete object type that holds an identifier for a thread
 **
 ** @remark Although this implementation of threads is based on
 ** POSIX, this thread id type is not compatible with POSIX @c
 ** pthread_t.
 **
 ** The use of the P99 implementation will result in all threads, even those
 ** not created through ::thrd_create, having a distinct
 ** object of type ::thrd_t as their ID. This ID is accessible through
 ** ::thrd_current.
 **/
P99_ENC_DECLARE(struct p00_thrd*, thrd_t);

p99_inline thrd_t* thrd_t_init(thrd_t *p00_id) {
  if (p00_id) {
    *p00_id = P99_LVAL(thrd_t const);
  }
  return p00_id;
}

p99_inline void thrd_t_destroy(thrd_t *p00_id) {
  /* special care for bogus warning given by icc */
  (void)p00_id;
}

p99_inline
char const* thrd2str(char *p00_buf, thrd_t p00_id) {
  unsigned char *p00_p = (unsigned char*)&p00_id;
  for (size_t p00_i = 0; p00_i < sizeof(thrd_t); ++p00_i) {
    snprintf(p00_buf + 2*p00_i, 3, "%02X", p00_p[p00_i]);
  }
  return p00_buf;
}

#define THRD2STR(ID) thrd2str((char[1 + sizeof(thrd_t) * 2]){0}, (ID))


/**
 ** @brief complete object type that holds an identifier for a mutex
 **
 ** @remark This type is just a wrapper around a POSIX @c pthread_mutex_t.
 **/
P99_ENC_DECLARE(pthread_mutex_t, mtx_t);

/**
 ** @brief function pointer type <code>int (*)(void*)</code> that is
 ** passed to ::thrd_create to create a new thread
 ** @see thrd_t
 */
typedef int (*thrd_start_t)(void*);

#ifndef ONCE_FLAG_INIT
typedef struct p99_once_flag once_flag;
#define ONCE_FLAG_INIT P99_ONCE_FLAG_INIT
#define call_once p99_call_once
#endif

typedef struct p99_once_flag p99_once_flag;

enum p00_once {
  p00_once_uninit = 0,
  p00_once_started,
  p00_once_finished,
};

/**
 ** @brief complete object type that holds a flag for use by
 ** ::p99_call_once
 **
 ** From the wording of the standard it is not clear whether a variable of
 ** this type @b must be initialized by means of ::P99_ONCE_FLAG_INIT. The
 ** corresponding POSIX structure requires the analog.
 **
 ** Therefore we don't use the POSIX structure, here, but cook this
 ** ourselves with atomic variables. In this way we can guarantee that a
 ** ::p99_once_flag that is initialized by the default initializer always
 ** has the correct state.
 */
struct p99_once_flag {
  union {
    enum p00_once p00_done;
    enum p00_once volatile p00_vdone;
  } p00_done;
  thrd_t p00_id;
  void (*const p00_init)(void);
  atomic_flag p00_flg;
};

P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_INIT_ONCE, 0)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_INIT_ONCE, 1)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_INIT_ONCE, 2)
#define P99_DECLARE_INIT_ONCE(T, NAME, ARG)                     \
/** @remark wrapper type around a T that is initialized once */ \
struct NAME {                                                   \
  p99_once_flag p00_once;                                       \
  T p00_val;                                                    \
};                                                              \
P99_DECLARE_STRUCT(NAME);                                       \
p99_inline                                                      \
void P99_PASTE3(p00_, NAME, _init_func)(T* ARG);                \
p99_inline                                                      \
void P99_PASTE3(p00_, NAME, _init_once)(NAME* ARG) {            \
  if (P99_UNLIKELY(!ARG->p00_once.p00_done.p00_done))           \
    do {                                                        \
      P99_SPIN_EXCLUDE(&ARG->p00_once.p00_flg) {                \
        if (!ARG->p00_once.p00_done.p00_vdone) {                \
          P99_PASTE3(p00_, NAME, _init_func)(&ARG->p00_val);    \
          ARG->p00_once.p00_done.p00_vdone = true;              \
        }                                                       \
      }                                                         \
    } while (!ARG->p00_once.p00_done.p00_vdone);                \
}                                                               \
p99_inline                                                      \
void P99_PASTE3(p00_, NAME, _init_func)(T* ARG)

#define P99_INIT_ONCE(NAME, VARP) P99_PASTE3(p00_, NAME, _init_once)(VARP)


/**
 ** @}
 **/

struct p00_thrd {
  atomic_flag p00_detached;
  unsigned p00_foreign;
  int p00_ret;
  pthread_t p00_id;
  union {
    struct {
      thrd_start_t p00_func;
      void *p00_arg;
    } p00_init;
    jmp_buf p00_jmp;
  } p00_ovrl;
};

/**
 ** @addtogroup thread_enum
 ** @{
 **/

/**
 ** @brief C11 mutex types
 **/
enum mtx_type {
  /**
   ** @brief passed to ::mtx_init to create a mutex object that supports
   ** neither timeout nor test and return
   **/
  mtx_plain = 0,

  /**
   ** @brief passed to ::mtx_init to create a mutex object that
   ** supports recursive locking
   **/
  mtx_recursive = 1,

  /**
   ** @brief passed to ::mtx_init to create a mutex object that supports timeout
   **
   ** In this implementation this feature is always provided.
   **/
  mtx_timed = 2,

  /**
   ** @brief passed to ::mtx_init to create a mutex object that
   ** deadlocks when locked twice
   **
   ** Consider a deadlock as a feature, here. Without this locking a
   ** mutex again is just undefined behavior, so anything could
   ** happen.
   **
   ** @remark This is an extension that comes with POSIX. Don't use it
   ** if you want your code to be portable outside POSIX.
   **/
  mtx_normal = 4,

  /**
   ** @brief passed to ::mtx_init to create a mutex object that
   ** returns an error when locked twice
   **
   ** @remark This is an extension that comes with POSIX. Don't use it
   ** if you want your code to be portable outside POSIX.
   **/
  mtx_errorcheck = 8,

  /**
   ** @brief used internally
   **
   ** This is meant to accumulate all extra flags that could be
   ** distinguished with POSIX mutex. This is not a valid flag to pass
   ** to ::mtx_init.
   **/
  mtx_extras = (mtx_normal | mtx_errorcheck | mtx_recursive),
};


/**
 ** @}
 **/

// 7.26.2 Initialization functions

/**
 ** @related thrd_t
 **/
p99_inline
void thrd_yield(void) {
  if (P99_UNLIKELY(sched_yield())) errno = 0;
}

/* Tentative definitions for global variables. This has the advantage
   that this defines weak symbols and we avoid to have to create a
   specific library. */
P99_DECLARE_THREAD_LOCAL(p00_thrd *, p00_thrd_local);

#define P00_THRD_LOCAL P99_THREAD_LOCAL(p00_thrd_local)

P99_WEAK(p00_foreign_nb)
_Atomic(size_t) p00_foreign_nb;

P99_WEAK(p00_foreign_tab)
p00_thrd ** p00_foreign_tab;

P99_WEAK(p00_foreign_cleanup)
void p00_foreign_cleanup(void) {
  size_t p00_foreign = atomic_load(&p00_foreign_nb);
  p00_thrd ** p00_thrd = p00_foreign_tab;
  p00_foreign_tab = 0;
  for (size_t p00_i = 0; p00_i < p00_foreign; ++p00_i) {
    if (!pthread_equal(p00_thrd[p00_i]->p00_id, pthread_self()))
      fputs("found foreign thread\n", stderr);
    free(p00_thrd[p00_i]);
  }
  free(p00_thrd);
}

/**
 ** @related thrd_t
 **
 ** @return identifier of the thread that called it
 **/
p99_inline
thrd_t thrd_current(void) {
  p00_thrd * p00_loc = P00_THRD_LOCAL;
  if (P99_UNLIKELY(!p00_loc)) {
    size_t p00_nb = atomic_fetch_add(&p00_foreign_nb, 1);
    if (!p00_nb) atexit(p00_foreign_cleanup);
    if ((p00_nb^(p00_nb-1)) == (p00_nb+(p00_nb-1))) {
      p00_foreign_tab = realloc(p00_foreign_tab, sizeof(p00_thrd*[2*(p00_nb+1)]));
    }
    p00_loc = malloc(sizeof *p00_loc);
    p00_foreign_tab[p00_nb] = p00_loc;
    *p00_loc = (p00_thrd) {
      .p00_id = pthread_self(),
       .p00_foreign = p00_nb + 1,
    };
    P00_THRD_LOCAL = p00_loc;
    if (p00_nb) {
      union {
        unsigned char raw[16];
        pthread_t thrd;
        unsigned long long ull;
      } id = { .raw = { 0 } };
      id.thrd = p00_loc->p00_id;
      fprintf(stderr, "foreign thread %llu is %zu\n", id.ull, p00_nb + 1);
    }
  }
  return (thrd_t)P99_ENC_INIT(p00_loc);
}

/**
 ** @related thrd_t
 **
 ** @return @c 0 if the thread @a p00_thr0 and the thread @a p00_thr1 refer to
 ** different threads. Otherwise a nonzero value is returned.
 **/
p99_inline
int thrd_equal(thrd_t p00_thr0, thrd_t p00_thr1) {
  return P99_ENC(p00_thr0) ==  P99_ENC(p00_thr1);
}

#define p00_call_once_2(FLAG, FUNC)                                          \
do {                                                                         \
  p99_once_flag *p00Mflag = (FLAG);                                          \
  if (P99_UNLIKELY(p00Mflag->p00_done.p00_done != p00_once_finished))        \
    do {                                                                     \
      atomic_flag_lock(&p00Mflag->p00_flg);                                  \
      switch (p00Mflag->p00_done.p00_vdone) {                                \
        /* we are doing the initialization */                                \
      case p00_once_uninit:                                                  \
        p00Mflag->p00_done.p00_done = 1;                                     \
        p00Mflag->p00_id = thrd_current();                                   \
        atomic_flag_unlock(&p00Mflag->p00_flg);                              \
        FUNC();                                                              \
        p00Mflag->p00_done.p00_done = 2;                                     \
        break;                                                               \
      case p00_once_started:                                                 \
        if (thrd_equal(p00Mflag->p00_id, thrd_current())) {                  \
          /* we are called recursively, abandon and return */                \
          atomic_flag_unlock(&p00Mflag->p00_flg);                            \
          p00Mflag = 0;                                                      \
          break;                                                             \
        }                                                                    \
        /* otherwise fall through */                                         \
      case p00_once_finished:                                                \
        atomic_flag_unlock(&p00Mflag->p00_flg);                              \
        break;                                                               \
      }                                                                      \
    } while (p00Mflag && p00Mflag->p00_done.p00_vdone != p00_once_finished); \
 } while (false)

p99_inline
void p00_call_once_1(p99_once_flag *p00_flag) {
  p00_call_once_2(p00_flag, p00_flag->p00_init);
}

#define p00_call_once_3(FLAG, FUNC, ARG)                                     \
do {                                                                         \
  p99_once_flag *p00Mflag = (FLAG);                                          \
  if (P99_UNLIKELY(p00Mflag->p00_done.p00_done != p00_once_finished))        \
    do {                                                                     \
      atomic_flag_lock(&p00Mflag->p00_flg);                                  \
      switch (p00Mflag->p00_done.p00_vdone) {                                \
        /* we are doing the initialization */                                \
      case p00_once_uninit:                                                  \
        p00Mflag->p00_done.p00_done = 1;                                     \
        p00Mflag->p00_id = thrd_current();                                   \
        atomic_flag_unlock(&p00Mflag->p00_flg);                              \
        FUNC(ARG);                                                           \
        p00Mflag->p00_done.p00_done = 2;                                     \
        break;                                                               \
      case p00_once_started:                                                 \
        if (thrd_equal(p00Mflag->p00_id, thrd_current())) {                  \
          /* we are called recursively, abandon and return */                \
          atomic_flag_unlock(&p00Mflag->p00_flg);                            \
          p00Mflag = 0;                                                      \
          break;                                                             \
        }                                                                    \
        /* otherwise fall through */                                         \
      case p00_once_finished:                                                \
        atomic_flag_unlock(&p00Mflag->p00_flg);                              \
        break;                                                               \
      }                                                                      \
    } while (p00Mflag && p00Mflag->p00_done.p00_vdone != p00_once_finished); \
 } while (false)

#define p00_call_once(N, ...)                                  \
P99_IF_EQ_1(N)                                                 \
(p00_call_once_1(__VA_ARGS__))                                 \
(P99_IF_EQ_2(N)                                                \
 (p00_call_once_2(__VA_ARGS__))                                \
 (p00_call_once_3(__VA_ARGS__)))

/**
 ** @brief Call a function @a FUNC exactly once, optionally
 ** providing it with argument @a ARG
 **
 ** This is an extension of the standard function ::call_once.
 **
 ** - If @a ARG is given, it must be compatible with type @c void* and is
 **   passed to @a FUNC as an argument. In this case @a FUNC must have
 **   the prototype <code>void FUNC(void*)</code>.
 **
 ** - If @a ARG is omitted @a FUNC should have the prototype
 **   <code>void FUNC(void)</code>, i.e not take any argument.
 **
 ** - If the field @c p00_init is initialized with an appropriate
 **   function, the @a FUNC can also be omitted and @c p00_init is then
 **   called instead.
 **
 ** @remark The @a FLAG is only protected by an
 ** ::atomic_flag. Therefore the functions that are passed to this
 ** should be nice and short.
 **
 ** @remark These functions are protected against recursion and should
 ** not deadlock when they are confronted with cyclic
 ** dependencies. But obviously, the order in which such a cycle is
 ** taken is not predictable.
 **
 ** @see p99_once_flag
 **/
#ifdef P00_DOXYGEN
#define p99_call_once(FLAG, FUNC, ARG)
#else
#define p99_call_once(...) p00_call_once(P99_NARG(__VA_ARGS__), __VA_ARGS__)
#endif

#ifdef P00_DOXYGEN
/**
 ** @brief Define a function that will be called exactly once by
 ** <code>P99_INIT_CHAIN(T)</code>.
 **
 ** The function has a prototype of <code>void someFunctionName(void)</code>.
 **
 ** @a T can be any valid identifier, the real function name will
 ** be mangled such that it will not clash with an existing name.
 **
 ** The ... list (optional) can be used to give a list of dependencies
 ** from other ::P99_INIT_CHAIN functions.
 ** @code
 ** P99_DEFINE_ONCE_CHAIN(toto) {
 **  // initialize some shared ressource
 ** }
 **
 ** P99_DEFINE_ONCE_CHAIN(tutu, toto) {
 **   // empty
 ** }
 ** @endcode
 **
 ** This will ensure that <code>P99_INIT_CHAIN(toto)</code> is always
 ** triggered by <code>P99_INIT_CHAIN(tutu)</code> and is run before we run
 ** the function @c tutu itself. As shown above in the example, many functions
 ** will be empty, serving just to ensure that all dynamic
 ** dependencies are initialized in the right order.
 ** @see P99_DECLARE_ONCE_CHAIN
 ** @see P99_INIT_CHAIN
 **/
#define P99_DEFINE_ONCE_CHAIN(T, ...)                          \
p99_once_flag p99_ ## T ## _once;                              \
void p00_ ## T ## _once_init(void)
#else
#define P99_DEFINE_ONCE_CHAIN(...)                             \
P99_IF_ELSE(P99_HAS_COMMA(__VA_ARGS__))                        \
 (P00_P99_DEFINE_ONCE_CHAIN_1(__VA_ARGS__))                    \
 (P00_P99_DEFINE_ONCE_CHAIN_0(__VA_ARGS__))
#endif

#define P00_P99_DEFINE_ONCE_CHAIN_0(T)                         \
static void P99_PASTE3(p00_, T, _once_init)(void);             \
p99_once_flag P99_PASTE3(p99_, T, _once) = {                   \
  .p00_init = P99_PASTE3(p00_, T, _once_init),                 \
};                                                             \
static void P99_PASTE3(p00_, T, _once_init)(void)

#define P00_ONCE_INIT(_0, T, _2) P99_INIT_CHAIN(T)

#define P00_P99_DEFINE_ONCE_CHAIN_1(T, ...)                              \
static void P99_PASTE3(p00_, T, _once_init0)(void);                      \
static void P99_PASTE3(p00_, T, _once_init)(void) {                      \
  P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_ONCE_INIT, __VA_ARGS__); \
  /* fprintf(stderr, "Initializing " #T "\n");*/                         \
  P99_PASTE3(p00_, T, _once_init0)();                                    \
 }                                                                       \
struct p99_once_flag P99_PASTE3(p99_, T, _once) = {                      \
  .p00_init = P99_PASTE3(p00_, T, _once_init),                           \
};                                                                       \
static void P99_PASTE3(p00_, T, _once_init0)(void)

/**
 ** @brief Declare the symbols that are needed for the macro
 ** ::P99_INIT_CHAIN().
 **
 ** @param T should be unique for each use of this macro.
 ** @see P99_INIT_CHAIN
 ** @see P99_DEFINE_ONCE_CHAIN
 **/
#define P99_DECLARE_ONCE_CHAIN(T)                              \
extern p99_once_flag P99_PASTE3(p99_, T, _once)

/**
 ** @brief Ensure that the function that was defined with
 ** ::P99_DEFINE_ONCE_CHAIN has been called exactly once before proceeding.
 **
 ** Such a call could be placed at the beginning of a user function to
 ** ensure that a shared resource is always initialized before its
 ** use. A better strategy though would be to call ::P99_INIT_CHAIN from @c
 ** main, e.g., before any threads of the application are started.
 ** @see P99_DECLARE_ONCE_CHAIN
 ** @see P99_DEFINE_ONCE_CHAIN
 **/
#define P99_INIT_CHAIN(T)                                                       \
p99_call_once(&P99_PASTE3(p99_, T, _once), P99_PASTE3(p99_, T, _once).p00_init)

// 7.26.3 Condition variable functions

/**
 ** @related cnd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int cnd_broadcast(cnd_t *p00_cond) {
  return pthread_cond_broadcast(&P99_ENCP(p00_cond)) ? thrd_error : thrd_success;
}

/**
 ** @related cnd_t
 **/
p99_inline
void cnd_destroy(cnd_t *p00_cond) {
  (void)pthread_cond_destroy(&P99_ENCP(p00_cond));
}

/**
 ** @related cnd_t
 **
 ** @return The ::cnd_init function returns ::thrd_success on success,
 ** or ::thrd_nomem if no memory could be allocated for the newly
 ** created condition, or ::thrd_error if the request could not be
 ** honored.
 **/
p99_inline
int cnd_init(cnd_t *p00_cond) {
  int p00_ret = pthread_cond_init(&P99_ENCP(p00_cond), 0);
  switch (p00_ret) {
  case 0:         return thrd_success;
  case ENOMEM:    return thrd_nomem;
  default:        return thrd_error;
  }
}

/**
 ** @related cnd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int cnd_signal(cnd_t *p00_cond) {
  return pthread_cond_signal(&P99_ENCP(p00_cond)) ? thrd_error : thrd_success;
}

#if (POSIX_TIMEOUTS > 0) || !defined(POSIX_TIMEOUTS)

/**
 ** @related cnd_t
 **
 ** @return ::thrd_success upon success, or ::thrd_timedout if the
 ** time specified in the call was reached without acquiring the
 ** requested resource, or ::thrd_error if the request could not be
 ** honored.
 **/
p99_inline
int cnd_timedwait(cnd_t *restrict p00_cond, mtx_t *restrict p00_mtx, const struct timespec *restrict p00_ts) {
  int p00_ret = pthread_cond_timedwait(&P99_ENCP(p00_cond), &P99_ENCP(p00_mtx), p00_ts);
  switch (p00_ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

#endif

/**
 ** @related cnd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int cnd_wait(cnd_t *p00_cond, mtx_t *p00_mtx) {
  return pthread_cond_wait(&P99_ENCP(p00_cond), &P99_ENCP(p00_mtx)) ? thrd_error : thrd_success;
}

// 7.26.4 Mutex functions

/**
 ** @related mtx_t
 **/
p99_inline
void mtx_destroy(mtx_t *p00_mtx) {
  (void)pthread_mutex_destroy(&P99_ENCP(p00_mtx));
}

/**
 ** @related mtx_t
 **
 ** @param p00_mtx A pointer to an unitialized mutex object
 ** @param p00_type One of the constants in ::mtx_type
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int mtx_init(mtx_t *p00_mtx, int p00_type) {
  if (p00_type & mtx_extras) {
    pthread_mutexattr_t p00_attr;
    int p00_ret = pthread_mutexattr_init(&p00_attr);
    if (P99_UNLIKELY(p00_ret)) return thrd_error;
    switch (p00_type & mtx_extras) {
    case mtx_normal: p00_ret = PTHREAD_MUTEX_NORMAL; break;
    case mtx_errorcheck: p00_ret = PTHREAD_MUTEX_ERRORCHECK; break;
    case mtx_recursive: p00_ret = PTHREAD_MUTEX_RECURSIVE; break;
    default: return thrd_error;
    }
    p00_ret = pthread_mutexattr_settype(&p00_attr, p00_ret);
    if (P99_UNLIKELY(p00_ret)) return thrd_error;
    p00_ret = pthread_mutex_init(&P99_ENCP(p00_mtx), &p00_attr);
    if (P99_UNLIKELY(p00_ret)) return thrd_error;
  } else {
    int p00_ret = pthread_mutex_init(&P99_ENCP(p00_mtx), 0);
    if (P99_UNLIKELY(p00_ret)) return thrd_error;
  }
  return thrd_success;
}

/**
 ** @related mtx_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int mtx_lock(mtx_t *p00_mtx) {
  return pthread_mutex_lock(&P99_ENCP(p00_mtx)) ? thrd_error : thrd_success;
}

#if (POSIX_TIMEOUTS > 0) || !defined(POSIX_TIMEOUTS)

/**
 ** @related mtx_t
 **
 ** @return ::thrd_success upon success, or ::thrd_timedout if the
 ** time specified in the call was reached without acquiring the
 ** requested resource, or ::thrd_error if the request could not be
 ** honored.
 **/
p99_inline
int mtx_timedlock(mtx_t *restrict p00_mtx, const struct timespec *restrict p00_ts) {
  int p00_ret = pthread_mutex_timedlock(&P99_ENCP(p00_mtx), p00_ts);
  switch (p00_ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

#endif

/**
 ** @related mtx_t
 **
 ** @return ::thrd_success on success, or ::thrd_busy if the resource
 ** requested is already in use, or ::thrd_error if the request could
 ** not be honored.
 **/
p99_inline
int mtx_trylock(mtx_t *p00_mtx) {
  int p00_ret = pthread_mutex_trylock(&P99_ENCP(p00_mtx));
  switch (p00_ret) {
  case 0:         return thrd_success;
  case EBUSY:     return thrd_busy;
  default:        return thrd_error;
  };
}

/**
 ** @related mtx_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int mtx_unlock(mtx_t *p00_mtx) {
  return pthread_mutex_unlock(&P99_ENCP(p00_mtx)) ? thrd_error : thrd_success;
}

/**
 ** @brief Protect the following block or statement with @c
 ** mtx_t @a MUT.
 **
 ** @see P99_CRITICAL for a tool that uses a spinlock that is
 ** allocated behind the scene.
 **
 ** This does some rudimentary error checking for the result of
 ** locking. If an error occurs the whole block and any other
 ** enclosing blocks that are protected with P99_UNWIND_PROTECT are
 ** aborted.
 **/
P99_BLOCK_DOCUMENT
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_MUTUAL_EXCLUDE, 0)
#define P99_MUTUAL_EXCLUDE(MUT)                                                  \
P00_BLK_START                                                                    \
P00_BLK_DECL(int, p00_errNo, 0)                                                  \
P99_GUARDED_BLOCK(mtx_t*,                                                        \
                  P99_FILEID(mut),                                               \
                  &(MUT),                                                        \
                  (void)(P99_UNLIKELY(p00_errNo = mtx_lock(P99_FILEID(mut)))     \
                         && (fprintf(stderr,                                     \
                                     __FILE__ ":"                                \
                                     P99_STRINGIFY(__LINE__) ": lock error for " \
                                     P99_STRINGIFY(MUT) ", %s",                  \
                                     strerror(p00_errNo)), 1)                    \
                         && (P99_FILEID(mut) = 0, 1)                             \
                         && (P99_UNWIND(-1), 1)                                  \
                         ),                                                      \
                  (void)(P99_FILEID(mut)                                         \
                         && mtx_unlock(P99_FILEID(mut))))



P99_SETJMP_INLINE(p00_thrd_create)
void * p00_thrd_create(void* p00_context);

/**
 ** @related thrd_t
 **
 ** @return ::thrd_success on success, or ::thrd_nomem if no memory
 ** could be allocated for the thread requested, or ::thrd_error if
 ** the request could not be honored.
 **/
p99_inline
int thrd_create(thrd_t *p00_thr, thrd_start_t p00_func, void *p00_arg) {
  p00_thrd * p00_cntxt = malloc(sizeof *p00_cntxt);
  if (!p00_cntxt) return thrd_nomem;
  *p00_cntxt = (p00_thrd const) {
    .p00_ovrl = {
      .p00_init = {
        .p00_func = p00_func,
        .p00_arg = p00_arg,
      },
    },
    .p00_detached = ATOMIC_FLAG_INIT,
  };
  int p00_ret = pthread_create(&p00_cntxt->p00_id, 0, p00_thrd_create, p00_cntxt);
  if (P99_UNLIKELY(p00_ret)) {
    free(p00_cntxt);
    switch (p00_ret) {
    case ENOMEM:    return thrd_nomem;
    default:        return thrd_error;
    };
  } else {
    P99_ENCP(p00_thr) = p00_cntxt;
    return thrd_success;
  }
}

/**
 ** @related thrd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int thrd_detach(thrd_t p00_thr) {
  /* The thread is not yet detached so its pthread id is still
     valid. If it already has finished, this will just free the
     resources that pthread holds for it. */
  int p00_ret = pthread_detach(P99_ENC(p00_thr)->p00_id) ? thrd_error : thrd_success;
  if (atomic_flag_test_and_set(&P99_ENC(p00_thr)->p00_detached)) {
    /* The thread has already finished. Free the state, since nobody
       will join it, anyhow. */
    free(P99_ENC(p00_thr));
    return thrd_success;
  } else {
    return p00_ret;
  }
}

#ifdef P00_DOXYGEN
/**
 ** @related thrd_t
 **/
p99_inline void thrd_exit(int p00_res);
#else
p99_inline
_Noreturn
void thrd_exit(int p00_res) {
  p00_thrd * p00_cntxt = P00_THRD_LOCAL;
  if (P99_LIKELY(p00_cntxt)) {
    if (!p00_cntxt->p00_foreign) {
      p00_cntxt->p00_ret = p00_res;
      longjmp(p00_cntxt->p00_ovrl.p00_jmp, 1);
    } else {
      free(p00_cntxt);
      P00_THRD_LOCAL = 0;
    }
  }
  /* should only be reached by threads that where created directly
     with pthreads, e.g main */
  pthread_exit(0);
}
#endif

/**
 ** @related thrd_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int thrd_join(thrd_t p00_thr, int *p00_res) {
  void *p00_res0;
  if (P99_UNLIKELY(pthread_join(P99_ENC(p00_thr)->p00_id, &p00_res0))) return thrd_error;
  if (p00_res) *p00_res = P99_ENC(p00_thr)->p00_ret;
  free(P99_ENC(p00_thr));
  return thrd_success;
}

/**
 ** @related thrd_t
 **
 ** @return @c 0 if the requested time has elapsed, @c -1 if it has
 ** been interrupted by a signal, or another negative value if it fails.
 ** Consistent with that, this implementation uses ::thrd_success,
 ** ::thrd_intr and ::thrd_error as return values.
 **/
p99_inline
int thrd_sleep(const struct timespec *p00_duration, struct timespec *p00_remaining) {
  errno = 0;
  int p00_ret = nanosleep(p00_duration, p00_remaining);
  if (p00_ret) {
    p00_ret = (errno == EINTR) ? thrd_intr : thrd_error;
    errno = 0;
    return p00_ret;
  } else return thrd_success;
}

/*
  This is static inline because of the following message from gcc:

   sorry, unimplemented: function ‘p00_thrd_create’ can never be inlined because it uses setjmp
*/
P99_SETJMP_INLINE(p00_thrd_create)
void * p00_thrd_create(void* p00_context) {
  p00_thrd * p00_cntxt = p00_context;
  P00_THRD_LOCAL = p00_cntxt;
  {
    thrd_start_t p00_func = p00_cntxt->p00_ovrl.p00_init.p00_func;
    void * p00_arg = p00_cntxt->p00_ovrl.p00_init.p00_arg;
    if (!setjmp(p00_cntxt->p00_ovrl.p00_jmp)) {
      p00_cntxt->p00_ret = p00_func(p00_arg);
    }
    if (atomic_flag_test_and_set(&p00_cntxt->p00_detached)) {
      free(p00_cntxt);
    }
  }
  P00_THRD_LOCAL = 0;
  return 0;
}

/* Add rudimentary support for the timespec data structure */

/* All this fuss is needed because TIME_UTC isn't allowed to be 0.
   And this is because timespec_get returns failure as 0, and success
   by returning the time base, oh well. */

/* A fallback version if all else fails */

p99_inline
int p00_timespec_get(struct timespec *p00_ts, int p00_base) {
  struct timeval t;
  if (gettimeofday(&t, P99_0(struct timezone*))) {
    errno = 0;
    return 0;
  } else {
    p00_ts->tv_sec = t.tv_sec;
    /* ensure that the usec value is first converted to a nsec
       value of the correct width ... */
    p00_ts->tv_nsec = t.tv_usec;
    /* ... and do the multiplication within that width. */
    p00_ts->tv_nsec *= 1000;
    return p00_base;
  }
}

/**
 ** @brief The ::timespec_get function sets the interval pointed to by
 ** @a p00_ts to hold the current calendar time based on the specified
 ** time base.
 **
 ** If @a p00_base is ::TIME_UTC, the @c tv_sec member is set to the
 ** number of seconds since an implementation defined epoch, truncated
 ** to a whole value and the @c tv_nsec member is set to the integral
 ** number of nanoseconds, rounded to the resolution of the system
 ** clock.
 **
 ** @return If the ::timespec_get function is successful it returns
 ** the nonzero value @a p00_base; otherwise, it returns zero.
 **
 ** @ingroup C11_library
 **/
p99_inline
int timespec_get(struct timespec *p00_ts, int p00_base);

#ifndef TIME_UTC

enum {
  p00_time_base,
  p00_time_utc,
  p00_time_monotonic,
# ifdef CLOCK_PROCESS_CPUTIME_ID
  p00_time_process_cputime_id,
# endif
# ifdef CLOCK_THREAD_CPUTIME_ID
  p00_time_thread_cputime_id,
# endif
  p00_time_base_max
};

/**
 ** @brief expands to an integer constant greater than 0 that
 ** designates the UTC time base since an implementation defined epoch
 **
 ** This is the only time base that is guaranteed to be available by C11.
 **
 ** @see TIME_MONOTONIC
 **/
# define TIME_UTC p00_time_utc

# if defined(CLOCK_REALTIME) || defined(P00_DOXYGEN)

# if defined(CLOCK_MONOTONIC) || defined(P00_DOXYGEN)
/**
 ** @brief expands to an integer constant greater than 0 that
 ** designates a real time clock who's base is usually the boot time
 ** of the processor
 **
 ** If this is available, this denotes a clock that should be a bit
 ** more efficient than ::TIME_UTC, since it usually doesn't have to
 ** perform a call into the OS kernel but may query the processor
 ** directly.
 **
 ** Use this if available and if you are only interested in times
 ** relative to your program execution, e.g for benchmarks.
 **/
#  define TIME_MONOTONIC p00_time_monotonic

# endif
# ifdef CLOCK_PROCESS_CPUTIME_ID
#  define TIME_PROCESS_CPUTIME_ID p00_time_process_cputime_id
# endif
# ifdef CLOCK_THREAD_CPUTIME_ID
#  define TIME_THREAD_CPUTIME_ID p00_time_thread_cputime_id,
# endif

p99_inline
clockid_t p00_getclockid(int base) {
  return (base >= p00_time_base_max)
         ? CLOCK_REALTIME
  : (clockid_t const[]) {
    [p00_time_base] = CLOCK_REALTIME,
                      [p00_time_utc] = CLOCK_REALTIME,
# ifdef CLOCK_MONOTONIC
                                       [p00_time_monotonic] = CLOCK_MONOTONIC,
# endif
# ifdef CLOCK_PROCESS_CPUTIME_ID
                                           [p00_time_process_cputime_id] = CLOCK_PROCESS_CPUTIME_ID,
# endif
# ifdef CLOCK_THREAD_CPUTIME_ID
                                               [p00_time_thread_cputime_id] = CLOCK_THREAD_CPUTIME_ID,
# endif
  }[base];
}

/**
 ** @brief The ::timespec_get function sets the interval pointed to by
 ** @a p00_ts to hold the current calendar time based on the specified
 ** time base.
 **
 ** If @a p00_base is ::TIME_UTC, the @c tv_sec member is set to the
 ** number of seconds since an implementation defined epoch, truncated
 ** to a whole value and the @c tv_nsec member is set to the integral
 ** number of nanoseconds, rounded to the resolution of the system
 ** clock.
 **
 ** @return If the ::timespec_get function is successful it returns
 ** the nonzero value @a p00_base; otherwise, it returns zero.
 **/
p99_inline
int timespec_get(struct timespec *p00_ts, int p00_base) {
  clockid_t p00_clkid = p00_getclockid(p00_base);
  if (clock_gettime(p00_clkid, p00_ts)) {
    errno = 0;
    return 0;
  } else
    return p00_base;
}

# elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
# include <mach/mach_time.h>

# define TIME_MONOTONIC p00_time_monotonic

P99_WEAK(p00_timebase)
double p00_timebase;

P99_WEAK(p00_timeoff_)
struct timespec p00_timeoff_;

P99_WEAK(p00_timeoff)
struct timespec const*const p00_timeoff = &p00_timeoff_;

P99_WEAK(p00_timeonce)
once_flag p00_timeonce = ONCE_FLAG_INIT;

p99_inline
void p00_timeonce_init(void) {
  /* Calibrate the monotonic time */
  mach_timebase_info_data_t p00_tb = P99_INIT;
  mach_timebase_info(&p00_tb);
  p00_timebase = p00_tb.numer;
  p00_timebase /= p00_tb.denom;
  /* Compute the offset of the monotonic time compared to UTC */
  /* Nanosec since system start, or something similar. */
  uint64_t p00_nsec = mach_absolute_time() * p00_timebase;
  p00_timespec_get(&p00_timeoff_, TIME_UTC);
  uint64_t const p00_giga = UINT64_C(1000000000);
  uint64_t p00_epoch = p00_timeoff_.tv_sec * p00_giga + p00_timeoff_.tv_nsec;
  p00_epoch -= p00_nsec;
  p00_timeoff_.tv_sec = p00_epoch / p00_giga;
  p00_timeoff_.tv_nsec = p00_epoch % p00_giga;
}

p99_inline
int timespec_get(struct timespec *p00_ts, int p00_base) {
  call_once(&p00_timeonce, p00_timeonce_init);
  uint64_t p00_nsec = mach_absolute_time() * p00_timebase;
  register uint64_t const p00_giga = UINT64_C(1000000000);
  p00_ts->tv_sec = p00_nsec / p00_giga;
  p00_ts->tv_nsec = p00_nsec % p00_giga;
  if (p00_base != TIME_MONOTONIC) {
    p00_ts->tv_sec += p00_timeoff->tv_sec;
    p00_ts->tv_nsec += p00_timeoff->tv_nsec;
    while (p00_ts->tv_nsec >= p00_giga) {
      p00_ts->tv_nsec -= p00_giga;
      ++p00_ts->tv_sec;
    }
  }
  return p00_base;
}
# else
# warning only low resolution gettimeofday found
# define timespec_get p00_timespec_get
# endif
#endif

/**
 ** @}
 **/

#endif
