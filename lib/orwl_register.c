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
#include "orwl_register.h"
#include "orwl_macro.h"

static
pthread_mutex_t orwl__reg_mut = PTHREAD_MUTEX_INITIALIZER;

orwl_register const* orwl_register_init(orwl_register const* field) {
  if (!field) return 0;
  if (!*(field->regptr)) {
    MUTUAL_EXCLUDE(orwl__reg_mut) {
      if (!*(field->regptr)) {
        orwl_register const*base = (field->start);
        for (size_t i = 0; base[i].fptr || base[i].dptr; ++i)
          *(base[i].regptr) = &(base[i]);
      }
    }
  }
  return field;
}

P99_INSTANTIATE(size_t, orwl_register_id, orwl_register const *);
P99_INSTANTIATE(void*, orwl_register_get, orwl_register const *);
P99_INSTANTIATE(void, orwl_register_call, orwl_register const*, void*);
P99_INSTANTIATE(void, orwl_domain_call, orwl_domain, size_t, void*);

void orwl_types_init(void) {
  orwl_register_init(ORWL_FTAB(orwl_types));
  for (size_t i = 0; ; ++i) {
    void *R = orwl_register_get(ORWL_FTAB(orwl_types) + i);
    if (!R) break;
    orwl_register_init(*(orwl_register**)R);
  }
}
