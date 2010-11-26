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
#ifndef   	ORWL_REMOTE_QUEUE_H_
# define   	ORWL_REMOTE_QUEUE_H_

#include "orwl_wait_queue.h"
#include P99_ADVANCE_ID

/**
 ** @brief A structure to regulate queues between different servers.
 **
 ** An orwl_mirror is a local data structure that represents an
 ** abstract @em %location in a ORWL (ordered read-write lock)
 ** system. It is initialized with two ::orwl_endpoint
 ** specifications. One corresponds to the address of the location
 ** that is to be mirrored, it may be local in the same process or
 ** remote on another host. The other ::orwl_endpoint corresponds to
 ** the address of the current process and will be used by others to
 ** connect to this host here to reply to requests.
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
P99_PROTOTYPE(orwl_mirror *, orwl_mirror_init, orwl_mirror *, orwl_endpoint, orwl_endpoint);

#define orwl_mirror_init(...) P99_CALL_DEFARG(orwl_mirror_init, 3, __VA_ARGS__)
#endif

DOCUMENT_INIT(orwl_mirror)
P99_DEFARG_DOCU(orwl_mirror_init)
inline
orwl_mirror *orwl_mirror_init(orwl_mirror *rq, /*!< [out] the object to iniialize */
                              orwl_endpoint h, /*!< [in] local, defaults to a temp variable */
                              orwl_endpoint t  /*!< [in] remote, defaults to a temp variable */
                              ) {
  if (!rq) return 0;
  pthread_mutex_init(&rq->mut, P99_0(pthread_mutexattr_t*));
  orwl_wq_init(&rq->local);
  rq->here = h;
  rq->there = t;
  return rq;
}

P99_DECLARE_DEFARG(orwl_mirror_init, , , );
#define orwl_mirror_init_defarg_1() ((orwl_endpoint){ .index = 0 })
#define orwl_mirror_init_defarg_2() ((orwl_endpoint){ .index = 0 })

DOCUMENT_DESTROY(orwl_mirror)
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
 ** In an ORWL (ordered read-write lock) system an ::orwl_handle is
 ** used to control locks that an application process issues on a
 ** local or remote lock location. As the rw in ORWL indicates, locks
 ** can be shared-read or exclusive-write. With ORWL a typical locking
 ** sequence is to
 ** -# announce a future locking by issuing a @em request, via
 **   ::orwl_read_request or ::orwl_write_request
 ** -# do some other work
 ** -# ensure that the lock is obtained by a blocking call to ::orwl_acquire
 ** -# do the work that is critical and that now is protected by the lock
 ** -# release the lock by means of ::orwl_release.
 **
 ** Obtaining a read (=shared) lock on a remote location means that
 ** other handles may have read locks simultaneously on the same
 ** location, but that a write lock is attributed to at most one
 ** handle at a time.
 **
 ** ORWL imposes a strict first come first serve (FIFO) policy for
 ** obtaining the locks:
 **
 ** - The earlier a @em request arrives at a location the earlier the
 **   corresponding acquire will succeed.
 ** - An acquire will block as long as there is an earlier
 **   incompatible lock on the same location.
 **
 ** All calls to the ORWL functions (but ::orwl_acquire) in such a
 ** sequence should be immediate and consume only few resources: the
 ** idea that most of the lock administration is done asynchronously
 ** behind the scenes and that the application only has to wait (in
 ** ::orwl_acquire) if this is unavoidable since the lock is hold by
 ** another handle.
 **
 ** Deviations from the above sequence are possible. ::orwl_test can
 ** be used to know if the lock can be obtained without
 ** blocking. ::orwl_cancel can be used to cancel a request that had
 ** been placed previously.
 **
 ** The application may associate data to each location of which it
 ** also may control the size, ::orwl_map and ::orwl_resize.
 **
 ** @see orwl_mirror
 ** @see orwl_read_request
 ** @see orwl_write_request
 ** @see orwl_acquire
 ** @see orwl_test
 ** @see orwl_release
 ** @see orwl_cancel
 ** @see orwl_map
 ** @see orwl_resize
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

#define ORWL_HANDLE_INITIALIZER { .rq = 0, .wh = 0, .svrID = P99_0(uint64_t) }

DOCUMENT_INIT(orwl_handle)
inline
orwl_handle *orwl_handle_init(orwl_handle *rh) {
  if (!rh) return 0;
  *rh = (orwl_handle const)ORWL_HANDLE_INITIALIZER;
  return rh;
}

DOCUMENT_DESTROY(orwl_handle)
inline
void orwl_handle_destroy(orwl_handle *rh) {
  orwl_handle_init(rh);
}

DECLARE_NEW_DELETE(orwl_handle);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle);


#define P00_DOCUMENT_SEED                                                                                                                                                                                                  \
/*! @remark Because this function might open a socket it needs an additional parameter @a seed for a random generator. This argument is provided by default, so usually you don't have to worry and may simply omit it. */

/**
 ** @brief Insert a write request in the FIFO at the location of @a rq
 ** @memberof orwl_mirror
 **
 ** Once such a write request will be achieved
 ** this will be the only lock (read or write) that can be hold by any
 ** handle simultaneously.
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_write_request)
orwl_state orwl_write_request(orwl_mirror* rq, /*!< [in,out] the location for the request */
                              orwl_handle* rh,   /*!< [in,out] the handle for the request */
                              rand48_t* seed         /*!< [in] defaults to a thread local seed */
                              );

/**
 ** @brief Insert a write request in the FIFO at the location of @a rq
 ** @memberof orwl_mirror
 **
 ** Once such a read request will be achieved other @em read request
 ** can be granted to other handles simultaneously.
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_read_request)
orwl_state orwl_read_request(orwl_mirror* rq, /*!< [in,out] the location for the request */
                             orwl_handle* rh,   /*!< [in,out] the handle for the request */
                             rand48_t* seed         /*!< [in] defaults to a thread local seed */
                             );

/**
 ** Release the lock that @a rh has obtained on its location
 ** @memberof orwl_handle
 **
 ** This also invalidates any data address that might have been
 ** obtained through a call to ::orwl_map.
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_release)
orwl_state orwl_release(orwl_handle* rh,   /*!< [in,out] the handle to be released */
                        rand48_t* seed         /*!< [in] defaults to a thread local seed */
                        );

/**
 ** Release the lock that @a rh has requested on its location
 ** @memberof orwl_handle
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_cancel)
orwl_state orwl_cancel(orwl_handle* rh,   /*!< [in,out] the handle to be canceled */
                       rand48_t* seed         /*!< [in] defaults to a thread local seed */
                       );

#ifndef DOXYGEN
P99_PROTOTYPE(orwl_state, orwl_write_request, orwl_mirror*, orwl_handle*, rand48_t*);
#define orwl_write_request(...)  P99_CALL_DEFARG(orwl_write_request, 3, __VA_ARGS__)
#define orwl_write_request_defarg_2() seed_get()

P99_PROTOTYPE(orwl_state, orwl_read_request, orwl_mirror*, orwl_handle*, rand48_t*);
#define orwl_read_request(...)  P99_CALL_DEFARG(orwl_read_request, 3, __VA_ARGS__)
#define orwl_read_request_defarg_2() seed_get()

P99_PROTOTYPE(orwl_state, orwl_release, orwl_handle*, rand48_t*);
#define orwl_release(...)  P99_CALL_DEFARG(orwl_release, 2, __VA_ARGS__)
#define orwl_release_defarg_1() seed_get()

P99_PROTOTYPE(orwl_state, orwl_cancel, orwl_handle*, rand48_t*);
#define orwl_cancel(...)  P99_CALL_DEFARG(orwl_cancel, 2, __VA_ARGS__)
#define orwl_cancel_defarg_1() seed_get()
#endif

/**
 ** @brief Check if handle @a rh has been requested for an inclusive lock.
 **
 ** @a rh may be in a state of being requested or having already
 ** acquired.
 ** @memberof orwl_handle
 **/
inline
bool orwl_inclusive(orwl_handle* rh) {
  return (rh && rh->wh && rh->wh->svrID);
}

/**
 ** @brief Block until a previously issued read or write request can
 ** be fulfilled
 ** @memberof orwl_handle
 **/
inline
orwl_state orwl_acquire(orwl_handle* rh) {
  if (!rh) return orwl_invalid;
  return orwl_wh_acquire(rh->wh, 0);
}

/**
 ** @brief Test if a previously issued read or write request can
 ** be fulfilled
 ** @memberof orwl_handle
 **/
inline
orwl_state orwl_test(orwl_handle* rh) {
  if (!rh) return orwl_invalid;
  return orwl_wh_test(rh->wh, 0);
}

/**
 ** @brief Obtain address and size of the data that is associated to a
 ** handle for reading and writing
 ** @memberof orwl_handle
 ** The application may associate data to each location of which it
 ** also may control the size. Once the lock is acquired for a given
 ** handle, this data is available through ::orwl_map, returning a
 ** pointer to the data and to its size. The pointer to the data will
 ** be invalid, as soon as the lock is again released.
 **
 ** @pre The handle @a rh must hold a write lock on the location to
 ** which it is linked.
 **
 ** @warning The new content of the data will only be visible for
 ** other lock handles once they obtain a lock after this handle
 ** releases its write lock.
 **
 ** @see orwl_mapro for the case that the lock that is hold is a read
 ** lock and / or the data should only be read.
 **
 **/
inline
uint64_t* orwl_map(orwl_handle* rh, size_t* data_len) {
  uint64_t* ret = 0;
  if (rh)
    switch (orwl_test(rh)) {
    case orwl_acquired: ;
      if (orwl_inclusive(rh)) break;
    case orwl_write_acquired: ;
      assert(rh->wh);
      ret = orwl_wh_map(rh->wh, data_len);
    default:;
    }
  return ret;
}

/**
 ** @brief Obtain address and size of the data that is associated to a
 ** handle for reading
 ** @memberof orwl_handle
 ** @pre The handle @a rh must hold a read lock on the location to
 ** which it is linked.
 **
 ** @see orwl_map for the case that the lock that is hold is a write
 ** lock the data and should also be written to.
 **/
inline
uint64_t const* orwl_mapro(orwl_handle* rh, size_t* data_len) {
  uint64_t* ret = 0;
  if (rh)
    switch (orwl_test(rh)) {
    case orwl_acquired: ;
    case orwl_write_acquired: ;
    case orwl_read_acquired: ;
      assert(rh->wh);
      ret = orwl_wh_map(rh->wh, data_len);
    default:;
    }
  return ret;
}

/**
 ** @brief Shrink or extend the data that is associated to a location.
 ** @memberof orwl_handle
 ** Initially, the data of a location is empty, i.e of 0 size. If the
 ** lock that a handle holds is exclusive ::orwl_resize can be used to
 ** resize the data to a new length. If such a resize operation
 ** is an extension of existing data that data is preserved and the
 ** newly appended area is filled with zero bytes.
 **
 ** @pre The handle @a rh must hold a write (exclusive) lock on the
 ** location to which it is linked. 
 **/
inline
void orwl_resize(orwl_handle* rh, size_t data_len) {
  if (orwl_test(rh) > orwl_valid) {
    assert(rh->wh);
    orwl_wh_resize(rh->wh, data_len);
  }
}

DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle);

DECLARE_ORWL_REGISTER(orwl_wh_acquire);
DECLARE_ORWL_REGISTER(orwl_wh_release);
DECLARE_ORWL_REGISTER(orwl_wh_cancel);

DECLARE_ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle);
DECLARE_ORWL_REGISTER_ALIAS(orwl_release, orwl_handle);
DECLARE_ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle);

#endif 	    /* !ORWL_REMOTE_QUEUE_H_ */
