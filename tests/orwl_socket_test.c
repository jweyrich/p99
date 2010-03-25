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

void test_callback(uint64_t const* mes, size_t n) {
  for (size_t i = 0; i < n; ++i)
    report(stdout, "%jX", (uintmax_t)mes[i]);
}


#define strtoul(...) strtoul _call_with(3, __VA_ARGS__)(strtoul, __VA_ARGS__)
declare_default_arg(strtoul, 2, int, 0);
declare_default_arg(strtoul, 1, char **, NULL);

define_default_arg(strtoul, 2, int);
define_default_arg(strtoul, 1, char **);

int main(int argc, char **argv) {
  in_addr_t addr[2] = { orwl_inet_addr(argv[1]), orwl_inet_addr(argv[3]) };
  uint16_t port[2] = { strtoul(argv[2]), strtoul(argv[4]) };
  size_t orwl_mynum = strtoul(argv[5]);
  size_t orwl_np = 2;
  orwl_server srv = {
    .ep = { .addr = addr[orwl_mynum], .port = port[orwl_mynum] },
    .max_connections = 4,
    .cb = test_callback,
  };
  report(stderr, "starting %jX:%jX", srv.ep.addr, srv.ep.port);
  pthread_t id;
  orwl_server_create(&srv, &id);
  report(stderr, "started %jX:%jX, got id %jX", srv.ep.addr, srv.ep.port, id);

  orwl_endpoint other = { .addr = addr[!orwl_mynum], .port = port[!orwl_mynum] };
  uint64_t mess[3] = { orwl_mynum, addr[orwl_mynum], port[orwl_mynum] };
  rand48_t seed = { orwl_mynum, addr[orwl_mynum], port[orwl_mynum] };
  /* wait until the other side is up. */
  while (orwl_send(&other, seed, mess, 3)) {
    int ret = pthread_kill(id, 0);
    if (ret) {
      char mesg[256] = INITIALIZER;
      strerror_r(ret, mesg, 256);
      report(stderr, "Server already terminated: %s", mesg);
      break;
    }
    sleepfor(0.2);
  }
  orwl_send(&other, seed, NULL, 0);
  orwl_server_join(id);
  orwl_pthread_wait_detached();
}
