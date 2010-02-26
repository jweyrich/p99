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
#include <stdint.h>
#include <pthread.h>
#include "orwl_new.h"
#include "orwl_enum.h"

/** @brief Return type for @c orwl functions
 **/
DECLARE_ENUM(orwl_state,
             orwl_invalid,         /**< call with an invalid object     **/
             orwl_valid,           /**< object valid, but not requested **/
             orwl_requested,       /**< unspecific request was placed   **/
             orwl_read_requested,  /**< read request was placed         **/
             orwl_write_requested, /**< write request was placed        **/
             orwl_acquired,        /**< unspecific request was acquired **/
             orwl_read_acquired,   /**< read request was acquired       **/
             orwl_write_acquired   /**< write request was acquired      **/
             );

struct _orwl_wq;

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
typedef struct _orwl_wq orwl_wq;

DECLARE_ONCE(orwl_wq);

struct _orwl_wh;

/**
 ** @brief The handle type corresponding to orwl_wq.
 **
 ** Locks through such a handle object are achieved in a two-step
 ** procedure. First, a lock is @em requested through
 ** orwl_wait_request(). This binds the orwl_wh to a
 ** designated orwl_wq and appends the request to the FIFO
 ** queue of it. Then in a second step, orwl_wait_acquire() waits
 ** until the request has become the first in the queue.
 **
 ** orwl_wait_release() releases the lock by popping the request from
 ** the FIFO. Thus, it allows for the following request in the FIFO
 ** (if any) to be acquired.
 **
 ** orwl_wh is mainly a pthread_cond_t that is bound to the
 ** fixed condition to be the first in the FIFO.
 **/
typedef struct _orwl_wh orwl_wh;

DECLARE_ONCE(orwl_wh);

struct _orwl_wq {
  pthread_mutex_t mut;
  orwl_wh *head;
  orwl_wh *tail;
};


struct _orwl_wh {
  pthread_cond_t cond;
  orwl_wq *location;
  orwl_wh *next;
  uint64_t waiters;
};

#define orwl_wh_garb ((orwl_wh*)(~(uintptr_t)0))
#define orwl_wq_garb ((orwl_wq*)(~(uintptr_t)0))

#define ORWL_WQ_INITIALIZER { PTHREAD_MUTEX_INITIALIZER }

void orwl_wq_init(orwl_wq *wq,
                         const pthread_mutexattr_t *attr);
void orwl_wq_destroy(orwl_wq *wq);

DECLARE_NEW_DELETE(orwl_wq, NULL);

inline
int orwl_wh_valid(orwl_wh *wh) {
  return wh
    && wh->location != orwl_wq_garb
    && wh->next != orwl_wh_garb;
}

inline
int orwl_wh_idle(orwl_wh *wh) {
  return wh && !wh->location && !wh->next;
}

/* This supposes that wq != NULL */
inline
int orwl_wq_valid(orwl_wq *wq) {
  return wq->head != orwl_wh_garb
    && wq->tail != orwl_wh_garb;
}

/* This supposes that wq != NULL */
inline
int orwl_wq_idle(orwl_wq *wq) {
  return !wq->head && !wq->tail;
}



#define ORWL_WH_INITIALIZER { PTHREAD_COND_INITIALIZER }

void orwl_wh_init(orwl_wh *wh,
                          const pthread_condattr_t *attr);
void orwl_wh_destroy(orwl_wh *wh);

DECLARE_NEW_DELETE(orwl_wh, NULL);

/**
 * @brief Insert a request on @a wh into location @a wq
 *
 * @return @c orwl_invalid if any of @a wh or @a wq was invalid, or if
 * there was already a request pending on @a wh. Otherwise returns @c
 * orwl_requested.
 **/
orwl_state orwl_wait_request(orwl_wh *wh, orwl_wq *wq, uint64_t howmuch);

/**
 * @brief Acquire a pending request on @a wh. Blocking until the
 * request is acquired.
 *
 * @return @c orwl_invalid if @a wh was invalid, or if there
 * was no request pending on @a wh. Otherwise it eventually blocks and
 * then returns @c orwl_acquired.
 **/
orwl_state orwl_wait_acquire(orwl_wh *wh, uint64_t howmuch);

orwl_state orwl_wait_acquire_locked(orwl_wh *wh, orwl_wq *wq);


/**
 * @brief Test for a pending request on @a wh. Never blocking.
 *
 * @return @c orwl_invalid if @a wh was invalid, @c orwl_valid if there
 * was no request pending on @a wh, @c orwl_requested if a request is
 * pending that is not yet acquired, and @c orwl_acquired if a request
 * is already acquired.
 **/
orwl_state orwl_wait_test(orwl_wh *wh, uint64_t howmuch);

/**
 * @brief Release a request on @a wh. Never blocking.
 *
 * @return @c orwl_invalid if @a wh was invalid, or if there was no
 * request acquired for @a wh. Otherwise it returns @c orwl_valid.
 **/
orwl_state orwl_wait_release(orwl_wh *wh);

/* This supposes that the corresponding wq != NULL */
inline
void orwl_wh_block(orwl_wh *wh, uint64_t howmuch) {
  wh->waiters += howmuch;
}

/* This supposes that the corresponding wq != NULL */
inline
void orwl_wh_deblock(orwl_wh *wh, uint64_t howmuch) {
  wh->waiters -= howmuch;
  /* If the condition has change, wake up all waiters */
  if (!wh->waiters) pthread_cond_broadcast(&wh->cond);
}

#endif
