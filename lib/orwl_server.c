/*
** orwl_server.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 20:24:07 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_server.h"

orwl_server* orwl_server_init(orwl_server *serv);

void orwl_server_close(orwl_server *serv) {
  MUTUAL_EXCLUDE(serv->host.mut)
    if (serv->fd_listen != -1) {
      int fd = serv->fd_listen;
      serv->fd_listen = -1;
      close(fd);
    }
}


void orwl_server_terminate(orwl_server *serv, rand48_t *seed) {
  orwl_send(&serv->host.ep, seed, NULL, 0);
}

DEFINE_DEFARG(orwl_server_terminate, , seed_get());


void orwl_server_destroy(orwl_server *serv) {
  orwl_server_close(serv);
  orwl_host *n = NULL;
  MUTUAL_EXCLUDE(serv->host.mut)
    n = serv->host.next;
  for (orwl_host *h = n; h != &serv->host; ) {
    MUTUAL_EXCLUDE(h->mut) {
      n = h;
      h = h->next;
    }
    orwl_host_delete(n);
  }
}

DEFINE_NEW_DELETE(orwl_server);
