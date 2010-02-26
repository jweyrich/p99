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

#define DECLARE_CALLBACK(T)                                             \
typedef struct {                                                        \
  pthread_cond_t cond;                                                  \
  T *Arg;                                                               \
  orwl_wh *Wh;                                                          \
  orwl_wq *Wq;                                                          \
} _ ## T ## _orwl_wh_t;                                                 \
void _ ## T ## _orwl_wh_t ## _init(_ ## T ## _orwl_wh_t *arg, int i) {  \
  pthread_cond_init(&arg->cond, NULL);                                  \
  arg->Arg = NULL;                                                      \
  arg->Wh = NULL;                                                       \
  arg->Wq = NULL;                                                       \
}                                                                       \
void _ ## T ## _orwl_wh_t ## _destroy(_ ## T ## _orwl_wh_t *arg) {      \
  if (!arg) return;                                                     \
  pthread_cond_destroy(&arg->cond);                                     \
  if (arg->Arg) T ## _delete(arg->Arg);                                 \
}                                                                       \
DECLARE_NEW(_ ## T ## _orwl_wh_t, 0)                                    \
DECLARE_DELETE(_ ## T ## _orwl_wh_t)                                    \
DECLARE_THREAD(_ ## T ## _orwl_wh_t);                                   \
extern int orwl_callback_attach_ ## T(T *Arg, orwl_wh *wh);             \
extern void orwl_callback_ ## T(T *Arg, orwl_wh *Wh)

#define DEFINE_CALLBACK(T)                                              \
DEFINE_THREAD(_ ## T ## _orwl_wh_t) {                                   \
  T *arg = Arg->Arg;                                                    \
  orwl_wh *wh = Arg->Wh;                                                \
  orwl_wq *wq = Arg->Wq;                                                \
  orwl_state state;                                                     \
  pthread_mutex_lock(&wq->mut);                                         \
  pthread_cond_signal(&Arg->cond);                                      \
  /* Afterwards Arg might already be deleted by the caller */           \
  state = orwl_wait_acquire_locked(wh, wq);                             \
  if (state == orwl_acquired) orwl_wh_deblock(wh, 1);                   \
  else                                                                  \
    fprintf(stderr, "thread %lu|%p failed to acquire\n", pthread_self(), (void*)wh); \
  pthread_mutex_unlock(&wq->mut);                                       \
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
        orwl_wh_block(wh, 2);                                           \
        /* create thread detached */                                    \
        ret = _ ## T ## _orwl_wh_t ## _create(pair, NULL);              \
        /* wait until we know that the client thread has attached to */ \
        /* wh                                                        */ \
        pthread_cond_wait(&pair->cond, &wq->mut);                       \
        orwl_wh_deblock(wh, 1);                                         \
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
