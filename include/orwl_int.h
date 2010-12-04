/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_INT_H_
# define   	ORWL_INT_H_

/**
 ** @file "orwl_int.h"
 ** @brief implement a compatibility layer for integer types.
 **/

#include "orwl_posix.h"
#include "orwl_new.h"
#include "p99_int.h"

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

/* For each one word integer type have a signed and unsigned variant. */
#define P00_ONE_TOK_(T, NAME)                                                   \
/*! @brief a `one token' abreviation for @c T */                                \
/*! These type names are needed to have a valid naming scheme for functions. */ \
typedef T NAME

P00_ONE_TOK_(unsigned long, ulong);
P00_ONE_TOK_(signed long, slong);
P00_ONE_TOK_(unsigned int, uint);
P00_ONE_TOK_(signed int, sint);
P00_ONE_TOK_(unsigned short, ushort);
P00_ONE_TOK_(signed short, sshort);
P00_ONE_TOK_(unsigned char, uchar);
P00_ONE_TOK_(signed char, schar);
P00_ONE_TOK_(long long, llong);
P00_ONE_TOK_(signed long long, sllong);
P00_ONE_TOK_(unsigned long long, ullong);

#define DECLARE_POINTER_TYPE(T)                                \
/*! @brief a pointer to T */                                   \
typedef T *P99_PASTE2(T, _ptr);                                \
/*! @brief a @c const pointer to T */                          \
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


#define DECLARE_BASIC(T)                                       \
/*! @brief initialize the object that @a id points to by 0. */ \
inline T* P99_PASTE2(T, _init)(T *id) {                        \
  if (id) *id = P99_LVAL(T);                                   \
  return id;                                                   \
}                                                              \
/*! @brief destroy the object that @a id points to. */         \
inline void P99_PASTE2(T, _destroy)(T*  id) {                  \
  /* empty */                                                  \
  (void)id;                                                    \
}                                                              \
DECLARE_NEW_DELETE(T)

#define DECLARE_BASIC_TYPE(T)                                  \
DECLARE_BASIC(T);                                              \
DECLARE_BASIC(P99_PASTE2(T, _cptr));                           \
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

inline
char const* void_cptr2p(char* buf, void_cptr x) {
  sprintf(buf, "%p", x);
  return buf;
}

#ifdef DOXYGEN
#define P00_DECLARE_ARI2STR(T, X, S, P)                                                             \
/*! @brief Return a `X' representation of @a x in @a buf. */                                        \
/*! Here `X' is taken as a format specifier as for @c printf. The output is eventually prefixed. */ \
  char const* P99_PASTE3(T, 2, X)(char* buf, T x);
#else
#define P00_DECLARE_ARI2STR(T, X, S, P)                        \
  char const* P99_PASTE3(T, 2, X)(char* buf, T x)
#endif

#define DECLARE_ARI2STR(T)                                     \
  P00_DECLARE_ARI2STR(T, d, , );                               \
  P00_DECLARE_ARI2STR(T, o, 0, 0);                             \
  P00_DECLARE_ARI2STR(T, u, , );                               \
  P00_DECLARE_ARI2STR(T, x, , 0x);                             \
  P00_DECLARE_ARI2STR(T, X, , 0x)


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


#define P00_ALLO(N) ((char[N]){0})

/**
 ** @brief convert @a x to unsigned decimal
 */
#define PRIu(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2u(P00_ALLO(3*sizeof(sllong)),x),                 \
        ushort2u(P00_ALLO(3*sizeof(sllong)),x),                \
        unsigned2u(P00_ALLO(3*sizeof(sllong)),x),              \
        ulong2u(P00_ALLO(3*sizeof(sllong)),x),                 \
        ullong2u(P00_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to decimal, possibly signed */
#define PRId(x)                                                \
P99_CHOOSE5(x,                                                 \
        schar2d(P00_ALLO(3*sizeof(sllong)),x),                 \
        short2d(P00_ALLO(3*sizeof(sllong)),x),                 \
        signed2d(P00_ALLO(3*sizeof(sllong)),x),                \
        long2d(P00_ALLO(3*sizeof(sllong)),x),                  \
        llong2d(P00_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to octal */
#define PRIo(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2o(P00_ALLO(3*sizeof(sllong)),x),                 \
        ushort2o(P00_ALLO(3*sizeof(sllong)),x),                \
        unsigned2o(P00_ALLO(3*sizeof(sllong)),x),              \
        ulong2o(P00_ALLO(3*sizeof(sllong)),x),                 \
        ullong2o(P00_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using a...f for 10 to 15 */
#define PRIx(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2x(P00_ALLO(2*sizeof(sllong)),x),                 \
        ushort2x(P00_ALLO(2*sizeof(sllong)),x),                \
        unsigned2x(P00_ALLO(2*sizeof(sllong)),x),              \
        ulong2x(P00_ALLO(2*sizeof(sllong)),x),                 \
        ullong2x(P00_ALLO(2*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using A...F for 10 to 15 */
#define PRIX(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2X(P00_ALLO(2*sizeof(sllong)),x),                 \
        ushort2X(P00_ALLO(2*sizeof(sllong)),x),                \
        unsigned2X(P00_ALLO(2*sizeof(sllong)),x),              \
        ulong2X(P00_ALLO(2*sizeof(sllong)),x),                 \
        ullong2X(P00_ALLO(2*sizeof(sllong)),x))

/*! @brief convert pointer @a x to hexadecimal */
#define PRIp(x) void_cptr2p(P00_ALLO(2*sizeof(void*) + 3), x)

/**
 ** @brief Output a series of @a n strings to file @a f.
 **
 ** This here only is a helper @ref mfputs which you should use
 ** instead.
 **/
inline
int P99_FSYMB(mfputs)(FILE* f, size_t n, char const*const*const A);

#if defined(POSIX_THREAD_SAFE_FUNCTIONS) && (POSIX_THREAD_SAFE_FUNCTIONS > 0)
inline
int P99_FSYMB(mfputs)(FILE* f, size_t n, char const*const*const A) {
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
#else
inline
int P99_FSYMB(mfputs)(FILE* f, size_t n, char const*const*const A) {
  int ret = 0;
  for (size_t i = 0; i < n && ret != EOF; ++i)
    if (A[i]) {
      char const* p = A[i];
      char c;
      while ((c = *p)) {
        putc(c, f);
        ++p;
      }
      ret += (p - A[i]);
    }
  return ret;
}
#endif

#define P00_mfputs(F, ...) P99_FSYMB(mfputs)(F, P99_LENGTH_ARR_ARG(char const*const, __VA_ARGS__))

/**
 ** @brief Output a series of strings to a @c FILE* given as first argument.
 **
 ** Use this as something like
 ** @code
 ** mfputs(stderr, "something ", SOME_STRING_VARIABLE, "something else");
 ** @endcode
 **
 ** This will put a lock on the @c FILE* argument and output all the
 ** string arguments in one go, undisturbed by the eventual output
 ** coming from another thread.
 **/
#define mfputs(...) P00_mfputs(__VA_ARGS__)



#endif 	    /* !ORWL_INT_H_ */
