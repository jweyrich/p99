/*
** orwl_endpoint.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 19:55:26 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_posix_default.h"
#include "orwl_socket.h"
#include "orwl_header.h"

orwl_endpoint* orwl_endpoint_init
     (orwl_endpoint *endpoint,
      in_addr_t addr,
      in_port_t port,
      uint64_t index
      );

void orwl_endpoint_destroy(orwl_endpoint *endpoint);


P99_DEFINE_DEFARG(orwl_endpoint_init, , P99_0(in_addr_t), P99_0(in_port_t), P99_0(uint64_t));

DEFINE_NEW_DELETE(orwl_endpoint);


orwl_endpoint* orwl_endpoint_parse(orwl_endpoint* ep, char const* name) {
  if (ep && name && name[0]) {
    addr_t addr = ADDR_T_INITIALIZER(0);
    port_t port = PORT_T_INITIALIZER(0);
    uint64_t index = 0;
    char const prefix[7 + 1] = "orwl://";
    if (strstr(name, prefix) == name) {
      name += 7;
    }
    {
      char const* name1 = NULL;
      size_t len = 0;
      if (name[0] == '[') {
        ++name;
        len = strcspn(name, "]");
        if (name[len] != ']') return NULL;
        name1 = name + (len + 1);
      } else {
        len = strcspn(name, ":");
        name1 = name + len;
      }
      if (!len) return NULL;
      assert(len);
      char * host = memcpy(calloc(len + 1u, 1), name, len);
      addr_t_init(&addr, orwl_inet_addr(host));
      name = name1;
      free(host);
    }
    if (name[0]) {
      if (name[0] != ':') return NULL;
      ++name;
      port_t_init(&port, str2uint16_t(name));
      size_t len = strcspn(name, "/");
      if (!len) return NULL;
      name += len;
    }
    if (name[0]) {
      if (name[0] != '/') return NULL;
      ++name;
      index = str2uint64_t(name);
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
      || ((in4_addr.s_addr == P99_TMAX(in_addr_t))
          && !memcmp(ep->addr.aaaa, &in6addr_any, 16))) {
    hostname(host);
  } else {
    if (in4_addr.s_addr == P99_TMAX(in_addr_t)) {
      union _sockaddr_alias addr6 = { .in6 = { .sin6_family = AF_INET6 } };
      memcpy(addr6.in6.sin6_addr.s6_addr, ep->addr.aaaa, 16);
      /* We need this, since sa_family is not necessarily atop of
         sin6_family */
      addr6.in46.sa_family = AF_INET6;
      strcat(host, "[");
      orwl_inet_ntop(&addr6.in46, host + 1);
      strcat(host, "]");
    } else {
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
uint64_t orwl_send(orwl_endpoint const* ep, rand48_t *seed, uint64_t* mess, size_t len) {
  uint64_t ret = P99_TMAX(uint64_t);
  int fd = -1;
  /* do all this work before opening the socket */
  uint64_t chal = orwl_rand64(seed);
  uint64_t repl = orwl_challenge(chal);
  header_t header = { [0] = chal };
  struct sockaddr_in addr = {
    .sin_addr = addr2net(&ep->addr),
    .sin_port = port2net(&ep->port),
    .sin_family = AF_INET
  };
  if (!repl) {
    report(1, "cannot send without a secret");
    goto FINISH;
  }

  fd = socket(PF_INET, SOCK_STREAM, 0);
  if (fd == -1) return ret;

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    goto FINISH;
  /* Send a challenge to the other side */
  if (orwl_send_(fd, header, header_t_els))
    goto FINISH;
  /* Receive the answer and another challenge from the other side */
  if (orwl_recv_(fd, header, header_t_els))
    goto FINISH;
  if (header[1] == repl) {
    /* The other side is authorized. Send the answer and the size of
       the message to the other side. */
    header[1] = orwl_challenge(header[0]);
    header[0] = len;
    if (orwl_send_(fd, header, header_t_els))
      goto FINISH;
    /* The authorized empty message indicates termination.
       If not so, we now send the real message. */
    if (len) {
      if (orwl_send_(fd, mess, len))
        goto FINISH;
      /* Receive a final message, until the other end closes the
         connection. */
      if (!orwl_recv_(fd, header, header_t_els)) {
        ret = header[0];
        goto FINISH;
      } else {
        report(1, "terminal reception not successful");
      }
    }
    ret = 0;
  } else  {
    /* The other side is not authorized. Terminate. */
    diagnose(fd, "fd %d, you are not who you pretend to be", fd);
    header[1] = 0;
    header[0] = 0;
    if (orwl_send_(fd, header, header_t_els))
      goto FINISH;
  }
 FINISH:
  close(fd);
  if (ret == P99_TMAX(uint64_t) && len) report(1, "send request didn't succeed");
  return ret;
}

bool orwl_send_(int fd, uint64_t const*mess, size_t len) {
  uint32_t *buf = uint32_t_vnew(2 * len);
  orwl_hton(buf, mess, len);
  ssize_t ret = send(fd, buf, sizeof(uint64_t) * len, MSG_WAITALL);
  uint32_t_vdelete(buf);
  return ret != sizeof(uint64_t) * len;
}

bool orwl_recv_(int fd, uint64_t *mess, size_t len) {
  uint32_t *buf = uint32_t_vnew(2 * len);
  ssize_t ret = recv(fd, buf, sizeof(uint64_t) * len, MSG_WAITALL);
  orwl_ntoh(mess, buf , len);
  uint32_t_vdelete(buf);
  return ret != sizeof(uint64_t) * len;
}

addr_t* addr_t_init(addr_t *A, in_addr_t I);

P99_DEFINE_DEFARG(addr_t_init, , P99_0(in_addr_t));

struct in_addr addr2net(addr_t const*A);

struct in6_addr addr2net6(addr_t const*A);

port_t* port_t_init(port_t *A, in_port_t P);

P99_DEFINE_DEFARG(port_t_init, , P99_0(in_port_t));

in_port_t port2net(port_t const*A);
uint64_t port2host(port_t const*A);
port_t net2port(in_port_t P);
port_t host2port(uint64_t A);

