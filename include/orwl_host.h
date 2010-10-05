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
#ifndef   	ORWL_HOST_H_
# define   	ORWL_HOST_H_

#include "orwl_endpoint.h"
#include P99_ADVANCE_ID

/**
 ** @brief List element to hold the hosts to which an ::orwl_server
 ** has been connected.
 **/
struct orwl_host {
  pthread_mutex_t mut;
  orwl_endpoint ep;
  orwl_host *prev;
  orwl_host *next;
  size_t refs;
};

#define ORWL_HOST_INITIALIZER(NAME, NADDR,  NPORT) {           \
    .mut = PTHREAD_MUTEX_INITIALIZER,                          \
    .ep = ORWL_ENDPOINT_INITIALIZER(NADDR, NPORT),             \
      .prev = &NAME,                                           \
      .next = &NAME,                                           \
      .refs = 2                                                \
      }

/**
 ** @memberof orwl_host
 **/
void orwl_host_connect(orwl_host *th, orwl_host *q);

/**
 ** @memberof orwl_host
 **/
void orwl_host_disconnect(orwl_host *th);

#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_host*, orwl_host_init, orwl_host *, in_addr_t, in_port_t);
#define orwl_host_init(...) P99_CALL_DEFARG(orwl_host_init, 3, __VA_ARGS__)
#define orwl_host_init_defarg_1() P99_0(in_addr_t)
#define orwl_host_init_defarg_2() P99_0(in_port_t)
#endif

DOCUMENT_INIT(orwl_host)
P99_DEFARG_DOCU(orwl_host_init)
inline
orwl_host* orwl_host_init(orwl_host *th,  /*!< [out] the object to iniialize */
                          in_addr_t addr, /*!< [in] defaults to the null address */
                          in_port_t port  /*!< [in] defaults to 0 */
                          ) {
  if (!th) return NULL;
  th->next = th;
  th->prev = th;
  th->refs = 0;
  orwl_endpoint_init(&th->ep, addr, port);
  pthread_mutex_init(&th->mut, NULL);
  return th;
}

DOCUMENT_DESTROY(orwl_host_init)
inline
void orwl_host_destroy(orwl_host *th) {
  orwl_host_disconnect(th);
}

DECLARE_NEW_DELETE(orwl_host);


#endif 	    /* !ORWL_HOST_H_ */
