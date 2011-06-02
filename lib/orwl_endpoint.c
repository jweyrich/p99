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
#include "orwl_posix.h"
#include "p99_posix_default.h"
#include "p99_c99_default.h"
#include "orwl_socket.h"
#include "orwl_header.h"
#include "orwl_endpoint.h"
#include "orwl_server.h"

orwl_endpoint* orwl_endpoint_init
     (orwl_endpoint *endpoint,
      in_addr_t addr,
      in_port_t port,
      uint64_t index
      );

void orwl_endpoint_destroy(orwl_endpoint *endpoint);


DEFINE_NEW_DELETE(orwl_endpoint);

P99_INSTANTIATE(orwl_endpoint, orwl_endpoint_get, char const* name);

orwl_endpoint* orwl_endpoint_parse(orwl_endpoint* ep, char const* name) {
  if (ep && name && name[0]) {
    orwl_addr addr = P99_INIT;
    orwl_port port = P99_INIT;
    uint64_t index = 0;
    char const prefix[7 + 1] = "orwl://";
    if (strstr(name, prefix) == name) {
      name += 7;
    }
    {
      char const* name1 = 0;
      size_t len = 0;
      if (name[0] == '[') {
        ++name;
        len = strcspn(name, "]");
        if (name[len] != ']') return 0;
        name1 = name + (len + 1);
      } else {
        len = strcspn(name, ":");
        name1 = name + len;
      }
      if (!len) return 0;
      assert(len);
      char * host = memcpy(calloc(len + 1u, 1), name, len);
      orwl_addr_init(&addr, orwl_inet_addr(host));
      name = name1;
      free(host);
    }
    if (name[0]) {
      if (name[0] != ':') return 0;
      ++name;
      orwl_port_init(&port, strtou16(name));
      size_t len = strcspn(name, "/");
      if (!len) return 0;
      name += len;
    }
    if (name[0]) {
      if (name[0] != '/') return 0;
      ++name;
      index = strtou64(name);
    }
    ep->addr = addr;
    ep->port = port;
    ep->index = index;
  }
  return ep;
}

/* Needed to switch off alias testing by the compiler */
union _sockaddr_alias {
  struct sockaddr_in in4;
  struct sockaddr_in6 in6;
  struct sockaddr in46;
};

char const* orwl_endpoint_print(orwl_endpoint const* ep, char* name) {
  name[0] = '\0';
  char host[256] = "";
  struct in_addr in4_addr = addr2net(&ep->addr);
  if (!in4_addr.s_addr
#if defined(POSIX_IPV6) && (POSIX_IPV6 > 0)
      || ((in4_addr.s_addr == P99_TMAX(in_addr_t))
          && !memcmp(ep->addr.aaaa, &in6addr_any, 16))
#endif
      ) {
    hostname(host);
  } else {
#if defined(POSIX_IPV6) && (POSIX_IPV6 > 0)
    if (in4_addr.s_addr == P99_TMAX(in_addr_t)) {
      union _sockaddr_alias addr6 = { .in6 = { .sin6_family = AF_INET6 } };
      memcpy(addr6.in6.sin6_addr.s6_addr, ep->addr.aaaa, 16);
      /* We need this, since sa_family is not necessarily atop of
         sin6_family */
      addr6.in46.sa_family = AF_INET6;
      strcat(host, "[");
      orwl_inet_ntop(&addr6.in46, host + 1);
      strcat(host, "]");
    } else
#endif
      {
        union _sockaddr_alias addr4 = {
          .in4 = {
            .sin_family = AF_INET,
            .sin_addr = in4_addr
          }
        };
        /* We need this, since sa_family is not necessarily atop of
           sin_family */
        addr4.in46.sa_family = AF_INET;
        orwl_inet_ntop(&addr4.in46, host);
      }
  }
  P99_STRCATS(name, "orwl://", host, ":", PRIu(port2net(&ep->port)), "/");
  if (ep->index) P99_STRCATS(name, PRIu(ep->index));
  return name;
}

/**
 ** @brief Lauch a remote procedure call with function @a F.
 **
 ** @msc
 **   caller,server,thread;
 **   caller -> server [label="connect()", URL="\ref connect()"];
 **   caller -> server [label="orwl_send_(chal1)", URL="\ref orwl_send_()"];
 **   server -> caller [label="orwl_recv_(chal2, repl1)", URL="\ref orwl_recv_()"];
 **   caller -> server [label="orwl_send_(len, repl2)", URL="\ref orwl_send_()"];
 **   server -> thread [label="pthread_create()"];
 **   caller -> thread [label="orwl_send_(mess)", URL="\ref orwl_send_()"];
 **   thread -> caller [label="orwl_recv_(ret)", URL="\ref orwl_recv_()"];
 ** @endmsc
 **/
uint64_t orwl_send(orwl_server *srv, orwl_endpoint const* there, rand48_t *seed, size_t len, uint64_t*const mess) {
  uint64_t ret = 0;
  /* We use a remote connection if either the local server address is
     not known or we see that the endpoint "there" is actually on
     server "srv" */
  if (!srv || !orwl_endpoint_similar(&srv->host.ep, there)) {
    /* do all this work before opening the socket */
    uint64_t const chal = orwl_rand64(seed);
    uint64_t const repl = orwl_challenge(chal);
    header_t header = HEADER_T_INITIALIZER(chal);
    struct sockaddr_in addr = {
      .sin_addr = addr2net(&there->addr),
      .sin_port = port2net(&there->port),
      .sin_family = AF_INET
    };
    if (P99_UNLIKELY(!repl)) {
      report(1, "cannot send without a secret\n");
      return ORWL_SEND_ERROR;
    }

    int const fd = socket(AF_INET, SOCK_STREAM, 0);
    if (P99_UNLIKELY(fd < 0)) {
      P99_HANDLE_ERRNO {
      default:
        perror("orwl_send could not open socket");
      }
      return ORWL_SEND_ERROR;
    }

    /* Now that we have a valid file descriptor, protect its closing. */
    P99_UNWIND_PROTECT {
      bool volatile success = false;
      /* connect and do a challenge / receive authentication with the
         other side */
      for (unsigned tries = 0; tries < 10; ++tries) {
        if (P99_UNLIKELY(connect(fd, (struct sockaddr*)&addr, sizeof(addr)))) {
          P99_HANDLE_ERRNO {
          default:
            perror("orwl_send could not connect socket");
            report(1, "address is %s", orwl_endpoint_print(there));
          }
          if (tries < 10) sleepfor(1E0);
          else P99_UNWIND_RETURN ORWL_SEND_ERROR;
        } else break;
      }
      if (P99_UNLIKELY(orwl_send_(fd, header, header_t_els, 0) || orwl_recv_(fd, header, header_t_els, 0))) {
        P99_HANDLE_ERRNO {
        default:
          perror("orwl_send could not exchange challenge");
        }
        P99_UNWIND_RETURN ORWL_SEND_ERROR;
      }

      /* The communication was successful */
      if (P99_LIKELY(header[1] == repl)) {
        /* The other side is authorized. Send the answer and the size of
           the message to the other side. */
        header[1] = orwl_challenge(header[0]);
        header[0] = len;
        if (P99_UNLIKELY(orwl_send_(fd, header, header_t_els, 0))) P99_UNWIND_RETURN ORWL_SEND_ERROR;
        /* The authorized empty message indicates termination.
           If not so, we now send the real message. */
        if (len) {
          if (P99_UNLIKELY(orwl_send_(fd, mess, len, header[2]))) P99_UNWIND_RETURN ORWL_SEND_ERROR;
          /* Receive a final message, until the other end closes the
             connection. */
          if (P99_UNLIKELY(orwl_recv_(fd, header, header_t_els, header[2])))
            report(1, "terminal reception not successful\n");
          else
            ret = header[0];
        }
        success = true;
      } else  {
        /* The other side is not authorized. Terminate. */
        diagnose(fd, "fd %d, you are not who you pretend to be", fd);
        header[1] = 0;
        header[0] = 0;
        if (P99_UNLIKELY(orwl_send_(fd, header, header_t_els, 0))) P99_UNWIND_RETURN ORWL_SEND_ERROR;
      }
    P99_PROTECT:
      close(fd);
      if (!success) report(1, "send request didn't succeed");
    }
  } else {
    /* This is supposed to be a local connection, directly create the
       thread without going through the server socket. */
    orwl_thread_cntrl* det =  P99_NEW(orwl_thread_cntrl);
    auth_sock *sock = P99_NEW(auth_sock,
                              -1, srv, len, ,
                              mess,
                              det);
    auth_sock_launch(sock, det);
    if (srv->info && srv->info_len) progress(1, (uintptr_t)sock, "%s", srv->info);
    /* Wait that the caller has copied the message and returned the
       control information, and tell him in turn that we have read the
       result. */
    orwl_thread_cntrl_wait_for_callee(det);
    ret = sock->ret;
    orwl_thread_cntrl_detach(det);
  }
  return ret;
}

enum { maxlen = 1 << 24 };

bool orwl_send_(int fd, uint64_t const*const mess, size_t len, uint64_t remo) {
  /* We only have to translate the message buffer, if we have an
     order that is different from network order and different from
     the order of the remote host. */
  uint64_t *const buf = ((ORWL_HOSTORDER != ORWL_NETWORDER)
                         && (remo != ORWL_NETWORDER))
    ? uint64_t_vnew(len)
    : (void*)0;
  if (buf) {
    orwl_hton(buf, mess, len);
  }

  char * bbuf = (void*)(buf ? buf : mess);

  P99_UNWIND_PROTECT {
    for (size_t blen = sizeof(uint64_t) * len; blen;) {
      /* Don't stress the network layer by sending too large messages
         at a time. */
      size_t const clen = (blen > maxlen) ? maxlen : blen;
      ssize_t const res = send(fd, bbuf, clen, 0);
      if (P99_LIKELY(res > 0)) {
        bbuf += res;
        blen -= res;
        if (res != clen)
          report(true, "orwl_send_ only succeeded partially (%zd / %zu), retrying\n",
                 res, clen);
      } else {
        report(1, "orwl_send_ did not make any progress\n");
        P99_HANDLE_ERRNO {
          P99_XCASE EINTR : {
            perror("orwl_send_ was interrupted, retrying");
          }
        P99_XDEFAULT : {
            perror("orwl_send_ had problems, aborting");
            P99_UNWIND_RETURN true;
          }
        }
        sleepfor(1E-6);
      }
    }
  P99_PROTECT:
    if (buf) uint64_t_vdelete(buf);
  }
  return false;
}

bool orwl_recv_(int fd, uint64_t *const mess, size_t len, uint64_t remo) {
  char * bbuf = (void*)mess;
  if (!len) report(1, "orwl_recv_ with len 0, skipping\n");
  P99_UNWIND_PROTECT {
    for (size_t blen = sizeof(uint64_t) * len; blen;) {
      /* Don't stress the network layer by receiving too large messages
         at a time. */
      size_t const clen = (blen > maxlen) ? maxlen : blen;
      ssize_t const res = recv(fd, bbuf, clen, MSG_WAITALL);
      if (P99_LIKELY(res > 0)) {
        bbuf += res;
        blen -= res;
        if (res != clen)
          report(true, "orwl_recv_ only succeeded partially (%zd / %zu), retrying\n",
                 res, clen);
      } else {
        report(1, "orwl_recv_ did not make any progress\n");
        P99_HANDLE_ERRNO {
          P99_XCASE EINTR : {
            perror("orwl_recv_ was interrupted, retrying");
          }
        P99_XDEFAULT : {
            perror("orwl_recv_ had problems, aborting");
            P99_UNWIND_RETURN true;
          }
        }
        sleepfor(1E-6);
      }
    }
  /* We only have to translate the message buffer, if we have an
     order that is different from network order and different from
     the order of the remote host. */
    if ((ORWL_HOSTORDER != ORWL_NETWORDER)
        && (remo != ORWL_NETWORDER)) {
      orwl_ntoh(mess, 0, len);
    }
  }
  return false;
}

P99_INSTANTIATE(orwl_addr*, orwl_addr_init, orwl_addr *, in_addr_t);
P99_INSTANTIATE(struct in_addr, addr2net, orwl_addr const*);
P99_INSTANTIATE(struct in6_addr, addr2net6, orwl_addr const*);
P99_INSTANTIATE(orwl_port*, orwl_port_init, orwl_port *, in_port_t);
P99_INSTANTIATE(in_port_t, port2net, orwl_port const*);
P99_INSTANTIATE(uint64_t, port2host, orwl_port const*);
P99_INSTANTIATE(orwl_port, net2port, in_port_t);
P99_INSTANTIATE(orwl_port, host2port, uint64_t);
P99_INSTANTIATE(bool, orwl_port_eq, orwl_port const*, orwl_port const*);
P99_INSTANTIATE(bool, orwl_addr_eq, orwl_addr const*, orwl_addr const*);
P99_INSTANTIATE(bool, orwl_endpoint_eq, orwl_endpoint const*, orwl_endpoint const*);
P99_INSTANTIATE(bool, orwl_endpoint_similar, orwl_endpoint const*, orwl_endpoint const*);

