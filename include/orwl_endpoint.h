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
#ifndef   	ORWL_ENDPOINT_H_
# define   	ORWL_ENDPOINT_H_

#include "orwl_rand.h"

P99_DECLARE_STRUCT(orwl_server);
P99_DECLARE_STRUCT(auth_sock);
P99_DECLARE_STRUCT(orwl_mirror);
P99_DECLARE_STRUCT(orwl_wq);
P99_DECLARE_STRUCT(orwl_endpoint);
P99_DECLARE_STRUCT(orwl_host);
P99_DECLARE_UNION(addr_t);
P99_DECLARE_UNION(port_t);
P99_DECLARE_STRUCT(orwl_wh);
P99_DECLARE_STRUCT(orwl_handle2);
P99_DECLARE_STRUCT(orwl_handle);

/**
 ** @brief Store IPv4 and IPv6 addresses in the same structure.
 **
 ** In case this is an IPv4 address the significant part is stored in
 ** word @c a[3]. @c a[0] and @c a[1] then hold the value 0 and @c a[2] holds
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

/**
 ** @brief Represent a remote ORWL location.
 **/
struct orwl_endpoint {
  port_t port;
  uint64_t index;
  addr_t addr;
};

/**
 ** @memberof addr_t
 **/
#define ADDR_T_INITIALIZER(NADDR) { .a[2] = htonl(0x0000FFFF), .a[3] = NADDR }
/**
 ** @memberof port_t
 **/
#define PORT_T_INITIALIZER(NPORT) { .p = NPORT }
/**
 ** @memberof orwl_endpoint
 **/
#define ORWL_ENDPOINT_INITIALIZER(NADDR,  NPORT) {             \
    .addr = ADDR_T_INITIALIZER(NADDR),                         \
      .port = PORT_T_INITIALIZER(NPORT),                       \
      }

#ifndef DOXYGEN
inline
P99_PROTOTYPE(addr_t*, addr_t_init, addr_t *, in_addr_t);
#define addr_t_init(...) P99_CALL_DEFARG(addr_t_init, 2, __VA_ARGS__)
#define addr_t_init_defarg_1() P99_0(in_addr_t)
#endif

DOCUMENT_INIT(addr_t)
P99_DEFARG_DOCU(addr_t)
inline
addr_t* addr_t_init(addr_t *A,  /*!< the object to initialize */
                    in_addr_t I0 /*!< defaults to the null address */
                    ) {
  if (!A) return 0;
  A->a[0] = P99_0(in_addr_t);
  A->a[1] = P99_0(in_addr_t);
  A->a[2] = htonl(0x0000FFFF);
  A->a[3] = I0;
  return A;
}

/**
 ** @brief Return the IPv4 address stored in @a A.
 **
 ** If this is not an IPv4 address return all bit ones.
 **
 ** @memberof addr_t
 **/
inline
struct in_addr addr2net(addr_t const*A) {
  struct in_addr ret = {
    .s_addr = ((!A->a[0]
                && !A->a[1]
                && (ntohl(A->a[2]) == 0x0000FFFF))
               ? A->a[3]
               : P99_TMAX(in_addr_t))
  };
  return ret;
}

#if (POSIX_IPV6 > 0) || DOXYGEN
/**
 ** @brief Return the IPv6 address stored in @a A.
 **
 ** This is only present if the platform supports IPv6. 
 ** @memberof addr_t
 **/
inline
struct in6_addr addr2net6(addr_t const*A) {
  struct in6_addr ret = { .s6_addr[0] = 0 };
  memcpy(ret.s6_addr, A->aaaa, 16);
  return ret;
}
#endif

/**
 ** @memberof port_t
 **/
inline
in_port_t port2net(port_t const*A) {
  return A->p;
}

/**
 ** @memberof port_t
 **/
inline
uint64_t port2host(port_t const*A) {
  return ntohs((uint16_t)(A->p));
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

#ifndef DOXYGEN
inline
P99_PROTOTYPE(port_t*, port_t_init, port_t *, in_port_t);
#define port_t_init(...) P99_CALL_DEFARG(port_t_init, 2, __VA_ARGS__)
#define port_t_init_defarg_1() P99_0(in_port_t)
#endif

DOCUMENT_INIT(port_t)
P99_DEFARG_DOCU(port_t)
inline
port_t* port_t_init(port_t *A,   /*!< the object to initialize */
                    in_port_t P  /*!< defaults to 0 */
                    ) {
  if (!A) return 0;
  *A = net2port(P);
  return A;
}

/**
 ** @memberof port_t
 **/
inline
bool port_t_eq(port_t const* A, port_t const* B) {
  return A ? (B ? (A->p == B->p) : false) : !B;
}

/**
 ** @memberof addr_t
 **/
inline
bool addr_t_eq(addr_t const* A, addr_t const* B) {
  return A ? (B ? !memcmp(A, B, sizeof(*A)) : false) : !B;
}

/**
 ** @memberof orwl_endpoint
 **/
inline
bool orwl_endpoint_similar(orwl_endpoint const* A, orwl_endpoint const* B) {
  return A ? (B ? (port_t_eq(&A->port, &B->port) && addr_t_eq(&A->addr, &B->addr)) : false) : !B;
}

/**
 ** @memberof orwl_endpoint
 **/
inline
bool orwl_endpoint_eq(orwl_endpoint const* A, orwl_endpoint const* B) {
  return
    orwl_endpoint_similar(A, B)
    && &A->index == &B->index;
}


#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_endpoint*, orwl_endpoint_init, orwl_endpoint*, in_addr_t, in_port_t, uint64_t);
#define orwl_endpoint_init(...) P99_CALL_DEFARG(orwl_endpoint_init, 4, __VA_ARGS__)
#define orwl_endpoint_init_defarg_1() P99_0(in_addr_t)
#define orwl_endpoint_init_defarg_2() P99_0(in_port_t)
#define orwl_endpoint_init_defarg_3() P99_0(uint64_t)
#endif

DOCUMENT_INIT(orwl_endpoint)
P99_DEFARG_DOCU(orwl_endpoint_init)
inline
orwl_endpoint* orwl_endpoint_init
(orwl_endpoint *endpoint, /*!< the object to initialize */
 in_addr_t addr,          /*!< defaults to the null address */
 in_port_t port,          /*!< defaults to 0 */
 uint64_t index           /*!< defaults to 0 */
 ) {
  if (!endpoint) return 0;
  addr_t_init(&endpoint->addr, addr);
  port_t_init(&endpoint->port, port);
  endpoint->index = index;
  return endpoint;
}

DOCUMENT_DESTROY(orwl_endpoint)
inline
void orwl_endpoint_destroy(orwl_endpoint *endpoint) {
  P99_TZERO(*endpoint);
  endpoint->index = P99_TMAX(uint64_t);
}


DECLARE_NEW_DELETE(orwl_endpoint);

orwl_endpoint* orwl_endpoint_parse(orwl_endpoint* ep, /*!< [out] the object to initialize */
                                   char const* name   /*!< [in] the string to parse */
                                   );

inline
orwl_endpoint orwl_endpoint_get(char const* name   /*!< [in] the string to parse */
                                ) {
  orwl_endpoint ret = P99_INIT;
  orwl_endpoint_parse(&ret, name);
  return ret;
}

P99_DEFARG_DOCU(orwl_endpoint_print)
char const* orwl_endpoint_print(orwl_endpoint const* ep, /*!< [in] the object to interpret */
                                char name[static 128]    /*!< [out] the string to initialize */
                                );

#ifndef DOXYGEN
P99_PROTOTYPE(char const*, orwl_endpoint_print, orwl_endpoint const*, char*);
#define orwl_endpoint_print(...) P99_CALL_DEFARG(orwl_endpoint_print, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_endpoint_print, , );
#define orwl_endpoint_print_defarg_1() P99_LVAL(char[128])
#endif

bool orwl_send_(int fd, uint64_t const*mess, size_t len, uint64_t remo);
bool orwl_recv_(int fd, uint64_t *mess, size_t len, uint64_t remo);

/**
 ** @brief Error value returned by ::orwl_send.
 **/
#define ORWL_SEND_ERROR UINT64_MAX

/**
 ** @memberof orwl_endpoint
 **/
uint64_t orwl_send(orwl_server* srv, orwl_endpoint const* there, rand48_t *seed, size_t len, uint64_t*const mess);

/**
 ** @brief Lauch a remote procedure call with function @a F.
 **
 ** @msc
 **   caller,main,server,thread,procedure;
 **   main -> server [label="orwl_server_create()", URL="\ref orwl_server_create()"];
 **   caller -> server [label="orwl_send(F, ...)", URL="\ref orwl_send()"];
 **   server->thread [label="auth_sock_create(F, ...)", URL="\ref auth_sock_create()"];
 **   thread->procedure [label="F(...)"];
 **   procedure->caller [label="auth_sock_close()", URL="\ref auth_sock_close()"];
 **   procedure->thread [label="\c return"];
 **   thread->main [label="pthread_exit()"];
 ** @endmsc
 **/
#define orwl_rpc(SRV, THERE, SEED, F, ...)                                   \
orwl_send(SRV, THERE, SEED, P99_LENGTH_ARR_ARG(uint64_t, ORWL_OBJID(F), __VA_ARGS__))

#endif 	    /* !ORWL_ENDPOINT_H_ */
