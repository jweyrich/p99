#include "orwl.h"
#include "p99_c99_default.h"
#include "p99_str.h"
#include "orwl_instrument.h"

static orwl_server srv = P99_INIT;
static orwl_graph *graph = 0;
static orwl_address_book *ab = 0;
static orwl_mirror *locations = 0;
static size_t shift_local_locations = 0;
static rand48_t *seed = 0;
static size_t count = 0;
static size_t sub_matrix_size = 0;
static size_t iterations = 0;
static float **main_task_matrix = 0;
static size_t nb_main_tasks = 0;
static size_t global_nb_tasks = 0;
static size_t global_rows;
static size_t global_lines;
static orwl_barrier init_barr;
static struct random_data *rand_states;
static char *rand_statebufs;
static size_t *list_tasks = 0;

#define MAIN 0
#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4
#define NORTHEAST 5
#define NORTHWEST 6
#define SOUTHEAST 7
#define SOUTHWEST 8

#define POS_NORTH 0
#define POS_SOUTH 1
#define POS_WEST 2
#define POS_EAST 3
#define POS_NORTHEASTNORTH 4
#define POS_NORTHEASTEAST 5
#define POS_NORTHWESTNORTH 6
#define POS_NORTHWESTWEST 7
#define POS_SOUTHEASTSOUTH 8
#define POS_SOUTHEASTEAST 9
#define POS_SOUTHWESTSOUTH 10
#define POS_SOUTHWESTWEST 11

size_t convert_sub_task(char const *str) {
  if (strncmp(str, "M", 3) == 0)
    return MAIN;
  else if (strncmp(str, "N", 3) == 0)
    return NORTH;
  else if (strncmp(str, "S", 3) == 0)
    return SOUTH;
  else if (strncmp(str, "W", 3) == 0)
    return WEST;
  else if (strncmp(str, "E", 3) == 0)
    return EAST;
  else if (strncmp(str, "NW", 3) == 0)
    return NORTHWEST;
  else if (strncmp(str, "SW", 3) == 0)
    return SOUTHWEST;
  else if (strncmp(str, "NE", 3) == 0)
    return NORTHEAST;
  else if (strncmp(str, "SE", 3) == 0)
    return SOUTHEAST;
  else {
    printf("Invalid sub task kind: %s\n", str);
    assert(0);
    return SIZE_MAX;
  }
}


size_t distant_main_task_pos_from_me(size_t omy_mt, size_t odistant_mt) {
  size_t x_my_mt, y_my_mt, x_distant_mt, y_distant_mt;
  size_t task_per_line = global_rows;
  size_t my_mt = omy_mt / 9;
  size_t distant_mt = odistant_mt / 9;
  x_my_mt = my_mt % task_per_line;
  y_my_mt = my_mt / task_per_line;
  x_distant_mt = distant_mt % task_per_line;
  y_distant_mt = distant_mt / task_per_line;
  if (x_my_mt < x_distant_mt) {
    if (y_my_mt < y_distant_mt)
      return SOUTHEAST;
    if (y_my_mt == y_distant_mt)
      return EAST;
    if (y_my_mt > y_distant_mt)
      return NORTHEAST;
  } else if (x_my_mt == x_distant_mt){
    if (y_my_mt < y_distant_mt)
      return SOUTH;
    if (y_my_mt > y_distant_mt)
      return NORTH;
  } else {
    if (y_my_mt < y_distant_mt)
      return SOUTHWEST;
    if (y_my_mt == y_distant_mt)
      return WEST;
    if (y_my_mt > y_distant_mt)
      return NORTHWEST;    
  }
  assert(0);
  return SIZE_MAX;
}

size_t distant_task_to_local_pos(size_t st, size_t my_mt, size_t distant_mt) {
  report(0," %zu %zu %zu %zu", st, my_mt, distant_mt, distant_main_task_pos_from_me(my_mt, distant_mt));
  switch (distant_main_task_pos_from_me(my_mt, distant_mt)) {
  case NORTH:
    switch (st) {
    case SOUTHEAST:
      return POS_NORTHEASTNORTH;
    case SOUTH:
      return POS_NORTH;
    case SOUTHWEST:
      return POS_NORTHWESTNORTH;
    default:
      assert(0);
      return SIZE_MAX;
    }
  case SOUTH:
    switch (st) {
    case NORTHEAST:
      return POS_SOUTHEASTSOUTH;
    case NORTH:
      return POS_SOUTH;
    case NORTHWEST:
      return POS_SOUTHWESTSOUTH;
    default:
      assert(0);
      return SIZE_MAX;
    }
  case EAST:
    switch (st) {
    case NORTHWEST:
      return POS_NORTHEASTEAST;
    case WEST:
      return POS_EAST;
    case SOUTHWEST:
      return POS_SOUTHEASTEAST;
    default:
      assert(0);
      return SIZE_MAX;
    }
  case WEST:
    switch (st) {
    case NORTHEAST:
      return POS_NORTHWESTWEST;
    case EAST:
      return POS_WEST;
    case SOUTHEAST:
      return POS_SOUTHWESTWEST;
    default:
      assert(0);
      return SIZE_MAX;
    }
  default:
    assert(0);
    return SIZE_MAX;
  }
}

size_t extract_size_t_from_str(int start, int end, char const *original_str) {
  const size_t size = end - start;
  char sub_str[size];
  strncpy(sub_str, &original_str[start], size);
  sub_str[size] = '\0';
  return strtouz(sub_str);
}

void extract_str_from_str(int start, int end, char const *original_str, char *dest_str) {
  const size_t size = end - start;
  strncpy(dest_str, &original_str[start], size);
  dest_str[size] = '\0';
}

size_t get_main_task_from_label(char const *str) {
  const char *pattern = "^([[:digit:]]+)-([[:alpha:]]+)$";
  regex_t re;
  size_t main_task = SIZE_MAX;
  if (regcomp(&re, pattern, REG_EXTENDED)) {
    printf("Pattern did not compile.\n");
  } 
  regmatch_t match[3]= {0};
  if (regexec(&re, str, 3, match, 0) == 0) {
    main_task = extract_size_t_from_str(match[1].rm_so,
					match[1].rm_eo,
					str);
  }
  regfree(&re);
  return main_task;
}

size_t get_sub_task_from_label(char const *str) {
  const char *pattern = "^([[:digit:]]+)-([[:alpha:]]+)$";
  regex_t re;
  size_t sub_task = SIZE_MAX;
  if (regcomp(&re, pattern, REG_EXTENDED)) {
    report(1, "Pattern did not compile");
    return SIZE_MAX;
  } 
  regmatch_t match[3]= {0};
  if (regexec(&re, str, 3, match, 0) == 0) {
    char label[3];
    extract_str_from_str(match[2].rm_so,
			 match[2].rm_eo,
			 str,
			 &label[0]);
    sub_task = convert_sub_task(label);
  }
  regfree(&re);
  return sub_task;
}

#define rel_pos(x,y) ((sub_matrix_size * (x)) + y)

/* milli-second difference between two struct timeval */
#define tv_diff(starttime,finishtime) ((finishtime.tv_sec - starttime.tv_sec) * 1000 + (finishtime.tv_usec - starttime.tv_usec) / 1000)

typedef struct _arg_t {
  size_t id;
  orwl_vertex *vertex;
  size_t my_location_pos;
  size_t main_task;
  size_t main_task_pos;
  size_t sub_task;
} arg_t;

arg_t* arg_t_init(arg_t *arg, size_t id, orwl_vertex *v,
		  size_t mlp, size_t mt, size_t mtp, size_t st) {
  arg->id = id;
  arg->vertex = v;
  arg->my_location_pos = mlp;
  arg->main_task = mt;
  arg->main_task_pos = mtp;
  arg->sub_task = st;
  return arg;
}

P99_PROTOTYPE(arg_t*, arg_t_init, arg_t*, size_t, orwl_vertex*, size_t, size_t, size_t, size_t);
#define arg_t_init(...) P99_CALL_DEFARG(arg_t_init, 7, __VA_ARGS__)
P99_DECLARE_DEFARG(arg_t_init,  , P99_0(size_t), P99_0(orwl_vertex*), P99_0(size_t), P99_0(size_t), P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(arg_t_init, , P99_0(size_t), P99_0(orwl_vertex*), P99_0(size_t), P99_0(size_t), P99_0(size_t), P99_0(size_t));

void arg_t_destroy(arg_t *arg) {
}

DECLARE_NEW_DELETE(arg_t);
DEFINE_NEW_DELETE(arg_t);
DECLARE_THREAD(arg_t);


void update_frontier(size_t id,
		     size_t sub_task, 
		     size_t main_task_pos, 
		     orwl_handle2 *my_task_handle) {
  //size_t frontier_size = (sub_task < 5) ? sub_matrix_size - 2 : 1;
  size_t frontier_size = 0;
  float * shared_data = orwl_write_map2_instr(my_task_handle, frontier_size * sizeof(float), 0);
  if (shared_data == 0) {
    report(1, "fail to map memory on task %zu, %zu, %zu", id, frontier_size, sub_task);
    assert(0);
  }

  switch (sub_task) {
  case NORTH:
    for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
      shared_data[i - 1] = main_task_matrix[main_task_pos][rel_pos(0, i)];
    break;
  case SOUTH:
    for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
      shared_data[i - 1] = main_task_matrix[main_task_pos][rel_pos(sub_matrix_size - 1, i)];
    break;
  case WEST:
    for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
      shared_data[i - 1] = main_task_matrix[main_task_pos][rel_pos(i, 0)];
    break;
  case EAST:
    for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
      shared_data[i - 1] = main_task_matrix[main_task_pos][rel_pos(i, sub_matrix_size - 1)];
    break;
  case NORTHWEST:
    shared_data[0] = main_task_matrix[main_task_pos][rel_pos(0, 0)];
    break;
  case NORTHEAST:
    shared_data[0] = main_task_matrix[main_task_pos][rel_pos(0, sub_matrix_size - 1)];
    break;
  case SOUTHWEST:
    shared_data[0] = main_task_matrix[main_task_pos][rel_pos(sub_matrix_size - 1, 0)];
    break;
  case SOUTHEAST:
    shared_data[0] = main_task_matrix[main_task_pos][rel_pos(sub_matrix_size - 1, sub_matrix_size - 1)];
    break;
  default:
    report(1, "we have a problem here ...");
    assert(0);
  }
}

DEFINE_THREAD(arg_t) {
  /* Some data declarations */
  bool has_neighbor[4] = {false};
  float * za = 0;
  float * zb = 0;
  float * zu = 0;
  float * zr = 0;
  float * zv = 0;
  float * zz = 0;
  orwl_handle2 main_task_handle = ORWL_HANDLE2_INITIALIZER;
  orwl_handle2 my_task_handle = ORWL_HANDLE2_INITIALIZER;
  orwl_handle2 handle_distant_pos[12] = {ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER,
					 ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER,
					 ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER,
					 ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER,
					 ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER,
					 ORWL_HANDLE2_INITIALIZER, ORWL_HANDLE2_INITIALIZER};

  /***************************************************************************/
  /*                              Connection step                            */
  /***************************************************************************/
  /* Only the main task makes connections on distant guys */
  if (Arg->sub_task == MAIN) {
    for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
      size_t neighbor_sub_task = get_sub_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
      size_t neighbor_main_task = get_main_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
      size_t location_pos = shift_local_locations + (Arg->main_task_pos * 12) + distant_task_to_local_pos(neighbor_sub_task, Arg->main_task, neighbor_main_task);
      orwl_make_distant_connection(Arg->vertex->neighbors[i], &srv, graph, ab, &locations[location_pos]);
    }
  }

  /* Wait to take all the distant locks */
  orwl_wait_to_initialize_locks(Arg->id, graph, ab,  seed);
  if (Arg->sub_task == MAIN) {  
    report(0, "%zu is starting lock init", Arg->id);
  }

  /***************************************************************************/
  /*                         Lock initialization step                        */
  /***************************************************************************/
  /* Send the lock requests */
  if (Arg->sub_task == MAIN) {
    /* Take the local lock in write mode */
    orwl_write_request2_instr(&locations[Arg->my_location_pos], &my_task_handle, 1);
    /* Take the distant locks in read mode*/
    for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
      size_t neighbor_sub_task = get_sub_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
      size_t neighbor_main_task = get_main_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
      size_t handle_pos = distant_task_to_local_pos(neighbor_sub_task, Arg->main_task, neighbor_main_task);
      if ((handle_pos == POS_NORTH) || (handle_pos == POS_SOUTH) ||
	  (handle_pos == POS_EAST) || (handle_pos == POS_WEST))
	has_neighbor[handle_pos] = true;
      size_t location_pos = shift_local_locations + (Arg->main_task_pos * 12) + handle_pos;    
      orwl_read_request2_instr(&locations[location_pos], &handle_distant_pos[handle_pos], 1);
      report(0, "connecting %zu on %zu (mt %zu - st %zu - loc %zu)", Arg->id, Arg->vertex->neighbors[i], neighbor_main_task, neighbor_sub_task, location_pos);
    }
  } else {
    /* Take the local lock in write mode */
    orwl_write_request2_instr(&locations[Arg->my_location_pos], &my_task_handle, 0);
    /* Take a lock on the main task in read mode */
    size_t pos = Arg->my_location_pos - Arg->sub_task;
    orwl_read_request2_instr(&locations[pos], &main_task_handle, 0);
  }

  /* Check if my neighbors are ready before starting, then realease the handle on my location */
  orwl_wait_to_start(Arg->id, graph, ab, &srv, nb_main_tasks * 9, seed);
  if (Arg->sub_task == MAIN) {  
    report(0, "%zu is starting the init iteration", Arg->id);
  }
  /* Fire ! */
  float qa;
  size_t pos;
  float * old_za = float_vnew(sub_matrix_size * sub_matrix_size);

  /***************************************************************************/
  /*                       Initialization iteration                          */
  /***************************************************************************/
  if (Arg->sub_task == MAIN) {
    //report(1, "Main task %zu: intialization iteration", Arg->id / 9);
    printf("Main task %zu: intialization iteration\n", Arg->id / 9);
    /* Data initialization */
    za = float_vnew(sub_matrix_size * sub_matrix_size);
    zb = float_vnew(sub_matrix_size * sub_matrix_size);
    zu = float_vnew(sub_matrix_size * sub_matrix_size);
    zr = float_vnew(sub_matrix_size * sub_matrix_size);
    zv = float_vnew(sub_matrix_size * sub_matrix_size);
    zz = float_vnew(sub_matrix_size * sub_matrix_size);
    main_task_matrix[Arg->main_task_pos] = za;
    /* Za, Zb, Zu, Zr, Zv, and Zz initialization */
    int r = 0;
    for (size_t i = 0; i < sub_matrix_size * sub_matrix_size; i++) {
      random_r(&rand_states[Arg->main_task_pos], &r); 
      za[i] = (float)(10.0 * r/(RAND_MAX - 1));
      random_r(&rand_states[Arg->main_task_pos], &r); 
      zb[i] = (float)(10.0 * r/(RAND_MAX - 1));
      random_r(&rand_states[Arg->main_task_pos], &r); 
      zu[i] = (float)(10.0 * r/(RAND_MAX - 1));
      random_r(&rand_states[Arg->main_task_pos], &r); 
      zv[i] = (float)(10.0 * r/(RAND_MAX - 1));
      random_r(&rand_states[Arg->main_task_pos], &r); 
      zz[i] = (float)(10.0 * r/(RAND_MAX - 1));
      random_r(&rand_states[Arg->main_task_pos], &r); 
    }

    orwl_acquire2_instr(&my_task_handle, 1);
    /* Take the locks on the neighbors to keep the same count of iterations */
    for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
      size_t neighbor_sub_task = get_sub_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
      size_t neighbor_main_task = get_main_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
      size_t handle_pos = distant_task_to_local_pos(neighbor_sub_task, Arg->main_task, neighbor_main_task);
      orwl_acquire2_instr(&handle_distant_pos[handle_pos], 1);
      orwl_release2_instr(&handle_distant_pos[handle_pos], 1);
    }
    orwl_release2_instr(&my_task_handle, 1);
  } else {
    orwl_acquire2_instr(&my_task_handle, 0);
    /* Frontier resizing */
    size_t frontier_size = (Arg->sub_task < 5) ? (sub_matrix_size - 2) : 1;
    orwl_truncate2_instr(&my_task_handle, frontier_size * sizeof(float), 0);

    orwl_acquire2_instr(&main_task_handle, 0);
    /* First input in frontier */
    update_frontier(Arg->id, Arg->sub_task, Arg->main_task_pos, &my_task_handle);
    orwl_release2_instr(&main_task_handle, 0);
    orwl_release2_instr(&my_task_handle, 0);
  }

  
  struct timeval start = {0};
  struct timeval end = {0};
  if (Arg->sub_task == MAIN)
    gettimeofday(&start, P99_0(struct timezone*));
  /***************************************************************************/
  /*                         Computation iterations                          */
  /***************************************************************************/

  for (size_t iter = 0 ; iter < iterations ; iter++) {
    if (Arg->sub_task == MAIN) {
      //report(1, "Main task %zu: iteration %zu", Arg->id / 9, iter);
      printf("Main task %zu: iteration %zu\n", Arg->id / 9, iter);
      memcpy(old_za, za, sub_matrix_size * sub_matrix_size * sizeof(float));

      /* Inner computation */
      for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
      	{
      	  for (size_t j = 1 ; j < (sub_matrix_size - 1); j++)
      	    {
      	      pos = rel_pos(i, j);
      	      qa = old_za[rel_pos(i + 1, j)] * zr[pos] + old_za[rel_pos(i - 1, j)] * zb[pos] +
      		old_za[rel_pos(i, j + 1)] * zu[pos] + old_za[rel_pos(i, j - 1)] * zv[pos] +
      		zz[pos];
      	      za[rel_pos(i, j)] = 0.175 * (qa - old_za[rel_pos(i, j)]);
      	    }
      	}


      orwl_acquire2_instr(&my_task_handle, 1);
      for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
	size_t neighbor_sub_task = get_sub_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
	size_t neighbor_main_task = get_main_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
	size_t handle_pos = distant_task_to_local_pos(neighbor_sub_task, Arg->main_task, neighbor_main_task);
	orwl_acquire2_instr(&handle_distant_pos[handle_pos], 1);
      }

      //size_t edge_frontier_size = (sub_matrix_size - 2) * sizeof(float);
      size_t edge_frontier_size = 0;
      /* North computation */
      if (has_neighbor[NORTH - 1]) {
	float const* shared_edge = orwl_read_map2_instr(&handle_distant_pos[POS_NORTH], edge_frontier_size, 1);
	for (size_t i = 1 ; i < (sub_matrix_size - 1); i++)
	  {
	    pos = rel_pos(0, i);
	    qa = old_za[rel_pos(1, i)] * zr[pos] + shared_edge[i - 1] * zb[pos] + old_za[rel_pos(0, i + 1)] * zu[pos] + old_za[rel_pos(0, i - 1)] * zv[pos] + zz[pos];
	    za[rel_pos(0, i)] = 0.175 * (qa - old_za[rel_pos(0, i)]);
	  }
      }

      /* South computation */
      if (has_neighbor[SOUTH - 1]) {
	float const* shared_edge = orwl_read_map2_instr(&handle_distant_pos[POS_SOUTH], edge_frontier_size, 1);
	for (size_t i = 1 ; i < (sub_matrix_size - 1); i++)
	  {
	    pos = rel_pos(sub_matrix_size - 1, i);
	    qa = shared_edge[i - 1] * zr[pos] + old_za[rel_pos(sub_matrix_size - 2, i)] * zb[pos] + old_za[rel_pos(sub_matrix_size - 1, i + 1)] * zu[pos] + old_za[rel_pos(sub_matrix_size - 1, i - 1)] * zv[pos] + zz[pos];
	    za[rel_pos(sub_matrix_size - 1, i)] = 0.175 * (qa - old_za[rel_pos(sub_matrix_size - 1, i)]);
	  }
      }

      /* West computation */
      if (has_neighbor[WEST - 1]) {
	float const* shared_edge = orwl_read_map2_instr(&handle_distant_pos[POS_WEST], edge_frontier_size, 1);
	for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
	  {
	    pos = rel_pos(i, 0);
	    qa = old_za[rel_pos(i + 1, 0)] * zr[pos] + old_za[rel_pos(i - 1, 0)] * zb[pos] + old_za[rel_pos(i, 1)] * zu[pos] + shared_edge[i - 1] * zv[pos] + zz[pos];
	    za[rel_pos(i, 0)] = 0.175 * (qa - old_za[rel_pos(i, 0)]);
	  }
      }

      /* East computation */
      if (has_neighbor[EAST - 1]) {
	float const* shared_edge = orwl_read_map2_instr(&handle_distant_pos[POS_EAST], edge_frontier_size, 1);
	for (size_t i = 1 ; i < (sub_matrix_size - 1) ; i++)
	  {
	    pos = rel_pos(i, sub_matrix_size - 1);
	    qa = old_za[rel_pos(i + 1, sub_matrix_size - 1)] * zr[pos] + old_za[rel_pos(i - 1, sub_matrix_size - 1)] * zb[pos] + shared_edge[i - 1] * zu[pos] + old_za[rel_pos(i, sub_matrix_size - 2)] * zv[pos] + zz[pos];
	    za[rel_pos(i, sub_matrix_size - 1)] = 0.175 * (qa - old_za[rel_pos(i, sub_matrix_size - 1)]);
	  }
      }
      
      //size_t corner_frontier_size = 1 * sizeof(float);
      size_t corner_frontier_size = 0;
      /* North east computation */
      if (has_neighbor[NORTH - 1] && has_neighbor[EAST - 1]) {
	float const* shared_corner1 = orwl_read_map2_instr(&handle_distant_pos[POS_NORTHEASTNORTH], corner_frontier_size, 1);
	float const* shared_corner2 = orwl_read_map2_instr(&handle_distant_pos[POS_NORTHEASTEAST], corner_frontier_size, 1);
	pos = rel_pos(0, sub_matrix_size - 1);
	qa = old_za[rel_pos(1, sub_matrix_size - 1)] * zr[pos] + shared_corner1[0] * zb[pos] + shared_corner2[0] * zu[pos] + old_za[rel_pos(0, sub_matrix_size - 2)] * zv[pos] + zz[pos];
	za[rel_pos(0, sub_matrix_size - 1)] = 0.175 * (qa - old_za[rel_pos(0, sub_matrix_size - 1)]);
      }

      /* North west computation */
      if (has_neighbor[NORTH - 1] && has_neighbor[WEST - 1]) {
	float const* shared_corner1 = orwl_read_map2_instr(&handle_distant_pos[POS_NORTHWESTNORTH], corner_frontier_size, 1);
	float const* shared_corner2 = orwl_read_map2_instr(&handle_distant_pos[POS_NORTHWESTWEST], corner_frontier_size, 1);
	pos = rel_pos(0, 0);
	qa = old_za[rel_pos(1, 0)] * zr[pos] + shared_corner1[0] * zb[pos] + old_za[rel_pos(0, 1)] * zu[pos] + shared_corner2[0] * zv[pos] + zz[pos];
	za[rel_pos(0, 0)] = 0.175 * (qa - old_za[rel_pos(0, 0)]);
      }

      /* South east computation */
      if (has_neighbor[SOUTH - 1] && has_neighbor[EAST - 1]) {
	float const* shared_corner1 = orwl_read_map2_instr(&handle_distant_pos[POS_SOUTHEASTSOUTH], corner_frontier_size, 1);
	float const* shared_corner2 = orwl_read_map2_instr(&handle_distant_pos[POS_SOUTHEASTEAST], corner_frontier_size, 1);
	pos = rel_pos(sub_matrix_size - 1, sub_matrix_size - 1);
	qa =  shared_corner1[0] * zr[pos] + old_za[rel_pos(sub_matrix_size - 2, sub_matrix_size - 1)] * zb[pos] + shared_corner2[0] * zu[pos] + old_za[rel_pos(sub_matrix_size - 1, sub_matrix_size - 2)] * zv[pos] + zz[pos];
	za[rel_pos(sub_matrix_size - 1, sub_matrix_size - 1)] = 0.175 * (qa - old_za[rel_pos(sub_matrix_size - 1, sub_matrix_size - 1)]);
      }

      /* South west computation */
      if (has_neighbor[SOUTH - 1] && has_neighbor[WEST - 1]) {
	float const* shared_corner1 = orwl_read_map2_instr(&handle_distant_pos[POS_SOUTHWESTSOUTH], corner_frontier_size, 1);
	float const* shared_corner2 = orwl_read_map2_instr(&handle_distant_pos[POS_SOUTHWESTWEST], corner_frontier_size, 1);
	pos = rel_pos(sub_matrix_size - 1, 0);
	qa = shared_corner1[0] * zr[pos] + old_za[rel_pos(sub_matrix_size - 2, 0)] * zb[pos] + old_za[rel_pos(sub_matrix_size - 1, 1)] * zu[pos] + shared_corner2[0] * zv[pos] + zz[pos];
	za[rel_pos(sub_matrix_size - 1, 0)] = 0.175 * (qa - old_za[rel_pos(sub_matrix_size - 1, 0)]);
      }


      for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++) {
	size_t neighbor_sub_task = get_sub_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
	size_t neighbor_main_task = get_main_task_from_label(graph->vertices[Arg->vertex->neighbors[i]].label);
	size_t handle_pos = distant_task_to_local_pos(neighbor_sub_task, Arg->main_task, neighbor_main_task);
	orwl_release2_instr(&handle_distant_pos[handle_pos], 1);
      }

      orwl_release2_instr(&my_task_handle, 1);


    } else {
      /* update my buffer */
      orwl_acquire2_instr(&my_task_handle, 0);
      orwl_acquire2_instr(&main_task_handle, 0);
      update_frontier(Arg->id, Arg->sub_task, Arg->main_task_pos, &my_task_handle);
      orwl_release2_instr(&main_task_handle, 0);
      orwl_release2_instr(&my_task_handle, 0);
    }
  }
  /* Cancel pending requests */
  orwl_cancel2_instr(&main_task_handle, 1);
  orwl_cancel2_instr(&my_task_handle, 1);

  if (Arg->id == list_tasks[0])
    print_statistics_instr(Arg->id);
  if (Arg->sub_task == MAIN)
    {
      for (size_t i = 0 ; i < Arg->vertex->nb_neighbors ; i++)
      	orwl_cancel2_instr(&handle_distant_pos[i], 1);
      gettimeofday(&end, P99_0(struct timezone*));
      long diff = tv_diff(start, end);
      printf("__%zu__tpi:%f\n", Arg->id, diff / (double)iterations);
    }
  ORWL_CRITICAL {
    count++;
    if (count == (nb_main_tasks * 9)) {
      sleep(2);
      report(0, "Everybody has finished");
    }
  }
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
      size_t main_task = str2uz(token);
      for (size_t i = 0 ; i < 9 ; i++) {
	list_id[count] = main_task + i;	
	count++;
      }
      token = strtok_r(P99_0(char*), &delim[0], &saveptr);
    }
  } else {
    /* if no separator is found, there is only one main task */
    size_t main_task = str2uz(str);
    for (size_t i = 0 ; i < 9 ; i++)
      list_id[i] = main_task + i;
    count = 9;
  }
  free(tmp);
  return count;
}

int main(int argc, char **argv) {
  if (argc < 9) {
    report(1, "only %d commandline arguments, this ain't enough", argc);
    return 1;
  }

  char graph_file[256] = {0};
  char global_ab_file[256] = {0};
  char local_ab_file[256] = {0};
  char main_tasks[4096] = {0};
  sub_matrix_size = strtouz(argv[1]);
  iterations = strtouz(argv[2]);
  global_rows = strtouz(argv[3]);
  global_lines = strtouz(argv[4]);
  strncpy(graph_file, argv[5], 256);
  strncpy(global_ab_file, argv[6], 256);
  strncpy(local_ab_file, argv[7], 256);
  strncpy(main_tasks, argv[8], 4096);

  global_nb_tasks = global_rows * global_lines * 9;

  /*
    ./orwl_benchmark_kernel23 100 10 2 2 graph2.txt global_ab.txt local_ab.txt 0,9,18,27
   */
  nb_main_tasks = strcountchr(main_tasks, ",") + 1;
  const size_t nb_tasks = nb_main_tasks * 9;
  list_tasks = size_t_vnew(nb_tasks);
  const size_t nb_locations = nb_main_tasks * 21;
  orwl_barrier_init(&init_barr, nb_tasks);
  get_task(main_tasks, list_tasks, ",");
  size_t list_locations[nb_tasks];
  for (int i = 0 ; i < nb_tasks ; i++)
    list_locations[i] = i;

  shift_local_locations = nb_tasks; /* before: exported locations, after: imported locations */

  /* local server initialization */
  seed = seed_get();
  orwl_types_init();
  orwl_start(&srv, SOMAXCONN, nb_locations);

  if (!orwl_alive(&srv)) return EXIT_FAILURE;

  orwl_server_block(&srv);

  locations = orwl_mirror_vnew(nb_locations);

  for (size_t i = 0 ; i < nb_tasks ; i++)
    orwl_make_local_connection(i, &srv, &locations[i]);

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
  report(0, "wait and load init files done");

  main_task_matrix = (float **)calloc(nb_main_tasks, sizeof(float*));
  rand_states = (struct random_data*)calloc(nb_main_tasks, sizeof(struct random_data));
  rand_statebufs = (char*)calloc(nb_main_tasks, 32);
  for (size_t i = 0 ; i < nb_main_tasks ; i++)
    initstate_r(random(), &rand_statebufs[i], 32, &rand_states[i]);

  pthread_t *tid = pthread_t_vnew(nb_tasks);
  arg_t *arg = arg_t_vnew(nb_tasks);
  for (size_t i = 0 ; i < nb_tasks ; i++) {
    arg_t *myarg = &arg[i];
    myarg->id = list_tasks[i];
    myarg->vertex = &graph->vertices[list_tasks[i]];
    myarg->my_location_pos = list_locations[i];
    myarg->main_task = get_main_task_from_label(graph->vertices[list_tasks[i]].label);
    myarg->main_task_pos = i / 9;
    myarg->sub_task = get_sub_task_from_label(graph->vertices[list_tasks[i]].label);
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
