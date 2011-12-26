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
#include <p99_c99.h>



// macros are
#define thread_local _Thread_local
#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
// which expands to a value that can be used to initialize an object of type once_flag; and
#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
# warning "definition of PTHREAD_DESTRUCTOR_ITERATIONS is missing"
# define TSS_DTOR_ITERATIONS 1
#else
# define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS
#endif
// which expands to an integer constant expression representing the maximum number of
// times that destructors will be called when a thread terminates.

// The types are
typedef pthread_cond_t cnd_t;
// which is a complete object type that holds an identifier for a condition variable;

typedef struct p00_thrd p00_thrd;
typedef struct p00_thrd* thrd_t;

// which is a complete object type that holds an identifier for a thread;
typedef pthread_key_t tss_t;
// which is a complete object type that holds an identifier for a thread-specific storage pointer;
typedef pthread_mutex_t mtx_t;
// which is a complete object type that holds an identifier for a mutex;
typedef void (*tss_dtor_t)(void*);
// which is the function pointer type void (*)(void*), used for a destructor for a
// thread-specific storage pointer;
typedef int (*thrd_start_t)(void*);
// which is the function pointer type int (*)(void*) that is passed to thrd_create
// to create a new thread; and
typedef pthread_once_t once_flag;
// which is a complete object type that holds a flag for use by call_once.

typedef int atomic_flag;

#define ATOMIC_FLAG_INIT 0


p99_inline
_Bool atomic_flag_test_and_set(volatile atomic_flag *object) {
  return __sync_val_compare_and_swap(object, 0, 1);
}

p99_inline
void atomic_flag_clear(volatile atomic_flag *object) {
  __sync_val_compare_and_swap(object, 1, 0);
}

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

// 7.26.2 Initialization functions

p99_inline
void call_once(once_flag *flag, void (*func)(void)) {
  (void)pthread_once(flag, func);
}

// 7.26.3 Condition variable functions

p99_inline
int cnd_broadcast(cnd_t *cond) {
  return pthread_cond_broadcast(cond) ? thrd_error : thrd_success;
}

p99_inline
void cnd_destroy(cnd_t *cond) {
  (void)pthread_cond_destroy(cond);
}

p99_inline
int cnd_init(cnd_t *cond) {
  int ret = pthread_cond_init(cond, 0);
  switch (ret) {
  case 0:         return thrd_success;
  case ENOMEM:    return thrd_nomem;
  default:        return thrd_error;
  }
}

p99_inline
int cnd_signal(cnd_t *cond) {
  return pthread_cond_signal(cond) ? thrd_error : thrd_success;
}

p99_inline
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mtx, const struct timespec *restrict ts) {
  int ret = pthread_cond_timedwait(cond, mtx, ts);
  switch (ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

p99_inline
int cnd_wait(cnd_t *cond, mtx_t *mtx) {
  return pthread_cond_wait(cond, mtx) ? thrd_error : thrd_success;
}

// 7.26.4 Mutex functions

p99_inline
void mtx_destroy(mtx_t *mtx) {
  (void)pthread_mutex_destroy(mtx);
}

p99_inline
int mtx_init(mtx_t *mtx, int type) {
  pthread_mutexattr_t attr;
  int ret = pthread_mutexattr_init(&attr);
  if (ret) return thrd_error;
  ret = pthread_mutexattr_settype(&attr, (type & mtx_recursive) ? PTHREAD_MUTEX_NORMAL : PTHREAD_MUTEX_RECURSIVE);
  if (ret) return thrd_error;
  ret = pthread_mutex_init(mtx, &attr);
  if (ret) return thrd_error;
  else return thrd_success;
}

p99_inline
int mtx_lock(mtx_t *mtx) {
  return pthread_mutex_lock(mtx) ? thrd_error : thrd_success;
}

p99_inline
int mtx_timedlock(mtx_t *restrict mtx, const struct timespec *restrict ts) {
  int ret = pthread_mutex_timedlock(mtx, ts);
  switch (ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

p99_inline
int mtx_trylock(mtx_t *mtx) {
  int ret = pthread_mutex_trylock(mtx);
  switch (ret) {
  case 0:         return thrd_success;
  case EBUSY:     return thrd_busy;
  default:        return thrd_error;
  };
}

p99_inline
int mtx_unlock(mtx_t *mtx) {
  return pthread_mutex_unlock(mtx) ? thrd_error : thrd_success;
}

_Thread_local p00_thrd * p00_cntxt;

static
void * p00_thrd_create(void* context);

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
  if (ret) {
    free(cntxt);
    switch (ret) {
    case ENOMEM:    return thrd_nomem;
    default:        return thrd_error;
    };
  } else {
    *thr = cntxt;
    return thrd_success;
  }
}

p99_inline
thrd_t thrd_current(void) {
  return p00_cntxt;
}

p99_inline
int thrd_detach(thrd_t thr) {
  /* The thread is not yet detached so its pthread id is still
     valid. If it already has finished, this will just free the
     resources that pthread holds for it. */
  int ret = pthread_detach(thr->id) ? thrd_error : thrd_success;
  if (atomic_flag_test_and_set(&thr->detached)) {
    /* The thread has already finished. Free the state, since nobody
       will join it, anyhow. */
    free(thr);
    return thrd_success;
  } else {
    return ret;
  }
}

p99_inline
int thrd_equal(thrd_t thr0, thrd_t thr1) {
  return !!pthread_equal(thr0->id, thr1->id);
}

p99_inline
_Noreturn
void thrd_exit(int res) {
  p00_thrd * cntxt = p00_cntxt;
  if (cntxt) {
    cntxt->ret = res;
    longjmp(cntxt->ovrl.jmp, 1);
  } else {
    // should never be reached
    pthread_exit(0);
  }
}

p99_inline
int thrd_join(thrd_t thr, int *res) {
  void *res0;
  if (pthread_join(thr->id, &res0)) return thrd_error;
  if (res) *res = thr->ret;
  free(thr);
  return thrd_success;
}

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

p99_inline
void thrd_yield(void) {
  if (sched_yield()) errno = 0;
}

p99_inline
int tss_create(tss_t *key, tss_dtor_t dtor) {
  return pthread_key_create(key, dtor) ? thrd_error : thrd_success;
}

p99_inline
void tss_delete(tss_t key) {
  (void)pthread_key_delete(key);
}

p99_inline
void *tss_get(tss_t key) {
  return pthread_getspecific(key);
}

p99_inline
int tss_set(tss_t key, void *val) {
  return pthread_setspecific(key, val) ? thrd_error : thrd_success;
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

#endif
