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
#ifndef   	ORWL_SOCKET_H_
# define   	ORWL_SOCKET_H_

#include "orwl_register.h"
#include "orwl_host.h"
#include "orwl_header.h"

#ifndef DOXYGEN
static_inline
P99_PROTOTYPE(void, orwl_hton, uint64_t *, uint64_t const *, size_t);
#define orwl_hton(...) P99_CALL_DEFARG(orwl_hton, 3, __VA_ARGS__)
#define orwl_hton_defarg_2() 1
#endif

P99_DEFARG_DOCU(orwl_hton)
static_inline
void
orwl_hton(uint64_t *n,        /*!< [out] array of length @a l */
          uint64_t const *h,  /*!< [in] array of length @a l */
          size_t l            /*!< [in] defaults to 1 */
          ) {
  uint64_t const* hv = h ? h : n;
  for (size_t i = 0; i < l; ++i) {
    n[i] = orwl_hton64(hv[i]);
  }
}

#ifndef DOXYGEN
static_inline
P99_PROTOTYPE(void, orwl_ntoh, uint64_t*, uint64_t const *, size_t);
#define orwl_ntoh(...) P99_CALL_DEFARG(orwl_ntoh, 3, __VA_ARGS__)
#define orwl_ntoh_defarg_2() 1
#endif

P99_DEFARG_DOCU(orwl_ntoh)
static_inline
void
orwl_ntoh(uint64_t* h,       /*!< [out] array of length @a l */
          uint64_t const *n, /*!< [in] array of length @a l */
          size_t l           /*!< [in] defaults to 1 */
          ) {
  uint64_t const *nv = n ? n : h;
  for (size_t i = 0; i < l; ++i) {
    h[i] = orwl_ntoh64(nv[i]);
  }
}

extern in_addr_t orwl_inet_addr(char const *name);

extern void orwl_ntoa(struct sockaddr_in const* addr, char *name);

#define diagnose(fd, form, ...)                                                                   \
do {                                                                                              \
  struct sockaddr_in addr = SOCKADDR_IN_INIIALIZER;                                               \
  if (getpeername(fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)}) != -1) { \
    char name[256] = "";                                                                          \
    orwl_ntoa(&addr, name);                                                                       \
    report(stderr, "connection from %s " form, name, __VA_ARGS__);                                \
  }                                                                                               \
 } while (0)


extern in_addr_t p00_inet4_addr;

DECLARE_ONCE_UPON(inet4_addr);

in_addr_t inet4_addr(void);

#ifndef DOXYGEN
static_inline
P99_PROTOTYPE(char const*, orwl_inet_ntop, struct sockaddr const*, char*, size_t);
P99_DECLARE_DEFARG(orwl_inet_ntop, , , );
#define orwl_inet_ntop(...) P99_CALL_DEFARG(orwl_inet_ntop, 3, __VA_ARGS__)
#define orwl_inet_ntop_defarg_1() ((char[INET6_ADDRSTRLEN]){ 0 })
#define orwl_inet_ntop_defarg_2() (INET6_ADDRSTRLEN)
#endif


P99_DEFARG_DOCU(orwl_inet_ntop)
static_inline
char const* orwl_inet_ntop(struct sockaddr const* addr,
                           char* buf, size_t size) {
  void const* src =
    ((addr->sa_family) == AF_INET
     ? (void const*)&(((struct sockaddr_in const*)(addr))->sin_addr)
     :  ((addr->sa_family) == AF_INET6
         ? (void const*)&(((struct sockaddr_in6 const*)(addr))->sin6_addr)
         : (void const*)0)
     );
  if (src) inet_ntop(addr->sa_family, src, buf, size);
  else strncpy(buf, "<invalid addr>", size);
  return buf;
}

/**
 ** @brief Negotiate a send request with remote endpoint @a ep.
 **/
uint64_t orwl_send(orwl_endpoint const* ep, rand48_t *seed, size_t len, uint64_t*const mess);

/**
 ** @brief An open socket through which we may receive some
 ** authenticated data and with which we may launch callbacks.
 **
 ** @see orwl_server
 ** @see orwl_rpc
 ** @see auth_sock_init
 ** @see auth_sock_destroy
 ** @see auth_sock_close
 ** @see auth_sock_create
 ** @see auth_sock_join
 ** @see auth_sock_insert_peer
 ** @see auth_sock_insert_host
 **/
struct auth_sock {
  struct orwl_server* srv; /*!< the server through which we received
                             this socket */
  int fd;                  /*!< the open file descriptor */
  size_t len;              /*!< the length of the message */
  uint64_t *mes;           /*!< the message itself */
  uint64_t const* back;    /*!< a backup of the message */
  uint64_t ret;            /*!< a place to store the return value of
                             the call */
  uint64_t remoteorder;   /*!< the byte order on the remote host */
};

#ifndef DOXYGEN
static_inline
P99_PROTOTYPE(auth_sock*, auth_sock_init, auth_sock *, int, struct orwl_server*, size_t, uint64_t);
#define auth_sock_init(...) P99_CALL_DEFARG(auth_sock_init, 5, __VA_ARGS__)
#define auth_sock_init_defarg_1() -1
#define auth_sock_init_defarg_2() P99_0(struct orwl_server*)
#define auth_sock_init_defarg_3() P99_0(size_t)
#define auth_sock_init_defarg_4() P99_0(uint64_t)
#endif

DOCUMENT_INIT(auth_sock)
P99_DEFARG_DOCU(auth_sock_init)
static_inline
auth_sock*
auth_sock_init(auth_sock *sock,         /*!< [out] */
               int fd,                  /*!< [in] file descriptor, defaults to -1 */
               struct orwl_server* srv, /*!< [in,out] defaults to a null pointer */
               size_t len,              /*!< [in] the length of the message 0 */
               uint64_t remo            /*!< [in] the byte order on remote */
               ) {
  if (!sock) return 0;
  P99_TZERO(*sock);
  sock->fd = fd;
  sock->srv = srv;
  sock->len = len;
  sock->remoteorder = remo;
  sock->mes = len ? uint64_t_vnew(len) : P99_0(uint64_t*);
  sock->back = sock->mes;
  return sock;
}

DOCUMENT_DESTROY(auth_sock)
void auth_sock_destroy(auth_sock *sock);
DECLARE_NEW_DELETE(auth_sock);
DECLARE_THREAD(auth_sock);

#define auth_sock_create(...) P99_CALL_DEFARG(auth_sock_create, 2, __VA_ARGS__)
#define auth_sock_create_defarg_1() P99_0(pthread_t*)

/**
 ** @memberof auth_sock
 **/
void auth_sock_close(auth_sock *sock);

#ifdef DOXYGEN
#define DEFINE_AUTH_SOCK_FUNC(F, ...)                                       \
/*! An ::auth_sock function interpreting a message received on a socket. */ \
/*! It interprets the message it receives as if it where declared*/         \
/*! @code uint64_t F(__VA_ARGS__) @endcode */                               \
/*! @see AUTH_SOCK_READ is used to interpret the message as specified */    \
/*! @memberof auth_sock */                                                  \
void F(auth_sock *Arg)
#define DECLARE_AUTH_SOCK_FUNC(F, ...) void F(auth_sock *Arg)
#else
#define DEFINE_AUTH_SOCK_FUNC(F, ...)                          \
DEFINE_ORWL_REGISTER_ALIAS(F, auth_sock);                      \
void F(auth_sock *Arg)

#define DECLARE_AUTH_SOCK_FUNC(F, ...)                         \
void F(auth_sock *Arg);                                        \
DECLARE_ORWL_REGISTER_ALIAS(F, auth_sock)
#endif

DECLARE_ORWL_TYPE_DYNAMIC(auth_sock);


#define AUTH_SOCK_READ(A, F, ...)                              \
(void)((void (*)(auth_sock*)){ F });                           \
P99_VASSIGNS((A)->mes, __VA_ARGS__);                           \
(A)->len -= P99_NARG(__VA_ARGS__);                             \
(A)->mes += P99_NARG(__VA_ARGS__)

/* some helper */
addr_t getpeer(auth_sock *Arg);


#ifndef DOXYGEN
#define hostname(...) P99_CALL_DEFARG(hostname, 2, __VA_ARGS__)
#define hostname_defarg_0() ((char[HOST_NAME_MAX]){ 0 })
#define hostname_defarg_1() HOST_NAME_MAX
#endif

/**
 ** @brief Obtain the %hostname of the execution host
 **
 ** This is a wrapper to the POSIX function @c gethostname,
 ** the difference is the signature.
 ** @return @a buffer if the call to @c gethostname was successful, a
 ** null pointer otherwise.
 **/
P99_DEFARG_DOCU(hostname)
static_inline
char const*
hostname(char buffer[],   /*!< [out] defaults to a temporary */
         size_t len       /*!< [in] maximum length of the name (HOST_NAME_MAX) */
         ) {
  return
    gethostname(buffer, len)
    ? P99_0(char*)
    : buffer;
}

#endif 	    /* !ORWL_SOCKET_H_ */
