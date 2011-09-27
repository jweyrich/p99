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

int main(int argc, char **argv) {
  report(1, "starting");
  int ret = 0;

  orwl_types_init();
  orwl_start(10, 4);
  if (!orwl_alive()) return EXIT_FAILURE;
  report(1, "started %s", orwl_endpoint_print(&orwl_server_get()->host.ep));

  if (argc > 1) {
    report(1, "connecting to %s", argv[1]);
    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(0, 0);
    orwl_endpoint_parse(&other, argv[1]);

    /* wait until the other side is up. */
    /* ep.port is already in host order */
    while (orwl_rpc(0, &other, seed_get(), orwl_proc_insert_peer, port2host(&orwl_server_get()->host.ep.port))
           == P99_TMAX(uint64_t)) {
      if (!orwl_alive()) break;
      sleepfor(0.2);
    }
  } else {
    for (size_t t = 0; t < 1000; ++t) {
      if (!orwl_alive()) break;
      sleepfor(1.0);
      report(1, "looping %zd", t);
      orwl_host *n = 0;
      MUTUAL_EXCLUDE(orwl_server_get()->host.mut)
      n = orwl_server_get()->host.next;
      report(1, "%p -- %p", (void*)&orwl_server_get()->host, (void*)n);
      for (orwl_host *h = n; h != &orwl_server_get()->host; ) {
        orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(0, 0);
        MUTUAL_EXCLUDE(h->mut) {
          other.addr = h->ep.addr;
          other.port = h->ep.port;
          h = h->next;
        }
        report(1, "sending to %s ", orwl_endpoint_print(&other));
        /* ep.addr and ep.port are already in host order */
        orwl_rpc(0, &other, seed_get(), orwl_proc_do_nothing,
                 orwl_server_get()->host.ep.addr.a[3], orwl_server_get()->host.ep.port.p, t);
      }
    }
  }

  if (ret) {
    char mesg[256] = "";
    strerror_r(ret, mesg, 256);
    report(1, "Server already terminated: %s", mesg);
    ret = EXIT_FAILURE;
  } else {
    ret = EXIT_SUCCESS;
  }
  orwl_pthread_wait_detached();
  report(1, "host %p and next %p", (void*)orwl_server_get()->host.next, (void*)&orwl_server_get()->host);
  orwl_stop();
  seed_get_clear();
  return ret;
}
