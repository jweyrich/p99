/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_socket.h"

#include "orwl_server.h"
#include "orwl_header.h"
#include "orwl_posix.h"
#include "p99_posix_default.h"

uint64_t orwl_hton64(uint64_t val);
uint64_t orwl_ntoh64(uint64_t val);

p99_instantiate void orwl_hton(uint64_t *n, uint64_t const *h, size_t l);
p99_instantiate void orwl_ntoh(uint64_t* h, uint64_t const *n, size_t l);

void orwl_ntoa(struct sockaddr_in const* addr, char *name) {
  sprintf(name, "orwl://%s:%" PRIu32 "/",
          (addr->sin_addr.s_addr
           ? orwl_inet_ntop((struct sockaddr const*)addr)
           : hostname()),
          addr->sin_port);
}

static
unsigned importance(in_addr_t a) {
  unsigned ret = ~0u;
  uint32_t h = ntohl(a);
  uchar high = (h >> 24);
  uchar low = (h >> 16) & 0xFF;
  switch (high) {
  case 0:
    ret = 0;
    break;
    /* The loopback network */
  case 127:
    ret = 1;
    break;
    /* Link local addresses */
  case 169:
    ret = (low == 254) ? 2 : ~0u;
    break;
    /* The private /16 networks */
  case 192:
    ret = (low == 168) ? 3 : ~0u;
    break;
    /* The private /12 network */
  case 172:
    ret = (16 <= low && low <= 31) ? 4 : ~0u;
    break;
    /* The private /8 network */
  case 10:
    ret = 5;
    break;
  default:;
    ret = ~0u;
  }
  return ret;
}

p99_instantiate char const* orwl_inet_ntop(struct sockaddr const* addr, char* buf, size_t size);
P99_DEFINE_DEFARG(orwl_inet_ntop, , , );

in_addr_t orwl_inet_addr(char const *name) {
  in_addr_t ret = P99_0(in_addr_t);
  struct addrinfo *res = 0;
  struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_flags = AI_V4MAPPED | AI_ADDRCONFIG | AI_CANONNAME | (name ? 0 : AI_PASSIVE),
    .ai_socktype = SOCK_STREAM,
  };
  getaddrinfo(name, P99_0(char*), &hints, &res);
  report(0, "%s's canonical name is %s",
         (name ? name : "<unspecific>"),
         (res ? res->ai_canonname : "unknonwn"));
  for (struct addrinfo *p = res; p; p = p->ai_next) {
    report(0, "%s's inet address is %s",
           (name ? name : "<unspecific>"),
           orwl_inet_ntop(p->ai_addr));
    switch (p->ai_family) {
    case AF_INET: {
      struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
      assert(addr->sin_family == AF_INET);
      struct in_addr* act = &(addr->sin_addr);
      if (importance(act->s_addr) > importance(ret)) {
        ret = act->s_addr;
      }
    }
      break;
    default:;
    }
  }
  freeaddrinfo(res);
  return ret;
}

p99_instantiate auth_sock* auth_sock_init(auth_sock *sock,
                                  int fd,
                                  orwl_server* srv,
                          size_t len,
                          uint64_t remo);

void auth_sock_close(auth_sock *sock) {
  /* Ack the termination of the call */
  header_t header = HEADER_T_INITIALIZER(sock->ret);
  orwl_send_(sock->fd, header, header_t_els, sock->remoteorder);
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

void auth_sock_destroy(auth_sock *sock) {
  if (sock->fd != -1) auth_sock_close(sock);
  if (sock->back) uint64_t_vdelete(sock->back);
  auth_sock_init(sock);
}

DEFINE_NEW_DELETE(auth_sock);

static
void server_callback(auth_sock* Arg) {
  AUTH_SOCK_READ(Arg, server_callback, uint64_t funcID);
  orwl_domain_call(ORWL_FTAB(auth_sock), funcID, Arg);
}

DEFINE_THREAD(auth_sock) {
  assert(Arg->mes);
  if (!orwl_recv_(Arg->fd, Arg->mes, Arg->len, Arg->remoteorder))
    if (Arg->srv) {
      /* do something with mess here */
      server_callback(Arg);
    }
  if (Arg->fd != -1) auth_sock_close(Arg);
}




addr_t getpeer(auth_sock *Arg) {
  struct sockaddr_in addr = SOCKADDR_IN_INIIALIZER;
  int ret = getpeername(Arg->fd, (struct sockaddr*)&addr, &P99_LVAL(socklen_t, sizeof(struct sockaddr_in)));
  return  (addr_t)ADDR_T_INITIALIZER((ret == -1) ? P99_0(in_addr_t) : addr.sin_addr.s_addr);
}


p99_instantiate char const* hostname(char *buffer, size_t len);
