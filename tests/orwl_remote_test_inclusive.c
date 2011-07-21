/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl.h"
#include "p99_c99_default.h"
#include "p99_str.h"

P99_DECLARE_STRUCT(arg_t);


/**
 ** Struct that contains the information that we want to pass on to
 ** each individual thread.
 **/
struct arg_t {
  size_t offset;
  /** The thread id. **/
  size_t mynum;
  /** The number of iterations. **/
  size_t phases;
  /** An array of orwl_np locations. **/
  orwl_mirror* left;
  /** A barrier that is used for initialization purposes. **/
  orwl_barrier* init_barr;
  /** A character array in the "server" thread that is used to show
   ** the progress of the application. **/
  char* info;
};

arg_t* arg_t_init(arg_t *arg, orwl_barrier* ba, size_t off, size_t m,
                  size_t ph, orwl_mirror* le, char* inf) {
  if (arg)
    *arg = P99_LVAL(arg_t const,
                    .offset = off,
                    .mynum = m,
                    .phases = ph,
                    .left = le,
                    .init_barr = ba,
                    .info = inf,
                   );
  return arg;
}

P99_PROTOTYPE(arg_t*, arg_t_init, arg_t *, orwl_barrier*, size_t, size_t, size_t, orwl_mirror*, char*);
#define arg_t_init(...) P99_CALL_DEFARG(arg_t_init, 7, __VA_ARGS__)
P99_DECLARE_DEFARG(arg_t_init, , P99_0(orwl_barrier*), P99_0(size_t), P99_0(size_t), P99_0(size_t), P99_0(orwl_mirror*), P99_0(char*));
P99_DEFINE_DEFARG(arg_t_init, , P99_0(orwl_barrier*), P99_0(size_t), P99_0(size_t), P99_0(size_t), P99_0(orwl_mirror*), P99_0(char*));

void arg_t_destroy(arg_t *arg) {
}

DECLARE_NEW_DELETE(arg_t);
DEFINE_NEW_DELETE(arg_t);

DECLARE_THREAD(arg_t);


/** Define the function that each thread runs. **/
DEFINE_THREAD(arg_t) {
  /* Lift all thread context fields to local variables of this thread */
  size_t const offset = Arg->offset;
  size_t const orwl_mynum = Arg->mynum;
  size_t const phases = Arg->phases;
  orwl_mirror*const left = Arg->left;
  orwl_barrier*const init_barr = Arg->init_barr;
  char* info = Arg->info;

  /**
   ** Initialization of some variables.
   **/

  /* Shift the info pointer to the position that this thread here is
   * due to manipulate. */
  if (info) info += 3*orwl_mynum + 1;

  /* Each thread holds 3 orwl_handle2. One for his own location and
   * one each for the location to the left and to the right. */
  orwl_handle2 handle[3] = { ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER };

  /* The buffer for the reentrant pseudo random generator */
  rand48_t* seed = seed_get();

  orwl_state ostate = orwl_invalid;

  /* Insert the three handles into the request queues. Positions 0 and
   * 2 are the neighbors, so these are only read requests. Position 1
   * is our own location where we want to write. */
  {
    orwl_barrier_wait(init_barr);
    /* Randomize, so the system will always be initialized differently. */
    sleepfor(orwl_drand(seed) * 1E-1);
    /* We have to be sure that the insertion into the three request
       queues is not mixed up. For the moment this is just guaranteed
       by holding a global mutex. */
    ORWL_CRITICAL {
      orwl_read_request2(&left[0], &handle[0]);
      orwl_write_request2(&left[1], &handle[1]);
      orwl_read_request2(&left[2], &handle[2]);
    }
    orwl_barrier_wait(init_barr);
    report(0, "initial barrier passed");
  }


  /* Do some resizing, but only in the initial phase. */
  {
    for (size_t i = 0; i < 3; ++i)
      orwl_acquire2(&handle[i]);
    size_t len = sizeof(uint64_t);
    char const* env = getenv("ORWL_HANDLE_SIZE");
    if (env) {
      size_t len2 = strtouz(env);
      if (len2 > len) len = len2;
    }
    orwl_truncate2(&handle[1], len);
    report(true, "handle resized to %zu byte                                             \n",
           len);
    for (size_t i = 0; i < 3; ++i)
      orwl_release2(&handle[i]);
  }


  for (size_t orwl_phase = 1; orwl_phase < phases; ++orwl_phase) {

    /** Initial Phase: do something having requested the lock but not
     ** necessarily having it obtained.
     **/
    double const twait = 0.3;
    double const rwait = twait * orwl_drand(seed);
    double const await = twait - rwait;

    /** Here for this dummy application we just take a nap **/
    sleepfor(await);


    /** Acquire all the three handles, our own and the two neighboring ones.
     ** This will block until the locks are obtained.
     **/
    for (size_t i = 0; i < 3; ++i)
      ostate = orwl_acquire2(&handle[i]);

    /* Now work on our data. */
    int64_t diff[3] = { P99_TMIN(int64_t), P99_0(int64_t), P99_TMIN(int64_t) };
    {
      /* For handle 1 we have gained write access. Change the globally
       * shared data for the whole application. */
      {
        uint64_t* data = orwl_write_map2(&handle[1]);
        assert(data);
        *data = orwl_phase;
      }
      /* For handle 0 and 2 we have read access. */
      {
        uint64_t const* data = orwl_read_map2(&handle[0]);
        assert(data);
        diff[0] = *data - (orwl_phase - 1);
      }
      {
        uint64_t const* data = orwl_read_map2(&handle[2]);
        assert(data);
        diff[2] = (orwl_phase - 1) - *data;
      }
    }

    /** take another nap where a real application would do its work
     ** that needs access to the shared resource.
     **/
    sleepfor(rwait);

    /** Now write some data that keeps track of the local state of all
     ** threads. */
    if (info) {
      char num[10];
      sprintf(num, "  %zX", orwl_phase);
      memcpy(info, num + strlen(num) - 2, 2);
      if (diff[2] == P99_TMIN(int64_t))
        info[2] = '|';
      else
info[2] = (abs(diff[2]) <= 2 ? ((char[]) { '-', '<', '.', '>', '+'})[diff[2] + 2] : '!');
      if (orwl_mynum == offset) {
        if (diff[0] == P99_TMIN(int64_t))
          info[-1] = '|';
        else
info[-1] = (abs(diff[0]) <= 2 ? ((char[]) { '-', '<', '.', '>', '+'})[diff[0] + 2] : '!');
      }
    }

    /* At the end of the phase, release our locks and launch the next
     * phase by placing a new request in the end of the queue. */
    for (size_t i = 0; i < 3; ++i)
      ostate = orwl_release2(&handle[i]);
  }

  /** And at the very end of the lifetime of the thread cancel all
   ** locks such that no other thread is blocked and return.
   **/
  for (size_t i = 0; i < 3; ++i)
    ostate = orwl_cancel2(&handle[i]);
  report(false, "finished");
}

int main(int argc, char **argv) {
  if (argc < 6) {
    report(1, "Usage: %s URL PHASES LOCAl GLOBAL OFFSET",
           argv[0]);
    report(1, "only %d commandline arguments, this ain't enough",
           argc);
    return 1;
  }

  orwl_types_init();

  /* condition the run */
  size_t phases = str2uz(argv[2]);
  size_t number = str2uz(argv[3]);
  orwl_np = str2uz(argv[4]);
  size_t offset = str2uz(argv[5]);

  orwl_mirror*const location_back = orwl_mirror_vnew(number + 2);
  orwl_mirror* location = location_back + 1;

  orwl_barrier init_barr;
  orwl_barrier_init(&init_barr, number);

  /* start the server thread and initialize it properly */
  orwl_server srv = P99_INIT;
  {
    orwl_start(&srv, SOMAXCONN, number * 2);
    if (!orwl_alive(&srv)) return EXIT_FAILURE;

    /** The string "info" will be used as sort of common black board by
     ** the threads such that we can visualize their state. It has one
     ** suplementary char in front such that we may always address
     ** field -1 from the threads.
     */
    size_t info_len = 3*orwl_np;
    char* info = calloc(info_len + 2);
    memset(info, ' ', info_len + 1);
    for (size_t i = 3; i < info_len; i += 3)
      info[i] = '|';
    srv.info = info;
    srv.info_len = info_len;
  }

  /* To test both models of thread creation, half of the threads are
   * created detached and half joinable */
  pthread_t *id = pthread_t_vnew(number/2);
  arg_t *arg = arg_t_vnew(number/2);

  report(1, "%s: starting %zu phases, %zu/%zu threads, offset %zu",
         argv[0], phases, number, orwl_np, offset);

  {
    /* set up the connections between the clients and the
     * server process. */
    report(1, "connecting to %s", argv[1]);
    orwl_endpoint there = orwl_endpoint_get(argv[1]);

    for (ssize_t i = -1; i <= (ssize_t)number; ++i) {
      size_t gpos = (orwl_np + i + offset) % orwl_np;
      there.index = gpos;
      orwl_mirror_connect(&location[i], &srv, there);
      report(0, "connected to %s", orwl_endpoint_print(&there));
    }
  }

  /* Fire up the worker threads. */
  for (size_t thread = 0; thread < number; ++thread) {
    if (thread%2) {
      /* The odd numbered ones are joinable and use an arg_t that is
       * managed by this main thread, here. */
      size_t thread2 = thread/2;
      arg_t_create_joinable(
        arg_t_init(&arg[thread2],
                   &init_barr,
                   offset,
                   thread + offset,
                   phases,
                   /* give it the starting
                    * position of the location on
                    * the left. */
                   &location[thread - 1],
                   srv.info),
        &id[thread2]
      );
    } else {
      /* The even numbered ones are created detached and receive a
       * freshly created arg_t for which they are responsible. */
      arg_t_create_detached(
        P99_NEW(arg_t,
                &init_barr,
                offset,
                thread + offset,
                phases,
                /* give it the starting
                 * position of the location on
                 * the left. */
                &location[thread - 1],
                srv.info)
      );
    }
  }

  /* wait for even numbered threads, that have been started joinable. */
  for (size_t thread2 = 0; thread2 < number/2; ++thread2) {
    arg_t_join(id[thread2]);
  }

  /* wait for the remaining threads, if any are still alive */
  orwl_pthread_wait_detached();

  /* now we can safely destruct ourselves */
  report(0, "%s: killing server", argv[0]);
  orwl_server_terminate(&srv);
  orwl_stop(&srv);

  report(1, "freeing arg");
  arg_t_vdelete(arg);
  report(1, "freeing id");
  pthread_t_vdelete(id);
  report(1, "destroying location");
  orwl_mirror_vdelete(location_back);
  seed_get_clear();
  return 0;
}
