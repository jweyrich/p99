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
      }
    }
    printf("before cleanup level %u, code %d\n", p99_unwind_level, p99_unwind_code);
  P99_PROTECT:
    printf("cleanup level %u, code %d\n", p99_unwind_level, p99_unwind_code);
    free(a);
  }
  /* this should cause no harm */
  P99_UNWIND(99);
}
