/*
** test-prepro.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Jul 19 10:00:06 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/


#include "p99_c99.h"
#include "p99_int.h"

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
}
