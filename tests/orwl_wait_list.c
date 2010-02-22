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

#define report(F, ...)                          \
do {                                            \
  if (!mynum)                                   \
    fprintf(stderr, "%lu/%lu:%lu: " F,          \
            (ulong)mynum,                       \
            (ulong)np,                          \
            (ulong)phase, __VA_ARGS__);         \
 } while(0)

#define threadof(x) ((((size_t)x) + (size_t)np) % (size_t)np)

void *start_thread(void* arg) {
  arg_t const*const Arg = (arg_t*)arg;
  size_t const mynum = Arg->mynum;
  size_t const np = Arg->np;
  size_t phase;
  double quot;
  for (phase = 0; phase < Arg->phases; ++phase) {
    size_t const twait = 10000;
    size_t const iwait = twait / 10;
    size_t rwait;
    size_t await;
    orwl_state ostate = orwl_invalid;
    {
      struct drand48_data buffer;
      srand48_r(mynum * np, &buffer);
      drand48_r(&buffer, &quot);
    }
    {
      rwait = (size_t)(twait * quot);
      await = twait - rwait;
      do {
        ostate = orwl_wait_request(handle[threadof(mynum + phase) + (phase % 2)*np],
                                   location);
        report("req, handle %lu, %s\n",
               (ulong)(threadof(mynum + phase) + (phase % 2)*np),
               orwl_state_name[ostate]);
        usleep(await);
      } while (ostate != orwl_requested);
      ostate = orwl_invalid;
    }
    size_t try = 0;
    static char const*const img = "|\\-/";
    while (ostate != orwl_acquired) {
      ostate = orwl_wait_acquire(handle[mynum + (phase % 2)*np]);
      if (ostate == orwl_acquired) break;
      report("%c acq, handle %lu, state %s, waiting for %lu\r",
             img[try % 4],
             (ulong)(mynum + np), orwl_state_name[ostate],
             threadof(mynum - phase));
      usleep(iwait);
      ++try;
    }
    report("acq, handle %lu, state %s                            \n",
           (ulong)(mynum + np), orwl_state_name[ostate]);
    usleep(rwait);
    orwl_wait_release(handle[mynum + (phase % 2)*np]);
    report("rel, handle %lu\n", (ulong)(mynum + (phase % 2)*np));
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
