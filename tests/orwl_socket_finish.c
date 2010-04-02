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
    rand48_t seed = { addr2net(&srv.host.ep.addr), port2net(&srv.host.ep.port) };

    in_addr_t addr = orwl_inet_addr(argv[1]);
    in_port_t port = strtoul(argv[2]);
    report(stderr, "ending %jX:0x%jX", (uintmax_t)addr, (uintmax_t)port);

    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(addr, port);
    errno = 0;
    /* wait until the other side is up. */
    orwl_send(&other, seed, NULL, 0);
    char messg[245];
    sprintf(messg, "finish server /%lX:0x%lX/", (ulong)addr, (ulong)port);
    perror(messg);
    errno = 0;
  }
}
