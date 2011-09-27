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
  /* This init here is necessary since we don't even start a server thread */
  orwl_types_init();
  if (argc > 1) {
    report(1, "connecting to %s", argv[1]);
    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(0, 0);
    orwl_endpoint_parse(&other, argv[1]);
    errno = 0;
    /* wait until the other side is up. */
    uint64_t ret = orwl_send(0, &other, seed_get(), 1, &P99_LVAL(orwl_buffer));
    char messg[245];
    sprintf(messg, "finish server %s %" PRIX64, argv[1], ret);
    perror(messg);
    errno = 0;
    return ret ? EXIT_FAILURE : EXIT_SUCCESS;
  } else {
    report(1, "Usage: %s SERVER_URL", argv[0]);
    return EXIT_FAILURE;
  }
}
