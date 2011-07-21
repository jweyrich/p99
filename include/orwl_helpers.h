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
#ifndef     ORWL_HELPERS_H_
# define    ORWL_HELPERS_H_

#include "orwl_endpoint.h"
#include "orwl_server.h"
#include "orwl_handle2.h"
#include "orwl_int.h"

P99_DECLARE_STRUCT(orwl_vertex);
P99_DECLARE_STRUCT(orwl_graph);
P99_DECLARE_STRUCT(orwl_address_book);


/**
 ** @brief A vertex in a directed colored graph
 **/
struct orwl_vertex {
  size_t color;
  char label[16];
  size_t nb_neighbors;
  size_t * neighbors;
};

/**
 ** @brief Dynamically initialize a ::orwl_vertex
 **
 ** @param vertex is the vertex to be initialized
 ** @param nb_neighbors is the number of neighbors on which vertex
 **        is connected.
 **/
DOCUMENT_INIT(orwl_vertex)
inline
orwl_vertex* orwl_vertex_init(orwl_vertex *vertex, size_t nb_neighbors) {
  if (vertex) {
    *vertex = P99_LVAL(orwl_vertex,
                       .color = 0,
                       .nb_neighbors = nb_neighbors,
                       .neighbors = (nb_neighbors
                                     ? vertex->neighbors = size_t_vnew(nb_neighbors)
                                         : 0),
                       .label = P99_INIT,
                      );
  }

  return vertex;
}

inline
P99_PROTOTYPE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t);
#define orwl_vertex_init(...) P99_CALL_DEFARG(orwl_vertex_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_vertex_init, , P99_0(size_t));

DOCUMENT_DESTROY(orwl_vertex)
inline
void orwl_vertex_destroy(orwl_vertex *vertex) {
  size_t_vdelete(vertex->neighbors);
}

DECLARE_NEW_DELETE(orwl_vertex);

/**
 ** @brief A directed and colored graph
 **/
struct orwl_graph {
  size_t nb_vertices;
  orwl_vertex *vertices;
};

/**
 ** @brief Dynamically initialize a ::orwl_graph
 **
 ** @param graph is the graph to be initialized
 ** @param nb_vertices is the number of vertices in the graph
 **/
DOCUMENT_INIT(orwl_graph)
inline
orwl_graph * orwl_graph_init(orwl_graph * graph, size_t nb_vertices) {
  if (graph) {
    *graph = P99_LVAL(orwl_graph,
                      .nb_vertices = nb_vertices,
                      .vertices = orwl_vertex_vnew(nb_vertices)
                     );
  }
  return graph;
}

inline
P99_PROTOTYPE(orwl_graph*, orwl_graph_init, orwl_graph *, size_t);
#define orwl_graph_init(...) P99_CALL_DEFARG(orwl_graph_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_graph_init, , P99_0(size_t));

DOCUMENT_DESTROY(orwl_graph)
inline
void orwl_graph_destroy(orwl_graph *graph) {
  orwl_vertex_vdelete(graph->vertices);
}

DECLARE_NEW_DELETE(orwl_graph);

/**
 ** @brief Read a directed and colored graph from a file in dot format
 **
 ** @param graph is a pointer on a ::orwl_graph*. Warning, memory
 **        is allocated inside this function.
 ** @param file is the path to a dot file
 ** @param nb_vertices is the number of vertices that are expected to be
 **        read in the graph
 ** @return true if the read has been correctly performed, false otherwise
 ** @memberof orwl_graph
 **/
bool orwl_graph_read(orwl_graph ** graph, char const* file, size_t nb_vertices);

/**
 ** @brief An address book that contains the endpoint and the location
           associated to all the vertices
 **/
struct orwl_address_book {
  size_t nb_vertices;
  orwl_endpoint *eps;
  size_t *locations;
};

/**
 ** @brief Dynamically initialize a ::orwl_address_book
 **
 ** @param ab is the address book to be initialized
 ** @param nb_vertices is the number of vertices in the graph (thus,
 **        in the address book)
 **/
DOCUMENT_INIT(orwl_address_book)
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

DOCUMENT_DESTROY(orwl_address_book)
inline
void orwl_address_book_destroy(orwl_address_book *ab) {
  orwl_endpoint_vdelete(ab->eps);
  size_t_vdelete(ab->locations);
}

DECLARE_NEW_DELETE(orwl_address_book);

/**
 ** @brief Read an address book
 **
 ** @param ab is a pointer on a ::orwl_address_book*. Warning, memory
 **        is allocated inside this function.
 ** @param file is the path to the address book file to be read
 ** @param nb_vertices is the number of entries that are expected to be
 **        read in the address book file
 ** @return true if the read has been correctly performed, false otherwise
 ** @memberof orwl_address_book
 **/
bool orwl_address_book_read(orwl_address_book **ab, char const *file, size_t nb_vertices);

/**
 ** @brief Block the application until a kick-off is given and load
 **        initialization files (graph and address book). Once called,
 **        this function produces a part of the address book with local
 **        information and writes it to id_filename. The idea is to
 **        collect the id_filename files on all the nodes with an external
 **        application that concatenates it to a global address book. Then
 **        The external application has to send the global address book on
 **        the nodes (the expected file name is specified in ab_filename).
 **        Finally, once the global address book is copied everywhere, the
 **        kick-off can be performed by deleting id_filename on the nodes.
 **
 ** @param ab is a pointer on a ::orwl_address_book*. Warning, memory
 **        is allocated inside this function
 ** @param ab_filename is the expected path to the global address book
 ** @param graph is a pointer on a ::orwl_graph*. Warning, memory
 **        is allocated inside this function
 ** @param graph is the path to the dot file containing the graph description
 ** @param serv is a pointer on the local ::orwl_server
 ** @param id_filename is the path to the partial address book produced
 ** @param nb_id is the number of vertices running on the local server
 ** @param list_id is an array containing the id of the local vertices
 ** @param list_locations is an array containing the location managed
 **        by a the local vertices
 ** @param nb_vertices is the global number of vertices in the graph
 ** @return true if everything has been correctly performed, false otherwise
 **/
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

/**
 ** @brief Connect a distant location to the local server
 **
 ** @param dest_id is the id of the distant vertex
 ** @param server is a pointer on the local ::orwl_server
 ** @param graph is a pointer on a ::orwl_graph
 ** @param ab is a pointer on a ::orwl_address_book
 ** @param location is a pointer on a local ::orwl_mirror
 **        where the distant location must be mirrored
 **/
void orwl_make_distant_connection(size_t dest_id,
                                  orwl_server *server,
                                  orwl_graph *graph,
                                  orwl_address_book *ab,
                                  orwl_mirror *location);

/**
 ** @brief Connect a local location to the local server
 **
 ** @param dest_id is the id of the local vertex
 ** @param server is a pointer on the local ::orwl_server
 ** @param location is a pointer on a local ::orwl_mirror
 **        where the local vertex location must be mirrored
 **/
void orwl_make_local_connection(size_t dest_id,
                                orwl_server *server,
                                orwl_mirror *location);


/**
 ** @brief Block an application thread until the corresponding
 **        vertex is able to initialize its locks. This is based
 **        on graph coloring and the policy is to block a vertex
 **        until all its neighbors in the undirected graph with
 **        a lower color are initialized.
 **
 ** @param id is the id of the vertex corresponding to the
 **        application thread
 ** @param graph is a pointer on a ::orwl_graph
 ** @param ab is a pointer on a ::orwl_address_book
 ** @param seed is a pointer on a ::rand48_t (required because
 **        RPC can be launched)
 ** @return true if everything has been correctly performed, false otherwise
 **/
bool orwl_wait_to_initialize_locks(size_t id,
                                   orwl_graph *graph,
                                   orwl_address_book *ab,
                                   rand48_t *seed);

/**
 ** @brief Block an application thread until the neighbors (in
 **        the undirected graph) of the corresponding vertex
 **        have initialized their locks.
 **
 ** @param id is the id of the vertex corresponding to the
 **        application thread
 ** @param graph is a pointer on a ::orwl_graph
 ** @param ab is a pointer on a ::orwl_address_book
 ** @param server is a pointer on the local ::orwl_server
 ** @param nb_local_tasks is the number of local vertices
 ** @param seed is a pointer on a ::rand48_t (required because
 **        RPC can be launched)
 ** @return true if everything has been correctly performed, false otherwise
 **/
bool orwl_wait_to_start(size_t id,
                        orwl_graph *graph,
                        orwl_address_book *ab,
                        orwl_server *server,
                        size_t nb_local_tasks,
                        rand48_t *seed);

#endif
