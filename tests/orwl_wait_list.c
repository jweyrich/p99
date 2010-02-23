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

static orwl_wq *location = NULL;
static orwl_wh *const*handle = NULL;
static size_t phases = 4;
static size_t threads = 1;

typedef struct _arg_t {
  size_t mynum;
  size_t np;
  size_t phases;
} arg_t;

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
    for (size_t try = 0; ostate != orwl_acquired; ++try) {
      ostate = orwl_wait_request(handle[threadof(mynum + (phase>>1)) + (phase % 2)*np],
                                 location);
      if (ostate == orwl_requested) break;
      progress(!mynum, try, "req, handle %lu, %s\n",
               (ulong)(threadof(mynum + (phase>>1)) + (phase % 2)*np),
               orwl_state_getname(ostate));
      sleepfor(await);
    }
    report(!mynum, "req, handle %lu, %s\n",
           (ulong)(threadof(mynum + (phase>>1)) + (phase % 2)*np),
           orwl_state_getname(ostate));
    ostate = orwl_invalid;
    for (size_t try = 0; ostate != orwl_acquired; ++try) {
      ostate = orwl_wait_acquire(handle[mynum + (phase % 2)*np]);
      if (ostate == orwl_acquired) break;
      progress(!mynum, try, "acq, handle %lu, state %s, waiting for %lu\r",
               (ulong)(mynum + np), orwl_state_getname(ostate),
               threadof(mynum - phase));
      sleepfor(iwait);
    }
    report(!mynum, "acq, handle %lu, state %s                            \n",
           (ulong)(mynum + np), orwl_state_getname(ostate));
    sleepfor(rwait);
    orwl_wait_release(handle[mynum + (phase % 2)*np]);
    report(!mynum, "rel, handle %lu\n", (ulong)(mynum + (phase % 2)*np));
  }
}


int main(int argc, char **argv) {
  size_t i;

  if (argc > 1) phases = strtoul(argv[1], NULL, 0);
  if (argc > 2) threads = strtoul(argv[2], NULL, 0);

  fprintf(stderr, "%s: starting with %lu phases and %lu threads\n",
          argv[0], phases, threads);
  /* Initialization of the static location */
  location = orwl_wq_new();
  handle = orwl_wh_vnew(2 * threads);

  pthread_t *id = malloc(threads * sizeof(pthread_t));
  arg_t *arg = malloc(threads * sizeof(arg_t));

  for (i = 0; i < threads; ++i) {
    arg[i].mynum = i;
    arg[i].np = threads;
    arg[i].phases = phases;
    arg_t_create(id + i, arg + i);
  }

  for (i = 0; i < threads; ++i) {
    arg_t_join(id[i]);
  }

  return 0;
}
