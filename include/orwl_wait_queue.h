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

/** @brief Return type for @c orwl functions
 ** 
 **/
typedef enum {
  orwl_invalid = -1,    /**< call with an invalid object     **/
  orwl_valid = 0,       /**< object valid, but not requested **/
  orwl_requested,       /**< unspecific request was placed   **/
  orwl_read_requested,  /**< read request was placed         **/
  orwl_write_requested, /**< write request was placed        **/
  orwl_acquired,        /**< unspecific request was acquired **/
  orwl_read_acquired,   /**< read request was acquired       **/
  orwl_write_acquired,  /**< write request was acquired      **/
  /* LEAVE AS LAST ELEMENT */
  orwl_state_sup
} orwl_state;

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

#define ORWL_WQ_INITIALIZER { PTHREAD_MUTEX_INITIALIZER }

void orwl_wq_init(orwl_wq *wq,
                         const pthread_mutexattr_t *attr);
void orwl_wq_destroy(orwl_wq *wq);

orwl_wq *orwl_wq_new(void);
void orwl_wq_delete(orwl_wq *wq);


#define ORWL_WH_INITIALIZER { PTHREAD_COND_INITIALIZER }

void orwl_wh_init(orwl_wh *wh,
                          const pthread_condattr_t *attr);
void orwl_wh_destroy(orwl_wh *wh);

orwl_wh *orwl_wh_new(void);
void orwl_wh_delete(orwl_wh *wh);

/**
 * @brief Insert a request on @a wh into location @a wq
 *
 * @return @c orwl_invalid if any of @a wh or @a wq was invalid, or if
 * there was already a request pending on @a wh. Otherwise returns @c
 * orwl_requested.
 **/
orwl_state orwl_wait_request(orwl_wh *wh, orwl_wq *wq);

/**
 * @brief Acquire a pending request on @a wh. Blocking until the
 * request is acquired.
 *
 * @return @c orwl_invalid if @a wh was invalid, or if there
 * was no request pending on @a wh. Otherwise it eventually blocks and
 * then returns @c orwl_acquired.
 **/
orwl_state orwl_wait_acquire(orwl_wh *wh);

/**
 * @brief Test for a pending request on @a wh. Never blocking.
 *
 * @return @c orwl_invalid if @a wh was invalid, @c orwl_valid if there
 * was no request pending on @a wh, @c orwl_requested if a request is
 * pending that is not yet acquired, and @c orwl_acquired if a request
 * is already acquired.
 **/
orwl_state orwl_wait_test(orwl_wh *wh);

/**
 * @brief Release a request on @a wh. Never blocking.
 *
 * @return @c orwl_invalid if @a wh was invalid, or if there was no
 * request acquired for @a wh. Otherwise it returns @c orwl_valid.
 **/
orwl_state orwl_wait_release(orwl_wh *wh);


#endif
