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

/* extern inline function with other declaration, must be emitted */
inline void p00_einl_p(void);

/* inline function with other extern inline declaration, must be emitted */
extern inline void p00_inl_einl(void);

/* inline function with other "bare" declaration, must be emitted */
inline void p00_inl_p(void);


int main(void) {
  // missing symbol:      
  // multiple definition: gcc-4.2
  p00_inl();
  // missing symbol:      
  // multiple definition: 
  p00_sinl();
  // missing symbol:      gcc-4.2
  // multiple definition: 
  p00_einl();
  // missing symbol:      gcc-4.2
  // multiple definition: 
  p00_einl_p();
  // missing symbol:      
  // multiple definition: gcc-4.2
  p00_inl_einl();
  // missing symbol:      
  // multiple definition: gcc-4.2
  p00_inl_p();
  return 0;
}
