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
#include "orwl_helpers.h"
#include "p99_c99_default.h"
#include "p99_str.h"

#define MAX_LINE_SIZE 2048

DEFINE_NEW_DELETE(orwl_locations);
DEFINE_NEW_DELETE(orwl_neighbor);
DEFINE_NEW_DELETE(orwl_vertex);
DEFINE_NEW_DELETE(orwl_graph);
DEFINE_NEW_DELETE(orwl_id);
DEFINE_NEW_DELETE(orwl_address_book);

P99_DEFINE_DEFARG(orwl_locations_init, , P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(orwl_neighbor_init, , P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(orwl_vertex_init, , P99_0(size_t), P99_0(size_t), P99_0(size_t));
P99_DEFINE_DEFARG(orwl_graph_init, , P99_0(size_t));
P99_DEFINE_DEFARG(orwl_id_init, , P99_0(size_t), P99_0(char const*));
P99_DEFINE_DEFARG(orwl_address_book_init, , P99_0(size_t));

P99_INSTANTIATE(orwl_locations*, orwl_locations_init, orwl_locations*, size_t, size_t);
P99_INSTANTIATE(void, orwl_locations_destroy, orwl_locations*);
P99_INSTANTIATE(orwl_neighbor*, orwl_neighbor_init, orwl_neighbor*, size_t, size_t);
P99_INSTANTIATE(void, orwl_neighbor_destroy, orwl_neighbor*);
P99_INSTANTIATE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t, size_t, size_t);
P99_INSTANTIATE(void, orwl_vertex_destroy, orwl_vertex*);
P99_INSTANTIATE(orwl_graph*, orwl_graph_init, orwl_graph *, size_t);
P99_INSTANTIATE(void, orwl_graph_destroy, orwl_graph *);
P99_INSTANTIATE(orwl_id*, orwl_id_init, orwl_id*, size_t, char const*);
P99_INSTANTIATE(void, orwl_id_destroy, orwl_id*);
P99_INSTANTIATE(orwl_address_book*, orwl_address_book_init, orwl_address_book *, size_t);
P99_INSTANTIATE(void, orwl_address_book_destroy, orwl_address_book *);


unsigned strcountchr(char *str, char chr) {
  unsigned count = 0;
  char *ptr = str;
  while (ptr != NULL) {
    ptr = strchr(ptr, chr);
    if (ptr != NULL) {
      count++;
      ptr = ptr + 1;
    }
  }
  return count;
}

unsigned strsplit(char const *str, char str_array[][MAX_LINE_SIZE], char delim) {
  unsigned count = 0;
  char *token;
  char *tmp = strdup(str);
  char *saveptr;
  if (strcountchr(tmp, delim) > 0) {
    token = strtok_r(tmp, &delim, &saveptr);
    while (token != NULL) {
      strncpy(str_array[count], token, MAX_LINE_SIZE);
      token = strtok_r(NULL, &delim, &saveptr);
      count++;
    }
  }
  free(tmp);
  return count;
}

unsigned pow2(unsigned exp) {
  if (exp > 0) {
    unsigned n = 1;
    for (unsigned i = 1 ; i < exp ; i++)
      n <<= 1;
    return n;
  } else {
    return 0;
  }
}

orwl_neighbor * orwl_graph_find_neighbor(orwl_vertex *vertex, size_t id) {
  if (vertex != NULL)
    for (size_t i = 0 ; i < vertex->nb_neighbors ; i++)
      if (vertex->neighbors[i].id == id) return &vertex->neighbors[i];
  return NULL;
}

orwl_neighbor * orwl_graph_insert_neighbor(orwl_vertex * vertex,
					   size_t id,
					   size_t nb_locations) {
  if ((vertex != NULL) && orwl_graph_find_neighbor(vertex, id) == NULL) {
    size_t i = 0;
    while (vertex->neighbors[i].id != 0) i++;
    orwl_neighbor_init(&vertex->neighbors[i], id, nb_locations);
    return &vertex->neighbors[i];
  }
  return NULL;
}

orwl_vertex * orwl_graph_find_vertex(orwl_graph * graph, size_t id) {
  if (graph != NULL)
    for (size_t i = 0 ; i < graph->nb_vertices ; i++)
      if (graph->vertices[i].id == id) return &graph->vertices[i];
  return NULL;
}

orwl_vertex * orwl_graph_insert_vertex(orwl_graph * graph,
				       size_t id,
				       size_t color,
				       size_t nb_neighbors) {
  if ((graph != NULL) && orwl_graph_find_vertex(graph, id) == NULL) {
    size_t i = 0;
    while (graph->vertices[i].id != 0) i++;
    orwl_vertex_init(&graph->vertices[i], id, color, nb_neighbors);
    return &graph->vertices[i];
  }
  return NULL;
}

bool orwl_graph_extract_line(orwl_graph * graph, char const * str) {
  /*
    A line pattern is as follow:
    vertex_nb,vertex_color:neighbor-local_location#distant_location(,local_location#distant_location)+(;neighbor-local_location#distant_location(,local_location#distant_location)+)+
   */
  char split_l1[2][MAX_LINE_SIZE] = {0};
  if (strsplit(str, split_l1, ':') != 2)
    return false;
  char split_l11[2][MAX_LINE_SIZE] = {0};
  if (strsplit(split_l1[0], split_l11, ';') != 2)
    return false;
  size_t id = strtouz(split_l11[0]);
  if (id < 1)
    return false;
  size_t color = strtouz(split_l11[1]);
  const size_t neighbors = strcountchr(split_l1[1], ';') + 1;
  orwl_vertex * pvertex = orwl_graph_insert_vertex(graph, id, color, neighbors);
  if (pvertex == NULL) 
    return false;
  char split_l12[neighbors][MAX_LINE_SIZE];
  if (strsplit(split_l1[1], split_l12, ';') != neighbors)
    return false;
  for (size_t i = 0 ; i < neighbors ; i++) {
    char split_l121[2][MAX_LINE_SIZE] = {0};
    if (strsplit(split_l12[i], split_l121, '-') != 2)
      return false;
    size_t id_neighbor = strtouz(split_l121[0]);
    const size_t nb_locations = strcountchr(split_l121[1], ',') + 1;
    orwl_neighbor * pneighbor = orwl_graph_insert_neighbor(pvertex, id_neighbor, nb_locations);
    if (pneighbor == NULL)
      return NULL;
    char split_l122[nb_locations][MAX_LINE_SIZE];
    if (strsplit(split_l121[1], split_l122, ',') != nb_locations)
      return false;
    for (size_t j = 0 ; j < nb_locations ; j++) {
      char split_l1221[2][MAX_LINE_SIZE];
      if (strsplit(split_l122[j], split_l1221, '#') != 2)
	return false;
      pneighbor->locations[j].local = strtouz(split_l1221[0]);
      pneighbor->locations[j].distant = strtouz(split_l1221[1]);
    }
  }
  return true;
}

orwl_graph * orwl_graph_read(orwl_graph * graph, char const* file) {
  FILE*volatile f = fopen(file, "r");
  if (f == NULL) {
    P99_HANDLE_ERRNO {
    P99_XDEFAULT : {
	perror("error when opening graph file");
	return NULL;
      }
    }
  }
  P99_UNWIND_PROTECT {
    char msg[6] = {0};
    if (!fgets(msg, 6, f)) {
      P99_HANDLE_ERRNO {
      P99_XDEFAULT : {
	  perror("error when reading the number of vertices in graph file");
	  P99_UNWIND_RETURN NULL;
	}
      }
    }
    size_t nb_vertices = str2uz(msg);
    graph = P99_NEW(orwl_graph, nb_vertices);
    for (size_t i = 0; i < nb_vertices; i++) {
      char msg[MAX_LINE_SIZE] = {0};
      if (!fgets(msg, MAX_LINE_SIZE, f)) {
	P99_HANDLE_ERRNO {
	P99_XDEFAULT : {
	    perror("error when reading the number of vertices in graph file");
	    orwl_graph_delete(graph);
	    P99_UNWIND_RETURN NULL;
	  }
	}
      }
      if (!orwl_graph_extract_line(graph, msg)) {
	printf("error while extracting a line in the graph file\n");
	orwl_graph_delete(graph);
	P99_UNWIND_RETURN NULL;
      }
    }
  P99_PROTECT:
    fclose(f);
  }
  return graph;
}

orwl_id * orwl_address_book_find_id(orwl_address_book *ab, size_t id) {
  if (ab != NULL)
    for (size_t i = 0 ; i < ab->nb_ids ; i++)
      if (ab->ids[i].num_id == id) return &ab->ids[i];
  return NULL;
}

orwl_id * orwl_address_book_insert_id(orwl_address_book *ab,
				      size_t id,
				      char const *address) {
  if ((ab != NULL) && orwl_address_book_find_id(ab, id) == NULL) {
    size_t i = 0;
    while (ab->ids[i].num_id != 0) i++; //find the next empty position   
    orwl_id_init(&ab->ids[i], id, address);
    return &ab->ids[i];
  }
  return NULL;
}

bool orwl_address_book_extract_line(orwl_address_book *ab, char const *str) {
    /*
    A line pattern is as follow:
    id-host
   */
  char split_l[2][MAX_LINE_SIZE] = {0};
  if (strsplit(str, split_l, '-') != 2)
    return false;
  size_t id = strtouz(split_l[0]);
  char * host = split_l[1];
  if (orwl_address_book_insert_id(ab, id, host) == NULL)
    return false;
  
  return true;
}

orwl_address_book* orwl_address_book_read(orwl_address_book *ab, char const *file) {
  FILE*volatile f = fopen(file, "r");
  if (f == NULL) {
    P99_HANDLE_ERRNO {
    P99_XDEFAULT : {
	perror("error when opening address book file");
	return NULL;
      }
    }
  }
  P99_UNWIND_PROTECT {
    char msg[6] = {0};
    if (!fgets(msg, 6, f)) {
      P99_HANDLE_ERRNO {
      P99_XDEFAULT : {
	  perror("error when reading the number of ids in address book file");
	  P99_UNWIND_RETURN NULL;
	}
      }
    }
    size_t nb_ids = str2uz(msg);
    ab = P99_NEW(orwl_address_book, nb_ids);
    for (size_t i = 0; i < nb_ids; i++) {
      char msg[MAX_LINE_SIZE] = {0};
      if (!fgets(msg, MAX_LINE_SIZE, f)) {
	P99_HANDLE_ERRNO {
	P99_XDEFAULT : {
	    perror("error when reading the number of ids in address book file");
	    orwl_address_book_delete(ab);
	    P99_UNWIND_RETURN NULL;
	  }
	}
      }
      if (!orwl_address_book_extract_line(ab, msg)) {
	printf("error when extracting a line in the address book file\n");
	orwl_address_book_delete(ab);
	P99_UNWIND_RETURN NULL;
      }
    }
  P99_PROTECT:
    fclose(f);
  }
  return ab;
}

bool orwl_dump_id(orwl_server *serv,
		  const char *filename,
		  size_t nb_id,
		  size_t *list_id) {
  char name[256] = {0};
  FILE*volatile out = fopen(filename, "w");
  if (out == NULL) {
    perror("error when opening file");
    return false;
  }
  P99_UNWIND_PROTECT {
    for (size_t i = 0 ; i < nb_id ; i++) {
      orwl_endpoint_print(&serv->host.ep, name);
      char str[256];
      snprintf(str, 256, "%zu-%s\n", list_id[i], name);
      if (fprintf(out, "%s", str) != strlen(str)) {
	perror("error when writing to file");
	P99_UNWIND_RETURN false;
      }
    }
  P99_PROTECT:
    fclose(out);
  }
  return true;
}

bool orwl_write_lockfile(const char *filename) {
  FILE * f = NULL;
  
  f = fopen(filename, "w");
  if (f == NULL) {
    perror("error when writing the lock file");
    return false;
  }
  fclose(f);
  return true;
}

void orwl_wait_until_file_is_here(const char *filename) {
  FILE * f = NULL;
  
  while ((f = fopen(filename, "r")) != NULL) {
    fclose(f);
    sleep(1);
  }
}

bool orwl_wait_and_load_init_files(orwl_address_book *ab,
				   const char *ab_filename,
				   orwl_graph * graph,
				   const char *graph_filename,
				   orwl_server *serv,
				   const char *id_filename,
				   size_t nb_id,
				   size_t *list_id,
				   const char *lock_filename) {
  if (!orwl_dump_id(serv, id_filename, nb_id, list_id))
    return false;
  if (!orwl_write_lockfile(lock_filename))
    return false;
  orwl_wait_until_file_is_here(lock_filename);
  ab = orwl_address_book_read(ab, ab_filename);
  if (ab == NULL)
    return false;
  graph = orwl_graph_read(graph, graph_filename);
  if (graph == NULL)
    return false;
  return true;
}

void orwl_make_connections(size_t id, 
			   orwl_server *server,
			   orwl_graph *graph, 
			   orwl_address_book *ab,
			   orwl_mirror* locations) {
  orwl_vertex * me = orwl_graph_find_vertex(graph, id);
  for (size_t i = 0 ; i < me->nb_neighbors ; i++) {
    orwl_id * neighbor_id = orwl_address_book_find_id(ab, me->neighbors[i].id);
    for (size_t j = 0 ; j < me->neighbors[i].nb_locations ; j++) {      
      orwl_endpoint there = neighbor_id->ep;
      there.index = me->neighbors[i].locations[j].distant;
      orwl_mirror_connect(&locations[me->neighbors[i].locations[j].local], server, there);
    }
  }
}


bool rpc_check_colored_init_finished(void) {
  /* TODO something here */

  return true;
}


size_t orwl_get_neighbors_in_undirected_graph(orwl_vertex **my_neighbors,
					      orwl_vertex * me,
					      size_t max_neighbors,
					      orwl_graph *graph) {
  size_t current = 0;
  /* first we add the neighbors where I request locks */
  for (size_t i = 0 ; i < me->nb_neighbors ; i++)
    my_neighbors[current++] = orwl_graph_find_vertex(graph, me->neighbors[i].id);
  /* then we add the neighbors that request my locks */
  for (size_t i = 0 ; i < graph->nb_vertices ; i++) {
    if (orwl_graph_find_neighbor(&graph->vertices[i], me->id) != NULL) {
      bool found = false;
      for (size_t j = 0 ; j < current ; j++) {
	if (&graph->vertices[i] == my_neighbors[j]) {
	  found = true;
	  break;
	}
      }
      if (!found) {
	my_neighbors[current++] = &graph->vertices[i];
      }
    }
  }
  return current;
}

void orwl_wait_to_initialize_locks(size_t id,
				   orwl_graph *graph,
				   orwl_server *server,
				   orwl_mirror *my_locations,
				   orwl_handle *h,
				   size_t nb_locations) {
  orwl_vertex * me = orwl_graph_find_vertex(graph, id);
  orwl_vertex * my_neighbors[graph->nb_vertices];
  for (size_t i = 0 ; i < graph->nb_vertices ; i++)
    my_neighbors[i] = NULL;

  size_t nb = orwl_get_neighbors_in_undirected_graph(my_neighbors,
						     me,
						     graph->nb_vertices,
						     graph);
  bool should_wait;
  do { 
    should_wait = false;
    for (size_t i = 0 ; i < nb ; i++) {
      if (my_neighbors[i]->color < me->color) {
	//is it already finished ? if yes, we can go
	if (!rpc_check_colored_init_finished()) {
	  should_wait = true;
	  sleepfor(0.5);
	  break;
	}
      }
    }
  } while (should_wait);

  for (size_t i = 0 ; i < nb_locations ; i++) {
    orwl_write_request(my_locations, &h[i]);
    orwl_acquire(&h[i]);
  }

  ORWL_CRITICAL {
    server->id_initialized = server->id_initialized | pow2(id);
  }
}

void orwl_wait_to_start(size_t id,
			orwl_graph *graph,
			orwl_server *server,
			orwl_handle *h,
			size_t nb_locations) {
  orwl_vertex * me = orwl_graph_find_vertex(graph, id);
  orwl_vertex * my_neighbors[graph->nb_vertices];
  for (size_t i = 0 ; i < graph->nb_vertices ; i++)
    my_neighbors[i] = NULL;
  
  size_t nb = orwl_get_neighbors_in_undirected_graph(my_neighbors,
						     me,
						     graph->nb_vertices,
						     graph);
  bool should_wait;
  do {
    should_wait = false;
    for (size_t i = 0 ; i < nb ; i++) {
      if (!rpc_check_colored_init_finished()) {
	should_wait = true;
	sleepfor(0.5);
	break;
      }
    }
  } while (should_wait);

  for (size_t i = 0 ; i < nb_locations ; i++)
    orwl_release(&h[i]);
}
