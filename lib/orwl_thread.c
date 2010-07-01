/*
** orwl_thread.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Feb 24 11:38:27 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_thread.h"

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


static pthread_attr_t attr_detached;
static pthread_attr_t attr_joinable;

/* The following three are needed to communicate termination of
 * detached threads.
 *
 * The counter is realized by a semaphore to ensure that thread
 * startup is as fast as possible. A call to orwl_sem_post should be the
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
  orwl_sem_init(&create_sem, 0, 0);
  pthread_attr_init(&attr_detached);
  pthread_attr_setdetachstate(&attr_detached, PTHREAD_CREATE_DETACHED);
  pthread_attr_init(&attr_joinable);
  pthread_attr_setdetachstate(&attr_joinable, PTHREAD_CREATE_JOINABLE);
}

int orwl_pthread_create_joinable(pthread_t *restrict thread,
                                 start_routine_t start_routine,
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
  sem_t semCaller;
  sem_t semCalled;
  start_routine_t start_routine;
  void *arg;
} orwl__routine_arg;



orwl__routine_arg* orwl__routine_arg_init(orwl__routine_arg *rt,
                                              start_routine_t start_routine,
                                              void* arg) {
  orwl_sem_init(&rt->semCaller, 0, 0);
  orwl_sem_init(&rt->semCalled, 0, 0);
  rt->start_routine = start_routine;
  rt->arg = arg;
  return rt;
}

PROTOTYPE(orwl__routine_arg*, orwl__routine_arg_init, orwl__routine_arg *, start_routine_t, void*);
#define orwl__routine_arg_init(...) CALL_WITH_DEFAULTS(orwl__routine_arg_init, 3, __VA_ARGS__)

DECLARE_DEFARG(orwl__routine_arg_init, , NULL, NULL);
DEFINE_DEFARG(orwl__routine_arg_init, , NULL, NULL);

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
  void *ret = INITIALIZER;
  SEM_RELAX(create_sem) {
    /* tell the creator that we are in charge */
    orwl_sem_post(&Routine_Arg->semCalled);
    ret = start_routine(arg);
  }
  /* The remaining part could be a bit slower but usually only at the
     very end of the program and in a situation where a lot of
     threads return simultaneously. */
  MUTUAL_EXCLUDE(create_mutex)
    pthread_cond_broadcast(&create_cond);
  /* wait if the creator might still be needing the semaphore */
  orwl_sem_wait(&Routine_Arg->semCaller);
  /* Be careful to eliminate all garbage that the wrapping has
     generated. */
  orwl__routine_arg_delete(Routine_Arg);
  Routine_Arg = NULL;
  routine_arg = NULL;
  return ret;
}


int orwl_pthread_create_detached(start_routine_t start_routine,
                                 void *restrict arg) {
  INIT_ONCE(orwl_pthread_create);
  /* Be sure to allocate the pair on the heap to leave full control
     to detached_wrapper() of what to do with it. */
  orwl__routine_arg *Routine_Arg = NEW(orwl__routine_arg, start_routine, arg);
  int ret = pthread_create(&(pthread_t){0},
                           &attr_detached,
                           detached_wrapper,
                           Routine_Arg);
  /* Wait until the routine is accounted for */
  orwl_sem_wait(&Routine_Arg->semCalled);
  /* Notify that Routine_Arg may safely be deleted thereafter */
  orwl_sem_post(&Routine_Arg->semCaller);
  return ret;
}

void orwl_pthread_wait_detached(void) {
  INIT_ONCE(orwl_pthread_create);
  MUTUAL_EXCLUDE(create_mutex) {
    for (int sval = 1;;) {
      orwl_sem_getvalue(&create_sem, &sval);
      if (!sval) break;
      pthread_cond_wait(&create_cond, &create_mutex);
    }
  }
}

pthread_t* pthread_t_init(pthread_t *id);
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

int orwl_sem_init(sem_t *sem, int pshared, unsigned int value);
int orwl_sem_destroy(sem_t *sem);
int orwl_sem_getvalue(sem_t *sem, int *sval);
int orwl_sem_post(sem_t *sem);
int orwl_sem_trywait(sem_t *sem);
int orwl_sem_wait(sem_t *sem);
int orwl_sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

char const* pthread2str(char *buf, pthread_t id);
