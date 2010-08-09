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
#include "p99_args.h"

#include <stdio.h>

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
  assert(toto(3, 1, 3, 5, 7) == 1u);
  assert(tutu(3, 1, 3, 5, 7) == 1u);
}
