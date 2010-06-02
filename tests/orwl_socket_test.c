/*
** orwl_socket_test.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Mar 24 21:50:12 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_socket.h"
#include "orwl_server.h"
#include "orwl_register.h"
#include "orwl_wait_queue.h"
#include "orwl_posix_default.h"
#include "orwl_auth_sock_functions.h"

int main(int argc, char **argv) {
  report(1, "starting");
  int ret = 0;

  orwl_types_init();
  orwl_server srv
    = ORWL_SERVER_INITIALIZER(
                              srv,
                              4,
                              TNULL(in_addr_t),
                              0);
  report(1, "starting %s ", orwl_endpoint_print(&srv.host.ep));
  pthread_t id;
  orwl_server_create(&srv, &id);
  report(1, "started %s" PRIX64, orwl_endpoint_print(&srv.host.ep));

  rand48_t seed = RAND48_T_INITIALIZER;

  if (argc > 1) {
    report(1, "connecting to %s", argv[1]);
    orwl_endpoint other = { INITIALIZER };
    orwl_endpoint_parse(&other, argv[1]);

    /* wait until the other side is up. */
    /* ep.port is already in host order */
    while (orwl_rpc(&other, &seed, auth_sock_insert_peer, port2host(&srv.host.ep.port))
           == TONES(uint64_t)) {
      ret = pthread_kill(id, 0);
      if (ret) break;
      sleepfor(0.2);
    }
  } else {
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
        report(1, "sending to %s ", orwl_endpoint_print(&other));
        /* ep.addr and ep.port are already in host order */
        orwl_rpc(&other, &seed, auth_sock_do_nothing,
                 srv.host.ep.addr.a[3], srv.host.ep.port.p, t);
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
