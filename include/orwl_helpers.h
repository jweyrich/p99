
#ifndef   	ORWL_HELPERS_H_
# define   	ORWL_HELPERS_H_

#include "orwl_endpoint.h"
#include "orwl_server.h"
#include "orwl_remote_queue.h"
#include "orwl_int.h"

P99_DECLARE_STRUCT(orwl_locations);
P99_DECLARE_STRUCT(orwl_neighbor);
P99_DECLARE_STRUCT(orwl_vertex);
P99_DECLARE_STRUCT(orwl_graph);
P99_DECLARE_STRUCT(orwl_id);
P99_DECLARE_STRUCT(orwl_address_book);

struct orwl_locations {
  size_t local;
  size_t distant;
};

inline
orwl_locations* orwl_locations_init(orwl_locations *locations,
				    size_t local,
				    size_t distant) {
  if (locations) {
    *locations = P99_LVAL(orwl_locations,
			  .local = local,
			  .distant = distant,
			  );
  }
  return locations;
}

inline
P99_PROTOTYPE(orwl_locations*, orwl_locations_init, orwl_locations*, size_t, size_t);
#define orwl_locations_init(...) P99_CALL_DEFARG(orwl_locations_init, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_locations_init, , P99_0(size_t), P99_0(size_t));

inline
void orwl_locations_destroy(orwl_locations *locations) {
}

DECLARE_NEW_DELETE(orwl_locations);

struct orwl_neighbor {
  size_t id;
  size_t nb_locations;
  orwl_locations * locations;
};

inline
orwl_neighbor* orwl_neighbor_init(orwl_neighbor *neighbor,
				  size_t id,
				  size_t nb_locations) {
  if (neighbor) {
    *neighbor = P99_LVAL(orwl_neighbor,
			 .id = id,
			 .nb_locations = nb_locations,
			 .locations = NULL,
			 );
    if (nb_locations > 0)
      neighbor->locations = orwl_locations_vnew(nb_locations);
  }
  return neighbor;
}

inline
P99_PROTOTYPE(orwl_neighbor*, orwl_neighbor_init, orwl_neighbor*, size_t, size_t);
#define orwl_neighbor_init(...) P99_CALL_DEFARG(orwl_neighbor_init, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_neighbor_init, , P99_0(size_t), P99_0(size_t));

inline
void orwl_neighbor_destroy(orwl_neighbor *neighbor) {
  orwl_locations_vdelete(neighbor->locations);
}

DECLARE_NEW_DELETE(orwl_neighbor);

struct orwl_vertex {
  size_t id;
  size_t color;
  size_t nb_neighbors;
  struct orwl_neighbor * neighbors;
};

inline
orwl_vertex* orwl_vertex_init(orwl_vertex *vertex,
			      size_t id,
			      size_t color,
			      size_t nb_neighbors) {
  if (vertex) {
    *vertex = P99_LVAL(orwl_vertex,
		       .id = id,
		       .color = color,
		       .nb_neighbors = nb_neighbors,
		       .neighbors = NULL,
		       );
    if (nb_neighbors > 0)
      vertex->neighbors = orwl_neighbor_vnew(nb_neighbors);
  }
 
  return vertex;
}

inline
P99_PROTOTYPE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t, size_t, size_t);
#define orwl_vertex_init(...) P99_CALL_DEFARG(orwl_vertex_init, 4, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_vertex_init, , P99_0(size_t), P99_0(size_t), P99_0(size_t));

inline
void orwl_vertex_destroy(orwl_vertex *vertex) {
  orwl_neighbor_vdelete(vertex->neighbors);
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

orwl_graph * orwl_graph_read(orwl_graph * graph, char const* file);

struct orwl_id {
  size_t num_id;
  orwl_endpoint ep;
};

inline
orwl_id* orwl_id_init(orwl_id *id,
		      size_t num_id,
		      char const *host) {
  if (id)
    *id = P99_LVAL(orwl_id,
		   .num_id = num_id,
		   );
  orwl_endpoint_parse(&id->ep, host);
  return id;
}

inline
P99_PROTOTYPE(orwl_id*, orwl_id_init, orwl_id*, size_t, char const*);
#define orwl_id_init(...) P99_CALL_DEFARG(orwl_id_init, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_id_init, , P99_0(size_t), P99_0(char const*));

inline
void orwl_id_destroy(orwl_id *id) {
}

DECLARE_NEW_DELETE(orwl_id);

struct orwl_address_book {
  size_t nb_ids;
  orwl_id * ids;
};

inline
orwl_address_book* orwl_address_book_init(orwl_address_book *ab,
					  size_t nb_ids) {
  if (ab) {
    *ab = P99_LVAL(orwl_address_book,
		   .nb_ids = nb_ids,
		   );
    if (nb_ids > 0)
      ab->ids = orwl_id_vnew(nb_ids);
  }
  return ab;
}

inline
P99_PROTOTYPE(orwl_address_book*, orwl_address_book_init, orwl_address_book *, size_t);
#define orwl_address_book_init(...) P99_CALL_DEFARG(orwl_address_book_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_address_book_init, , P99_0(size_t));

inline
void orwl_address_book_destroy(orwl_address_book *ab) {
  orwl_id_vdelete(ab->ids);
}

DECLARE_NEW_DELETE(orwl_address_book);

orwl_address_book* orwl_address_book_read(orwl_address_book *ab, char const *file);

bool orwl_wait_and_load_init_files(orwl_address_book *ab,
				   const char *ab_filename,
				   orwl_graph * graph,
				   const char *graph_filename,
				   orwl_server *serv,
				   const char *id_filename,
				   size_t nb_id,
				   size_t *list_id,
				   const char *lock_filename);

void orwl_make_connections(size_t id, 
			   orwl_server *server,
			   orwl_graph *graph, 
			   orwl_address_book *ab,
			   orwl_mirror* locations);

void orwl_wait_to_initialize_locks(size_t id,
				   orwl_graph *graph,
				   orwl_server *server,
				   orwl_mirror *my_locations,
				   orwl_handle *h,
				   size_t nb_locations);

void orwl_wait_to_start(size_t id,
			orwl_graph *graph,
			orwl_server *server,
			orwl_handle *h,
			size_t nb_locations);

#endif
