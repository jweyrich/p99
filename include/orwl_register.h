/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_REGISTER_H_
# define   	ORWL_REGISTER_H_

#include "orwl_int.h"
#include "p99_enum.h"
#include "p99_id.h"
#include "orwl_document.h"

P99_DECLARE_STRUCT(orwl_register);
typedef orwl_register const*const orwl_domain;
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

#define ORWL_REGISTER_INITIALIZER P99_INIT

DOCUMENT_INIT(orwl_register)
orwl_register const* orwl_register_init(orwl_register const* field);

/**
 ** @memberof orwl_register
 **/
inline size_t orwl_register_id(orwl_register const* field) {
  if (!(field->regptr))
    orwl_register_init(field);
  ptrdiff_t ret = field - field->start;
  return ret;
}

/**
 ** @memberof orwl_register
 **/
inline
void *orwl_register_get(orwl_register const* field) {
  return field->dptr;
}

/**
 ** @memberof orwl_register
 **/
inline
void orwl_register_call(orwl_register const* field, void* arg) {
  orwl_plain fptr= field->fptr;
  if (fptr) fptr(arg);
}

/**
 ** @memberof orwl_register
 **/
inline
void orwl_domain_call(orwl_domain domain, size_t id, void *arg) {
  orwl_register const* field = domain + id;
  orwl_register_call(field, arg);
}


#define P00_ORWL_ALIAS(NAME) P99_PASTE3(p00_, NAME, _alias)
#define ORWL_ALIAS(NAME) P00_ORWL_ALIAS(NAME)

#define P00_ORWL_REGISTER(NAME) P99_PASTE3(p00_, NAME, _regptr)
#define ORWL_REGISTER(NAME) P00_ORWL_REGISTER(NAME)

#define P00_ORWL_FTAB(NAME) P99_PASTE3(p00_, NAME, _ftab)
#define ORWL_FTAB(NAME) P00_ORWL_FTAB(NAME)

#define ORWL_DOMAIN_TABLE(NAME) P99_PASTE2(p00_domain_tab_, NAME)

#define DECLARE_ORWL_REGISTER(NAME)                            \
extern orwl_register const*ORWL_REGISTER(NAME)

#define DEFINE_ORWL_REGISTER(NAME)                             \
orwl_register const*ORWL_REGISTER(NAME) = 0

#define DECLARE_ORWL_REGISTER_ALIAS(NAME, T)                   \
extern void ORWL_ALIAS(NAME)(T *x);                            \
extern orwl_register const*ORWL_REGISTER(NAME)

#define DEFINE_ORWL_REGISTER_ALIAS(NAME, T)                    \
void ORWL_ALIAS(NAME)(T *x) {  NAME(x); }                      \
orwl_register const*ORWL_REGISTER(NAME) = 0

#define ORWL_REGISTER_FUNCTION(NAME, T) {                      \
.start = ORWL_DOMAIN_TABLE(ORWL_FTAB(T)),                      \
  .regptr = &ORWL_REGISTER(NAME),                              \
      .fptr = (orwl_plain)NAME                                 \
     }

#define ORWL_REGISTER_ALIAS(NAME, T) {                         \
.start = ORWL_DOMAIN_TABLE(ORWL_FTAB(T)),                      \
  .regptr = &ORWL_REGISTER(NAME),                              \
      .fptr = (orwl_plain)ORWL_ALIAS(NAME)                     \
     }

#define ORWL_REGISTER_DATA(NAME, T) {                          \
.start = ORWL_DOMAIN_TABLE(ORWL_FTAB(T)),                      \
  .regptr = &ORWL_REGISTER(NAME),                              \
      .dptr = (void*)&NAME                                     \
     }

#define DECLARE_ORWL_DOMAIN(NAME) extern orwl_domain NAME

#define DEFINE_ORWL_DOMAIN(NAME, ...)                                                               \
static orwl_register const ORWL_DOMAIN_TABLE(NAME)[] = { __VA_ARGS__ , ORWL_REGISTER_INITIALIZER }; \
orwl_domain NAME = ORWL_DOMAIN_TABLE(NAME)

/**
 ** @brief Register the functions of type @a T to be able to identify
 ** them remotely.
 **
 ** @see DEFINE_ORWL_TYPE_DYNAMIC to know what functions are / can be registered.
 **/
#define DECLARE_ORWL_TYPE_DYNAMIC(T)                           \
DECLARE_ORWL_DOMAIN(ORWL_FTAB(T));                             \
DECLARE_ORWL_REGISTER(ORWL_FTAB(T));                           \
DECLARE_ORWL_REGISTER(P99_PASTE2(T, _sizeof));                 \
DECLARE_ORWL_REGISTER(P99_PASTE2(T, _typename));               \
DECLARE_ORWL_REGISTER(P99_PASTE2(T, _init));                   \
DECLARE_ORWL_REGISTER(P99_PASTE2(T, _destroy))

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
#define DEFINE_ORWL_TYPE_DYNAMIC(T, ...)                            \
DEFINE_ORWL_REGISTER(ORWL_FTAB(T));                                 \
DEFINE_ORWL_REGISTER(P99_PASTE2(T, _sizeof));                       \
static size_t const P99_PASTE2(T, _sizeof) = sizeof(T);             \
DEFINE_ORWL_REGISTER(P99_PASTE2(T, _typename));                     \
static char const P99_PASTE2(T, _typename)[] = #T;                  \
DEFINE_ORWL_REGISTER_ALIAS(P99_PASTE2(T, _init), T);                \
DEFINE_ORWL_REGISTER_ALIAS(P99_PASTE2(T, _destroy), T);             \
DEFINE_ORWL_DOMAIN(ORWL_FTAB(T),                                    \
                   ORWL_REGISTER_DATA(P99_PASTE2(T, _sizeof), T),   \
                   ORWL_REGISTER_DATA(P99_PASTE2(T, _typename), T), \
                   ORWL_REGISTER_ALIAS(P99_PASTE2(T, _init), T),    \
                   ORWL_REGISTER_ALIAS(P99_PASTE2(T, _destroy), T), \
                   __VA_ARGS__)

#define ORWL_TYPE_DYNAMIC_INIT(T) orwl_register_init(ORWL_FTAB(T))

DECLARE_ORWL_DOMAIN(orwl_types);
void orwl_types_init(void);

#define ORWL_REGISTER_TYPE(T) ORWL_REGISTER_DATA(ORWL_FTAB(T), orwl_types)

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
#define DEFINE_ORWL_TYPES(...)                                 \
  DEFINE_ORWL_DOMAIN(ORWL_FTAB(orwl_types), __VA_ARGS__)

DECLARE_ORWL_DOMAIN(ORWL_FTAB(orwl_types));

/**
 ** @brief Get the ID of an object that is registered in an ::orwl_domain.
 **/
#define ORWL_OBJID(F) (orwl_register_id(ORWL_REGISTER(F)))

/**
 ** @brief Get the ID of a type that is registered with DECLARE_ORWL_TYPE_DYNAMIC().
 **/
#define ORWL_TYPEID(T) (orwl_register_id(ORWL_REGISTER(ORWL_FTAB(T))))


#endif 	    /* !ORWL_REGISTER_H_ */
