/*
** orwl_new.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Mar  3 21:51:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stddef.h>
#include "orwl_new.h"

void* _vnew(size_t n);
size_t _vlen(void const*p);
void _vdelete(void const*p);
uintmax_t const*_vfind(void const*p);
