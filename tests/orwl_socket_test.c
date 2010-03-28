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

#define strtoul(...) strtoul _call_with(3, __VA_ARGS__)(strtoul, __VA_ARGS__)
declare_default_arg(strtoul, 2, int, 0);
declare_default_arg(strtoul, 1, char **, NULL);

define_default_arg(strtoul, 2, int);
define_default_arg(strtoul, 1, char **);

void test_callback(auth_sock *Arg) {
  diagnose(Arg->fd, "message of size %d", Arg->len);
  for (size_t i = 0; i < Arg->len; ++i)
    report(stdout, "%jX", (uintmax_t)Arg->mes[i]);
  orwl_domain_call(ORWL_FTAB(auth_sock), Arg->mes[0], Arg);
}

int main(int argc, char **argv) {
  report(stderr, "starting");
  /* ORWL_TYPE_DYNAMIC_INIT(auth_sock); */
  orwl_types_init();
  orwl_server srv
    = ORWL_SERVER_INITIALIZER(
                              srv,
                              test_callback,
                              4,
                              orwl_inet_addr(argv[1]),
                              0);
  report(stderr, "starting %jX:0x%jX", srv.host.ep.addr, srv.host.ep.port);
  pthread_t id;
  orwl_server_create(&srv, &id);
  report(stderr, "started %jX:0x%jX, got id 0x%jX", srv.host.ep.addr, srv.host.ep.port, id);

  rand48_t seed = { srv.host.ep.addr, srv.host.ep.port };

  if (argc > 2) {
    in_addr_t addr = orwl_inet_addr(argv[2]);
    uint16_t port = strtoul(argv[3]);


    orwl_endpoint other = { .addr = addr, .port = port };
    uint64_t mess[2] = {  ORWL_OBJID(insert_peer), srv.host.ep.port };
    /* wait until the other side is up. */
    while (orwl_send(&other, seed, mess, 2)) {
      int ret = pthread_kill(id, 0);
      if (ret) {
        char mesg[256] = INITIALIZER;
        strerror_r(ret, mesg, 256);
        report(stderr, "Server already terminated: %s", mesg);
        break;
      }
      sleepfor(0.2);
    }
    report(stderr, "server %jX:0x%jX is set up", srv.host.ep.addr, srv.host.ep.port, id);
  } else {
    report(stderr, "initial server %jX:0x%jX is set up", srv.host.ep.addr, srv.host.ep.port, id);
    for (size_t t = 0; t < 1000; ++t) {
      sleepfor(1.0);
      report(stderr, "looping %jd", t);
      orwl_host *n = NULL;
      MUTUAL_EXCLUDE(srv.host.mut)
        n = srv.host.next;
      report(stderr, "%p -- %p", (void*)&srv.host, (void*)n);
      for (orwl_host *h = n; h != &srv.host; ) {
        orwl_endpoint other = INITIALIZER;
        MUTUAL_EXCLUDE(h->mut) {
          other.addr = h->ep.addr;
          other.port = h->ep.port;
          h = h->next;
        }
        uint64_t mess[4] = { ORWL_OBJID(do_nothing), srv.host.ep.addr, srv.host.ep.port, t };
        report(stderr, "sending to /%X:0x%X/ ", other.addr, other.port);
        orwl_send(&other, seed, mess, 4);
      }
    }
    //orwl_send(&other, seed, NULL, 0);
  }

  orwl_server_join(id);
  orwl_pthread_wait_detached();
}
