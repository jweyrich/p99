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
#include "orwl_posix_default.h"

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
DECLARE_POINTER_TYPE(int8_t);
DECLARE_POINTER_TYPE(uint8_t);
DECLARE_POINTER_TYPE(int16_t);
DECLARE_POINTER_TYPE(uint16_t);
DECLARE_POINTER_TYPE(int32_t);
DECLARE_POINTER_TYPE(uint32_t);
DECLARE_POINTER_TYPE(int64_t);
DECLARE_POINTER_TYPE(uint64_t);


#define DECLARE_BASIC(T)                        \
inline T* T ## _init(T *id) {                   \
  *id = TNULL(T);                               \
  return id;                                    \
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
T* T ## _init(T *id);                           \
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
DECLARE_BASIC_TYPE(int8_t);
DECLARE_BASIC_TYPE(uint8_t);
DECLARE_BASIC_TYPE(int16_t);
DECLARE_BASIC_TYPE(uint16_t);
DECLARE_BASIC_TYPE(int32_t);
DECLARE_BASIC_TYPE(uint32_t);
DECLARE_BASIC_TYPE(int64_t);
DECLARE_BASIC_TYPE(uint64_t);

#define _STRTO(T, ...)                          \
(ISSIGNED(T)                                    \
 ? ((sizeof(T) == sizeof(long))                 \
    ? (T)strtol(__VA_ARGS__)                    \
    : (T)strtoll(__VA_ARGS__))                  \
 : ((sizeof(T) == sizeof(unsigned long))        \
    ? (T)strtoul(__VA_ARGS__)                   \
    : (T)strtoull(__VA_ARGS__)))

#define str2dobule(...) strtod(__VA_ARGS__)
#define str2float(...) strtof(__VA_ARGS__)
#define str2ulong(...) _STRTO(ulong, __VA_ARGS__)
#define str2slong(...) _STRTO(slong, __VA_ARGS__)
#define str2long(...) _STRTO(long, __VA_ARGS__)
#define str2uint(...) _STRTO(uint, __VA_ARGS__)
#define str2sint(...) _STRTO(sint, __VA_ARGS__)
#define str2int(...) _STRTO(int, __VA_ARGS__)
#define str2ushort(...) _STRTO(ushort, __VA_ARGS__)
#define str2short(...) _STRTO(short, __VA_ARGS__)
#define str2uchar(...) _STRTO(uchar, __VA_ARGS__)
#define str2schar(...) _STRTO(schar, __VA_ARGS__)
#define str2unsigned(...) _STRTO(unsigned, __VA_ARGS__)
#define str2signed(...) _STRTO(signed, __VA_ARGS__)
#define str2size_t(...) _STRTO(size_t, __VA_ARGS__)
#define str2ssize_t(...) _STRTO(ssize_t, __VA_ARGS__)
#define str2intmax_t(...) _STRTO(intmax_t, __VA_ARGS__)
#define str2uintmax_t(...) _STRTO(uintmax_t, __VA_ARGS__)
#define str2intptr_t(...) _STRTO(intptr_t, __VA_ARGS__)
#define str2uintptr_t(...) _STRTO(uintptr_t, __VA_ARGS__)
#define str2ptrdiff_t(...) _STRTO(ptrdiff_t, __VA_ARGS__)
#define str2int8_t(...) _STRTO(int8_t, __VA_ARGS__)
#define str2uint8_t(...) _STRTO(uint8_t, __VA_ARGS__)
#define str2int16_t(...) _STRTO(int16_t, __VA_ARGS__)
#define str2uint16_t(...) _STRTO(uint16_t, __VA_ARGS__)
#define str2int32_t(...) _STRTO(int32_t, __VA_ARGS__)
#define str2uint32_t(...) _STRTO(uint32_t, __VA_ARGS__)
#define str2int64_t(...) _STRTO(int64_t, __VA_ARGS__)
#define str2uint64_t(...) _STRTO(uint64_t, __VA_ARGS__)



#endif 	    /* !ORWL_INT_H_ */
