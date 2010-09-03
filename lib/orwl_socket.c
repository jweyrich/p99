/*
** orwl_socket.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 22:00:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_socket.h"

#include "orwl_server.h"
#include "orwl_header.h"
#include "orwl_posix.h"
#include "p99_posix_default.h"

static uint32_t mycode = 0;

#define if_code(C0, C1, C2, C3) ((((uint32_t)C0) << 0) | (((uint32_t)C1) << 8) | (((uint32_t)C2) << 16) | (((uint32_t)C3) << 24))

#define orwl_if_code(C0, C1, C2, C3)                    \
do {                                                    \
  static uint32_t const c = if_code(C0, C1, C2, C3);    \
  if (ntohl(c) == if_code(0, 1, 2, 3)) {                \
    mycode = c;                                         \
    goto END;                                           \
  }                                                     \
 } while (false)


DECLARE_ONCE_UPON(mycode);
DEFINE_ONCE_UPON(mycode) {
    orwl_if_code(0, 1, 2, 3);
    orwl_if_code(0, 1, 3, 2);
    orwl_if_code(0, 2, 1, 3);
    orwl_if_code(0, 2, 3, 1);
    orwl_if_code(0, 3, 1, 2);
    orwl_if_code(0, 3, 2, 1);

    orwl_if_code(1, 0, 2, 3);
    orwl_if_code(1, 0, 3, 2);
    orwl_if_code(1, 2, 0, 3);
    orwl_if_code(1, 2, 3, 0);
    orwl_if_code(1, 3, 0, 2);
    orwl_if_code(1, 3, 2, 0);

    orwl_if_code(2, 0, 1, 3);
    orwl_if_code(2, 0, 3, 1);
    orwl_if_code(2, 1, 0, 3);
    orwl_if_code(2, 1, 3, 0);
    orwl_if_code(2, 3, 0, 1);
    orwl_if_code(2, 3, 1, 0);

    orwl_if_code(3, 0, 1, 2);
    orwl_if_code(3, 0, 2, 1);
    orwl_if_code(3, 1, 0, 2);
    orwl_if_code(3, 1, 2, 0);
    orwl_if_code(3, 2, 0, 1);
    orwl_if_code(3, 2, 1, 0);
 END:;
}

bool same_endianess(uint32_t c) {
  INIT_ONCE_UPON(mycode, mycode);
  return c == mycode;
}

void orwl_hton(uint32_t *n, uint64_t const *h, size_t l);
P99_DEFINE_DEFARG(orwl_hton, , , 1);
void orwl_ntoh(uint64_t* h, uint32_t const *n, size_t l);
P99_DEFINE_DEFARG(orwl_ntoh, , , 1);


in_addr_t p99__inet4_addr = P99_0(in_addr_t);

DEFINE_ONCE_UPON(inet4_addr) {
  char const* str = getenv("INET4");
  struct in_addr inaddr = IN_ADDR_INITIALIZER;
  if (inet_aton(str, &inaddr)) {
    p99__inet4_addr = inaddr.s_addr;
  }
}

in_addr_t inet4_addr(void);

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
P99_DEFINE_DEFARG(orwl_inet_ntop, , , );

in_addr_t orwl_inet_addr(char const *name) {
  in_addr_t ret = P99_0(in_addr_t);
  struct addrinfo *res = NULL;
  struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_flags = AI_V4MAPPED | AI_ADDRCONFIG | AI_CANONNAME | (name ? 0 : AI_PASSIVE),
    .ai_socktype = SOCK_STREAM,
  };
  getaddrinfo(name, NULL, &hints, &res);
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

auth_sock* auth_sock_init(auth_sock *sock,
                                  int fd,
                                  orwl_server* srv,
                                  size_t len);

P99_DEFINE_DEFARG(auth_sock_init, , -1, NULL, P99_0(size_t));

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
    }
  if (Arg->fd != -1) auth_sock_close(Arg);
}




addr_t getpeer(auth_sock *Arg) {
  struct sockaddr_in addr = SOCKADDR_IN_INIIALIZER;
  int ret = getpeername(Arg->fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)});
  return  (addr_t)ADDR_T_INITIALIZER((ret == -1) ? P99_0(in_addr_t) : addr.sin_addr.s_addr);
}


char const* hostname(char *buffer, size_t len);
