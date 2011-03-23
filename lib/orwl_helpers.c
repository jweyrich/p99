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
#include "orwl_auth_sock_functions.h"
#include "limits.h"

DEFINE_NEW_DELETE(orwl_vertex);
DEFINE_NEW_DELETE(orwl_graph);
DEFINE_NEW_DELETE(orwl_address_book);

P99_DEFINE_DEFARG(orwl_vertex_init, , P99_0(size_t));
P99_DEFINE_DEFARG(orwl_graph_init, , P99_0(size_t));
P99_DEFINE_DEFARG(orwl_address_book_init, , P99_0(size_t));

P99_INSTANTIATE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t);
P99_INSTANTIATE(void, orwl_vertex_destroy, orwl_vertex*);
P99_INSTANTIATE(orwl_graph*, orwl_graph_init, orwl_graph *, size_t);
P99_INSTANTIATE(void, orwl_graph_destroy, orwl_graph *);
P99_INSTANTIATE(orwl_address_book*, orwl_address_book_init, orwl_address_book *, size_t);
P99_INSTANTIATE(void, orwl_address_book_destroy, orwl_address_book *);

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

char * remove_eol(char *str) {
  if (str[strlen(str) - 1] == '\n')
    str[strlen(str) - 1] = '\0';
  return str;
}

bool orwl_graph_find_neighbor(orwl_vertex *vertex, size_t id) {
  if (vertex != NULL)
    for (size_t i = 0 ; i < vertex->nb_neighbors ; i++)
      if (vertex->neighbors[i] == id) return true;
  return false;
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

bool orwl_graph_extract_line(orwl_graph *graph,
			     char const *str, int pass,
			     size_t *nb_neighbors,
			     regex_t *re_connection,
			     regex_t *re_attributes) {

  regmatch_t connection_match[3]= {0};
  regmatch_t attributes_match[4]= {0};

  /* matching a connection line */
  if (regexec(re_connection, str, 3, connection_match, 0) == 0) {
    size_t src_vertex = extract_size_t_from_str(connection_match[1].rm_so,
						connection_match[1].rm_eo,
						str);
    size_t dst_vertex = extract_size_t_from_str(connection_match[2].rm_so,
						connection_match[2].rm_eo,
						str);
    if (pass == 0) {
      nb_neighbors[src_vertex]++;
    } else {
      if (graph->vertices[src_vertex].neighbors == NULL) {
	size_t nb = nb_neighbors[src_vertex];
	orwl_vertex_init(&graph->vertices[src_vertex], nb);
	for (size_t i = 0 ; i < nb ; i++)
	  graph->vertices[src_vertex].neighbors[i] = SIZE_MAX;
      }
      size_t i = 0;
      while (graph->vertices[src_vertex].neighbors[i] != SIZE_MAX) i++;
      graph->vertices[src_vertex].neighbors[i] = dst_vertex;
    }
  }
  /* matching an attribute line (only at the second pass) */
  if ((pass == 1) && (regexec(re_attributes, str, 4, attributes_match, 0) == 0)) {
    size_t vertex = extract_size_t_from_str(attributes_match[1].rm_so,
					    attributes_match[1].rm_eo,
					    str);
    
    size_t color = extract_size_t_from_str(attributes_match[2].rm_so,
					   attributes_match[2].rm_eo,
					   str);
    
    char label[16] = {0};
    extract_str_from_str(attributes_match[3].rm_so,
			 attributes_match[3].rm_eo,
			 str,
			 &label[0]);
    graph->vertices[vertex].color = color;
    strncpy(graph->vertices[vertex].label, label, 16);
  }
  return true;
}

orwl_graph * orwl_graph_read(orwl_graph * graph, char const* file, size_t nb_vertices) {
  FILE*volatile f = fopen(file, "r");
  if (f == NULL) {
    P99_HANDLE_ERRNO {
    P99_XDEFAULT : {
	perror("error when opening graph file");
	return NULL;
      }
    }
  }
  graph = P99_NEW(orwl_graph, nb_vertices);
  char msg[LINE_MAX] = {0};
  size_t nb_neighbors[nb_vertices];
  for (size_t i = 0 ; i < nb_vertices ; i++)
    nb_neighbors[i] = 0;

  const char *connection = "^[[:blank:]]*([[:digit:]]+)[[:blank:]]*->[[:blank:]]*([[:digit:]]+)[[:blank:]]*$";
  const char *attributes = "^[[:blank:]]*([[:digit:]]+) \\[color=\"([[:digit:]]+)\",[[:blank:]]*label=\"([[:alnum:]]+)\"\\]$";
  regex_t re_connection, re_attributes;

  if (regcomp(&re_connection, connection, REG_EXTENDED)) {
    printf("Connection pattern did not compile.\n");
    return NULL;
  }
  if (regcomp(&re_attributes, attributes, REG_EXTENDED)) {
    printf("Attributes pattern did not compile.\n");
    return NULL;
  }
  P99_UNWIND_PROTECT {
    for (size_t pass = 0 ; pass < 2 ; pass++) {
      if (fseek(f, 0, SEEK_SET) != 0) {
	P99_HANDLE_ERRNO {
	P99_XDEFAULT : {
	    perror("error when seeking the begin of the file");
	    orwl_graph_delete(graph);
	    P99_UNWIND_RETURN NULL;
	  }
	}
      }
      while (fgets(msg, LINE_MAX, f) != NULL) {
	if (!orwl_graph_extract_line(graph, remove_eol(msg), pass, nb_neighbors, &re_connection, &re_attributes)) {
	  printf("error while extracting a line in the graph file\n");
	  orwl_graph_delete(graph);
	  P99_UNWIND_RETURN NULL;
	}
      }
    }
  P99_PROTECT:
    fclose(f);
    regfree(&re_connection);
    regfree(&re_attributes);
  }
  return graph;
}

void orwl_address_book_insert_id(orwl_address_book *ab,
				 size_t id,
				 char const *address) {
  if (ab != NULL)
    orwl_endpoint_parse(&ab->eps[id], address);
}

bool orwl_address_book_extract_line(orwl_address_book *ab, char const *str, regex_t *re) {
    /*
    A line pattern is as follow:
    id-host
   */
  regmatch_t match[3]= {0};

  /* matching a connection line */
  if (regexec(re, str, 3, match, 0) == 0) {
    size_t id = extract_size_t_from_str(match[1].rm_so,
					match[1].rm_eo,
					str);    
    char host[16] = {0};
    extract_str_from_str(match[2].rm_so,
			 match[2].rm_eo,
			 str,
			 &host[0]);
    orwl_address_book_insert_id(ab, id, host);
  }
  return true;
}

orwl_address_book* orwl_address_book_read(orwl_address_book *ab, char const *file, size_t nb_vertices) {
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
    ab = P99_NEW(orwl_address_book, nb_vertices);
    const char *pattern = "^([[:digit:]]+)-(.+)$";
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED)) {
      printf("Pattern did not compile.\n");
      return false;
    } 
    for (size_t i = 0; i < nb_vertices; i++) {
      char msg[LINE_MAX] = {0};
      if (!fgets(msg, LINE_MAX, f)) {
	P99_HANDLE_ERRNO {
	P99_XDEFAULT : {
	    perror("error when reading a line in the address book file");
	    orwl_address_book_delete(ab);
	    P99_UNWIND_RETURN NULL;
	  }
	}
      }
      if (!orwl_address_book_extract_line(ab, remove_eol(msg), &re)) {
	printf("error when extracting a line in the address book file\n");
	orwl_address_book_delete(ab);
	P99_UNWIND_RETURN NULL;
      }
    }
  P99_PROTECT:
    regfree(&re);
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

void orwl_wait_until_file_is_here(const char *filename) {
  FILE * f = NULL;
  
  while ((f = fopen(filename, "r")) != NULL) {
    fclose(f);
    sleep(1);
  }
}

bool orwl_wait_and_load_init_files(orwl_address_book **ab,
				   const char *ab_filename,
				   orwl_graph **graph,
				   const char *graph_filename,
				   orwl_server *serv,
				   const char *id_filename,
				   size_t nb_id,
				   size_t *list_id,
				   size_t nb_vertices) {
  if (!orwl_dump_id(serv, id_filename, nb_id, list_id))
    return false;
  orwl_wait_until_file_is_here(id_filename);
  *ab = orwl_address_book_read(*ab, ab_filename, nb_vertices);
  if (*ab == NULL)
    return false;
  *graph = orwl_graph_read(*graph, graph_filename, nb_vertices);
  if (*graph == NULL)
    return false;
  return true;
}

void orwl_make_connection(size_t dest_id,
			  orwl_server *server,
			  orwl_graph *graph, 
			  orwl_address_book *ab,
			  orwl_mirror *location,
			  size_t dest_location_pos) {
  orwl_endpoint there = ab->eps[dest_id];
  there.index = dest_location_pos;
  orwl_mirror_connect(location, server, there);
}


bool rpc_check_colored_init_finished(size_t id,
				     orwl_graph *graph,
				     orwl_address_book *ab,
				     rand48_t *seed) {
  orwl_endpoint there = ab->eps[id];
  /* warning, this is a blocking operation */
  return (orwl_rpc(&there, seed, auth_sock_check_initialization, pow2(id)) == 1);
}

size_t orwl_get_neighbors_in_undirected_graph(orwl_vertex **my_neighbors,
					      size_t id,
					      size_t max_neighbors,
					      orwl_graph *graph) {
  size_t current = 0;
  orwl_vertex * me = &graph->vertices[id];
  /* first we add the neighbors where I request locks */
  for (size_t i = 0 ; i < me->nb_neighbors ; i++)
    my_neighbors[current++] = &graph->vertices[me->neighbors[i]];
  /* then we add the neighbors that request my locks */
  for (size_t i = 0 ; i < graph->nb_vertices ; i++) {
    if (i != id) 
      if (orwl_graph_find_neighbor(&graph->vertices[i], id)) {
	bool found = false;
	for (size_t j = 0 ; j < current ; j++) {
	  if (&graph->vertices[i] == my_neighbors[j]) {
	    found = true;
	    break;
	  }
	}
	if (!found)
	  my_neighbors[current++] = &graph->vertices[i];
      }
  }
  return current;
}

size_t orwl_get_id_from_vertex(orwl_graph *graph, orwl_vertex *vertex) {
  for (size_t i = 0 ; i < graph->nb_vertices ; i++)
    if (&graph->vertices[i] == vertex)
      return i;
  return SIZE_MAX;
}

bool orwl_wait_to_initialize_locks(size_t id,
				   orwl_graph *graph,
				   orwl_address_book *ab,
				   orwl_server *server,
				   orwl_mirror *my_locations,
				   orwl_handle *h,
				   size_t nb_locations,
				   rand48_t *seed) {
  orwl_vertex * me = &graph->vertices[id];
  orwl_vertex * my_neighbors[graph->nb_vertices]; /* upper limit */
  for (size_t i = 0 ; i < graph->nb_vertices ; i++)
    my_neighbors[i] = NULL;

  size_t nb = orwl_get_neighbors_in_undirected_graph(my_neighbors,
  						     id,
  						     graph->nb_vertices,
  						     graph);

  for (size_t i = 0 ; i < nb ; i++) {
    /* we ensure that the neighbors with a lower color are initialized */
    if (my_neighbors[i]->color < me->color) {
      size_t vertex_id = orwl_get_id_from_vertex(graph, my_neighbors[i]);
      if (vertex_id == SIZE_MAX) {
	printf("error when getting the id number of a vertex");
	return false;
      }
      rpc_check_colored_init_finished(vertex_id, graph, ab, seed);
    }
  }
  
  /* locking all the locations */
  for (size_t i = 0 ; i < nb_locations ; i++) {
    orwl_write_request(my_locations, &h[i]);
    orwl_acquire(&h[i]);
  }
  return true;
}

bool orwl_wait_to_start(size_t id,
			orwl_graph *graph,
			orwl_address_book *ab,
			orwl_server *server,
			orwl_handle *h,
			size_t nb_locations,
			rand48_t *seed) {

  ORWL_CRITICAL {
    server->id_initialized = server->id_initialized | pow2(id);
  }

  orwl_vertex * my_neighbors[graph->nb_vertices];
  for (size_t i = 0 ; i < graph->nb_vertices ; i++)
    my_neighbors[i] = NULL;
  
  size_t nb = orwl_get_neighbors_in_undirected_graph(my_neighbors,
  						     id,
  						     graph->nb_vertices,
  						     graph);
  /* we ensure that all the neighbors are initialized */
  for (size_t i = 0 ; i < nb ; i++) {
    size_t vertex_id = orwl_get_id_from_vertex(graph, my_neighbors[i]);
      if (vertex_id == SIZE_MAX) {
	printf("error when getting the id number of a vertex");
	return false;
      }
      rpc_check_colored_init_finished(vertex_id, graph, ab, seed);
  }

  /* unlocking the locations */
  for (size_t i = 0 ; i < nb_locations ; i++)
    orwl_release(&h[i]);
  
  return true;
}
