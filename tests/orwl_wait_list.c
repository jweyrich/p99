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
static size_t handles = 4;
static size_t threads = 1;

typedef struct _arg_t {
  size_t mynum;
  size_t np;
  size_t handles;
} arg_t;

#define report(F, ...) fprintf(stderr, "%lu/%lu: " F, (ulong)mynum, (ulong)np, __VA_ARGS__)

void *start_thread(void* arg) {
  arg_t *Arg = (arg_t*)arg;
  size_t mynum = Arg->mynum;
  size_t np = Arg->np;
  size_t handles = Arg->handles;
  double quot;
  {
    struct drand48_data buffer;
    srand48_r(mynum * np, &buffer);
    drand48_r(&buffer, &quot);
  }
  size_t const twait = 1000000;
  size_t rwait = (size_t)(twait * quot);
  size_t await = twait - rwait;
  report("%lu handles\n", (ulong)handles);
  orwl_wait_request(handle[mynum], location);
  report("req, %lu handles\n", (ulong)handles);
  usleep(rwait);
  orwl_wait_acquire(handle[mynum]);
  report("acq, %lu handles\n", (ulong)handles);
  usleep(await);
  orwl_wait_release(handle[mynum]);
  report("rel, %lu handles\n", (ulong)handles);
  return NULL;
}


int main(int argc, char **argv) {
  size_t i;

  if (argc > 1) handles = strtoul(argv[1], NULL, 0);
  if (argc > 2) threads = strtoul(argv[2], NULL, 0);

  fprintf(stderr, "%s: starting with %lu handles and %lu threads\n",
          argv[0], handles, threads);
  /* Initialization of the static location */
  location = orwl_wq_new();
  handle = orwl_wh_vnew(handles);

  pthread_t *id = malloc(threads * sizeof(pthread_t));
  arg_t *arg = malloc(threads * sizeof(arg_t));

  for (i = 0; i < threads; ++i) {
    arg[i].mynum = i;
    arg[i].np = threads;
    arg[i].handles = handles;
    pthread_create(id + i, NULL, start_thread, arg + i);
  }

  for (i = 0; i < threads; ++i) {
    pthread_join(id[i], NULL);
  }

  return 0;
}
