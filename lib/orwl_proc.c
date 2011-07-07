/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_proc.h"
#include "orwl_proc_symbols.h"

#include "orwl_server.h"
#include "orwl_header.h"
#include "orwl_posix.h"
#include "p99_posix_default.h"

DEFINE_ONCE(orwl_proc,
            orwl_rand) {
}


P99_INSTANTIATE(orwl_proc*, orwl_proc_init, orwl_proc *,
                                  int,
                                  orwl_server*,
                uint64_t,
                orwl_buffer,
                orwl_thread_cntrl*);

void orwl_proc_untie_caller(orwl_proc *sock) {
  if (sock->is_untied) return;
  report(0, "detaching %p", (void*)sock);
  if (sock->det) {
    /* This thread is launched locally. */
    orwl_thread_cntrl_freeze(sock->det);
  } else {
    /* Ack the termination of the call */
    orwl_header header = ORWL_HEADER_INITIALIZER(sock->ret);
    orwl_send_(sock->fd, header, orwl_header_els, sock->remoteorder);
    /* Since we are doing blocking send / receive the probability that
       we have a walking duplicate of an ancient package is
       minimal. Thus allow the reuse of ports. */
    if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &P99_LVAL(int const, 1), sizeof(int)) < 0)
      P99_HANDLE_ERRNO {
      P99_XDEFAULT :
        perror("setting socket to SO_REUSEADDR failed");
      }
    if (setsockopt(sock->fd, SOL_SOCKET, SO_LINGER, &P99_LVAL(struct linger, .l_onoff = 1, .l_linger = 1), sizeof(struct linger)) < 0)
      P99_HANDLE_ERRNO {
      P99_XDEFAULT :
        perror("setting socket to linger failed");
      }
    if (close(sock->fd) < 0)
      P99_HANDLE_ERRNO {
      P99_XDEFAULT :
        perror("close on socket failed");
      }
    sock->fd = -1;
  }
  sock->is_untied = true;
}

void orwl_proc_destroy(orwl_proc *sock) {
  orwl_proc_untie_caller(sock);
  if (sock->det) {
    orwl_thread_cntrl_wait_for_caller(sock->det);
    orwl_thread_cntrl_delete(sock->det);
  }
  if (sock->back.data) free(sock->back.data);
  orwl_proc_init(sock);
}

DEFINE_NEW_DELETE(orwl_proc);

DEFINE_ORWL_PROC_FUNC(orwl_server_callback, uint64_t funcID) {
  ORWL_PROC_READ(Arg, orwl_server_callback, uint64_t funcID);
  orwl_domain_call(ORWL_FTAB(orwl_proc), funcID, Arg);
}

DEFINE_THREAD(orwl_proc) {
  assert(Arg->mes.data);
  report(0, "starting %p", (void*)Arg);
  if (Arg->fd == -1
      || (!orwl_recv_(Arg->fd, Arg->mes.data, Arg->mes.len, Arg->remoteorder)
          && Arg->srv)) {
    /* do something with mess here */
    orwl_server_callback(Arg);
  }
  orwl_proc_untie_caller(Arg);
  report(0, "ending %p", (void*)Arg);
}

orwl_addr orwl_proc_getpeer(orwl_proc const*Arg) {
  struct sockaddr_in addr = SOCKADDR_IN_INIIALIZER;
  int ret = getpeername(Arg->fd, (struct sockaddr*)&addr, &P99_LVAL(socklen_t, sizeof(struct sockaddr_in)));
  return  (orwl_addr)ORWL_ADDR_INITIALIZER((ret == -1) ? P99_0(in_addr_t) : addr.sin_addr.s_addr);
}
