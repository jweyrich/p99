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
  struct orwl_wh *whs;     /*!< handles to block the server, if any */
  char* info;              /*!< an informative string that is
                             presented in the terminal */
  size_t info_len;         /*!< the length of #info */
};

#define ORWL_SERVER_INITIALIZER(NAME, MAXC, ADDR, PORT)         \
{                                                               \
  .fd_listen = -1,                                              \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT),         \
  .max_connections = MAXC                                       \
}

DOCUMENT_INIT(orwl_server)
P99_DEFARG_DOCU(orwl_server_init)
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
P99_PROTOTYPE(orwl_server*, orwl_server_init, orwl_server *, size_t, size_t, in_addr_t, in_port_t);
#define orwl_server_init(...) P99_CALL_DEFARG(orwl_server_init, 5, __VA_ARGS__)
#define orwl_server_init_defarg_1() (size_t)20u
#define orwl_server_init_defarg_2() P99_0(size_t)
#define orwl_server_init_defarg_3() P99_0(in_addr_t)
#define orwl_server_init_defarg_4() P99_0(in_port_t)
#endif



void orwl_server_destroy(orwl_server *serv);

DECLARE_NEW_DELETE(orwl_server);

DECLARE_THREAD(orwl_server);

void orwl_server_close(orwl_server *serv);

P99_DEFARG_DOCU(orwl_server_terminate)
void
orwl_server_terminate(orwl_server *serv,   /*!< the server to terminate */
                       rand48_t* seed  /*!< [in] defaults to a thread local seed */
                      );

/**
 ** @brief Block the server initially.
 **
 ** Use this to first insert all handles in their queues and then kick
 ** off the system with ::orwl_server_unblock.
 **/
void orwl_server_block(orwl_server *serv);

/**
 ** @brief Unblock the server.
 **
 ** Use this to kick off the system after having inserted all handles
 ** in their queues.
 **
 ** @see orwl_server_unblock.
 **/
void orwl_server_unblock(orwl_server *serv);


#ifndef DOXYGEN
P99_PROTOTYPE(void, orwl_server_terminate, orwl_server *, rand48_t *);
#define orwl_server_terminate(...) P99_CALL_DEFARG(orwl_server_terminate, 2, __VA_ARGS__)
#define orwl_server_terminate_defarg_1() seed_get()
#endif


#endif 	    /* !ORWL_SERVER_H_ */
