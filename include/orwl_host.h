/*
** orwl_host.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May  7 20:14:54 2010 Jens Gustedt
** Last update Fri May  7 20:14:54 2010 Jens Gustedt
*/

#ifndef   	ORWL_HOST_H_
# define   	ORWL_HOST_H_

#include "orwl_endpoint.h"

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

#define ORWL_HOST_INITIALIZER(NAME, NADDR,  NPORT) {    \
    .mut = PTHREAD_MUTEX_INITIALIZER,                   \
    .ep = ORWL_ENDPOINT_INITIALIZER(NADDR, NPORT),      \
      .prev = &NAME,                                    \
      .next = &NAME,                                    \
      .refs = 2                                         \
      }

void orwl_host_connect(orwl_host *th, orwl_host *q);
void orwl_host_disconnect(orwl_host *th);

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

#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_host*, orwl_host_init, orwl_host *, in_addr_t, in_port_t);
#define orwl_host_init(...) P99_CALL_DEFARG(orwl_host_init, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(orwl_host_init, , P99_0(in_addr_t), P99_0(in_port_t));
#endif

inline
void orwl_host_destroy(orwl_host *th) {
  orwl_host_disconnect(th);
}

DECLARE_NEW_DELETE(orwl_host);


#endif 	    /* !ORWL_HOST_H_ */
