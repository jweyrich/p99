/*
** orwl_types.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 28 14:54:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

/**
 ** @file orwl_types.c
 **
 ** @brief Collect the type information in one big meta register
 **/


#include "orwl_macro.h"
#include "orwl_wait_queue.h"
#include "orwl_socket.h"


DEFINE_ORWL_TYPES(ORWL_REGISTER_TYPE(orwl_state),
                  ORWL_REGISTER_TYPE(orwl_wq),
                  ORWL_REGISTER_TYPE(orwl_wh),
                  ORWL_REGISTER_TYPE(auth_sock)
                  );

