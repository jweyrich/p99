/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
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

P99_INSTANTIATE(void, orwl_hton, uint64_t*, uint64_t const*, size_t);
P99_INSTANTIATE(void, orwl_ntoh, uint64_t*, uint64_t const*, size_t);

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

P99_INSTANTIATE(char const*, orwl_inet_ntop, struct sockaddr const* addr, char* buf, size_t size);
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
  if (res) freeaddrinfo(res);
  return ret;
}

P99_INSTANTIATE(char const*, hostname, char *, size_t);
