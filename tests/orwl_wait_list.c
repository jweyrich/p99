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
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "orwl_thread.h"
#include "orwl_wait_queue.h"
#include "orwl_callback.h"

static orwl_wq location;
static orwl_wh *handle = NULL;
static size_t phases = 4;

#define threadof(x) ((((size_t)x) + orwl_np) % orwl_np)

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

void FUNC_DEFAULT(size_t_init)(size_t *size, size_t def) {
  *size = def;
}

#define size_t_init(...) DEFINE_FUNC_DEFAULT(size_t_init, 2, __VA_ARGS__)
declare_default_arg(size_t_init, 1, size_t, 0);
define_default_arg(size_t_init, 1, size_t);

void size_t_destroy(size_t *arg) {
  /* empty */
}

DECLARE_NEW_DELETE(size_t);
DEFINE_NEW_DELETE(size_t);

typedef struct {
  size_t mynum;
  size_t phase;
} cb_t;

void cb_t_init(cb_t *cb) {
  cb->mynum = 0;
  cb->phase = 0;
}

void cb_t_destroy(cb_t *cb) {
  /* empty */
}

DECLARE_NEW_DELETE(cb_t);
DEFINE_NEW_DELETE(cb_t);

DECLARE_CALLBACK(cb_t);

DEFINE_CALLBACK(cb_t) {
  if (!Arg->mynum)
    fprintf(stderr, "callback %zu phase %zu\n",
            Arg->mynum, Arg->phase);
}

typedef struct _arg_t {
  size_t mynum;
  size_t phases;
} arg_t;

void FUNC_DEFAULT(arg_t_init)(arg_t *arg, size_t def) {
  arg->mynum = def;
  arg->phases = def;
}

#define arg_t_init(...) DEFINE_FUNC_DEFAULT(arg_t_init, 2, __VA_ARGS__)
declare_default_arg(arg_t_init, 1, size_t, 0);
define_default_arg(arg_t_init, 1, size_t);


void arg_t_destroy(arg_t *arg) {
}

DECLARE_NEW_DELETE(arg_t);
DEFINE_NEW_DELETE(arg_t);

DECLARE_THREAD(arg_t);

DEFINE_THREAD(arg_t) {
  size_t const orwl_mynum = Arg->mynum;
  size_t const phases = Arg->phases;
  /* The buffer for the reentrant pseudo random generator */
  unsigned short xsubi[3] = { orwl_mynum, orwl_np, time(NULL) };

  for (size_t orwl_phase = 0; orwl_phase < phases; ++orwl_phase) {
    double const twait = 0.01;
    double const iwait = twait / 10.0;
    double const rwait = twait * erand48(xsubi);
    double const await = twait - rwait;
    /* the position to be requested */
    size_t preq = threadof(orwl_mynum + (orwl_phase>>1)) + (orwl_phase % 2)*orwl_np;
    /* the postion where we put the callback and that we acquire */
    size_t pacq = orwl_mynum + (orwl_phase % 2)*orwl_np;
    orwl_state ostate =
      orwl_wq_request(&location, handle + preq, 1);
    report(!orwl_mynum,  "req, handle %zu, %s",
           preq, orwl_state_getname(ostate));
    /**/
    cb_t *cb = cb_t_new();
    cb->mynum = orwl_mynum;
    cb->phase = orwl_phase;
    ostate = orwl_invalid;
    for (size_t try = 0; ostate != orwl_requested; ++try) {
      ostate = orwl_wh_test(handle + pacq);
      if (ostate == orwl_requested || ostate == orwl_acquired) break;
      progress(!orwl_mynum,  try, "test, handle %zu, %s",
               pacq, orwl_state_getname(ostate));
      sleepfor(iwait);
    }
    report(!orwl_mynum, "test, handle %zu, %s",
           pacq, orwl_state_getname(ostate));
    orwl_callback_attach_cb_t(cb, handle + pacq);
    /**/
    sleepfor(await);
    ostate = orwl_wh_acquire(handle + pacq);
    report(!orwl_mynum,  "acq, handle %zu, state %s                            ",
           pacq, orwl_state_getname(ostate));
    sleepfor(rwait);
    orwl_wh_release(handle + pacq);
    report(!orwl_mynum,  "rel, handle %zu", pacq);
  }
}


int main(int argc, char **argv) {
  if (argc > 1) phases = strtoul(argv[1], NULL, 0);
  if (argc > 2) orwl_np = strtoul(argv[2], NULL, 0);

  report(1, "%s: starting with %zu phases and %zu threads",
          argv[0], phases, orwl_np);
  /* Initialization of the static location */
  orwl_wq_init(&location);
  handle = orwl_wh_vnew(2 * orwl_np);

  /* Half of the threads are created detached and half joinable */
  pthread_t *id = pthread_t_vnew(orwl_np/2);
  arg_t *arg = arg_t_vnew(orwl_np/2);

  for (size_t i = 0; i < orwl_np; ++i) {
    arg_t *myarg = (i%2 ? arg + (i/2) : arg_t_new());
    myarg->mynum = i;
    myarg->phases = phases;
    if (i%2)
      arg_t_create(myarg, id + (i/2));
    else
      arg_t_create(myarg, NULL);
  }


  report(1, "%s: waiting for %zu joinable threads",
         argv[0], orwl_np/2);
  for (size_t i = 0; i < orwl_np/2; ++i) {
    arg_t_join(id[i]);
  }

  report(1, "%s: waiting for %zu detached threads",
          argv[0], orwl_np - orwl_np/2);
  orwl_pthread_wait_detached();

  report(1, "freeing arg");
  arg_t_vdelete(arg);
  report(1, "freeing id");
  pthread_t_vdelete(id);
  report(1, "freeing handle");
  orwl_wh_vdelete(handle);
  report(1, "destroying location");
  orwl_wq_destroy(&location);
  return 0;
}
