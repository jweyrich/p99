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

#include "orwl_host.h"

struct orwl_server {
  int fd_listen;           /*!< this servers file descriptor */
  orwl_host host;          /*!< a list of hosts that this server has seen */
  size_t max_connections;  /*!< maximum number of simultaneous connections */
  size_t max_queues;       /*!< number of locations served by this */
  struct orwl_wq *wqs;     /*!< the priority queues of these
                              locations, if any */
};

#define ORWL_SERVER_INITIALIZER(NAME, MAXC, ADDR, PORT)         \
{                                                               \
  .fd_listen = -1,                                              \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT),         \
  .max_connections = MAXC                                       \
}

orwl_server*
orwl_server_init(orwl_server *serv,       /*!< [out] the object to iniialize */
                 size_t max_connections,  /*!< [in] maximum socket queue length,
                                            defaults to 20 */
                 size_t max_queues,       /*!< [in] the maximum number of locations,
                                            defaults to 0 */
                 in_addr_t addr,          /*!< [in] defaults to the
                                             null address */
                 in_port_t port           /*!< [in] defaults to 0 */
                 );

#ifndef DOXYGEN
PROTOTYPE(orwl_server*, orwl_server_init, orwl_server *, size_t, size_t, in_addr_t, in_port_t);
DECLARE_DEFARG(orwl_server_init, , (size_t)20u, TNULL(size_t), TNULL(in_addr_t), TNULL(in_port_t));
#define orwl_server_init(...) CALL_WITH_DEFAULTS(orwl_server_init, 5, __VA_ARGS__)
#endif



void orwl_server_destroy(orwl_server *serv);

DECLARE_NEW_DELETE(orwl_server);

DECLARE_THREAD(orwl_server);

void orwl_server_close(orwl_server *serv);

void
orwl_server_terminate(orwl_server *,   /*!< the server to terminate */
                       rand48_t* seed  /*!< [in] defaults to a thread local seed */
                      );


#ifndef DOXYGEN
PROTOTYPE(void, orwl_server_terminate, orwl_server *, rand48_t *);
DECLARE_DEFARG(orwl_server_terminate, , seed_get());
#define orwl_server_terminate(...) CALL_WITH_DEFAULTS(orwl_server_terminate, 2, __VA_ARGS__)
#endif


#endif 	    /* !ORWL_SERVER_H_ */
