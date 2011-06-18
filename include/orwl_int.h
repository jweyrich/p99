/* This may look like nonsense, but it really is -*- mode: C -*-             */
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
#ifndef   	ORWL_INT_H_
# define   	ORWL_INT_H_

/**
 ** @file "orwl_int.h"
 ** @brief implement a compatibility layer for integer types.
 **/

#include "orwl_posix.h"
#include "orwl_new.h"
#include "p99_int.h"
#include "p99_typenames.h"

extern unsigned char const orwl_garb[16];

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
#define TGARB(T) ((T)(void*)orwl_garb)



#define DECLARE_BASIC(T)                                       \
DECLARE_NEW_DELETE(T)

#define DEFINE_BASIC(T)                                        \
P99_INSTANTIATE(T*, P99_PASTE2(T, _init), T *);                \
P99_INSTANTIATE(void, P99_PASTE2(T, _destroy), T*);            \
DEFINE_NEW_DELETE(T)

#define DECLARE_BASIC_TYPE(T)                                  \
DECLARE_BASIC(T);                                              \
DECLARE_BASIC(P99_PASTE2(T, _cptr));                           \
DECLARE_BASIC(P99_PASTE2(T, _ptr))

#define DEFINE_BASIC_TYPE(T)                                   \
DEFINE_BASIC(T);                                               \
DEFINE_BASIC(P99_PASTE2(T, _cptr));                            \
DEFINE_BASIC(P99_PASTE2(T, _ptr))



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

/** @addtogroup bool_specials @c bool specials
 ** @brief @c bool must be handled specially since it is not a @c
 ** typedef but a macro
 ** @{
 **/
DECLARE_BASIC_TYPE(_Bool);
#define bool_cptr_delete _Bool_cptr_delete
#define bool_cptr_destroy _Bool_cptr_destroy
#define bool_cptr_init _Bool_cptr_init
#define bool_cptr_vdelete _Bool_cptr_vdelete
#define bool_cptr_vnew _Bool_cptr_vnew
#define bool_cptr_vrealloc _Bool_cptr_vrealloc
#define bool_delete _Bool_delete
#define bool_destroy _Bool_destroy
#define bool_init _Bool_init
#define bool_ptr_delete _Bool_ptr_delete
#define bool_ptr_destroy _Bool_ptr_destroy
#define bool_ptr_init _Bool_ptr_init
#define bool_ptr_vdelete _Bool_ptr_vdelete
#define bool_ptr_vnew _Bool_ptr_vnew
#define bool_ptr_vrealloc _Bool_ptr_vrealloc
#define bool_vdelete _Bool_vdelete
#define bool_vnew _Bool_vnew
#define bool_vrealloc _Bool_vrealloc
/**
 ** @}
 **/

inline
char const* void_cptr2p(char* buf, void_cptr x) {
  sprintf(buf, "%p", x);
  return buf;
}

#ifdef DOXYGEN
#define O_RWL_DECLARE_ARI2STR(T, X, S, P)                                                           \
/*! @brief Return a `X' representation of @a x in @a buf. */                                        \
/*! Here `X' is taken as a format specifier as for @c printf. The output is eventually prefixed. */ \
  char const* P99_PASTE3(T, 2, X)(char* buf, T x);
#else
#define O_RWL_DECLARE_ARI2STR(T, X, S, P)                      \
  char const* P99_PASTE3(T, 2, X)(char* buf, T x)
#endif

#define DECLARE_ARI2STR(T)                                     \
  O_RWL_DECLARE_ARI2STR(T, d, , );                             \
  O_RWL_DECLARE_ARI2STR(T, o, 0, 0);                           \
  O_RWL_DECLARE_ARI2STR(T, u, , );                             \
  O_RWL_DECLARE_ARI2STR(T, x, , 0x);                           \
  O_RWL_DECLARE_ARI2STR(T, X, , 0x)


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


#define O_RWL_ALLO(N) (P99_LVAL(char[N]))

/**
 ** @brief convert @a x to unsigned decimal
 */
#define PRIu(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2u(O_RWL_ALLO(3*sizeof(sllong)),x),               \
        ushort2u(O_RWL_ALLO(3*sizeof(sllong)),x),              \
        unsigned2u(O_RWL_ALLO(3*sizeof(sllong)),x),            \
        ulong2u(O_RWL_ALLO(3*sizeof(sllong)),x),               \
        ullong2u(O_RWL_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to decimal, possibly signed */
#define PRId(x)                                                \
P99_CHOOSE5(x,                                                 \
        schar2d(O_RWL_ALLO(3*sizeof(sllong)),x),               \
        short2d(O_RWL_ALLO(3*sizeof(sllong)),x),               \
        signed2d(O_RWL_ALLO(3*sizeof(sllong)),x),              \
        long2d(O_RWL_ALLO(3*sizeof(sllong)),x),                \
        llong2d(O_RWL_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to octal */
#define PRIo(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2o(O_RWL_ALLO(3*sizeof(sllong)),x),               \
        ushort2o(O_RWL_ALLO(3*sizeof(sllong)),x),              \
        unsigned2o(O_RWL_ALLO(3*sizeof(sllong)),x),            \
        ulong2o(O_RWL_ALLO(3*sizeof(sllong)),x),               \
        ullong2o(O_RWL_ALLO(3*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using a...f for 10 to 15 */
#define PRIx(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2x(O_RWL_ALLO(2*sizeof(sllong)),x),               \
        ushort2x(O_RWL_ALLO(2*sizeof(sllong)),x),              \
        unsigned2x(O_RWL_ALLO(2*sizeof(sllong)),x),            \
        ulong2x(O_RWL_ALLO(2*sizeof(sllong)),x),               \
        ullong2x(O_RWL_ALLO(2*sizeof(sllong)),x))

/*! @brief convert @a x to hexadecimal using A...F for 10 to 15 */
#define PRIX(x)                                                \
P99_CHOOSE5(x,                                                 \
        uchar2X(O_RWL_ALLO(2*sizeof(sllong)),x),               \
        ushort2X(O_RWL_ALLO(2*sizeof(sllong)),x),              \
        unsigned2X(O_RWL_ALLO(2*sizeof(sllong)),x),            \
        ulong2X(O_RWL_ALLO(2*sizeof(sllong)),x),               \
        ullong2X(O_RWL_ALLO(2*sizeof(sllong)),x))

/*! @brief convert pointer @a x to hexadecimal */
#define PRIp(x) void_cptr2p(O_RWL_ALLO(2*sizeof(void*) + 3), x)

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

#define O_RWL_mfputs(F, ...) P99_FSYMB(mfputs)(F, P99_LENGTH_ARR_ARG(char const*const, __VA_ARGS__))

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
#define mfputs(...) O_RWL_mfputs(__VA_ARGS__)



#endif 	    /* !ORWL_INT_H_ */
