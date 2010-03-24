/*
** orwl_rand.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Mar 22 23:07:05 2010 Jens Gustedt
** Last update Mon Mar 22 23:07:05 2010 Jens Gustedt
*/

#ifndef   	ORWL_RAND_H_
# define   	ORWL_RAND_H_

#include <stdlib.h>
#include <math.h>
#include "orwl_int.h"

typedef unsigned short rand48_t[3];

inline
uint32_t orwl_rand(rand48_t xsubi) {
  return jrand48(xsubi);
}

inline
double orwl_drand(rand48_t xsubi) {
  return erand48(xsubi);
}

inline
uint64_t orwl_rand64(rand48_t xsubi) {
  double ret = ldexp(orwl_drand(xsubi), 48);
  return (uint64_t)ret;
}

uint64_t orwl_mix(uint64_t a, uint64_t b);

uint64_t orwl_challenge(uint64_t a);


#endif 	    /* !ORWL_RAND_H_ */
