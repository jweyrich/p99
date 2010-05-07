/*
** orwl_server.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 20:22:40 2010 Jens Gustedt
** Last update Fri May  7 20:22:40 2010 Jens Gustedt
*/

#ifndef   	ORWL_SERVER_H_
# define   	ORWL_SERVER_H_

#include "orwl_rand.h"
#include "orwl_host.h"

struct orwl_server {
  int fd_listen;
  orwl_host host;
  unsigned max_connections;
  server_cb_t const cb;
};

#define ORWL_SERVER_INITIALIZER(NAME, CB, MAXC, ADDR, PORT)     \
{                                                               \
  .fd_listen = -1,                                              \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT),         \
  .cb = CB,                                                     \
  .max_connections = MAXC                                       \
}

inline
orwl_server* orwl_server_init(orwl_server *serv) {
  memset(serv, 0, sizeof(orwl_server));
  serv->fd_listen = -1;
  orwl_host_init(&serv->host);
  serv->host.refs = 1;
  return serv;
}

void orwl_server_destroy(orwl_server *serv);

DECLARE_NEW_DELETE(orwl_server);

DECLARE_THREAD(orwl_server);

void orwl_server_close(orwl_server *serv);

#ifndef DOXYGEN
PROTOTYPE(void, orwl_server_terminate, orwl_server *, rand48_t *);
DECLARE_DEFARG(orwl_server_terminate, , seed_get());
#define orwl_server_terminate(...) CALL_WITH_DEFAULTS(orwl_server_terminate, 2, __VA_ARGS__)
#endif


#endif 	    /* !ORWL_SERVER_H_ */
