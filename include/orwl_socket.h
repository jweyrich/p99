/*
** orwl_socket.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 21:46:01 2010 Jens Gustedt
** Last update Sun Mar 21 21:46:01 2010 Jens Gustedt
*/

#ifndef   	ORWL_SOCKET_H_
# define   	ORWL_SOCKET_H_

#include "orwl_macro.h"
#include "orwl_int.h"
#include "orwl_enum.h"

enum { inet_order = 0x3210 };

extern bool same_endianess(uint32_t c);

#endif 	    /* !ORWL_SOCKET_H_ */
