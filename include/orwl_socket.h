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
#ifndef   	ORWL_SOCKET_H_
# define   	ORWL_SOCKET_H_

#include "orwl_register.h"
#include "orwl_host.h"
#include "orwl_header.h"

#ifndef DOXYGEN
inline
P99_PROTOTYPE(void, orwl_hton, uint64_t *, uint64_t const *, size_t);
#define orwl_hton(...) P99_CALL_DEFARG(orwl_hton, 3, __VA_ARGS__)
#define orwl_hton_defarg_2() 1
#endif

#ifndef HOST_NAME_MAX
# ifdef POSIX_HOST_NAME_MAX
#  define HOST_NAME_MAX POSIX_HOST_NAME_MAX
# endif
#endif
P99_DEFARG_DOCU(orwl_hton)
inline
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
inline
P99_PROTOTYPE(void, orwl_ntoh, uint64_t*, uint64_t const *, size_t);
#define orwl_ntoh(...) P99_CALL_DEFARG(orwl_ntoh, 3, __VA_ARGS__)
#define orwl_ntoh_defarg_2() 1
#endif

P99_DEFARG_DOCU(orwl_ntoh)
inline
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


#ifndef DOXYGEN
inline
P99_PROTOTYPE(char const*, orwl_inet_ntop, struct sockaddr const*, char*, size_t);
P99_DECLARE_DEFARG(orwl_inet_ntop, , , );
#define orwl_inet_ntop(...) P99_CALL_DEFARG(orwl_inet_ntop, 3, __VA_ARGS__)
#define orwl_inet_ntop_defarg_1() ((char[INET6_ADDRSTRLEN]){ 0 })
#define orwl_inet_ntop_defarg_2() (INET6_ADDRSTRLEN)
#endif


P99_DEFARG_DOCU(orwl_inet_ntop)
inline
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
uint64_t orwl_send(orwl_server *srv, orwl_endpoint const* there, rand48_t *seed, size_t len, uint64_t*const mess);

/**
 ** @brief Data for a procedure call
 **
 ** These can either be remote, with data that is authenticated
 ** through the ::orwl_server mechanism. Or they are called locally
 ** when during the processing of the call request that possibility is
 ** detected.
 **
 ** @see orwl_server
 ** @see orwl_rpc
 ** @see orwl_proc_init
 ** @see orwl_proc_destroy
 ** @see orwl_proc_untie_caller
 ** @see orwl_proc_create
 ** @see orwl_proc_join
 ** @see orwl_proc_insert_peer
 ** @see orwl_proc_insert_host
 **/
struct orwl_proc {
  uint64_t ret;            /*!< a place to store the return value of
                             the call */
  /* the message */
  size_t len;              /*!< the length of the message */
  uint64_t *mes;           /*!< the message itself */
  uint64_t const* back;    /*!< a backup of the message */
  /* internal control fields */
  struct orwl_server* srv; /*!< the server through which we received this socket */
  bool is_untied;          /*!< orwl_proc_untie_caller has been called once */
  orwl_thread_cntrl* det;  /*!< non-null if we are local */
  int fd;                  /*!< the open file descriptor */
  uint64_t remoteorder;    /*!< the byte order on the remote host */
};

DECLARE_ONCE(orwl_proc);

#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_proc*, orwl_proc_init, orwl_proc *, int, struct orwl_server*, size_t, uint64_t, uint64_t*, orwl_thread_cntrl*);
#define orwl_proc_init(...) P99_CALL_DEFARG(orwl_proc_init, 7, __VA_ARGS__)
#define orwl_proc_init_defarg_1() -1
#define orwl_proc_init_defarg_2() P99_0(struct orwl_server*)
#define orwl_proc_init_defarg_3() P99_0(size_t)
#define orwl_proc_init_defarg_4() P99_0(uint64_t)
#define orwl_proc_init_defarg_5() P99_0(uint64_t*)
#define orwl_proc_init_defarg_6() P99_0(orwl_thread_cntrl*)
#endif

DOCUMENT_INIT(orwl_proc)
P99_DEFARG_DOCU(orwl_proc_init)
inline
orwl_proc*
orwl_proc_init(orwl_proc *sock,         /*!< [out] */
               int fd,                  /*!< [in] file descriptor, defaults to -1 */
               struct orwl_server* srv, /*!< [in,out] defaults to a null pointer */
               size_t len,              /*!< [in] the length of the message 0 */
               uint64_t remo,           /*!< [in] the byte order on remote */
               uint64_t *m,             /*!< [in] the message or 0 */
               orwl_thread_cntrl *det   /*!< [in] non 0 if a local connection */
               ) {
  if (!sock) return 0;
  *sock = P99_LVAL(orwl_proc const,
                   .fd = fd,
                   .srv = srv,
                   .len = len,
                   .remoteorder = remo,
                   .mes = (!m && len) ? uint64_t_vnew(len) : m,
                   .det = det,
                   );
  sock->back = (!m && len) ? sock->mes : 0;
  return sock;
}

DOCUMENT_DESTROY(orwl_proc)
void orwl_proc_destroy(orwl_proc *sock);
DECLARE_NEW_DELETE(orwl_proc);
DECLARE_THREAD(orwl_proc);

#define orwl_proc_create(...) P99_CALL_DEFARG(orwl_proc_create, 2, __VA_ARGS__)
#define orwl_proc_create_defarg_1() P99_0(pthread_t*)

/**
 ** @memberof orwl_proc
 **/
void orwl_proc_untie_caller(orwl_proc *sock);

#ifdef DOXYGEN
#define DEFINE_ORWL_PROC_FUNC(F, ...)                                       \
/*! An ::orwl_proc function interpreting a message received on a socket. */ \
/*! It interprets the message it receives as if it where declared*/         \
/*! @code uint64_t F(__VA_ARGS__) @endcode */                               \
/*! @see ORWL_PROC_READ is used to interpret the message as specified */    \
/*! @memberof orwl_proc */                                                  \
void F(orwl_proc *Arg)
#define DECLARE_ORWL_PROC_FUNC(F, ...) void F(orwl_proc *Arg)
#else
#define DEFINE_ORWL_PROC_FUNC(F, ...)                          \
DEFINE_ORWL_REGISTER_ALIAS(F, orwl_proc);                      \
void F(orwl_proc *Arg)

#define DECLARE_ORWL_PROC_FUNC(F, ...)                         \
void F(orwl_proc *Arg);                                        \
DECLARE_ORWL_REGISTER_ALIAS(F, orwl_proc)
#endif

DECLARE_ORWL_TYPE_DYNAMIC(orwl_proc);


#define ORWL_PROC_READ(A, F, ...)                              \
(void)((void (*)(orwl_proc*)){ F });                           \
P99_VASSIGNS((A)->mes, __VA_ARGS__);                           \
(A)->len -= P99_NARG(__VA_ARGS__);                             \
(A)->mes += P99_NARG(__VA_ARGS__)

/*! @brief an accessor function */
/*! @memberof orwl_proc */
orwl_addr orwl_proc_getpeer(orwl_proc const*Arg);


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
inline
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
