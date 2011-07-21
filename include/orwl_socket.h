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
#ifndef     ORWL_SOCKET_H_
# define    ORWL_SOCKET_H_

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

#endif      /* !ORWL_SOCKET_H_ */
