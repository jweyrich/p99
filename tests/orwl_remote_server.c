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
static bool background = false;
static bool block = false;
static int verbose = -1;

static char const options[] = "a:c:l:hvbf";

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
    case 'b':
      block = !block;
      break;
    case 'f':
      background = !background;
      break;
    case 'v':
      if (verbose < 0) verbose = 1;
      else ++verbose;
      break;
    case 'h':
      fprintf(stderr, "Usage: %s [-bfv] [-a ORWL_ADDR] [-c MAXCONN] [-l LOCATIONS]\n", argv[0]);
      fprintf(stderr, "    -b block initially\n");
      fprintf(stderr, "    -f fork into background\n");
      fprintf(stderr, "    -v be verbose (off with -f)\n");
      exit(EXIT_FAILURE);
      break;
    default:
      fprintf(stderr, "Usage: %s [-b] [-a ORWL_ADDR] [-c MAXCONN] [-l LOCATIONS]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


int main(int argc, char **argv) {
  int ret = 0;
  orwl_types_init();
  process_opt(argc, argv);
  if (verbose < 0) verbose = background ? 0 : 1;

  report(verbose, "ORWL server with maximal %" PRIu64 " connections, %" PRIu64 " locations", con, len);

  int fd[2] = { fileno(stdin), -1 };
  if (block && background) {
    pipe(fd);
  }

  pid_t pid = background ? fork() : 0;

  if (pid < 0) {
    perror("can't fork");
    exit(EXIT_FAILURE);
  }

  if (!pid) {
    if (background) {
      fclose(stdin);
      if (block)
        stdin = fdopen(fd[0], "r");
    }
    orwl_server* srv = NEW(orwl_server, con, len);
    if (address[0]) orwl_endpoint_parse(&srv->host.ep, address);
    pthread_t srv_id;
    orwl_server_create(srv, &srv_id);
    /* give the server the chance to fire things up */
    while (!port2net(&srv->host.ep.port)) sleepfor(0.01);
    char const* server_name = orwl_endpoint_print(&srv->host.ep);
    {
      char* info = calloc(256);
      snprintf(info, 256, "server at %s                                               ", server_name);
      if (verbose) {
        srv->info = info;
        srv->info_len = 256;
      }
    }

    if (block) {
      orwl_server_block(srv);
      progress(1, 0, "%s waiting for kick off                                           ",
               server_name);
      fgets((char[32]){0}, 32, stdin);
      orwl_server_unblock(srv);
      if (background)
        fclose(stdin);
    }

    if (verbose)
      for (size_t t = 0; ; ++t) {
        ret = pthread_kill(srv_id, 0);
        if (ret) break;
        sleepfor(1.0);
        progress(1, t, "%s idle                                           ",
                 server_name);
      }
    orwl_server_join(srv_id);
    orwl_server_delete(srv);
  } else {
    if (block) {
      close(fd[0]);
      char mess[32] = {0};
      FILE* out = fdopen(fd[1], "w");
      fgets(mess, 32, stdin);
      fputs(mess, out);
      fclose(out);
    }
  }


  return 0;
}