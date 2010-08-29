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
  = "sign and magn",
  [p99_signed_representation_ones]
  = "one's compl",
  [p99_signed_representation_twos]
  = "two's compl",
};

#define SAYIT(T)                                                \
printf("%20s:\t%4u\t%5u\t%3zu\t%20jd\t%20ju,\t%3ssigned%15s\n", \
       #T,                                                      \
       P99_TPREC(T),                                            \
       P99_TWIDTH(T),                                           \
       P99_TPADDING(T),                                         \
       (intmax_t)P99_TMIN(T),                                   \
       (uintmax_t)P99_TMAX(T),                                  \
       (P99_ISSIGNED(T) ? "" : "un"),                           \
       (!P99_ISSIGNED(T)                                        \
        ? ""                                                    \
        : representation[P99_SIGNED_REPRESENTATION(T)]))

#define SAYIT3(EXPR)                                            \
printf("%20s:\t%4u\t%5u\t%3zu\t%20jd\t%20ju,\t%3ssigned%15s\n", \
       #EXPR,                                                   \
       P99_EPREC(EXPR),                                         \
       P99_EWIDTH(EXPR),                                        \
       P99_EPADDING(EXPR),                                      \
       (intmax_t)P99_EMIN(EXPR),                                \
       (uintmax_t)P99_EMAX(EXPR),                               \
       (P99_SIGNED(EXPR) ? "" : "un"),                          \
       (!P99_SIGNED(EXPR)                                       \
        ? ""                                                    \
        : representation[P99_E_REPRESENTATION(EXPR)]))

typedef enum { a1, b1 } enum1;
typedef enum { a2 = -1, b2 } enum2;
typedef enum { a3, b3, c3 } enum3;
typedef enum { a4 = -1, b4, c4 } enum4;
/* The following should fail, since enum constants are supposed to be
   int. Currently clang accepts this and gcc doesn't. */
//typedef enum { a5 = (unsigned)-1, b5, c5 } enum5;

int main(int argc, char** argv) {
  printf("%20s:\t%2s\t%2s\t%2s\t%20s\t%20s,\t%3ssigned%15s\n",
         "type", "prec", "width", "pad", "min", "max", "{un}", "sign repr");
  printf("--------------------------- proper types ---------------------------------\n");
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

  printf("--------------------------- typedefs for specific width -------------------\n");
#if defined(UINT8_MAX)
  /* if this exists this must not have padding and thus CHAR_BIT == 8 */
  SAYIT(uint8_t);
#endif
  /* the following type is required */
  /* CHAR_BIT != 8 and we should see padding on this type */
  SAYIT(uint_least8_t);
  SAYIT(uint_fast8_t);
#if defined(INT8_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int8_t);
#endif
  /* the following type is required */
  /* either this has padding or this not in two's representation */
  SAYIT(int_least8_t);
  SAYIT(int_fast8_t);

#if defined(UINT12_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint12_t);
#elif defined(UINT_LEAST12_MAX)
  SAYIT(uint_least12_t);
#endif
#if defined(INT12_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int12_t);
#elif defined(INT_LEAST12_MAX)
  SAYIT(int_least12_t);
#endif

#if defined(UINT16_MAX)
  /* if this exists this must not have padding */
  SAYIT(uint16_t);
#endif
  /* the following type is required */
  SAYIT(uint_least16_t);
  SAYIT(uint_fast16_t);
#if defined(INT16_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int16_t);
#endif
  /* the following type is required */
  SAYIT(int_least16_t);
  SAYIT(int_fast16_t);

#if defined(UINT24_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint24_t);
#elif defined(UINT_LEAST24_MAX)
  SAYIT(uint_least24_t);
#endif
#if defined(INT24_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int24_t);
#elif defined(INT_LEAST24_MAX)
  SAYIT(int_least24_t);
#endif

#if defined(UINT32_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint32_t);
#endif
  /* the following type is required */
  SAYIT(uint_least32_t);
  SAYIT(uint_fast32_t);
#if defined(INT32_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int32_t);
#endif
  /* the following type is required */
  SAYIT(int_least32_t);
  SAYIT(int_fast32_t);

#if defined(UINT48_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint48_t);
#elif defined(UINT_LEAST48_MAX)
  SAYIT(uint_least48_t);
#endif
#if defined(INT48_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int48_t);
#elif defined(INT_LEAST48_MAX)
  SAYIT(int_least48_t);
#endif

#if defined(UINT64_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint64_t);
#endif
  /* the following type is required */
  SAYIT(uint_least64_t);
  SAYIT(uint_fast64_t);
#if defined(INT64_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int64_t);
#endif
  /* the following type is required */
  SAYIT(int_least64_t);
  SAYIT(int_fast64_t);

#if defined(UINT96_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint96_t);
#elif defined(UINT_LEAST96_MAX)
  SAYIT(uint_least96_t);
#endif
#if defined(INT96_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int96_t);
#elif defined(INT_LEAST96_MAX)
  SAYIT(int_least96_t);
#endif

#if defined(UINT128_MAX)
  /* if this exists this must be two's representation */
  SAYIT(uint128_t);
#elif defined(UINT_LEAST128_MAX)
  SAYIT(uint_least128_t);
#endif
#if defined(INT128_MAX)
  /* if this exists this must be two's representation */
  SAYIT(int128_t);
#elif defined(INT_LEAST128_MAX)
  SAYIT(int_least128_t);
#endif

  printf("--------------------------- required typedefs -----------------------------\n");
  SAYIT(bool);
  SAYIT(size_t);
  SAYIT(ptrdiff_t);
  SAYIT(uintmax_t);
  SAYIT(intmax_t);
  SAYIT(wchar_t);

  printf("--------------------------- optional typedefs -------------------------------\n");
#if defined(SIG_ATOMIC_MAX)
  SAYIT(sig_atomic_t);
#endif
#if defined(UINTPTR_MAX)
  SAYIT(uintptr_t);
#endif
#if defined(INTPTR_MAX)
  SAYIT(intptr_t);
#endif
  printf("--------------------------- enumeration types -------------------------------\n");
  SAYIT(enum1);
  SAYIT(enum2);
  SAYIT(enum3);
  SAYIT(enum4);
  printf("----------------------------- enum integral constants --------------------\n");
  SAYIT3(a1);
  SAYIT3(a2);
  SAYIT3(a3);
  SAYIT3(a4);
  printf("------------------------------ enum type expressions ----------------------\n");
  SAYIT3((enum1)a1);
  SAYIT3((enum2)a2);
  SAYIT3((enum3)a3);
  SAYIT3((enum4)a4);
  printf("----------------------------- side effect check ---------------------------\n");
  /* using an expression with side effects should not execute these
     side effects. Check this with an assertion. */
  int i = 0;
  SAYIT3(i++);
  assert(!i);
  printf("----------------------------- integer constants -------------------------\n");
  /* Different types of integer constants may have different
     width. The minimum that we will be able to see is `int`. */
  SAYIT3(1);
  SAYIT3(1u);
  SAYIT3(1l);
  SAYIT3(1ul);
  SAYIT3(1ll);
  SAYIT3(1ull);
  SAYIT3(0x7FFF);
  SAYIT3(0x8000);
  SAYIT3(0x7FFFFFFF);
  SAYIT3(0x80000000);
  SAYIT3(0x7FFFFFFFFFFFFFFF);
  SAYIT3(0x8000000000000000);
  printf("------------------------- extension types (macros might not work, yet) -----\n");
#ifdef p99x_uintmax
  SAYIT(p99x_uintmax);
#endif
#ifdef p99x_intmax
  SAYIT(p99x_intmax);
#endif
#ifdef p99x_uint128
  SAYIT(p99x_uint128);
#endif
#ifdef p99x_int128
  SAYIT(p99x_int128);
#endif
  printf("------------------------ extension expressions (macros might not work, yet) -----\n");
#ifdef p99x_uintmax
  SAYIT3((p99x_uintmax)0);
#endif
#ifdef p99x_intmax
  SAYIT3((p99x_intmax)0);
#endif
#ifdef p99x_uint128
  SAYIT3((p99x_uint128)0);
#endif
#ifdef p99x_int128
  SAYIT3((p99x_int128)0);
#endif
}
