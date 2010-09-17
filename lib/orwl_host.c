/* This may look like nonsense, but it really is -*- C -*-                   */
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

void orwl_host_destroy(orwl_host *th);


DEFINE_NEW_DELETE(orwl_host);
