/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
/*
** orwl_sem.c
**
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
**
** Started on  Fri Nov 19 17:36:26 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_sem.h"
#include "p99_defarg.h"

P99_INSTANTIATE(int, orwl_sem_destroy, orwl_sem *);
P99_INSTANTIATE(int, orwl_sem_getvalue, orwl_sem *, int *);
P99_INSTANTIATE(int, orwl_sem_init, orwl_sem *, unsigned int);
P99_INSTANTIATE(int, orwl_sem_post, orwl_sem *);
P99_INSTANTIATE(int, orwl_sem_trywait, orwl_sem *);
P99_INSTANTIATE(int, orwl_sem_wait, orwl_sem *);
P99_INSTANTIATE(unsigned, orwl_sem_assert, orwl_sem* s);
