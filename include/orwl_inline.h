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

#endif
