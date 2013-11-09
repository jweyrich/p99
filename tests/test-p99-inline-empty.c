/*
** inl.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Thu Nov 25 17:53:38 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdio.h>
#include "test-p99-inline.h"

int mein(void) {
  p00_inl();
  p00_sinl();
  p00_einl();
  p00_einl_p();
  p00_einl_inl();
  p00_inl_einl();
  p00_inl_p();
  return 0;
}
