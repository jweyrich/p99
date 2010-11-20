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

int    orwl_sem_destroy(orwl_sem *);
int    orwl_sem_getvalue(orwl_sem *, int *);
int    orwl_sem_init(orwl_sem *, unsigned int);
int    orwl_sem_post(orwl_sem *);
int    orwl_sem_trywait(orwl_sem *);
int    orwl_sem_wait(orwl_sem *);

unsigned orwl_sem_assert(orwl_sem* s);
