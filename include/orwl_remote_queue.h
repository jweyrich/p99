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
#ifndef     ORWL_REMOTE_QUEUE_H_
# define    ORWL_REMOTE_QUEUE_H_

#include "orwl_wait_queue.h"
#include "orwl_proc_symbols.h"
#include P99_ADVANCE_ID
#include "orwl_timing.h"

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
 **   orwl_proc<-orwl_server [label="orwl_proc_request()"];
 **   orwl_proc;
 **   orwl_proc->orwl_server [label="Ack"];
 **   orwl_server->orwl_mirror [label="orwl_rpc()"];
 **   orwl_mirror->orwl_handle [label="Ack "];
 **   cli_wh<-orwl_handle [label="orwl_acquire"];
 **   orwl_proc->cli_wh [label="acquired"];
 **   cli_wh->orwl_handle [label="orwl_acquired"];
 **   cli_wh<-orwl_handle [label="orwl_release"];
 **   cli_wh<-orwl_handle [label="orwl_release"];
 ** @endmsc
 ** @see orwl_handle
 **/
struct orwl_mirror {

  /** @privatesection
   ** @{
   **/

  pthread_mutex_t mut;  /**< control access during insertion */
  orwl_server* srv;   /**< the local endpoint to which we report */
  orwl_endpoint there;  /**< the remote that centralizes the order */
  orwl_wq local;        /**< the local queue that interfaces the
                           remote */

  /**
   ** @}
   **/

};

DECLARE_ONCE(orwl_mirror);

/**
 ** @memberof orwl_mirror
 **/
#define ORWL_MIRROR_INITIALIZER(SRV, THERE) {                  \
.mut = PTHREAD_MUTEX_INITIALIZER,                              \
  .local = ORWL_WQ_INITIALIZER,                                \
  .srv = SRV,                                                  \
  .there = THERE,                                              \
  }

#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_mirror *, orwl_mirror_init, orwl_mirror *, orwl_server*, orwl_endpoint);

#define orwl_mirror_init(...) P99_CALL_DEFARG(orwl_mirror_init, 3, __VA_ARGS__)
#endif

DOCUMENT_INIT(orwl_mirror)
P99_DEFARG_DOCU(orwl_mirror_init)
inline
orwl_mirror *orwl_mirror_init(orwl_mirror *rq, /*!< [out] the object to iniialize */
                              orwl_server *srv, /*!< [in] local, defaults to a temp variable */
                              orwl_endpoint t  /*!< [in] remote, defaults to a temp variable */
                             ) {
  if (rq)
    *rq = (orwl_mirror const)ORWL_MIRROR_INITIALIZER(srv, t);
  return rq;
}

P99_DECLARE_DEFARG(orwl_mirror_init, , , );
#define orwl_mirror_init_defarg_1() P99_0(orwl_server*)
#define orwl_mirror_init_defarg_2() P99_LVAL(orwl_endpoint, .index = 0 )

DOCUMENT_DESTROY(orwl_mirror)
inline
void orwl_mirror_destroy(orwl_mirror *rq) {
  orwl_wq_destroy(&rq->local);
  orwl_endpoint_destroy(&rq->there);
}

DECLARE_NEW_DELETE(orwl_mirror);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_mirror);


#ifndef DOXYGEN
P99_PROTOTYPE(void, orwl_mirror_connect, orwl_mirror *, orwl_endpoint, int64_t, orwl_server*);
#define orwl_mirror_connect(...) P99_CALL_DEFARG(orwl_mirror_connect, 4, __VA_ARGS__)
#define orwl_mirror_connect_defarg_2() -INT64_C(1)
#define orwl_mirror_connect_defarg_3() orwl_server_get()
#endif

/**
 ** @brief Initialize and connect a ::orwl_mirror @a rq with local
 ** server @a srv to the endpoint @a endp
 ** @memberof orwl_mirror
 **/
void orwl_mirror_connect(orwl_mirror *rq, orwl_endpoint endp, int64_t index, orwl_server* srv);


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
  /** @privatesection
   ** @{
   **/

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

  /**
   ** @}
   **/

};

DECLARE_ONCE(orwl_handle);

/**
 ** @memberof orwl_handle
 **/
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


#define O_RWL_DOCUMENT_SEED                                                                                                                                                                                                \
/*! @remark Because this function might open a socket it needs an additional parameter @a seed for a random generator. This argument is provided by default, so usually you don't have to worry and may simply omit it. */

/**
 ** @brief Insert a write request in the FIFO at the location of @a rq
 ** @memberof orwl_mirror
 **
 ** Once such a write request will be achieved
 ** this will be the only lock (read or write) that can be hold by any
 ** handle simultaneously.
 **/
O_RWL_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_write_request)
orwl_state orwl_write_request(orwl_mirror* rq, /*!< [in,out] the location for the request */
                              orwl_handle* rh, /*!< [in,out] the handle for the request */
                              size_t size,     /*!< [in] vector size, defaults to 1 */
                              rand48_t* seed   /*!< [in] defaults to a thread local seed */
                             );

/**
 ** @brief Insert a write request in the FIFO at the location of @a rq
 ** @memberof orwl_mirror
 **
 ** Once such a read request will be achieved other @em read request
 ** can be granted to other handles simultaneously.
 **/
O_RWL_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_read_request)
orwl_state orwl_read_request(orwl_mirror* rq, /*!< [in,out] the location for the request */
                             orwl_handle* rh, /*!< [in,out] the handle for the request */
                             size_t size,     /*!< [in] vector size, defaults to 1 */
                             rand48_t* seed   /*!< [in] defaults to a thread local seed */
                            );

/**
 ** Release the lock that @a rh has obtained on its location
 ** @memberof orwl_handle
 **
 ** This also invalidates any data address that might have been
 ** obtained through a call to ::orwl_map and sends the modified data
 ** back to the local or remote server.
 ** @todo Keep track if the data has been mapped and only send it back
 ** in that case.
 ** @todo Do the send back asynchronously.
 **/
O_RWL_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_release)
orwl_state orwl_release(orwl_handle* rh,   /*!< [in,out] the handle to be released */
                        size_t size,       /*!< [in] the size of the vector */
                        rand48_t* seed     /*!< [in] defaults to a thread local seed */
                       );

/**
 ** Release the lock that @a rh has requested on its location
 ** @memberof orwl_handle
 **/
O_RWL_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_cancel)
orwl_state orwl_cancel(orwl_handle* rh,   /*!< [in,out] the handle to be canceled */
                       rand48_t* seed     /*!< [in] defaults to a thread local seed */
                      );

#ifndef DOXYGEN
P99_PROTOTYPE(orwl_state, orwl_write_request, orwl_mirror*, orwl_handle*, size_t, rand48_t*);
#define orwl_write_request(...)  P99_CALL_DEFARG(orwl_write_request, 4, __VA_ARGS__)
#define orwl_write_request_defarg_2() 1u
#define orwl_write_request_defarg_3() seed_get()

P99_PROTOTYPE(orwl_state, orwl_read_request, orwl_mirror*, orwl_handle*, size_t, rand48_t*);
#define orwl_read_request(...)  P99_CALL_DEFARG(orwl_read_request, 4, __VA_ARGS__)
#define orwl_read_request_defarg_2() 1u
#define orwl_read_request_defarg_3() seed_get()

P99_PROTOTYPE(orwl_state, orwl_release, orwl_handle*, size_t, rand48_t*);
#define orwl_release(...)  P99_CALL_DEFARG(orwl_release, 3, __VA_ARGS__)
#define orwl_release_defarg_1() 1u
#define orwl_release_defarg_2() seed_get()

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
 ** @brief Block until a set of previously issued read or write request can
 ** be fulfilled
 ** @memberof orwl_handle
 ** @todo By means of a version counter avoid to copy data over the
 ** wire that we know already.
 **/
inline
orwl_state orwl_acquire(orwl_handle* rh, size_t size) {
  orwl_state ret =  orwl_state_amount;
  for (size_t i = 0; i < size; ++i) {
      if (rh) {
        ORWL_TIMING(total_acquire) {
          orwl_state state = orwl_wh_acquire(rh[i].wh, 0);
          if (state < ret) ret = state;
        }
      }
  }
  return ret;
}

inline
P99_PROTOTYPE(orwl_state, orwl_acquire, orwl_handle*, size_t);
#define orwl_acquire(...)  P99_CALL_DEFARG(orwl_acquire, 2, __VA_ARGS__)
#define orwl_acquire_defarg_1() 1u


#ifdef DOXYGEN
/**
 ** @brief Protect the following block or statement with
 ** ::owrl_handle @a HANDLE.
 **
 ** This does some rudimentary error checking for the result of the
 ** locking. If an error occurs the whole block and any other
 ** enclosing blocks that protected with P99_UNWIND_PROTECT are
 ** aborted.
 **
 ** @param HANDLE a pointer to a vector of ::orwl_handle, should not
 ** be @c 0.
 ** @param SIZE, optional, the length of the vector, default
 ** if omitted is @c 1.
 **/
P99_BLOCK_DOCUMENT
#define ORWL_SECTION(HANDLE, SIZE)
#else
#define ORWL_SECTION(...)                       \
 P99_IF_LT(P99_NARG(__VA_ARGS__),2)             \
  (O_RWL_SECTION(__VA_ARGS__, 1U))              \
  (O_RWL_SECTION(__VA_ARGS__))
#define O_RWL_SECTION(HANDLE, SIZE)                                     \
P00_BLK_START                                                           \
P00_BLK_DECL(orwl_state, P99_FILEID(state), orwl_invalid)               \
  P00_BLK_DECL(size_t const, P99_FILEID(size), SIZE)                    \
  P99_GUARDED_BLOCK(orwl_handle*,                                       \
                    P99_FILEID(handle),                                 \
                    (HANDLE),                                           \
                    (void)(P99_UNLIKELY                                 \
                           ((P99_FILEID(state) = orwl_acquire(P99_FILEID(handle), P99_FILEID(size))) \
                            != orwl_acquired)                           \
                           && (errno = EINVAL)                          \
                           && (perror(__FILE__ ":" P99_STRINGIFY(__LINE__) ": lock error for " P99_STRINGIFY(HANDLE)), 1) \
                           && (errno = 0)                               \
                           && (P99_FILEID(handle) = 0, 1)               \
                           && (P99_UNWIND(-1), 1)                       \
                           ),                                           \
                    (void)(P99_FILEID(handle)                           \
                           && orwl_release(P99_FILEID(handle), P99_FILEID(size))))
#endif


/**
 ** @brief Test if a set of previously issued read or write request can
 ** be fulfilled
 ** @memberof orwl_handle
 **
 ** Returns the "minimum" value among the return values for all @a
 ** size elements of the vector @a rh, i.e ::orwl_invalid if at least
 ** one of the handles is invalid, ::orwl_requested if at least one is
 ** not yet acquired and ::orwl_acquired if all have been acquired.
 **/
inline
orwl_state orwl_test(orwl_handle* rh, size_t size) {
  orwl_state ret =  orwl_state_amount;
  for (size_t i = 0; i < size; ++i) {
    if (rh) {
      ORWL_TIMING(total_test)
        orwl_state res = orwl_wh_test(&rh->wh[i], 0);
      if (res < ret) return ret = res;
    } else {
      ret = orwl_invalid;
    }
  }
  return ret;
}

inline
P99_PROTOTYPE(orwl_state, orwl_test, orwl_handle*, size_t);
#define orwl_test(...)  P99_CALL_DEFARG(orwl_test, 2, __VA_ARGS__)
#define orwl_test_defarg_1() 1u



enum {
  /**
   ** @brief The number of additional items that are transferred in
   ** the header of a ::orwl_push operation.
   **/
  orwl_push_header
  = orwl_server_callback_header
  + orwl_proc_release_header,
  orwl_push_withdata = 1,
  orwl_push_keep = 2,
};

#define ORWL_PUSH_HEADER 4

P99_COMPILETIME_ASSERT(orwl_push_header == ORWL_PUSH_HEADER);


/**
 ** @brief push the associated data to a remote.
 **
 ** This data is to be handled by orwl_proc_release at the other side.
 ** @memberof orwl_handle
 ** @private
 **/
void orwl_push(orwl_server *srv, orwl_endpoint const*ep,
               orwl_wq *wq, uint64_t whID,
               bool withdata, bool keep);

/**
 ** @brief Obtain address and size of the data that is associated to a
 ** location for reading and writing
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
 ** @param rh the handle in question
 ** @param data_len [out] the length of the data in number of elements
 **
 ** @return An address to access the data that is associated with the
 ** handle. If the request is invalid @c 0 is returned and if
 ** applicable @c *data_len is set to @c 0, too.
 ** @warning The return address is only valid until @a rh is
 ** released.
 ** @warning The return address may (and will) be different between
 ** different calls to that function.
 ** @warning The new content of the data will only be visible for
 ** other lock handles once they obtain a lock after this handle
 ** releases its write lock.
 **
 ** @see orwl_mapro for the case that the lock that is hold is a read
 ** lock and / or the data should only be read.
 **
 ** @todo Rename ::orwl_map to something like @c orwl_write_map64
 ** @todo Keep track if we have mapped this data for writing via a
 ** "dirty" flag.
 ** @todo Make sure that only this interface (and ::orwl_mapro) does the endian
 ** transformation, if necessary.
 **/
inline
uint64_t* orwl_map(orwl_handle* rh, size_t* data_len) {
  uint64_t* ret = 0;
  if (data_len) *data_len = 0;
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
 ** location for reading and writing
 ** @memberof orwl_handle
 ** The application may associate data to each location of which it
 ** also may control the size. Once the lock is acquired for a given
 ** handle, this data is available through ::orwl_map, returning a
 ** pointer to the data and to its size. The pointer to the data will
 ** be invalid, as soon as the lock is again released.
 **
 ** @param rh the handle in question
 ** @param [out] data_len the length of the data in bytes
 **
 ** @pre The handle @a rh must hold a write lock on the location to
 ** which it is linked.
 **
 ** @return An address to access the data that is associated with the
 ** location. If the request is invalid @c 0 is returned and if
 ** applicable @c *data_len is set to @c 0, too.
 ** @warning The return address is only valid until @a rh is
 ** released.
 ** @warning The return address may (and will) be different between
 ** different calls to that function.
 ** @warning The new content of the data will only be visible for
 ** other lock handles once they obtain a lock after this handle
 ** releases its write lock.
 **
 ** @see orwl_mapro for the case that the lock that is hold is a read
 ** lock and / or the data should only be read.
 **
 ** @todo Keep track if we have mapped this data for writing via a
 ** "dirty" flag.
 **
 ** @todo Make sure that this interface (and ::orwl_read_map) don't do
 ** the endian transformation. They are supposed to work on
 ** uninterpreted bytes.
 **/
inline
void* orwl_write_map(orwl_handle* rh, size_t* data_len) {
  uint64_t* ret = 0;
  ORWL_TIMING(total_write_map) {
    if (data_len) *data_len = 0;
    if (rh)
      switch (orwl_test(rh)) {
      case orwl_acquired: ;
        if (orwl_inclusive(rh)) break;
      case orwl_write_acquired: ;
        assert(rh->wh);
        ret = orwl_wh_map(rh->wh, data_len);
        if (ret && data_len)
          *data_len *= sizeof(uint64_t);
      default:;
      }
  }
  return ret;
}


/**
 ** @brief Obtain address and size of the data that is associated to a
 ** location for reading
 ** @memberof orwl_handle
 ** @pre The handle @a rh must hold a read lock on the location to
 ** which it is linked.
 ** @param rh the handle in question
 ** @param [out] data_len the length of the data in number of elements
 ** @return An address to access the data that is associated with the
 ** location. If the request is invalid @c 0 is returned and if
 ** applicable @c *data_len is set to @c 0, too.
 ** @warning The return address is only valid until @a rh is
 ** released.
 ** @warning The return address may (and will) be different between
 ** different calls to that function.
 **
 ** @see orwl_map for the case that the lock that is hold is a write
 ** lock the data and should also be written to.
 **/
inline
uint64_t const* orwl_mapro(orwl_handle* rh, size_t* data_len) {
  uint64_t* ret = 0;
  if (data_len) *data_len = 0;
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
 ** @brief Obtain address and size of the data that is associated to a
 ** location for reading
 ** @memberof orwl_handle
 ** @pre The handle @a rh must hold a read lock on the location to
 ** which it is linked.
 ** @param rh the handle in question
 ** @param [out] data_len the length of the data in bytes
 ** @return An address to access the data that is associated with the
 ** location. If the request is invalid @c 0 is returned and if
 ** applicable @c *data_len is set to @c 0, too.
 ** @warning The return address is only valid until @a rh is
 ** released.
 ** @warning The return address may (and will) be different between
 ** different calls to that function.
 **
 ** @see orwl_write_map for the case that the lock that is hold is a write
 ** lock the data and should also be written to.
 **/
inline
void const* orwl_read_map(orwl_handle* rh, size_t* data_len) {
  uint64_t const* ret = 0;
  ORWL_TIMING(total_read_map) {
    if (data_len) *data_len = 0;
    if (rh)
      switch (orwl_test(rh)) {
      case orwl_acquired: ;
      case orwl_write_acquired: ;
      case orwl_read_acquired: ;
        assert(rh->wh);
        ret = orwl_wh_map(rh->wh, data_len);
        if (ret &&  data_len)
          *data_len *= sizeof(uint64_t);
      default:;
      }
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
 ** @warning @a data_len is accounted in elements of a width of 64 bits.
 **
 ** @pre The handle @a rh must hold a write (exclusive) lock on the
 ** location to which it is linked.
 ** @todo Update a "dirty" flag.
 ** @todo Rename this to something like @c orwl_resize64 to make it
 ** clear that this acts on 64 bit elements.
 ** @see orwl_truncate for a variant that only accounts for bytes
 **/
inline
void orwl_resize(orwl_handle* rh, size_t data_len) {
  if (orwl_test(rh) > orwl_valid) {
    assert(rh->wh);
    orwl_wh_resize(rh->wh, data_len);
  }
}

/**
 ** @brief Shrink or extend the data that is associated to a location.
 ** @memberof orwl_handle
 ** Initially, the data of a location is empty, i.e of 0 size. If the
 ** lock that a handle holds is exclusive ::orwl_truncate can be used to
 ** resize the data to a new length. If such a resize operation
 ** is an extension of existing data that data is preserved and the
 ** newly appended area is filled with zero bytes.
 ** @warning @a data_len is accounted in bytes but the real length
 ** that is associated is the next multiple that can hold elements of
 ** width 64 bit.
 **
 ** @pre The handle @a rh must hold a write (exclusive) lock on the
 ** location to which it is linked.
 ** @todo Update a "dirty" flag.
 ** @see orwl_resize for a variant that accounts for 64 bit elements
 **/
inline
void orwl_truncate(orwl_handle* rh, size_t data_len) {
  ORWL_TIMING(total_truncate) {
    if (orwl_test(rh) > orwl_valid) {
      size_t len = data_len / sizeof(uint64_t);
      len += (data_len % sizeof(uint64_t)) ? 1 : 0;
      assert(rh->wh);
      orwl_wh_resize(rh->wh, len);
    }
  }
}

/** @brief Scale the object corresponding to @a mirr to size @a size
 **
 ** This is a short cut to set the size of an object before doing any
 ** operation on it.
 **
 ** @see orwl_truncate for a more general operation that can be issued
 ** at any time of the execution.
 **/
void orwl_scale(orwl_mirror *mirr, size_t size);


DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle);

DECLARE_ORWL_REGISTER(orwl_wh_acquire);
DECLARE_ORWL_REGISTER(orwl_wh_release);
DECLARE_ORWL_REGISTER(orwl_wh_cancel);

DECLARE_ORWL_REGISTER_ALIAS(orwl_acquire, orwl_handle);
DECLARE_ORWL_REGISTER_ALIAS(orwl_release, orwl_handle);
DECLARE_ORWL_REGISTER_ALIAS(orwl_cancel, orwl_handle);

#endif      /* !ORWL_REMOTE_QUEUE_H_ */
