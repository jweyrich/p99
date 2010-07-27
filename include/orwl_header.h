/*
** orwl_header.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 21:33:11 2010 Jens Gustedt
** Last update Fri May  7 21:33:11 2010 Jens Gustedt
*/

#ifndef   	ORWL_HEADER_H_
# define   	ORWL_HEADER_H_

#include "orwl_int.h"

enum { header_t_els = 2 };

typedef uint64_t header_t[header_t_els];

#define HEADER_T_INITIALIZER { 0 }


#endif 	    /* !ORWL_HEADER_H_ */
