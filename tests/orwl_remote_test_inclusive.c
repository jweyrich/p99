/*
** orwl_wait_list.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Feb 20 10:17:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_remote_queue.h"
#include "orwl_callback.h"
#include "orwl_server.h"
#include "orwl_socket.h"
#include "orwl_auth_sock_functions.h"

static orwl_mirror* location_back = NULL;
static orwl_mirror* location = NULL;
static size_t phases = 4;
static size_t offset = 0;
static size_t number = 0;

#define threadof(x) ((((size_t)x) + orwl_np) % orwl_np)

typedef struct {
  size_t mynum;
  size_t phase;
} cb_t;

cb_t* cb_t_init(cb_t *cb) {
  cb->mynum = 0;
  cb->phase = 0;
  return cb;
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
  orwl_mirror* left;
} arg_t;

arg_t* arg_t_init(arg_t *arg, size_t m, size_t ph, orwl_mirror* le) {
  arg->mynum = m;
  arg->phases = ph;
  arg->left = le;
  return arg;
}

PROTOTYPE(arg_t*, arg_t_init, arg_t *, size_t, size_t, orwl_mirror*);
#define arg_t_init(...) CALL_WITH_DEFAULTS(arg_t_init, 4, __VA_ARGS__)
DECLARE_DEFARG(arg_t_init, , TNULL(size_t), TNULL(size_t), NULL);
DEFINE_DEFARG(arg_t_init, , TNULL(size_t), TNULL(size_t), NULL);


void arg_t_destroy(arg_t *arg) {
}

DECLARE_NEW_DELETE(arg_t);
DEFINE_NEW_DELETE(arg_t);

DECLARE_THREAD(arg_t);

typedef orwl_handle orwl_handle2[2];

static pthread_barrier_t init_barr = { INITIALIZER };

DEFINE_THREAD(arg_t) {
  size_t const orwl_mynum = Arg->mynum;
  size_t const phases = Arg->phases;
  orwl_mirror* left = Arg->left;

  orwl_handle2 leh[3] = { [0] = { [0] = INITIALIZER } };

  /* The buffer for the reentrant pseudo random generator */
  rand48_t* seed = seed_get();

  orwl_state ostate = orwl_invalid;

  static pthread_mutex_t muti = PTHREAD_MUTEX_INITIALIZER;
  MUTUAL_EXCLUDE(muti)
  for (size_t i = 0; i < 3; ++i) {
    ostate = orwl_invalid;
    report(1, "request, handle %zu, %s",
           (orwl_np + orwl_mynum + (i - 1)) % orwl_np, orwl_state_getname(ostate));
    for (size_t try = 0; ostate == orwl_invalid; ++try) {
      ostate = (i % 2
                ? orwl_write_request(&left[i], &((leh[i])[0]), seed)
                : orwl_read_request(&left[i], &((leh[i])[0]), seed)
                );
      if (ostate == orwl_requested || ostate == orwl_acquired) break;
      progress(1,  try, "request, handle %zu, %s",
               (orwl_np + orwl_mynum + (i - 1)) % orwl_np, orwl_state_getname(ostate));
      sleepfor(0.2);
    }
    report(1, "request, handle %zu, %s",
           (orwl_np + orwl_mynum + (i - 1)) % orwl_np, orwl_state_getname(ostate));
  }

  pthread_barrier_wait(&init_barr);
  report(1, "initial barrier passed");

  for (size_t orwl_phase = 0; orwl_phase < phases; ++orwl_phase) {
    double const twait = 0.01;
    double const rwait = twait * orwl_drand(seed);
    double const await = twait - rwait;
    bool const parity = orwl_phase % 2;
    /**/
    /* cb_t *cb = NEW(cb_t); */
    /* cb->mynum = orwl_mynum; */
    /* cb->phase = orwl_phase; */
    /**/
    sleepfor(await);
    for (size_t i = 0; i < 3; ++i) {
      ostate = orwl_acquire(&((leh[i])[parity]));
      report(1,  "acq, handle %zu, state %s                            ",
             (orwl_mynum + (i - 1) + orwl_np) % orwl_np, orwl_state_getname(ostate));
    }
    if (orwl_phase < phases - 1)
    for (size_t i = 0; i < 3; ++i) {
      ostate = orwl_invalid;
      for (size_t try = 0; ostate == orwl_invalid; ++try) {
        ostate = (i % 2
                  ? orwl_write_request(&left[i], &((leh[i])[!parity]), seed)
                  : orwl_read_request(&left[i], &((leh[i])[!parity]), seed)
                  );
        if (ostate == orwl_requested || ostate == orwl_acquired) break;
        report(1, "request, handle %zu, %s",
                 (orwl_mynum + (i - 1) + orwl_np) % orwl_np, orwl_state_getname(ostate));
        sleepfor(0.2);
      }
    }
    sleepfor(rwait);
    for (size_t i = 0; i < 3; ++i) {
      ostate = orwl_release(&((leh[i])[parity]), seed);
      report(1,  "rel, handle %zu", (orwl_mynum + (i - 1) + orwl_np) % orwl_np);
    }
  }
  pthread_barrier_wait(&init_barr);
  report(1, "final barrier passed");
  report(true, "finished");

}

int main(int argc, char **argv) {
  int ret = 0;
  if (argc < 4) {
    report(1, "only %d commandline arguments, this ain't enough",
           argc);
    return 1;
  }

  orwl_types_init();

  orwl_server* srv = NEW(orwl_server, 4, 10);
  pthread_t srv_id;
  orwl_server_create(srv, &srv_id);
  rand48_t* seed = seed_get();

  report(1, "connecting to %s", argv[1]);
  orwl_endpoint other = { INITIALIZER };
  orwl_endpoint_parse(&other, argv[1]);

  phases = str2size_t(argv[2]);
  number = str2size_t(argv[3]);
  orwl_np = str2size_t(argv[4]);
  offset = str2size_t(argv[5]);

  report(1, "%s: starting %zu phases, %zu/%zu threads, offset %zu",
         argv[0], phases, number, orwl_np, offset);

  pthread_barrier_init(&init_barr, NULL, number);

  /* Initialization of the static location */
  location_back = orwl_mirror_vnew(number + 2);
  location = location_back + 1;

  for (ssize_t i = -1; i <= (ssize_t)number; ++i) {
    size_t lpos = (orwl_np + i) % orwl_np;
    size_t gpos = (orwl_np + i + offset) % orwl_np;
    report(1, "%zd: connecting to %zu (%zu)", i, lpos, gpos);
    orwl_endpoint there = other;
    there.index = gpos;

    orwl_mirror_init(&location[i], srv->host.ep, there);

    /* wait until the other side is up. */
    /* ep.port is already in host order */
    while (orwl_rpc(&there, seed, auth_sock_insert_peer, port2host(&srv->host.ep.port))
           == TONES(uint64_t)) {
      ret = pthread_kill(srv_id, 0);
      if (ret) break;
      sleepfor(0.2);
    }
    report(1, "connected to %s", orwl_endpoint_print(&there));
  }

  sleepfor(2.0);

  /* Half of the threads are created detached and half joinable */
  pthread_t *id = pthread_t_vnew(number/2);
  arg_t *arg = arg_t_vnew(number/2);

  for (size_t i = 0; i < number; ++i) {
    arg_t *myarg = (i%2 ? arg + (i/2) : NEW(arg_t));
    myarg->mynum = i + offset;
    myarg->phases = phases;
    myarg->left = &location[i - 1];
    if (i%2)
      arg_t_create(myarg, id + (i/2));
    else
      arg_t_create(myarg, NULL);
  }


  report(1, "%s: waiting for %zu joinable threads",
         argv[0], number/2);
  for (size_t i = 0; i < number/2; ++i) {
    arg_t_join(id[i]);
  }

  report(1, "%s: waiting for %zu detached threads",
         argv[0], number - number/2);
  if (ret) {
    char mesg[256] = INITIALIZER;
    strerror_r(ret, mesg, 256);
    report(1, "Server already terminated: %s", mesg);
  }
  orwl_pthread_wait_detached();
  report(1, "%s: killing server", argv[0]);
  orwl_server_terminate(srv, seed);
  orwl_server_join(srv_id);
  report(1, "host %p and next %p", (void*)srv->host.next, (void*)&srv->host);
  orwl_server_delete(srv);

  report(1, "freeing arg");
  arg_t_vdelete(arg);
  report(1, "freeing id");
  pthread_t_vdelete(id);
  report(1, "destroying location");
  orwl_mirror_vdelete(location_back);

  return 0;
}
