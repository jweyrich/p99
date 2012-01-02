/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2010-2011 Jens Gustedt, INRIA, France                      */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_COMPILER_H
#define P99_COMPILER_H

#include "p99_args.h"

/**
 ** @file
 ** @brief Group compiler dependencies together in one file
 **
 ** This has been tested with different versions of gcc (GNU, versions 4.0
 ** to 4.4), clang, opencc (OPEN64) and icc (INTEL).
 **/


/**
 ** @def P99_COMPILER
 **
 ** @brief Identify the compiler
 **
 ** All compilers have some specific macro that identifies them but it
 ** is difficult for user code to track all of them. This macro here
 ** can be used to identify the compiler in a portable way. E.g by
 ** something like
 **
 ** @code
 ** #if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
 ** ...
 ** #endif
 ** @endcode
 **
 ** See @link p99_generated.h @endlink for a list of the compilers
 ** that are known to P99.
 **/

/**
 ** @addtogroup compiler Compiler Specific Tuning and Extensions
 **
 ** Many compilers provide extensions that could be
 ** useful. Unfortunately similar extension are often presented
 ** differently with different compilers. Here we try to give macros
 ** that abstract from these compiler specifics.
 **
 ** As I personally mainly have compilers that all claim themselves to
 ** be compatible with gcc, this collection is strongly biased towards
 ** gcc extensions. Please don't hesitate to point me to documentation
 ** of other compilers that could be useful, here.
 **
 ** @see P99_COMPILER
 ** @see P99_IF_COMPILER
 ** @{
 **/


/**
 ** @example test-p99-conformance.c
 ** @brief That file provides a test for C99 conformance.
 **
 ** It only tests the "compiler" part of C, that is without any
 ** include files. Any C implementation, whether hosted or
 ** freestanding should comply to this.
 **
 ** To convince your compiler to compile this you have perhaps to
 ** provide some additional parameters on the command line. E.g for
 ** the gcc family of compilers (including third party pretenders) you
 ** usually have to give "-std=c99" to switch to C99 mode. But other
 ** parameters may be in order, consult your compiler documentation.
 **
 ** This file is split into several parts that hopefully are
 ** self explaining. Each of the parts has a macro @c SKIP_... that
 ** lets you skip a test.
 **/

#define P00_PREFIX0(N) P00_PREFIX0_(N)
#define P00_PREFIX0_(N) 0 ## N
#define P00_STRINGIFY(...) #__VA_ARGS__
/**
 ** @brief Transform the argument list into one string.
 **/
#define P99_STRINGIFY(...) P00_STRINGIFY(__VA_ARGS__)

/* be sure to put all compilers that are faking gcc before gcc itself */
#if P99_COMPILER & P99_COMPILER_CLANG
# undef P00_COMPILER_PRAGMA_CLANG
# define P00_COMPILER_PRAGMA_CLANG(STR) _Pragma(STR)
# undef P99_COMPILER_VERSION
# define P99_COMPILER_VERSION                                  \
 "clang "                                                      \
 __clang_version__                                             \
 "; gnu "                                                      \
 P99_STRINGIFY(__GNUC__) "."                                   \
 P99_STRINGIFY(__GNUC_MINOR__) "."                             \
 P99_STRINGIFY(__GNUC_PATCHLEVEL__)

#elif P99_COMPILER & P99_COMPILER_INTEL
# undef P99_COMPILER_VERSION
# define P99_COMPILER_VERSION                                  \
 "intel " P99_STRINGIFY(__INTEL_COMPILER)                      \
 "; gnu "                                                      \
 P99_STRINGIFY(__GNUC__) "."                                   \
 P99_STRINGIFY(__GNUC_MINOR__) "."                             \
 P99_STRINGIFY(__GNUC_PATCHLEVEL__)

#elif P99_COMPILER & P99_COMPILER_PCC
# undef P99_COMPILER_VERSION
# define P99_COMPILER_VERSION                                  \
"pcc "                                                         \
 P99_STRINGIFY(__PCC__)                                        \
 "." P99_STRINGIFY(__PCC_MINOR__)                              \
 "." P99_STRINGIFY(__PCC_MINORMINOR__)                         \
 "; gnu "                                                      \
 P99_STRINGIFY(__GNUC__) "."                                   \
 P99_STRINGIFY(__GNUC_MINOR__) "."                             \
 P99_STRINGIFY(__GNUC_PATCHLEVEL__)

#elif P99_COMPILER & P99_COMPILER_TINYC
# undef P99_COMPILER_VERSION
# define P99_COMPILER_VERSION                                  \
  "tinyc "                                                     \
  P99_STRINGIFY(__TINYC__)

#elif P99_COMPILER & P99_COMPILER_OPEN64
# undef P99_COMPILER_VERSION
# define P99_COMPILER_VERSION                                  \
 "open64 " __OPEN64__                                          \
 "; gnu "                                                      \
 P99_STRINGIFY(__GNUC__) "."                                   \
 P99_STRINGIFY(__GNUC_MINOR__) "."                             \
 P99_STRINGIFY(__GNUC_PATCHLEVEL__)

#elif P99_COMPILER & P99_COMPILER_GNU
# undef P99_COMPILER_VERSION
# define P99_COMPILER_VERSION                                  \
 "gnu "                                                        \
 P99_STRINGIFY(__GNUC__) "."                                   \
 P99_STRINGIFY(__GNUC_MINOR__) "."                             \
 P99_STRINGIFY(__GNUC_PATCHLEVEL__)
#endif

/* intel is cheating about the gcc abi they support */
#if P99_COMPILER & P99_COMPILER_INTEL
# if (__ICC < 1300) && (__GNUC__ == 4) && (__GNUC_MINOR__ > 2)
#  undef __GNUC_MINOR__
#  define __GNUC_MINOR__ 2
#  undef __GNUC_PATCHLEVEL__
#  define __GNUC_PATCHLEVEL__ 0
# endif
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

#ifndef P00_DOXYGEN
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
# if  (!P99_TOK_EQ(1, 1) || P99_TOK_EQ(1, 0))
/* If you come here your preprocessor is not able to expand P99 macros
   correctly. Most probably this is a bug in your preprocessor
   implementation, but it could also be that your implementation just
   interprets the standard different. In any case, we can't
   proceed. */
#  error "The preprocessor is not P99 compatible"
# endif
#endif
#endif

#if P99_COMPILER & P99_COMPILER_INTEL
# define p99_inline __attribute__((always_inline)) inline
# ifndef __GNUC__
#  define P00_NO_HAVE_TGMATH_H
# endif
#elif P99_COMPILER & P99_COMPILER_PCC
//# error "The P99 preprocessor files can't work with the pcc compiler, yet"
#elif P99_COMPILER & P99_COMPILER_CLANG
# define p99_inline __attribute__((always_inline)) inline
/* clang can't nail a variable to a register, yet */
# define P99_FIXED_REGISTER(REG)
#elif P99_COMPILER & (P99_COMPILER_GNU | P99_COMPILER_OPEN64)
# define P99_ATLEAST
/* gcc prior to version 4.2.1 has the inline keyword but with slightly
   different semantics.
   Be sure to always inline functions in this cases.
   */
# if !defined(__GNUC_STDC_INLINE__) && P99_GCC_VERSION < 40300UL
#  ifdef inline
#   undef inline
#  endif
#  ifdef __GNUC_GNU_INLINE__
#   define inline __attribute__((gnu_inline,weak)) __inline__
#   define p99_inline __attribute__((always_inline,gnu_inline,weak)) __inline__
#  else
#   define inline __attribute__((weak)) __inline__
#   define p99_inline __attribute__((always_inline,weak)) __inline__
#  endif
#  define p00_instantiate
# else
#  define inline __inline__
#  define p00_instantiate extern __inline__
#  define p99_inline __attribute__((always_inline)) __inline__
# endif
#endif

# if !defined(p99_inline) || defined(P00_DOXYGEN)
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

# ifndef p00_instantiate
/**
 ** @brief Force a function symbol to be emitted.
 **
 ** The default that is used for this is to add the keywords
 ** <code>extern inline</code>. By that the definition that has been
 ** seen in this unit is not an @c inline definition and thus the
 ** function is generated and a symbol is emitted.
 **/
# define p00_instantiate extern inline
# endif

#ifndef P99_FIXED_REGISTER
# ifdef __GNUC__
#  define P99_FIXED_REGISTER(REG) __asm__(P99_STRINGIFY(REG))
# else
/**
 ** @brief Fix a variable to a specific register, if the platform
 ** supports this.
 **
 **/
#  define P99_FIXED_REGISTER(REG)
# endif
#endif


/**
 ** @addtogroup C11 Emulating features of C11
 **
 ** The new C standard C11 (published in December 2011) introduces
 ** some new features that are already present in many compilers or
 ** OS, but sometimes with different syntax or interfaces. We provide
 ** interfaces to some of them with the intention that once compilers
 ** that implement C11 come out these interfaces can directly relate
 ** to the C11 feature.
 **
 ** @{
 **/

/**
 ** @addtogroup C11_keywords Some new C11 keywords
 **
 ** @{
 **/

#if P99_GCC_VERSION >= 40600UL
/* In C1x this macro must exactly have that value. */
#define static_assert _Static_assert
#endif

/* implement emulation of some C11 features */
#if  __STDC_VERSION__ > 201101L
# include <assert.h>
# include <stdalign.h>
# include <stdnoreturn.h>
#endif /* C11 emulation support */

#ifndef static_assert
/**
 ** @brief Evaluate expression @a EXPR at compile time and ensure that
 ** it is fulfilled.
 **
 ** The expression must be given as first argument. The second
 ** argument is a diagnostic string that should be part of the
 ** diagnostic message when the assertion fails.
 **
 ** Not all compilers give a useful message when an assertion is not
 ** fulfilled, but all should give at least the line number and stop
 ** compilation.
 **
 ** @remark This functionality will be directly supported in C1x.
 **/
#define static_assert(EXPR, DIAGSTR)                              \
extern char const p00_compiletime_assert[                         \
 sizeof((void const*[3*(!!(EXPR)) - 1]){                          \
    &p00_compiletime_assert,                                      \
   "static assertion failed: " P99_STRINGIFY(EXPR) ", " DIAGSTR}) \
]
extern char const p00_compiletime_assert[sizeof(void const*[2])];
#endif

#ifndef alignof
#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_INTEL | P99_COMPILER_OPEN64)
#define _Alignof(T) __alignof__(T)
#define alignof _Alignof
#endif
#endif

#ifndef alignas
#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_INTEL | P99_COMPILER_OPEN64)
#define _Alignas(X) __attribute__((__aligned__(X)))
#define alignas _Alignas
#endif
#endif

#ifndef noreturn
#define noreturn _Noreturn
#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_INTEL | P99_COMPILER_OPEN64)
/* use __noreturn__ for the attribute since noreturn is now a macro */
#define _Noreturn __attribute__((__noreturn__))
#elif !defined(P00_DOXYGEN)
#define _Noreturn _Pragma(NORETURN)
#endif
#endif

#ifndef thread_local
#define thread_local _Thread_local
#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_INTEL | P99_COMPILER_OPEN64 | P99_COMPILER_IBM | P99_COMPILER_SUN)
#define _Thread_local __thread
#elif P99_COMPILER & (P99_COMPILER_MICROSOFT | P99_COMPILER_BORLAND)
#define _Thread_local __declspec(thread)
#endif
#endif



static_assert(1, "test of static assertions");

/**
 ** @}
 **/

/**
 ** @}
 **/

/**
 ** @addtogroup compiler_utilities A small collection of utilities
 **
 ** @{
 **/

#define P00_HARMLESS_DECLARATION                               \
extern char const p00_harmless_declaration[                    \
 sizeof((void const*[1]){ &p00_harmless_declaration })         \
]

extern char const p00_harmless_declaration[sizeof(void const*[1])];

/**
 ** @brief A meta macro that forces the addition of a semicolon after
 ** a call to the macro that it terminates.
 **
 ** This should only be used for macros that replace declarations.
 **/
#define P99_MACRO_END(...) P00_HARMLESS_DECLARATION


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
# define P99_UNLIKELY(...) P99_EXPECT(!!(__VA_ARGS__), 0)
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
# define P99_LIKELY(...) P99_EXPECT(!!(__VA_ARGS__), 1)
#endif


/**
 ** @def P99_DEPRECATED
 ** @brief Deprecate a declaration that is given as the argument list.
 **/
#if !defined(P99_DEPRECATED) || defined(P00_DOXYGEN)
# ifdef __GNUC__
#  define P99_DEPRECATED(...) /*! \deprecated __VA_ARGS__ */ __attribute__((deprecated))
# else
#  define P99_DEPRECATED(...) /*! \deprecated __VA_ARGS__ */
# endif
#endif

/**
 ** @def P99_ATLEAST
 ** @brief Specify for array parameters that they have a least amount of elements.
 **
 ** This is an addition through an abuse of the @c static keyword that
 ** older compilers might not support.
 **/
#ifndef P99_ATLEAST
# define P99_ATLEAST static
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

/* special repair work for non-compliant compilers */
#if P99_COMPILER & P99_COMPILER_INTEL
# ifndef __GNUC__
#  define P00_NO_HAVE_TGMATH_H
# endif
#endif

/**
 ** @brief Issue the pragma that is given as supplementary argument
 ** iff the actual compiler is @a COMP.
 ** @see P99_COMPILER
 **/
#define P99_IF_COMPILER(COMP, ...) P00_COMPILER_PRAGMA_ ## COMP(P99_STRINGIFY(__VA_ARGS__))

#ifndef P00_DOXYGEN

/* Disable bogus warnings that are provoked by the code in this file. */

P99_IF_COMPILER(INTEL, warning(disable: 1418)) /* external function definition with no prior declaration */
P99_IF_COMPILER(INTEL, warning(disable: 1419)) /* external declaration in primary source file */

#endif

/** @}
 **/


/** @}
 **/

#endif
