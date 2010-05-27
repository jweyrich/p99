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
#include "orwl_endpoint.h"

/**
 ** @brief A structure to regulate queues between different servers.
 **
 ** @msc
 **   orwl_server,orwl_mirror,cli_wh,orwl_handle;
 **   cli_wh<-orwl_handle [label="orwl_request()", URL="\ref orwl_request()", ID="1"];
 **   orwl_mirror<-cli_wh [label="orwl_request()", URL="\ref orwl_request()", ID="1"];
 **   orwl_server<-orwl_mirror [label="orwl_rpc()"];
 **   orwl_auth_sock<-orwl_server [label="orwl_auth_sock_request()"];
 **   orwl_auth_sock;
 **   orwl_auth_sock->orwl_server [label="Ack"];
 **   orwl_server->orwl_mirror [label="orwl_rpc()"];
 **   orwl_mirror->orwl_handle [label="Ack "];
 **   cli_wh<-orwl_handle [label="orwl_acquire"];
 **   orwl_auth_sock->cli_wh [label="acquired"];
 **   cli_wh->orwl_handle [label="orwl_acquired"];
 **   cli_wh<-orwl_handle [label="orwl_release"];
 **   cli_wh<-orwl_handle [label="orwl_release"];
 ** @endmsc
 ** @see orwl_handle
 **/
struct orwl_mirror {
  pthread_mutex_t mut;  /**< control access during insertion */
  orwl_endpoint here;   /**< the local endpoint to which we report */
  orwl_endpoint there;  /**< the remote that centralizes the order */
  orwl_wq local;        /**< the local queue that interfaces the
                           remote */
};

#define ORWL_MIRROR_INITIALIZER { .mut = PTHREAD_MUTEX_INITIALIZER, .local = ORWL_WQ_INITIALIZER }

#ifndef DOXYGEN
inline
PROTOTYPE(orwl_mirror *, orwl_mirror_init, orwl_mirror *, orwl_endpoint, orwl_endpoint);

#define orwl_mirror_init(...) CALL_WITH_DEFAULTS(orwl_mirror_init, 3, __VA_ARGS__)
#endif

inline
orwl_mirror *orwl_mirror_init(orwl_mirror *rq, /*!< [out] the object to iniialize */
                              orwl_endpoint h, /*!< [in] local, defaults to a temp variable */
                              orwl_endpoint t  /*!< [in] remote, defaults to a temp variable */
                              ) {
  pthread_mutex_init(&rq->mut);
  orwl_wq_init(&rq->local);
  rq->here = h;
  rq->there = t;
  return rq;
}

DECLARE_DEFARG(orwl_mirror_init, , (orwl_endpoint){ .index = 0 }, (orwl_endpoint){ .index = 0 });

inline
void orwl_mirror_destroy(orwl_mirror *rq) {
  orwl_wq_destroy(&rq->local);
  orwl_endpoint_destroy(&rq->here);
  orwl_endpoint_destroy(&rq->there);
}

DECLARE_NEW_DELETE(orwl_mirror);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_mirror);

/**
 ** @brief An ORWL lock handle for remote locations.
 **
 ** @see orwl_mirror
 ** @see orwl_read_request
 ** @see orwl_write_request
 ** @see orwl_acquire
 ** @see orwl_test
 ** @see orwl_release
 ** @see orwl_cancel
 **/
struct orwl_handle {
  /**
   ** @brief The queue that regulates the local accesses.
   **/
  orwl_mirror *rq;
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
typedef struct orwl_handle orwl_handle;
#endif

#define ORWL_HANDLE_INITIALIZER INITIALIZER

inline
orwl_handle *orwl_handle_init(orwl_handle *rh) {
  TZERO(*rh);
  return rh;
}

inline
void orwl_handle_destroy(orwl_handle *rh) {
  orwl_handle_init(rh);
}

DECLARE_NEW_DELETE(orwl_handle);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle);


orwl_state orwl_write_request(orwl_mirror* location, /*!< [in,out] the location for the request */
                              orwl_handle* handle,   /*!< [in,out] the handle for the request */
                              rand48_t* seed         /*!< [in] defaults to a thread local seed */
                              );

orwl_state orwl_read_request(orwl_mirror* location, /*!< [in,out] the location for the request */
                             orwl_handle* handle,   /*!< [in,out] the handle for the request */
                             rand48_t* seed         /*!< [in] defaults to a thread local seed */
                             );

orwl_state orwl_release(orwl_handle* handle,   /*!< [in,out] the handle to be released */
                        rand48_t* seed         /*!< [in] defaults to a thread local seed */
                        );

orwl_state orwl_cancel(orwl_handle* handle,   /*!< [in,out] the handle to be canceled */
                       rand48_t* seed         /*!< [in] defaults to a thread local seed */
                       );

#ifndef DOXYGEN
PROTOTYPE(orwl_state, orwl_write_request, orwl_mirror*, orwl_handle*, rand48_t*);
#define orwl_write_request(...)  CALL_WITH_DEFAULTS(orwl_write_request, 3, __VA_ARGS__)
DECLARE_DEFARG(orwl_write_request, , , seed_get());

PROTOTYPE(orwl_state, orwl_read_request, orwl_mirror*, orwl_handle*, rand48_t*);
#define orwl_read_request(...)  CALL_WITH_DEFAULTS(orwl_read_request, 3, __VA_ARGS__)
DECLARE_DEFARG(orwl_read_request, , , seed_get());

PROTOTYPE(orwl_state, orwl_release, orwl_handle*, rand48_t*);
#define orwl_release(...)  CALL_WITH_DEFAULTS(orwl_release, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_release, , seed_get());

PROTOTYPE(orwl_state, orwl_cancel, orwl_handle*, rand48_t*);
#define orwl_cancel(...)  CALL_WITH_DEFAULTS(orwl_cancel, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_cancel, , seed_get());
#endif

inline
orwl_state orwl_acquire(orwl_handle* rh) {
  return orwl_wh_acquire(rh->wh, 0);
}

inline
orwl_state orwl_test(orwl_handle* rh) {
  return orwl_wh_test(rh->wh, 0);
}

inline
void orwl_map(orwl_handle* rh, uint64_t** data, size_t* data_len) {
  if (orwl_test(rh) > orwl_valid) {
    assert(rh->wh);
    orwl_wh_map(rh->wh, data, data_len);
  }
}

inline
void orwl_resize(orwl_handle* rh, size_t data_len) {
  if (orwl_test(rh) > orwl_valid) {
    assert(rh->wh);
    orwl_wh_resize(rh->wh, data_len);
  }
}

DECLARE_ORWL_REGISTER(orwl_wh_acquire);
DECLARE_ORWL_REGISTER(orwl_wh_release);
DECLARE_ORWL_REGISTER(orwl_wh_cancel);


#endif 	    /* !ORWL_REMOTE_QUEUE_H_ */
