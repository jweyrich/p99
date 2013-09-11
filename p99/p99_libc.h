/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work is as follows :            */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_LIBC_H
#define P99_LIBC_H

/**
 ** @file
 ** @brief Group C library dependencies together in one file
 **/

#include <float.h>
/* For a conforming compiler, this should now have been defined. Set
   it to "indeterminable" otherwise. */
#ifndef FLT_EVAL_METHOD
# define FLT_EVAL_METHOD (-1)
#endif
#ifndef P00_NO_HAVE_ISO646_H
# include <iso646.h>
#endif
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* This is defined in some header file since C11. Since C11 allows to
   repeat typedefs as long as they resolve to the same type, this can
   be done at any place. */
typedef size_t rsize_t;

#ifndef RSIZE_MAX
/* This is the recommended practice if there is no other value
   available. */
# define RSIZE_MAX (SIZE_MAX >> 1)
#endif

#if __STDC_HOSTED__
# include <assert.h>
# include <wchar.h>
# include <wctype.h>
#endif

/* implement emulation of some C11 features */
#if p99_has_feature(stdalign_h)
# include <stdalign.h>
#endif
#if p99_has_feature(stdnoreturn_h)
# include <stdnoreturn.h>
#endif
/* end C11 emulation support */

#if __STDC_HOSTED__
# if p99_has_feature(uchar_h)
#   include <uchar.h>
# else
/* Define the unicode character types directly. */
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
# endif
#endif /* uchar */


#endif
