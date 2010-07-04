/*
** orwl_wait_list.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Feb 20 10:17:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_posix_default.h"
#include "orwl_handle2.h"
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
  char* info;
} cb_t;

cb_t* cb_t_init(cb_t *cb, size_t m, size_t p, char* i) {
  cb->mynum = m;
  cb->phase = p;
  cb->info = i;
  return cb;
}

PROTOTYPE(cb_t*, cb_t_init, cb_t*, size_t, size_t, char*);
FSYMB_DOCUMENTATION(cb_t_init)
#define cb_t_init(...) CALL_WITH_DEFAULTS(cb_t_init, 4, __VA_ARGS__)
DECLARE_DEFARG(cb_t_init, , TNULL(size_t), TNULL(size_t), NULL);
DEFINE_DEFARG(cb_t_init, , TNULL(size_t), TNULL(size_t), NULL);

void cb_t_destroy(cb_t *cb) {
  /* empty */
}

DECLARE_NEW_DELETE(cb_t);
DEFINE_NEW_DELETE(cb_t);

DECLARE_CALLBACK(cb_t);

DEFINE_CALLBACK(cb_t) {
  if (Arg->info)
    memset(Arg->info, ' ', 2);
}

typedef struct _arg_t {
  size_t offset;
  size_t mynum;
  size_t phases;
  orwl_mirror* left;
  char* info;
} arg_t;

arg_t* arg_t_init(arg_t *arg, size_t m, size_t ph, orwl_mirror* le) {
  arg->mynum = m;
  arg->phases = ph;
  arg->left = le;
  arg->info = NULL;
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

static pthread_barrier_t init_barr = { INITIALIZER };

DEFINE_THREAD(arg_t) {
  size_t const offset = Arg->offset;
  size_t const orwl_mynum = Arg->mynum;
  size_t const phases = Arg->phases;
  orwl_mirror* left = Arg->left;

  orwl_handle2 leh[3] = { ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER };

  char* info = Arg->info;
  if (info) info += 3*orwl_mynum + 1;

  /* The buffer for the reentrant pseudo random generator */
  rand48_t* seed = seed_get();

  orwl_state ostate = orwl_invalid;

  static pthread_mutex_t muti = PTHREAD_MUTEX_INITIALIZER;
  MUTUAL_EXCLUDE(muti)
  for (size_t i = 0; i < 3; ++i) {
    ostate = orwl_invalid;
    report(false, "request, handle %zu, %s",
           (orwl_np + orwl_mynum + (i - 1)) % orwl_np, orwl_state_getname(ostate));
    ostate = (i % 2
              ? orwl_write_request2(&left[i], &leh[i], seed)
              : orwl_read_request2(&left[i], &leh[i], seed)
              );
    report(false, "request, handle %zu, %s",
           (orwl_np + orwl_mynum + (i - 1)) % orwl_np, orwl_state_getname(ostate));
  }

  pthread_barrier_wait(&init_barr);
  report(1, "initial barrier passed");

  for (size_t orwl_phase = 0; orwl_phase < phases; ++orwl_phase) {
    double const twait = 0.1;
    double const rwait = twait * orwl_drand(seed);
    double const await = twait - rwait;
    /**/
    sleepfor(await);
    for (size_t i = 0; i < 3; ++i) {
      ostate = orwl_acquire2(&leh[i], seed);
      report(false,  "acq, handle %zu, state %s                            ",
             (orwl_mynum + (i - 1) + orwl_np) % orwl_np, orwl_state_getname(ostate));
    }
    if (!orwl_phase) {
      orwl_resize2(&leh[1], 1, seed);
      report(true, "handle resized");
      uint64_t* data;
      size_t data_len;
      orwl_map2(&leh[1], &data, &data_len, seed);
      memset(data, 0, data_len * sizeof(uint64_t));
      report(true, "handle mapped");
    }

    int diff[3] = { TMIN(int), TNULL(int), TMIN(int) };
    if (info) {
      for (size_t i = 0; i < 3; ++i) {
        uint64_t* data;
        size_t data_len;
        orwl_map2(&leh[i], &data, &data_len, seed);
        if (data_len) {
          switch (i) {
          case 0:
            diff[0] = data[0] - (orwl_phase - 1);
            break;
          case 1:
            data[0] = orwl_phase;
            break;
          case 2:
            diff[2] = (orwl_phase - 1) - data[0];
            break;
          default:
            break;
          }
          report(false, "%zu found suplement of length %zu, says %" PRIX64 " we are at %zu",
                 i, data_len, data[0], orwl_phase);
        }
      }
      char num[10];
      sprintf(num, "  %zX", orwl_phase);
      memcpy(info, num + strlen(num) - 2, 2);
      if (diff[2] == TMIN(int))
        info[2] = '|';
      else
        info[2] = (abs(diff[2]) <= 2 ? ((char[]){ '-', '<', '.', '>', '+'})[diff[2] + 2] : '!');
      if (orwl_mynum == offset) {
        if (diff[0] == TMIN(int))
          info[-1] = '|';
        else
          info[-1] = (abs(diff[0]) <= 2 ? ((char[]){ '-', '<', '.', '>', '+'})[diff[0] + 2] : '!');
      }
    }
    /* if (orwl_phase < phases - 1) { */
    /*   cb_t *cb = NEW(cb_t, orwl_mynum, orwl_phase, info); */
    /*   orwl_callback_attach_cb_t(cb, ((leh[1])[!parity]).wh); */
    /* } */
    sleepfor(rwait);
    if (info) {
      char num[10];
      sprintf(num, "  %zx", orwl_phase);
      memcpy(info, num + strlen(num) - 2, 2);
      if (diff[2] == TMIN(int))
        info[2] = '|';
      else
        info[2] = (abs(diff[2]) <= 2 ? ((char[]){ '<', '.', '>', '+', '!'})[diff[2] + 2] : '!');
    }
    for (size_t i = 0; i < 3; ++i) {
      ostate = orwl_release2(&leh[i], seed);
      report(false,  "rel, handle %zu", (orwl_mynum + (i - 1) + orwl_np) % orwl_np);
    }
  }
  for (size_t i = 0; i < 3; ++i) {
    ostate = orwl_cancel2(&leh[i], seed);
    report(false,  "can, handle %zu", (orwl_mynum + (i - 1) + orwl_np) % orwl_np);
  }
  pthread_barrier_wait(&init_barr);
  report(true, "final barrier passed");
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

  /* info has one suplementary char in front such that we may always
     address field -1 from the threads. */
  size_t info_len = 3*orwl_np;
  char* info = calloc(info_len + 2);
  memset(info, ' ', info_len + 1);
  for (size_t i = 3; i < info_len; i += 3)
    info[i] = '|';
  srv->info = info;
  srv->info_len = info_len;

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

  /* Half of the threads are created detached and half joinable */
  pthread_t *id = pthread_t_vnew(number/2);
  arg_t *arg = arg_t_vnew(number/2);

  for (size_t i = 0; i < number; ++i) {
    arg_t *myarg = (i%2 ? arg + (i/2) : NEW(arg_t));
    myarg->offset = offset;
    myarg->mynum = i + offset;
    myarg->phases = phases;
    myarg->left = &location[i - 1];
    myarg->info = info;
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
  seed_get_clear();
  return 0;
}
