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

#include "orwl_register.h"
#include "orwl_host.h"

P99_DEFARG_DOCU(orwl_hton)
inline
void
orwl_hton(uint32_t *n,        /*!< [out] array of length 2 @a l */
          uint64_t const *h,  /*!< [in] array of length @a l */
          size_t l            /*!< [in] defaults to 1 */
          ) {
  for (size_t i = 0; i < l; ++i) {
    n[0] = htonl((uint32_t)h[0]);
    n[1] = htonl((uint32_t)(h[0] >> 32));
    h += 1;
    n += 2;
  }
}

#ifndef DOXYGEN
inline
P99_PROTOTYPE(void, orwl_hton, uint32_t *, uint64_t const *, size_t);
#define orwl_hton(...) P99_CALL_DEFARG(orwl_hton, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_hton, , , 1);
#endif

P99_DEFARG_DOCU(orwl_ntoh)
inline
void
orwl_ntoh(uint64_t* h,       /*!< [out] array of length @a l */
          uint32_t const *n, /*!< [in] array of length 2 @a l */
          size_t l           /*!< [in] defaults to 1 */
          ) {
  for (size_t i = 0; i < l; ++i) {
    h[0] = ((uint64_t)ntohl(n[0])) | (((uint64_t)ntohl(n[1])) << 32);
    n += 2;
    h += 1;
  }
}

#ifndef DOXYGEN
inline
P99_PROTOTYPE(void, orwl_ntoh, uint64_t*, uint32_t const *, size_t);
#define orwl_ntoh(...) P99_CALL_DEFARG(orwl_ntoh, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_ntoh, , , 1);
#endif

extern in_addr_t orwl_inet_addr(char const *name);

extern void orwl_ntoa(struct sockaddr_in const* addr, char *name);

#define diagnose(fd, form, ...)                                         \
do {                                                                    \
  struct sockaddr_in addr = SOCKADDR_IN_INIIALIZER;                     \
  if (getpeername(fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)}) != -1) { \
    char name[256] = "";                                                \
    orwl_ntoa(&addr, name);                                             \
    report(stderr, "connection from %s " form, name, __VA_ARGS__);      \
  }                                                                     \
 } while (0)


extern in_addr_t p99__inet4_addr;

DECLARE_ONCE_UPON(inet4_addr);


inline
in_addr_t inet4_addr(void) {
  INIT_ONCE_UPON(inet4_addr, p99__inet4_addr);
  return p99__inet4_addr;
}

P99_DEFARG_DOCU(orwl_inet_ntop)
inline
char const* orwl_inet_ntop(struct sockaddr const* addr,
                           char* buf, size_t size) {
  void const* src =
    ((addr->sa_family) == AF_INET
     ? (void const*)&(((struct sockaddr_in const*)(addr))->sin_addr)
     :  ((addr->sa_family) == AF_INET6
         ? (void const*)&(((struct sockaddr_in6 const*)(addr))->sin6_addr)
         : NULL)
     );
  if (src) inet_ntop(addr->sa_family, src, buf, size);
  else strncpy(buf, "<invalid addr>", size);
  return buf;
}

#ifndef DOXYGEN
inline
P99_PROTOTYPE(char const*, orwl_inet_ntop, struct sockaddr const*, char*, size_t);
P99_DECLARE_DEFARG(orwl_inet_ntop, , , );
#define orwl_inet_ntop(...) P99_CALL_DEFARG(orwl_inet_ntop, 3, __VA_ARGS__)
#define orwl_inet_ntop_defarg_1() ((char[INET6_ADDRSTRLEN]){ 0 })
#define orwl_inet_ntop_defarg_2() (INET6_ADDRSTRLEN)
#endif


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
};

DOCUMENT_INIT(auth_sock)
P99_DEFARG_DOCU(auth_sock_init)
inline
auth_sock*
auth_sock_init(auth_sock *sock,         /*!< [out] */
               int fd,                  /*!< [in] file descriptor, defaults to -1 */
               struct orwl_server* srv, /*!< [in,out] defaults to @c NULL */
               size_t len               /*!< [in] the length of the message 0 */
               ) {
  if (!sock) return NULL;
  P99_TZERO(*sock);
  sock->fd = fd;
  sock->srv = srv;
  sock->len = len;
  sock->mes = len ? uint64_t_vnew(len) : NULL;
  sock->back = sock->mes;
  return sock;
}

#ifndef DOXYGEN
inline
P99_PROTOTYPE(auth_sock*, auth_sock_init, auth_sock *, int, struct orwl_server*, size_t);
P99_DECLARE_DEFARG(auth_sock_init, , -1, NULL, P99_0(size_t));


#define auth_sock_init(...) P99_CALL_DEFARG(auth_sock_init, 4, __VA_ARGS__)
#endif

void auth_sock_destroy(auth_sock *sock);
DECLARE_NEW_DELETE(auth_sock);
DECLARE_THREAD(auth_sock);

void auth_sock_close(auth_sock *sock);

#ifdef DOXYGEN
#define DEFINE_AUTH_SOCK_FUNC(F, ...)                                   \
/*! An ::auth_sock function interpreting a message received on a socket. */ \
/*! It interprets the message it receives as if it where declared*/     \
/*! @code uint64_t F(__VA_ARGS__) @endcode */                           \
/*! @see AUTH_SOCK_READ is used to interpret the message as specified */ \
void F(auth_sock *Arg)
#define DECLARE_AUTH_SOCK_FUNC(F, ...) void F(auth_sock *Arg)
#else
#define DEFINE_AUTH_SOCK_FUNC(F, ...)                   \
void (*P99_PASTE2(F, _signature))(__VA_ARGS__) = NULL;  \
DEFINE_ORWL_REGISTER_ALIAS(F, auth_sock);               \
void F(auth_sock *Arg)

#define DECLARE_AUTH_SOCK_FUNC(F, ...)                  \
extern void (*P99_PASTE2(F, _signature))(__VA_ARGS__);  \
DECLARE_ORWL_REGISTER_ALIAS(F, auth_sock);              \
void F(auth_sock *Arg)
#endif

DECLARE_ORWL_TYPE_DYNAMIC(auth_sock);


#define AUTH_SOCK_READ(A, F, ...)                               \
(void)((void (*)(__VA_ARGS__)){P99_PASTE2(F, _signature)});     \
P99_VASSIGNS((A)->mes, __VA_ARGS__);                                \
(A)->len -= P99_NARG(__VA_ARGS__);                              \
(A)->mes += P99_NARG(__VA_ARGS__)

/* some helper */
addr_t getpeer(auth_sock *Arg);


P99_DEFARG_DOCU(hostname)
inline
char const*
hostname(char buffer[static 64], /*!< [out] defaults to a temporary */
         size_t len              /*!< [in] maximum length of the name (64) */
         ) {
  gethostname(buffer, len);
  return buffer;
}

#ifndef DOXYGEN
inline
P99_PROTOTYPE(char const*, hostname, char *, size_t);
P99_DECLARE_DEFARG(hostname, , );
#define hostname(...) P99_CALL_DEFARG(hostname, 2, __VA_ARGS__)
#define hostname_defarg_0() ((char[64]){ 0 })
#define hostname_defarg_1() (64)
#endif
#endif 	    /* !ORWL_SOCKET_H_ */
