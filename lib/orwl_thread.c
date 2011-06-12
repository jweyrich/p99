/* This may look like nonsense, but it really is -*- mode: C -*-             */
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
#include "orwl_count.h"

size_t const orwl_mynum = ~(size_t)0;
size_t orwl_np = ~(size_t)0;
size_t orwl_phase = 0;

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

static orwl_count counter = ORWL_COUNT_INITIALIZER;

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

DEFINE_ONCE(orwl_thread,
            orwl_gettime,
            pthread_mutex_t,
            pthread_cond_t,
            pthread_rwlock_t) {
  pthread_attr_init(&pthread_attr_detached_);
  pthread_attr_setdetachstate(&pthread_attr_detached_, PTHREAD_CREATE_DETACHED);
  pthread_attr_init(&pthread_attr_joinable_);
  pthread_attr_setdetachstate(&pthread_attr_joinable_, PTHREAD_CREATE_JOINABLE);
}

P99_INSTANTIATE(orwl_thread_cntrl*, orwl_thread_cntrl_init, orwl_thread_cntrl*);
P99_INSTANTIATE(void, orwl_thread_cntrl_destroy, orwl_thread_cntrl*);
P99_INSTANTIATE(void, orwl_thread_cntrl_freeze, orwl_thread_cntrl*);
P99_INSTANTIATE(void, orwl_thread_cntrl_detach, orwl_thread_cntrl*);
P99_INSTANTIATE(void, orwl_thread_cntrl_wait_for_caller, orwl_thread_cntrl*);
P99_INSTANTIATE(void, orwl_thread_cntrl_wait_for_callee, orwl_thread_cntrl*);

DEFINE_DELETE(orwl_thread_cntrl);

int orwl_pthread_create_joinable(pthread_t *restrict thread,
                                 start_routine_t start_routine,
                                 void *restrict arg) {
  return pthread_create(thread, pthread_attr_joinable, start_routine, arg);
}

/* The detached cases are a bit more involved. We wrap another function
 * around the user thread function since we have to do some
 * preparation and clean up to do.
 *
 * Therefore we need a @c struct that combines the function pointer
 * and its argument.
 */

typedef struct {
  orwl_thread_cntrl *det;
  bool own;
  start_routine_t start_routine;
  void *arg;
} o_rwl_launch_arg;

static
o_rwl_launch_arg* o_rwl_launch_arg_init(o_rwl_launch_arg *rt,
                                        start_routine_t start_routine,
                                        void* arg,
                                        orwl_thread_cntrl* det) {
  if (!rt) return 0;
  *rt = (o_rwl_launch_arg){
    .det = det ? det : P99_NEW(orwl_thread_cntrl),
    .own = !det,
    .start_routine = start_routine,
    .arg = arg
  };
  return rt;
}

static
P99_PROTOTYPE(o_rwl_launch_arg*, o_rwl_launch_arg_init, o_rwl_launch_arg *, start_routine_t, void*, orwl_thread_cntrl*);
#define o_rwl_launch_arg_init(...) P99_CALL_DEFARG(o_rwl_launch_arg_init, 4, __VA_ARGS__)

#define o_rwl_launch_arg_init_defarg_1() P99_0(start_routine_t)
#define o_rwl_launch_arg_init_defarg_2() P99_0(void*)
#define o_rwl_launch_arg_init_defarg_3() P99_0(orwl_thread_cntrl*)

static
void o_rwl_launch_arg_destroy(o_rwl_launch_arg *rt) {
  /* wait if the creator might still be needing the semaphore */
  if (rt->own) {
    orwl_thread_cntrl_wait_for_caller(rt->det);
    orwl_thread_cntrl_delete(rt->det);
    rt->det = 0;
  }
}

DECLARE_NEW_DELETE(o_rwl_launch_arg, static);
DEFINE_NEW_DELETE(o_rwl_launch_arg, static);

static
void *o_rwl_launch_arg_wrapper(void *routine_arg) {
  o_rwl_launch_arg *Routine_Arg = routine_arg;
  orwl_thread_cntrl *det = Routine_Arg->det;
  start_routine_t start_routine = Routine_Arg->start_routine;
  void *restrict arg = Routine_Arg->arg;
  /* This should be fast since usually there should never be a waiter
     blocked on this semaphore. */
  void *ret = 0;
  ORWL_ACCOUNT(counter) {
    /* The application routine must call orwl_thread_cntrl_freeze to unblock the
       caller. */
    if (Routine_Arg->own) orwl_thread_cntrl_freeze(det);
    ret = start_routine(arg);
  }
  /* Be careful to eliminate all garbage that the wrapping has
     generated. */
  o_rwl_launch_arg_delete(Routine_Arg);
  Routine_Arg = 0;
  routine_arg = 0;
  return ret;
}


int orwl_pthread_launch(start_routine_t start_routine,
                               void *restrict arg,
                               orwl_thread_cntrl* det) {
  /* Be sure to allocate the pair on the heap to leave full control
     to o_rwl_launch_arg_wrapper() of what to do with it. */
  o_rwl_launch_arg *Routine_Arg = P99_NEW(o_rwl_launch_arg, start_routine, arg, det);
  int ret = pthread_create(&P99_LVAL(pthread_t),
                           pthread_attr_detached,
                           o_rwl_launch_arg_wrapper,
                           Routine_Arg);
  if (!det) {
    /* Wait until the routine is accounted for */
    orwl_thread_cntrl_wait_for_callee(Routine_Arg->det);
    /* Notify that Routine_Arg may safely be deleted thereafter */
    orwl_thread_cntrl_detach(Routine_Arg->det);
  }
  return ret;
}

P99_INSTANTIATE(int, orwl_pthread_create_detached, start_routine_t, void *restrict);

void orwl_pthread_wait_detached(void) {
  orwl_count_wait(&counter);
}

P99_INSTANTIATE(pthread_t*, pthread_t_init, pthread_t *);
P99_INSTANTIATE(void, pthread_t_destroy, pthread_t *);

DEFINE_NEW_DELETE(pthread_t);

P99_INSTANTIATE(char const*, pthread2str, char*, pthread_t);
