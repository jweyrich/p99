/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
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
  pthread_t id;
};

#define ORWL_SERVER_INITIALIZER(NAME, MAXC, ADDR, PORT)        \
{                                                              \
  .fd_listen = -1,                                             \
  .host = ORWL_HOST_INITIALIZER(NAME.host, ADDR, PORT, 2),     \
  .max_connections = MAXC                                      \
}

DOCUMENT_INIT(orwl_server)
P99_DEFARG_DOCU(orwl_server_init)
orwl_server*
orwl_server_init(orwl_server *serv,       /*!< [out] the object to initialize */
                 size_t max_connections,  /*!< [in] maximum socket queue length,
                                            defaults to 20 */
                 size_t max_queues,       /*!< [in] the maximum number of locations,
                                            defaults to 0 */
                 char const* endp         /*!< [in] defaults to the
                                            null address */
                 );

#ifndef DOXYGEN
P99_PROTOTYPE(orwl_server*, orwl_server_init, orwl_server *, size_t, size_t, char const*);
#define orwl_server_init(...) P99_CALL_DEFARG(orwl_server_init, 4, __VA_ARGS__)
#define orwl_server_init_defarg_1() (size_t)20u
#define orwl_server_init_defarg_2() P99_0(size_t)
#define orwl_server_init_defarg_3() P99_0(char const*)
#endif



DOCUMENT_DESTROY(orwl_server)
void orwl_server_destroy(orwl_server *serv);

DECLARE_NEW_DELETE(orwl_server);

/**
 ** @brief lauch a server that fulfills rpc requests
 ** @msc
 **   caller1,procedure1,thread1,main,server,thread2,procedure2,caller2;
 **   main -> server [label="orwl_server_create()", URL="\ref orwl_server_create()"];
 **   caller1 -> server [label="orwl_send(procedure1, ...)", URL="\ref orwl_send()"];
 **   caller2 -> server [label="orwl_send(procedure2, ...)", URL="\ref orwl_send()"];
 **   server->thread1 [label="auth_sock_create(procedure1, ...)", URL="\ref auth_sock_create()"];
 **   thread1->procedure1 [label="procedure1(...)"];
 **   server->thread2 [label="auth_sock_create(procedure2, ...)", URL="\ref auth_sock_create()"];
 **   procedure1->caller1 [label="auth_sock_close()", URL="\ref auth_sock_close()"];
 **   thread2->procedure2 [label="procedure2(...)"];
 **   procedure1->thread1 [label="return"];
 **   procedure2->caller2 [label="auth_sock_close()", URL="\ref auth_sock_close()"];
 **   thread1->main [label="pthread_exit()"];
 **   procedure2->thread2 [label="return"];
 **   thread2->main [label="pthread_exit()"];
 **   server->main [label="pthread_exit()"];
 ** @endmsc
 **/
DECLARE_THREAD(orwl_server);

/**
 ** @memberof orwl_server
 **/
void orwl_server_close(orwl_server *serv);

/**
 ** @memberof orwl_server
 **/
P99_DEFARG_DOCU(orwl_server_terminate)
void
orwl_server_terminate(orwl_server *serv   /*!< the server to terminate */
                      );

/**
 ** @brief Block the server initially.
 **
 ** Use this to first insert all handles in their queues and then kick
 ** off the system with ::orwl_server_unblock.
 **
 ** @memberof orwl_server
 **/
void orwl_server_block(orwl_server *serv);

/**
 ** @brief Unblock the server.
 **
 ** Use this to kick off the system after having inserted all handles
 ** in their queues.
 **
 ** @see orwl_server_block.
 ** @memberof orwl_server
 **/
void orwl_server_unblock(orwl_server *serv);


/**
 ** @memberof orwl_server
 **/
void
orwl_start(orwl_server *serv,       /*!< [out] the server object to initialize */
           size_t max_connections,  /*!< [in] maximum socket queue length,
                                      defaults to 20 */
           size_t max_queues,       /*!< [in] the maximum number of locations,
                                      defaults to 0 */
           char const* endp         /*!< [in] defaults to the
                                      null address */
           );

#ifndef DOXYGEN
P99_PROTOTYPE(void, orwl_start, orwl_server *, size_t, size_t, char const*);
#define orwl_start(...) P99_CALL_DEFARG(orwl_start, 4, __VA_ARGS__)
#define orwl_start_defarg_1() (size_t)20u
#define orwl_start_defarg_2() P99_0(size_t)
#define orwl_start_defarg_3() P99_0(char const*)
#endif

/**
 ** @memberof orwl_server
 **/
void
orwl_stop(orwl_server *serv);

/**
 ** @memberof orwl_server
 **/
inline
bool
orwl_alive(orwl_server *serv) {
  return !pthread_kill(serv->id, 0);
}

#endif 	    /* !ORWL_SERVER_H_ */
