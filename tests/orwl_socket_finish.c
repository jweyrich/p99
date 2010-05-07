/*
** orwl_socket_test.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Mar 24 21:50:12 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <signal.h>
#include <stdio.h>
#include "orwl_socket.h"
#include "orwl_server.h"
#include "orwl_register.h"
#include "orwl_wait_queue.h"
#include "orwl_posix_default.h"

inline void F_(int a) { }

void F_(int a);

PROTOTYPE(void, ftoaster);


PROTOTYPE(void, ftaster, int);

DECLARE_DEFARG(ftaster, -1);
DEFINE_DEFARG(ftaster, -1);


#define ftaster(...) CALL_WITH_DEFAULTS(ftaster, 1, __VA_ARGS__)

void ftaster _SKIP_ (int A) {
  report(1, "ftaster has %d", A);
}

PROTOTYPE(void, ftister, int, unsigned);

void ftister(int A, unsigned B) {
  report(1, "ftister has %d %u", A, B);
}
DECLARE_DEFARG(ftister, -2, 1);
DEFINE_DEFARG(ftister, -2, 1);

#define ftister(...) CALL_WITH_DEFAULTS(ftister, 2, __VA_ARGS__)

typedef struct { int x; } test_t;

PROTOTYPE(void, ftester, test_t, unsigned, double);
void ftester(test_t A, unsigned B, double C) {
  report(1, "ftester has %d %u %g", A.x, B, C);
}
DECLARE_DEFARG(ftester, ((test_t){ -2 }), 2, 0.5);
DEFINE_DEFARG(ftester, ((test_t){ -2 }), 2, 0.5);

#define ftester(...) CALL_WITH_DEFAULTS(ftester, 3, __VA_ARGS__)


DECLARE_AUTH_SOCK_FUNC(test_callback, uint64_t funcID);

DEFINE_AUTH_SOCK_FUNC(test_callback, uint64_t funcID) {
  diagnose(Arg->fd, "message of size %zd", Arg->len);
  for (size_t i = 0; i < Arg->len; ++i)
    report(stdout, "%" PRIX64, Arg->mes[i]);
  AUTH_SOCK_READ(Arg, test_callback, uint64_t funcID);
  orwl_domain_call(ORWL_FTAB(auth_sock), funcID, Arg);
}

int main(int argc, char **argv) {
  report(1, "starting");
  /* ORWL_TYPE_DYNAMIC_INIT(auth_sock); */
  orwl_types_init();
  if (argc > 2) {
    orwl_server srv
      = ORWL_SERVER_INITIALIZER(
                                srv,
                                test_callback,
                                4,
                                orwl_inet_addr(argv[1]),
                                0);
    rand48_t seed = RAND48_T_INITIALIZER;

    in_addr_t addr = orwl_inet_addr(argv[1]);
    in_port_t port = str2uint16_t(argv[2]);
    report(1, "ending %" PRIX32 ":0x%" PRIX16, addr, port);

    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(addr, port);
    errno = 0;
    /* wait until the other side is up. */
    uint64_t ret = orwl_send(&other, &seed, NULL, 0);
    char messg[245];
    sprintf(messg, "finish server /%" PRIX32 ":0x%" PRIX16 "/, %" PRIX64 "",
            addr, port, ret);
    perror(messg);
    errno = 0;
  }
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
  report(1, "%s %s %s %s %s", PRI(char, X,), PRI(short, X,), PRI(signed, X,), PRI(long, X,), PRI(long long, X,));
  report(1, "%zu %zu %zu %zu %zu", sizeof(char), sizeof(short), sizeof(signed), sizeof(long), sizeof(long long));
  mfputs(stderr, JOIN("aa", "BB", "CC "), PRIX(37ull), " ", PRId(37), " ", PRIo(37), " ", PRIp(argv), "\n");
  int Y[17] = {  REPS(1, 17) };
  int Z[22] = {  POSS(22) };
  ASGS(Z, int x, double k, int y, bool r);
  ASGS(Y, int s);
  ASGS(Y, int i, float o);
  ASGS(Y, void);
  ASGS(Y,);
  IF_DEC_LT(7,5)(true)(false);
  IF_DEC_GE(5,5)(true)(false);
  IF_DEC_LE(7,5)(true)(false);
  IF_DEC_GT(5,5)(true)(false);

  IF_DEC_GT(0,0)(true)(false);
  IF_DEC_GE(0,0)(true)(false);
  IF_DEC_LT(0,0)(true)(false);
  IF_DEC_LE(0,0)(true)(false);

  IF_DEC_GT(1,0)(true)(false);

  REVS("a", "b", "c");
  long double const a0 = +0.5L;
  long double const a1 = DEC_DOUBLE();
  long double const a2 = DEC_DOUBLE(,1);
  long double const a3 = DEC_DOUBLE(,,9);
  long double const a4 = HEX_DOUBLE(,,9,-,7);
  long double const a5 = HEX_DOUBLE();
  long double const a6 = HEX_DOUBLE(,1);
  long double const a7 = HEX_DOUBLE(,,9);
  long double const a8 = HEX_DOUBLE(,,8,-,,L);
  long double const a9 = HEX_DOUBLE(-,1,8,-,1);

  _dec_add(3, 7);
  DEC_MUL(7, 2);
  DEC_MUL(9, 3);
  //DEC_MUL(8, 8);
  DEC_MUL(10, 5);

  DEC_DIV(7, 2);
  DEC_DIV(9, 3);
  DEC_DIV(8, 8);
  DEC_DIV(7, 8);
  DEC_DIV(10, 5);
  DEC_DIV(3, 7);

  DEC_MOD(7, 2);
  DEC_MOD(9, 3);
  DEC_MOD(8, 8);
  DEC_MOD(7, 8);
  DEC_MOD(10, 5);
  DEC_MOD(3, 7);
}
