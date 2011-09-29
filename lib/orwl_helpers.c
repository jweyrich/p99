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
#include "orwl_proc_symbols.h"
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


char * remove_eol(char *str) {
  size_t len = strlen(str) - 1;
  if (str[len] == '\n')
    str[len] = '\0';
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

  regmatch_t connection_match[3]= { P99_RVAL(regmatch_t) };
  regmatch_t attributes_match[4]= { P99_RVAL(regmatch_t) };

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

bool orwl_graph_read(orwl_graph ** graph, char const* file, size_t nb_vertices) {
  FILE*volatile f = fopen(file, "r");
  if (f == NULL) {
    P99_HANDLE_ERRNO {
P99_XDEFAULT : {
        perror("error when opening graph file");
        return false;
      }
    }
  }
  *graph = P99_NEW(orwl_graph, nb_vertices);
  char msg[LINE_MAX] = {0};
  size_t nb_neighbors[nb_vertices];
  for (size_t i = 0 ; i < nb_vertices ; i++)
    nb_neighbors[i] = 0;

  const char *connection = "^[[:blank:]]*([[:digit:]]+)[[:blank:]]*->[[:blank:]]*([[:digit:]]+)[[:blank:]]*$";
  const char *attributes = "^[[:blank:]]*([[:digit:]]+)[[:blank:]]*\\[color=\"([[:digit:]]+)\",[[:blank:]]*label=\"([[:digit:]]+-[[:alnum:]]+)\"\\]$";
  regex_t re_connection, re_attributes;

  if (regcomp(&re_connection, connection, REG_EXTENDED)) {
    printf("Connection pattern did not compile.\n");
    return false;
  }
  if (regcomp(&re_attributes, attributes, REG_EXTENDED)) {
    printf("Attributes pattern did not compile.\n");
    return false;
  }
  P99_UNWIND_PROTECT {
    for (size_t pass = 0 ; pass < 2 ; pass++) {
      if (fseek(f, 0, SEEK_SET) != 0) {
        P99_HANDLE_ERRNO {
P99_XDEFAULT : {
            perror("error when seeking the begin of the file");
            orwl_graph_delete(*graph);
            P99_UNWIND_RETURN false;
          }
        }
      }
      while (fgets(msg, LINE_MAX, f) != NULL) {
        if (!orwl_graph_extract_line(*graph, remove_eol(msg), pass, nb_neighbors, &re_connection, &re_attributes)) {
          printf("error while extracting a line in the graph file\n");
          orwl_graph_delete(*graph);
          P99_UNWIND_RETURN false;
        }
      }
    }
P99_PROTECT:
    fclose(f);
    regfree(&re_connection);
    regfree(&re_attributes);
  }
  return true;
}

void orwl_address_book_insert_id(orwl_address_book *ab,
                                 size_t id,
                                 size_t location,
                                 char const *address) {
  if (ab != NULL) {
    orwl_endpoint_parse(&ab->eps[id], address);
    ab->locations[id] = location;
  }
}

bool orwl_address_book_extract_line(orwl_address_book *ab, char const *str, regex_t *re) {
  /*
  A line pattern is as follow:
  id-location-host
     */
  regmatch_t match[4]= { P99_RVAL(regmatch_t) };

  /* matching a connection line */
  if (regexec(re, str, 4, match, 0) == 0) {
    size_t id = extract_size_t_from_str(match[1].rm_so,
                                        match[1].rm_eo,
                                        str);
    size_t location = extract_size_t_from_str(match[2].rm_so,
                      match[2].rm_eo,
                      str);
    char host[256] = {0};
    extract_str_from_str(match[3].rm_so,
                         match[3].rm_eo,
                         str,
                         &host[0]);
    orwl_address_book_insert_id(ab, id, location, host);
  }
  return true;
}

bool orwl_address_book_read(orwl_address_book **ab, char const *file, size_t nb_vertices) {
  FILE*volatile f = fopen(file, "r");
  if (f == NULL) {
    P99_HANDLE_ERRNO {
P99_XDEFAULT : {
        perror("error when opening address book file");
        return false;
      }
    }
  }
  P99_UNWIND_PROTECT {
    *ab = P99_NEW(orwl_address_book, nb_vertices);
    const char *pattern = "^([[:digit:]]+)-([[:digit:]]+)-(.+)$";
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
            orwl_address_book_delete(*ab);
            P99_UNWIND_RETURN false;
          }
        }
      }
      if (!orwl_address_book_extract_line(*ab, remove_eol(msg), &re)) {
        printf("error when extracting a line in the address book file\n");
        orwl_address_book_delete(*ab);
        P99_UNWIND_RETURN false;
      }
    }
P99_PROTECT:
    regfree(&re);
    fclose(f);
  }
  return true;
}

bool orwl_dump_id(orwl_server *serv,
                  const char *filename,
                  size_t nb_id,
                  size_t *list_id,
                  size_t *list_locations) {
  size_t maxlen = 255;
  char * str = malloc(maxlen + 1);
  char name[256] = {0};
  FILE*volatile out = fopen(filename, "w");
  if (out == NULL) {
    perror("error when opening file");
    return false;
  }
  P99_UNWIND_PROTECT {
    for (size_t i = 0 ; i < nb_id ; i++) {
      orwl_endpoint ep = serv->host.ep;
      /* trick to make orwl_endpoint_print() work */
      ep.addr =  P99_LVAL(orwl_addr, .a = { [0] = 0, [1] = 0, [2] = htonl(0x0000FFFF), [3] = 0 } );
      orwl_endpoint_print(&ep, name);
      size_t len;
RETRY:
      len = snprintf(str, maxlen, "%zu-%zu-%s\n", list_id[i], list_locations[i], name);
      if (len > maxlen) {
        free(str);
        maxlen = len;
        str = malloc(maxlen + 1);
        goto RETRY;
      }
      if (fputs(str, out) == EOF) {
        perror("error when writing to file");
        P99_UNWIND_RETURN false;
      }
    }
P99_PROTECT:
    fclose(out);
    free(str);
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

bool orwl_wait_and_load_init_files(char const *ab_filename,
                                   char const *graph_filename,
                                   char const *id_filename,
                                   size_t nb_id,
                                   size_t *list_id,
                                   size_t *list_locations,
                                   size_t nb_vertices,
                                   orwl_server *serv) {
  serv->id_initialized = orwl_notifier_vrealloc(serv->id_initialized, nb_vertices);
  serv->global_barrier = orwl_notifier_vrealloc(serv->global_barrier, nb_vertices);
  if (!orwl_dump_id(serv, id_filename, nb_id, list_id, list_locations))
    return false;
  orwl_wait_until_file_is_here(id_filename);
  if (!orwl_address_book_read(&serv->ab, ab_filename, nb_vertices))
    return false;
  if (!orwl_graph_read(&serv->graph, graph_filename, nb_vertices))
    return false;
  return true;
}

void orwl_make_distant_connection(size_t dest_id,                                  
                                  orwl_mirror *location,
				  orwl_server *server) {
  orwl_endpoint there = server->ab->eps[dest_id];
  orwl_mirror_connect(location, there, server->ab->locations[dest_id], server);
  report(0, "connected to %s", orwl_endpoint_print(&there));
}

void orwl_make_local_connection(size_t dest_id,
                                orwl_mirror *location,
                                orwl_server *server) {
  orwl_endpoint there = server->host.ep;
  orwl_mirror_connect(location, there, dest_id, server);
  report(0, "connected to %s", orwl_endpoint_print(&there));
}

static
void o_rwl_rpc_check_colored_init_finished(size_t id,
                                     orwl_graph *graph,
                                     orwl_address_book *ab,
                                     rand48_t *seed) {
  orwl_endpoint there = ab->eps[id];
  /* warning, this is a blocking operation */
  orwl_rpc(0, &there, seed, orwl_proc_check_initialization, id);
}

size_t orwl_get_neighbors_in_undirected_graph(orwl_vertex **my_neighbors,
					      size_t id,
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
				   orwl_server* serv,
                                   rand48_t *seed) {
  orwl_vertex * me = &serv->graph->vertices[id];
  orwl_vertex * my_neighbors[serv->graph->nb_vertices]; /* upper limit */
  for (size_t i = 0 ; i < serv->graph->nb_vertices ; i++)
    my_neighbors[i] = NULL;

  size_t nb = orwl_get_neighbors_in_undirected_graph(my_neighbors,
						     id,
						     serv->graph);
  for (size_t i = 0 ; i < nb ; i++) {
    assert(my_neighbors[i]->color != me->color);
    /* we ensure that the neighbors with a lower color are initialized */
    if (my_neighbors[i]->color < me->color) {
      size_t vertex_id = orwl_get_id_from_vertex(serv->graph, my_neighbors[i]);
      if (vertex_id == SIZE_MAX) {
        printf("error when getting the id number of a vertex");
        return false;
      }
      o_rwl_rpc_check_colored_init_finished(vertex_id, serv->graph, serv->ab, seed);
    }
  }

  return true;
}

static
void o_rwl_rpc_check_barrier(size_t id,
		       orwl_address_book *ab,
		       rand48_t *seed) {
  orwl_endpoint there = ab->eps[id];
  orwl_rpc(0, &there, seed, orwl_proc_barrier, id);
}

void orwl_global_barrier_init(size_t id, orwl_server *server) {
  orwl_notifier_unset(&server->global_barrier[id]);
}

int orwl_global_barrier_wait(size_t id,
                             size_t nb_tasks,
                             orwl_server *server,
                             rand48_t *seed) {
  orwl_notifier_set(&server->global_barrier[id]);
  for (size_t task = 0 ; task < nb_tasks ; task++)
    if (task != id)
      o_rwl_rpc_check_barrier(task, server->ab, seed);
  return id ? 0 : PTHREAD_BARRIER_SERIAL_THREAD;
}

bool orwl_wait_to_start(size_t id,
                        size_t nb_local_tasks,
                        orwl_server *server,
                        rand48_t *seed) {
  orwl_notifier_set(&server->id_initialized[id]);
  orwl_vertex * me = &server->graph->vertices[id];
  orwl_vertex * my_neighbors[server->graph->nb_vertices]; /* upper limit */
  size_t nb = orwl_get_neighbors_in_undirected_graph(my_neighbors,
						     id,
						     server->graph);

  for (size_t i = 0 ; i < nb ; i++) {
    assert(my_neighbors[i]->color != me->color);
    if (my_neighbors[i]->color > me->color) {
      size_t vertex_id = orwl_get_id_from_vertex(server->graph, my_neighbors[i]);
      if (vertex_id == SIZE_MAX) {
        printf("error when getting the id number of a vertex");
        return false;
      }
      o_rwl_rpc_check_colored_init_finished(vertex_id, server->graph, server->ab, seed);
    }
  }

  orwl_server_delayed_unblock(server, nb_local_tasks);
  return true;
}

void print_statistics(size_t id) {
  printf("print_statistics does nothing\n");
}
