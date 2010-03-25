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


extern in_addr_t _inet4_addr;

DECLARE_ONCE_UPON(inet4_addr, _inet4_addr);


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


struct orwl_server;

#ifndef __cplusplus
typedef struct orwl_server orwl_server;
#endif

typedef void (*server_cb_t)(uint64_t const* mes, size_t n);

struct orwl_server {
  orwl_endpoint ep;
  unsigned max_connections;
  server_cb_t const cb;
};


void orwl_server_init(orwl_server *serv) {
  memset(serv, 0, sizeof(orwl_server));
}

void orwl_server_destroy(orwl_server *serv) {
  /* empty */
}

DECLARE_NEW_DELETE(orwl_server);
DEFINE_NEW_DELETE(orwl_server);

DECLARE_THREAD(orwl_server);

#endif 	    /* !ORWL_SOCKET_H_ */
