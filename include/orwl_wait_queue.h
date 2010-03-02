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
#include "orwl_macro.h"


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
  uintptr_t clock;
};


struct _orwl_wh {
  pthread_cond_t cond;
  orwl_wq *location;
  orwl_wh *next;
  uintptr_t tokens;
  uintptr_t priority;
};

#define orwl_wh_garb ((orwl_wh*)(~(uintptr_t)0))
#define orwl_wq_garb ((orwl_wq*)(~(uintptr_t)0))

#define ORWL_WQ_INITIALIZER { PTHREAD_MUTEX_INITIALIZER }

void FUNC_DEFAULT(orwl_wq_init)(orwl_wq *wq,
                                const pthread_mutexattr_t *attr);

#define orwl_wq_init(...) DEFINE_FUNC_DEFAULT(orwl_wq_init, 2, __VA_ARGS__)

declare_default_arg(orwl_wq_init, 1, const pthread_mutexattr_t *, NULL);

void orwl_wq_destroy(orwl_wq *wq);

DECLARE_NEW_DELETE(orwl_wq);

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

void FUNC_DEFAULT(orwl_wh_init)(orwl_wh *wh,
                          const pthread_condattr_t *attr);
#define orwl_wh_init(...) DEFINE_FUNC_DEFAULT(orwl_wh_init, 2, __VA_ARGS__)

declare_default_arg(orwl_wh_init, 1, const pthread_condattr_t *, NULL);


void orwl_wh_destroy(orwl_wh *wh);

DECLARE_NEW_DELETE(orwl_wh);

typedef struct {
  orwl_wh *wh;
  uintptr_t howmuch;
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
 ** with orwl_wait_acquire() or orwl_wait_test().
 **
 ** The tokens are only considered to be loaded on @a wh if the call is
 ** successful.
 **/
orwl_state _orwl_wait_request(orwl_wq *wq, VA_ARGS(number));

/**
 ** @brief Macro to hide the difficulties of variable length arguments.
 **
 ** This is just a wrapper for _orwl_wait_request() that adds the
 ** additional @c NULL at the end of the list.
 **/
#define orwl_wait_request(WQ, ...) _orwl_wait_request(WQ, LEN_MODARG(2, __VA_ARGS__))

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
orwl_state FUNC_DEFAULT(orwl_wait_acquire)(orwl_wh *wh, uintptr_t howmuch);

#define orwl_wait_acquire(...) DEFINE_FUNC_DEFAULT(orwl_wait_acquire, 2, __VA_ARGS__)
declare_default_arg(orwl_wait_acquire, 1, uintptr_t, 1);

/**
 ** Of internal use. Supposes that @a wh is in the queue of @a wq and
 ** that the lock on @a wq is taken by this thread.
 **/
orwl_state orwl_wait_acquire_locked(orwl_wh *wh, orwl_wq *wq);


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
orwl_state FUNC_DEFAULT(orwl_wait_test)(orwl_wh *wh, uintptr_t howmuch);

#define orwl_wait_test(...) DEFINE_FUNC_DEFAULT(orwl_wait_test, 2, __VA_ARGS__)
declare_default_arg(orwl_wait_test, 1, uintptr_t, 0);

/**
 ** @brief Release a request on @a wh. If @a wh had been acquired this
 ** is blocking until all tokens are unloaded.
 **
 ** @return @c orwl_invalid if @a wh was invalid, or if there was no
 ** request acquired for @a wh. Otherwise it returns @c orwl_valid.
 **/
orwl_state orwl_wait_release(orwl_wh *wh);

/* This supposes that the corresponding wq != NULL */
inline
void FUNC_DEFAULT(orwl_wh_load)(orwl_wh *wh, uintptr_t howmuch) {
  wh->tokens += howmuch;
}

#define orwl_wh_load(...) DEFINE_FUNC_DEFAULT(orwl_wh_load, 2, __VA_ARGS__)
declare_default_arg(orwl_wh_load, 1, uintptr_t, 1);

/* This supposes that the corresponding wq != NULL */
inline
void FUNC_DEFAULT(orwl_wh_unload)(orwl_wh *wh, uintptr_t howmuch) {
  wh->tokens -= howmuch;
  /* If the condition has change, wake up all tokens */
  if (!wh->tokens) pthread_cond_broadcast(&wh->cond);
}

#define orwl_wh_unload(...) DEFINE_FUNC_DEFAULT(orwl_wh_unload, 2, __VA_ARGS__)
declare_default_arg(orwl_wh_unload, 1, uintptr_t, 1);

#endif
