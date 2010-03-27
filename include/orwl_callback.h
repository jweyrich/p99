/*
** orwl_callback.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Thu Feb 25 15:58:33 2010 Jens Gustedt
** Last update Thu Feb 25 15:58:33 2010 Jens Gustedt
*/

#ifndef   	ORWL_CALLBACK_H_
# define   	ORWL_CALLBACK_H_

#include <pthread.h>
#include "orwl_wait_queue.h"
#include "orwl_thread.h"

#define _CALLBACK_PAIR(T) _ ## T ## _orwl_wh_t

/**
 ** @brief Declare that a type @a T acts as a callback that is
 ** piggybacked on a orwl_wh_t.
 **
 ** In a .c file you must define the callback function by means of the
 ** DEFINE_CALLBACK() macro. If your type @a T is cb_t, say, you then
 ** may have two functions declared:
 ** @code
 ** int orwl_callback_attach_cb_t(cb_t *Arg, orwl_wh *wh);
 ** void orwl_callback_cb_t(cb_t *Arg, orwl_wh *wh);
 ** @endcode
 **
 ** The first is used to attach the callback function (with argument
 ** @c Arg) to wait handle @c wh. The second is how the user supplied
 ** callback function is invoked. I.e it see the same two arguments as
 ** were passed through @c orwl_callback_attach_cb_t.
 **
 ** Since the callback function is invoked asynchronously, the caller
 ** may never know when argument @c arg is not used anymore. Therefore
 ** the control on @c arg passes entirely to the callback
 ** routines. User code should never @c free or delete it. It will be
 ** automatically deleted (via @c cb_t_delete) once the callback has
 ** returned.
 **/
#define DECLARE_CALLBACK(T)                                             \
typedef struct {                                                        \
  pthread_cond_t cond;                                                  \
  T *Arg;                                                               \
  orwl_wh *Wh;                                                          \
  orwl_wq *Wq;                                                          \
} _ ## T ## _orwl_wh_t;                                                 \
 DOCUMENT_INIT(_ ## T ## _orwl_wh_t)                                    \
void _ ## T ## _orwl_wh_t ## _init(_ ## T ## _orwl_wh_t *arg) {         \
  pthread_cond_init(&arg->cond, NULL);                                  \
  arg->Arg = NULL;                                                      \
  arg->Wh = NULL;                                                       \
  arg->Wq = NULL;                                                       \
}                                                                       \
 DOCUMENT_DESTROY(_ ## T ## _orwl_wh_t)                                 \
void _ ## T ## _orwl_wh_t ## _destroy(_ ## T ## _orwl_wh_t *arg) {      \
  if (!arg) return;                                                     \
  pthread_cond_destroy(&arg->cond);                                     \
  if (arg->Arg) T ## _delete(arg->Arg);                                 \
}                                                                       \
DECLARE_NEW_DELETE(_ ## T ## _orwl_wh_t);                               \
DECLARE_THREAD(_ ## T ## _orwl_wh_t);                                   \
extern int orwl_callback_attach_ ## T(T *Arg, orwl_wh *wh);             \
extern void orwl_callback_ ## T(T *Arg, orwl_wh *Wh)

/** @brief Define the callback function for type @a T.
 **
 ** @see DECLARE_CALLBACK
 **/
#define DEFINE_CALLBACK(T)                                              \
  DEFINE_NEW_DELETE(_ ## T ## _orwl_wh_t);                              \
DEFINE_THREAD(_ ## T ## _orwl_wh_t) {                                   \
  T *arg = Arg->Arg;                                                    \
  orwl_wh *wh = Arg->Wh;                                                \
  orwl_wq *wq = Arg->Wq;                                                \
  orwl_state state;                                                     \
  MUTUAL_EXCLUDE(wq->mut) {                                             \
    pthread_cond_signal(&Arg->cond);                                    \
    /* Afterwards Arg might already be deleted by the caller */         \
    state = orwl_wh_acquire_locked(wh, wq);                             \
    if (state == orwl_acquired) orwl_wh_unload(wh, 1);                  \
    else                                                                \
      fprintf(stderr, "thread %lu|%p failed to acquire\n", pthread_self(), (void*)wh); \
  }                                                                     \
  if (state == orwl_acquired)                                           \
    orwl_callback_ ## T(arg, wh);                                       \
}                                                                       \
int orwl_callback_attach_ ## T(T *arg, orwl_wh *wh) {                   \
  int ret = 0;                                                          \
  if (orwl_wh_valid(wh)) {                                              \
    orwl_wq *wq = wh->location;                                         \
    if (wq) {                                                           \
      pthread_mutex_lock(&wq->mut);                                     \
      if (wq->head == wh) {                                             \
        pthread_mutex_unlock(&wq->mut);                                 \
        orwl_callback_ ## T(arg, wh);                                   \
      } else {                                                          \
        _ ## T ## _orwl_wh_t *pair = _ ## T ## _orwl_wh_t ## _new();    \
        pair->Arg = arg;                                                \
        pair->Wh = wh;                                                  \
        pair->Wq = wq;                                                  \
        /* the subthread will take care of arg, we should not touch */  \
        /* it anymore */                                                \
        arg = NULL;                                                     \
        /* One token for ourselves, one for our child */                \
        orwl_wh_load(wh, 2);                                            \
        /* create thread detached */                                    \
        ret = _ ## T ## _orwl_wh_t ## _create(pair, NULL);              \
        /* wait until we know that the client thread has attached to */ \
        /* wh                                                        */ \
        pthread_cond_wait(&pair->cond, &wq->mut);                       \
        orwl_wh_unload(wh, 1);                                          \
        /* Be careful to eliminate all garbage that the wrapping has */ \
        /* generated.                                                */ \
        /*_ ## T ## _orwl_wh_t ## _delete(pair);*/                      \
        pthread_mutex_unlock(&wq->mut);                                 \
      }                                                                 \
    }                                                                   \
  }                                                                     \
  if (arg) T ## _delete(arg);                                           \
  return ret;                                                           \
}                                                                       \
void orwl_callback_ ## T(T *Arg, orwl_wh *Wh)

#endif 	    /* !ORWL_CALLBACK_H_ */
