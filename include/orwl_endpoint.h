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

struct orwl_rq;

#ifndef __cplusplus
typedef struct orwl_rq orwl_rq;
#endif

struct orwl_wq;

struct orwl_endpoint;

/**
 ** @brief Store IPv4 and IPv6 addresses in the same structure.
 **
 ** In case this is an IPv4 address the significant part is stored in
 ** word @c a[3], @c a[0] and @c a[1] then hold 0 and @c a[2] holds
 ** the value @c 0x0000FFFF.
 **
 ** This should be used as an opaque type, don't use network addresses
 ** as numbers or so.
 **/
union addr_t {
  in_addr_t a[4];
  uint8_t aaaa[16];
};

/**
 ** @brief Store a network port number in an opaque type.
 **
 ** This should be used as an opaque type, don't see port `ids' as
 ** numbers or so.
 **/
union port_t {
  in_port_t p;
  uint8_t pp[2];
};

#ifndef __cplusplus
typedef struct orwl_endpoint orwl_endpoint;
typedef union addr_t addr_t;
typedef union port_t port_t;
#endif

struct orwl_endpoint {
  port_t port;
  addr_t addr;
};


#define ADDR_T_INITIALIZER(NADDR) { .a[2] = htonl(0x0000FFFF), .a[3] = NADDR }
#define PORT_T_INITIALIZER(NPORT) { .p = NPORT }
#define ORWL_ENDPOINT_INITIALIZER(NADDR,  NPORT) {      \
    .addr = ADDR_T_INITIALIZER(NADDR),                  \
      .port = PORT_T_INITIALIZER(NPORT),                \
      }

inline
addr_t* addr_t_init(addr_t *A, in_addr_t I) {
  A->a[0] = TNULL(in_addr_t);
  A->a[1] = TNULL(in_addr_t);
  A->a[2] = htonl(0x0000FFFF);
  A->a[3] = I;
  return A;
}

#ifndef DOXYGEN
inline
PROTOTYPE(addr_t*, addr_t_init, addr_t *, in_addr_t);
#define addr_t_init(...) CALL_WITH_DEFAULTS(addr_t_init, 2, __VA_ARGS__)
DECLARE_DEFARG(addr_t_init, , TNULL(in_addr_t));
#endif

/**
 ** @brief Return the IPv4 address stored in @a A.
 **
 ** If this is not an IPv4 address return all bit ones.
 **/
inline
struct in_addr addr2net(addr_t const*A) {
  struct in_addr ret = {
    .s_addr = ((!A->a[0]
                && !A->a[1]
                && (ntohl(A->a[2]) == 0x0000FFFF))
               ? A->a[3]
               : TONES(in_addr_t))
  };
  return ret;
}

inline
struct in6_addr addr2net6(addr_t const*A) {
  struct in6_addr ret = { .s6_addr[0] = 0 };
  memcpy(ret.s6_addr, A->aaaa, 16);
  return ret;
}

inline
in_port_t port2net(port_t const*A) {
  return A->p;
}

inline
uint64_t port2host(port_t const*A) {
  return ntohs(A->p);
}

inline
port_t net2port(in_port_t P) {
  port_t ret = { .p = P };
  return ret;
}

inline
port_t host2port(uint64_t A) {
  port_t ret = { .p = htons(A) };
  return ret;
}

inline
port_t* port_t_init(port_t *A, in_port_t P) {
  *A = net2port(P);
  return A;
}

#ifndef DOXYGEN
inline
PROTOTYPE(port_t*, port_t_init, port_t *, in_port_t);
#define port_t_init(...) CALL_WITH_DEFAULTS(port_t_init, 2, __VA_ARGS__)
DECLARE_DEFARG(port_t_init, , TNULL(in_port_t));
#endif

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

bool orwl_send_(int fd, uint64_t const*mess, size_t len);
bool orwl_recv_(int fd, uint64_t *mess, size_t len);

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
