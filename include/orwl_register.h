/*
** orwl_register.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Mar  6 22:57:53 2010 Jens Gustedt
** Last update Sat Mar  6 22:57:53 2010 Jens Gustedt
*/

#ifndef   	ORWL_REGISTER_H_
# define   	ORWL_REGISTER_H_

#include "orwl_inline.h"
#include "orwl_int.h"

typedef  void (*orwl_plain)(void*);

/**
 ** @brief Statically register an object or a function.
 **/
struct orwl_register {
  struct orwl_register const*const start;
  struct orwl_register const**const regptr;
  orwl_plain const fptr;
  void *const dptr;
};

#ifndef __cplusplus
typedef struct orwl_register orwl_register;
#endif

typedef orwl_register const*const orwl_domain;

void orwl_register_init(orwl_register const* field);

inline size_t orwl_register_id(orwl_register const* field) {
  if (!(field->regptr))
    orwl_register_init(field);
  ptrdiff_t ret = field - field->start;
  return ret;
}

inline
void *orwl_register_get(orwl_register const* field) {
  return field->dptr;
}

inline
void orwl_register_call(orwl_register const* field, void* arg) {
  orwl_plain fptr= field->fptr;
  if (fptr) fptr(arg);
}

inline
void orwl_domain_call(orwl_domain domain, size_t id, void *arg) {
  orwl_register const* field = domain + id;
  orwl_register_call(field, arg);
}


#define _ORWL_ALIAS(NAME) _ ## NAME ## _alias
#define ORWL_ALIAS(NAME) _ORWL_ALIAS(NAME)

#define _ORWL_REGISTER(NAME) _ ## NAME ## _regptr
#define ORWL_REGISTER(NAME) _ORWL_REGISTER(NAME)

#define _ORWL_FTAB(NAME) _ ## NAME ## _ftab
#define ORWL_FTAB(NAME) _ORWL_FTAB(NAME)

#define __ORWL_DOMAIN_TABLE(L) _domain_tab ## L
#define _ORWL_DOMAIN_TABLE(L) __ORWL_DOMAIN_TABLE(L)
#define ORWL_DOMAIN_TABLE _ORWL_DOMAIN_TABLE(__LINE__)

#define DECLARE_ORWL_REGISTER(NAME)             \
extern orwl_register const*ORWL_REGISTER(NAME)

#define DEFINE_ORWL_REGISTER(NAME)              \
orwl_register const*ORWL_REGISTER(NAME) = NULL

#define DEFINE_ORWL_REGISTER_ALIAS(NAME, T)     \
  static void ORWL_ALIAS(NAME)(T *x) {  NAME(x); }  \
orwl_register const*ORWL_REGISTER(NAME) = NULL

#define ORWL_REGISTER_FUNCTION(NAME) {          \
.start = ORWL_DOMAIN_TABLE,                     \
  .regptr = &ORWL_REGISTER(NAME),               \
      .fptr = (orwl_plain)NAME                  \
     }

#define ORWL_REGISTER_ALIAS(NAME) {             \
.start = ORWL_DOMAIN_TABLE,                     \
  .regptr = &ORWL_REGISTER(NAME),               \
      .fptr = (orwl_plain)ORWL_ALIAS(NAME)          \
     }

#define ORWL_REGISTER_DATA(NAME) {              \
.start = ORWL_DOMAIN_TABLE,                     \
  .regptr = &ORWL_REGISTER(NAME),               \
      .dptr = (void*)&NAME                      \
     }

#define DECLARE_ORWL_DOMAIN(NAME) extern orwl_domain NAME

#define DEFINE_ORWL_DOMAIN(NAME, ...)                                   \
static orwl_register const ORWL_DOMAIN_TABLE[] = { __VA_ARGS__ , INITIALIZER}; \
orwl_domain NAME = ORWL_DOMAIN_TABLE

/**
 ** @brief Register the functions of type @a T to be able to identify
 ** them remotely.
 **
 ** @see DEFINE_ORWL_TYPE_DYNAMIC to know what functions are / can be registered.
 **/
#define DECLARE_ORWL_TYPE_DYNAMIC(T)            \
DECLARE_ORWL_DOMAIN(ORWL_FTAB(T));              \
DECLARE_ORWL_REGISTER(ORWL_FTAB(T));            \
DECLARE_ORWL_REGISTER(T ## _sizeof);            \
DECLARE_ORWL_REGISTER(T ## _init);              \
DECLARE_ORWL_REGISTER(T ## _destroy)

/**
 ** @brief Register the functions of type @a T to be able to identify
 ** them remotely.
 **
 ** This always assumes that @a T has an init and a destroy function
 ** and registers theses two.
 **
 ** Other functions must declare their register in the corresponding .h
 ** @code
 ** void myfunc(mytype x);
 **
 ** DECLARE_ORWL_REGISTER(myfunc);
 ** @endcode
 **
 ** Then the register must be instantiated in the .c file
 ** @code
 ** DEFINE_ORWL_REGISTER_ALIAS(myfunc, mytype);
 ** @endcode
 **
 ** Inside the variable argument list for this macro here you must
 ** then use ORWL_REGISTER_ALIAS()
 ** @code
 ** DEFINE_ORWL_TYPE_DYNAMIC(mytype,
 **    ORWL_REGISTER_ALIAS(myfunc),
 **    .
 **    .
 **    );
 ** @endcode
 **
 ** @see DECLARE_ORWL_TYPE_DYNAMIC on how to make this definition
 ** visible in a .h file.
 ** @see DEFINE_ORWL_TYPES to establish the meta-register containing
 ** all the types.
 **/
#define DEFINE_ORWL_TYPE_DYNAMIC(T, ...)                        \
DEFINE_ORWL_REGISTER(ORWL_FTAB(T));                             \
DEFINE_ORWL_REGISTER(T ## _sizeof);                             \
static size_t const T ## _sizeof = sizeof(T);                   \
DEFINE_ORWL_REGISTER_ALIAS(T ## _init, T);                      \
DEFINE_ORWL_REGISTER_ALIAS(T ## _destroy, T);                   \
DEFINE_ORWL_DOMAIN(ORWL_FTAB(T),                                \
                   ORWL_REGISTER_DATA(T ## _sizeof),            \
                   ORWL_REGISTER_ALIAS(T ## _init),             \
                   ORWL_REGISTER_ALIAS(T ## _destroy),          \
                   __VA_ARGS__)

#define ORWL_TYPE_DYNAMIC_INIT(T) orwl_register_init(ORWL_FTAB(T))

DECLARE_ORWL_DOMAIN(orwl_types);
void orwl_types_init(void);

#define ORWL_REGISTER_TYPE(T) ORWL_REGISTER_DATA(ORWL_FTAB(T))

/**
 ** @brief Register all types that are to be accessible through the
 ** registration mechanism.
 **
 ** This should look something like
 ** @code
 ** DEFINE_ORWL_TYPES(ORWL_REGISTER_TYPE(orwl_wq),
 **                   ORWL_REGISTER_TYPE(orwl_wh));
 ** @endcode
 ** The effect of this is that all the types mentioned here get a
 ** unique ID that makes them identifiable remotely.
 ** @see DECLARE_ORWL_TYPE_DYNAMIC and ::DEFINE_ORWL_TYPE_DYNAMIC on
 ** how to prepare a type to be visible through this system.
 **/
#define DEFINE_ORWL_TYPES(...)                  \
DEFINE_ORWL_DOMAIN(orwl_types, __VA_ARGS__)

/**
 ** @brief Get the ID of an object that is registered in an ::orwl_domain.
 **/
#define ORWL_OBJID(F) (orwl_register_id(ORWL_REGISTER(F)))

/**
 ** @brief Get the ID of a type that is registered with DECLARE_ORWL_TYPE_DYNAMIC().
 **/
#define ORWL_TYPEID(T) (orwl_register_id(ORWL_REGISTER(ORWL_FTAB(T))))


#endif 	    /* !ORWL_REGISTER_H_ */
