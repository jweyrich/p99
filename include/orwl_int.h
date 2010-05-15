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

/**
 ** @file "orwl_int.h"
 ** @brief implement a compatibility layer for integer types.
 **/

#include "orwl_new.h"

/* For each one word integer type have a signed and unsigned variant. */ \
#define _ONE_TOK_(T, NAME)                                              \
/*! @brief a `one token' abreviation for @c T */        \
/*! These type names are needed to have a valid naming scheme for functions. */ \
typedef T NAME

_ONE_TOK_(unsigned long, ulong);
_ONE_TOK_(signed long, slong);
_ONE_TOK_(unsigned int, uint);
_ONE_TOK_(signed int, sint);
_ONE_TOK_(unsigned short, ushort);
_ONE_TOK_(signed short, sshort);
_ONE_TOK_(unsigned char, uchar);
_ONE_TOK_(signed char, schar);
_ONE_TOK_(signed long long, sllong);
_ONE_TOK_(unsigned long long, ullong);

#define DECLARE_POINTER_TYPE(T)                 \
/*! @brief a pointer to T */                    \
typedef T *T ## _ptr;                           \
/*! @brief a @c const pointer to T */           \
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


#define DECLARE_BASIC(T)                                        \
/*! @brief initialize the object that @a id points to by 0. */  \
inline T* T ## _init(T *id) {                                   \
  *id = TNULL(T);                                               \
  return id;                                                    \
}                                                               \
/*! @brief destroy the object that @a id points to. */          \
inline void T ## _destroy(T *id) {                              \
  /* empty */                                                   \
}                                                               \
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

/** @brief convert a string to a double */
#define str2double(...) strtod(__VA_ARGS__)
/** @brief convert a string to a float */
#define str2float(...) strtof(__VA_ARGS__)
/** @brief convert a string to integer */
#define str2ulong(...) _STRTO(ulong, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2slong(...) _STRTO(slong, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2long(...) _STRTO(long, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint(...) _STRTO(uint, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2sint(...) _STRTO(sint, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int(...) _STRTO(int, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2ushort(...) _STRTO(ushort, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2short(...) _STRTO(short, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uchar(...) _STRTO(uchar, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2schar(...) _STRTO(schar, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2unsigned(...) _STRTO(unsigned, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2signed(...) _STRTO(signed, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2size_t(...) _STRTO(size_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2ssize_t(...) _STRTO(ssize_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2intmax_t(...) _STRTO(intmax_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uintmax_t(...) _STRTO(uintmax_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2intptr_t(...) _STRTO(intptr_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uintptr_t(...) _STRTO(uintptr_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2ptrdiff_t(...) _STRTO(ptrdiff_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int8_t(...) _STRTO(int8_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint8_t(...) _STRTO(uint8_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int16_t(...) _STRTO(int16_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint16_t(...) _STRTO(uint16_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int32_t(...) _STRTO(int32_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint32_t(...) _STRTO(uint32_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int64_t(...) _STRTO(int64_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint64_t(...) _STRTO(uint64_t, __VA_ARGS__)

inline
char const* void_cptr2p(char* buf, void_cptr x) {
  sprintf(buf, "%p", x);
  return buf;
}

#ifdef DOXYGEN
#define _DECLARE_ARI2STR(T, X, S, P)                                    \
/*! @brief Return a `X' representation of @a x in @a buf. */            \
/*! Here `X' is taken as a format specifier as for @c printf. The output is eventually prefixed. */ \
char const* T ## 2 ## X (char* buf, T x);
#else
#define _DECLARE_ARI2STR(T, X, S, P)            \
inline                                          \
 char const* PASTE3(T, 2, X)(char* buf, T x) {  \
  char* form = STRDUP(#P, PRI(T,X,S));          \
  sprintf(buf, form, x);                        \
  free(form);                                   \
  return buf;                                   \
}
#endif

#define DECLARE_ARI2STR(T)                      \
  _DECLARE_ARI2STR(T, d, , )                    \
  _DECLARE_ARI2STR(T, o, 0, 0)                  \
  _DECLARE_ARI2STR(T, u, , )                    \
  _DECLARE_ARI2STR(T, x, , 0x)                  \
  _DECLARE_ARI2STR(T, X, , 0x)


#define _DEFINE_ARI2STR(T, X)                     \
  char const* PASTE3(T, 2, X)(char* buf, T x)

#define DEFINE_ARI2STR(T)                      \
  _DEFINE_ARI2STR(T, d);                       \
  _DEFINE_ARI2STR(T, o);                       \
  _DEFINE_ARI2STR(T, u);                       \
  _DEFINE_ARI2STR(T, x);                       \
  _DEFINE_ARI2STR(T, X)

DECLARE_ARI2STR(schar)
DECLARE_ARI2STR(uchar)
DECLARE_ARI2STR(short)
DECLARE_ARI2STR(ushort)
DECLARE_ARI2STR(signed)
DECLARE_ARI2STR(unsigned)
DECLARE_ARI2STR(long)
DECLARE_ARI2STR(ulong)
DECLARE_ARI2STR(sllong)
DECLARE_ARI2STR(ullong)


#define _ALLO(N) ((char[N]){0})

/**
 ** @def PRIu
 ** @brief convert @a x to unsigned decimal
 */
#define PRIu(x)                                 \
CHOOSE5(x,                                      \
        uchar2u(_ALLO(3*sizeof(sllong)),x),      \
        ushort2u(_ALLO(3*sizeof(sllong)),x),     \
        unsigned2u(_ALLO(3*sizeof(sllong)),x),   \
        ulong2u(_ALLO(3*sizeof(sllong)),x),      \
        ullong2u(_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to decimal, possibly signed */
#define PRId(x)                                 \
CHOOSE5(x,                                      \
        schar2d(_ALLO(3*sizeof(sllong)),x),      \
        short2d(_ALLO(3*sizeof(sllong)),x),      \
        signed2d(_ALLO(3*sizeof(sllong)),x),     \
        long2d(_ALLO(3*sizeof(sllong)),x),       \
        llong2d(_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to octal */
#define PRIo(x)                                 \
CHOOSE5(x,                                      \
        uchar2o(_ALLO(3*sizeof(sllong)),x),      \
        ushort2o(_ALLO(3*sizeof(sllong)),x),     \
        unsigned2o(_ALLO(3*sizeof(sllong)),x),   \
        ulong2o(_ALLO(3*sizeof(sllong)),x),      \
        ullong2o(_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using a...f for 10 to 15 */
#define PRIx(x)                                 \
CHOOSE5(x,                                      \
        uchar2x(_ALLO(2*sizeof(sllong)),x),      \
        ushort2x(_ALLO(2*sizeof(sllong)),x),     \
        unsigned2x(_ALLO(2*sizeof(sllong)),x),   \
        ulong2x(_ALLO(2*sizeof(sllong)),x),      \
        ullong2x(_ALLO(2*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using A...F for 10 to 15 */
#define PRIX(x)                                 \
CHOOSE5(x,                                      \
        uchar2X(_ALLO(2*sizeof(sllong)),x),      \
        ushort2X(_ALLO(2*sizeof(sllong)),x),     \
        unsigned2X(_ALLO(2*sizeof(sllong)),x),   \
        ulong2X(_ALLO(2*sizeof(sllong)),x),      \
        ullong2X(_ALLO(2*sizeof(sllong)),x))

/*! @brief convert pointer @a x to hexadecimal */
#define PRIp(x) void_cptr2p(_ALLO(2*sizeof(void*) + 3), x)

/**
 ** @brief Output a series of @a n strings to file @a f.
 **
 ** This here only is a helper @ref mfputs which you should use
 ** instead.
 **/
inline
int mfputs_func(FILE* f, size_t n, char const*const* A) {
  int ret = 0;
  flockfile(f);
  for (size_t i = 0; i < n && ret != EOF; ++i)
    if (A[i]) {
      char const* p = A[i];
      char c;
      while ((c = *p)) {
        putc_unlocked(c, f);
        ++p;
      }
      ret += (p - A[i]);
    }
  funlockfile(f);
  return ret;
}

#define _mfputs(F, ...) mfputs_func(F, NARG(__VA_ARGS__), (char const*[]){__VA_ARGS__})


/**
 ** @brief Output a series of to a file given as first argument.
 **
 ** Use this as something like
 ** @code
 ** mfputs(sderr, "something ", SOME_STRING_VARIABLE, "something else");
 ** @endcode
 **/
#define mfputs(...) _mfputs(__VA_ARGS__)

#endif 	    /* !ORWL_INT_H_ */
