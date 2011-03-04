/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_DOCUMENT_H_
# define   	ORWL_DOCUMENT_H_

#include "p99_defarg.h"

/** @brief The version of the ORWL library indicated by the commit date.
 **
 ** This should appear as an RFC2822 style date as you encounter e.g in emails.
 ** If this doesn't contain a date but a bizarre `format' with $ signs
 ** you have a version that is directly taken from the git repostitory.
 **/
#define ORWL_VERSION_DATE "$Format:%cd$"

/** @brief The version of the ORWL library indicated by a unique commit ID.
 **
 ** This should appear as a long string of hexadecimal digits.
 ** If this contains but a bizarre "format" with $ signs
 ** you have a version that is directly taken from the git repostitory.
 **/
#define ORWL_VERSION_ID "$Format:%H$"

#define ORWL__VERSION_ID "$Id$"

#define P99_VA_ARGS_DOCUMENTATION(NAME)                                                                                               \
                                                                                                                                      \
/*! @see P99_VA_ARGS */                                                                                                               \
/*! This is actually implemented as a macro that helps to provide the length of the variable length argument list to the function. */


/**
 ** @brief Add some indications to a @c _init documentation.
 */
#define DOCUMENT_INIT(T)                                                        \
/*! @brief Initialize a variable of type T                                   */ \
/*! @see P99_NEW needs a version of this that takes just the T* as argument. */ \
/*! @memberof T                                                              */

P99_MACRO_END(DOCUMENT_INIT);

/**
 ** @brief Add some indications to a @c _destroy documentation.
 */
#define DOCUMENT_DESTROY(T)                                              \
/*! @brief Destroy a variable of type T @see T ## _delete needs this. */ \
/*! @memberof T                                                       */

#endif 	    /* !ORWL_DOCUMENT_H_ */
