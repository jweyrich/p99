/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
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
#include "orwl_instrument.h"

#define MEGA (1024 * 1024)

static orwl_graph *graph = 0;
static orwl_address_book *ab = 0;
static orwl_mirror *local_locations = 0;
static size_t shared_memory_size = 0;
static size_t iterations = 0;
static size_t inner_iterations = 0;
static size_t nb_tasks = 0;
static size_t global_nb_tasks = 0;
static orwl_barrier init_barr;
static size_t *list_tasks = 0;


typedef struct _arg_t {
  size_t id;
  orwl_vertex *vertex;
  size_t my_location_pos;
  size_t task_pos;
  orwl_thread_cntrl *det;
} arg_t;

arg_t* arg_t_init(arg_t *arg, size_t id, orwl_vertex *v, size_t mlp, size_t tp,
                  orwl_thread_cntrl* det) {
  if (arg) {
    *arg = P99_LVAL(arg_t,
                    .id = id,
                    .vertex = v,
                    .my_location_pos = mlp,
                    .task_pos = tp,
                    .det = det,
                   );
  }
  return arg;
}

P99_PROTOTYPE(arg_t*, arg_t_init, arg_t*, size_t, orwl_vertex*, size_t, size_t, orwl_thread_cntrl*);
#define arg_t_init(...) P99_CALL_DEFARG(arg_t_init, 6, __VA_ARGS__)
P99_DECLARE_DEFARG(arg_t_init,  , P99_0(size_t), P99_0(orwl_vertex*), P99_0(size_t), P99_0(size_t), P99_0(orwl_thread_cntrl*));
P99_DEFINE_DEFARG(arg_t_init, , P99_0(size_t), P99_0(orwl_vertex*), P99_0(size_t), P99_0(size_t), P99_0(orwl_thread_cntrl*));

void arg_t_destroy(arg_t *arg) {
}

DECLARE_NEW_DELETE(arg_t);
DEFINE_NEW_DELETE(arg_t);

DECLARE_THREAD(arg_t);

DEFINE_THREAD(arg_t) {
  orwl_handle2 my_task_handle = ORWL_HANDLE2_INITIALIZER;
  orwl_handle2 * handle_distant_pos = orwl_handle2_vnew(global_nb_tasks);
  uint64_t * my_data = 0;
  uint64_t ** distant_mapped_data = calloc(global_nb_tasks, sizeof(uint64_t*));
  orwl_mirror * distant_locations = orwl_mirror_vnew(global_nb_tasks);
  rand48_t * seed = seed_get();

  /***************************************************************************/
  /*                              Connection step                            */
  /***************************************************************************/
  /* Only the main task makes connections on distant guys */
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
    orwl_make_distant_connection(Arg->vertex->neighbors[i], orwl_server_get(), graph, ab, &distant_locations[Arg->vertex->neighbors[i]]);
  }

  /* Take the local lock in write mode */
  orwl_write_request2(&local_locations[Arg->my_location_pos], &my_task_handle);

  /* Do not continue until each thread has put its write request */
  orwl_thread_cntrl_freeze(Arg->det);
  orwl_thread_cntrl_wait_for_caller(Arg->det);

  /* Wait to take all the distant locks */
  orwl_wait_to_initialize_locks(Arg->id, graph, ab, seed);

  /* Take the distant locks in read mode*/
  for (size_t i = 0; i < Arg->vertex->nb_neighbors; ++i)
    orwl_read_request2(&distant_locations[Arg->vertex->neighbors[i]],
                       &handle_distant_pos[Arg->vertex->neighbors[i]]);

  /* Check if my neighbors are ready before starting, then realease the handle on my location */
  orwl_wait_to_start(Arg->id, graph, ab, orwl_server_get(), nb_tasks, seed);


  /***************************************************************************/
  /*                       Initialization iteration                          */
  /***************************************************************************/
  orwl_acquire2(&my_task_handle);
  my_data = orwl_write_map2(&my_task_handle);
  /* Let's put anything in the shared memory */
  for (size_t i = 0 ; i < ((shared_memory_size * MEGA) / sizeof(uint64_t)) ; i++) {
    my_data[i] = orwl_rand(seed);
  }
  orwl_disconnect2(&my_task_handle);
  /* Take the distant locks to keep the correct number of phases */
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
    orwl_acquire2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
    orwl_disconnect2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
  }


  /* Fire ! */
  /***************************************************************************/
  /*                         Computation iterations                          */
  /***************************************************************************/

  for (size_t iter = 0 ; iter < iterations ; iter++) {
    ORWL_TIMER(total_iteration) {
      printf("Task %zu: iteration %zu\n", Arg->id, iter);
      
      ORWL_TIMER(appli_local_acquire)
	orwl_acquire2(&my_task_handle);

      ORWL_TIMER(appli_write_map)
	my_data = orwl_write_map2(&my_task_handle);

      ORWL_TIMER(appli_computation) {
	/* CPU consuming computation */
	double a,b,c;
	volatile double d = 0;
	for (size_t i = 0 ; i < 1000000 ; i++) {
	  b = (i + 1) / (sin((i + 2) / (i + 1)) + 1.01);
	  d = 0;
	  for (size_t j = 0 ; j < inner_iterations ; j++) {
	    c = (j + 3) / (cos((i + 4) / (i + 1)) + 1.02);
	    a = (b * c) - (b / (i + (j * c)));
	    d += a;
	  }
	}
      }

      ORWL_TIMER(appli_local_release)
	orwl_next2(&my_task_handle);

      ORWL_TIMER(appli_distant_acquire)
	for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
	  orwl_acquire2(&handle_distant_pos[Arg->vertex->neighbors[i]]);

      ORWL_TIMER(appli_read_map)
	for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
	  distant_mapped_data[i] = (uint64_t*)orwl_read_map2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
      
      ORWL_TIMER(appli_distant_release)
	for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
	  orwl_next2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
    }
  }
  orwl_disconnect2(&my_task_handle);
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
    orwl_disconnect2(&handle_distant_pos[Arg->vertex->neighbors[i]]);

  seed_get_clear();
  orwl_handle2_vdelete(handle_distant_pos);
}

unsigned strcountchr(char *str, const char *chr) {
  unsigned count = 0;
  char *ptr = str;
  while (ptr != 0) {
    ptr = strchr(ptr, chr[0]);
    if (ptr != 0) {
      count++;
      ptr = ptr + 1;
    }
  }
  return count;
}

unsigned get_task(char const *str, size_t list_id[], const char *delim) {
  unsigned count = 0;
  char *token = 0;
  char *tmp = strdup(str);
  char *saveptr = 0;
  if (strcountchr(tmp, delim) > 0) {
    token = strtok_r(tmp, &delim[0], &saveptr);
    while (token != 0) {
      size_t task = str2uz(token);
      list_id[count] = task;
      count++;
      token = strtok_r(P99_0(char*), &delim[0], &saveptr);
    }
  } else {
    size_t task = str2uz(str);
    list_id[0] = task;
    count = 1;
  }
  free(tmp);
  return count;
}

int main(int argc, char **argv) {
  if (argc < 9) {
    report(1, "only %d commandline arguments, this ain't enough", argc);
    return 1;
  }
  ORWL_TIMER(appli_wall_time) {
    char graph_file[256] = {0};
    char global_ab_file[256] = {0};
    char local_ab_file[256] = {0};
    char main_tasks[4096] = {0};
    iterations = strtouz(argv[1]);
    global_nb_tasks = strtouz(argv[2]);
    strncpy(graph_file, argv[3], 256);
    strncpy(global_ab_file, argv[4], 256);
    strncpy(local_ab_file, argv[5], 256);
    strncpy(main_tasks, argv[6], 4096);
    shared_memory_size = strtouz(argv[7]);
    inner_iterations = strtouz(argv[8]);

    nb_tasks = strcountchr(main_tasks, ",") + 1;
    list_tasks = size_t_vnew(nb_tasks);
    const size_t nb_locations = nb_tasks;
    orwl_barrier_init(&init_barr, nb_tasks);
    get_task(main_tasks, list_tasks, ",");
    size_t list_locations[nb_tasks];
    for (int i = 0 ; i < nb_tasks ; i++)
      list_locations[i] = i;

    /* local server initialization */
    orwl_types_init();
    orwl_start(nb_locations, SOMAXCONN);

    if (!orwl_alive()) return EXIT_FAILURE;

    orwl_server_block();

    local_locations = orwl_mirror_vnew(nb_locations);

    for (size_t i = 0 ; i < nb_tasks ; i++) {
      orwl_make_local_connection(i, orwl_server_get(), &local_locations[i]);
      orwl_scale(&local_locations[i], (shared_memory_size * MEGA));
    }
    report(1, "local connections done");

    if (!orwl_wait_and_load_init_files(&ab, global_ab_file,
				       &graph, graph_file,
				       orwl_server_get(),
				       local_ab_file,
				       nb_tasks, list_tasks, 
				       list_locations,
				       global_nb_tasks)) {

      report(1, "can't load some files");
      return EXIT_FAILURE;
    }

    orwl_thread_cntrl ** det = calloc(nb_tasks, sizeof(orwl_thread_cntrl *));

    for (size_t i = 0 ; i < nb_tasks ; i++) {
      det[i] = P99_NEW(orwl_thread_cntrl);
      arg_t *myarg
	= P99_NEW(arg_t,
		  list_tasks[i],
		  &graph->vertices[list_tasks[i]],
		  list_locations[i],
		  i,
		  det[i]);
      arg_t_launch(myarg, det[i]);
    }
    
    for (size_t i = 0 ; i < nb_tasks ; i++) {
      orwl_thread_cntrl_wait_for_callee(det[i]);
      orwl_thread_cntrl_detach(det[i]);
    }

    orwl_pthread_wait_detached();
    free(det);
  }
  orwl_server_terminate();
  orwl_stop();
  sleep(3);
  size_t_vdelete(list_tasks);
  return(0);
}
