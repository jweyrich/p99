/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2011 Emmanuel Jeanvoine, INRIA, France                      */
/* (C) copyright  2010-2012 Jens Gustedt, INRIA, France                       */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_C99_H_
# define    P99_C99_H_

/**
 ** @file
 ** @brief C99 specific include files that are required by the standard.
 **
 ** Using this file adds some restrictions about keywords on your
 ** code. Most prominent are probably @c bool, @c true, @c false and
 ** @c I, but there a lot more such as
 ** @c and @c and_eq @c bitand @c bitor @c compl @c not @c not_eq @c or @c or_eq @c xor @c xor_eq
 **/

#include "p99_compiler.h"

#if __STDC_HOSTED__ || defined(P00_DOXYGEN)
# ifndef __STDC_NO_COMPLEX__
#  include <complex.h>
# endif
# include <ctype.h>
# include <errno.h>
# include <fenv.h>
# include <inttypes.h>
# include <locale.h>
# include <math.h>
# include <setjmp.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# ifndef P00_NO_HAVE_TGMATH_H
#  include <tgmath.h>
# endif
# include <time.h>
# include <wchar.h>
# include <wctype.h>
#endif

/* Additions by C11 */
# if __STDC_VERSION__ > 201100L
#  ifndef  __STDC_NO_ATOMICS__
#   include <stdatomic.h>
#  endif
#  ifndef __STDC_NO_THREADS__
#   include <threads.h>
#  endif
# endif
#endif      /* !P99_C99_H_ */
