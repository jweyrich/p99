#ifndef ORWL_INLINE_H

/* gcc prior to version 4.3 has the inline keyword but with slitely
   different semantics.
   Be sure to allways inline functions in this cases.
   */
# ifdef __GNUC__
#  ifdef inline
#   undef inline
#  endif
#  if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 3))
#   define inline __attribute__((gnu_inline, always_inline)) extern __inline__
#  else
#   define inline __inline__
#  endif
# endif


#endif
