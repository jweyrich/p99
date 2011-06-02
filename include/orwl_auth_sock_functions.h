/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_AUTH_SOCK_FUNCTIONS_H_
# define   	ORWL_AUTH_SOCK_FUNCTIONS_H_

#include "orwl_socket.h"


DECLARE_ORWL_PROC_FUNC(orwl_proc_write_request, uint64_t wqPOS, uint64_t whID, uint64_t port);
DECLARE_ORWL_PROC_FUNC(orwl_proc_read_request, uint64_t wqPOS, uint64_t cliID, uint64_t svrID, uint64_t port);
DECLARE_ORWL_PROC_FUNC(orwl_proc_release, uintptr_t whID);
DECLARE_ORWL_PROC_FUNC(orwl_proc_insert_peer, uint64_t port);
DECLARE_ORWL_PROC_FUNC(orwl_proc_insert_host, uint64_t addr, uint64_t port);
DECLARE_ORWL_PROC_FUNC(orwl_proc_do_nothing, void);
DECLARE_ORWL_PROC_FUNC(orwl_proc_check_initialization, uint64_t id_pow2);


#endif 	    /* !ORWL_AUTH_SOCK_FUNCTIONS_H_ */
