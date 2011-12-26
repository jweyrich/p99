/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
/*
** test-case.c
**
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
**
** Started on  Sun Sep 26 16:04:57 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <setjmp.h>
#include "p99_block.h"
#include "p99_swap.h"

double ad = 1.0, bd = 2.0;
uint64_t a4 = 3, b4 = -1;
char ac = 'a', bc = 'b';
struct big { uintmax_t B[1024]; } ab = { .B = { [0] = 7 }}, bb = { .B = { [0] = 1 }};
struct mimic { char B[sizeof(double)]; } am = { .B = { [0] = 7 }}, bm = { .B = { [0] = 1 }};
void const *av = &av, *bv = &bv;



int main(int argc, char *argv[]) {

  errno = 42;

  P99_HANDLE_ERRNO {
    fprintf(stderr, "The compiler should tell us that this fprintf here is unreachable.\n");
P99_XCASE EINTR : {
      fprintf(stderr, "Autsch: call to schnoeck was interrupted!\n");
      // do something else in that case
    }
P99_XCASE ENOMEM :
    fprintf(stderr, "Autsch: call to schnoeck didn't have enough memory!\n");
P99_XDEFAULT : {
      fprintf(stderr, "AUTSCH: call to schnoeck failed with unhandled case!\n");
      perror("AUTSCH");
      /* This is a return that should be triggered as dangerous when
         this code is compiled with P99_CHECK_RETURN. */
      return EXIT_FAILURE;
    }
  }

  fprintf(stderr, "now errno is set to %d\n", errno);

  P99_UNWIND_PROTECT {
    char *volatile a = malloc(32);
    P99_UNWIND_PROTECT {
      char *volatile b = malloc(32);
      if (argc > 2) P99_UNWIND(argv[1][0] - 'a');
      else if (argc > 1) P99_UNWIND_RETURN argv[1][0];
      printf("before cleanup level %u, code %d\n", p99_unwind_level, p99_unwind_code);
P99_PROTECT: {
        printf("cleanup level %u, code %d\n", p99_unwind_level, p99_unwind_code);
        free(b);
        /* This is a return that is protected, so it should *not* be
           triggered as dangerous when this code is compiled with
           P99_CHECK_RETURN. */
        return EXIT_SUCCESS;
      }
    }
    printf("before cleanup level %u, code %d\n", p99_unwind_level, p99_unwind_code);
P99_PROTECT:
    printf("cleanup level %u, code %d\n", p99_unwind_level, p99_unwind_code);
    free(a);
  }
  /* this should cause no harm */
  P99_UNWIND(99);

  P99_SWAP(ad, bd);
  P99_SWAP(a4, b4);
  P99_SWAP(ac, bc);
  P99_SWAP(ab, bb);
  P99_SWAP(am, bm);
  P99_SWAP(av, bv);
  /* This one is bogus but we can't avoid it */
  P99_SWAP(ad, b4);
  P99_SWAP(a4, bd);
  /* Use this to test failure */
  /* P99_SWAP(ac, bd); */
  /* P99_SWAP(bd, ac); */
  /* P99_SWAP(ad, bm); */
}
