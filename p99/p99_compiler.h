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
#ifndef P99_COMPILER_H
#define P99_COMPILER_H

/**
 ** @file
 ** @brief Group compiler dependencies together in one file
 **
 ** This has been tested with different versions of gcc (GNU, versions 4.0
 ** to 4.4), clang, opencc (OPEN64) and icc (INTEL).
 **
 ** The problem makers that are handled the @c inline keyword and some
 ** simple branch prediction feature.
 **/

#define P00_PREFIX0(N) P00_PREFIX0_(N)
#define P00_PREFIX0_(N) 0 ## N
#define P00_STRINGIFY_(X) #X
#define P00_STRINGIFY(X) P00_STRINGIFY_(X)

/* This long list of compilers does not mean that we tested P99, nor
   does it even imply that there is a C99 mode for them. We just list
   compilers and detection macros for them for completeness. The
   information for that detection was wildly collected from the web.
   They are listed in alphabetic order, and their numbering is
   nothing that is supposed to stay fixed, reliable or anything. */

#define P99_COMPILER_BORLAND	(1u << 0)
#define P99_COMPILER_CLANG	(1u << 1)
#define P99_COMPILER_COMEAU	(1u << 2)
#define P99_COMPILER_CRAY	(1u << 3)
#define P99_COMPILER_DEC	(1u << 4)
#define P99_COMPILER_GNU	(1u << 5)
#define P99_COMPILER_HP		(1u << 6)
#define P99_COMPILER_IBM	(1u << 7)
#define P99_COMPILER_INTEL	(1u << 8)
#define P99_COMPILER_KAI	(1u << 9)
#define P99_COMPILER_LCC	(1u << 10)
#define P99_COMPILER_METROWERKS	(1u << 11)
#define P99_COMPILER_MICROSOFT	(1u << 12)
#define P99_COMPILER_PORTLAND	(1u << 13)
#define P99_COMPILER_SGI	(1u << 14)
#define P99_COMPILER_SUN	(1u << 15)
#define P99_COMPILER_WATCOM	(1u << 16)
#define P99_COMPILER_OPEN64	(1u << 17)

/* be sure to put all compilers that are faking gcc before gcc itself */
#if defined(__clang__)
# define P99_COMPILER P99_COMPILER_CLANG
# define P99_COMPILER_VERSION                                  \
 "clang " P00_STRINGIFY(__clang__)                            \
 "; gnu "                                                      \
 P00_STRINGIFY(__GNUC__) "."                                  \
 P00_STRINGIFY(__GNUC_MINOR__) "."                            \
 P00_STRINGIFY(__GNUC_PATCHLEVEL__)

#elif defined(__INTEL_COMPILER)
# define P99_COMPILER P99_COMPILER_INTEL
# define P99_COMPILER_VERSION                                  \
 "intel " P00_STRINGIFY(__INTEL_COMPILER)                      \
 "; gnu "                                                      \
 P00_STRINGIFY(__GNUC__) "."                                  \
 P00_STRINGIFY(__GNUC_MINOR__) "."                            \
 P00_STRINGIFY(__GNUC_PATCHLEVEL__)

#elif defined(__OPEN64__)
# define P99_COMPILER P99_COMPILER_OPEN64
# define P99_COMPILER_VERSION                                  \
 "open64 " __OPEN64__                                          \
 "; gnu "                                                      \
 P00_STRINGIFY(__GNUC__) "."                                  \
 P00_STRINGIFY(__GNUC_MINOR__) "."                            \
 P00_STRINGIFY(__GNUC_PATCHLEVEL__)

/* compilers that (as far as we know) don't pretend to be gcc */
#elif defined(__BORLANDC__)
# define P99_COMPILER P99_COMPILER_BORLAND
# define P99_COMPILER_VERSION "borland "
#elif defined(__COMO__)
# define P99_COMPILER P99_COMPILER_COMEAU
# define P99_COMPILER_VERSION "comeau "
#elif defined(_CRAYC)
# define P99_COMPILER P99_COMPILER_CRAY
# define P99_COMPILER_VERSION "cray "
#elif defined(__DECC_VER)
# define P99_COMPILER P99_COMPILER_DEC
# define P99_COMPILER_VERSION "dec "
#elif defined(__HP_cc)
# define P99_COMPILER P99_COMPILER_HP
# define P99_COMPILER_VERSION "hp "
#elif defined(__IBMC__)
# define P99_COMPILER P99_COMPILER_IBM
# define P99_COMPILER_VERSION "ibm "
#elif defined(__KCC)
# define P99_COMPILER P99_COMPILER_KAI
# define P99_COMPILER_VERSION "kai "
#elif defined(__LCC__)
# define P99_COMPILER P99_COMPILER_LCC
# define P99_COMPILER_VERSION "lcc "
#elif defined(__MWERKS__)
# define P99_COMPILER P99_COMPILER_METROWERKS
# define P99_COMPILER_VERSION "metrowerks "
#elif defined(_MSC_VER)
# define P99_COMPILER P99_COMPILER_MICROSOFT
# define P99_COMPILER_VERSION "microsoft "
#elif defined(__PGI)
# define P99_COMPILER P99_COMPILER_PORTLAND
# define P99_COMPILER_VERSION "portland "
#elif defined(__sgi)
# define P99_COMPILER P99_COMPILER_SGI
# define P99_COMPILER_VERSION "sgi "
#elif defined(__SUNPRO_C)
# define P99_COMPILER P99_COMPILER_SUN
# define P99_COMPILER_VERSION "sun "
#elif defined(__WATCOMC__)
# define P99_COMPILER P99_COMPILER_WATCOM
# define P99_COMPILER_VERSION "watcom "

/* put gcc last */
#elif defined(__GNUC__)
# define P99_COMPILER P99_COMPILER_GNU
# define P99_COMPILER_VERSION                                  \
 "gnu "                                                        \
 P00_STRINGIFY(__GNUC__) "."                                  \
 P00_STRINGIFY(__GNUC_MINOR__) "."                            \
 P00_STRINGIFY(__GNUC_PATCHLEVEL__)

#else
# define P99_COMPILER 0x0U
#endif

# ifdef __GNUC__
#  define P00_GCC_VERSION(A, B, C) P00_GCC_VERSION_(A, B, C)
#  define P00_GCC_VERSION_(A, B, C) A ## B ## C ## UL
#  ifdef __GNUC_PATCHLEVEL__
#   if __GNUC_PATCHLEVEL__ < 10
#    define P00_GNUC_PATCHLEVEL__ P00_PREFIX0(__GNUC_PATCHLEVEL__)
#   else
#    define P00_GNUC_PATCHLEVEL__ __GNUC_PATCHLEVEL__
#   endif
#  else
#    define P00_GNUC_PATCHLEVEL__ 00
#  endif
#  if __GNUC_MINOR__ < 10
#   define P00_GNUC_MINOR__ P00_PREFIX0(__GNUC_MINOR__)
#  else
#   define P00_GNUC_MINOR__ __GNUC_MINOR__
#  endif
#  define P99_GCC_VERSION P00_GCC_VERSION(__GNUC__, P00_GNUC_MINOR__, P00_GNUC_PATCHLEVEL__)
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
# define P00_VA_ARGS__(...) __VA_ARGS__
enum { p00_trailing_comma_in_enum__ = -1, };
inline
signed p00_trailing_comma_in_initializer__(void) {
  signed a[] = { p00_trailing_comma_in_enum__ , };
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
# define p99_inline __attribute__((always_inline)) __inline__
#elif P99_COMPILER & (P99_COMPILER_GNU | P99_COMPILER_OPEN64)
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
# define p99_inline __attribute__((always_inline)) inline
#endif


# ifndef static_inline
#  define static_inline static inline
# endif
# ifndef p99_inline
/**
 ** @brief Try to force a function to be always inlined
 **
 ** Since there is no default language construct to ensure this, this
 ** will always only be an approximation depending on the compiler.
 **
 ** The functions that are defined by P99 itself are declared/defined
 ** with this.
 **/
#  define p99_inline static inline
# endif

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_INTEL | P99_COMPILER_OPEN64)
# if P99_GCC_VERSION >= 30000UL
#  define P99_EXPECT(EXP, VAL) __builtin_expect((EXP), (VAL))
# endif
#endif

/**
 ** @brief A token sequence that can be used in a @c #pragma directive
 ** to indicate a parallel for loop.
 **
 ** When using this in a @c #pragma, the resulting code then may be
 ** parallelized and (if the platform
 ** supports this) multiple threads may be used to speed up the
 ** execution. This will only be effective if your compiler supports
 ** the <a href="http://openmp.org/wp/">Open Multi-Processing</a>
 ** extension (OpenMP for short, or OMP even shorter) and you told him
 ** to use it.
 **/
#ifdef _OPENMP
#define P99_PARALLEL_PRAGMA omp parallel for
#else
#define P99_PARALLEL_PRAGMA
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

/**
 ** @brief Mark the conditional expression as being unlikely
 **
 ** This can e.g be useful to test returns of system functions. Many
 ** of these return @c 0 if everything goes well and something else
 ** otherwise.
 **
 ** Use with care, humans are particularly bad concerning branch
 ** prediction. If unsure, first test your conditional for some time
 **
 ** to see that the case is really as unlikely as you think.
 ** @see P99_EXPECT
 ** @see P99_LIKELY
 **/
#ifndef P99_UNLIKELY
# define P99_UNLIKELY(...) P99_EXPECT((__VA_ARGS__), 0)
#endif

/**
 ** @brief Mark the conditional expression as being likely
 **
 ** This can e.g be useful to test the return value of @c printf.
 **
 ** Use with care, humans are particularly bad concerning branch
 ** prediction. If unsure, first test your conditional for some time
 ** to see that the case is really as likely as you think.
 **
 ** @see P99_EXPECT
 ** @see P99_UNLIKELY
 **/
#ifndef P99_LIKELY
# define P99_LIKELY(...) P99_EXPECT((__VA_ARGS__), 1)
#endif


#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
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
