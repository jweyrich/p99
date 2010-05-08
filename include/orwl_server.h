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
  size_t max_connections;
  size_t max_queues;
  orwl_rq *rqs;
  server_cb_t cb;
};

#define ORWL_SERVER_INITIALIZER(NAME, CB, MAXC, ADDR, PORT)     \
{                                                               \
  .fd_listen = -1,                                              \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT),         \
  .cb = CB,                                                     \
  .max_connections = MAXC                                       \
}

orwl_server* orwl_server_init(orwl_server *serv,
                              server_cb_t cb,
                              size_t max_connections,
                              size_t max_queues,
                              in_addr_t addr,
                              in_port_t port);

#ifndef DOXYGEN
PROTOTYPE(orwl_server*, orwl_server_init,
          orwl_server*, server_cb_t,
          size_t, size_t, in_addr_t, in_port_t);
DECLARE_DEFARG(orwl_server_init, , NULL, (size_t)20u, TNULL(size_t), TNULL(in_addr_t), TNULL(in_port_t));
#define orwl_server_init(...) CALL_WITH_DEFAULTS(orwl_server_init, 6, __VA_ARGS__)
#endif



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
