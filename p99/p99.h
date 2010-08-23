/**
 ** p99.h
 **
 ** Made by Jens Gustedt
 ** Login   <gustedt@damogran.loria.fr>
 **
 ** Started on  Mon Aug 23 15:59:41 2010 Jens Gustedt
 ** Last update Mon Aug 23 15:59:41 2010 Jens Gustedt
 **/

#ifndef   	P99_H_
# define   	P99_H_

/**
 ** @file
 ** @brief Use this to include all of P99.
 **/

/**
 ** @mainpage P99 - Preprocessor macros and functions for c99
 **
 ** P99 aims to provide utilities that are only possible with the
 ** @ref c99 "features"
 ** that come with C99 (read ANSI C as normalized in 1999)
 ** and that where absent in C89. The features include among others
 **  - @ref variadic
 **  - @ref inline
 **  - @ref initializers
 **  - @ref compound
 **
 ** With these it implements @ref utilities "utilities" that
 ** previously had not been possible to program in C (or C macros) or
 ** that were very difficult to implement
 **  - @ref defaults
 **  - @ref blocks
 **  - @ref condi
 **  - @ref alloc
 **
 ** P99 also provides numerous facilities for
 ** @ref programming "macro programming."
 **
 ** P99 is not a C library in the classical sense but merely a
 ** collection of include files:
 **  - There is no binary library that is to be linked to your
 **    executable. The few functions that are provided are small
 **    wrappers that are compiled directly into your code.
 **  - There is nothing to configure, P99 include files should work
 **    out of the box with any conforming C99 compiler.
 **
 **
 ** @section credits Credits and Rights
 ** @subsection author Author and Maintainer
 ** @author <a href="http://www.loria.fr/~gustedt/">Jens Gustedt</a>
 ** @date 2010
 **
 ** @subsection copyright Copyright
 ** Copyright &copy; 2010 Jens Gustedt, INRIA, France, http://www.inria.fr/
 **
 ** Permission to use, copy, modify, and distribute this software and
 ** its documentation under the terms as provided in the file LICENSE
 ** is hereby granted. No representations are made about the
 ** suitability of this software for any purpose. It is provided "as
 ** is" without express or implied warranty.
 **
 ** @section utilities Implemented utilities
 ** @subsection defaults Default arguments to functions
 ** @subsection blocks Guarded blocks
 ** @subsection condi Preprocessor conditionals and loops
 ** @subsection alloc Allocation and initialization facilities
 **
 ** @section programming Macro programming with P99
 **
 ** @section c99 C99 features
 ** @subsection variadic Variadic macros
 ** @subsection inline Inline functions
 ** @subsection initializers Named initializers
 ** @subsection compound Compound literals
 **/



#include "p99_block.h"
#include "p99_choice.h"
#include "p99_defarg.h"
#include "p99_int.h"
#include "p99_new.h"

#endif 	    /* !P99_H_ */
