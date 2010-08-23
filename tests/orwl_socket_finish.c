/*
** orwl_socket_test.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Mar 24 21:50:12 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_posix_default.h"
#include "orwl_socket.h"
#include "orwl_server.h"
#include "orwl_register.h"
#include "orwl_wait_queue.h"

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
    uint64_t ret = orwl_send(&other, &seed, 0, NULL);
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
  int Y[17] = {  P99_DUPL(17, 1) };
  int Z[22] = {  P99_POSS(22) };
  P99_VASSIGNS(Z, int x, double k, int y, bool r);
  P99_VASSIGNS(Y, int s);
  P99_VASSIGNS(Y, int i, float o);
  P99_VASSIGNS(Y, void);
  P99_VASSIGNS(Y,);
}
