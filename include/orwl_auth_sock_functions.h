/*
** orwl_auth_sock_functions.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat May 15 23:22:44 2010 Jens Gustedt
** Last update Sat May 15 23:22:44 2010 Jens Gustedt
*/

#ifndef   	ORWL_AUTH_SOCK_FUNCTIONS_H_
# define   	ORWL_AUTH_SOCK_FUNCTIONS_H_

#include "p99_id.h"
#include "orwl_socket.h"


DECLARE_AUTH_SOCK_FUNC(auth_sock_write_request, uint64_t wqPOS, uint64_t whID, uint64_t port);
DECLARE_AUTH_SOCK_FUNC(auth_sock_read_request, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port);
DECLARE_AUTH_SOCK_FUNC(auth_sock_release, uintptr_t whID);
DECLARE_AUTH_SOCK_FUNC(auth_sock_insert_peer, uint64_t port);
DECLARE_AUTH_SOCK_FUNC(auth_sock_insert_host, uint64_t addr, uint64_t port);
DECLARE_AUTH_SOCK_FUNC(auth_sock_do_nothing, void);


#endif 	    /* !ORWL_AUTH_SOCK_FUNCTIONS_H_ */
