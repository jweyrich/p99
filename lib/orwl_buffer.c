/*
** orwl_buffer.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Jul  5 08:47:56 2011 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_buffer.h"

P99_INSTANTIATE(orwl_buffer*, orwl_buffer_init, orwl_buffer *, size_t, uint64_t*);
P99_INSTANTIATE(void, orwl_buffer_destroy, orwl_buffer*);
