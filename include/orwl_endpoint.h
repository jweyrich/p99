/*
** orwl_endpoint.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 19:52:37 2010 Jens Gustedt
** Last update Fri May  7 19:52:37 2010 Jens Gustedt
*/

#ifndef   	ORWL_ENDPOINT_H_
# define   	ORWL_ENDPOINT_H_

#include <unistd.h>
#include <arpa/inet.h>

#include "orwl_macro.h"
#include "orwl_int.h"
#include "orwl_rand.h"

struct orwl_server;

#ifndef __cplusplus
typedef struct orwl_server orwl_server;
#endif

struct auth_sock;

#ifndef __cplusplus
typedef struct auth_sock auth_sock;
#endif

typedef void (*server_cb_t)(auth_sock *);



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

#ifndef DOXYGEN
inline
PROTOTYPE(addr_t*, addr_t_init, addr_t *, in_addr_t);
#define addr_t_init(...) CALL_WITH_DEFAULTS(addr_t_init, 2, __VA_ARGS__)
DECLARE_DEFARG(addr_t_init, , TNULL(in_addr_t));
#endif

inline
in_addr_t addr2net(addr_t const*A) {
  return htonl(A->a);
}

inline
port_t* port_t_init(port_t *A, in_port_t P) {
  A->p = ntohs(P);
  return A;
}

#ifndef DOXYGEN
inline
PROTOTYPE(port_t*, port_t_init, port_t *, in_port_t);
#define port_t_init(...) CALL_WITH_DEFAULTS(port_t_init, 2, __VA_ARGS__)
DECLARE_DEFARG(port_t_init, , TNULL(in_port_t));
#endif

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


#ifndef DOXYGEN
inline
PROTOTYPE(orwl_endpoint*, orwl_endpoint_init, orwl_endpoint*, in_addr_t, in_port_t);
#define orwl_endpoint_init(...) CALL_WITH_DEFAULTS(orwl_endpoint_init, 3, __VA_ARGS__)
DECLARE_DEFARG(orwl_endpoint_init, , TNULL(in_addr_t), TNULL(in_port_t));
#endif

DECLARE_NEW_DELETE(orwl_endpoint);

orwl_endpoint* orwl_endpoint_parse(orwl_endpoint* ep, char const* name);
char const* orwl_endpoint_print(orwl_endpoint const* ep, char name[static 128]);

#ifndef DOXYGEN
PROTOTYPE(char const*, orwl_endpoint_print, orwl_endpoint const*, char*);
#define orwl_endpoint_print(...) CALL_WITH_DEFAULTS(orwl_endpoint_print, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_endpoint_print, , );
#define orwl_endpoint_print_defarg_1() ((char[128])INITIALIZER)
#endif

uint64_t orwl_send(orwl_endpoint const* ep, rand48_t *seed, uint64_t* mess, size_t len);

#define orwl_rpc(EP, SEED, F, ...)                      \
orwl_send(EP,                                           \
          SEED,                                         \
          (uint64_t[ NARG(~, __VA_ARGS__) ]){           \
            ORWL_OBJID(F),                              \
              __VA_ARGS__                               \
              },                                        \
          NARG(~,  __VA_ARGS__))




#endif 	    /* !ORWL_ENDPOINT_H_ */
