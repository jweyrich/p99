/*
** orwl_socket.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 21:46:01 2010 Jens Gustedt
** Last update Sun Mar 21 21:46:01 2010 Jens Gustedt
*/

#ifndef   	ORWL_SOCKET_H_
# define   	ORWL_SOCKET_H_

#include <unistd.h>
#include <arpa/inet.h>

#include "orwl_macro.h"
#include "orwl_int.h"
#include "orwl_enum.h"
#include "orwl_thread.h"
#include "orwl_rand.h"
#include "orwl_register.h"
#include "orwl_posix_default.h"

inline
void orwl_hton(uint32_t *n, uint64_t const *h, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    n[0] = htonl((uint32_t)h[0]);
    n[1] = htonl((uint32_t)(h[0] >> 32));
    h += 1;
    n += 2;
  }
}

#define orwl_hton(...) CALL_WITH_DEFAULTS(orwl_hton, 3, __VA_ARGS__)
declare_defarg(orwl_hton, 2, size_t, 1);


inline
void orwl_ntoh(uint64_t* h, uint32_t const *n, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    h[0] = ((uint64_t)ntohl(n[0])) | (((uint64_t)ntohl(n[1])) << 32);
    n += 2;
    h += 1;
  }
}

#define orwl_ntoh(...) CALL_WITH_DEFAULTS(orwl_ntoh, 3, __VA_ARGS__)
declare_defarg(orwl_ntoh, 2, size_t, 1);

extern in_addr_t orwl_inet_addr(char const *name);

extern void orwl_ntoa(struct sockaddr_in *addr, char *name);

#define diagnose(fd, form, ...)                                         \
do {                                                                    \
  struct sockaddr_in addr = INITIALIZER;                                \
  if (getpeername(fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)}) != -1) { \
    char name[256] = INITIALIZER;                                       \
    orwl_ntoa(&addr, name);                                             \
    report(stderr, "connection from /%s/ " form, name, __VA_ARGS__);    \
  }                                                                     \
 } while (0)


extern in_addr_t _inet4_addr;

DECLARE_ONCE_UPON(inet4_addr);


inline
in_addr_t inet4_addr(void) {
  INIT_ONCE_UPON(inet4_addr, _inet4_addr);
  return _inet4_addr;
}

struct orwl_endpoint;

struct addr_t { uint64_t a; };
struct port_t { uint64_t p; };

#ifndef __cplusplus
typedef struct orwl_endpoint orwl_endpoint;
typedef struct addr_t addr_t;
typedef struct port_t port_t;
#endif

struct orwl_endpoint {
  addr_t addr;
  port_t port;
};


#define ADDR_T_INITIALIZER(NADDR) { .a = ntohl(NADDR) }
#define PORT_T_INITIALIZER(NPORT) { .p = ntohs(NPORT) }
#define ORWL_ENDPOINT_INITIALIZER(NADDR,  NPORT) {      \
    .addr = ADDR_T_INITIALIZER(NADDR),                  \
      .port = PORT_T_INITIALIZER(NPORT),                \
      }

inline
addr_t* addr_t_init(addr_t *A, in_addr_t I) {
  A->a = ntohl(I);
  return A;
}

#define addr_t_init(...) CALL_WITH_DEFAULTS(addr_t_init, 2, __VA_ARGS__)
declare_defarg(addr_t_init, 1, in_addr_t, TNULL(in_addr_t));

inline
in_addr_t addr2net(addr_t const*A) {
  return htonl(A->a);
}

inline
port_t* port_t_init(port_t *A, in_port_t P) {
  A->p = ntohs(P);
  return A;
}

#define port_t_init(...) CALL_WITH_DEFAULTS(port_t_init, 2, __VA_ARGS__)
declare_defarg(port_t_init, 1, in_port_t, TNULL(in_port_t));

inline
in_port_t port2net(port_t const*A) {
  return htons(A->p);
}



DOCUMENT_INIT(orwl_endpoint)
FSYMB_DOCUMENTATION(orwl_endpoint_init)
inline
orwl_endpoint* orwl_endpoint_init
(orwl_endpoint *endpoint,
 in_addr_t addr,
 in_port_t port
 ) {
  addr_t_init(&endpoint->addr, addr);
  port_t_init(&endpoint->port, port);
  return endpoint;
}

DOCUMENT_DESTROY(orwl_endpoint)
inline
void orwl_endpoint_destroy(orwl_endpoint *endpoint) {
  /* empty */
}


#define orwl_endpoint_init(...) CALL_WITH_DEFAULTS(orwl_endpoint_init, 3, __VA_ARGS__)

declare_defarg(orwl_endpoint_init, 2, in_port_t, TNULL(in_port_t));
declare_defarg(orwl_endpoint_init, 1, in_addr_t, TNULL(in_addr_t));

DECLARE_NEW_DELETE(orwl_endpoint);

uint64_t orwl_send(orwl_endpoint const* ep, rand48_t *seed, uint64_t* mess, size_t len);

#define orwl_rpc(EP, SEED, F, ...)                      \
orwl_send(EP,                                           \
          SEED,                                         \
          (uint64_t[ _NARG_64(~, __VA_ARGS__) ]){       \
            ORWL_OBJID(F),                              \
              __VA_ARGS__                               \
              },                                        \
          _NARG_64(~,  __VA_ARGS__))

struct orwl_host;

typedef struct orwl_host orwl_host;

struct orwl_host {
  pthread_mutex_t mut;
  orwl_endpoint ep;
  orwl_host *prev;
  orwl_host *next;
  size_t refs;
};

#define ORWL_HOST_INITIALIZER(NAME, NADDR,  NPORT) {    \
    .mut = PTHREAD_MUTEX_INITIALIZER,                   \
    .ep = ORWL_ENDPOINT_INITIALIZER(NADDR, NPORT),      \
      .prev = &NAME,                                    \
      .next = &NAME,                                    \
      .refs = 2                                         \
      }

void orwl_host_connect(orwl_host *th, orwl_host *q);
void orwl_host_disconnect(orwl_host *th);

inline
orwl_host* orwl_host_init(orwl_host *th, in_addr_t addr, in_port_t port) {
  th->next = th;
  th->prev = th;
  th->refs = 0;
  orwl_endpoint_init(&th->ep, addr, port);
  pthread_mutex_init(&th->mut);
  return th;
}

#define orwl_host_init(...) CALL_WITH_DEFAULTS(orwl_host_init, 3, __VA_ARGS__)
declare_defarg(orwl_host_init, 2, in_port_t, 0);
declare_defarg(orwl_host_init, 1, in_addr_t, 0);


inline
void orwl_host_destroy(orwl_host *th) {
  orwl_host_disconnect(th);
}

DECLARE_NEW_DELETE(orwl_host);

struct orwl_server;

#ifndef __cplusplus
typedef struct orwl_server orwl_server;
#endif

struct auth_sock;

#ifndef __cplusplus
typedef struct auth_sock auth_sock;
#endif

typedef void (*server_cb_t)(auth_sock *);

struct auth_sock {
  orwl_server* srv;
  int fd;
  size_t len;
  uint64_t *mes;
  uint64_t *back;
  uint64_t ret;
};

inline
auth_sock* auth_sock_init(auth_sock *sock,
                                  int fd,
                                  orwl_server* srv,
                                  size_t len) {
  memset(sock, 0, sizeof(auth_sock));
  sock->fd = fd;
  sock->srv = srv;
  sock->len = len;
  sock->mes = len ? uint64_t_vnew(len) : NULL;
  sock->back = sock->mes;
  return sock;
}

declare_defarg(auth_sock_init, 3, size_t, 0);
declare_defarg(auth_sock_init, 2, orwl_server*, NULL);
declare_defarg(auth_sock_init, 1, int, -1);


#define auth_sock_init(...) CALL_WITH_DEFAULTS(auth_sock_init, 4, __VA_ARGS__)

void auth_sock_destroy(auth_sock *sock);
DECLARE_NEW_DELETE(auth_sock);
DECLARE_THREAD(auth_sock);

void auth_sock_close(auth_sock *sock);


void auth_sock_insert_peer(auth_sock *Arg);
void auth_sock_insert_host(auth_sock *Arg);
void auth_sock_do_nothing(auth_sock *Arg);

DECLARE_ORWL_TYPE_DYNAMIC(auth_sock);

DECLARE_ORWL_REGISTER(auth_sock_insert_peer);
DECLARE_ORWL_REGISTER(auth_sock_insert_host);
DECLARE_ORWL_REGISTER(auth_sock_do_nothing);

#define AUTH_SOCK_READ(A, ...)                  \
ASGS((A)->mes, __VA_ARGS__);                    \
(A)->len -= _NARG_64(__VA_ARGS__);              \
(A)->mes += _NARG_64(__VA_ARGS__)

/* some helper */
addr_t getpeer(auth_sock *Arg);


struct orwl_server {
  int fd_listen;
  orwl_host host;
  unsigned max_connections;
  server_cb_t const cb;
};

#define ORWL_SERVER_INITIALIZER(NAME, CB, MAXC, ADDR, PORT)     \
{                                                               \
  .fd_listen = -1,                                              \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT),         \
  .cb = CB,                                                     \
  .max_connections = MAXC                                       \
}

inline
orwl_server* orwl_server_init(orwl_server *serv) {
  memset(serv, 0, sizeof(orwl_server));
  serv->fd_listen = -1;
  orwl_host_init(&serv->host);
  serv->host.refs = 1;
  return serv;
}

void orwl_server_destroy(orwl_server *serv);

DECLARE_NEW_DELETE(orwl_server);

DECLARE_THREAD(orwl_server);

void orwl_server_close(orwl_server *serv);

void orwl_server_terminate(orwl_server *serv, rand48_t *seed);
declare_defarg(orwl_server_terminate, 1, rand48_t*, &(rand48_t)RAND48_T_INITIALIZER);
#define orwl_server_terminate(...) CALL_WITH_DEFAULTS(orwl_server_terminate, 2, __VA_ARGS__)


#endif 	    /* !ORWL_SOCKET_H_ */
