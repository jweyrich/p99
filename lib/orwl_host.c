/*
** orwl_host.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 20:17:04 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_host.h"

void orwl_host_connect(orwl_host *th, orwl_host *q) {
  if (!th || !q) return;
  if (th->prev != th || th->next != th) return;
  for (bool ok = false; !ok; ) {
    MUTUAL_EXCLUDE(th->mut) {
      if (!pthread_mutex_trylock(&q->mut)) {
        orwl_host *p = q->prev;
        if (p == q || !pthread_mutex_trylock(&p->mut)) {
          ok = true;
          /* splice us between q and p */
          th->next = q;
          th-> prev = p;
          th->refs += 2;
          q->prev = th;
          p->next = th;
          if (p != q) pthread_mutex_unlock(&p->mut);
        }
        pthread_mutex_unlock(&q->mut);
      }
    }
    if (!ok) {
      sleepfor(1E-0);
      report(1,"looping");
    }
  }
}

void orwl_host_disconnect(orwl_host *th) {
  if (!th) return;
  if (th->prev || th->next) return;
  for (bool ok = false; !ok; ) {
    MUTUAL_EXCLUDE(th->mut) {
      orwl_host *q = th->next;
      orwl_host *p = q->prev;
      if (!pthread_mutex_trylock(&q->mut)) {
        if (p == q || !pthread_mutex_trylock(&p->mut)) {
          ok = true;
          th->next = th;
          th->prev = th;
          th->refs -= 2;
          q->prev = p;
          p->next = q;
          if (p != q) pthread_mutex_unlock(&p->mut);
        }
        pthread_mutex_unlock(&q->mut);
      }
    }
    if (!ok) sleepfor(1E-6);
  }
}

orwl_host* orwl_host_init(orwl_host *th, in_addr_t addr, in_port_t port);

P99_DEFINE_DEFARG(orwl_host_init, , TNULL(in_addr_t), TNULL(in_port_t));


void orwl_host_destroy(orwl_host *th);


DEFINE_NEW_DELETE(orwl_host);
