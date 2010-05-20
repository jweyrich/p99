/*
** orwl_wait_list.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Feb 20 10:17:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <signal.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "orwl_thread.h"
#include "orwl_rand.h"
#include "orwl_remote_queue.h"
#include "orwl_callback.h"
#include "orwl_server.h"


static char address[256] = INITIALIZER;
static uint64_t con = 20;
static uint64_t len = 10;

static char const options[] = "a:c:l:h";

static
void process_opt(int argc, char **argv) {

  for (int opt = 0; opt != -1; opt = getopt(argc, argv, options)) {
    switch (opt) {
    case 0: break;
    case 'a':
      strcpy(address, optarg);
      break;
    case 'c':
      con = str2uint64_t(optarg);
      break;
    case 'l':
      len = str2uint64_t(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-a ADDR] [-c MAXCONN] [-l LOCATIONS]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


int main(int argc, char **argv) {
  int ret = 0;
  orwl_types_init();
  process_opt(argc, argv);
  report(1, "ORWL server with maximal %" PRIu64 " connections, %" PRIu64 " locations", con, len);
  orwl_server* srv = NEW(orwl_server, con, len);
  if (address[0]) orwl_endpoint_parse(&srv->host.ep, address);
  pthread_t srv_id;
  orwl_server_create(srv, &srv_id);
  /* give the server the chance to fire things up */
  while (!port2net(&srv->host.ep.port)) sleepfor(0.01);
  {
    char const* server_name = orwl_endpoint_print(&srv->host.ep);
    char* info = calloc(256);
    snprintf(info, 256, "server at %s                                               ", server_name);
    srv->info = info;
    srv->info_len = 256;
  }

  for (size_t t = 0; ; ++t) {
    ret = pthread_kill(srv_id, 0);
    if (ret) break;
    sleepfor(1.0);
    progress(1, t, " server idle                                           ");
  }
  orwl_server_join(srv_id);
  orwl_server_delete(srv);

  return 0;
}
