/*
** orwl_enum.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Feb 22 17:51:29 2010 Jens Gustedt
** Last update Mon Feb 22 17:51:29 2010 Jens Gustedt
*/

#ifndef   	ORWL_ENUM_H_
# define   	ORWL_ENUM_H_

#include <string.h>
#include "orwl_once.h"

/**
 ** @brief Declare a simple enumeration type.
 **
 ** This macro only works for enumeration types that have no
 ** assignments to the constants. Use it in something as
 ** @code
 ** DECLARE_ENUM(bool, false, true);
 ** @endcode
 ** 
 **/
#define DECLARE_ENUM(T, ...)                                            \
typedef enum { __VA_ARGS__ } T;                                         \
static T const _ ## T ## _consts[] = {  __VA_ARGS__ };                  \
enum { _ ## T ## _max = (sizeof(_ ## T ## _consts) / sizeof(T)) };      \
static char _ ## T ## _concat[] =  # __VA_ARGS__;                       \
enum { _ ## T ## _concat_len = sizeof(_ ## T ## _concat) };             \
DECLARE_ONCE(T);                                                        \
extern char const* T ## _getname(T x)


#define DEFINE_ENUM(T)                                                  \
static char const* _ ## T ## _names[_ ## T ## _max] = { };              \
DEFINE_ONCE(T) {                                                        \
  size_t i;                                                             \
  char *head = _ ## T ## _concat;                                       \
  for (i = 1; i < _ ## T ## _max; ++i) {                                \
    head = index(head, ',');                                            \
    for (; *head == ',' || *head == ' '; ++head)                        \
      *head = '\0';                                                     \
    _ ## T ## _names[i] = head;                                         \
    ++head;                                                             \
  }                                                                     \
  /* be sure to have this assigned last */                              \
  _ ## T ## _names[0] = _ ## T ## _concat;                              \
}                                                                       \
char const* T ## _getname(T x) {                                        \
  unsigned pos = x;                                                     \
  INIT_ONCE(T);                                                         \
  return (pos < _ ## T ## _max) ? _ ## T ## _names[pos] : "((" #T ")unknown value)"; \
}

DECLARE_ENUM(bool, false, true);

#endif 	    /* !ORWL_ENUM_H_ */
