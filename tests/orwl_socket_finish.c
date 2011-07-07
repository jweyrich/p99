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

inline void F_(int a) { }

void F_(int a);


P99_PROTOTYPE(void, ftoaster);

/* It makes not much sense to define a macro with 0 arguments by
   itself. But such a thing might occur through an automatic
   generation of sources, so better test for it and treat the corner
   case. */
#define ftoaster(...) P99_CALL_DEFARG(ftoaster, 0, __VA_ARGS__)

/* check a version that declares with explicit `void' parameter list */
void ftoaster (void) {
  report(1, "ftoaster has is called");
}


P99_PROTOTYPE(void, ftaster, int);

P99_DECLARE_DEFARG(ftaster, -1);
P99_DEFINE_DEFARG(ftaster, -1);


#define ftaster(...) P99_CALL_DEFARG(ftaster, 1, __VA_ARGS__)

void ftaster(int A) {
  report(1, "ftaster has %d", A);
}


P99_PROTOTYPE(void, ftister, int, unsigned);

void ftister(int A, unsigned B) {
  report(1, "ftister has %d %u", A, B);
}
P99_DECLARE_DEFARG(ftister, -2, 1);
P99_DEFINE_DEFARG(ftister, -2, 1);

#define ftister(...) P99_CALL_DEFARG(ftister, 2, __VA_ARGS__)

typedef struct { int x; } test_t;


P99_PROTOTYPE(void, ftester, test_t, unsigned, double);
void ftester(test_t A, unsigned B, double C) {
  report(1, "ftester has %d %u %g", A.x, B, C);
}
P99_DECLARE_DEFARG(ftester, ((test_t){ -2 }), 2, 0.5);
P99_DEFINE_DEFARG(ftester, ((test_t){ -2 }), 2, 0.5);

#define ftester(...) P99_CALL_DEFARG(ftester, 3, __VA_ARGS__)

int main(int argc, char **argv) {
  report(1, "starting");
  /* ORWL_TYPE_DYNAMIC_INIT(auth_sock); */
  orwl_types_init();
  if (argc > 1) {
    rand48_t seed = RAND48_T_INITIALIZER;

    report(1, "connecting to %s", argv[1]);
    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(0, 0);
    orwl_endpoint_parse(&other, argv[1]);
    errno = 0;
    /* wait until the other side is up. */
    uint64_t ret = orwl_send(0, &other, &seed, P99_LVAL(orwl_buffer));
    char messg[245];
    sprintf(messg, "finish server %s %" PRIX64,
            argv[1], ret);
    perror(messg);
    errno = 0;
  }
  (void)ftoaster;
  ftoaster();
  (void)ftaster;
  ftaster();
  ftaster(12);
  (void)ftister;
  ftister();
  ftister(13);
  ftister(14, 15);
  (void)ftester;
  ftester();
  ftester((test_t){16});
  ftester((test_t){17}, 18);
  ftester((test_t){19}, 20, 19.9999);
  report(1, "%s %s %s %s %s", P99_PRI(char, X,), P99_PRI(short, X,), P99_PRI(signed, X,), P99_PRI(long, X,), P99_PRI(long long, X,));
  report(1, "%zu %zu %zu %zu %zu", sizeof(char), sizeof(short), sizeof(signed), sizeof(long), sizeof(long long));
  mfputs(stderr, P99_JOIN("aa", "BB", "CC "), PRIX(37ull), " ", PRId(37), " ", PRIo(37), " ", PRIp(argv), "\n");
  orwl_posix_init();
  orwl_posix_print();
}
