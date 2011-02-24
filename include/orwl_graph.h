
#ifndef   	ORWL_GRAPH_H_
# define   	ORWL_GRAPH_H_

#include "orwl_int.h"

P99_DECLARE_STRUCT(orwl_neighbor);

struct orwl_neighbor {
  size_t id;
  size_t nb_locations;
  size_t * locations;
};

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
      neighbor->locations = size_t_vnew(nb_locations);
  }
  return neighbor;
}

P99_PROTOTYPE(orwl_neighbor*, orwl_neighbor_init, orwl_neighbor*, size_t, size_t);
#define orwl_neighbor_init(...) P99_CALL_DEFARG(orwl_neighbor_init, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_neighbor_init, , P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(orwl_neighbor_init, , P99_0(size_t), P99_0(size_t));


void orwl_neighbor_destroy(orwl_neighbor *neighbor) {
  size_t_vdelete(neighbor->locations);
}

DECLARE_NEW_DELETE(orwl_neighbor);
DEFINE_NEW_DELETE(orwl_neighbor);


P99_DECLARE_STRUCT(orwl_vertex);

struct orwl_vertex {
  size_t id;
  size_t color;
  size_t nb_neighbors;
  struct orwl_neighbor * neighbors;
};

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

P99_PROTOTYPE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t, size_t, size_t);
#define orwl_vertex_init(...) P99_CALL_DEFARG(orwl_vertex_init, 4, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_vertex_init, , P99_0(size_t), P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(orwl_vertex_init, , P99_0(size_t), P99_0(size_t), P99_0(size_t));


void orwl_vertex_destroy(orwl_vertex *vertex) {
  orwl_neighbor_vdelete(vertex->neighbors);
}

DECLARE_NEW_DELETE(orwl_vertex);
DEFINE_NEW_DELETE(orwl_vertex);


P99_DECLARE_STRUCT(orwl_graph);

struct orwl_graph {
  size_t nb_vertices;
  orwl_vertex *vertices;
};

orwl_graph * orwl_graph_init(orwl_graph * graph, size_t nb_vertices) {
  graph->nb_vertices = nb_vertices;
  graph->vertices = orwl_vertex_vnew(nb_vertices);
  return graph;
}

P99_PROTOTYPE(orwl_graph*, orwl_graph_init, orwl_graph *, size_t);
#define orwl_graph_init(...) P99_CALL_DEFARG(orwl_graph_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_graph_init, , P99_0(size_t));
P99_DEFINE_DEFARG(orwl_graph_init, , P99_0(size_t));

void orwl_graph_destroy(orwl_graph *graph) {
  orwl_vertex_vdelete(graph->vertices);
}

DECLARE_NEW_DELETE(orwl_graph);
DEFINE_NEW_DELETE(orwl_graph);



orwl_graph * orwl_graph_read(orwl_graph * graph, char const* file);



#endif
