/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_thread.h"
#include "orwl_sem.h"
#include "orwl_atomic.h"
#include "p99_posix_default.h"

#if defined(__linux__) && !defined(NO_FUTEX)
# include <linux/futex.h>
# include <sys/time.h>
# include <sys/syscall.h>

/* syscall is a va_arg function, so we would never be sure of the
   type conversions that could take place. Clearly specify the
   interface. */
inline
int orwl_futex(int *uaddr, int op, int val, const struct timespec *timeout,
          int *uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}

#define orwl_futex(...) P99_CALL_DEFARG(orwl_futex, 6, __VA_ARGS__)
#define orwl_futex_defarg_3() ((void*)0)
#define orwl_futex_defarg_4() 0
#define orwl_futex_defarg_5() 0

/* Wait until the value to which @a uaddr points to is equal to @a
   expected. */
inline
int orwl_futex_wait(int* uaddr, int expected) {
  for (;;) {
    int val = *uaddr;
    if (val == expected) return 0;
    if (orwl_futex(uaddr, FUTEX_WAIT, val) < 0) {
      switch (errno) {
      case EWOULDBLOCK: continue;
      case EINTR: continue;
      }
      int ret = errno;
      errno = 0;
      return ret;
    }
  }
}

/* Wakeup waiters for address @a uaddr.

   If there are @a wakeup waiters, as much waiters are woke up. If
   there are less, all waiters are woken up. */
inline
int orwl_futex_wake(int* uaddr, int wakeup) {
  int ret = orwl_futex(uaddr, FUTEX_WAKE, wakeup);
  return ret;
}

/* Wakeup one waiter for address @a uaddr, if there is any. */
inline
int orwl_futex_signal(int* uaddr) {
  return orwl_futex_wake(uaddr, 1);
}

/* Wakeup all waiters for address @a uaddr, if there are any. */
inline
int orwl_futex_broadcast(int* uaddr) {
  return orwl_futex_wake(uaddr, INT_MAX);
}

P99_INSTANTIATE(int, orwl_futex, int *uaddr, int op, int val, const struct timespec *timeout,
                int *uaddr2, int val3);
P99_INSTANTIATE(int, orwl_futex_wait, int* uaddr, int expected);
P99_INSTANTIATE(int, orwl_futex_wake, int* uaddr, int wakeup);
P99_INSTANTIATE(int, orwl_futex_signal, int* uaddr);
P99_INSTANTIATE(int, orwl_futex_broadcast, int* uaddr);

#define HAVE_FUTEX 1
#endif

size_t const orwl_mynum = ~(size_t)0;
size_t orwl_np = ~(size_t)0;
size_t orwl_phase = 0;

/**
 ** @brief Account the @c pthread_rwlock_t @a COUNT during execution of a
 ** dependent block or statement.
 **/
P99_BLOCK_DOCUMENT
#define ACCOUNT(COUNT) P99_PROTECTED_BLOCK(orwl_count_lock(&COUNT), orwl_count_unlock(&COUNT))

void orwl_report(size_t mynum, size_t np, size_t phase, char const* format, ...) {
  static char const form0[] = "%s:%zX: %s\n";
  static char const form1[] = "%0*zX/%zX:%08zX: %s\n";
  static size_t const headstart =
    (sizeof(form0) < sizeof(form1) ? sizeof(form1) : sizeof(form0))
    + sizeof(uintmax_t)/4 /* for the thread id */
    + 8  /* for the phase  id */;
  size_t const headlen = headstart + strlen(format);
  char *head = malloc(headlen);
  if (mynum == (size_t)-1)
    snprintf(head, headlen, form0,
             PTHREAD2STR(pthread_self()),
             phase, format);
  else
    snprintf(head, headlen, form1,
            (np < 0x10 ? 1 : (np < 0x100 ? 2 : (np < 0x1000 ? 3 : 4))),
            mynum, np, phase, format);
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, head, ap);
  free(head);
}

void orwl_progress(size_t t, size_t mynum, size_t np, size_t phase, char const* format, ...) {
  static char const form0[] = "%0*jX:%08zX: (%c) %s\r";
  static char const form1[] = "%0*zX/%zX:%08zX: (%c) %s\r";
  static char const img[] = "|\\-/";
  static size_t const headstart =
    (sizeof(form0) < sizeof(form1) ? sizeof(form1) : sizeof(form0))
    + sizeof(uintmax_t)/4 /* for the thread id */
    + 8  /* for the phase  id */;
  size_t const headlen = headstart + strlen(format);
  char *head = malloc(headlen);
  if (mynum == (size_t)-1)
    snprintf(head, headlen, form0,
             (int)(sizeof(uintmax_t)/4),
             (uintmax_t)pthread_self(),
             phase, img[t%4], format);
  else
    snprintf(head, headlen, form1,
             (np < 0x10 ? 1 : (np < 0x100 ? 2 : (np < 0x1000 ? 3 : 4))),
             mynum, np, phase, img[t%4], format);
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, head, ap);
  free(head);
}


static pthread_attr_t pthread_attr_detached_;
static pthread_attr_t  pthread_attr_joinable_;
static pthread_mutexattr_t pthread_mutexattr_thread_;
static pthread_mutexattr_t pthread_mutexattr_process_;
static pthread_condattr_t pthread_condattr_thread_;
static pthread_condattr_t pthread_condattr_process_;
static pthread_rwlockattr_t pthread_rwlockattr_thread_;
static pthread_rwlockattr_t pthread_rwlockattr_process_;

pthread_attr_t  const*const pthread_attr_detached = &pthread_attr_detached_;
pthread_attr_t  const*const pthread_attr_joinable = &pthread_attr_joinable_;
pthread_mutexattr_t const*const pthread_mutexattr_thread = &pthread_mutexattr_thread_;
pthread_mutexattr_t const*const pthread_mutexattr_process = &pthread_mutexattr_process_;
pthread_condattr_t const*const pthread_condattr_thread = &pthread_condattr_thread_;
pthread_condattr_t const*const pthread_condattr_process = &pthread_condattr_process_;
pthread_rwlockattr_t const*const pthread_rwlockattr_thread = &pthread_rwlockattr_thread_;
pthread_rwlockattr_t const*const pthread_rwlockattr_process = &pthread_rwlockattr_process_;

#if (defined(ATOMIC_OPS) && ATOMIC_OPS > 0) || (defined(__GNUC__) && (!defined(GNUC_NO_SYNC) || defined(GNUC_SYNC_REPLACE)))
# define HAVE_ATOMIC
#endif

/* The following is needed to communicate termination of
 * detached threads.
 *
 * The counter is realized by a semaphore to ensure that thread
 * startup is as fast as possible. A call to sem_post should be the
 * most efficient operation for such a case.
 *
 * The wait routine on the other hand then has to check for the value
 * 0 of the semaphore. Semaphores are not made for this and we can't
 * wait on just using the semaphore. Thus we use a mutex / cond pair
 * to use pthread signaling through conditions.
 */
typedef struct orwl_count orwl_count;
struct orwl_count {
  volatile union { atomic_size_t large; int narrow; } overl;
#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
  pthread_mutex_t mut;
  pthread_cond_t cnd;
#endif
};

#if !defined(HAVE_FUTEX) || !defined(HAVE_ATOMIC)
# define ORWL_COUNT_INITIALIZER                 \
{                                               \
  .overl = { .large = 0 },                      \
    .mut = PTHREAD_MUTEX_INITIALIZER,           \
       .cnd = PTHREAD_COND_INITIALIZER          \
}
#else
# define ORWL_COUNT_INITIALIZER                 \
{                                               \
  .overl = { .large = 0 },                      \
}
#endif

inline void orwl_count_lock(orwl_count* counter);
inline void orwl_count_unlock(orwl_count* counter);
inline int orwl_count_wait(orwl_count* counter);

static orwl_count counter = ORWL_COUNT_INITIALIZER;



#ifdef HAVE_ATOMIC
#ifndef HAVE_FUTEX
inline
void orwl_count_lock(orwl_count* counter) {
  atomic_fetch_add(&counter->overl.large, (size_t)1);
}

inline
void orwl_count_unlock(orwl_count* counter) {
  /* if we are the last notify the waiters */
  size_t val = atomic_fetch_sub(&counter->overl.large, (size_t)1);
  if (P99_UNLIKELY(val == 1))
    MUTUAL_EXCLUDE(counter->mut)
      pthread_cond_broadcast(&counter->cnd);
}

inline
int orwl_count_wait(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    while (atomic_load(&counter->overl.large)) pthread_cond_wait(&counter->cnd, &counter->mut);
  }
  return 0;
}

# else

inline
void orwl_count_lock(orwl_count* counter) {
  atomic_fetch_add(&counter->overl.large, (size_t)1);
}

inline
void orwl_count_unlock(orwl_count* counter) {
  /* if we are the last notify the waiters */
  register int val = atomic_fetch_sub(&counter->overl.large, (size_t)1);
  if (P99_UNLIKELY(val == 1))
    orwl_futex_broadcast((int*)&(counter->overl.narrow));
}

inline
int orwl_count_wait(orwl_count* counter) {
  int ret = 0;
  while (true) {
    if (!atomic_load(&counter->overl.large)) break;
    /* futexes unfortunately only work on int */
    /* we chose the least significant one to be sure to capture an
       intermittent change of the value */
    ret = orwl_futex_wait((int*)&(counter->overl.narrow), 0);
    if (ret) break;
    /* Check if really the whole counter is 0, so we iterate. */
  }
  return ret;
}
#endif
#else

inline
void orwl_count_lock(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    ++(counter->overl.large);
  }
}

inline
void orwl_count_unlock(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    --(counter->overl.large);
    if (!(counter->overl.large)) pthread_cond_broadcast(&counter->cnd);
  }
}

inline
int orwl_count_wait(orwl_count* counter) {
  MUTUAL_EXCLUDE(counter->mut) {
    while (counter->overl.large) pthread_cond_wait(&counter->cnd, &counter->mut);
  }
  return 0;
}
#endif

P99_INSTANTIATE(void, orwl_count_lock, orwl_count*);
P99_INSTANTIATE(void, orwl_count_unlock, orwl_count*);
P99_INSTANTIATE(int, orwl_count_wait, orwl_count*);


DEFINE_ONCE(pthread_mutex_t) {
  pthread_mutexattr_init(&pthread_mutexattr_process_);
#if defined(POSIX_THREAD_PROCESS_SHARED) && (POSIX_THREAD_PROCESS_SHARED > 0)
  pthread_mutexattr_setpshared(&pthread_mutexattr_process_, PTHREAD_PROCESS_SHARED);
#else
  pthread_mutexattr_setpshared(&pthread_mutexattr_process_, PTHREAD_PROCESS_PRIVATE);
#endif
  pthread_mutexattr_init(&pthread_mutexattr_thread_);
  pthread_mutexattr_setpshared(&pthread_mutexattr_thread_, PTHREAD_PROCESS_PRIVATE);
}

DEFINE_ONCE(pthread_cond_t) {
  pthread_condattr_init(&pthread_condattr_process_);
#if defined(POSIX_THREAD_PROCESS_SHARED) && (POSIX_THREAD_PROCESS_SHARED > 0)
  pthread_condattr_setpshared(&pthread_condattr_process_, PTHREAD_PROCESS_SHARED);
#else
  pthread_condattr_setpshared(&pthread_condattr_process_, PTHREAD_PROCESS_PRIVATE);
#endif
  pthread_condattr_init(&pthread_condattr_thread_);
  pthread_condattr_setpshared(&pthread_condattr_thread_, PTHREAD_PROCESS_PRIVATE);
}

DEFINE_ONCE(pthread_rwlock_t) {
  pthread_rwlockattr_init(&pthread_rwlockattr_process_);
#if defined(POSIX_THREAD_PROCESS_SHARED) && (POSIX_THREAD_PROCESS_SHARED > 0)
  pthread_rwlockattr_setpshared(&pthread_rwlockattr_process_, PTHREAD_PROCESS_SHARED);
#else
  pthread_rwlockattr_setpshared(&pthread_rwlockattr_process_, PTHREAD_PROCESS_PRIVATE);
#endif
  pthread_rwlockattr_init(&pthread_rwlockattr_thread_);
  pthread_rwlockattr_setpshared(&pthread_rwlockattr_thread_, PTHREAD_PROCESS_PRIVATE);
}

DEFINE_ONCE(orwl_thread) {
  INIT_ONCE(pthread_mutex_t);
  INIT_ONCE(pthread_cond_t);
  INIT_ONCE(pthread_rwlock_t);
  pthread_attr_init(&pthread_attr_detached_);
  pthread_attr_setdetachstate(&pthread_attr_detached_, PTHREAD_CREATE_DETACHED);
  pthread_attr_init(&pthread_attr_joinable_);
  pthread_attr_setdetachstate(&pthread_attr_joinable_, PTHREAD_CREATE_JOINABLE);
}

int orwl_pthread_create_joinable(pthread_t *restrict thread,
                                 start_routine_t start_routine,
                                 void *restrict arg) {
  INIT_ONCE(orwl_thread);
  return pthread_create(thread, pthread_attr_joinable, start_routine, arg);
}

/* The joinable case is a bit more involved. We wrap another function
 * around the user thread function since we have to do some
 * preparation and clean up to do.
 *
 * Therefore we need a @c struct that combines the function pointer
 * and its argument.
 */

typedef struct {
  orwl_sem semCaller;
  orwl_sem semCalled;
  start_routine_t start_routine;
  void *arg;
} orwl__routine_arg;


orwl__routine_arg* orwl__routine_arg_init(orwl__routine_arg *rt,
                                              start_routine_t start_routine,
                                              void* arg) {
  if (!rt) return 0;
  orwl_sem_init(&rt->semCaller, 0u);
  orwl_sem_init(&rt->semCalled, 0u);
  rt->start_routine = start_routine;
  rt->arg = arg;
  return rt;
}

P99_PROTOTYPE(orwl__routine_arg*, orwl__routine_arg_init, orwl__routine_arg *, start_routine_t, void*);
#define orwl__routine_arg_init(...) P99_CALL_DEFARG(orwl__routine_arg_init, 3, __VA_ARGS__)

P99_DECLARE_DEFARG(orwl__routine_arg_init, , P99_0(start_routine_t), P99_0(void*));
P99_DEFINE_DEFARG(orwl__routine_arg_init, , P99_0(start_routine_t), P99_0(void*));

void orwl__routine_arg_destroy(orwl__routine_arg *rt) {
  /* empty */
}

DECLARE_NEW_DELETE(orwl__routine_arg);
DEFINE_NEW_DELETE(orwl__routine_arg);

static
void *detached_wrapper(void *routine_arg) {
  orwl__routine_arg *Routine_Arg = routine_arg;
  start_routine_t start_routine = Routine_Arg->start_routine;
  void *restrict arg = Routine_Arg->arg;
  /* This should be fast since usually there should never be a waiter
     blocked on this semaphore. */
  void *ret = 0;
  ACCOUNT(counter) {
    /* tell the creator that we are in charge */
    orwl_sem_post(&Routine_Arg->semCalled);
    ret = start_routine(arg);
  }
  /* wait if the creator might still be needing the semaphore */
  orwl_sem_wait(&Routine_Arg->semCaller);
  /* Be careful to eliminate all garbage that the wrapping has
     generated. */
  orwl__routine_arg_delete(Routine_Arg);
  Routine_Arg = 0;
  routine_arg = 0;
  return ret;
}


int orwl_pthread_create_detached(start_routine_t start_routine,
                                 void *restrict arg) {
  INIT_ONCE(orwl_thread);
  /* Be sure to allocate the pair on the heap to leave full control
     to detached_wrapper() of what to do with it. */
  orwl__routine_arg *Routine_Arg = P99_NEW(orwl__routine_arg, start_routine, arg);
  int ret = pthread_create(&P99_LVAL(pthread_t),
                           pthread_attr_detached,
                           detached_wrapper,
                           Routine_Arg);
  /* Wait until the routine is accounted for */
  orwl_sem_wait(&Routine_Arg->semCalled);
  /* Notify that Routine_Arg may safely be deleted thereafter */
  orwl_sem_post(&Routine_Arg->semCaller);
  return ret;
}

void orwl_pthread_wait_detached(void) {
  INIT_ONCE(orwl_thread);
  orwl_count_wait(&counter);
}

P99_INSTANTIATE(pthread_t*, pthread_t_init, pthread_t *);
P99_INSTANTIATE(void, pthread_t_destroy, pthread_t *);

DEFINE_NEW_DELETE(pthread_t);

void sleepfor(double t) {
  double const mega = 1E+9;
  double const nano = 1E-9;
  while (t > 0.0) {
    double sec = trunc(t);
    struct timespec rem = P99_INIT;
    struct timespec req = {
      .tv_sec = (time_t)sec,
      .tv_nsec = (time_t)((t - sec) * mega)
    };
    if (!nanosleep(&req, &rem)) return;
    sec = (double)rem.tv_sec + nano*rem.tv_nsec;
    if (sec >= t) return;
    t = sec;
  }
}

P99_INSTANTIATE(char const*, pthread2str, char*, pthread_t);
