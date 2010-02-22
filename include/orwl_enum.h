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

#include <pthread.h>
#include <string.h>

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
static char const _ ## T ## _concat[] =  # __VA_ARGS__;                 \
enum { _ ## T ## _concat_len = sizeof(_ ## T ## _concat) };             \
extern char const* T ## _getname(T x)


#define DEFINE_ENUM(T)                                                  \
char const* T ## _getname(T x) {                                        \
  static char const* names[_ ## T ## _max] = { };                       \
  static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;               \
  if (!names[0]) {                                                      \
    pthread_mutex_lock(&mut);                                           \
    if (!names[0]) {                                                    \
      static char concat[_ ## T ## _concat_len] = { };                  \
      memcpy(concat, _ ## T ## _concat, _ ## T ## _concat_len);         \
      size_t i;                                                         \
      char *head = concat;                                              \
      for (i = 1; i < _ ## T ## _max; ++i) {                            \
        head = index(head, ',');                                        \
        for (; *head == ',' || *head == ' '; ++head)                    \
          *head = '\0';                                                 \
        names[i] = head;                                                \
        ++head;                                                         \
      }                                                                 \
      /* be sure to have this assigned last */                          \
      names[0] = concat;                                                \
    }                                                                   \
    pthread_mutex_unlock(&mut);                                         \
  }                                                                     \
  unsigned pos = x;                                                     \
  return (pos < _ ## T ## _max) ? names[pos] : "((" #T ")unknown value)"; \
}

DECLARE_ENUM(bool, false, true);

#endif 	    /* !ORWL_ENUM_H_ */
