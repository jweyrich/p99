/* This may look like nonsense, but it really is -*- mode: C -*-             */
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
/**
 ** @file orwl_types.c
 **
 ** @brief Collect the type information in one big meta register
 **/


#include "p99_id.h"
#include "orwl_wait_queue.h"
#include "orwl_socket.h"

DECLARE_ORWL_DOMAIN(ORWL_FTAB(orwl_state));

DEFINE_ONCE(orwl_state) { }

DEFINE_ORWL_TYPES(orwl_state,
                  orwl_wq,
                  orwl_wh,
                  auth_sock
                  );

