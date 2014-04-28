/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2010-2013 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_getopt.h"
#include "p99_c99.h"
#include "p99_int.h"
#include "p99_defarg.h"
#include "p99_enum.h"


P99_GETOPT_SYNOPSIS("a P99 test program for integer type features and commandline options");

P99_GETOPT_DECLARE(a, unsigned, unsignedVar, 47, "unsigned", "this an unsigned variable");
P99_GETOPT_DECLARE(b, bool, boolVar, false, "flag", "you can toggle this on (and off)");
P99_GETOPT_DECLARE(I, char, charVar, 47, "unknown");
P99_GETOPT_DECLARE(d, double, doubleVar, 0, "real", "a double precision element");
P99_GETOPT_DECLARE(AT, char const*, char_cptrVar, 0, 0, "just a string");
#if P99_TEST_GETOPT_CASES & 1
P99_GETOPT_DECLARE(h, bool, h_cptrVar, 0, 0, "don't do that, if you can avoid");
P99_GETOPT_DECLARE(HELP, bool, help_cptrVar, 0, 0, "don't do that, really, this is reserved to P99 help");
#endif
#if P99_TEST_GETOPT_CASES & 2
P99_GETOPT_DECLARE(i, bool, i_cptrVar, 0, "help", "never do that");
#endif
P99_GETOPT_DEFINE(a, unsigned, unsignedVar, 47, "unsigned", "this an unsigned variable");
P99_GETOPT_DEFINE(b, bool, boolVar, false, "flag", "you can toggle this on (and off)");
P99_GETOPT_DEFINE(I, char, charVar, 47, "unknown");
P99_GETOPT_DEFINE(d, double, doubleVar, 0, "real", "a double precision element");
P99_GETOPT_DEFINE(AT, char const*, char_cptrVar, 0, 0, "just a string");
#if P99_TEST_GETOPT_CASES & 1
P99_GETOPT_DEFINE(h, bool, h_cptrVar, 0, 0, "don't do that, if you can avoid");
P99_GETOPT_DEFINE(HELP, bool, help_cptrVar, 0, 0, "don't do that, really, this is reserved to P99 help");
#endif
#if P99_TEST_GETOPT_CASES & 2
P99_GETOPT_DEFINE(i, bool, i_cptrVar, 0, "help", "never do that");
#endif


#define DEF(SUFF)                                                     \
  P99_INSTANTIATE(P99_BUILTIN_TYPE(SUFF), P99_PASTE2(p99_twos, SUFF), \
                  P99_BUILTIN_TYPE(u, SUFF));                         \
  P99_INSTANTIATE(P99_BUILTIN_TYPE(SUFF), P99_PASTE2(p99_add, SUFF),  \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  int[P99_ATLEAST 1]);                                \
  P99_INSTANTIATE(P99_BUILTIN_TYPE(SUFF), P99_PASTE2(p00_add1, SUFF), \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  int[P99_ATLEAST 1]);                                \
  P99_INSTANTIATE(P99_BUILTIN_TYPE(SUFF), P99_PASTE2(p00_add2, SUFF), \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  int[P99_ATLEAST 1]);                                \
  P99_INSTANTIATE(P99_BUILTIN_TYPE(SUFF), P99_PASTE2(p99_sub, SUFF),  \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  P99_BUILTIN_TYPE(SUFF),                             \
                  int[P99_ATLEAST 1])

DEF(c);
DEF(hh);
DEF(h);
DEF();
DEF(l);
DEF(ll);

int compar(void const* a, void const* b, void* context) {
  unsigned const * A = a;
  unsigned const * B = b;
  {
    unsigned a = *A;
    unsigned b = *B;
    return
      ((a == b)
       ? 0
       : ((a < b)
          ? -1
          : 1));
  }
}


/* check how an addition looks like if overflow trapping is enabled */
int just_add(int a, int b) {
  return a + b;
}


uintmax_t p99_low2shift(uintmax_t x);

P99_PROTOTYPE(char*, print_uintmax, p99x_uintmax, char*);
P99_PROTOTYPE(const char*, print_intmax, p99x_intmax, char*);
P99_PROTOTYPE(const char*, print_uintmax_X, p99x_uintmax, char*);

#define UINT64_D19 UINT64_C(10000000000000000000)

#ifdef p99x_uintmax
char* print_uintmax_rec(p99x_uintmax x, char* tmp) {
  return tmp +
         ((x > UINT64_D19)
          ? sprintf(print_uintmax_rec(x / UINT64_D19, tmp), "%019" PRIu64, (uint64_t)(x % UINT64_D19))
          : sprintf(tmp, "%" PRIu64, (uint64_t)x));
}
char* print_uintmax(p99x_uintmax x, char* tmp) {
  print_uintmax_rec(x, tmp);
  return tmp;
}
char* print_uintmax_X_rec(p99x_uintmax x, char* tmp) {
  return tmp +
         ((x > UINT64_MAX)
          ? sprintf(print_uintmax_X_rec(((x >> 32) >> 32), tmp), "%016" PRIX64, (uint64_t)x)
          : sprintf(tmp, "%" PRIX64, (uint64_t)x));
}
char const* print_uintmax_X(p99x_uintmax x, char* tmp) {
  tmp[0] = '0'; tmp[1] = 'X';
  print_uintmax_X_rec(x, tmp + 2);
  return tmp;
}
char const* print_intmax(p99x_intmax x, char* tmp) {
  if (x < 0) {
    tmp[0] = '-';
    print_uintmax(-x, tmp + 1);
  } else {
    print_uintmax(x, tmp);
  }
  return tmp;
}
#else
char* print_uintmax(p99x_uintmax x, char* tmp) {
  sprintf(tmp, "%" PRIu64, x);
  return tmp;
}
const char* print_intmax(p99x_intmax x, char* tmp) {
  sprintf(tmp, "%" PRId64, x);
  return tmp;
}
#endif

#define print_uintmax(...) P99_CALL_DEFARG(print_uintmax, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(print_uintmax, ,);
#define print_uintmax_defarg_1() ((char[40]){ 0 })

#define print_uintmax_X(...) P99_CALL_DEFARG(print_uintmax_X, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(print_uintmax_X, ,);
#define print_uintmax_X_defarg_1() ((char[40]){ 0 })

#define print_intmax(...) P99_CALL_DEFARG(print_intmax, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(print_intmax, , );
#define print_intmax_defarg_1() ((char[40]){ 0 })

#define PRINT_LARGE(X)                                         \
(P99_SIGNED(X) ? print_intmax((X)) : print_uintmax((X)))

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

#ifndef alignof
# define alignof(X) 0
#endif

#define SAYIT(T)                                               \
printf("%20s:\t%4u\t%5u\t%5zu\t%5zu\t%20s\t%20s,\t%3ssigned%15s\n",  \
       #T,                                                     \
       P99_TPREC(T),                                           \
       P99_TWIDTH(T),                                          \
       P99_TPADDING(T),                                        \
       alignof(T),                                             \
       PRINT_LARGE(P99_TMIN(T)),                               \
       PRINT_LARGE(P99_TMAX(T)),                               \
       (P99_ISSIGNED(T) ? "" : "un"),                          \
       (!P99_ISSIGNED(T)                                       \
        ? ""                                                   \
        : representation[P99_SIGNED_REPRESENTATION(T)]))

#define SAYIT3(EXPR)                                           \
printf("%20s:\t%4u\t%5u\t%5zu%5zu\t%20s\t%20s,\t%3ssigned%15s\n",  \
       #EXPR,                                                  \
       P99_EPREC(EXPR),                                        \
       P99_EWIDTH(EXPR),                                       \
       P99_EPADDING(EXPR),                                     \
       alignof(EXPR),                                          \
       PRINT_LARGE(P99_EMIN(EXPR)),                            \
       PRINT_LARGE(P99_EMAX(EXPR)),                            \
       (P99_SIGNED(EXPR) ? "" : "un"),                         \
       (!P99_SIGNED(EXPR)                                      \
        ? ""                                                   \
        : representation[P99_E_REPRESENTATION(EXPR)]))

static unsigned strangeness = 0;

#define IFSTRANGENESS(MIN, MAX, UMAX, T)                                                                                 \
do {                                                                                                                     \
  if (MAX == UMAX) {                                                                                                     \
    printf("P99 strangeness:\tprecisions of signed and unsigned %s are equal\n", #T);                                    \
    ++strangeness;                                                                                                       \
    if (MIN < -MAX) {                                                                                                    \
      printf("P99 strangeness:\tnegative of minimum value for signed %s is not representable in unsigned %s\n", #T, #T); \
      ++strangeness;                                                                                                     \
    }                                                                                                                    \
  }                                                                                                                      \
 } while(0)

typedef enum { a1, b1 } enum1;
typedef enum { a2 = -1, b2 } enum2;
typedef enum { a3, b3, c3 } enum3;
typedef enum { a4 = -1, b4, c4 } enum4;
/* The following should fail, since enum constants are supposed to be
   int. Currently clang accepts this and gcc doesn't. */
//typedef enum { a5 = (unsigned)-1, b5, c5 } enum5;

int main(int argc, char** argv) {
  p99_getopt_initialize(&argc, &argv);
  char const versionDate[] = { P99_VERSION_DATE };
  char const versionID[] = { P99_VERSION_ID };
  char const version_Date[] = { P00_VERSION_DATE };
  char const version_ID[] = { P00_VERSION_ID };
  bool const unreleased = versionDate[0] == '$';
  printf("P99 date:\t%40s %s\n",
         unreleased ? version_Date : versionDate,
         unreleased ?  "(unreleased)" : "");
  printf("P99 identifier:\t%40s %s\n",
         unreleased ? version_ID : versionID,
         unreleased ?  "(unreleased)" : "");
  printf("P99 compiler:\t%40s\n", P99_COMPILER_VERSION);
#ifdef __DATE__
  printf("P99 compiler: compile date was %s\n", __DATE__);
#else
  printf("P99 warning: compile date is not available\n");
#endif
#ifdef __FILE__
  printf("P99 compiler: %s\n", __FILE__);
#else
  printf("P99 warning: source file name not available\n");
#endif
#ifdef __STDC__
  printf("P99 compiler: %s\n",
         __STDC__
         ? "standard C compiler"
         : "doesn't pretend to follow standard C");
#else
  printf("P99 compiler: doesn't know of standard C\n");
#endif
#ifdef __STDC_HOSTED__
  printf("P99 compiler: %s\n",
         __STDC_HOSTED__
         ? "hosted environment"
         : "free standing environment");
#else
  printf("P99 compiler: unknown environment type\n");
#endif
#ifdef __STDC_VERSION__
  printf("P99 compiler: C standard version %s\n", P99_STRINGIFY(__STDC_VERSION__));
#else
  printf("P99 compiler: C standard version number is not available\n");
#endif
#ifdef __STDC_IEC_559__
  printf("P99 compiler: conforming to IEC 60559, floating-point arithmetic\n");
# ifdef __STDC_IEC_559_COMPLEX__
  printf("P99 compiler: conforming to IEC 60559, compatible complex arithmetic\n");
# else
  printf("P99 compiler: not conforming to IEC 60559, compatible complex arithmetic\n");
# endif
#else
  printf("P99 compiler: not conforming to IEC 60559, floating-point arithmetic\n");
#endif
#ifdef __STDC_ISO_10646__
  static bool unicode = true;
  printf("P99 compiler: ISO/IEC 10646 Unicode standard version as of %ld, %ld\n",
         __STDC_ISO_10646__ % 100L,
         __STDC_ISO_10646__ / 100L);
#else
  static bool unicode = false;
  printf("P99 compiler: ISO/IEC 10646 Unicode standard version is not available\n");
#endif
  enum { UINT_WIDTH = P99_TWIDTH(unsigned) };
  /* signed and unsigned int have the same size, but they may have
     different width. */
  typedef union bitfield {
signed s:UINT_WIDTH;
unsigned u:UINT_WIDTH;
int i:UINT_WIDTH;
    signed S;
    unsigned U;
    unsigned char C[sizeof(int)];
  } bitfield;
  /* use minus 256 since this can never lead to a trap representation */
  bitfield m_256 = { .C = { 0 }};
  m_256.U = (0u - 256u);
  P99_IF_COMPILER(INTEL, warning(disable: 186)) /* pointless comparison of unsigned integer with zero */
  printf("P99 diagnostic:\tplatform endianess is %s\n", p99_endianness_getname(P99_ENDIANNESS)+4);
  printf("P99 diagnostic:\t\t\t int in bitfield is %s\n", (m_256.i <= 0 ? "signed" : "unsigned"));
  printf("P99 diagnostic:\t\t  signed int in bitfield is %s\n", (m_256.s <= 0 ? "signed" : "unsigned"));
  printf("P99 diagnostic:\t\tunsigned int in bitfield is %s\n", (m_256.u <= 0 ? "signed" : "unsigned"));
  /* Check to see which bit is the sign bit. */
  size_t signbit = 0;
  bool signincluded = false;
  /* To do that we have to be careful not to generate a trap
     representation. Therefore we generate bit patterns that always
     have at least one bit on and off in each byte. */
  for (unsigned i = 0; i < CHAR_BIT; ++i) {
    unsigned char pattern = UCHAR_MAX;
    unsigned char mask = (1u << i);
    pattern ^= mask;
    memset(&m_256.C, pattern, sizeof(int));
    unsigned Uback = m_256.U;
    if (m_256.S > 0) {
      /* The ith bit in one of the bytes is the sign bit. Search for
         the byte */
      for (size_t j = 0; j < sizeof(int); ++j) {
        m_256.C[j] = UCHAR_MAX;
        if (m_256.S < 0) {
          /* this is it */
          signbit = j * CHAR_BIT + i;
          signincluded = (Uback < m_256.U);
          /* Reset the bit pattern to have just that sign bit on */
          memset(&m_256.C, 0, sizeof(int));
          m_256.C[j] = mask;
          break;
        }
        m_256.C[j] = pattern;
      }
    }
  }
  printf("P99 diagnostic:\tsign bit of signed int is in position %zu\n", signbit);
  printf("P99 diagnostic:\tsign bit of signed int %s value bit for unsigned int\n",
         signincluded ? "is a" : "is no");
  if (signincluded) {
    //m_256.S = -256;
    unsigned u_256 = m_256.U;
    u_256 -= 1u;
    u_256 <<= 1u;
    u_256 |= 1u;
    if (u_256 < UINT_MAX) {
      ++strangeness;
      printf("P99 strangeness:\twidth of signed int < width of unsigned int\n");
      printf("P99 strangeness:\tbit pattern of all ones signed int seen as an unsigned: 0x%X\n",
             u_256);
    } else {
      setlocale(LC_ALL, "");
      printf("P99 diagnostic:\twidth of signed int %lc width of unsigned int\n",
             unicode ? L'\x2243' : L'=');
    }
  } else {
    printf("P99 strangeness:\twidth of signed int > width of unsigned int\n");
    ++strangeness;
  }
  /* uncomment the following two lines to test the strangeness detection */
  // IFSTRANGENESS(-1, 1, 1, bogus);
  // IFSTRANGENESS(-2, 1, 1, embararssing);
  IFSTRANGENESS(SCHAR_MIN, SCHAR_MAX, UCHAR_MAX, char);
  IFSTRANGENESS(SHRT_MIN, SHRT_MAX, USHRT_MAX, short);
  IFSTRANGENESS(INT_MIN, INT_MAX, UINT_MAX, int);
  IFSTRANGENESS(LONG_MIN, LONG_MAX, ULONG_MAX, long);
  IFSTRANGENESS(LLONG_MIN, LLONG_MAX, ULLONG_MAX, long long);
  if (strangeness) {
    printf("P99 warning: Because of the strangeness %u of this platform, some of the following computations may be incorrect\n",
           strangeness);
  }
  printf("%20s:\t%2s\t%2s\t%2s\t%2s\t%20s\t%20s,\t%3ssigned%15s\n",
         "type", "prec", "width", "pad", "align", "min", "max", "{un}", "sign repr");
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
  SAYIT(wint_t);

#if defined(SIG_ATOMIC_MAX)
  SAYIT(sig_atomic_t);
#else
  printf("**** sig_atomic_t seems to be missing ****\n");
#endif

  printf("--------------------------- optional typedefs -------------------------------\n");
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
  printf("------------------------ endianness behavior -----\n");
  printf ("Endianess results in %#04"PRIX16", %#08"PRIX32", %#016"PRIX64", %s\n",
          P99_HTON(2, 0x0001),
          P99_HTON(4, 0x00010203),
          P99_HTON(8, 0x0001020304050607),
#ifdef p99x_uint128
          print_uintmax_X(P99_HTON(16, ((p99x_uint128)0x0001020304050607) << 64 | ((p99x_uint128)0x08090A0B0C0D0E0F)))
#else
          "<128 bit types not supported>"
#endif
         );
  printf ("Endianess results in %#04"PRIX16", %#08"PRIX32", %#016"PRIX64", %s\n",
          P99_NTOH(2, 0x0001),
          P99_NTOH(4, 0x00010203),
          P99_NTOH(8, 0x0001020304050607),
#ifdef p99x_uint128
          print_uintmax_X(P99_NTOH(16, ((p99x_uint128)0x0001020304050607) << 64 | ((p99x_uint128)0x08090A0B0C0D0E0F)))
#else
          "<128 bit types not supported>"
#endif
         );
  printf("------------------------ testing for overflow behavior -----\n");
  int oth = argc - 1;
  {
    int val = INT_MAX;
    int err[1] = { 0 };
    int sum = p99_add(val, oth, err);
    printf ("adding %d to INT_MAX gives %d: %s\n", oth, sum, P99_STRERROR(err[0]));
#ifndef P99_NOTRAP
    printf ("adding %d to INT_MAX gives %d\n", oth, just_add(val, oth));
#endif
    val = INT_MIN;
    err[0] = 0;
    sum = p99_add(val, -oth, err);
    printf ("adding %d to INT_MIN gives %d: %s\n", -oth, sum, P99_STRERROR(err[0]));
#ifndef P99_NOTRAP
    printf ("adding %d to INT_MIN gives %d\n", -oth, just_add(val, -oth));
#endif
    err[0] = 0;
    sum = p99_sub(val, oth, err);
    printf ("subtrackting %d from INT_MIN gives %d: %s\n", oth, sum, P99_STRERROR(err[0]));
  }
  {
    signed char val = SCHAR_MAX;
    int err[1] = { 0 };
    signed char sum = p99_addhh(val, oth, err);
    printf ("adding %d to SCHAR_MAX gives %d: %s\n", oth, sum, P99_STRERROR(err[0]));
    val = SCHAR_MIN;
    err[0] = 0;
    sum = p99_addhh(val, -oth, err);
    printf ("adding %d to SCHAR_MIN gives %d: %s\n", -oth, sum, P99_STRERROR(err[0]));
    err[0] = 0;
    sum = p99_subhh(val, oth, err);
    printf ("subtrackting %d from SCHAR_MIN gives %d: %s\n", oth, sum, P99_STRERROR(err[0]));
  }
  {
    unsigned A[6] = { 1, 3, 4, 5, 8, 10 };
    unsigned el = 5;
    unsigned * res = bsearch_s(&el, A, 6, sizeof *A, compar, 0);
    printf("searching for %u, gave %u\n", el, res ? *(unsigned*)res : UINT_MAX);
    el = 11;
    res = bsearch_s(&el, A, 6, sizeof *A, compar, 0);
    printf("searching for %u, gave %u\n", el, res ? *(unsigned*)res : UINT_MAX);
    el = 1;
    res = bsearch_s(&el, A, 6, sizeof *A, compar, 0);
    printf("searching for %u, gave %u\n", el, res ? *(unsigned*)res : UINT_MAX);
    el = 7;
    res = bsearch_s(&el, A, 6, sizeof *A, compar, 0);
    printf("searching for %u, gave %u\n", el, res ? *(unsigned*)res : UINT_MAX);
    el = 0;
    res = bsearch_s(&el, A, 6, sizeof *A, compar, 0);
    printf("searching for %u, gave %u\n", el, res ? *(unsigned*)res : UINT_MAX);
  }
  P99_PRINTF("a = %s, b = %s, I = %s, d = %s, @ = \"%s\"\n", unsignedVar, boolVar, charVar, doubleVar, char_cptrVar);
  return EXIT_SUCCESS;
}
