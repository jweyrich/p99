#include "orwl_helpers.h"
#include "p99_c99_default.h"
#include "p99_str.h"
#include "orwl_auth_sock_functions.h"
#include "limits.h"

DEFINE_NEW_DELETE(orwl_vertex);
DEFINE_NEW_DELETE(orwl_graph);
DEFINE_NEW_DELETE(orwl_id);
DEFINE_NEW_DELETE(orwl_address_book);

P99_DEFINE_DEFARG(orwl_vertex_init, , P99_0(size_t));
P99_DEFINE_DEFARG(orwl_graph_init, , P99_0(size_t));
P99_DEFINE_DEFARG(orwl_id_init, , P99_0(size_t), P99_0(char const*));
P99_DEFINE_DEFARG(orwl_address_book_init, , P99_0(size_t));

P99_INSTANTIATE(orwl_vertex*, orwl_vertex_init, orwl_vertex*, size_t);
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

unsigned strsplit(char const *str, char str_array[][LINE_MAX], char delim) {
  unsigned count = 0;
  char *token;
  char *tmp = strdup(str);
  char *saveptr;
  if (strcountchr(tmp, delim) > 0) {
    token = strtok_r(tmp, &delim, &saveptr);
    while (token != NULL) {
      strncpy(str_array[count], token, LINE_MAX);
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
      if (src_vertex == 1) {
	printf("reading 1 - %zu:%zu - %s\n",src_vertex, dst_vertex, str);
      }
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

  const char *connection = "^[[:blank:]]*([[:digit:]]+)[[:blank:]]*->[[:blank:]]*([[:digit:]]+)[[:blank:]]*";
  const char *attributes = "^[[:blank:]]*([[:digit:]]+) \\[color=\"([[:digit:]]+)\",[[:blank:]]*label=\"([[:alnum:]]+)\"\\]";
  regex_t re_connection, re_attributes;

  if (regcomp(&re_connection, connection, REG_EXTENDED)) {
    printf("Connection pattern did not compile.\n");
    return NULL;
  }
  if (regcomp(&re_attributes, attributes, REG_EXTENDED)) {
    printf("Attributes pattern did not compile.\n");
    return NULL;
  }

  for (size_t pass = 0 ; pass < 2 ; pass++) {
    if (pass == 1) {
      for (size_t i = 0 ; i < graph->nb_vertices ; i++) {
	printf("VOISINS %zu -> %zu\n", i,  nb_neighbors[i]);
      }
    }
    fseek(f, 0, SEEK_SET);
    P99_HANDLE_ERRNO {
    P99_XDEFAULT : {
	perror("error when seeking the begin of the file");
	fclose(f);
	return NULL;
      }
    }
    while (fgets(msg, LINE_MAX, f) != NULL) {
      P99_HANDLE_ERRNO {
      P99_XDEFAULT : {
	  perror("error when reading a line in graph file");
	  orwl_graph_delete(graph);
	  fclose(f);
	  regfree(&re_connection);
	  regfree(&re_attributes);
	  return NULL;
	}
      }
      if (!orwl_graph_extract_line(graph, msg, pass, nb_neighbors, &re_connection, &re_attributes)) {
	printf("error while extracting a line in the graph file\n");
	orwl_graph_delete(graph);
	fclose(f);
	regfree(&re_connection);
	regfree(&re_attributes);
	return NULL;
      }
    }
  }

  regfree(&re_connection);
  regfree(&re_attributes);
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
  char split_l[2][LINE_MAX] = {0};
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
      char msg[LINE_MAX] = {0};
      if (!fgets(msg, LINE_MAX, f)) {
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
  *ab = orwl_address_book_read(*ab, ab_filename);
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
  orwl_id * dest = orwl_address_book_find_id(ab, dest_id);
  orwl_endpoint there = dest->ep;
  there.index = dest_location_pos;
  orwl_mirror_connect(location, server, there);
}


bool rpc_check_colored_init_finished(size_t id,
				     orwl_graph *graph,
				     orwl_address_book *ab,
				     rand48_t *seed) {
  orwl_id * dest_id = orwl_address_book_find_id(ab, id);
  orwl_endpoint there = dest_id->ep;
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
