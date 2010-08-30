/*
** orwl_int.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Mar 20 13:17:50 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_int.h"
#include "p99_map.h"

#define DEFINE_BASIC(T)                         \
T* P99_PASTE2(T, _init)(T *id);                 \
void P99_PASTE2(T, _destroy)(T* id);            \
DEFINE_NEW_DELETE(T)

#define DEFINE_BASIC_TYPE(T)                    \
DEFINE_BASIC(T);                                \
DEFINE_BASIC(P99_PASTE2(T, _cptr));                 \
DEFINE_BASIC(P99_PASTE2(T, _ptr))

#define P99__DEFINE_ARI2STR(T, X, S, P)                 \
char const* P99_PASTE3(T, 2, X)(char* buf, T x) {       \
  char* form = P99_STRDUP(#P, P99_PRI(T,X,S));          \
  sprintf(buf, form, x);                                \
  free(form);                                           \
  return buf;                                           \
}                                                       \
char const* P99_PASTE3(T, 2, X)(char* buf, T x)

#define DEFINE_ARI2STR(T)                       \
  P99__DEFINE_ARI2STR(T, d, , );                \
  P99__DEFINE_ARI2STR(T, o, 0, 0);              \
  P99__DEFINE_ARI2STR(T, u, , );                \
  P99__DEFINE_ARI2STR(T, x, , 0x);              \
  P99__DEFINE_ARI2STR(T, X, , 0x)

DEFINE_BASIC(void_ptr);
DEFINE_BASIC(void_cptr);

DEFINE_BASIC_TYPE(double);
DEFINE_BASIC_TYPE(float);
DEFINE_BASIC_TYPE(ulong);
DEFINE_BASIC_TYPE(slong);
DEFINE_BASIC_TYPE(long);
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

char const* void_cptr2p(char* buf, void_cptr x);

int P99_FSYMB(mfputs)(FILE* f, size_t n, char const*const*const A);
