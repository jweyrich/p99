/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_CALLBACK_H_
# define   	ORWL_CALLBACK_H_

#include "orwl_wait_queue.h"

#define P00_CALLBACK_PAIR(T) P99_PASTE3(p00_, T, _orwl_wh_t)

/**
 ** @brief Declare that a type @a T acts as a callback that is
 ** piggybacked on a ::orwl_wh.
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
#define DECLARE_CALLBACK(T)                                                   \
typedef struct {                                                              \
  pthread_cond_t cond;                                                        \
  T *Arg;                                                                     \
  orwl_wh *Wh;                                                                \
  orwl_wq *Wq;                                                                \
} P00_CALLBACK_PAIR(T);                                                       \
 DOCUMENT_INIT(P00_CALLBACK_PAIR(T))                                          \
inline                                                                        \
P00_CALLBACK_PAIR(T) *                                                        \
P99_PASTE2(P00_CALLBACK_PAIR(T), _init)(P00_CALLBACK_PAIR(T) *arg,            \
                                   T* Arg,                                    \
                                   orwl_wh *Wh,                               \
                                   orwl_wq *Wq                                \
                                   ) {                                        \
   pthread_cond_init(&arg->cond, P99_0(pthread_condattr_t*));                 \
   arg->Arg = Arg;                                                            \
   arg->Wh = Wh;                                                              \
   arg->Wq = Wq;                                                              \
   return arg;                                                                \
}                                                                             \
 DOCUMENT_DESTROY(P00_CALLBACK_PAIR(T))                                       \
inline                                                                        \
 void P99_PASTE2(P00_CALLBACK_PAIR(T), _destroy)(P00_CALLBACK_PAIR(T) *arg) { \
  if (!arg) return;                                                           \
  pthread_cond_destroy(&arg->cond);                                           \
  if (arg->Arg) P99_PASTE2(T, _delete)(arg->Arg);                             \
}                                                                             \
DECLARE_DELETE(P00_CALLBACK_PAIR(T))                                          \
DECLARE_THREAD(P00_CALLBACK_PAIR(T));                                         \
extern int P99_PASTE2(orwl_callback_attach_, T)(T *Arg, orwl_wh *wh);         \
extern void P99_PASTE2(orwl_callback_, T)(T *Arg, orwl_wh *Wh)

/** @brief Define the callback function for type @a T.
 **
 ** @see DECLARE_CALLBACK
 **/
#define DEFINE_CALLBACK(T)                                                        \
P99_INSTANTIATE(                                                                  \
                P00_CALLBACK_PAIR(T)*,                                            \
                P99_PASTE2(P00_CALLBACK_PAIR(T), _init),                          \
                P00_CALLBACK_PAIR(T)*,                                            \
                T*,                                                               \
                orwl_wh*,                                                         \
                orwl_wq*                                                          \
                );                                                                \
P99_INSTANTIATE(                                                                  \
                void,                                                             \
                P99_PASTE2(P00_CALLBACK_PAIR(T), _destroy),                       \
                P00_CALLBACK_PAIR(T)*);                                           \
DEFINE_DELETE(P00_CALLBACK_PAIR(T));                                              \
DEFINE_THREAD(P00_CALLBACK_PAIR(T)) {                                             \
  T *arg = Arg->Arg;                                                              \
  orwl_wh *wh = Arg->Wh;                                                          \
  orwl_wq *wq = Arg->Wq;                                                          \
  orwl_state state = orwl_invalid;                                                \
  MUTUAL_EXCLUDE(wq->mut) {                                                       \
    pthread_cond_signal(&Arg->cond);                                              \
    /* Afterwards Arg might already be deleted by the caller */                   \
    state = orwl_wh_acquire_locked(wh, wq);                                       \
    if (state == orwl_acquired) orwl_wh_unload(wh, 1);                            \
    else                                                                          \
      fprintf(stderr,                                                             \
              "thread %lu|%p failed to acquire\n",                                \
              (ulong)pthread_self(),                                              \
              (void*)wh);                                                         \
  }                                                                               \
  if (state == orwl_acquired)                                                     \
    P99_PASTE2(orwl_callback_, T)(arg, wh);                                       \
}                                                                                 \
P99_INSTANTIATE(                                                                  \
                 int, P99_PASTE2(orwl_callback_attach_, T), T*, orwl_wh*);        \
 /* inline  */                                                                    \
int P99_PASTE2(orwl_callback_attach_, T)(T *arg, orwl_wh *wh) {                   \
  int ret = 0;                                                                    \
  if (orwl_wh_valid(wh)) {                                                        \
    orwl_wq *wq = wh->location;                                                   \
    if (wq) {                                                                     \
      pthread_mutex_lock(&wq->mut);                                               \
      if (wq->head == wh) {                                                       \
        pthread_mutex_unlock(&wq->mut);                                           \
        P99_PASTE2(orwl_callback_, T)(arg, wh);                                   \
      } else {                                                                    \
        P00_CALLBACK_PAIR(T) *pair                                                \
          = P99_NEW(P00_CALLBACK_PAIR(T), arg, wh, wq);                           \
        /* the subthread will take care of arg, we should not touch */            \
        /* it anymore */                                                          \
        arg = P99_0(T*);                                                          \
        /* One token for ourselves, one for our child */                          \
        orwl_wh_load(wh, 2);                                                      \
        /* create thread detached */                                              \
        ret = P99_PASTE2(P00_CALLBACK_PAIR(T), _create)(pair, P99_0(pthread_t*)); \
        /* wait until we know that the client thread has attached to */           \
        /* wh                                                        */           \
        pthread_cond_wait(&pair->cond, &wq->mut);                                 \
        orwl_wh_unload(wh, 1);                                                    \
        /* Be careful to eliminate all garbage that the wrapping has */           \
        /* generated.                                                */           \
        /*p00_ ## T ## _orwl_wh_t ## _delete(pair);*/                             \
        pthread_mutex_unlock(&wq->mut);                                           \
      }                                                                           \
    }                                                                             \
  }                                                                               \
  if (arg) P99_PASTE2(T, _delete)(arg);                                           \
  return ret;                                                                     \
}                                                                                 \
 P99_INSTANTIATE(void, P99_PASTE2(orwl_callback_, T), T*, orwl_wh*);              \
 /* inline  */                                                                    \
    void P99_PASTE2(orwl_callback_, T)(T *Arg, orwl_wh *Wh)

#endif 	    /* !ORWL_CALLBACK_H_ */
