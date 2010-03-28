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

inline
void FUNC_DEFAULT(orwl_hton)(uint32_t *n, uint64_t const *h, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    n[0] = htonl((uint32_t)h[0]);
    n[1] = htonl((uint32_t)(h[0] >> 32));
    h += 1;
    n += 2;
  }
}

#define orwl_hton(...) DEFINE_FUNC_DEFAULT(orwl_hton, 3, __VA_ARGS__)
declare_default_arg(orwl_hton, 2, size_t, 1);


inline
void FUNC_DEFAULT(orwl_ntoh)(uint64_t* h, uint32_t const *n, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    h[0] = ((uint64_t)ntohl(n[0])) | (((uint64_t)ntohl(n[1])) << 32);
    n += 2;
    h += 1;
  }
}

#define orwl_ntoh(...) DEFINE_FUNC_DEFAULT(orwl_ntoh, 3, __VA_ARGS__)
declare_default_arg(orwl_ntoh, 2, size_t, 1);

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

#ifndef __cplusplus
typedef struct orwl_endpoint orwl_endpoint;
#endif

struct orwl_endpoint {
  uint64_t addr;
  uint64_t port;
};

DOCUMENT_INIT(orwl_endpoint)
FUNC_DEFAULT_DOCUMENTATION(orwl_endpoint_init)
inline
void FUNC_DEFAULT(orwl_endpoint_init)
(orwl_endpoint *endpoint,
 in_addr_t addr,
 in_port_t port
 ) {
  orwl_endpoint const ep = { addr, port };
  memcpy(endpoint, &ep, sizeof(orwl_endpoint));
}

DOCUMENT_DESTROY(orwl_endpoint)
inline
void orwl_endpoint_destroy(orwl_endpoint *endpoint) {
  /* empty */
}


#define orwl_endpoint_init(...) DEFINE_FUNC_DEFAULT(orwl_endpoint_init, 3, __VA_ARGS__)

declare_default_arg(orwl_endpoint_init, 2, in_port_t, TNULL(in_port_t));
declare_default_arg(orwl_endpoint_init, 1, in_addr_t, TNULL(in_addr_t));

DECLARE_NEW_DELETE(orwl_endpoint);

bool orwl_send(orwl_endpoint const* ep, rand48_t seed, uint64_t const* mess, size_t len);


struct orwl_host;

typedef struct orwl_host orwl_host;

struct orwl_host {
  pthread_mutex_t mut;
  orwl_endpoint ep;
  orwl_host *prev;
  orwl_host *next;
  size_t refs;
};

#define ORWL_HOST_INITIALIZER(NAME, ADDR, PORT) { \
  .mut = PTHREAD_MUTEX_INITIALIZER,             \
      .ep = { .addr = ADDR, .port = PORT},        \
  .prev = &NAME,                                \
  .next = &NAME,                                \
  .refs = 2                                     \
}

void orwl_host_connect(orwl_host *th, orwl_host *q);
void orwl_host_disconnect(orwl_host *th);

inline
void orwl_host_init(orwl_host *th) {
  th->next = th;
  th->prev = th;
  th->refs = 0;
  pthread_mutex_init(&th->mut);
}

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
};

inline
void FUNC_DEFAULT(auth_sock_init)(auth_sock *sock,
                                  int fd,
                                  orwl_server* srv,
                                  size_t len) {
  memset(sock, 0, sizeof(auth_sock));
  sock->fd = fd;
  sock->srv = srv;
  sock->len = len;
  if (sock->mes) uint64_t_vdelete(sock->mes);
  sock->mes = len ? uint64_t_vnew(len) : NULL;
}

declare_default_arg(auth_sock_init, 3, size_t, 0);
declare_default_arg(auth_sock_init, 2, orwl_server*, NULL);
declare_default_arg(auth_sock_init, 1, int, -1);


#define auth_sock_init(...) DEFINE_FUNC_DEFAULT(auth_sock_init, 4, __VA_ARGS__);

void auth_sock_destroy(auth_sock *sock);
DECLARE_NEW_DELETE(auth_sock);
DECLARE_THREAD(auth_sock);

struct orwl_server {
  orwl_host host;
  unsigned max_connections;
  server_cb_t const cb;
};

#define ORWL_SERVER_INITIALIZER(NAME, CB, MAXC, ADDR, PORT)     \
{                                                               \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT),         \
  .cb = CB,                                                     \
  .max_connections = MAXC                                       \
}

void orwl_server_init(orwl_server *serv) {
  memset(serv, 0, sizeof(orwl_server));
  orwl_host_init(&serv->host);
  serv->host.refs = 1;
}

void orwl_server_destroy(orwl_server *serv) {
  /* empty */
}

DECLARE_NEW_DELETE(orwl_server);
DEFINE_NEW_DELETE(orwl_server);

DECLARE_THREAD(orwl_server);

#endif 	    /* !ORWL_SOCKET_H_ */
