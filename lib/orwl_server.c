/*
** orwl_server.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 20:24:07 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_posix_default.h"
#include "orwl_server.h"

#include "orwl_header.h"
#include "orwl_socket.h"
#include "orwl_wait_queue.h"

orwl_server* orwl_server_init(orwl_server *serv,
                              size_t max_connections,
                              size_t max_queues,
                              in_addr_t addr,
                              in_port_t port) {
  memset(serv, 0, sizeof(orwl_server));
  serv->fd_listen = -1;
  orwl_host_init(&serv->host, addr, port);
  serv->host.refs = 1;
  serv->max_connections = max_connections;
  serv->max_queues = max_queues;
  serv->wqs = max_queues ? orwl_wq_vnew(max_queues) : NULL;
  return serv;
}

P99_DEFINE_DEFARG(orwl_server_init, , (size_t)20u, TNULL(size_t), TNULL(in_addr_t), TNULL(in_port_t));

void orwl_server_close(orwl_server *serv) {
  MUTUAL_EXCLUDE(serv->host.mut)
    if (serv->fd_listen != -1) {
      int fd = serv->fd_listen;
      serv->fd_listen = -1;
      close(fd);
    }
}


void orwl_server_terminate(orwl_server *serv, rand48_t *seed) {
  orwl_send(&serv->host.ep, seed, NULL, 0);
}

P99_DEFINE_DEFARG(orwl_server_terminate, , seed_get());


void orwl_server_destroy(orwl_server *serv) {
  orwl_server_close(serv);
  orwl_host *n = NULL;
  MUTUAL_EXCLUDE(serv->host.mut)
    n = serv->host.next;
  for (orwl_host *h = n; h != &serv->host; ) {
    MUTUAL_EXCLUDE(h->mut) {
      n = h;
      h = h->next;
    }
    orwl_host_delete(n);
  }
  if (serv->wqs) orwl_wq_vdelete(serv->wqs);
  if (serv->whs) orwl_wh_vdelete(serv->whs);
  if (serv->info) free(serv->info);
  orwl_server_init(serv);
}

DEFINE_NEW_DELETE(orwl_server);

/**
 ** @brief lauch a server that fulfills rpc requests
 ** @msc
 **   caller1,procedure1,thread1,main,server,thread2,procedure2,caller2;
 **   main -> server [label="orwl_server_create()", URL="\ref orwl_server_create()"];
 **   caller1 -> server [label="orwl_send(procedure1, ...)", URL="\ref orwl_send()"];
 **   caller2 -> server [label="orwl_send(procedure2, ...)", URL="\ref orwl_send()"];
 **   server->thread1 [label="auth_sock_create(procedure1, ...)", URL="\ref auth_sock_create()"];
 **   thread1->procedure1 [label="procedure1(...)"];
 **   server->thread2 [label="auth_sock_create(procedure2, ...)", URL="\ref auth_sock_create()"];
 **   procedure1->caller1 [label="auth_sock_close()", URL="\ref auth_sock_close()"];
 **   thread2->procedure2 [label="procedure2(...)"];
 **   procedure1->thread1 [label="return"];
 **   procedure2->caller2 [label="auth_sock_close()", URL="\ref auth_sock_close()"];
 **   thread1->main [label="pthread_exit()"];
 **   procedure2->thread2 [label="return"];
 **   thread2->main [label="pthread_exit()"];
 **   server->main [label="pthread_exit()"];
 ** @endmsc
 **/
DEFINE_THREAD(orwl_server) {
  report(0, "starting server");
  Arg->fd_listen = socket(AF_INET);
  if (Arg->fd_listen != -1) {
    rand48_t seed = RAND48_T_INITIALIZER;
    struct sockaddr_in addr = {
      .sin_addr = addr2net(&Arg->host.ep.addr),
      .sin_port = port2net(&Arg->host.ep.port),
      .sin_family = AF_INET,
    };
    socklen_t len = sizeof(addr);
    if (bind(Arg->fd_listen, (struct sockaddr*) &addr, sizeof(addr)) == -1)
      goto TERMINATE;
    /* If the port was not yet specified find and store it. */
    if (!addr.sin_port) {
      if (getsockname(Arg->fd_listen, (struct sockaddr*)&addr, &len) == -1)
        goto TERMINATE;
      port_t_init(&Arg->host.ep.port, addr.sin_port);
    }
    char const* server_name = orwl_endpoint_print(&Arg->host.ep);
    report(0, "server listening at %s", server_name);
    if (listen(Arg->fd_listen, Arg->max_connections) == -1)
      goto TERMINATE;
    for (uint64_t t = 1; Arg->fd_listen != -1; ++t) {
      /* Do this work before being connected */
      uint64_t chal = orwl_rand64(&seed);
      uint64_t repl = orwl_challenge(chal);
      header_t header = INITIALIZER;

      if (Arg->info && Arg->info_len) progress(1, t, "%s", Arg->info);

      if (!repl) {
        report(1, "cannot serve without a secret");
        close(Arg->fd_listen);
        Arg->fd_listen = -1;
        goto TERMINATE;
      }
      int fd = -1;
      do {
        fd = accept(Arg->fd_listen);
      } while(fd == -1);

      /* Receive a challenge from the new connection */
      if (orwl_recv_(fd, header, header_t_els))
        goto FINISH;
      header[1] = orwl_challenge(header[0]);
      header[0] = chal;
      /* Send the reply and a new challenge to the new connection */
      if (orwl_send_(fd, header, header_t_els))
        goto FINISH;
      /* Receive the reply and the message length from the new connection */
      if (orwl_recv_(fd, header, header_t_els))
        goto FINISH;
      if (header[1] == repl) {
        size_t len = header[0];
        if (len) {
          auth_sock *sock = NEW(auth_sock, fd, Arg, len);
          auth_sock_create(sock, NULL);
          /* The spawned thread will close the fd. */
          continue;
        } else {
          /* The authorized empty message indicates termination */
          diagnose(fd, "Received termination message, closing fd %d", fd);
          close(fd);
          close(Arg->fd_listen);
          Arg->fd_listen = -1;
          break;
        }
      } else {
        diagnose(fd, "You are not authorized to to talk on fd %d", fd);
      }
    FINISH:
      close(fd);
    }
  }
 TERMINATE:
  if (errno) {
    perror("cannot proceed");
  }
  if (Arg->fd_listen != -1) close(Arg->fd_listen);
}

void orwl_server_block(orwl_server *srv) {
  assert(!srv->whs);
  srv->whs = orwl_wh_vnew(srv->max_queues);
  for (uint64_t i = 0; i < srv->max_queues; ++i) {
    orwl_wh *whp = &srv->whs[i];
    orwl_wq_request(&srv->wqs[i], &whp, 1);
  }
}

void orwl_server_unblock(orwl_server *srv) {
  assert(srv->whs);
  for (uint64_t i = 0; i < srv->max_queues; ++i) {
    orwl_wh_acquire(&srv->whs[i]);
    orwl_wh_release(&srv->whs[i]);
  }
  orwl_wh_vdelete(srv->whs);
  srv->whs = NULL;
}
