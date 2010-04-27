/*
** orwl_wait_queue.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri Feb 19 11:44:10 2010 Jens Gustedt
** Last update Fri Feb 19 11:44:10 2010 Jens Gustedt
*/

#ifndef   	ORWL_WAIT_QUEUE_H_
# define   	ORWL_WAIT_QUEUE_H_

#include <assert.h>
#include "orwl_new.h"
#include "orwl_enum.h"
#include "orwl_macro.h"
#include "orwl_register.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Return type for @c orwl functions
 **/
DECLARE_ENUM(orwl_state,
             orwl_invalid,         /*!< call with an invalid object     */
             orwl_valid,           /*!< object valid, but not requested */
             orwl_requested,       /*!< unspecific request was placed   */
             orwl_read_requested,  /*!< read request was placed         */
             orwl_write_requested, /*!< write request was placed        */
             orwl_acquired,        /*!< unspecific request was acquired */
             orwl_read_acquired,   /*!< read request was acquired       */
             orwl_write_acquired   /*!< write request was acquired      */
             );

DOCUMENT_INIT(orwl_state)
FSYMB_DOCUMENTATION(orwl_state_init)
inline
orwl_state* orwl_state_init
(orwl_state *el,                    /*!< wait queue to initialize */
 orwl_state val                     /*!< defaults to orwl_invalid */
 ) {
  *el = val;
  return el;
}

#ifndef DOXYGEN
inline
PROTOTYPE(orwl_state*, orwl_state_init, orwl_state *, orwl_state);
#define orwl_state_init(...) CALL_WITH_DEFAULTS(orwl_state_init, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_state_init, , orwl_invalid);
#endif

DOCUMENT_DESTROY(orwl_state)
inline
void orwl_state_destroy(orwl_state *el) {
  /* empty */
}

DECLARE_NEW_DELETE(orwl_state);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_state);

struct orwl_wq;

#ifndef __cplusplus
typedef struct orwl_wq orwl_wq;
#endif

DECLARE_ONCE(orwl_wq);

struct orwl_wh;

#ifndef __cplusplus
typedef struct orwl_wh orwl_wh;
#endif

DECLARE_ONCE(orwl_wh);

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
  pthread_mutex_t mut;
  orwl_wh *head;
  orwl_wh *tail;
  uint64_t clock;
};


/**
 ** @brief The handle type corresponding to orwl_wq.
 **
 ** Locks through such a handle object are achieved in a two-step
 ** procedure. First, a lock is @em requested through
 ** ::orwl_wq_request. This binds the orwl_wh to a
 ** designated orwl_wq and appends the request to the FIFO
 ** queue of it. Then in a second step, ::orwl_wh_acquire waits
 ** until the request has become the first in the queue.
 **
 ** ::orwl_wh_release releases the lock by popping the request from
 ** the FIFO. Thus, it allows for the following request in the FIFO
 ** (if any) to be acquired.
 **
 ** orwl_wh is mainly a @c pthread_cond_t that is bound to the
 ** fixed condition to be the first in the FIFO.
 **/
struct orwl_wh {
  /** A wh will wait on that condition for requests and acquires. */
  pthread_cond_t cond;
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
  uint64_t tokens;
  /** @brief The historical position in the wait queue.
   **
   ** The special case of negative priority is used for wh that
   ** represent exclusive locks, i.e where no other token may be
   ** loaded.
   **/
  int64_t priority;
};

#define orwl_wh_garb ((orwl_wh*)(TONES(uintptr_t)))
#define orwl_wq_garb ((orwl_wq*)(TONES(uintptr_t)))

#define ORWL_WQ_INITIALIZER { .mut = PTHREAD_MUTEX_INITIALIZER, .clock = 1 }

  DOCUMENT_INIT(orwl_wq)
FSYMB_DOCUMENTATION(orwl_wq_init)
orwl_wq* orwl_wq_init
(orwl_wq *wq,                    /*!< wait queue to initialize */
 const pthread_mutexattr_t *attr /*!< defaults to @c NULL */
);

#ifndef DOXYGEN
  PROTOTYPE(orwl_wq*, orwl_wq_init, orwl_wq*, const pthread_mutexattr_t*);
#define orwl_wq_init(...) CALL_WITH_DEFAULTS(orwl_wq_init, 2, __VA_ARGS__)

DECLARE_DEFARG(orwl_wq_init, , NULL);
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
   **/
inline
int orwl_wh_valid(orwl_wh *wh) {
  return wh
    && wh->location != orwl_wq_garb
    && wh->next != orwl_wh_garb;
}

  /**
   ** @brief Test @a wh for idleness.
   **
   ** This is just a simple test that @a wh doesn't link to any
   ** location.
   **/
inline
int orwl_wh_idle(orwl_wh *wh) {
  return wh && !wh->location && !wh->next;
}

  /**
   ** @brief Test @a wq for validity.
   **
   ** This is just a simple test that @a wq has not just been
   ** destroyed.
   **
   ** This supposes that @a wq != NULL.
   **/
inline
int orwl_wq_valid(orwl_wq *wq) {
  return wq->head != orwl_wh_garb
    && wq->tail != orwl_wh_garb;
}

  /**
   ** @brief Test @a wq for idleness.
   **
   ** Idleness here means that there is no ::orwl_wh in the FIFO queue
   ** of this location.
   **
   ** This supposes that wq != NULL
   **/
inline
int orwl_wq_idle(orwl_wq *wq) {
  return !wq->head && !wq->tail;
}



#define ORWL_WH_INITIALIZER { PTHREAD_COND_INITIALIZER }

  DOCUMENT_INIT(orwl_wh)
  FSYMB_DOCUMENTATION(orwl_wh_init)
orwl_wh* orwl_wh_init
(orwl_wh *wh,
 const pthread_condattr_t *attr /*!< [in] defaults to @c NULL */);

#ifndef DOXYGEN
  PROTOTYPE(orwl_wh*, orwl_wh_init, orwl_wh *, const pthread_condattr_t *);
#define orwl_wh_init(...) CALL_WITH_DEFAULTS(orwl_wh_init, 2, __VA_ARGS__)

DECLARE_DEFARG(orwl_wh_init, , NULL);
#endif

  DOCUMENT_DESTROY(orwl_wh)
void orwl_wh_destroy(orwl_wh *wh);

DECLARE_NEW_DELETE(orwl_wh);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_wh);

typedef struct {
  orwl_wh *wh;
  uint64_t howmuch;
} _orwl_wh_pair;

/**
 ** @brief Insert a request of @c howmuch tokens on @c wh into location
 ** @a wq. Blocking if one of the @c wh is already requested.
 **
 ** The argument @a number gives the number of pairs (@c wh, @c
 ** howmuch) of orwl_wh and token number pairs that are to be inserted
 ** consequently in the FIFO of @a wq.
 **
 ** @return @c orwl_invalid if any of @c wh or @a wq was
 ** invalid. Otherwise returns @c orwl_requested. Blocking until it is
 ** detected that none of the @c wh is already requested.
 **
 ** This places @c howmuch tokens on each @c wh. Any of the @c wh will
 ** only be possible to be released if, first, it is acquired (that is
 ** it moves front in the FIFO) and then if all tokens are unloaded
 ** with orwl_wh_acquire() or orwl_wh_test().
 **
 ** A @c wh that is given as @c NULL is considered to relate to the
 ** last such handle that had inserted previously, if such a handle
 ** exists.
 **
 ** The tokens are only considered to be loaded on @a wh if the call is
 ** successful.
 **/
  VA_ARGS_DOCUMENTATION(orwl_wq_request)
orwl_state FSYMB(orwl_wq_request)(orwl_wq *wq, VA_ARGS(number));

#define orwl_wq_request(WQ, ...) FSYMB(orwl_wq_request)(WQ, LEN_MODARG(orwl_wq_request, 2, __VA_ARGS__))

VA_TYPES(orwl_wq_request, orwl_wh**, int64_t);

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
 **/
  FSYMB_DOCUMENTATION(orwl_wh_acquire)
orwl_state orwl_wh_acquire
  (orwl_wh *wh,
   uint64_t howmuch    /*!< defaults to @c 1 */);

#ifndef DOXYGEN
  PROTOTYPE(orwl_state, orwl_wh_acquire, orwl_wh*, uint64_t);
#define orwl_wh_acquire(...) CALL_WITH_DEFAULTS(orwl_wh_acquire, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_wh_acquire, , 1);
#endif

/**
 ** Of internal use. Supposes that @a wh is in the queue of @a wq and
 ** that the lock on @a wq is taken by this thread.
 **/
orwl_state orwl_wh_acquire_locked(orwl_wh *wh, orwl_wq *wq);


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
 **/
  FSYMB_DOCUMENTATION(orwl_wh_test)
orwl_state orwl_wh_test
  (orwl_wh *wh,
   uint64_t howmuch  /*!< defaults to 0 */);

#ifndef DOXYGEN
  PROTOTYPE(orwl_state, orwl_wh_test, orwl_wh*, uint64_t);
#define orwl_wh_test(...) CALL_WITH_DEFAULTS(orwl_wh_test, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_wh_test, , 0);
#endif

/**
 ** @brief Release a request on @a wh. If @a wh had been acquired this
 ** is blocking until all tokens are unloaded.
 **
 ** @return @c orwl_invalid if @a wh was invalid, or if there was no
 ** request acquired for @a wh. Otherwise it returns @c orwl_valid.
 **/
orwl_state orwl_wh_release(orwl_wh *wh);

  /** @brief load @a howmuch additional tokens on @a wh.
   ** 
   ** This supposes that the corresponding @c wq != NULL and that @c
   ** wq is already locked.
   ** @see orwl_wh_unload
   */
  FSYMB_DOCUMENTATION(orwl_wh_load)
inline
uint64_t orwl_wh_load
  (orwl_wh *wh,
   uint64_t howmuch  /*!< defaults to 1 */) {
    if (wh->priority > 0)
      wh->tokens += howmuch;
    else
      howmuch = 0;
    return howmuch;
  }

#ifndef DOXYGEN
inline
PROTOTYPE(uint64_t, orwl_wh_load, orwl_wh *, uint64_t);
#define orwl_wh_load(...) CALL_WITH_DEFAULTS(orwl_wh_load, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_wh_load, , 1);
#endif

  /** @brief unload @a howmuch additional tokens from @a wh.
   ** 
   ** This supposes that the corresponding @c wq != NULL and that @c
   ** wq is already locked. If by this action the token count drops to
   ** zero, eventual waiters for this @a wh are notified.
   ** @see orwl_wh_load
   */
  FSYMB_DOCUMENTATION(orwl_wh_unload)
inline
uint64_t orwl_wh_unload
  (orwl_wh *wh,
   uint64_t howmuch  /*!< defaults to 1 */) {
    if (wh->tokens < howmuch) howmuch = wh->tokens;
    wh->tokens -= howmuch;
    /* If the condition has changed, wake up all tokens */
    if (!wh->tokens) pthread_cond_broadcast(&wh->cond);
    return howmuch;
  }

#ifndef DOXYGEN
inline
PROTOTYPE(uint64_t, orwl_wh_unload, orwl_wh *, uint64_t);
#define orwl_wh_unload(...) CALL_WITH_DEFAULTS(orwl_wh_unload, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_wh_unload, , 1);
#endif

DECLARE_ORWL_REGISTER(orwl_wh_acquire);
DECLARE_ORWL_REGISTER(orwl_wh_release);
DECLARE_ORWL_REGISTER(orwl_wh_load);
DECLARE_ORWL_REGISTER(orwl_wh_unload);

#ifdef __cplusplus
}
#endif


#endif
