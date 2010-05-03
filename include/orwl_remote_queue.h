/*
** orwl_remote_queue.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Apr  6 15:29:03 2010 Jens Gustedt
** Last update Tue Apr  6 15:29:03 2010 Jens Gustedt
*/

#ifndef   	ORWL_REMOTE_QUEUE_H_
# define   	ORWL_REMOTE_QUEUE_H_

#include "orwl_wait_queue.h"
#include "orwl_socket.h"

/**
 ** @brief A structure to regulate queues between different servers.
 **
 ** @see orwl_rh
 **/
struct orwl_rq {
  pthread_mutex_t mut;  /**< control access during insertion */
  orwl_endpoint here;   /**< the local endpoint to which we report */
  orwl_endpoint there;  /**< the remote that centralizes the order */
  uint64_t ID;          /**< the id of the queue on the remote */
  orwl_wq local;        /**< the local queue that interfaces the
                           remote */
};

#ifndef __cplusplus
typedef struct orwl_rq orwl_rq;
#endif

#define ORWL_RQ_INITIALIZER { .mut = PTHREAD_MUTEX_INITIALIZER, .local = ORWL_WQ_INITIALIZER }

#ifndef DOXYGEN
inline
PROTOTYPE(orwl_rq *, orwl_rq_init, orwl_rq *, orwl_endpoint, orwl_endpoint, uint64_t);

#define orwl_rq_init(...) CALL_WITH_DEFAULTS(orwl_rq_init, 4, __VA_ARGS__)
#endif

inline
orwl_rq *orwl_rq_init(orwl_rq *rq, orwl_endpoint h, orwl_endpoint t, uint64_t id) {
  pthread_mutex_init(&rq->mut);
  orwl_wq_init(&rq->local);
  rq->here = h;
  rq->there = t;
  rq->ID = id;
  return rq;
}

DECLARE_DEFARG(orwl_rq_init, , (orwl_endpoint){{0}}, (orwl_endpoint){{0}}, TNULL(uint64_t));

inline
void orwl_rq_destroy(orwl_rq *rq) {
  orwl_wq_destroy(&rq->local);
  orwl_endpoint_destroy(&rq->here);
  orwl_endpoint_destroy(&rq->there);
  rq->ID = 0;
}

DECLARE_NEW_DELETE(orwl_rq);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_rq);

struct orwl_rh {
  /**
   ** @brief The queue that regulates the local accesses.
   **/
  orwl_rq *rq;
  /**
   ** @brief The handle in the local queue.
   **/
  orwl_wh *wh;
  /**
   ** @brief An ID of a local or remote handle that holds the global lock.
   **
   ** This will be notified whence here locally we have released the lock.
   **/
  uint64_t svrID;
};

#ifndef __cplusplus
typedef struct orwl_rh orwl_rh;
#endif

#define ORWL_RH_INITIALIZER INITIALIZER

inline
orwl_rh *orwl_rh_init(orwl_rh *rh) {
  memset(rh, 0, sizeof(orwl_rh));
  return rh;
}

inline
void orwl_rh_destroy(orwl_rh *rh) {
  orwl_rh_init(rh);
}

DECLARE_NEW_DELETE(orwl_rh);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_rh);

orwl_state orwl_request_excl(orwl_rq *rq, orwl_rh* rh, rand48_t *seed);
orwl_state orwl_request_incl(orwl_rq *rq, orwl_rh* rh, rand48_t *seed);
orwl_state orwl_release(orwl_rh* rh, rand48_t *seed);

inline
orwl_state orwl_acquire(orwl_rh* rh) {
  return orwl_wh_acquire(rh->wh, 0);
}

inline
orwl_state orwl_test(orwl_rh* rh) {
  return orwl_wh_test(rh->wh, 0);
}

DECLARE_AUTH_SOCK_FUNC(auth_sock_request_excl, uintptr_t wqID, uint64_t whID, uint64_t port);
DECLARE_AUTH_SOCK_FUNC(auth_sock_request_incl, uintptr_t wqID, uint64_t cliID, uint64_t svrID, uint64_t port);
DECLARE_AUTH_SOCK_FUNC(auth_sock_release, uintptr_t whID);





#endif 	    /* !ORWL_REMOTE_QUEUE_H_ */
