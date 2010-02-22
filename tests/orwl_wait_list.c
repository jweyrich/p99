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

void *start_thread(void* arg) {
  arg_t const*const Arg = (arg_t*)arg;
  size_t const mynum = Arg->mynum;
  size_t const np = Arg->np;
  size_t const phases = Arg->phases;
  size_t phase;
  /* The buffer for the reentrant pseudo random generator */
  unsigned short xsubi[3] = { mynum, np, time(NULL) };

  for (phase = 0; phase < phases; ++phase) {
    size_t const twait = 10000;
    size_t const iwait = twait / 10;
    orwl_state ostate = orwl_invalid;
    size_t rwait = (size_t)(twait * erand48(xsubi));
    size_t await = twait - rwait;
    size_t try;
    for (try = 0; ostate != orwl_acquired; ++try) {
      ostate = orwl_wait_request(handle[threadof(mynum + (phase>>1)) + (phase % 2)*np],
                                 location);
      if (ostate == orwl_requested) break;
      progress(!mynum, try, "req, handle %lu, %s\n",
               (ulong)(threadof(mynum + (phase>>1)) + (phase % 2)*np),
               orwl_state_name[ostate]);
      usleep(await);
    }
    report(!mynum, "req, handle %lu, %s\n",
           (ulong)(threadof(mynum + (phase>>1)) + (phase % 2)*np),
           orwl_state_name[ostate]);
    ostate = orwl_invalid;
    for (try = 0; ostate != orwl_acquired; ++try) {
      ostate = orwl_wait_acquire(handle[mynum + (phase % 2)*np]);
      if (ostate == orwl_acquired) break;
      progress(!mynum, try, "acq, handle %lu, state %s, waiting for %lu\r",
               (ulong)(mynum + np), orwl_state_name[ostate],
               threadof(mynum - phase));
      usleep(iwait);
    }
    report(!mynum, "acq, handle %lu, state %s                            \n",
           (ulong)(mynum + np), orwl_state_name[ostate]);
    usleep(rwait);
    orwl_wait_release(handle[mynum + (phase % 2)*np]);
    report(!mynum, "rel, handle %lu\n", (ulong)(mynum + (phase % 2)*np));
  }
  return NULL;
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
    pthread_create(id + i, NULL, start_thread, arg + i);
  }

  for (i = 0; i < threads; ++i) {
    pthread_join(id[i], NULL);
  }

  return 0;
}
