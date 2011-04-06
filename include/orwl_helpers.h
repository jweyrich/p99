
#ifndef   	ORWL_HELPERS_H_
# define   	ORWL_HELPERS_H_

#include "orwl_endpoint.h"
#include "orwl_server.h"
#include "orwl_remote_queue.h"
#include "orwl_int.h"

P99_DECLARE_STRUCT(orwl_vertex);
P99_DECLARE_STRUCT(orwl_graph);
P99_DECLARE_STRUCT(orwl_address_book);


struct orwl_vertex {
  size_t color;
  size_t nb_neighbors;
  char label[16];
  size_t * neighbors;
};

inline
orwl_vertex* orwl_vertex_init(orwl_vertex *vertex, size_t nb_neighbors) {
  if (vertex) {
    *vertex = P99_LVAL(orwl_vertex,
		       .color = 0,
		       .nb_neighbors = nb_neighbors,
		       .neighbors = NULL,
		       .label = {0},
		       );
    if (nb_neighbors > 0)
      vertex->neighbors = size_t_vnew(nb_neighbors);
  }
 
  return vertex;
}

inline
P99_PROTOTYPE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t);
#define orwl_vertex_init(...) P99_CALL_DEFARG(orwl_vertex_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_vertex_init, , P99_0(size_t));

inline
void orwl_vertex_destroy(orwl_vertex *vertex) {
  size_t_vdelete(vertex->neighbors);
}

DECLARE_NEW_DELETE(orwl_vertex);

struct orwl_graph {
  size_t nb_vertices;
  orwl_vertex *vertices;
};

inline
orwl_graph * orwl_graph_init(orwl_graph * graph, size_t nb_vertices) {
  graph->nb_vertices = nb_vertices;
  graph->vertices = orwl_vertex_vnew(nb_vertices);
  return graph;
}

inline
P99_PROTOTYPE(orwl_graph*, orwl_graph_init, orwl_graph *, size_t);
#define orwl_graph_init(...) P99_CALL_DEFARG(orwl_graph_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_graph_init, , P99_0(size_t));

inline
void orwl_graph_destroy(orwl_graph *graph) {
  orwl_vertex_vdelete(graph->vertices);
}

DECLARE_NEW_DELETE(orwl_graph);

orwl_graph * orwl_graph_read(orwl_graph * graph, char const* file, size_t nb_vertices);

struct orwl_address_book {
  size_t nb_vertices;
  orwl_endpoint *eps;
  size_t *locations;
};

inline
orwl_address_book* orwl_address_book_init(orwl_address_book *ab,
					  size_t nb_vertices) {
  if (ab) {
    *ab = P99_LVAL(orwl_address_book,
		   .nb_vertices = nb_vertices,
		   );
    if (nb_vertices > 0) {
      ab->eps = orwl_endpoint_vnew(nb_vertices);
      ab->locations = size_t_vnew(nb_vertices);
    }
  }
  return ab;
}

inline
P99_PROTOTYPE(orwl_address_book*, orwl_address_book_init, orwl_address_book *, size_t);
#define orwl_address_book_init(...) P99_CALL_DEFARG(orwl_address_book_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_address_book_init, , P99_0(size_t));

inline
void orwl_address_book_destroy(orwl_address_book *ab) {
  orwl_endpoint_vdelete(ab->eps);
  size_t_vdelete(ab->locations);
}

DECLARE_NEW_DELETE(orwl_address_book);

orwl_address_book* orwl_address_book_read(orwl_address_book *ab, char const *file, size_t nb_vertices);

bool orwl_wait_and_load_init_files(orwl_address_book **ab,
				   const char *ab_filename,
				   orwl_graph **graph,
				   const char *graph_filename,
				   orwl_server *serv,
				   const char *id_filename,
				   size_t nb_id,
				   size_t *list_id,
				   size_t *list_locations,
				   size_t nb_vertices);

void orwl_make_distant_connection(size_t dest_id,
				  orwl_server *server,
				  orwl_graph *graph, 
				  orwl_address_book *ab,
				  orwl_mirror *location);

void orwl_make_local_connection(size_t dest_id,
				orwl_server *server,
				orwl_mirror *location);

bool orwl_wait_to_initialize_locks(size_t id,
				   orwl_graph *graph,
				   orwl_address_book *ab,
				   rand48_t *seed);

bool orwl_wait_to_start(size_t id,
			orwl_graph *graph,
			orwl_address_book *ab,
			orwl_server *server,
			orwl_handle *h,
			rand48_t *seed);

void orwl_lock_locations(size_t start, size_t nb_tasks, 
			 orwl_handle *initialization_handle,
			 orwl_mirror *locations);

#endif
