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
#include "orwl_int.h"
#include "p99_map.h"
#include "p99_id.h"

#define DEFINE_BASIC(T)                                        \
  P99_INSTANTIATE(T*, P99_PASTE2(T, _init), T *);              \
  P99_INSTANTIATE(void, P99_PASTE2(T, _destroy), T*);        \
DEFINE_NEW_DELETE(T)

#define DEFINE_BASIC_TYPE(T)                                   \
DEFINE_BASIC(T);                                               \
DEFINE_BASIC(P99_PASTE2(T, _cptr));                            \
DEFINE_BASIC(P99_PASTE2(T, _ptr))

#define P00_DEFINE_ARI2STR(T, X, S, P)                         \
char const* P99_PASTE3(T, 2, X)(char* buf, T x) {              \
  char* form = P99_STRDUP(#P, P99_PRI(T,X,S));                 \
  sprintf(buf, form, x);                                       \
  free(form);                                                  \
  return buf;                                                  \
}                                                              \
P99_MACRO_END(define_ari2str, T, X)

#define DEFINE_ARI2STR(T)                                      \
  P00_DEFINE_ARI2STR(T, d, , );                                \
  P00_DEFINE_ARI2STR(T, o, 0, 0);                              \
  P00_DEFINE_ARI2STR(T, u, , );                                \
  P00_DEFINE_ARI2STR(T, x, , 0x);                              \
  P00_DEFINE_ARI2STR(T, X, , 0x)

DEFINE_BASIC(void_ptr);
DEFINE_BASIC(void_cptr);

DEFINE_BASIC_TYPE(double);
DEFINE_BASIC_TYPE(float);
DEFINE_BASIC_TYPE(ulong);
DEFINE_BASIC_TYPE(slong);
DEFINE_BASIC_TYPE(long);
DEFINE_BASIC_TYPE(llong);
DEFINE_BASIC_TYPE(ullong);
DEFINE_BASIC_TYPE(sllong);
DEFINE_BASIC_TYPE(uint);
DEFINE_BASIC_TYPE(sint);
DEFINE_BASIC_TYPE(int);
DEFINE_BASIC_TYPE(ushort);
DEFINE_BASIC_TYPE(short);
DEFINE_BASIC_TYPE(uchar);
DEFINE_BASIC_TYPE(schar);
DEFINE_BASIC_TYPE(char);

DEFINE_BASIC_TYPE(unsigned);
DEFINE_BASIC_TYPE(signed);

DEFINE_BASIC_TYPE(size_t);
DEFINE_BASIC_TYPE(ssize_t);
DEFINE_BASIC_TYPE(intmax_t);
DEFINE_BASIC_TYPE(uintmax_t);
DEFINE_BASIC_TYPE(intptr_t);
DEFINE_BASIC_TYPE(uintptr_t);
DEFINE_BASIC_TYPE(ptrdiff_t);
DEFINE_BASIC_TYPE(int8_t);
DEFINE_BASIC_TYPE(uint8_t);
DEFINE_BASIC_TYPE(int16_t);
DEFINE_BASIC_TYPE(uint16_t);
DEFINE_BASIC_TYPE(int32_t);
DEFINE_BASIC_TYPE(uint32_t);
DEFINE_BASIC_TYPE(int64_t);
DEFINE_BASIC_TYPE(uint64_t);

DEFINE_ARI2STR(schar);
DEFINE_ARI2STR(uchar);
DEFINE_ARI2STR(short);
DEFINE_ARI2STR(ushort);
DEFINE_ARI2STR(signed);
DEFINE_ARI2STR(unsigned);
DEFINE_ARI2STR(long);
DEFINE_ARI2STR(ulong);
DEFINE_ARI2STR(llong);
DEFINE_ARI2STR(sllong);
DEFINE_ARI2STR(ullong);

P99_INSTANTIATE(char const*, void_cptr2p, char*, void_cptr);
P99_INSTANTIATE(int, P99_FSYMB(mfputs), FILE*, size_t, char const*const*const);
