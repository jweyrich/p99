/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef     ORWL_H_
# define    ORWL_H_

#include "orwl_handle2.h"
#include "orwl_posix.h"
#include "orwl_proc_symbols.h"
#include "orwl_rand.h"
#include "orwl_remote_queue.h"
#include "orwl_server.h"
#include "orwl_socket.h"
#include "orwl_thread.h"
#include "orwl_helpers.h"

/**
 ** @file
 ** @brief Use this to include all of ORWL.
 **/

/**
 ** @mainpage ORWL - Ordered Read-Write Locks
 **
 ** @section credits Credits and Rights
 ** @subsection author Author and Maintainer
 ** @author <a href="http://www.loria.fr/~gustedt/">Jens Gustedt</a>
 ** @date 2010 - 2011
 **
 ** @subsection version Version
 **
 ** The version that this documentation describes can be identified
 ** via the macros ::ORWL_VERSION_DATE, namely $Format:%cd$. It also is
 ** tagged with an hexadecimal ID tag that is given in
 ** ::ORWL_VERSION_ID, namely $Format:%H$.
 **
 ** @subsection copyright Copyright
 ** Copyright &copy; 2010-2011 Jens Gustedt, INRIA, France, http://www.inria.fr/
 **
 ** @htmlinclude SHORTLICENCE-closed.txt
 **
 ** @subsection license License
 **
 ** <pre>
 ** License is still to be defined.
 ** </pre>
 **/

/**
 ** @page configuration Configuration
 **
 ** This library currently uses
 **
 ** - GNU <code>make</code>
 ** - <code>getconf</code>
 ** - <code>tr</code>
 ** - <code>sed</code>
 ** - and your C compiler
 **
 ** to determine most of its configuration automatically at compile
 ** time. Relatively recent POSIX systems should not have any
 ** particular problem with that.
 **
 ** Your C compiler should be complying to the C99 standard. ORWL uses
 ** P99 under the hood, so you will probably not get away with a
 ** compiler that only has C89 with no extension gearing towards
 ** C99. It can be fine-tuned by the following @c make parameters
 ** - CC the name of the compiler
 ** - OPT additional options that you want to pass to the compilation phase
 ** example
 ** @code
 ** make CC=clang OPT='-march=native'
 ** @endcode
 **
 ** @see ATOMIC_OPS
 ** @see DEFINE__GCC_HAVE_SYNC_COMPARE_AND_SWAP
 ** @see GNUC_NO_SYNC
 ** @see GNUC_SYNC_REPLACE
 ** @see HAVE_ATOMIC
 ** @see NO_FUTEX
 ** @see NO_NANOSLEEP
 ** @see NO_USLEEP
 ** @see POSIX_BARRIERS
 ** @see POSIX_IPV6
 ** @see POSIX_SEMAPHORES
 ** @see POSIX_THREAD_PROCESS_SHARED
 ** @see POSIX_THREAD_SAFE_FUNCTIONS
 ** @see POSIX_TIMEOUTS
 **/

#ifdef DOXYGEN

#define O_RWL_DOCUMENT_CONFIG(NAME)                                                                            \
/*! @brief Configuration option */                                                                             \
/*! Generally configuration options are determined at compile time, see @ref configuration "configuration". */

/**
 ** @addtogroup configuration_parameters Configuration parameters
 ** @{
 **/

/**
 ** @addtogroup atomic_configuration Configuration of atomic operations
 ** @{
 **/

O_RWL_DOCUMENT_CONFIG(ATOMIC_OPS)
#define ATOMIC_OPS


O_RWL_DOCUMENT_CONFIG(DEFINE__GCC_HAVE_SYNC_COMPARE_AND_SWAP)
#define DEFINE__GCC_HAVE_SYNC_COMPARE_AND_SWAP


O_RWL_DOCUMENT_CONFIG(GNUC_NO_SYNC)
#define GNUC_NO_SYNC


O_RWL_DOCUMENT_CONFIG(GNUC_SYNC_REPLACE)
#define GNUC_SYNC_REPLACE


O_RWL_DOCUMENT_CONFIG(HAVE_ATOMIC)
#define HAVE_ATOMIC


/**
 ** @}
 **/

/**
 ** @addtogroup linux_configuration Linux specific configuration
 ** @{
 **/

O_RWL_DOCUMENT_CONFIG(NO_FUTEX)
/**
 ** Define this macro to compile the library without futex
 ** support. This is probably only useful for testing code coverage.
 **/
#define NO_FUTEX

/**
 ** @}
 **/

/**
 ** @addtogroup time_configuration Configuration parameters that concern measurement etc of time.
 ** @{
 **/

O_RWL_DOCUMENT_CONFIG(NO_NANOSLEEP)
/**
 ** Define this macro to compile the library without @c nanosleep. @c
 ** usleep will be used instead. This is probably only useful for
 ** testing code coverage.
 ** @see NO_USLEEP
 **/
#define NO_NANOSLEEP


O_RWL_DOCUMENT_CONFIG(NO_USLEEP)
/**
 ** Define this macro to compile the library without @c usleep. If @c
 ** nanosleep isn't available, too, a hack using @c select will be
 ** used instead. This is probably only useful for testing code
 ** coverage.  @see NO_NANOSLEEP
 **/
#define NO_USLEEP

/**
 ** @}
 **/


/**
 ** @addtogroup posix_configuration POSIX configuration parameters
 ** @brief ORWL uses a lot of different POSIX features
 **
 ** Some of these features are optional and can be replaced when this
 ** is detected at compile time.
 ** @{
 **/


O_RWL_DOCUMENT_CONFIG(POSIX_BARRIERS)
#define POSIX_BARRIERS


O_RWL_DOCUMENT_CONFIG(POSIX_IPV6)
#define POSIX_IPV6


O_RWL_DOCUMENT_CONFIG(POSIX_SEMAPHORES)
#define POSIX_SEMAPHORES


O_RWL_DOCUMENT_CONFIG(POSIX_THREAD_PROCESS_SHARED)
#define POSIX_THREAD_PROCESS_SHARED


O_RWL_DOCUMENT_CONFIG(POSIX_THREAD_SAFE_FUNCTIONS)
#define POSIX_THREAD_SAFE_FUNCTIONS


O_RWL_DOCUMENT_CONFIG(POSIX_TIMEOUTS)
#define POSIX_TIMEOUTS

/**
 ** @}
 **/

/**
 ** @}
 **/

#endif /* configuration options */

#endif      /* !ORWL_H_ */
