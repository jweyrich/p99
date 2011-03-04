/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl.h"
#include "p99_posix_default.h"
#include "p99_c99_default.h"

static char address[256] = "";
static char const* lockfilename = 0;
static uint64_t con = 20;
static uint64_t len = 10;
static bool background = false;
static bool block = false;
static int verbose = -1;

#define P99_ERROR_RETURN(INFO)                                 \
  P99_HANDLE_ERRNO {                                           \
  P99_XDEFAULT : {                                             \
      perror("error when reading from stdin");                 \
      P99_UNWIND_RETURN p99_errno;                             \
    }                                                          \
  }

static char const options[] = "a:c:l:L:hvbf";

static
void process_opt(int argc, char **argv) {

  for (int opt = 0; opt != -1; opt = getopt(argc, argv, options)) {
    switch (opt) {
    case 0: break;
    case 'a':
      strcpy(address, optarg);
      break;
    case 'c':
      con = strtou64(optarg);
      break;
    case 'l':
      len = strtou64(optarg);
      break;
    case 'L':
      lockfilename = strdup(optarg);
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
      fprintf(stderr, "Usage: %s [-bfv] [-a ORWL_ADDR] [-c MAXCONN] [-l LOCATIONS] [-L LOCKFILE]\n", argv[0]);
      fprintf(stderr, "    -b block initially\n");
      fprintf(stderr, "    -f fork into background\n");
      fprintf(stderr, "    -v be verbose (off with -f)\n");
      exit(EXIT_FAILURE);
      break;
    default:
      fprintf(stderr, "Usage: %s [-b] [-a ORWL_ADDR] [-c MAXCONN] [-l LOCATIONS] [-L LOCKFILE]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


int main(int argc, char **argv) {
  orwl_types_init();
  process_opt(argc, argv);
  if (verbose < 0) verbose = background ? 0 : 1;

  report(verbose, "ORWL server with maximal %" PRIu64 " connections, %" PRIu64 " locations", con, len);

  int fd[2] = { fileno(stdin), -1 };
  if (block && background) {
    if (pipe(fd)) {
      perror("Couldn't create pipe");
      exit(errno);
    }
  }

  pid_t pid = background ? fork() : 0;

  if (pid < 0) {
    perror("can't fork");
    exit(EXIT_FAILURE);
  }

  P99_UNWIND_PROTECT {
    if (!pid)
      P99_UNWIND_PROTECT {
        if (background) {
          int fdin = fileno(stdin);
          if (block) dup2(fdin, fd[0]);
          else close(fdin);
        }
        orwl_server srv;
        orwl_start(&srv, con, len, address);
        if (!orwl_alive(&srv)) P99_UNWIND_RETURN EXIT_FAILURE;
        char const* server_name = orwl_endpoint_print(&srv.host.ep);
        if (verbose) {
          size_t ilen = 3 * len + 1;
          if (ilen < 256) ilen = 256;
          char* info = calloc(ilen + 1);
          snprintf(info, ilen, "server at %s                                               ", server_name);
          srv.info = info;
          srv.info_len = ilen;
        }

        if (lockfilename) {
          char* tlf = P99_STRDUP(lockfilename, "XXXXXX");
          mode_t oldmask = umask(077);
          P99_UNWIND_PROTECT {
            int volatile lockfd = mkstemp(tlf);
            if (lockfd < 0)
              P99_ERROR_RETURN("could not open lockfile");
            size_t len = strlen(server_name);
            if (write(lockfd, server_name, len) != len) {
              free((void*)lockfilename);
              lockfilename = 0;
              P99_ERROR_RETURN("could not write to lockfile");
            }
            if (link(tlf, lockfilename))
              P99_ERROR_RETURN("could not link to lockfile, another server might be running");
          P99_PROTECT:
            umask(oldmask);
            if (lockfd >= 0) {
              unlink(tlf);
              close(lockfd);
            }
            if (tlf) free(tlf);
          }
        }
        if (block)
          P99_UNWIND_PROTECT {
            orwl_server_block(&srv);
            progress(1, 0, "%s waiting for kick off                                           ",
                     server_name);
            if (!fgets((char[32]){0}, 32, stdin))
              P99_ERROR_RETURN("error when reading from stdin");
          P99_PROTECT:
            orwl_server_unblock(&srv);
            if (background)
              fclose(stdin);
          }

        if (verbose) {
          size_t ilen = 3 * len + 1;
          srv.info[ilen] = '\0';
          char* info = memset(srv.info, ' ', ilen);
          for (size_t i = 0; i < ilen; i += 3)
            info[i] = '|';
          for (size_t t = 0; ; ++t) {
            if (!orwl_alive(&srv)) break;
            sleepfor(0.1);
            size_t have_data = 0;
            for (size_t i = 0; i < len; ++i) {
              if (srv.wqs[i].data) {
                ++have_data;
                uint8_t val = *(srv.wqs[i].data);
                char buf[3];
                snprintf(buf, 3, "%.2" PRIX8 "|", val);
                memcpy(info + (3 * i) + 1, buf, 2);
              }
            }
            if (!have_data)
              progress(1, t, "%s idle                                           ",
                       server_name);
          }
        }
      P99_PROTECT:
        orwl_stop(&srv);
      } else {
      if (block)
        P99_UNWIND_PROTECT {
          close(fd[0]);
          char mess[32] = {0};
          FILE*volatile out = fdopen(fd[1], "w");
          if (!fgets(mess, 32, stdin)) {
            kill(pid, SIGKILL);
            P99_ERROR_RETURN("error when reading from stdin");
          }
          report(verbose, "ORWL server is launched");
          fputs(mess, out);
        P99_PROTECT:
          fclose(out);
        }
    }
  P99_PROTECT:
    if (lockfilename) {
      report(verbose, "ORWL server trying to remove %s", lockfilename);
      unlink(lockfilename);
      free((void*)lockfilename);
    }
  }


  return 0;
}
