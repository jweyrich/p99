/*
** orwl_thread.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Feb 24 11:38:27 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdint.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "orwl_thread.h"
#include "orwl_once.h"
#include "orwl_new.h"

size_t const orwl_mynum = ~(size_t)0;
size_t orwl_np = ~(size_t)0;
size_t orwl_phase = 0;

void orwl_report(size_t mynum, size_t np, size_t phase, char const* format, ...) {
  static char const form0[] = "%0*jX:%08zX: %s\n";
  static char const form1[] = "%0*zX/%zX:%08zX: %s\n";
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


static pthread_attr_t attr_detached;
static pthread_attr_t attr_joinable;

/* The following three are needed to communicate termination of
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
static sem_t create_sem;
static pthread_mutex_t create_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t create_cond = PTHREAD_COND_INITIALIZER;

DEFINE_ONCE(orwl_pthread_create) {
  sem_init(&create_sem, 0, 0);
  pthread_attr_init(&attr_detached);
  pthread_attr_setdetachstate(&attr_detached, PTHREAD_CREATE_DETACHED);
  pthread_attr_init(&attr_joinable);
  pthread_attr_setdetachstate(&attr_joinable, PTHREAD_CREATE_JOINABLE);
}

int orwl_pthread_create_joinable(pthread_t *restrict thread,
                                 void *(*start_routine)(void*),
                                 void *restrict arg) {
  INIT_ONCE(orwl_pthread_create);
  return pthread_create(thread, &attr_joinable, start_routine, arg);
}

/* The joinable case is a bit more involved. We wrap another function
 * around the user thread function since we have to do some
 * preparation and clean up to do.
 *
 * Therefore we need a @c struct that combines the function pointer
 * and its argument.
 */

typedef struct {
  void *(*start_routine)(void*);
  void *arg;
} _routine_arg;

void _routine_arg_init(_routine_arg *rt) {
  rt->start_routine = NULL;
  rt->arg = NULL;
}

void _routine_arg_destroy(_routine_arg *rt) {
  /* empty */
}

DECLARE_NEW_DELETE(_routine_arg);
DEFINE_NEW_DELETE(_routine_arg);

static
void *detached_wrapper(void *routine_arg) {
  _routine_arg *Routine_Arg = routine_arg;
  void *(*start_routine)(void*) = Routine_Arg->start_routine;
  void *restrict arg = Routine_Arg->arg;
  /* Be careful to eliminate all garbage that the wrapping has
     generated. */
  _routine_arg_delete(Routine_Arg);
  Routine_Arg = NULL;
  routine_arg = NULL;
  /* This should be fast since usually there should never be a waiter
     block on this semaphore. */
  sem_post(&create_sem);
  /* Do the real work */
  void *ret = start_routine(arg);
  /* This should return immediately, since we ourselves have posted
     a token */
  sem_wait(&create_sem);
  /* The remaining part could be a bit slower but usually only at the
     very end of the program and in a situation where a lot of
     threads return simultaneously. */
  pthread_mutex_lock(&create_mutex);
  pthread_cond_broadcast(&create_cond);
  pthread_mutex_unlock(&create_mutex);
  return ret;
}


int orwl_pthread_create_detached(void *(*start_routine)(void*),
                                 void *restrict arg) {
  INIT_ONCE(orwl_pthread_create);
  /* Be sure to allocate the pair on the heap to leave full control
     to detached_wrapper() of what to do with it. */
  _routine_arg *Routine_Arg = _routine_arg_new();
  Routine_Arg->start_routine = start_routine;
  Routine_Arg->arg = arg;
  return pthread_create(&(pthread_t){0},
                           &attr_detached,
                           detached_wrapper,
                           Routine_Arg);
}

void orwl_pthread_wait_detached(void) {
  INIT_ONCE(orwl_pthread_create);
  pthread_mutex_lock(&create_mutex);
  for (int sval = 1;;) {
    sem_getvalue(&create_sem, &sval);
    if (!sval) break;
    pthread_cond_wait(&create_cond, &create_mutex);
  }
  pthread_mutex_unlock(&create_mutex);
}

void pthread_t_init(pthread_t *id);
void pthread_t_destroy(pthread_t *id);

DEFINE_NEW_DELETE(pthread_t);

void sleepfor(double t) {
  double const mega = 1E+9;
  double const nano = 1E-9;
  while (t > 0.0) {
    double sec = trunc(t);
    struct timespec rem = {
      .tv_sec = 0,
      .tv_nsec = 0
    };
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
