/*
** orwl_new.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Mar  3 21:51:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_new.h"

size_t p99__blocks(size_t n, size_t stride);
size_t p99__vnew_size_len(size_t n);
void* p99__vnew(size_t n);
size_t p99__vlen(void const*p);
void p99__vdelete(void const*p);
size_t const*p99__vfind(void const*p);
