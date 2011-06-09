#include "orwl.h"
#include "p99_c99_default.h"
#include "p99_str.h"
#include "orwl_instrument.h"

#define MEGA (1024 * 1024)

static orwl_server srv = P99_INIT;
static orwl_graph *graph = 0;
static orwl_address_book *ab = 0;
static orwl_mirror *local_locations = 0;
static rand48_t *seed = 0;
static size_t shared_memory_size = 16 * MEGA;
static size_t iterations = 0;
static size_t nb_tasks = 0;
static size_t global_nb_tasks = 0;
static orwl_barrier init_barr;
static struct random_data *rand_states;
static char *rand_statebufs;
static size_t *list_tasks = 0;


typedef struct _arg_t {
  size_t id;
  orwl_vertex *vertex;
  size_t my_location_pos;
  size_t task_pos;
} arg_t;

arg_t* arg_t_init(arg_t *arg, size_t id, orwl_vertex *v, size_t mlp, size_t tp) {
  arg->id = id;
  arg->vertex = v;
  arg->my_location_pos = mlp;
  arg->task_pos = tp;
  return arg;
}

P99_PROTOTYPE(arg_t*, arg_t_init, arg_t*, size_t, orwl_vertex*, size_t, size_t);
#define arg_t_init(...) P99_CALL_DEFARG(arg_t_init, 5, __VA_ARGS__)
P99_DECLARE_DEFARG(arg_t_init,  , P99_0(size_t), P99_0(orwl_vertex*), P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(arg_t_init, , P99_0(size_t), P99_0(orwl_vertex*), P99_0(size_t), P99_0(size_t));

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

  /***************************************************************************/
  /*                              Connection step                            */
  /***************************************************************************/
  /* Only the main task makes connections on distant guys */
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
    orwl_make_distant_connection(Arg->vertex->neighbors[i], &srv, graph, ab, &distant_locations[Arg->vertex->neighbors[i]]);
  }

  /* Take the local lock in write mode */
  orwl_write_request2(&local_locations[Arg->my_location_pos], &my_task_handle);
  sleepfor(5); /* we need a distributed barrier here */

  /* Wait to take all the distant locks */
  orwl_wait_to_initialize_locks(Arg->id, graph, ab,  seed);

  /* Take the distant locks in read mode*/
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
    orwl_read_request2(&distant_locations[Arg->vertex->neighbors[i]], &handle_distant_pos[Arg->vertex->neighbors[i]]);
  }

  /* Check if my neighbors are ready before starting, then realease the handle on my location */
  orwl_wait_to_start(Arg->id, graph, ab, &srv, nb_tasks, seed);


  /* Fire ! */

  /***************************************************************************/
  /*                       Initialization iteration                          */
  /***************************************************************************/
  orwl_acquire2(&my_task_handle);
  orwl_truncate2(&my_task_handle, shared_memory_size);
  orwl_release2(&my_task_handle);

  /* Take the locks on the neighbors to keep the same count of iterations */
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
    orwl_acquire2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
    orwl_release2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
  }
  /***************************************************************************/
  /*                         Computation iterations                          */
  /***************************************************************************/

  for (size_t iter = 0 ; iter < iterations ; iter++) {
    printf("Task %zu: iteration %zu\n", Arg->id, iter);
    my_data = orwl_write_map2(&my_task_handle);

      /* CPU consuming computation */
      double a,b,c;
      double * r = calloc(10000000, sizeof(double));
      for (size_t i = 0 ; i < 10000000 ; i++) {
	b = (i + 1) / (sin((i + 2) / (i + 1)) + 1.01);
	for (size_t j = 0 ; j < 10 ; j++) {
	  c = (j + 3) / (cos((i + 4) / (i + 1)) + 1.02);
	  a = (b * c) - (b / (i + (j * c)));
	  r[i] += a;
	}
      }
      free(r);
      /* Let's put anything in the shared memory */
      for (size_t i = 0 ; i < (shared_memory_size / sizeof(uint64_t)) ; i++) {
	int32_t r;
	random_r(&rand_states[Arg->task_pos], &r); 
	my_data[i] = r;
      }
      orwl_release2(&my_task_handle);

      for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
      	distant_mapped_data[i] = (uint64_t*)orwl_read_map2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
      
      for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
      	orwl_release2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
  }
  orwl_cancel2(&my_task_handle);
  for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
    orwl_cancel2(&handle_distant_pos[Arg->vertex->neighbors[i]]);
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
  if (argc < 7) {
    report(1, "only %d commandline arguments, this ain't enough", argc);
    return 1;
  }

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

  nb_tasks = strcountchr(main_tasks, ",") + 1;
  list_tasks = size_t_vnew(nb_tasks);
  const size_t nb_locations = nb_tasks;
  orwl_barrier_init(&init_barr, nb_tasks);
  get_task(main_tasks, list_tasks, ",");
  size_t list_locations[nb_tasks];
  for (int i = 0 ; i < nb_tasks ; i++)
    list_locations[i] = i;

  /* local server initialization */
  seed = seed_get();
  orwl_types_init();
  orwl_start(&srv, SOMAXCONN, nb_locations);

  if (!orwl_alive(&srv)) return EXIT_FAILURE;

  orwl_server_block(&srv);

  local_locations = orwl_mirror_vnew(nb_locations);

  for (size_t i = 0 ; i < nb_tasks ; i++)
    orwl_make_local_connection(i, &srv, &local_locations[i]);

  report(1, "local connections done");

  if (!orwl_wait_and_load_init_files(&ab, global_ab_file,
  				     &graph, graph_file,
  				     &srv,
  				     local_ab_file,
  				     nb_tasks, list_tasks, 
				     list_locations,
				     global_nb_tasks)) {

    report(1, "can't load some files");
    return EXIT_FAILURE;
  }

  rand_states = (struct random_data*)calloc(nb_tasks, sizeof(struct random_data));
  rand_statebufs = (char*)calloc(nb_tasks, 32);
  for (size_t i = 0 ; i < nb_tasks ; i++)
    initstate_r(random(), &rand_statebufs[i], 32, &rand_states[i]);

  pthread_t *tid = pthread_t_vnew(nb_tasks);
  arg_t *arg = arg_t_vnew(nb_tasks);
  for (size_t i = 0 ; i < nb_tasks ; i++) {
    arg_t *myarg = &arg[i];
    myarg->id = list_tasks[i];
    myarg->vertex = &graph->vertices[list_tasks[i]];
    myarg->my_location_pos = list_locations[i];
    myarg->task_pos = i;
    arg_t_create(myarg, &tid[i]);
  }

  /* wait the threads */
  for (size_t i = 0 ; i < nb_tasks ; i++) {
    arg_t_join(tid[i]);
  }
  orwl_server_terminate(&srv);
  orwl_stop(&srv);
  seed_get_clear();
  size_t_vdelete(list_tasks);
  return 0;
}
