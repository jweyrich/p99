/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/* all rights reserved,  2011 Matias E. Vara, INRIA, France                  */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_WAIT_QUEUE_H_
# define   	ORWL_WAIT_QUEUE_H_

#include "orwl_register.h"
#include "orwl_endpoint.h"
#include "orwl_count.h"
#include P99_ADVANCE_ID

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Return type for @c orwl functions
 **/
P99_DECLARE_ENUM(orwl_state,
             orwl_invalid,
             orwl_valid,
             orwl_requested,
             orwl_read_requested,
             orwl_write_requested,
             orwl_acquired,
             orwl_read_acquired,
             orwl_write_acquired
             );

  /** @var orwl_state orwl_invalid
   ** call with an invalid object
   **/
  /** @var orwl_state orwl_valid
   ** object valid, but not requested
   **/
  /** @var orwl_state orwl_requested
   ** unspecific request was placed
   **/
  /** @var orwl_state orwl_read_requested
   ** read request was placed
   **/
  /** @var orwl_state orwl_write_requested
   ** write request was placed
   **/
  /** @var orwl_state orwl_acquired
   ** unspecific request was acquired
   **/
  /** @var orwl_state orwl_read_acquired
   ** read request was acquired
   **/
  /** @var orwl_state orwl_write_acquired
   ** write request was acquired
   **/


#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_state*, orwl_state_init, orwl_state *, orwl_state);
#define orwl_state_init(...) P99_CALL_DEFARG(orwl_state_init, 2, __VA_ARGS__)
#define orwl_state_init_defarg_1() orwl_invalid
#endif

DOCUMENT_INIT(orwl_state)
P99_DEFARG_DOCU(orwl_state_init)
inline
orwl_state* orwl_state_init
(orwl_state *el,                    /*!< wait queue to initialize */
 orwl_state val                     /*!< defaults to orwl_invalid */
 ) {
  *el = val;
  return el;
}

DOCUMENT_DESTROY(orwl_state)
inline
void orwl_state_destroy(orwl_state *el) {
  /* special care for bogus warning given by icc */
  (void)el;
}

DECLARE_NEW_DELETE(orwl_state);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_state);

DECLARE_ONCE(orwl_wq);

DECLARE_ONCE(orwl_wh);

P99_POINTER_TYPE(orwl_wh);
DECLARE_ATOMIC_OPS(orwl_wh_ptr);

/**
 ** @brief A proactive locking object with FIFO policy.
 **
 ** Locks on such an object can only be achieved through a handle of
 ** type orwl_wh. Such a lock is not bound to the process or
 ** thread that issued the lock request, but to the handle object.
 **
 ** In essence, orwl_wq is just a pthread_mutex_t with some
 ** additional sugar around it.
 **/
struct orwl_wq {

  /** @privatesection
   ** @{
   **/

  pthread_mutex_t mut;  /**< The mutex used to control the access to the queue **/
  pthread_cond_t  cond;
  orwl_wh *head;        /**< The head of the priority queue */
  orwl_wh *tail;        /**< The tail of the priority queue */
  uint64_t clock;       /**< A counter that is increased at each
                           event that this queue encounters. */
  orwl_buffer data;     /**< The data that is associated with this
                           queue */
  bool borrowed:1;      /**< True if the data is not to be released
                           from here. */

  /**
   ** @}
   **/

};


/**
 ** @brief The handle type corresponding to ::orwl_wq.
 **
 ** ::orwl_wh is mainly a @c pthread_cond_t that is bound to the
 ** fixed condition to be the first in the FIFO.
 **
 ** Locks through such a handle object are achieved in a two-step
 ** procedure. First, a lock is @em requested through
 ** ::orwl_wq_request. This binds the ::orwl_wh to a
 ** designated ::orwl_wq and appends the request to the FIFO
 ** queue of it. Then in a second step, ::orwl_wh_acquire waits
 ** until the request has become the first in the queue.
 **
 ** An ::orwl_wh is loaded with #tokens. Each ::orwl_wq_request places
 ** tokes on the corresponding handle and ::orwl_wh_acquire removes
 ** these tokens. It is up to the code that uses a handle to watch
 ** that the number of placed and removed tokens match.
 **
 ** Depending on the field #svrID a handle can be in an inclusive or
 ** exclusive state. In inclusive state and when it is still at the
 ** tail of the corresponding queue subsequent calls ::orwl_wq_request
 ** may load additional tokens on the handle. Such a ::orwl_wq_request
 ** will always be refused for a handle that is in exclusive state.
 **
 ** ::orwl_wh_release releases the lock by popping the request from
 ** the FIFO. Thus, it allows for the following request in the FIFO
 ** (if any) to be acquired. A ::orwl_wh_release may only be performed
 ** if all tokens have been removed, i.e if all lock holders have had
 ** notice that the lock is acquired. If #tokens is not 0 whence
 ** ::orwl_wh_release is called, the call blocks until the condition
 ** is fulfilled.
 **
 **/
struct orwl_wh {

  /** @privatesection
   ** @{
   **/

  /** The location to which this wh links. */
  orwl_wq *location;
  /** The next wh in the priority queue. */
  orwl_wh *next;
  /** @brief The number of tokens that have been loaded on this wh.
   **
   ** Generally this will reflect the number of threads that may wait
   ** for this handle to be acquired. A wh can only be released when
   ** this has dropped to zero.
   **/
  orwl_count tokens;
  /** @brief This is set iff the orwl_wh is acquired.
   **
   ** This allows to check for this condition atomically. Acquire can
   ** then be guaranteed by just blocking on this field. 
   **/
  orwl_notifier acq;
  /**
   ** @brief The historical position in the wait queue.
   **/
  uint64_t priority;
  /**
   ** @brief An ID of a local or remote orwl_wh in case that this
   ** orwl_wh represents an inclusive lock.
   **/
  uint64_t svrID;

  /**
   ** @}
   **/

};

  /**
   ** @memberof orwl_wq
   **/
#define ORWL_WQ_INITIALIZER { .mut = PTHREAD_MUTEX_INITIALIZER, .cond = PTHREAD_COND_INITIALIZER, .clock = 1, .borrowed = false }

  DOCUMENT_INIT(orwl_wq)
P99_DEFARG_DOCU(orwl_wq_init)
orwl_wq* orwl_wq_init
(orwl_wq *wq,                    /*!< wait queue to initialize */
 const pthread_mutexattr_t *attr /*!< defaults to a null pointer */
);

#ifndef DOXYGEN
  P99_PROTOTYPE(orwl_wq*, orwl_wq_init, orwl_wq*, const pthread_mutexattr_t*);
#define orwl_wq_init(...) P99_CALL_DEFARG(orwl_wq_init, 2, __VA_ARGS__)
#define orwl_wq_init_defarg_1() P99_0(const pthread_mutexattr_t*)
#endif

  DOCUMENT_DESTROY(orwl_wq)
void orwl_wq_destroy(orwl_wq *wq);

DECLARE_NEW_DELETE(orwl_wq);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_wq);

  /**
   ** @brief Test @a wh for validity.
   **
   ** This is just a simple test that @a wh has not just been
   ** destroyed.
   **
   ** @memberof orwl_wh
   **/
inline
int orwl_wh_valid(orwl_wh *wh) {
  orwl_wh *wh_next = atomic_load_orwl_wh_ptr(&wh->next);
  return wh
    && wh->location != TGARB(orwl_wq*)
    && wh_next != TGARB(orwl_wh*);
}

  /**
   ** @brief Test @a wh for idleness.
   **
   ** This is just a simple test that @a wh doesn't link to any
   ** location.
   **
   ** @memberof orwl_wh
   **/
inline
int orwl_wh_idle(orwl_wh *wh) {
  orwl_wh *wh_next = atomic_load_orwl_wh_ptr(&wh->next);
  return wh && !wh->location && !wh_next;
}

  /**
   ** @brief Test @a wq for validity.
   **
   ** This is just a simple test that @a wq has not just been
   ** destroyed.
   **
   ** This supposes that @a wq is not a null pointer.
   **
   ** @memberof orwl_wq
   **/
inline
int orwl_wq_valid(orwl_wq *wq) {
  orwl_wh *wq_head = atomic_load_orwl_wh_ptr(&wq->head);
  orwl_wh *wq_tail = atomic_load_orwl_wh_ptr(&wq->tail);
  return  wq_head != TGARB(orwl_wh*)
    && wq_tail != TGARB(orwl_wh*);
}

  /**
   ** @brief Test @a wq for idleness.
   **
   ** Idleness here means that there is no ::orwl_wh in the FIFO queue
   ** of this location.
   **
   ** This supposes that @a wq is not a null pointer.
   **
   ** @memberof orwl_wq
   **/
inline
int orwl_wq_idle(orwl_wq *wq) {
  orwl_wh *wq_head = atomic_load_orwl_wh_ptr(&wq->head);
  orwl_wh *wq_tail = atomic_load_orwl_wh_ptr(&wq->tail);
  return !wq_head && !wq_tail;
}


  /**
   ** @memberof orwl_wh
   **/
#define ORWL_WH_INITIALIZER {                   \
  .tokens = ORWL_COUNT_INITIALIZER(0),          \
  .acq = ORWL_NOTIFIER_INITIALIZER              \
  }

  DOCUMENT_INIT(orwl_wh)
  P99_DEFARG_DOCU(orwl_wh_init)
orwl_wh* orwl_wh_init
  (orwl_wh *wh, /*!< the handle to be initialized */
   const pthread_condattr_t *attr /*!< [in] defaults to a null pointer */
   );

#ifndef DOXYGEN
  P99_PROTOTYPE(orwl_wh*, orwl_wh_init, orwl_wh *, const pthread_condattr_t *);
#define orwl_wh_init(...) P99_CALL_DEFARG(orwl_wh_init, 2, __VA_ARGS__)
#define orwl_wh_init_defarg_1() P99_0(const pthread_condattr_t *)
#endif

  DOCUMENT_DESTROY(orwl_wh)
void orwl_wh_destroy(orwl_wh *wh);

DECLARE_NEW_DELETE(orwl_wh);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_wh);

/**
 ** @brief Insert a list of request pair of <code>(wh, howmuch)</code> into location
 ** @a wq.
 **
 ** You call it by something like
 ** @code
 ** orwl_state state = orwl_wq_request(wq, wh0, howmuch0)
 ** @endcode
 **
 ** @return ::orwl_invalid if any of @c wh or @a wq was
 ** invalid. Otherwise returns ::orwl_requested.
 **
 ** For a @c wh argument that is not a null pointer, this inserts the
 ** handle into the queue and places @c howmuch tokens on @c *wh. @c
 ** wh will only be possible to be released if, first, it is acquired
 ** (that is it moves front in the FIFO) and then if all tokens are
 ** unloaded with ::orwl_wh_acquire or ::orwl_wh_test.
 **
 ** A @c wh that is a null pointer is considered to relate to the last
 ** such handle that is at the tail of the queue, if such a handle
 ** exists. Such a NULL-request will check if this trailing handle is
 ** in inclusive state and if so will place the tokens on that handle
 ** and return the address of that handle in @c wh. If the trailing
 ** handle doesn't exist or if it is in exclusive state the call fails
 ** and returns ::orwl_invalid.
 **
 ** The tokens are only considered to be loaded on @a wh if the call is
 ** successful.
 **
 ** @memberof orwl_wq
 **/
  orwl_state orwl_wq_request(orwl_wq *wq, /*!< the queue to act on */
                             orwl_wh **wh,
                             uint64_t hm
                             );

  orwl_state orwl_wq_request2(orwl_wq *wq, /*!< the queue to act on */
                              orwl_wh *wh0,
                              uint64_t hm0,
                              orwl_wh *wh1,
                              uint64_t hm1
                             );
/**
 ** @private
 ** @memberof orwl_wq
 **/
orwl_state orwl_wq_request_locked(orwl_wq *wq, /*!< the queue to act on */
                                  orwl_wh **wh,
                                  uint64_t hm
                                  );

  /**
   ** @brief Insert a handle into the queue where we know that the
   ** lock is already held.
   **
   ** @memberof orwl_wq
   ** @private
   **/
void orwl_wq_request_append(orwl_wq *wq,  /*!< the locked queue to act on */
                            orwl_wh *wh,  /*!< the handle to be inserted */
                            uint64_t howmuch /*!< the number of tokies to place */
                            );

  /**
   ** @brief Link this queue to a different data buffer
   **
   ** @memberof orwl_wq
   ** @private
   **/
void orwl_wq_link(orwl_wq *wq,       /*!< the locked queue to act on */
                  orwl_buffer data,    /*!< data buffer that is provided
                                      from elsewhere */
                  bool borrowed      /*!< whether this location here
                                       is responsible for the data */
                  );

/**
 ** @brief Acquire a pending request on @a wh. Blocking until the
 ** request is acquired.
 **
 ** @return @c orwl_invalid if @a wh was invalid, or if there
 ** was no request pending on @a wh. Otherwise it eventually blocks and
 ** then returns @c orwl_acquired.
 **
 ** The tokens are considered to be removed frome @a wh iff the call
 ** returns orwl_acquired.
 **
 ** @memberof orwl_wh
 **/
  P99_DEFARG_DOCU(orwl_wh_acquire)
orwl_state orwl_wh_acquire
  (orwl_wh *wh,       /*!< the handle to act upon */
   uint64_t howmuch   /*!< defaults to @c 1 */
   );

#ifndef DOXYGEN
  P99_PROTOTYPE(orwl_state, orwl_wh_acquire, orwl_wh*, uint64_t);
#define orwl_wh_acquire(...) P99_CALL_DEFARG(orwl_wh_acquire, 2, __VA_ARGS__)
#define orwl_wh_acquire_defarg_1() 1
#endif

/**
 ** Of internal use. Supposes that @a wh is in the queue of @a wq and
 ** that the lock on @a wq is taken by this thread.
 **
   ** @memberof orwl_wh
   ** @private
 **/
  orwl_state orwl_wh_acquire_locked(orwl_wh *wh, /*!< the handle to act upon */
                                    orwl_wq *wq  /*!< the locked queue in which @a wh
                                                    is already inserted */
                                    );


/**
 ** @brief Test for a pending request on @a wh. Never blocking.
 **
 ** @return @c orwl_invalid if @a wh was invalid, @c orwl_valid if there
 ** was no request pending on @a wh, @c orwl_requested if a request is
 ** pending that is not yet acquired, and @c orwl_acquired if a request
 ** is already acquired.
 **
 ** The tokens are considered to be removed frome @a wh iff the call
 ** returns orwl_acquired.
 **
 ** @memberof orwl_wh
 **/
  P99_DEFARG_DOCU(orwl_wh_test)
orwl_state orwl_wh_test
  (orwl_wh *wh /*!< the handle to act upon */,
   uint64_t howmuch  /*!< defaults to 0 */);

#ifndef DOXYGEN
  P99_PROTOTYPE(orwl_state, orwl_wh_test, orwl_wh*, uint64_t);
#define orwl_wh_test(...) P99_CALL_DEFARG(orwl_wh_test, 2, __VA_ARGS__)
#define orwl_wh_test_defarg_1() 0
#endif

/**
 ** @brief Release a request on @a wh. If @a wh had been acquired this
 ** is blocking until all tokens are unloaded.
 **
 ** @return @c orwl_invalid if @a wh was invalid, or if there was no
 ** request acquired for @a wh. Otherwise it returns @c orwl_valid.
 **
 ** @memberof orwl_wh
 **/
orwl_state orwl_wh_release(orwl_wh *wh /*!< the handle to act upon */);

#ifndef DOXYGEN
inline
P99_PROTOTYPE(uint64_t, orwl_wh_load, orwl_wh *, uint64_t);
#define orwl_wh_load(...) P99_CALL_DEFARG(orwl_wh_load, 2, __VA_ARGS__)
#define orwl_wh_load_defarg_1() 1
#endif

  /**
   ** @brief load @a howmuch additional tokens on @a wh.
   **
   ** This supposes that the corresponding @c wq is not a null pointer and that @c
   ** wh is already locked.
   ** @see orwl_wh_unload
   **
   ** @memberof orwl_wh
   ** @private
   **/
  P99_DEFARG_DOCU(orwl_wh_load)
inline
uint64_t orwl_wh_load
  (orwl_wh *wh /*!< the handle to act upon */,
   uint64_t howmuch  /*!< defaults to 1 */) {
    return orwl_count_inc(&wh->tokens, howmuch);
  }

#ifndef DOXYGEN
inline
P99_PROTOTYPE(uint64_t, orwl_wh_unload, orwl_wh *, uint64_t);
#define orwl_wh_unload(...) P99_CALL_DEFARG(orwl_wh_unload, 2, __VA_ARGS__)
#define orwl_wh_unload_defarg_1() UINT64_C(1)
#endif

  /**
   ** @brief unload @a howmuch additional tokens from @a wh.
   **
   ** This supposes that the corresponding @c wq is not a null pointer and that @c
   ** wh is already locked. If by this action the token count drops to
   ** zero, eventual waiters for this @a wh are notified.
   ** @see orwl_wh_load
   **
   ** @memberof orwl_wh
   ** @private
   */
  P99_DEFARG_DOCU(orwl_wh_unload)
inline
uint64_t orwl_wh_unload
  (orwl_wh *wh /*!< the handle to act upon */,
   uint64_t howmuch  /*!< defaults to 1 */) {
    return orwl_count_dec(&wh->tokens, howmuch);
  }

  /**
   ** @memberof orwl_wq
   ** @private
   */
uint64_t* orwl_wq_map_locked(orwl_wq* wq, size_t* data_len);

  /**
   ** @memberof orwl_wq
   ** @private
   */
void orwl_wq_resize_locked(orwl_wq* wq, size_t len);

/**
 ** @brief Obtain address and size of the data that is associated to a
 ** location for reading and writing
 ** @memberof orwl_wh
 ** The application may associate data to each location of which it
 ** also may control the size. Once the lock is acquired for a given
 ** handle, this data is available through ::orwl_wh_map, returning a
 ** pointer to the data and to its size. The pointer to the data will
 ** be invalid, as soon as the lock is again released.
 **
 ** @pre The handle @a wh must hold a lock on the location to
 ** which it is linked.
 **
 ** @param wh the handle in question
 ** @param data_len [out] the length of the data in number of elements
 **
 ** @return An address to access the data that is associated with the
 ** location. If the request is invalid @c 0 is returned and if
 ** applicable @c *data_len is set to @c 0, too.
 ** @warning The return address is only valid until @a wh is
 ** released.
 ** @warning The return address may (and will) be different between
 ** different calls to that function.
 ** @warning The new content of the data will only be visible for
 ** other lock handles once they obtain a lock after this handle
 ** releases its lock.
 **
 ** @todo Keep track if we have mapped this data for writing via a
 ** "dirty" flag.
 **/
uint64_t* orwl_wh_map(orwl_wh* wh, size_t* data_len);
void orwl_wh_resize(orwl_wh* wh, size_t data_len);

DECLARE_ORWL_REGISTER(orwl_wh_acquire);
DECLARE_ORWL_REGISTER(orwl_wh_release);
DECLARE_ORWL_REGISTER(orwl_wh_load);
DECLARE_ORWL_REGISTER(orwl_wh_unload);

DECLARE_ORWL_REGISTER_ALIAS(orwl_wh_acquire, orwl_wh);
DECLARE_ORWL_REGISTER_ALIAS(orwl_wh_release, orwl_wh);
DECLARE_ORWL_REGISTER_ALIAS(orwl_wh_load, orwl_wh);
DECLARE_ORWL_REGISTER_ALIAS(orwl_wh_unload, orwl_wh);

#ifdef __cplusplus
}
#endif


#endif
