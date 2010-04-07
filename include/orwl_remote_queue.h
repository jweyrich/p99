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

struct orwl_rq {
  orwl_endpoint here;
  orwl_endpoint there;
  uint64_t ID;
  orwl_wq local;
};

#ifndef __cplusplus
typedef struct orwl_rq orwl_rq;
#endif

#define ORWL_RQ_INITIALIZER { .local = ORWL_WQ_INITIALIZER }

inline
orwl_rq *orwl_rq_init(orwl_rq *rq, orwl_endpoint h, orwl_endpoint t, uint64_t id) {
  orwl_wq_init(&rq->local);
  //orwl_endpoint_init(&rq->here);
  rq->here = h;
  //orwl_endpoint_init(&rq->there);
  rq->there = t;
  rq->ID = id;
  return rq;
}

#define orwl_rq_init(...) CALL_WITH_DEFAULTS(orwl_rq_init, 4, __VA_ARGS__)
declare_defarg(orwl_rq_init, 3, uint64_t, TNULL(uint64_t));
declare_defarg(orwl_rq_init, 2, orwl_endpoint, (orwl_endpoint){{0}});
declare_defarg(orwl_rq_init, 1, orwl_endpoint, (orwl_endpoint){{0}});

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
  orwl_wh *wh;
  orwl_rq *rq;
  uint64_t rID;
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

orwl_state orwl_request(orwl_rq *rq, orwl_rh* rh, size_t token, rand48_t *seed);
orwl_state orwl_release(orwl_rh* rh, rand48_t *seed);

inline
orwl_state orwl_acquire(orwl_rh* rh, size_t token) {
  return orwl_wh_acquire(rh->wh, token);
}

inline
orwl_state orwl_test(orwl_rh* rh, size_t token) {
  return orwl_wh_test(rh->wh, token);
}


void orwl_rq_serve_request(auth_sock *Arg);
void orwl_rq_trigger_acquired(auth_sock *Arg);
void orwl_rq_triggered_release(auth_sock *Arg);

DECLARE_ORWL_TYPE_DYNAMIC(auth_sock);
DECLARE_ORWL_REGISTER(orwl_rq_serve_request);
DECLARE_ORWL_REGISTER(orwl_rq_triggered_release);





#endif 	    /* !ORWL_REMOTE_QUEUE_H_ */
