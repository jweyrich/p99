/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_id.h"
#include "p99_args.h"
#include "p99_map.h"
#include "p99_defarg.h"

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

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

#define P00_ARR_OP_MINMAX(NAME, TYPE, SUFF, OP)                                         \
 TYPE P99_FSYMB(P99_PASTE4(p99_, NAME, _, SUFF))(size_t number, TYPE const*const arr) { \
    TYPE ret = arr[0];                                                                  \
    for (size_t i = 1; i < number; ++i)                                                 \
      if (arr[i] OP ret) ret = arr[i];                                                  \
    return ret;                                                                         \
}                                                                                       \
P99_MACRO_END(_arr_op_minmax_, NAME, _, SUFF)


P00_ARR_OP_MINMAX(min, unsigned char, hhu, <);
P00_ARR_OP_MINMAX(min, unsigned short, hu, <);
P00_ARR_OP_MINMAX(min, unsigned, u, <);
P00_ARR_OP_MINMAX(min, unsigned long, lu, <);
P00_ARR_OP_MINMAX(min, unsigned long long, llu, <);
P00_ARR_OP_MINMAX(min, uintmax_t, ju, <);
P00_ARR_OP_MINMAX(min, size_t, zu, <);
P00_ARR_OP_MINMAX(min, uint8_t, 8u, <);
P00_ARR_OP_MINMAX(min, uint16_t, 16u, <);
P00_ARR_OP_MINMAX(min, uint32_t, 32u, <);
P00_ARR_OP_MINMAX(min, uint64_t, 64u, <);

P00_ARR_OP_MINMAX(min, signed char, hhi, <);
P00_ARR_OP_MINMAX(min, signed short, hi, <);
P00_ARR_OP_MINMAX(min, signed, i, <);
P00_ARR_OP_MINMAX(min, signed long, li, <);
P00_ARR_OP_MINMAX(min, signed long long, lli, <);
P00_ARR_OP_MINMAX(min, intmax_t, ji, <);
P00_ARR_OP_MINMAX(min, ptrdiff_t, ti, <);
P00_ARR_OP_MINMAX(min, int8_t, 8i, <);
P00_ARR_OP_MINMAX(min, int16_t, 16i, <);
P00_ARR_OP_MINMAX(min, int32_t, 32i, <);
P00_ARR_OP_MINMAX(min, int64_t, 64i, <);

P00_ARR_OP_MINMAX(min, float, f, <);
P00_ARR_OP_MINMAX(min, double, d, <);
P00_ARR_OP_MINMAX(min, long double, l, <);

P00_ARR_OP_MINMAX(max, unsigned char, hhu, >);
P00_ARR_OP_MINMAX(max, unsigned short, hu, >);
P00_ARR_OP_MINMAX(max, unsigned, u, >);
P00_ARR_OP_MINMAX(max, unsigned long, lu, >);
P00_ARR_OP_MINMAX(max, unsigned long long, llu, >);
P00_ARR_OP_MINMAX(max, uintmax_t, ju, >);
P00_ARR_OP_MINMAX(max, size_t, zu, >);
P00_ARR_OP_MINMAX(max, uint8_t, 8u, >);
P00_ARR_OP_MINMAX(max, uint16_t, 16u, >);
P00_ARR_OP_MINMAX(max, uint32_t, 32u, >);
P00_ARR_OP_MINMAX(max, uint64_t, 64u, >);

P00_ARR_OP_MINMAX(max, signed char, hhi, >);
P00_ARR_OP_MINMAX(max, signed short, hi, >);
P00_ARR_OP_MINMAX(max, signed, i, >);
P00_ARR_OP_MINMAX(max, signed long, li, >);
P00_ARR_OP_MINMAX(max, signed long long, lli, >);
P00_ARR_OP_MINMAX(max, intmax_t, ji, >);
P00_ARR_OP_MINMAX(max, ptrdiff_t, ti, >);
P00_ARR_OP_MINMAX(max, int8_t, 8i, >);
P00_ARR_OP_MINMAX(max, int16_t, 16i, >);
P00_ARR_OP_MINMAX(max, int32_t, 32i, >);
P00_ARR_OP_MINMAX(max, int64_t, 64i, >);

P00_ARR_OP_MINMAX(max, float, f, >);
P00_ARR_OP_MINMAX(max, double, d, >);
P00_ARR_OP_MINMAX(max, long double, l, >);

#define p99_min_hhu(...) P99_FSYMB(p99_min_hhu)(P99_LENGTH_ARR_ARG(unsigned char, __VA_ARGS__))
#define p99_min_hu(...) P99_FSYMB(p99_min_hu)(P99_LENGTH_ARR_ARG(unsigned short, __VA_ARGS__))
#define p99_min_u(...) P99_FSYMB(p99_min_u)(P99_LENGTH_ARR_ARG(unsigned, __VA_ARGS__))
#define p99_min_lu(...) P99_FSYMB(p99_min_lu)(P99_LENGTH_ARR_ARG(unsigned long, __VA_ARGS__))
#define p99_min_llu(...) P99_FSYMB(p99_min_llu)(P99_LENGTH_ARR_ARG(unsigned long long, __VA_ARGS__))
#define p99_min_ju(...) P99_FSYMB(p99_min_ju)(P99_LENGTH_ARR_ARG(uintmax_t, __VA_ARGS__))
#define p99_min_zu(...) P99_FSYMB(p99_min_zu)(P99_LENGTH_ARR_ARG(size_t, __VA_ARGS__))
#define p99_min_8u(...) P99_FSYMB(p99_min_8u)(P99_LENGTH_ARR_ARG(uint8_t, __VA_ARGS__))
#define p99_min_16u(...) P99_FSYMB(p99_min_16u)(P99_LENGTH_ARR_ARG(uint16_t, __VA_ARGS__))
#define p99_min_32u(...) P99_FSYMB(p99_min_32u)(P99_LENGTH_ARR_ARG(uint32_t, __VA_ARGS__))
#define p99_min_64u(...) P99_FSYMB(p99_min_64u)(P99_LENGTH_ARR_ARG(uint64_t, __VA_ARGS__))

#define p99_min_hhi(...) P99_FSYMB(p99_min_hhi)(P99_LENGTH_ARR_ARG(signed char, __VA_ARGS__))
#define p99_min_hi(...) P99_FSYMB(p99_min_hi)(P99_LENGTH_ARR_ARG(signed short, __VA_ARGS__))
#define p99_min_i(...) P99_FSYMB(p99_min_i)(P99_LENGTH_ARR_ARG(signed, __VA_ARGS__))
#define p99_min_li(...) P99_FSYMB(p99_min_li)(P99_LENGTH_ARR_ARG(signed long, __VA_ARGS__))
#define p99_min_lli(...) P99_FSYMB(p99_min_lli)(P99_LENGTH_ARR_ARG(signed long long, __VA_ARGS__))
#define p99_min_ji(...) P99_FSYMB(p99_min_ji)(P99_LENGTH_ARR_ARG(intmax_t, __VA_ARGS__))
#define p99_min_ti(...) P99_FSYMB(p99_min_ti)(P99_LENGTH_ARR_ARG(ptrdiff_t, __VA_ARGS__))
#define p99_min_8i(...) P99_FSYMB(p99_min_8i)(P99_LENGTH_ARR_ARG(int8_t, __VA_ARGS__))
#define p99_min_16i(...) P99_FSYMB(p99_min_16i)(P99_LENGTH_ARR_ARG(int16_t, __VA_ARGS__))
#define p99_min_32i(...) P99_FSYMB(p99_min_32i)(P99_LENGTH_ARR_ARG(int32_t, __VA_ARGS__))
#define p99_min_64i(...) P99_FSYMB(p99_min_64i)(P99_LENGTH_ARR_ARG(int64_t, __VA_ARGS__))
#define p99_min_ji(...) P99_FSYMB(p99_min_ji)(P99_LENGTH_ARR_ARG(intmax_t, __VA_ARGS__))

#define p99_min_f(...) P99_FSYMB(p99_min_f)(P99_LENGTH_ARR_ARG(float, __VA_ARGS__))
#define p99_min_d(...) P99_FSYMB(p99_min_d)(P99_LENGTH_ARR_ARG(double, __VA_ARGS__))
#define p99_min_l(...) P99_FSYMB(p99_min_l)(P99_LENGTH_ARR_ARG(long double, __VA_ARGS__))

#define p99_max_hhu(...) P99_FSYMB(p99_max_hhu)(P99_LENGTH_ARR_ARG(unsigned char, __VA_ARGS__))
#define p99_max_hu(...) P99_FSYMB(p99_max_hu)(P99_LENGTH_ARR_ARG(unsigned short, __VA_ARGS__))
#define p99_max_u(...) P99_FSYMB(p99_max_u)(P99_LENGTH_ARR_ARG(unsigned, __VA_ARGS__))
#define p99_max_lu(...) P99_FSYMB(p99_max_lu)(P99_LENGTH_ARR_ARG(unsigned long, __VA_ARGS__))
#define p99_max_llu(...) P99_FSYMB(p99_max_llu)(P99_LENGTH_ARR_ARG(unsigned long long, __VA_ARGS__))
#define p99_max_ju(...) P99_FSYMB(p99_max_ju)(P99_LENGTH_ARR_ARG(uintmax_t, __VA_ARGS__))
#define p99_max_zu(...) P99_FSYMB(p99_max_zu)(P99_LENGTH_ARR_ARG(size_t, __VA_ARGS__))
#define p99_max_8u(...) P99_FSYMB(p99_max_8u)(P99_LENGTH_ARR_ARG(uint8_t, __VA_ARGS__))
#define p99_max_16u(...) P99_FSYMB(p99_max_16u)(P99_LENGTH_ARR_ARG(uint16_t, __VA_ARGS__))
#define p99_max_32u(...) P99_FSYMB(p99_max_32u)(P99_LENGTH_ARR_ARG(uint32_t, __VA_ARGS__))
#define p99_max_64u(...) P99_FSYMB(p99_max_64u)(P99_LENGTH_ARR_ARG(uint64_t, __VA_ARGS__))

#define p99_max_hhi(...) P99_FSYMB(p99_max_hhi)(P99_LENGTH_ARR_ARG(signed char, __VA_ARGS__))
#define p99_max_hi(...) P99_FSYMB(p99_max_hi)(P99_LENGTH_ARR_ARG(signed short, __VA_ARGS__))
#define p99_max_i(...) P99_FSYMB(p99_max_i)(P99_LENGTH_ARR_ARG(signed, __VA_ARGS__))
#define p99_max_li(...) P99_FSYMB(p99_max_li)(P99_LENGTH_ARR_ARG(signed long, __VA_ARGS__))
#define p99_max_lli(...) P99_FSYMB(p99_max_lli)(P99_LENGTH_ARR_ARG(signed long long, __VA_ARGS__))
#define p99_max_ji(...) P99_FSYMB(p99_max_ji)(P99_LENGTH_ARR_ARG(intmax_t, __VA_ARGS__))
#define p99_max_ti(...) P99_FSYMB(p99_max_ti)(P99_LENGTH_ARR_ARG(ptrdiff_t, __VA_ARGS__))
#define p99_max_8i(...) P99_FSYMB(p99_max_8i)(P99_LENGTH_ARR_ARG(int8_t, __VA_ARGS__))
#define p99_max_16i(...) P99_FSYMB(p99_max_16i)(P99_LENGTH_ARR_ARG(int16_t, __VA_ARGS__))
#define p99_max_32i(...) P99_FSYMB(p99_max_32i)(P99_LENGTH_ARR_ARG(int32_t, __VA_ARGS__))
#define p99_max_64i(...) P99_FSYMB(p99_max_64i)(P99_LENGTH_ARR_ARG(int64_t, __VA_ARGS__))
#define p99_max_ji(...) P99_FSYMB(p99_max_ji)(P99_LENGTH_ARR_ARG(intmax_t, __VA_ARGS__))

#define p99_max_f(...) P99_FSYMB(p99_max_f)(P99_LENGTH_ARR_ARG(float, __VA_ARGS__))
#define p99_max_d(...) P99_FSYMB(p99_max_d)(P99_LENGTH_ARR_ARG(double, __VA_ARGS__))
#define p99_max_l(...) P99_FSYMB(p99_max_l)(P99_LENGTH_ARR_ARG(long double, __VA_ARGS__))

#define P00_ARR_GCD(TYPE, SUFF)                                                     \
TYPE P99_PASTE3(p00_, gcd_, SUFF)(TYPE a, TYPE b) {                                 \
  if (!a) return b;                                                                 \
  if (b)                                                                            \
    for (TYPE mod = b % a; mod; mod = b % a) {                                      \
      b = a;                                                                        \
      a = mod;                                                                      \
    }                                                                               \
  return a;                                                                         \
}                                                                                   \
TYPE P99_FSYMB(P99_PASTE3(p99_, gcd_, SUFF))(size_t number, TYPE const*const arr) { \
  if (number == 1) return arr[0];                                                   \
  TYPE ret = P99_PASTE3(p00_, gcd_, SUFF)(arr[0], arr[1]);                          \
  for (size_t i = 2; i < number; ++i)                                               \
    ret = P99_PASTE3(p00_, gcd_, SUFF)(ret, arr[i]);                                \
  return ret;                                                                       \
}                                                                                   \
TYPE P99_FSYMB(P99_PASTE3(p99_, lcm_, SUFF))(size_t number, TYPE const*const arr) { \
  if (number == 1) return arr[0];                                                   \
  TYPE gcd = P99_FSYMB(P99_PASTE3(p99_, gcd_, SUFF))(number, arr);                  \
  TYPE ret = arr[0];                                                                \
  for (size_t i = 1; i < number; ++i)                                               \
    ret *= (arr[i] / gcd);                                                          \
  return ret;                                                                       \
}                                                                                   \
P99_MACRO_END(_arr_gcd_, SUFF)


#define p99_gcd_hhu(...) P99_FSYMB(p99_gcd_hhu)(P99_LENGTH_ARR_ARG(unsigned char, __VA_ARGS__))
#define p99_gcd_hu(...) P99_FSYMB(p99_gcd_hu)(P99_LENGTH_ARR_ARG(unsigned short, __VA_ARGS__))
#define p99_gcd_u(...) P99_FSYMB(p99_gcd_u)(P99_LENGTH_ARR_ARG(unsigned, __VA_ARGS__))
#define p99_gcd_lu(...) P99_FSYMB(p99_gcd_lu)(P99_LENGTH_ARR_ARG(unsigned long, __VA_ARGS__))
#define p99_gcd_llu(...) P99_FSYMB(p99_gcd_llu)(P99_LENGTH_ARR_ARG(unsigned long long, __VA_ARGS__))
#define p99_gcd_ju(...) P99_FSYMB(p99_gcd_ju)(P99_LENGTH_ARR_ARG(uintmax_t, __VA_ARGS__))
#define p99_gcd_zu(...) P99_FSYMB(p99_gcd_zu)(P99_LENGTH_ARR_ARG(size_t, __VA_ARGS__))
#define p99_gcd_8u(...) P99_FSYMB(p99_gcd_8u)(P99_LENGTH_ARR_ARG(uint8_t, __VA_ARGS__))
#define p99_gcd_16u(...) P99_FSYMB(p99_gcd_16u)(P99_LENGTH_ARR_ARG(uint16_t, __VA_ARGS__))
#define p99_gcd_32u(...) P99_FSYMB(p99_gcd_32u)(P99_LENGTH_ARR_ARG(uint32_t, __VA_ARGS__))
#define p99_gcd_64u(...) P99_FSYMB(p99_gcd_64u)(P99_LENGTH_ARR_ARG(uint64_t, __VA_ARGS__))

#define p99_lcm_hhu(...) P99_FSYMB(p99_lcm_hhu)(P99_LENGTH_ARR_ARG(unsigned char, __VA_ARGS__))
#define p99_lcm_hu(...) P99_FSYMB(p99_lcm_hu)(P99_LENGTH_ARR_ARG(unsigned short, __VA_ARGS__))
#define p99_lcm_u(...) P99_FSYMB(p99_lcm_u)(P99_LENGTH_ARR_ARG(unsigned, __VA_ARGS__))
#define p99_lcm_lu(...) P99_FSYMB(p99_lcm_lu)(P99_LENGTH_ARR_ARG(unsigned long, __VA_ARGS__))
#define p99_lcm_llu(...) P99_FSYMB(p99_lcm_llu)(P99_LENGTH_ARR_ARG(unsigned long long, __VA_ARGS__))
#define p99_lcm_ju(...) P99_FSYMB(p99_lcm_ju)(P99_LENGTH_ARR_ARG(uintmax_t, __VA_ARGS__))
#define p99_lcm_zu(...) P99_FSYMB(p99_lcm_zu)(P99_LENGTH_ARR_ARG(size_t, __VA_ARGS__))
#define p99_lcm_8u(...) P99_FSYMB(p99_lcm_8u)(P99_LENGTH_ARR_ARG(uint8_t, __VA_ARGS__))
#define p99_lcm_16u(...) P99_FSYMB(p99_lcm_16u)(P99_LENGTH_ARR_ARG(uint16_t, __VA_ARGS__))
#define p99_lcm_32u(...) P99_FSYMB(p99_lcm_32u)(P99_LENGTH_ARR_ARG(uint32_t, __VA_ARGS__))
#define p99_lcm_64u(...) P99_FSYMB(p99_lcm_64u)(P99_LENGTH_ARR_ARG(uint64_t, __VA_ARGS__))


P00_ARR_GCD(unsigned char, hhu);
P00_ARR_GCD(unsigned short, hu);
P00_ARR_GCD(unsigned, u);
P00_ARR_GCD(unsigned long, lu);
P00_ARR_GCD(unsigned long long, llu);
P00_ARR_GCD(uintmax_t, ju);
P00_ARR_GCD(size_t, zu);
P00_ARR_GCD(uint8_t, 8u);
P00_ARR_GCD(uint16_t, 16u);
P00_ARR_GCD(uint32_t, 32u);
P00_ARR_GCD(uint64_t, 64u);



#define P00_ARR_OP_MINMAX_FIXED(NAME, TYPE, SUFF)                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed2)(void) {                                     \
  return P99_PASTE4(p99_, NAME, _, SUFF)(5, 7);                                           \
}                                                                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed3)(void) {                                     \
  return P99_PASTE4(p99_, NAME, _, SUFF)(3, 5, 7);                                        \
}                                                                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed4)(void) {                                     \
  return P99_PASTE4(p99_, NAME, _, SUFF)(1, 3, 5, 7);                                     \
}                                                                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed5)(void) {                                     \
  return P99_PASTE4(p99_, NAME, _, SUFF)(3, 1, 3, 5, 7);                                  \
}                                                                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed6)(void) {                                     \
  return P99_PASTE4(p99_, NAME, _, SUFF)(3, 1, 3, 5, 7, 3);                               \
}                                                                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed7)(void) {                                     \
  return P99_PASTE4(p99_, NAME, _, SUFF)(3, 1, 3, 5, 7, 3, 1);                            \
}                                                                                         \
TYPE P99_PASTE5(p99_, NAME, _, SUFF, _fixed15)(void) {                                    \
  return P99_PASTE4(p99_, NAME, _, SUFF)(3, 1, 3, 5, 7, 3, 1, 3, 5, 0, 7, 3, 1, 3, 5, 7); \
}                                                                                         \
P99_MACRO_END(_arr_op_minmax_fixed_, NAME, _, SUFF)



P00_ARR_OP_MINMAX_FIXED(min, uintmax_t, ju);
P00_ARR_OP_MINMAX_FIXED(min, intmax_t, ji);
P00_ARR_OP_MINMAX_FIXED(min, float, f);
P00_ARR_OP_MINMAX_FIXED(min, double, d);
P00_ARR_OP_MINMAX_FIXED(min, long double, l);

P00_ARR_OP_MINMAX_FIXED(max, uintmax_t, ju);
P00_ARR_OP_MINMAX_FIXED(max, intmax_t, ji);
P00_ARR_OP_MINMAX_FIXED(max, float, f);
P00_ARR_OP_MINMAX_FIXED(max, double, d);
P00_ARR_OP_MINMAX_FIXED(max, long double, l);

P00_ARR_OP_MINMAX_FIXED(gcd, unsigned, u);
P00_ARR_OP_MINMAX_FIXED(lcm, unsigned, u);

#define SAYIT(...) printf("gcd is %ju\n", p99_gcd_ju(__VA_ARGS__))

void va_arg_func(int a, double b, ...) {
  /* empty */
}

#define va_arg_func(...) P99_CALL_VA_ARG(va_arg_func, 2, double, __VA_ARGS__)
#define va_arg_func_defarg_0() 9
#define va_arg_func_defarg_1() 1.E-4
#define va_arg_func_defarg() 7.1

int main(int argc, char** argv) {
  assert(toto(3, 1, 3, 5, 7) == 1u);
  assert(tutu(3, 1, 3, 5, 7) == 1u);
  SAYIT(10u, 12u);
  SAYIT(12u, 10u);
  SAYIT(99u, 110u);
  SAYIT(998u, 1112u);
  SAYIT(2397u, 7191u);
  SAYIT(2397u, 7191u, 2703u);
  int Y[17] = {  P99_DUPL(17, 1) };
  int Z[22] = {  P99_POSS(22) };
  P99_VASSIGNS(Z, int x, double k, int y, bool r);
  P99_VASSIGNS(Y, int s);
  P99_VASSIGNS(Y, int i, float o);
  P99_VASSIGNS(Y, void);
  P99_VASSIGNS(Y,);
  P99_VASSIGNS(Z, P99_ACCESSORS(Y, 17));

  va_arg_func();
  va_arg_func(1);
  va_arg_func(1, 2);
  va_arg_func(1, 2, 3);
  va_arg_func(1, , , 4);
}
