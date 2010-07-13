/*
** atomic_counter.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Jul 10 13:23:24 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "atomic_counter.h"

void atomic_counter_inc(atomic_counter* p);
void atomic_counter_dec(atomic_counter* p);
void atomic_counter_wait(atomic_counter* p);
unsigned atomic_counter_getvalue(atomic_counter* p);

P99_DEFINE_DEFARG(atomic_counter_init, , PTHREAD_PROCESS_PRIVATE, 0u);


DEFINE_ONCE(atomic_counter) {
  /* empty, no static dependencies for the moment */
}


atomic_counter* atomic_counter_init(atomic_counter* p, int pshared, unsigned int value) {
  /* add this the day you add static dependencies */
  /* INIT_ONCE(atomic_counter); */
  sem_init(&p->atomic_pos, pshared, value);
  sem_init(&p->atomic_neg, pshared, 1u);
  sem_init(&p->atomic_mut, pshared, 1u);
  return p;
}
