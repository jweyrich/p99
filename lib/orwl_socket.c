/*
** orwl_socket.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 22:00:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "orwl_socket.h"
#include "orwl_rand.h"
#include "orwl_posix_default.h"
#include "orwl_remote_queue.h"
#include "orwl_server.h"
#include "orwl_header.h"

static uint32_t mycode = 0;

typedef uint32_t U;
#define Code(C0, C1, C2, C3) ((((U)C0) << 0) | (((U)C1) << 8) | (((U)C2) << 16) | (((U)C3) << 24))

#define ifCode(C0, C1, C2, C3)                          \
do {                                                    \
  static uint32_t const c = Code(C0, C1, C2, C3);       \
  if (ntohl(c) == Code(0, 1, 2, 3)) {                   \
    mycode = c;                                         \
    goto END;                                           \
  }                                                     \
 } while (false)


DECLARE_ONCE_UPON(mycode);
DEFINE_ONCE_UPON(mycode) {
    ifCode(0, 1, 2, 3);
    ifCode(0, 1, 3, 2);
    ifCode(0, 2, 1, 3);
    ifCode(0, 2, 3, 1);
    ifCode(0, 3, 1, 2);
    ifCode(0, 3, 2, 1);

    ifCode(1, 0, 2, 3);
    ifCode(1, 0, 3, 2);
    ifCode(1, 2, 0, 3);
    ifCode(1, 2, 3, 0);
    ifCode(1, 3, 0, 2);
    ifCode(1, 3, 2, 0);

    ifCode(2, 0, 1, 3);
    ifCode(2, 0, 3, 1);
    ifCode(2, 1, 0, 3);
    ifCode(2, 1, 3, 0);
    ifCode(2, 3, 0, 1);
    ifCode(2, 3, 1, 0);

    ifCode(3, 0, 1, 2);
    ifCode(3, 0, 2, 1);
    ifCode(3, 1, 0, 2);
    ifCode(3, 1, 2, 0);
    ifCode(3, 2, 0, 1);
    ifCode(3, 2, 1, 0);
 END:;
}

bool same_endianess(uint32_t c) {
  INIT_ONCE_UPON(mycode, mycode);
  return c == mycode;
}

void orwl_hton(uint32_t *n, uint64_t const *h, size_t l);
DEFINE_DEFARG(orwl_hton, , , 1);
void orwl_ntoh(uint64_t* h, uint32_t const *n, size_t l);
DEFINE_DEFARG(orwl_ntoh, , , 1);


in_addr_t _inet4_addr = INITIALIZER;

DEFINE_ONCE_UPON(inet4_addr) {
  char const* str = getenv("INET4");
  struct in_addr inaddr = INITIALIZER;
  if (inet_aton(str, &inaddr)) {
    _inet4_addr = inaddr.s_addr;
  }
}

in_addr_t inet4_addr(void);

addr_t* addr_t_init(addr_t *A, in_addr_t I);

DEFINE_DEFARG(addr_t_init, , TNULL(in_addr_t));

struct in_addr addr2net(addr_t const*A);

struct in6_addr addr2net6(addr_t const*A);

port_t* port_t_init(port_t *A, in_port_t P);

DEFINE_DEFARG(port_t_init, , TNULL(in_port_t));

in_port_t port2net(port_t const*A);

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

char const* orwl_inet_ntop(struct sockaddr const* addr, char* buf, size_t size);
DEFINE_DEFARG(orwl_inet_ntop, , , );

in_addr_t orwl_inet_addr(char const *name) {
  in_addr_t ret = INITIALIZER;
  struct addrinfo *res = INITIALIZER;
  struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_flags = AI_V4MAPPED | AI_ADDRCONFIG | AI_CANONNAME | (name ? 0 : AI_PASSIVE),
    .ai_socktype = SOCK_STREAM,
  };
  getaddrinfo(name, NULL, &hints, &res);
  report(1, "%s's canonical name is %s",
         (name ? name : "<unspecific>"),
         (res ? res->ai_canonname : "unknonwn"));
  for (struct addrinfo *p = res; p; p = p->ai_next) {
    report(1, "%s's inet address is %s",
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

auth_sock* auth_sock_init(auth_sock *sock,
                                  int fd,
                                  orwl_server* srv,
                                  size_t len);

DEFINE_DEFARG(auth_sock_init, , -1, NULL, TNULL(size_t));

void auth_sock_close(auth_sock *sock) {
  /* Ack the termination of the call */
  header_t header = { sock->ret };
  orwl_send_(sock->fd, header, header_t_els);
  close(sock->fd);
  sock->fd = -1;
}

void auth_sock_destroy(auth_sock *sock) {
  if (sock->fd != -1) auth_sock_close(sock);
  if (sock->back) uint64_t_vdelete(sock->back);
  auth_sock_init(sock);
}

DEFINE_NEW_DELETE(auth_sock);

DECLARE_AUTH_SOCK_FUNC(server_callback, uint64_t funcID);

DEFINE_AUTH_SOCK_FUNC(server_callback, uint64_t funcID) {
  AUTH_SOCK_READ(Arg, server_callback, uint64_t funcID);
  orwl_domain_call(ORWL_FTAB(auth_sock), funcID, Arg);
}

DEFINE_THREAD(auth_sock) {
  assert(Arg->mes);
  if (!orwl_recv_(Arg->fd, Arg->mes, Arg->len))
    if (Arg->srv) {
      /* do something with mess here */
      server_callback(Arg);
      //report(1, "finished callback with %zd elements", Arg->len);
    }
  if (Arg->fd != -1) auth_sock_close(Arg);
}




addr_t getpeer(auth_sock *Arg) {
  struct sockaddr_in addr = INITIALIZER;
  int ret = getpeername(Arg->fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)});
  return  (addr_t)ADDR_T_INITIALIZER((ret == -1) ? TNULL(in_addr_t) : addr.sin_addr.s_addr);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_peer, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_peer, uint64_t port);
  orwl_host *h = NEW(orwl_host);
  /* mes and addr_t is already in host order */
  h->ep.addr = getpeer(Arg);
  h->ep.port.p = port;
  report(1, "inserting peer %s", orwl_endpoint_print(&h->ep));
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_host, uint64_t addr, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_host, uint64_t addr, uint64_t port);
  orwl_host *h = NEW(orwl_host);
  /* mes is already in host order */
  addr_t_init(&h->ep.addr, addr);
  port_t_init(&h->ep.port, port);
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_do_nothing, void) {
  /* empty */
}

DEFINE_ORWL_TYPE_DYNAMIC(auth_sock,
                         ORWL_REGISTER_ALIAS(auth_sock_insert_peer),
                         ORWL_REGISTER_ALIAS(auth_sock_insert_host),
                         ORWL_REGISTER_ALIAS(auth_sock_do_nothing),
                         ORWL_REGISTER_ALIAS(auth_sock_request_excl),
                         ORWL_REGISTER_ALIAS(auth_sock_request_incl),
                         ORWL_REGISTER_ALIAS(auth_sock_release)
                         );

char const* hostname(char *buffer, size_t len);
