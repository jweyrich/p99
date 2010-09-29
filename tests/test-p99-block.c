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



/************** the try/catch implementation ***********************/


typedef enum {
  p99_try_level = 0,
  p99_throw_errno = 0,
  p99_rethrow_errno = 0,
} p99_throw_enum;


#define TRY_SETJUMP /* tous ce qu'il faut */

#define TRY_INTERNAL(EXPR)                                              \
for (int _try_int = 0; _try_int < 1; ++_try_int)                        \
  for (int p99_rethrow_errno = 0; _try_int < 1; ++_try_int)             \
    for (int const p99_throw_errno = (EXPR); _try_int < 1; ++_try_int)  \
      for (int const p99_try_level0 = p99_try_level + 1; _try_int < 1; ++_try_int) \
        for (int const p99_try_level = p99_try_level0; p99_try_level && (_try_int < 1); ++_try_int) \
          TRY_SETJUMP switch(p99_throw_errno) case 0:

#define TRY_DERIVED TRY_SETJUMP P99_UNCASE

#define CATCH(N) P99_XCASE (N): TRY_DERIVED

#define CATCHALL P99_XDEFAULT: TRY_DERIVED

#define RETHROW P99_BLOCK(p99_rethrow_errno = p99_throw_errno; break;)



int main(int argc, char *argv[]) {
  TRY_INTERNAL(argc - 1) {
    printf("general case\n");
    CATCH(1) {
      printf("caught 1\n");
    }
    CATCH(2) {
      printf("caught 2\n");
      if (argv[1][0] == 'a') RETHROW;
      else printf("caught 2, passed\n");
    }
    CATCH(3) {
      if (argv[1][0] == 'b') goto CLEANUP;
      printf("caught 3\n");
    }
    CATCHALL {
      /* Error: CATCH that is inside another CATCH block without
         intermediate TRY */
      // CATCH(0) printf("caught 3\n");
      // CATCH(1) printf("caught 3\n");
    default: printf("catch all\n");
    }
    /* Error: rethrow that is not inside a catch block */
    //RETHROW;
    printf("clean up, rethrow has %d\n", p99_rethrow_errno);
  }
  /* Error: CATCH that is not inside a TRY block */
  //CATCH(3) printf("caught 3\n");
  /* Error: rethrow that is not inside a catch block */
  //RETHROW;
  P99_PREFER(fprintf(stderr, "Happy: all allocation went well!\n");)
    CLEANUP: {
    // Do some repair work and exit gracefully
    fprintf(stderr, "Unhappy: something went wrong!\n");
  }


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
      if (argc > 1) P99_UNWIND(argv[1][0] - 'a');
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
