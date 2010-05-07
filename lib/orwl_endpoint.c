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

