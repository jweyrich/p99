/*
** orwl_endpoint.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 19:55:26 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_socket.h"
#include "orwl_header.h"

orwl_endpoint* orwl_endpoint_init
     (orwl_endpoint *endpoint,
      in_addr_t addr,
      in_port_t port
      );

void orwl_endpoint_destroy(orwl_endpoint *endpoint);


DEFINE_DEFARG(orwl_endpoint_init, , TNULL(in_addr_t), TNULL(in_port_t));

DEFINE_NEW_DELETE(orwl_endpoint);


orwl_endpoint* orwl_endpoint_parse(orwl_endpoint* ep, char const* name) {
  if (ep && name) {
    addr_t addr = INITIALIZER;
    port_t port = INITIALIZER;
    char const prefix[7 + 1] = "orwl://";
    if (strstr(name, prefix) == name) {
      name += 7;
    }
    {
      char const* host = NULL;
      if (name[0] == '[') {
        ++name;
        size_t len = strcspn(name, "]");
        if (!len) return NULL;
        if (name[len] != ']') return NULL;
        host = strndup(name, len);
        name += (len + 1);
      } else {
        size_t len = strcspn(name, ":");
        if (!len) return NULL;
        host = strndup(name, len);
        name += len;
      }
      if (host) {
        addr_t_init(&addr, orwl_inet_addr(host));
        free((void*)host);
      }
    }
    if (name[0]) {
      if (name[0] != ':') return NULL;
      ++name;
      port_t_init(&port, str2uint16_t(name));
    }
    *ep = (orwl_endpoint){ .addr = addr, .port = port};
  }
  return ep;
}

char const* orwl_endpoint_print(orwl_endpoint const* ep, char* name) {
  name[0] = '\0';
  struct sockaddr_in addr = { .sin_addr = { .s_addr = htonl((uint32_t)(ep->addr.a)) } };
  snprintf(name, 128, "orwl://%s:%" PRIu32 "/",
          (ep->addr.a
           ? orwl_inet_ntop((struct sockaddr*)&addr)
           : hostname()),
          port2net(&ep->port));
  return name;
}

uint64_t orwl_send(orwl_endpoint const* ep, rand48_t *seed, uint64_t* mess, size_t len) {
  uint64_t ret = TONES(uint64_t);
  int fd = -1;
  /* do all this work before opening the socket */
  uint64_t chal = orwl_rand64(seed);
  uint64_t repl = orwl_challenge(chal);
  header_t header = { [0] = chal };
  struct sockaddr_in addr = {
    .sin_addr = { .s_addr = addr2net(&ep->addr) },
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
  if (ret == TONES(uint64_t) && len) report(1, "send request didn't succeed");
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

