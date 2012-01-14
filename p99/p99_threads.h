/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2011-2012 Jens Gustedt, INRIA, France                      */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_THREADS_H
#define P99_THREADS_H 1

#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#if defined(__GNUC__) || defined(P00_DOXYGEN)
# include "p99_atomic.h"
#endif

/**
 ** @addtogroup threads C11 thread emulation on top of POSIX threads
 **
 ** This is a relatively straight forward implementation of the C11
 ** thread model on top of POSIX threads. The main difficulty in that
 ** is that the thread entry function signature differs between the
 ** two. C11 thread returns an <code>int</code> whereas POSIX returns
 ** a <code>void*</code>.
 **
 ** You find the thread management interfaces through the
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
 ** of type ::once_flag
 **
 ** This implementation here has the particularity that this
 ** initialization is equivalent to the default initialization by @c
 ** 0.  This specific property is then use in the fallback
 ** implementation of thread local storage.
 **
 ** @memberof once_flag
 **/
#define ONCE_FLAG_INIT P99_INIT

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
P99_ENC_DECLARE(pthread_cond_t, cnd_t);

typedef struct p00_thrd p00_thrd;
/**
 ** @brief complete object type that holds an identifier for a thread
 **/
P99_ENC_DECLARE(struct p00_thrd*, thrd_t);

/**
 ** @brief complete object type that holds an identifier for a
 ** thread-specific storage pointer
 **/
P99_ENC_DECLARE(pthread_key_t, tss_t);
/**
 ** @brief complete object type that holds an identifier for a mutex
 **/
P99_ENC_DECLARE(pthread_mutex_t, mtx_t);

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

typedef struct once_flag once_flag;

enum p00_once {
  p00_once_uninit = 0,
  p00_once_started,
  p00_once_finished,
};

/**
 ** @brief complete object type that holds a flag for use by
 ** ::call_once
 **
 ** From the wording of the standard it is not clear if a variable of
 ** this type @b must be initialized by means of ::ONCE_FLAG_INIT. The
 ** corresponding POSIX structure requires the analog.
 **
 ** Therefore we don't use the POSIX structure, here, but cook this
 ** ourselves with atomic variables. By that we can guarantee that a
 ** ::once_flag that is initialized by the default initializer always
 ** has the correct state.
 */
struct once_flag {
  union {
    enum p00_once done;
    enum p00_once volatile volatile_done;
  } done;
  thrd_t id;
  atomic_flag flg;
};


/**
 ** @}
 **/

struct p00_thrd {
  pthread_t id;
  size_t foreign;
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
   ** @brief passed to ::mtx_init to create a mutex object that supports recursive locking
   **/
  mtx_recursive = 1,
  /**
   ** @brief passed to ::mtx_init to create a mutex object that supports timeout
   **/
  mtx_timed = 2,
};

/**
 ** @brief C11 thread function return values
 **/
enum thrd_status {
  /**
   ** @brief returned by a timed wait function to indicate that the time specified in the call was reached without acquiring the requested resource
   **/
  thrd_timedout = ETIMEDOUT,
  /**
   ** @brief returned by a function to indicate that the requested operation succeeded
   **/
  thrd_success = 0,
  /**
   ** @brief returned by a function to indicate that the requested
   ** operation failed because a resource requested by a test and
   ** return function is already in use
   **/
  thrd_busy = EBUSY,
  /**
   ** @brief returned by a function to indicate that the requested operation failed
   **/
  thrd_error = INT_MIN,
  /**
   ** @brief returned by a function to indicate that the requested
   ** operation failed because it was unable to allocate memory
   **/
  thrd_nomem = ENOMEM,
  /**
   ** @brief (extension) returned by ::thrd_sleep to indicate that the
   ** corresponding request has been interrupted by a signal
   **/
  thrd_intr = -1
};

/**
 ** @}
 **/

// 7.26.2 Initialization functions

/**
 ** @memberof thrd_t
 **/
p99_inline
void thrd_yield(void) {
  if (P99_UNLIKELY(sched_yield())) errno = 0;
}

p99_inline thrd_t thrd_current(void);
p99_inline int thrd_equal(thrd_t thr0, thrd_t thr1);

/**
 ** @memberof once_flag
 **/
p99_inline
void call_once(once_flag *flag, void (*func)(void)) {
  if (P99_UNLIKELY(flag->done.done != p00_once_finished))
    do {
      atomic_flag_lock(&flag->flg);
      switch (flag->done.volatile_done) {
        /* we are doing the initialization */
      case p00_once_uninit:
        flag->done.done = 1;
        flag->id = thrd_current();
        atomic_flag_unlock(&flag->flg);
        func();
        flag->done.done = 2;
        break;
      case p00_once_started:
        if (thrd_equal(flag->id, thrd_current())) {
          /* we are called recursively, abandon and return */
          atomic_flag_unlock(&flag->flg);
          return;
        }
        /* otherwise fall through */
      case p00_once_finished:
        atomic_flag_unlock(&flag->flg);
        break;
      }
    } while (flag->done.volatile_done != p00_once_finished);
}

/**
 ** @brief Call a function @a func exactly once by providing it with
 ** argument @a arg
 **
 ** This is an extension of the standard function ::call_once that
 ** doesn't allow to pass an argument to @a func.
 **
 ** @memberof once_flag
 **/
p99_inline
void p99_call_once(once_flag *flag, void (*func)(void*), void* arg) {
  if (P99_UNLIKELY(flag->done.done != p00_once_finished))
    do {
      atomic_flag_lock(&flag->flg);
      switch (flag->done.volatile_done) {
        /* we are doing the initialization */
      case p00_once_uninit:
        flag->done.done = 1;
        flag->id = thrd_current();
        atomic_flag_unlock(&flag->flg);
        func(arg);
        flag->done.done = 2;
        break;
      case p00_once_started:
        if (thrd_equal(flag->id, thrd_current())) {
          /* we are called recursively, abandon and return */
          atomic_flag_unlock(&flag->flg);
          return;
        }
        /* otherwise fall through */
      case p00_once_finished:
        atomic_flag_unlock(&flag->flg);
        break;
      }
    } while (flag->done.volatile_done != p00_once_finished);
}

P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_INIT_ONCE, 0)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_INIT_ONCE, 1)
P00_DOCUMENT_IDENTIFIER_ARGUMENT(P99_DECLARE_INIT_ONCE, 2)
#define P99_DECLARE_INIT_ONCE(T, NAME, ARG)                     \
/** @remark wrapper type around a T that is initialized once */ \
struct NAME {                                                   \
  once_flag p00_once;                                           \
  T p00_val;                                                    \
};                                                              \
P99_DECLARE_STRUCT(NAME);                                       \
p99_inline                                                      \
void P99_PASTE3(p00_, NAME, _init_func)(T* ARG);                \
p99_inline                                                      \
void P99_PASTE3(p00_, NAME, _init_once)(NAME* ARG) {            \
  if (P99_UNLIKELY(!ARG->p00_once.done.done))                   \
    do {                                                        \
      atomic_flag_lock(&ARG->p00_once.flg);                     \
      if (!ARG->p00_once.done.volatile_done) {                  \
        P99_PASTE3(p00_, NAME, _init_func)(&ARG->p00_val);      \
        ARG->p00_once.done.volatile_done = true;                \
      }                                                         \
      atomic_flag_unlock(&ARG->p00_once.flg);                   \
    } while (!ARG->p00_once.done.volatile_done);                \
}                                                               \
p99_inline                                                      \
void P99_PASTE3(p00_, NAME, _init_func)(T* ARG)

#define P99_INIT_ONCE(NAME, VARP) P99_PASTE3(p00_, NAME, _init_once)(VARP)

/**
 ** @memberof tss_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **
 ** If successful, sets the thread-specific storage pointed to by key
 ** to a value that uniquely identifies the newly created
 ** pointer. Otherwise, the thread-specific storage pointed to by key
 ** is set to an undefined value.
 **/
p99_inline
int tss_create(tss_t *key, tss_dtor_t dtor) {
  return pthread_key_create(&P99_ENCP(key), dtor) ? thrd_error : thrd_success;
}

/**
 ** @memberof tss_t
 **/
p99_inline
void tss_delete(tss_t key) {
  (void)pthread_key_delete(P99_ENC(key));
}

/**
 ** @memberof tss_t
 **
 ** @return the value for the current thread if successful, or @c 0 if
 ** unsuccessful.
 **/
p99_inline
void *tss_get(tss_t key) {
  return pthread_getspecific(P99_ENC(key));
}

/**
 ** @memberof tss_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int tss_set(tss_t key, void *val) {
  return pthread_setspecific(P99_ENC(key), val) ? thrd_error : thrd_success;
}

#if defined(thread_local) && !defined(P99_EMULATE_THREAD_LOCAL)

#define P99_DECLARE_THREAD_LOCAL(T, NAME)                      \
P99_WEAK(NAME)                                                 \
thread_local T NAME

#define P99_THREAD_LOCAL(NAME) (NAME)

#else
/**
 ** @brief A stub structure to hold a thread local variable if
 ** ::thread_local is not available.
 **
 ** Don't use this type directly but use ::P99_DECLARE_THREAD_LOCAL to
 ** declare a variable and ::P99_THREAD_LOCAL to access it.
 **
 ** A hypothetical example for the use of such a variable would be @c
 ** errno:
 **
 ** @code
 ** P99_DECLARE_THREAD_LOCAL(int, errno_loc);
 ** #define errno P99_THREAD_LOCAL(errno_loc)
 ** @endcode
 **
 ** With just these two lines @c errno always evaluates to an lvalue
 ** representing a thread local object. That is you can use it
 ** everywhere a normal variable of type @c int could be used:
 **
 ** @code
 ** if (errno == EINTR) ...
 ** errno = 0;
 ** my_func(&errno);
 ** @endcode
 **/
P99_DECLARE_INIT_ONCE(tss_t, p99_tss, key) {
  int ret = tss_create(key, free);
  if (ret) {
    errno = ret;
    perror("can't create thread specific key");
    abort();
  }
}

p99_inline
void* p00_thread_local_get(p99_tss * key, size_t size) {
  P99_INIT_ONCE(p99_tss, key);
  void * ret = tss_get(P99_ENCP(key));
  if (P99_UNLIKELY(!ret)) {
    ret = calloc(1, size);
    tss_set(P99_ENCP(key), ret);
  }
  return ret;
}

/**
 ** @brief declare a thread local variable @a NAME of type @a T
 **
 ** @remark such a variable must be declared in global scope
 **
 ** @see P99_THREAD_LOCAL to access the variable
 ** @memberof p99_tss
 **/
#define P99_DECLARE_THREAD_LOCAL(T, NAME)                      \
/** @see P99_THREAD_LOCAL to access the variable */            \
P99_WEAK(NAME)                                                 \
p99_tss NAME;                                                  \
typedef T P99_PASTE3(p00_, NAME, _type)

/**
 ** @brief an lvalue expression that returns the thread local instance
 ** of variable @a NAME
 **
 ** @see P99_DECLARE_THREAD_LOCAL to declare the variable
 ** @memberof p99_tss
 **/
#define P99_THREAD_LOCAL(NAME) (*(P99_PASTE3(p00_, NAME, _type)*)p00_thread_local_get(&(NAME), sizeof(P99_PASTE3(p00_, NAME, _type))))

#endif

/* Tentative definitions for global variables. This has the advantage
   that this defines weak symbols and we avoid to have to create a
   specific library. */
P99_DECLARE_THREAD_LOCAL(p00_thrd *, p00_thrd_local);

#define P00_THRD_LOCAL P99_THREAD_LOCAL(p00_thrd_local)

// 7.26.3 Condition variable functions

/**
 ** @memberof cnd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int cnd_broadcast(cnd_t *cond) {
  return pthread_cond_broadcast(&P99_ENCP(cond)) ? thrd_error : thrd_success;
}

/**
 ** @memberof cnd_t
 **/
p99_inline
void cnd_destroy(cnd_t *cond) {
  (void)pthread_cond_destroy(&P99_ENCP(cond));
}

/**
 ** @memberof cnd_t
 **
 ** @return The ::cnd_init function returns ::thrd_success on success,
 ** or ::thrd_nomem if no memory could be allocated for the newly
 ** created condition, or ::thrd_error if the request could not be
 ** honored.
 **/
p99_inline
int cnd_init(cnd_t *cond) {
  int ret = pthread_cond_init(&P99_ENCP(cond), 0);
  switch (ret) {
  case 0:         return thrd_success;
  case ENOMEM:    return thrd_nomem;
  default:        return thrd_error;
  }
}

/**
 ** @memberof cnd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int cnd_signal(cnd_t *cond) {
  return pthread_cond_signal(&P99_ENCP(cond)) ? thrd_error : thrd_success;
}

/**
 ** @memberof cnd_t
 **
 ** @return ::thrd_success upon success, or ::thrd_timedout if the
 ** time specified in the call was reached without acquiring the
 ** requested resource, or ::thrd_error if the request could not be
 ** honored.
 **/
p99_inline
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mtx, const struct timespec *restrict ts) {
  int ret = pthread_cond_timedwait(&P99_ENCP(cond), &P99_ENCP(mtx), ts);
  switch (ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

/**
 ** @memberof cnd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int cnd_wait(cnd_t *cond, mtx_t *mtx) {
  return pthread_cond_wait(&P99_ENCP(cond), &P99_ENCP(mtx)) ? thrd_error : thrd_success;
}

// 7.26.4 Mutex functions

/**
 ** @memberof mtx_t
 **/
p99_inline
void mtx_destroy(mtx_t *mtx) {
  (void)pthread_mutex_destroy(&P99_ENCP(mtx));
}

/**
 ** @memberof mtx_t
 **
 ** @param mtx A pointer to an unitialized mutex object
 ** @param type One of the constants in ::mtx_type
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int mtx_init(mtx_t *mtx, int type) {
  pthread_mutexattr_t attr;
  int ret = pthread_mutexattr_init(&attr);
  if (ret) return thrd_error;
  ret = pthread_mutexattr_settype(&attr, (type & mtx_recursive) ? PTHREAD_MUTEX_NORMAL : PTHREAD_MUTEX_RECURSIVE);
  if (ret) return thrd_error;
  ret = pthread_mutex_init(&P99_ENCP(mtx), &attr);
  if (ret) return thrd_error;
  else return thrd_success;
}

/**
 ** @memberof mtx_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int mtx_lock(mtx_t *mtx) {
  return pthread_mutex_lock(&P99_ENCP(mtx)) ? thrd_error : thrd_success;
}

/**
 ** @memberof mtx_t
 **
 ** @return ::thrd_success upon success, or ::thrd_timedout if the
 ** time specified in the call was reached without acquiring the
 ** requested resource, or ::thrd_error if the request could not be
 ** honored.
 **/
p99_inline
int mtx_timedlock(mtx_t *restrict mtx, const struct timespec *restrict ts) {
  int ret = pthread_mutex_timedlock(&P99_ENCP(mtx), ts);
  switch (ret) {
  case 0:         return thrd_success;
  case ETIMEDOUT: return thrd_timedout;
  default:        return thrd_error;
  };
}

/**
 ** @memberof mtx_t
 **
 ** @return ::thrd_success on success, or ::thrd_busy if the resource
 ** requested is already in use, or ::thrd_error if the request could
 ** not be honored.
 **/
p99_inline
int mtx_trylock(mtx_t *mtx) {
  int ret = pthread_mutex_trylock(&P99_ENCP(mtx));
  switch (ret) {
  case 0:         return thrd_success;
  case EBUSY:     return thrd_busy;
  default:        return thrd_error;
  };
}

/**
 ** @memberof mtx_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int mtx_unlock(mtx_t *mtx) {
  return pthread_mutex_unlock(&P99_ENCP(mtx)) ? thrd_error : thrd_success;
}

/**
 ** @brief Protect the following block or statement with @c
 ** mtx_t @a MUT.
 **
 ** @see P99_CRITICAL for a tool that uses a spinlock that is
 ** allocated behind the scene.
 **
 ** This does some rudimentary error checking for the result of the
 ** locking. If an error occurs the whole block and any other
 ** enclosing blocks that protected with P99_UNWIND_PROTECT are
 ** aborted.
 **/
P99_BLOCK_DOCUMENT
#define P99_MUTUAL_EXCLUDE(MUT)                                         \
P00_BLK_START                                                           \
P00_BLK_DECL(int, p00_errNo, 0)                                         \
P99_GUARDED_BLOCK(mtx_t*,                                               \
                  P99_FILEID(mut),                                      \
                  &(MUT),                                               \
                  (void)(P99_UNLIKELY(p00_errNo = mtx_lock(P99_FILEID(mut))) \
                         && (fprintf(stderr,                            \
                                     __FILE__ ":"                       \
                                     P99_STRINGIFY(__LINE__) ": lock error for " \
                                     P99_STRINGIFY(MUT) ", %s",         \
                                     strerror(p00_errNo)), 1)           \
                         && (P99_FILEID(mut) = 0, 1)                    \
                         && (P99_UNWIND(-1), 1)                         \
                         ),                                             \
                  (void)(P99_FILEID(mut)                                \
                         && mtx_unlock(P99_FILEID(mut))))



P99_SETJMP_INLINE(p00_thrd_create)
void * p00_thrd_create(void* context);

/**
 ** @memberof thrd_t
 **
 ** @return ::thrd_success on success, or ::thrd_nomem if no memory
 ** could be allocated for the thread requested, or ::thrd_error if
 ** the request could not be honored.
 **/
p99_inline
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
    P99_ENCP(thr) = cntxt;
    return thrd_success;
  }
}

P99_WEAK(p00_foreign_threads)
_Atomic(size_t) p00_foreign_threads;

/**
 ** @memberof thrd_t
 **
 ** @return identifier of the thread that called it
 **/
p99_inline
thrd_t thrd_current(void) {
  p00_thrd * loc = P00_THRD_LOCAL;
  if (P99_UNLIKELY(!loc)) {
    size_t foreign = atomic_fetch_add(&p00_foreign_threads, 1);
    loc = malloc(sizeof *loc);
    *loc = (p00_thrd) {
      .id = pthread_self(),
      .foreign = foreign + 1,
    };
    P00_THRD_LOCAL = loc;
    if (foreign) fprintf(stderr, "foreign thread %lu is %zu\n", loc->id, foreign + 1);
  }
  return (thrd_t)P99_ENC_INIT(loc);
}

/**
 ** @memberof thrd_t
 **
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int thrd_detach(thrd_t thr) {
  /* The thread is not yet detached so its pthread id is still
     valid. If it already has finished, this will just free the
     resources that pthread holds for it. */
  int ret = pthread_detach(P99_ENC(thr)->id) ? thrd_error : thrd_success;
  if (atomic_flag_test_and_set(&P99_ENC(thr)->detached)) {
    /* The thread has already finished. Free the state, since nobody
       will join it, anyhow. */
    free(P99_ENC(thr));
    return thrd_success;
  } else {
    return ret;
  }
}

/**
 ** @memberof thrd_t
 **
 ** @return @c 0 if the thread @a thr0 and the thread @a thr1 refer to
 ** different threads. Otherwise a nonzero value is returned.
 **/
p99_inline
int thrd_equal(thrd_t thr0, thrd_t thr1) {
  return P99_ENC(thr0) ==  P99_ENC(thr1);
}

/**
 ** @memberof thrd_t
 **/
p99_inline
_Noreturn
void thrd_exit(int res) {
  p00_thrd * cntxt = P00_THRD_LOCAL;
  if (P99_LIKELY(cntxt)) {
    if (cntxt->foreign) {
      cntxt->ret = res;
      longjmp(cntxt->ovrl.jmp, 1);
    } else {
      free(cntxt);
      P00_THRD_LOCAL = 0;
    }
  }
  /* should only be reached by threads that where created directly
     with pthreads, e.g main */
  pthread_exit(0);
}

/**
 ** @memberof thrd_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int thrd_join(thrd_t thr, int *res) {
  void *res0;
  if (P99_UNLIKELY(pthread_join(P99_ENC(thr)->id, &res0))) return thrd_error;
  if (res) *res = P99_ENC(thr)->ret;
  free(P99_ENC(thr));
  return thrd_success;
}

/**
 ** @memberof thrd_t
 **
 ** @return @c 0 if the requested time has elapsed, @c -1 if it has
 ** been interrupted by a signal, or a negative value if it fails.
 ** Consistently with that, this implementation uses ::thrd_success,
 ** ::thrd_intr and ::thrd_error as return values.
 **/
p99_inline
int thrd_sleep(const struct timespec *duration, struct timespec *remaining) {
  errno = 0;
  int ret = nanosleep(duration, remaining);
  if (ret) {
    ret = (errno == EINTR) ? thrd_intr : thrd_error;
    errno = 0;
    return ret;
  } else return thrd_success;
}

/*
  This is static inline because of the following message from gcc:

   sorry, unimplemented: function ‘p00_thrd_create’ can never be inlined because it uses setjmp
*/
P99_SETJMP_INLINE(p00_thrd_create)
void * p00_thrd_create(void* context) {
  p00_thrd * cntxt = context;
  P00_THRD_LOCAL = cntxt;
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
  P00_THRD_LOCAL = 0;
  return 0;
}

/**
 ** @}
 **/

#endif
