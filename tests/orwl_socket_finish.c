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
#include "orwl_register.h"
#include "orwl_wait_queue.h"
#include "orwl_posix_default.h"

void ftaster _SKIP_ (int A);

declare_defarg_0(ftaster, int, -1);
define_defarg_0(ftaster, int);


#define ftaster(...) CALL_WITH_DEFAULTS_EVEN_EMPTY(ftaster, 1, _COMMA_ __VA_ARGS__)

void ftaster _SKIP_ (int A) {
  report(stderr, "ftaster has %d", A);
}

void ftister(int A, unsigned B) {
  report(stderr, "ftister has %d %u", A, B);
}
declare_defarg(ftister, 1, unsigned, 1);
declare_defarg_0(ftister, int, -2);
define_defarg(ftister, 1, unsigned);
define_defarg_0(ftister, int);

#define ftister(...) CALL_WITH_DEFAULTS_EVEN_EMPTY(ftister, 2, _COMMA_ __VA_ARGS__)

typedef struct { int x; } test_t;

void ftester(test_t A, unsigned B, double C) {
  report(stderr, "ftester has %d %u %g", A.x, B, C);
}
declare_defarg(ftester, 2, double, 0.5);
declare_defarg(ftester, 1, unsigned, 2);
declare_defarg_0(ftester, test_t, (test_t){ -2 });
define_defarg(ftester, 2, double);
define_defarg(ftester, 1, unsigned);
define_defarg_0(ftester, test_t);

#define ftester(...) CALL_WITH_DEFAULTS_EVEN_EMPTY(ftester, 3, _COMMA_ __VA_ARGS__)


void test_callback(auth_sock *Arg) {
  diagnose(Arg->fd, "message of size %zd", Arg->len);
  for (size_t i = 0; i < Arg->len; ++i)
    report(stdout, "%jX", (uintmax_t)Arg->mes[i]);
  orwl_domain_call(ORWL_FTAB(auth_sock), Arg->mes[0], Arg);
}

int main(int argc, char **argv) {
  report(stderr, "starting");
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
    in_port_t port = strtoul(argv[2]);
    report(stderr, "ending %jX:0x%jX", (uintmax_t)addr, (uintmax_t)port);

    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(addr, port);
    errno = 0;
    /* wait until the other side is up. */
    uint64_t ret = orwl_send(&other, &seed, NULL, 0);
    char messg[245];
    sprintf(messg, "finish server /%lX:0x%lX/, %jX",
            (ulong)addr, (ulong)port, (uintmax_t)ret);
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
}
