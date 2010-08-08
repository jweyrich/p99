/*
** test-prepro.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Jul 19 10:00:06 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/


#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include <stddef.h>

#include "p99_compiler.h"
#include "p99_int.h"

#include <stdio.h>
#include <signal.h>


static
char const* representation[4] = {
  [p99_signed_representation_invalid]
  = "it has an unknown representation for negative values that is not C99 compliant",
  [p99_signed_representation_magnitude]
  = "sign and magnitude",
  [p99_signed_representation_ones]
  = "one's complement",
  [p99_signed_representation_twos]
  = "two's complement",
};

#define SAYIT(T)                                                \
printf("%20s:\t%20jd\t...\t%20ju,\t%5ssigned%20s\n",            \
       #T,                                                      \
       (intmax_t)P99_TMIN(T),                                   \
       (uintmax_t)P99_TMAX(T),                                  \
       (P99_ISSIGNED(T) ? "" : "un"),                           \
       (!P99_ISSIGNED(T)                                        \
        ? ""                                                    \
        : representation[P99_SIGNED_REPRESENTATION(T)]))

typedef enum { a1, b1 } enum1;
typedef enum { a2 = -1, b2, c2 } enum2;


unsigned P99_FSYMB(toto)(unsigned a, P99_VA_ARGS(number));
#define toto(A, ...) P99_FSYMB(toto)(A, P99_LENGTH_VA_ARG(__VA_ARGS__))

unsigned P99_FSYMB(toto)(unsigned a, P99_VA_ARGS(number)) {
  unsigned ret = 0;
  va_list ap;
  va_start(ap, number);
  for (size_t i = 0; i < number; ++i) {
    ret += va_arg(ap, unsigned);
  }
  va_end(ap);
  return ret % a;
}

unsigned P99_FSYMB(tutu)(unsigned a, size_t number, unsigned *arr);
#define tutu(A, ...) P99_FSYMB(tutu)(A, P99_LENGTH_ARR_ARG(unsigned, __VA_ARGS__))

unsigned P99_FSYMB(tutu)(unsigned a, size_t number, unsigned *arr) {
  unsigned ret = 0;
  for (size_t i = 0; i < number; ++i) {
    ret += arr[i];
  }
  return ret % a;
}


unsigned toto_fixed(void) {
  return toto(3, 1, 3, 5, 7);
}

unsigned tutu_fixed(void) {
  return tutu(3, 1, 3, 5, 7);
}

static_inline
uintmax_t P99_FSYMB(p99_minu)(size_t number, uintmax_t const*const arr);
#define p99_minu(...) P99_FSYMB(p99_minu)(P99_LENGTH_ARR_ARG(uintmax_t, __VA_ARGS__))

static_inline
uintmax_t P99_FSYMB(p99_minu)(size_t number, uintmax_t const*const arr) {
  uintmax_t ret = arr[0];
  for (size_t i = 1; i < number; ++i) {
    if (arr[i] < ret) ret = arr[i];
  }
  return ret;
}

uintmax_t p99_minu_fixed(void) {
  return p99_minu(3, 1, 3, 5, 7, 3, 1, 3, 5, 0, 7ll, 3, 1, 3, 5, 7);
}


static_inline
intmax_t P99_FSYMB(p99_mins)(size_t number, intmax_t const*const arr);
#define p99_mins(...) P99_FSYMB(p99_mins)(P99_LENGTH_ARR_ARG(intmax_t, __VA_ARGS__))

static_inline
intmax_t P99_FSYMB(p99_mins)(size_t number, intmax_t const*const arr) {
  intmax_t ret = arr[0];
  for (size_t i = 1; i < number; ++i) {
    if (arr[i] < ret) ret = arr[i];
  }
  return ret;
}

int main(int argc, char** argv) {
  SAYIT(_Bool);
  SAYIT(char);
  SAYIT(unsigned char);
  SAYIT(signed char);
  SAYIT(unsigned short);
  SAYIT(signed short);
  SAYIT(unsigned);
  SAYIT(signed);
  SAYIT(unsigned long);
  SAYIT(signed long);
  SAYIT(unsigned long long);
  SAYIT(signed long long);
  SAYIT(uint8_t);
  SAYIT(int8_t);
  SAYIT(uint16_t);
  SAYIT(int16_t);
  SAYIT(uint32_t);
  SAYIT(int32_t);
  SAYIT(uint64_t);
  SAYIT(int64_t);
  SAYIT(wchar_t);
  SAYIT(size_t);
  SAYIT(sig_atomic_t);
  SAYIT(ptrdiff_t);
  SAYIT(uintptr_t);
  SAYIT(intptr_t);
  SAYIT(uintmax_t);
  SAYIT(intmax_t);
  SAYIT(enum1);
  SAYIT(enum2);
  assert(toto(3, 1, 3, 5, 7) == 1u);
  assert(tutu(3, 1, 3, 5, 7) == 1u);
}
