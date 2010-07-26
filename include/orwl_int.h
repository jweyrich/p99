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

#include <stdio.h>
#include <string.h>
#include "p99_paste.h"
#include "orwl_new.h"

/**
 ** @brief Define an (almost) all purpose initializer
 **/
#define INITIALIZER INIT1

#define INIT0 (0)
#define INIT1 { 0 }
#define INIT2 { { 0 } }

/**
 ** @brief Typed @c NULL
 **
 ** Define a @c NULL initialized constant of type @a T.
 **/
#define TNULL(T) NULL1(T)

#define NULL0(T) ((T)INIT0)
#define NULL1(T) ((T)INIT1)
#define NULL2(T) ((T)INIT2)

/**
 ** @brief Typed ones
 **
 ** Define a all-one-bits initialized constant of integer type @a T.
 **/
#define TONES(T) (~TNULL(T))


extern char **environ;

/**
 ** @brief Invalidated pointer
 **
 ** Define a pointer that is guaranteed not to point to any location
 ** that might be user defined, neither @c static, @c auto or on the heap.
 **
 ** We use the @c environ pointer for such a purpose. Change to some
 ** other known global variable (not function) if this is not good
 ** enough. In contrast of using @c -1 or something similar, this is
 ** guaranteed to not point to user allocated space on any platform.
 **/
#define TGARB(T) ((T)(void*)environ)

/**
 ** @brief Signedness of a type
 **
 ** Determine if @a T corresponds to a signed integer type or not.
 **/
#define ISSIGNED(T) (TONES(T) < TNULL(T))

/**
 ** @brief Typed max value
 **
 ** Define the largest value that integer type @a T may hold.
 **/
#define TMAX(T) (~TMIN(T))

/**
 ** @brief Typed min value
 **
 ** Define the smallest value that integer type @a T may hold.
 **/
#define TMIN(T) (((T)ISSIGNED(T)) << ((sizeof(T)*CHAR_BIT)-1))

/* For each one word integer type have a signed and unsigned variant. */
#define P99__ONE_TOK_(T, NAME)                                          \
/*! @brief a `one token' abreviation for @c T */        \
/*! These type names are needed to have a valid naming scheme for functions. */ \
typedef T NAME

P99__ONE_TOK_(unsigned long, ulong);
P99__ONE_TOK_(signed long, slong);
P99__ONE_TOK_(unsigned int, uint);
P99__ONE_TOK_(signed int, sint);
P99__ONE_TOK_(unsigned short, ushort);
P99__ONE_TOK_(signed short, sshort);
P99__ONE_TOK_(unsigned char, uchar);
P99__ONE_TOK_(signed char, schar);
P99__ONE_TOK_(long long, llong);
P99__ONE_TOK_(signed long long, sllong);
P99__ONE_TOK_(unsigned long long, ullong);

#define DECLARE_POINTER_TYPE(T)                 \
/*! @brief a pointer to T */                    \
typedef T *P99_PASTE2(T, _ptr);                 \
/*! @brief a @c const pointer to T */           \
typedef T const*P99_PASTE2(T, _cptr)

DECLARE_POINTER_TYPE(void);
DECLARE_POINTER_TYPE(double);
DECLARE_POINTER_TYPE(float);

DECLARE_POINTER_TYPE(ullong);
DECLARE_POINTER_TYPE(sllong);
DECLARE_POINTER_TYPE(llong);
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
inline T* P99_PASTE2(T, _init)(T *id) {                         \
  if (id) *id = TNULL(T);                                       \
  return id;                                                    \
}                                                               \
/*! @brief destroy the object that @a id points to. */          \
 inline void P99_PASTE2(T, _destroy)(T*  id) {                  \
  /* empty */                                                   \
}                                                               \
DECLARE_NEW_DELETE(T)

#define DECLARE_BASIC_TYPE(T)                   \
DECLARE_BASIC(T);                               \
DECLARE_BASIC(P99_PASTE2(T, _cptr));            \
DECLARE_BASIC(P99_PASTE2(T, _ptr))

DECLARE_BASIC(void_ptr);
DECLARE_BASIC(void_cptr);

DECLARE_BASIC_TYPE(double);
DECLARE_BASIC_TYPE(float);
DECLARE_BASIC_TYPE(ullong);
DECLARE_BASIC_TYPE(sllong);
DECLARE_BASIC_TYPE(llong);
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

#define P99__STRTO(T, ...)                      \
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
#define str2ulong(...) P99__STRTO(ulong, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2slong(...) P99__STRTO(slong, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2long(...) P99__STRTO(long, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint(...) P99__STRTO(uint, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2sint(...) P99__STRTO(sint, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int(...) P99__STRTO(int, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2ushort(...) P99__STRTO(ushort, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2short(...) P99__STRTO(short, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uchar(...) P99__STRTO(uchar, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2schar(...) P99__STRTO(schar, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2unsigned(...) P99__STRTO(unsigned, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2signed(...) P99__STRTO(signed, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2size_t(...) P99__STRTO(size_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2ssize_t(...) P99__STRTO(ssize_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2intmax_t(...) P99__STRTO(intmax_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uintmax_t(...) P99__STRTO(uintmax_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2intptr_t(...) P99__STRTO(intptr_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uintptr_t(...) P99__STRTO(uintptr_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2ptrdiff_t(...) P99__STRTO(ptrdiff_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int8_t(...) P99__STRTO(int8_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint8_t(...) P99__STRTO(uint8_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int16_t(...) P99__STRTO(int16_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint16_t(...) P99__STRTO(uint16_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int32_t(...) P99__STRTO(int32_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint32_t(...) P99__STRTO(uint32_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2int64_t(...) P99__STRTO(int64_t, __VA_ARGS__)
/** @brief convert a string to integer */
#define str2uint64_t(...) P99__STRTO(uint64_t, __VA_ARGS__)

inline
char const* void_cptr2p(char* buf, void_cptr x) {
  sprintf(buf, "%p", x);
  return buf;
}

#ifdef DOXYGEN
#define P99__DECLARE_ARI2STR(T, X, S, P)                                \
/*! @brief Return a `X' representation of @a x in @a buf. */            \
/*! Here `X' is taken as a format specifier as for @c printf. The output is eventually prefixed. */ \
  char const* P99_PASTE3(T, 2, X)(char* buf, T x);
#else
#define P99__DECLARE_ARI2STR(T, X, S, P)            \
  char const* P99_PASTE3(T, 2, X)(char* buf, T x)
#endif

#define DECLARE_ARI2STR(T)                      \
  P99__DECLARE_ARI2STR(T, d, , );               \
  P99__DECLARE_ARI2STR(T, o, 0, 0);             \
  P99__DECLARE_ARI2STR(T, u, , );               \
  P99__DECLARE_ARI2STR(T, x, , 0x);             \
  P99__DECLARE_ARI2STR(T, X, , 0x)


DECLARE_ARI2STR(schar);
DECLARE_ARI2STR(uchar);
DECLARE_ARI2STR(short);
DECLARE_ARI2STR(ushort);
DECLARE_ARI2STR(signed);
DECLARE_ARI2STR(unsigned);
DECLARE_ARI2STR(long);
DECLARE_ARI2STR(ulong);
DECLARE_ARI2STR(llong);
DECLARE_ARI2STR(sllong);
DECLARE_ARI2STR(ullong);


#define P99__ALLO(N) ((char[N]){0})

/**
 ** @brief convert @a x to unsigned decimal
 */
#define PRIu(x)                                 \
CHOOSE5(x,                                      \
        uchar2u(P99__ALLO(3*sizeof(sllong)),x),      \
        ushort2u(P99__ALLO(3*sizeof(sllong)),x),     \
        unsigned2u(P99__ALLO(3*sizeof(sllong)),x),   \
        ulong2u(P99__ALLO(3*sizeof(sllong)),x),      \
        ullong2u(P99__ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to decimal, possibly signed */
#define PRId(x)                                 \
CHOOSE5(x,                                      \
        schar2d(P99__ALLO(3*sizeof(sllong)),x),      \
        short2d(P99__ALLO(3*sizeof(sllong)),x),      \
        signed2d(P99__ALLO(3*sizeof(sllong)),x),     \
        long2d(P99__ALLO(3*sizeof(sllong)),x),       \
        llong2d(P99__ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to octal */
#define PRIo(x)                                 \
CHOOSE5(x,                                      \
        uchar2o(P99__ALLO(3*sizeof(sllong)),x),      \
        ushort2o(P99__ALLO(3*sizeof(sllong)),x),     \
        unsigned2o(P99__ALLO(3*sizeof(sllong)),x),   \
        ulong2o(P99__ALLO(3*sizeof(sllong)),x),      \
        ullong2o(P99__ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using a...f for 10 to 15 */
#define PRIx(x)                                 \
CHOOSE5(x,                                      \
        uchar2x(P99__ALLO(2*sizeof(sllong)),x),      \
        ushort2x(P99__ALLO(2*sizeof(sllong)),x),     \
        unsigned2x(P99__ALLO(2*sizeof(sllong)),x),   \
        ulong2x(P99__ALLO(2*sizeof(sllong)),x),      \
        ullong2x(P99__ALLO(2*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using A...F for 10 to 15 */
#define PRIX(x)                                 \
CHOOSE5(x,                                      \
        uchar2X(P99__ALLO(2*sizeof(sllong)),x),      \
        ushort2X(P99__ALLO(2*sizeof(sllong)),x),     \
        unsigned2X(P99__ALLO(2*sizeof(sllong)),x),   \
        ulong2X(P99__ALLO(2*sizeof(sllong)),x),      \
        ullong2X(P99__ALLO(2*sizeof(sllong)),x))

/*! @brief convert pointer @a x to hexadecimal */
#define PRIp(x) void_cptr2p(P99__ALLO(2*sizeof(void*) + 3), x)

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

#define P99__mfputs(F, ...) mfputs_func(F, P99_NARG(__VA_ARGS__), (char const*[]){__VA_ARGS__})


/**
 ** @brief Output a series of to a file given as first argument.
 **
 ** Use this as something like
 ** @code
 ** mfputs(sderr, "something ", SOME_STRING_VARIABLE, "something else");
 ** @endcode
 **/
#define mfputs(...) P99__mfputs(__VA_ARGS__)

#endif 	    /* !ORWL_INT_H_ */
