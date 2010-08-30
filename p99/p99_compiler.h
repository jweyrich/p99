#ifndef P99_COMPILER_H
#define P99_COMPILER_H

/**
 ** @file
 ** @brief Group compiler dependencies together in one file
 **
 ** This has been tested with different versions of gcc (versions 4.0
 ** to 4.4) and clang.
 **
 ** The problem makers that are handled the @c inline keyword and some
 ** simple branch prediction feature.
 **/

#define P99__PREFIX0(N) P99__PREFIX0_(N)
#define P99__PREFIX0_(N) 0 ## N

/* This long list of compilers does not mean that we tested P99, nor
   does it even imply that there is a C99 mode for them. We just list
   compilers and detection macros for them for completeness. The
   information for that detection was wildly collected from the web.
   They are listed in alphabetic order, and their numbering is
   nothing that is supposed to stay fixed, reliable or anything. */

#define P99_COMPILER_BORLAND	0x00001U
#define P99_COMPILER_CLANG	0x00002U
#define P99_COMPILER_COMEAU	0x00004U
#define P99_COMPILER_CRAY	0x00008U
#define P99_COMPILER_DEC	0x00010U
#define P99_COMPILER_GNU	0x00020U
#define P99_COMPILER_HP		0x00040U
#define P99_COMPILER_IBM	0x00080U
#define P99_COMPILER_INTEL	0x00100U
#define P99_COMPILER_KAI	0x00200U
#define P99_COMPILER_LCC	0x00400U
#define P99_COMPILER_METROWERKS	0x00800U
#define P99_COMPILER_MICROSOFT	0x01000U
#define P99_COMPILER_PORTLAND	0x02000U
#define P99_COMPILER_SGI	0x04000U
#define P99_COMPILER_SUN	0x08000U
#define P99_COMPILER_WATCOM	0x10000U

/* be sure to put all compilers that are faking gcc before gcc itself */
#if defined(__clang__)
# define P99_COMPILER P99_COMPILER_CLANG
#elif defined(__ICC)
# define P99_COMPILER P99_COMPILER_INTEL
/* compilers that (as far as we know) don't pretend to be gcc */
#elif defined(__BORLANDC__)
# define P99_COMPILER P99_COMPILER_BORLAND
#elif defined( __clang__)
# define P99_COMPILER P99_COMPILER_CLANG
#elif defined(__COMO__)
# define P99_COMPILER P99_COMPILER_COMEAU
#elif defined(_CRAYC)
# define P99_COMPILER P99_COMPILER_CRAY
#elif defined(__DECC_VER)
# define P99_COMPILER P99_COMPILER_DEC
#elif defined(__HP_cc)
# define P99_COMPILER P99_COMPILER_HP
#elif defined(__IBMC__)
# define P99_COMPILER P99_COMPILER_IBM
#elif defined(__KCC)
# define P99_COMPILER P99_COMPILER_KAI
#elif defined(__LCC__)
# define P99_COMPILER P99_COMPILER_LCC
#elif defined(__MWERKS__)
# define P99_COMPILER P99_COMPILER_METROWERKS
#elif defined(_MSC_VER)
# define P99_COMPILER P99_COMPILER_MICROSOFT
#elif defined(__PGI)
# define P99_COMPILER P99_COMPILER_PORTLAND
#elif defined(__sgi)
# define P99_COMPILER P99_COMPILER_SGI
#elif defined(__SUNPRO_C)
# define P99_COMPILER P99_COMPILER_SUN
#elif defined(__WATCOMC__)
# define P99_COMPILER P99_COMPILER_WATCOM
#elif defined(__GNUC__)
# define P99_COMPILER P99_COMPILER_GNU
#else
# define P99_COMPILER 0x00000U
#endif

# ifdef __GNUC__
#  define P99__GCC_VERSION(A, B, C) P99__GCC_VERSION_(A, B, C)
#  define P99__GCC_VERSION_(A, B, C) A ## B ## C ## UL
#  ifdef __GNUC_PATCHLEVEL__
#   if __GNUC_PATCHLEVEL__ < 10
#    define P99__GNUC_PATCHLEVEL__ P99__PREFIX0(__GNUC_PATCHLEVEL__)
#   else
#    define P99__GNUC_PATCHLEVEL__ __GNUC_PATCHLEVEL__
#   endif
#  else
#    define P99__GNUC_PATCHLEVEL__ 00
#  endif
#  if __GNUC_MINOR__ < 10
#   define P99__GNUC_MINOR__ P99__PREFIX0(__GNUC_MINOR__)
#  else
#   define P99__GNUC_MINOR__ __GNUC_MINOR__
#  endif
#  define P99_GCC_VERSION P99__GCC_VERSION(__GNUC__, P99__GNUC_MINOR__, P99__GNUC_PATCHLEVEL__)
# endif


#ifdef __cplusplus
/* C++ as of 1998 is currently not compatible with the macros here. It
   is missing:
   - variate macros
   - compound literals
   - trailing commas in initializers
   - trailing commas in enum declarations
   we test for some of this to see if we have any chance to pass this
   through. Otherwise this should error out early.
 */
# define P99__VA_ARGS__(...) __VA_ARGS__
enum { p99__trailing_comma_in_enum__ = -1, };
inline
signed p99__trailing_comma_in_initializer__(void) {
  signed a[] = { p99__trailing_comma_in_enum__ , };
  return a[0];
}
#else
# if !defined(__STDC_VERSION__) ||  (__STDC_VERSION__ < 199901L)
/* If you come here your compiler is not conforming to C99, since
   this requires the macro __STDC_VERSION__ to be set to the
   indicated value (or larger).

   You still might be able to use P99, but you would have to cheat on
   that value. You are on your own. */
#  error "The P99 preprocessor files need a C99 compliant compiler"
# endif
#endif

#if P99_COMPILER & P99_COMPILER_CLANG
# define inline __attribute__((always_inline)) __inline__
# define force_inline __attribute__((always_inline)) __inline__
#elif P99_COMPILER & P99_COMPILER_GNU
/* gcc prior to version 4.3 has the inline keyword but with slightly
   different semantics.
   Be sure to allways inline functions in this cases.
   */
# if P99_GCC_VERSION < 40300UL
#  ifdef inline
#   undef inline
#  endif
#  ifdef __GNUC_GNU_INLINE__
#   define inline __attribute__((gnu_inline,always_inline)) static __inline__
#  else
#   define inline __attribute__((always_inline)) static __inline__
#  endif
#  define static_inline inline
# endif
# define force_inline __attribute__((always_inline)) __inline__
#endif


# ifndef static_inline
#  define static_inline static inline
# endif
# ifndef force_inline
#  define force_inline static inline
# endif

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_INTEL)
# if P99_GCC_VERSION >= 30000UL
#  define P99_EXPECT(EXP, VAL) __builtin_expect((EXP), (VAL))
# endif
#endif

/**
 ** @def P99_EXPECT
 ** @brief Provide a compiler hint concerning the likelihood of a
 ** certain value in an expression @a EXP.
 **
 ** With this you indicate that in the overwhelming number of cases
 ** the integer expression @a EXP will evaluate to @a VAL.
 **
 ** Currently this is only implemented for gcc.
 **/
#ifndef P99_EXPECT
# define P99_EXPECT(EXP, VAL) (EXP)
#endif

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU)
# if defined(__LONG_MAX__) && defined(__LONG_LONG_MAX__) && (P99_GCC_VERSION >= 30000UL)
#  if (__LONG_MAX__ == 9223372036854775807) && (__LONG_LONG_MAX__ == 9223372036854775807)
#   define p99x_uintmax p99x_uintmax
#   define p99x_intmax p99x_intmax
#   define p99x_uint128 p99x_uint128
#   define p99x_int128 p99x_int128
typedef __uint128_t p99x_uintmax;
typedef __int128_t p99x_intmax;
typedef __uint128_t p99x_uint128;
typedef __int128_t p99x_int128;
#  endif
# endif
#endif


#endif
