/*
** orwl_wait_list.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Feb 20 10:17:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "orwl_thread.h"
#include "orwl_wait_queue.h"
#include "orwl_callback.h"

static orwl_wq *location = NULL;
static orwl_wh *const*handle = NULL;
static size_t phases = 4;
static size_t threads = 1;

#define report(exp, F, ...)                     \
do {                                            \
  if (exp)                                      \
    fprintf(stderr, "%lu/%lu:%lu: " F,          \
            (ulong)mynum,                       \
            (ulong)np,                          \
            (ulong)phase,                       \
            __VA_ARGS__);                       \
 } while(0)


#define progress(exp, t, F, ...)                \
do {                                            \
  static char const*const img = "|\\-/";        \
  if (exp)                                      \
    fprintf(stderr, "%lu/%lu:%lu: (%c) " F,     \
            (ulong)mynum,                       \
            (ulong)np,                          \
            (ulong)phase,                       \
            img[((ulong)t) % 4],                \
            __VA_ARGS__);                       \
 } while(0)

#define threadof(x) ((((size_t)x) + (size_t)np) % (size_t)np)

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

void size_t_init(size_t *size, size_t def) {
  *size = def;
}

void size_t_destroy(size_t *arg) {
  /* empty */
}

DECLARE_NEW_DELETE(size_t, 0);
DEFINE_NEW_DELETE(size_t);

typedef struct {
  uint64_t mynum;
  uint64_t phase;
} cb_t;

void cb_t_init(cb_t *cb, uint64_t v) {
  cb->mynum = v;
  cb->phase = v;
}

void cb_t_destroy(cb_t *cb) {
  /* empty */
}

DECLARE_NEW_DELETE(cb_t, 0);
DEFINE_NEW_DELETE(cb_t);

DECLARE_CALLBACK(cb_t);

DEFINE_CALLBACK(cb_t) {
  if (!Arg->mynum) fprintf(stderr, "callback %lu phase %lu\n",
          (ulong)Arg->mynum,
          (ulong)Arg->phase);
}

typedef struct _arg_t {
  size_t mynum;
  size_t np;
  size_t phases;
} arg_t;

void arg_t_init(arg_t *arg, size_t def) {
  arg->mynum = def;
  arg->np = def;
  arg->phases = def;
}

void arg_t_destroy(arg_t *arg) {
}

DECLARE_NEW_DELETE(arg_t, 0);
DEFINE_NEW_DELETE(arg_t);

DECLARE_THREAD(arg_t);

DEFINE_THREAD(arg_t) {
  size_t const mynum = Arg->mynum;
  size_t const np = Arg->np;
  size_t const phases = Arg->phases;
  /* The buffer for the reentrant pseudo random generator */
  unsigned short xsubi[3] = { mynum, np, time(NULL) };

  for (size_t phase = 0; phase < phases; ++phase) {
    double const twait = 0.01;
    double const iwait = twait / 10.0;
    double const rwait = twait * erand48(xsubi);
    double const await = twait - rwait;
    orwl_state ostate = orwl_invalid;
    for (size_t try = 0; ostate != orwl_requested; ++try) {
      ostate = orwl_wait_request(handle[threadof(mynum + (phase>>1)) + (phase % 2)*np],
                                 location,
                                 1);
      if (ostate == orwl_requested) break;
      progress(!mynum,  try, "req, handle %lu, %s\r",
               (ulong)(threadof(mynum + (phase>>1)) + (phase % 2)*np),
               orwl_state_getname(ostate));
      sleepfor(await);
    }
    report(!mynum,  "req, handle %lu, %s\n",
           (ulong)(threadof(mynum + (phase>>1)) + (phase % 2)*np),
           orwl_state_getname(ostate));
    /**/
    cb_t *cb = cb_t_new();
    cb->mynum = mynum;
    cb->phase = phase;
    ostate = orwl_invalid;
    for (size_t try = 0; ostate != orwl_requested; ++try) {
      ostate = orwl_wait_test(handle[mynum + (phase % 2)*np], 0);
      if (ostate == orwl_requested || ostate == orwl_acquired) break;
      progress(!mynum,  try, "test, handle %lu, %s (%lu, %lu)\r",
               (ulong)(mynum + (phase % 2)*np),
               orwl_state_getname(ostate),
               (ulong)ostate,
               (ulong)orwl_requested);
      sleepfor(await);
    }
    report(!mynum, "test, handle %lu, %s\n",
           (ulong)(mynum + (phase % 2)*np),
           orwl_state_getname(ostate));
    orwl_callback_attach_cb_t(cb, handle[mynum + (phase % 2)*np]);
    /**/
    ostate = orwl_invalid;
    for (size_t try = 0; ostate != orwl_acquired; ++try) {
      ostate = orwl_wait_acquire(handle[mynum + (phase % 2)*np], 1);
      if (ostate == orwl_acquired) break;
      progress(!mynum,  try, "acq, handle %lu, state %s, waiting for %lu\r",
               (ulong)(mynum + (phase % 2)*np), orwl_state_getname(ostate),
               threadof(mynum - phase));
      sleepfor(iwait);
    }
    report(!mynum,  "acq, handle %lu, state %s                            \n",
           (ulong)(mynum + (phase % 2)*np), orwl_state_getname(ostate));
    sleepfor(rwait);
    ostate = orwl_invalid;
    for (size_t try = 0; ostate != orwl_valid; ++try) {
      ostate = orwl_wait_release(handle[mynum + (phase % 2)*np]);
      if (ostate == orwl_valid) break;
      progress(!mynum,  try, "rel, handle %lu, state %s, waiting for %lu\r",
               (ulong)(mynum + (phase % 2)*np), orwl_state_getname(ostate),
               threadof(mynum - phase));
      sleepfor(iwait);
    }
    report(!mynum,  "rel, handle %lu\n", (ulong)(mynum + (phase % 2)*np));
  }
}


int main(int argc, char **argv) {
  if (argc > 1) phases = strtoul(argv[1], NULL, 0);
  if (argc > 2) threads = strtoul(argv[2], NULL, 0);

  fprintf(stderr, "%s: starting with %lu phases and %lu threads\n",
          argv[0], phases, threads);
  /* Initialization of the static location */
  location = orwl_wq_new();
  handle = orwl_wh_vnew(2 * threads);

  /* Half of the threads are created detached and half joinable */
  pthread_t *const*id = size_t_vnew(threads/2);
  arg_t *const*arg = arg_t_vnew(threads/2);

  for (size_t i = 0; i < threads; ++i) {
    arg_t *myarg = (i%2 ? arg[i/2] : arg_t_new());
    myarg->mynum = i;
    myarg->np = threads;
    myarg->phases = phases;
    if (i%2)
      arg_t_create(myarg, id[i/2]);
    else
      arg_t_create(myarg, NULL);
  }


  fprintf(stderr, "%s: waiting for %lu joinable threads\n",
          argv[0], threads/2);
  for (size_t i = 0; i < threads/2; ++i) {
    arg_t_join(*id[i]);
  }

  fprintf(stderr, "%s: waiting for %lu detached threads\n",
          argv[0], threads - threads/2);
  orwl_pthread_wait_detached();

  fprintf(stderr, "freeing arg\n");
  arg_t_vdelete(arg);
  fprintf(stderr, "freeing id\n");
  size_t_vdelete(id);
  fprintf(stderr, "freeing handle\n");
  orwl_wh_vdelete(handle);
  fprintf(stderr, "freeing location\n");
  orwl_wq_delete(location);
  return 0;
}
