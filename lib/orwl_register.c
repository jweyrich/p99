/*
** orwl_register.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Mar  6 22:58:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_register.h"

#include "orwl_once.h"

#include "orwl_macro.h"

static
pthread_mutex_t orwl__reg_mut = PTHREAD_MUTEX_INITIALIZER;

orwl_register const* orwl_register_init(orwl_register const* field) {
  if (!field) return NULL;
  if (!*(field->regptr)) {
    MUTUAL_EXCLUDE(orwl__reg_mut) {
      if (!*(field->regptr)) {
        orwl_register const*base = (field->start);
        for (size_t i = 0; base[i].fptr != NULL || base[i].dptr != NULL; ++i)
          *(base[i].regptr) = &(base[i]);
      }
    }
  }
  return field;
}

size_t orwl_register_id(orwl_register const *field);
void *orwl_register_get(orwl_register const *field);
void orwl_register_call(orwl_register const *field, void *arg);

void orwl_domain_call(orwl_domain domain, size_t id, void *arg);

void orwl_types_init(void) {
  orwl_register_init(ORWL_FTAB(orwl_types));
  for (size_t i = 0; ; ++i) {
    void *R = orwl_register_get(ORWL_FTAB(orwl_types) + i);
    if (!R) break;
    orwl_register_init(*(orwl_register**)R);
  }
}
