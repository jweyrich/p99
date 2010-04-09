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
    report(stdout, "%" PRIX64 "", Arg->mes[i]);
  orwl_domain_call(ORWL_FTAB(auth_sock), Arg->mes[0], Arg);
}

int main(int argc, char **argv) {
  report(1, "starting");
  int ret = 0;

  orwl_types_init();
  orwl_server srv
    = ORWL_SERVER_INITIALIZER(
                              srv,
                              test_callback,
                              4,
                              orwl_inet_addr(argv[1]),
                              0);
  report(1, "starting %" PRIX32 ":0x%" PRIX16,
         addr2net(&srv.host.ep.addr),
         port2net(&srv.host.ep.port));
  pthread_t id;
  orwl_server_create(&srv, &id);
  report(1, "started %" PRIX32 ":0x%" PRIX16 ", got id 0x%" PRIX64,
         addr2net(&srv.host.ep.addr),
         port2net(&srv.host.ep.port),
         id);

  rand48_t seed = RAND48_T_INITIALIZER;

  if (argc > 2) {
    in_addr_t addr = orwl_inet_addr(argv[2]);
    in_port_t port = str2uint16_t(argv[3]);


    orwl_endpoint other = ORWL_ENDPOINT_INITIALIZER(addr, port);
    /* wait until the other side is up. */
    /* ep.port is already in host order */
    while (orwl_rpc(&other, &seed, auth_sock_insert_peer, srv.host.ep.port.p)
           == TONES(uint64_t)) {
      ret = pthread_kill(id, 0);
      if (ret) break;
      sleepfor(0.2);
    }
    report(1, "server %" PRIX32 ":0x%" PRIX16 " is set up",
           addr2net(&srv.host.ep.addr),
           port2net(&srv.host.ep.port));
  } else {
    report(1, "initial server %" PRIX32 ":0x%" PRIX16 " is set up",
           addr2net(&srv.host.ep.addr),
           port2net(&srv.host.ep.port));
    for (size_t t = 0; t < 1000; ++t) {
      ret = pthread_kill(id, 0);
      if (ret) break;
      sleepfor(1.0);
      report(1, "looping %zd", t);
      orwl_host *n = NULL;
      MUTUAL_EXCLUDE(srv.host.mut)
        n = srv.host.next;
      report(1, "%p -- %p", (void*)&srv.host, (void*)n);
      for (orwl_host *h = n; h != &srv.host; ) {
        orwl_endpoint other = { INITIALIZER };
        MUTUAL_EXCLUDE(h->mut) {
          other.addr = h->ep.addr;
          other.port = h->ep.port;
          h = h->next;
        }
        report(1, "sending to /%" PRIX32 ":0x%" PRIX16 "/ ",
               addr2net(&other.addr),
               port2net(&other.port));
        /* ep.addr and ep.port are already in host order */
        orwl_rpc(&other, &seed, auth_sock_do_nothing,
                  srv.host.ep.addr.a, srv.host.ep.port.p, t);
      }
    }
  }

  orwl_server_join(id);
  if (ret) {
    char mesg[256] = INITIALIZER;
    strerror_r(ret, mesg, 256);
    report(1, "Server already terminated: %s", mesg);
  }
  orwl_pthread_wait_detached();
  report(1, "host %p and next %p", (void*)srv.host.next, (void*)&srv.host);
  orwl_server_destroy(&srv);
}
