/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_posix.h"
#include "p99_posix_default.h"
#include "orwl_server.h"

#include "orwl_header.h"
#include "orwl_socket.h"
#include "orwl_wait_queue.h"
#include "orwl_remote_queue.h"

DEFINE_ONCE(orwl_server,
            orwl_thread,
            auth_sock,
            orwl_wq,
            orwl_wh,
            orwl_handle,
            orwl_rand) {
}

orwl_server* orwl_server_init(orwl_server *serv,
                              size_t max_connections,
                              size_t max_queues,
                              char const* endp) {
  INIT_ONCE(orwl_server);
  if (serv) {
    *serv = (orwl_server) {
      .fd_listen = -1,
      .max_connections = max_connections,
      .max_queues = max_queues,
      .wqs = max_queues ? orwl_wq_vnew(max_queues) : P99_0(void*),
      .host = ORWL_HOST_INITIALIZER(serv->host, 0, 0, 1),
      .id_initialized = P99_0(bool*),
      .unblocked_locations = 0,
    };
    pthread_rwlock_init(&serv->lock);
    if (endp && endp[0]) orwl_endpoint_parse(&serv->host.ep, endp);
  }
  return serv;
}

void orwl_server_close(orwl_server *serv) {
  if (serv) {
    MUTUAL_EXCLUDE(serv->host.mut) {
      if (serv->fd_listen != -1) {
        int fd = serv->fd_listen;
        serv->fd_listen = -1;
        close(fd);
      }
    }
  }
}


void orwl_server_terminate(orwl_server *serv) {
  if (serv) orwl_send(&serv->host.ep, seed_get(), 0, P99_0(uint64_t*));
}

void orwl_server_destroy(orwl_server *serv) {
  orwl_server_close(serv);
  orwl_host *n = 0;
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
  if (serv->id_initialized) bool_vdelete(serv->id_initialized);
  if (serv->info) free(serv->info);
  orwl_server_init(serv);
}

DEFINE_NEW_DELETE(orwl_server);

DEFINE_THREAD(orwl_server) {
  report(0, "starting server");
  orwl_posix_init();
  char const* volatile errorstr = 0;
  Arg->fd_listen = socket(AF_INET);
  if (P99_LIKELY(Arg->fd_listen >= 0)) {
    rand48_t* seed = seed_get();
    struct sockaddr_in addr = {
      .sin_addr = addr2net(&Arg->host.ep.addr),
      .sin_port = port2net(&Arg->host.ep.port),
      .sin_family = AF_INET,
    };
    socklen_t len = sizeof(addr);

    /* Now that we have a valid file descriptor, protect its closing. */
    P99_UNWIND_PROTECT {

      if (P99_UNLIKELY(bind(Arg->fd_listen, (struct sockaddr*) &addr, sizeof(addr)))) {
        errorstr = "orwl_server could not bind";
        if (!errno) errno = EINVAL;
        P99_UNWIND_RETURN;
      }
      /* If the port was not yet specified find and store it. */
      if (!addr.sin_port) {
        if (P99_UNLIKELY(getsockname(Arg->fd_listen, (struct sockaddr*)&addr, &len))) {
          errorstr = "orwl_server could not find listening address";
          if (!errno) errno = EINVAL;
          P99_UNWIND_RETURN;
        }
        port_t_init(&Arg->host.ep.port, addr.sin_port);
      }
      char const* server_name = orwl_endpoint_print(&Arg->host.ep);
      report(1, "server listening at %s", server_name);
      if (P99_UNLIKELY(listen(Arg->fd_listen, Arg->max_connections))) {
        errorstr = "orwl_server could not listen";
        if (!errno) errno = EINVAL;
        P99_UNWIND_RETURN;
      }
      for (uint64_t t = 1; Arg->fd_listen != -1; ++t) {
        /* Do this work before being connected */
        uint64_t const chal = orwl_rand64(seed);
        uint64_t const repl = orwl_challenge(chal);
        header_t header = HEADER_T_INITIALIZER(0);

        if (Arg->info && Arg->info_len) progress(1, t, "%s", Arg->info);

        if (P99_UNLIKELY(!repl)) {
          errorstr = "orwl_server cannot serve without a secret";
          if (!errno) errno = EINVAL;
          P99_UNWIND_RETURN;
        }
        int fd = -1;
        for (;fd < 0;) {
          fd = accept(Arg->fd_listen);
          if (fd >= 0) break;
          perror("orwl_server encountered error in accept, retrying");
          errno = 0;
          sleepfor(1E-2);
        }

        /* Now that we have a valid file descriptor, protect its closing. */
        P99_UNWIND_PROTECT {
          /* Receive a challenge from the new connection */
          if (P99_UNLIKELY(orwl_recv_(fd, header, header_t_els, 0)))
            P99_UNWIND(1);
          header[1] = orwl_challenge(header[0]);
          header[0] = chal;
          /* challenge / reply of the new connection */
          if (P99_UNLIKELY(orwl_send_(fd, header, header_t_els, 0)
                           || orwl_recv_(fd, header, header_t_els, 0)))
            P99_UNWIND(1);
          if (header[1] == repl) {
            size_t len = header[0];
            if (len) {
              auth_sock *sock = P99_NEW(auth_sock, fd, Arg, len, header[2]);
              auth_sock_create(sock);
              /* The spawned thread will close the fd. */
              fd = -1;
            } else {
              /* The authorized empty message indicates termination */
              errorstr = "orwl_server received termination message";
              P99_UNWIND_RETURN;
            }
          } else {
            diagnose(fd, "You are not authorized to talk on fd %d", fd);
          }
        P99_PROTECT:
          if (fd != -1) close(fd);
        }
      }
    P99_PROTECT:
      report(1, "\n");
      if (!errorstr) errorstr = "orwl_server finished";
      if (errno) {
        perror(errorstr);
        errno = 0;
      } else fprintf(stderr, "%s.\n", errorstr);
      if (Arg->fd_listen != -1) {
        close(Arg->fd_listen);
        Arg->fd_listen = -1;
      }
      Arg->host.ep.port.p = P99_TMAX(in_port_t);
    }
  }
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
  srv->whs = 0;
}

void orwl_server_delayed_unblock(orwl_server *srv, size_t nb_tasks) {
  ORWL_CRITICAL {
    srv->unblocked_locations++;
  }
  if (srv->unblocked_locations == nb_tasks)
    orwl_server_unblock(srv);
}

void
orwl_start(orwl_server *serv,       /*!< [out] the server object to initialize */
           size_t max_connections,  /*!< [in] maximum socket queue length,
                                      defaults to 20 */
           size_t max_queues,       /*!< [in] the maximum number of locations,
                                      defaults to 0 */
           char const* endp         /*!< [in] defaults to the
                                      null address */
           ) {
  orwl_server_init(serv, max_connections, max_queues, endp);
  orwl_server_create(serv, &serv->id);
  /* give the server the chance to fire things up */
  while (!port2net(&serv->host.ep.port)) sleepfor(0.01);
}

void
orwl_stop(orwl_server *serv) {
  orwl_server_join(serv->id);
  orwl_server_destroy(serv);
}

P99_INSTANTIATE(bool, orwl_alive, orwl_server *);
