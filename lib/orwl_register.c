/*
** orwl_register.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Mar  6 22:58:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stddef.h>
#include "orwl_thread.h"
#include "orwl_register.h"
#include "orwl_wait_queue.h"

pthread_mutex_t _reg_mut = PTHREAD_MUTEX_INITIALIZER;

void orwl_register_init(orwl_register const* field) {
  if (!(field->regptr)) {
    pthread_mutex_lock(&_reg_mut);
    if (!(field->regptr)) {
      orwl_register const*base = (field->start);
      for (size_t i = 0; base[i].fptr != NULL && base[i].dptr != NULL; ++i) {
        *(base[i].regptr) = &(base[i]);
      }
    }
    pthread_mutex_unlock(&_reg_mut);
  }
}

size_t orwl_register_id(orwl_register const *field);
void *orwl_register_get(orwl_register const *field);
void orwl_register_call(orwl_register const *field, void *arg);

void orwl_domain_call(orwl_domain domain, size_t id, void *arg);

DEFINE_ORWL_TYPES(ORWL_REGISTER_TYPE(orwl_wq),
                  ORWL_REGISTER_TYPE(orwl_wh)
                  );

size_t hu(void) {
  return ORWL_TYPEID(orwl_wh) + ORWL_OBJID(orwl_wait_release);
}
