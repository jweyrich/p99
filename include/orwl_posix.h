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
#ifndef   	ORWL_POSIX_H_
#define   	ORWL_POSIX_H_

/**
 ** @file
 ** @brief Define feature macros and include all necessary files from
 ** C99 and POSIX.
 **/

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#elif _XOPEN_SOURCE < 600
# error "Need at least XOPEN specification 6 to compile this file"
#endif
#ifndef _BSD_SOURCE
# define _BSD_SOURCE 1
#endif

#ifdef __gnu_linux__
#include <features.h>
#endif

#ifdef ORWL_NO_SEM_NOINTR
# include "p99_c99.h"
# include <semaphore.h>
#else
# include "semaphore_nointr.h"
#endif
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <nl_types.h>

#endif 	    /* !ORWL_POSIX_H_ */
