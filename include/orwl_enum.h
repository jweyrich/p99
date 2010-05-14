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

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include "orwl_once.h"

/**
 ** @brief Declare a simple enumeration type.
 **
 ** This macro only works for enumeration types that have no
 ** assignments to the constants. To define an enumeration type @c
 ** color Use it in something as
 ** @code
 ** DECLARE_ENUM(color, red, green, blue);
 ** @endcode
 **
 ** As additional benefits you obtain:
 **
 ** - constants @c color_min and @c color_max that in this example
 **   here would correspond to @c red and @c blue, respectively
 ** - a function @c color_getname that returns a string containing the
 **   name of its argument.
 **
 ** To have this functional, you have to put an line
 ** @code
 ** DEFINE_ENUM(color);
 ** @endcode
 **
 ** in one of your object files.
 **
 ** Unfortunately you can't use doxygen like comments inside the
 ** argument list to document the enumeration constants. You have to
 ** document the constants separately. For the above example you would
 ** do
 ** @code
 **   /// @var color red
 **   /// the most redish color of all colors
 ** @endcode
 ** Observe the bizarre naming convention, here. Although in C (and
 ** C++) `red' is declared in the same scope as `enum color' you have
 ** to prefix it with `color' such that the documentation lands inside
 ** the one for `color'.
 **/
#define DECLARE_ENUM(T, ...)                                            \
/*! @see DECLARE_ENUM was used for the declaration of this enumeration type */ \
/*! @see T ## _getname for access to the names of the constants as strings */ \
typedef enum { __VA_ARGS__ ,                                            \
               /*! upper bound of the @ref T constants */               \
               T ## _amount,                                            \
               /*! the largest @ref T constant */                       \
               T ## _max = ((size_t)(T ## _amount) - 1u),               \
               /*! the smallest @ref T constant */                      \
               T ## _min = 0                                            \
} T;                                                                    \
extern char const* _ ## T ## _names[T ## _amount];                      \
DECLARE_ONCE(T);                                                        \
 /*! @brief Get a string with the name of constant @a x of type @ref T */ \
inline                                                                  \
char const* T ## _getname(T x) {                                        \
  unsigned pos = x;                                                     \
  INIT_ONCE(T);                                                         \
  return (pos < T ## _amount) ? _ ## T ## _names[pos] : "((" #T ")unknown value)"; \
}                                                                       \
static char _ ## T ## _concat[] =  # __VA_ARGS__;                       \
enum _decl_enum_ ## T { _ ## T ## _concat_len = sizeof(_ ## T ## _concat) }


/**
 ** @brief Define the necessary symbols for a simple enumeration type.
 **
 ** Use this with DECLARE_ENUM(), which see.
 **/
#define DEFINE_ENUM(T)                                                  \
/* Ensure that the table is generated in this object file */            \
char const* _ ## T ## _names[T ## _amount] = INITIALIZER;               \
DEFINE_ONCE(T) {                                                        \
  char *head = _ ## T ## _concat;                                       \
  for (T i = T ## _min; i < T ## _max; ++i) {                           \
    _ ## T ## _names[i] = head;                                         \
    head = index(head, ',');                                            \
    for (; *head == ',' || *head == ' '; ++head)                        \
      *head = '\0';                                                     \
  }                                                                     \
  _ ## T ## _names[T ## _max] = head;                                   \
}                                                                       \
/* Ensure that the function symbol is generated in this object file */  \
char const* T ## _getname(T x)


#endif 	    /* !ORWL_ENUM_H_ */
