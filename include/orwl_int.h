/*
** orwl_int.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Mar 20 13:15:14 2010 Jens Gustedt
** Last update Sat Mar 20 13:15:14 2010 Jens Gustedt
*/

#ifndef   	ORWL_INT_H_
# define   	ORWL_INT_H_

#include <stdint.h>
#include <stddef.h>
#include "orwl_macro.h"
#include "orwl_new.h"

/* For each one word integer type have a signed and unsigned variant. */
typedef unsigned long ulong;
typedef signed long slong;
typedef unsigned int uint;
typedef signed int sint;
typedef unsigned short ushort;
typedef signed short sshort;
typedef unsigned char uchar;
typedef signed char schar;

#define DECLARE_POINTER_TYPE(T)                 \
typedef T *T ## _ptr;                           \
typedef T const*T ## _cptr

DECLARE_POINTER_TYPE(void);
DECLARE_POINTER_TYPE(double);
DECLARE_POINTER_TYPE(float);

DECLARE_POINTER_TYPE(long);
DECLARE_POINTER_TYPE(ulong);
DECLARE_POINTER_TYPE(slong);
DECLARE_POINTER_TYPE(int);
DECLARE_POINTER_TYPE(uint);
DECLARE_POINTER_TYPE(sint);
DECLARE_POINTER_TYPE(short);
DECLARE_POINTER_TYPE(ushort);
DECLARE_POINTER_TYPE(sshort);
DECLARE_POINTER_TYPE(char);
DECLARE_POINTER_TYPE(uchar);
DECLARE_POINTER_TYPE(schar);

DECLARE_POINTER_TYPE(signed);
DECLARE_POINTER_TYPE(unsigned);

DECLARE_POINTER_TYPE(size_t);
DECLARE_POINTER_TYPE(ssize_t);
DECLARE_POINTER_TYPE(intmax_t);
DECLARE_POINTER_TYPE(uintmax_t);
DECLARE_POINTER_TYPE(intptr_t);
DECLARE_POINTER_TYPE(uintptr_t);
DECLARE_POINTER_TYPE(ptrdiff_t);


#define DECLARE_BASIC(T)                        \
inline void T ## _init(T *id) {                 \
  *id = TNULL(T);                               \
}                                               \
inline void T ## _destroy(T *id) {              \
  /* empty */                                   \
}                                               \
DECLARE_NEW_DELETE(T)

#define DECLARE_BASIC_TYPE(T)                   \
DECLARE_BASIC(T);                               \
DECLARE_BASIC(T ## _cptr);                      \
DECLARE_BASIC(T ## _ptr)

#define DEFINE_BASIC(T)                         \
void T ## _init(T *id);                         \
void T ## _destroy(T *id);                      \
DEFINE_NEW_DELETE(T)

#define DEFINE_BASIC_TYPE(T)                    \
DEFINE_BASIC(T);                                \
DEFINE_BASIC(T ## _cptr);                       \
DEFINE_BASIC(T ## _ptr)

DECLARE_BASIC(void_ptr);
DECLARE_BASIC(void_cptr);

DECLARE_BASIC_TYPE(double);
DECLARE_BASIC_TYPE(float);
DECLARE_BASIC_TYPE(ulong);
DECLARE_BASIC_TYPE(slong);
DECLARE_BASIC_TYPE(long);
DECLARE_BASIC_TYPE(uint);
DECLARE_BASIC_TYPE(sint);
DECLARE_BASIC_TYPE(int);
DECLARE_BASIC_TYPE(ushort);
DECLARE_BASIC_TYPE(short);
DECLARE_BASIC_TYPE(uchar);
DECLARE_BASIC_TYPE(schar);
DECLARE_BASIC_TYPE(char);

DECLARE_BASIC_TYPE(unsigned);
DECLARE_BASIC_TYPE(signed);

DECLARE_BASIC_TYPE(size_t);
DECLARE_BASIC_TYPE(ssize_t);
DECLARE_BASIC_TYPE(intmax_t);
DECLARE_BASIC_TYPE(uintmax_t);
DECLARE_BASIC_TYPE(intptr_t);
DECLARE_BASIC_TYPE(uintptr_t);
DECLARE_BASIC_TYPE(ptrdiff_t);


#endif 	    /* !ORWL_INT_H_ */
