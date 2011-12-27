/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2011 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_THREADS_H
#define P99_THREADS_H 1

#ifndef __GNUC__
# error "P99 threads needs gcc specific features"
#endif
#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "p99_c99.h"

#define P00_ENCAPSULATE(T, NAME)                               \
typedef struct {                                               \
  /** @private */                                              \
  T val;                                                       \
} NAME

/**
 ** @addtogroup threads
 ** @{
 **/

/**
 ** @addtogroup thread_macros
 ** @{
 **/

/**
 ** @brief expands to a value that can be used to initialize an object
 ** of type ::once_flag
 ** @memberof once_flag
 **/
#define ONCE_FLAG_INIT { .val = PTHREAD_ONCE_INIT, }
#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
# warning "definition of PTHREAD_DESTRUCTOR_ITERATIONS is missing"
/**
 ** @brief expands to an integer constant expression representing the
 ** maximum number of times that destructors will be called when a
 ** thread terminates
 ** @memberof tss_t
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
 **/
P00_ENCAPSULATE(pthread_cond_t, cnd_t);

typedef struct p00_thrd p00_thrd;
/**
 ** @brief complete object type that holds an identifier for a thread
 **/
P00_ENCAPSULATE(struct p00_thrd*, thrd_t);

/**
 ** @brief complete object type that holds an identifier for a
 ** thread-specific storage pointer
 **/
P00_ENCAPSULATE(pthread_key_t, tss_t);
/**
 ** @brief complete object type that holds an identifier for a mutex
 **/
P00_ENCAPSULATE(pthread_mutex_t, mtx_t);

/**
 ** @brief which is the function pointer type <code>void
 ** (*)(void*)</code>, used for a destructor for a thread-specific
 ** storage pointer
 ** @see tss_t
 **/
typedef void (*tss_dtor_t)(void*);

/**
 ** @brief function pointer type <code>int (*)(void*)</code> that is
 ** passed to ::thrd_create to create a new thread
 ** @see thrd_t
 */
typedef int (*thrd_start_t)(void*);

/**
 ** @brief complete object type that holds a flag for use by
 ** ::call_once
 */
P00_ENCAPSULATE(pthread_once_t, once_flag);

P00_ENCAPSULATE(int, atomic_flag);
/**
 ** @brief Atomic access to a value of type <code>int</code>
 ** @see atomic_int for the possible operations on this type
 **/
P00_ENCAPSULATE(int, atomic_int);
/**
 ** @brief Atomic access to a value of type <code>int</code>
 ** @see atomic_load
 ** @see atomic_store
 ** @see atomic_compare_exchange_weak
 ** @see ATOMIC_VAR_INIT
 ** @see atomic_init
 **/
P00_ENCAPSULATE(_Bool, atomic_bool);

#define P00_ENCAPSULATE_ATOMIC(T, NAME)                                 \
/** @brief Atomic access to a value of type <code>T</code> @see atomic_int for the possible operations on this type */ \
P00_ENCAPSULATE(T, NAME)

P00_ENCAPSULATE_ATOMIC(char, atomic_char);
P00_ENCAPSULATE_ATOMIC(signed char, atomic_schar);
P00_ENCAPSULATE_ATOMIC(unsigned char, atomic_uchar);
P00_ENCAPSULATE_ATOMIC(short, atomic_short);
P00_ENCAPSULATE_ATOMIC(unsigned short, atomic_ushort);
P00_ENCAPSULATE_ATOMIC(unsigned int, atomic_uint);
P00_ENCAPSULATE_ATOMIC(long, atomic_long);
P00_ENCAPSULATE_ATOMIC(unsigned long, atomic_ulong);
P00_ENCAPSULATE_ATOMIC(long long, atomic_llong);
P00_ENCAPSULATE_ATOMIC(unsigned long long, atomic_ullong);
//P00_ENCAPSULATE_ATOMIC(char16_t, atomic_char16_t);
//P00_ENCAPSULATE_ATOMIC(char32_t, atomic_char32_t);
P00_ENCAPSULATE_ATOMIC(wchar_t, atomic_wchar_t);
P00_ENCAPSULATE_ATOMIC(int_least8_t, atomic_int_least8_t);
P00_ENCAPSULATE_ATOMIC(uint_least8_t, atomic_uint_least8_t);
P00_ENCAPSULATE_ATOMIC(int_least16_t, atomic_int_least16_t);
P00_ENCAPSULATE_ATOMIC(uint_least16_t, atomic_uint_least16_t);
P00_ENCAPSULATE_ATOMIC(int_least32_t, atomic_int_least32_t);
P00_ENCAPSULATE_ATOMIC(uint_least32_t, atomic_uint_least32_t);
P00_ENCAPSULATE_ATOMIC(int_least64_t, atomic_int_least64_t);
P00_ENCAPSULATE_ATOMIC(uint_least64_t, atomic_uint_least64_t);
P00_ENCAPSULATE_ATOMIC(int_fast8_t, atomic_int_fast8_t);
P00_ENCAPSULATE_ATOMIC(uint_fast8_t, atomic_uint_fast8_t);
P00_ENCAPSULATE_ATOMIC(int_fast16_t, atomic_int_fast16_t);
P00_ENCAPSULATE_ATOMIC(uint_fast16_t, atomic_uint_fast16_t);
P00_ENCAPSULATE_ATOMIC(int_fast32_t, atomic_int_fast32_t);
P00_ENCAPSULATE_ATOMIC(uint_fast32_t, atomic_uint_fast32_t);
P00_ENCAPSULATE_ATOMIC(int_fast64_t, atomic_int_fast64_t);
P00_ENCAPSULATE_ATOMIC(uint_fast64_t, atomic_uint_fast64_t);
P00_ENCAPSULATE_ATOMIC(intptr_t, atomic_intptr_t);
P00_ENCAPSULATE_ATOMIC(uintptr_t, atomic_uintptr_t);
P00_ENCAPSULATE_ATOMIC(size_t, atomic_size_t);
P00_ENCAPSULATE_ATOMIC(ptrdiff_t, atomic_ptrdiff_t);
P00_ENCAPSULATE_ATOMIC(intmax_t, atomic_intmax_t);
P00_ENCAPSULATE_ATOMIC(uintmax_t, atomic_uintmax_t);

/**
 ** @}
 **/

/**
 ** @memberof atomic_flag
 **/
#define ATOMIC_FLAG_INIT { .val = 0, }

/**
 ** @memberof atomic_flag
 **/
p99_inline
_Bool atomic_flag_test_and_set(volatile atomic_flag *object) {
  return __sync_val_compare_and_swap(&object->val, 0, 1);
}

/**
 ** @memberof atomic_flag
 **/
p99_inline
void atomic_flag_clear(volatile atomic_flag *object) {
  __sync_val_compare_and_swap(&object->val, 1, 0);
}

/**
 ** @memberof atomic_int
 **/
#define ATOMIC_VAR_INIT(V) { .val = (V), }

p99_inline
_Bool p00_is_lock_free(volatile void * objp, size_t size) {
  switch (size * CHAR_BIT) {
#ifdef UINT8_MAX
  case 8: return true;
#endif
#ifdef UINT16_MAX
  case 16: return true;
#endif
#ifdef UINT24_MAX
  case 24: return true;
#endif
#ifdef UINT32_MAX
  case 32: return true;
#endif
#ifdef UINT48_MAX
  case 48: return true;
#endif
#ifdef UINT64_MAX
  case 64: return true;
#endif
#ifdef UINT96_MAX
  case 96: return true;
#endif
#ifdef UINT128_MAX
  case 128: return true;
#endif
  default: return false;
  }
}

/**
 ** @memberof atomic_int
 **/
#define atomic_is_lock_free(OBJECT, DESIRED) p00_is_lock_free(&(OBJECT)->val, sizeof((OBJECT)->val))


/**
 ** @memberof atomic_int
 **/
#define atomic_init(OBJECT, VAL) ((void)((OBJECT)->val = (VAL)))

#define P00_STORE(N)                                                    \
do { /* empty */ }                                                      \
 while(!__sync_bool_compare_and_swap                                    \
       ((uint ## N ## _t volatile*)objp,                                \
        __sync_bool_compare_and_swap((uint ## N ## _t volatile*)objp, 0, 0), \
        (uint ## N ## _t)desired))

p99_inline
void p00_store(volatile void * objp, size_t size, uintmax_t desired) {
  switch (size * CHAR_BIT) {
#ifdef UINT8_MAX
  case 8: P00_STORE(8); break;
#endif
#ifdef UINT16_MAX
  case 16: P00_STORE(16); break;
#endif
#ifdef UINT24_MAX
  case 24: P00_STORE(24); break;
#endif
#ifdef UINT32_MAX
  case 32: P00_STORE(32); break;
#endif
#ifdef UINT48_MAX
  case 48: P00_STORE(48); break;
#endif
#ifdef UINT64_MAX
  case 64: P00_STORE(64); break;
#endif
#ifdef UINT96_MAX
  case 96: P00_STORE(96); break;
#endif
#ifdef UINT128_MAX
  case 128: P00_STORE(128); break;
#endif
  }
}

/**
 ** @memberof atomic_int
 **/
#define atomic_store(OBJECT, DESIRED) p00_store(&(OBJECT)->val, sizeof((OBJECT)->val), (DESIRED))

/**
 ** @memberof atomic_int
 **/
#define atomic_load(OBJECT) __sync_val_compare_and_swap(&(OBJECT)->val, 0, 0)

p99_inline
_Bool p00_compare_exchange(uintmax_t val, uintmax_t expected, size_t size, void* expectedP) {
  _Bool ret = (val == expected);
  if (!ret) {
    switch (size * CHAR_BIT) {
#ifdef UINT8_MAX
    case 8: *(uint8_t*)expectedP = (uint8_t)val; break;
#endif
#ifdef UINT16_MAX
    case 16: *(uint16_t*)expectedP = (uint16_t)val; break;
#endif
#ifdef UINT24_MAX
    case 24: *(uint24_t*)expectedP = (uint24_t)val; break;
#endif
#ifdef UINT32_MAX
    case 32: *(uint32_t*)expectedP = (uint32_t)val; break;
#endif
#ifdef UINT48_MAX
    case 48: *(uint48_t*)expectedP = (uint48_t)val; break;
#endif
#ifdef UINT64_MAX
    case 64: *(uint64_t*)expectedP = (uint64_t)val; break;
#endif
#ifdef UINT96_MAX
    case 96: *(uint96_t*)expectedP = (uint96_t)val; break;
#endif
#ifdef UINT128_MAX
    case 128: *(uint128_t*)expectedP = (uint128_t)val; break;
#endif
    }
  }
  return ret;
}

/**
 ** @memberof atomic_int
 **/
#define atomic_compare_exchange_weak(OBJECT, EXPECTED, DESIRED)         \
p00_compare_exchange                                                    \
(__sync_val_compare_and_swap(&(OBJECT)->val, *(EXPECTED), (uintmax_t)(DESIRED)), \
 *(EXPECTED),                                                           \
 sizeof(*(EXPECTED)),                                                   \
 (EXPECTED))


/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_add(OBJECT, OPERAND) __sync_fetch_and_add(&(OBJECT)->val, OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_sub(OBJECT, OPERAND) __sync_fetch_and_sub(&(OBJECT)->val, OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_or(OBJECT, OPERAND) __sync_fetch_and_or(&(OBJECT)->val, OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_and(OBJECT, OPERAND) __sync_fetch_and_and(&(OBJECT)->val, OPERAND)

/**
 ** @memberof atomic_int
 **/
#define atomic_fetch_xor(OBJECT, OPERAND) __sync_fetch_and_xor(&(OBJECT)->val, OPERAND)

struct p00_thrd {
  pthread_t id;
  int ret;
  atomic_flag detached;
  union {
    struct {
      thrd_start_t func;
      void *arg;
    } init;
    jmp_buf jmp;
  } ovrl;
};

/**
 ** @addtogroup thread_enum
 ** @{
 **/

// The enumeration constants are
enum {
  mtx_plain = 0,
  // which is passed to mtx_init to create a mutex object that supports neither timeout nor
  // test and return;
  mtx_recursive = 1,
  // which is passed to mtx_init to create a mutex object that supports recursive locking;
  mtx_timed = 2,
  // which is passed to mtx_init to create a mutex object that supports timeout;
};

enum {
  thrd_timedout = ETIMEDOUT,
  // which is returned by a timed wait function to indicate that the time specified in the call
  // was reached without acquiring the requested resource;
  thrd_success = 0,
  // which is returned by a function to indicate that the requested operation succeeded;
  thrd_busy = EBUSY,
  // which is returned by a function to indicate that the requested operation failed because a
  // resource requested by a test and return function is already in use;
  thrd_error = EINVAL,
  // which is returned by a function to indicate that the requested operation failed; and
  thrd_nomem = ENOMEM
  // which is returned by a function to indicate that the requested operation failed because it
  // was unable to allocate memory.
};

/**
 ** @}
 **/

// 7.26.2 Initialization functions

/**
 ** @memberof once_flag
 **/
p99_inline
void call_once(once_flag *flag, void (*func)(void)) {
  (void)pthread_once(&flag->val, func);
}

// 7.26.3 Condition variable functions

/**
 ** @memberof cnd_t
 **/
p99_inline
int cnd_broadcast(cnd_t *cond) {
  return pthread_cond_broadcast(&cond->val) ? thrd_error : thrd_success;
}

/**
 ** @memberof cnd_t
 **/
p99_inline
void cnd_destroy(cnd_t *cond) {
  (void)pthread_cond_destroy(&cond->val);
}

/**
 ** @memberof cnd_t
 **/
p99_inline
int cnd_init(cnd_t *cond) {
  int ret = pthread_cond_init(&cond->val, 0);
  switch (ret) {
  case 0:         return thrd_success;
  case ENOMEM:    return thrd_nomem;
  default:        return thrd_error;
  }
}

/**
 ** @memberof cnd_t
 **/
p99_inline
int cnd_signal(cnd_t *cond) {
  return pthread_cond_signal(&cond->val) ? thrd_error : thrd_success;
}

/**
 ** @memberof cnd_t
 **/
p99_inline
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mtx, const struct timespec *restrict ts) {
  int ret = pthread_cond_timedwait(&cond->val, &mtx->val, ts);
  switch (ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

/**
 ** @memberof cnd_t
 **/
p99_inline
int cnd_wait(cnd_t *cond, mtx_t *mtx) {
  return pthread_cond_wait(&cond->val, &mtx->val) ? thrd_error : thrd_success;
}

// 7.26.4 Mutex functions

/**
 ** @memberof mtx_t
 **/
p99_inline
void mtx_destroy(mtx_t *mtx) {
  (void)pthread_mutex_destroy(&mtx->val);
}

/**
 ** @memberof mtx_t
 **/
p99_inline
int mtx_init(mtx_t *mtx, int type) {
  pthread_mutexattr_t attr;
  int ret = pthread_mutexattr_init(&attr);
  if (ret) return thrd_error;
  ret = pthread_mutexattr_settype(&attr, (type & mtx_recursive) ? PTHREAD_MUTEX_NORMAL : PTHREAD_MUTEX_RECURSIVE);
  if (ret) return thrd_error;
  ret = pthread_mutex_init(&mtx->val, &attr);
  if (ret) return thrd_error;
  else return thrd_success;
}

/**
 ** @memberof mtx_t
 **/
p99_inline
int mtx_lock(mtx_t *mtx) {
  return pthread_mutex_lock(&mtx->val) ? thrd_error : thrd_success;
}

/**
 ** @memberof mtx_t
 **/
p99_inline
int mtx_timedlock(mtx_t *restrict mtx, const struct timespec *restrict ts) {
  int ret = pthread_mutex_timedlock(&mtx->val, ts);
  switch (ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

/**
 ** @memberof mtx_t
 **/
p99_inline
int mtx_trylock(mtx_t *mtx) {
  int ret = pthread_mutex_trylock(&mtx->val);
  switch (ret) {
  case 0:         return thrd_success;
  case EBUSY:     return thrd_busy;
  default:        return thrd_error;
  };
}

/**
 ** @memberof mtx_t
 **/
p99_inline
int mtx_unlock(mtx_t *mtx) {
  return pthread_mutex_unlock(&mtx->val) ? thrd_error : thrd_success;
}

_Thread_local p00_thrd * p00_cntxt;

static
void * p00_thrd_create(void* context);

/**
 ** @memberof thrd_t
 **/
p99_inline
static
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
  p00_thrd * cntxt = malloc(sizeof *cntxt);
  if (!cntxt) return thrd_nomem;
  *cntxt = (p00_thrd const) {
    .ovrl = {
      .init = {
        .func = func,
        .arg = arg,
      },
    },
    .detached = ATOMIC_FLAG_INIT,
   };
  int ret = pthread_create(&cntxt->id, 0, p00_thrd_create, cntxt);
  if (P99_UNLIKELY(ret)) {
    free(cntxt);
    switch (ret) {
    case ENOMEM:    return thrd_nomem;
    default:        return thrd_error;
    };
  } else {
    thr->val = cntxt;
    return thrd_success;
  }
}

/**
 ** @memberof thrd_t
 **/
p99_inline
thrd_t thrd_current(void) {
  return (thrd_t) { .val = p00_cntxt, };
}

/**
 ** @memberof thrd_t
 **/
p99_inline
int thrd_detach(thrd_t thr) {
  /* The thread is not yet detached so its pthread id is still
     valid. If it already has finished, this will just free the
     resources that pthread holds for it. */
  int ret = pthread_detach(thr.val->id) ? thrd_error : thrd_success;
  if (atomic_flag_test_and_set(&thr.val->detached)) {
    /* The thread has already finished. Free the state, since nobody
       will join it, anyhow. */
    free(thr.val);
    return thrd_success;
  } else {
    return ret;
  }
}

/**
 ** @memberof thrd_t
 **/
p99_inline
int thrd_equal(thrd_t thr0, thrd_t thr1) {
  return thr0.val ==  thr1.val;
}

/**
 ** @memberof thrd_t
 **/
p99_inline
_Noreturn
void thrd_exit(int res) {
  p00_thrd * cntxt = p00_cntxt;
  if (P99_LIKELY(cntxt)) {
    cntxt->ret = res;
    longjmp(cntxt->ovrl.jmp, 1);
  } else {
    // should never be reached
    pthread_exit(0);
  }
}

/**
 ** @memberof thrd_t
 **/
p99_inline
int thrd_join(thrd_t thr, int *res) {
  void *res0;
  if (P99_UNLIKELY(pthread_join(thr.val->id, &res0))) return thrd_error;
  if (res) *res = thr.val->ret;
  free(thr.val);
  return thrd_success;
}

/**
 ** @memberof thrd_t
 **/
p99_inline
int thrd_sleep(const struct timespec *duration, struct timespec *remaining) {
  errno = 0;
  int ret = nanosleep(duration, remaining);
  if (ret) {
    ret = errno == EINTR ? -1 : -2;
    errno = 0;
  }
  return ret;
}

/**
 ** @memberof thrd_t
 **/
p99_inline
void thrd_yield(void) {
  if (P99_UNLIKELY(sched_yield())) errno = 0;
}

/**
 ** @memberof tss_t
 **/
p99_inline
int tss_create(tss_t *key, tss_dtor_t dtor) {
  return pthread_key_create(&key->val, dtor) ? thrd_error : thrd_success;
}

/**
 ** @memberof tss_t
 **/
p99_inline
void tss_delete(tss_t key) {
  (void)pthread_key_delete(key.val);
}

/**
 ** @memberof tss_t
 **/
p99_inline
void *tss_get(tss_t key) {
  return pthread_getspecific(key.val);
}

/**
 ** @memberof tss_t
 **/
p99_inline
int tss_set(tss_t key, void *val) {
  return pthread_setspecific(key.val, val) ? thrd_error : thrd_success;
}

static
void * p00_thrd_create(void* context) {
  p00_thrd * cntxt = context;
  p00_cntxt = cntxt;
  {
    thrd_start_t func = cntxt->ovrl.init.func;
    void * arg = cntxt->ovrl.init.arg;
    if (!setjmp(cntxt->ovrl.jmp)) {
      cntxt->ret = func(arg);
    }
    if (atomic_flag_test_and_set(&cntxt->detached)) {
      free(cntxt);
    }
  }
  p00_cntxt = 0;
  return 0;
}

/**
 ** @}
 **/

#endif
