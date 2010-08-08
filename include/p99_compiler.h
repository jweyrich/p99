#ifndef P99_COMPILER_H
#define P99_COMPILER_H

/** @file
 ** @brief Group compiler dependencies together in one file
 **
 ** For the moment the only compilers we have tested are different
 ** versions of gcc and the only feature that causes problems is the
 ** @c inline keyword.
 **/

#define P99__PREFIX0(N) P99__PREFIX0_(N)
#define P99__PREFIX0_(N) 0 ## N

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

# ifdef __GNUC__
/* gcc prior to version 4.3 has the inline keyword but with slightly
   different semantics.
   Be sure to allways inline functions in this cases.
   */
#  if P99_GCC_VERSION < 40300UL
#   ifdef inline
#    undef inline
#   endif
#   define inline __attribute__((gnu_inline,always_inline)) static inline
#   define static_inline inline
#  endif
# endif

# ifndef static_inline
#  define static_inline static inline
# endif

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
#if (defined(__GNUC__) && (P99_GCC_VERSION >= 30000UL))
#define P99_EXPECT(EXP, VAL) __builtin_expect((EXP), (VAL))
#else
#define P99_EXPECT(EXP, VAL) (EXP)
#endif


#endif
