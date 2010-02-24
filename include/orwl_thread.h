/*
** orwl_thread.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Feb 23 21:32:15 2010 Jens Gustedt
** Last update Tue Feb 23 21:32:15 2010 Jens Gustedt
*/

#ifndef   	ORWL_THREAD_H_
# define   	ORWL_THREAD_H_

#include <pthread.h>

/**
 ** @brief Internal interface to pthread_create() for joinable threads.
 **/
extern int orwl_pthread_create_joinable(pthread_t *restrict thread,
                                        void *(*start_routine)(void*),
                                        void *restrict arg);

/**
 ** @brief Internal interface to pthread_create() for detached threads.
 **/
extern int orwl_pthread_create_detached(void *(*start_routine)(void*),
                                        void *restrict arg);

/**
 ** @brief Wait for all threads that have been created detached.
 **/
extern void orwl_pthread_wait_detached(void);

/**
 ** @brief Some simple thread launching mechanism.
 **
 ** This declares two functions to create and join a thread that
 ** receives an argument of type @a T.
 ** @code
 ** typedef worker {
 **    int val;
 **    int ret;
 ** } worker;
 ** DECLARE_THREAD(worker);
 ** @endcode
 **
 ** This declares two functions that are accessible by the program and
 ** can be seen as declared as
 ** @code
 ** int worker_create(worker* arg, pthread_t *id);
 ** worker *worker_join(pthread_t id)
 ** @endcode
 ** Here the semantics are similar to pthread_create()  and
 ** pthread_join() with the following differences:
 **
 ** - The argument that the thread receives and returns is of type @a T*
 ** - The return value of @c worker_join will normally be the same as
 **   the input value @c arg for the corresponding call to @c worker_create.
 ** - The worker thread function itself must be created with the macro DEFINE_THREAD().
 ** - Inside the worker thread the argument to the thread is visible as
 ** @code
 ** T *const Arg;
 ** @endcode
 **
 ** -  worker_create may be called with @c id set to @c NULL. The
 **    thread is then created detached, as obviously you don't know
 **    any thread id to join to. All threads that are created this way
 **    should be awaited for at the end of the main program. Use
 **    orwl_pthread_wait_detached() for that purpose.
 **/
#define DECLARE_THREAD(T)                                               \
static inline T *T ## _join(pthread_t id) {                             \
  void *ret = NULL;                                                     \
  pthread_join(id, &ret);                                               \
  return ret;                                                           \
}                                                                       \
extern void *T ## _start_joinable(void* arg);                           \
extern void *T ## _start_detached(void* arg);                           \
static inline int T ## _create(T* arg, pthread_t *id) {                 \
  if (id)                                                               \
    return orwl_pthread_create_joinable(id, T ## _start_joinable, arg); \
  else                                                                  \
    return orwl_pthread_create_detached(T ## _start_detached, arg);     \
}                                                                       \
extern void *T ## _start_joinable(void* arg)

/**
 ** @brief Define the function that is to be executed by a thread
 ** creation for type @a T.
 **
 ** For details see DECLARE_THREAD(). With the example from there we
 ** should use this in the corresponding .c file as
 ** @code
 ** DEFINE_THREAD(worker) {
 **  Arg->ret = 43 * Arg->val;
 ** }
 ** @endcode
 **
 ** Anywhere in your code you could spawn a thread and then join it
 ** with something like this
 ** @code
 ** worker work = { .val = 37 };
 ** pthread_t id;
 ** worker_create(&work, &id);
 ** ... do something in parallel ...
 ** worker *reply = worker_join(id);
 ** ... do something with reply->ret ...
 ** @endcode
 **
 ** Beware that in this simple example the variable @c work lives on
 ** the stack of the calling function. So this function must
 ** imperatively call @c worker_join before it exits. If you can't be
 ** sure of that, use calls to @c worker_new and @c worker_delete
 ** (declared with DECLARE_NEW_DELETE(worker)) to allocate and free
 ** the memory.
 ** @code
 ** worker *work = worker_new();
 ** worker->val = 37;
 ** pthread_t id;
 ** worker_create(&work, &id);
 ** ... do something in parallel ...
 ** ... maybe return from the calling function ...
 ** ... but keep the `id' handy ...
 ** worker *reply = worker_join(id);
 ** ... do something with reply->ret ...
 ** ... don't forget to free the memory ...
 ** worker_delete(reply);
 ** @endcode
 **
 ** Another case occurs when you want the thread to be created in a
 ** detached state. This can simply be achieved by passing @c NULL
 ** instead of a pointer to an @c id. In such a case you don't have
 ** much control on when the thread is finished so you @b must
 ** use calls to @c worker_new to allocate the memory.
 ** After your thread has terminated, @c worker_delete will
 ** automatically be called to free the space that had been allocated.
 ** @code
 ** worker *work = worker_new();
 ** worker->val = 37;
 ** worker_create(&work, NULL);
 ** ... do something in parallel ...
 ** ... maybe return from the calling function ...
 ** ... at the very end of your main, wait for all detached threads ...
 ** orwl_pthread_wait_detached();
 ** @endcode
 **/
#define DEFINE_THREAD(T)                                        \
static inline void _ ## T ## _start(T* Arg);                    \
void *T ## _start_joinable(void* arg) {                         \
  T *Arg = (T*)arg;                                             \
  _ ## T ## _start(Arg);                                        \
  return arg;                                                   \
}                                                               \
void *T ## _start_detached(void* arg) {                         \
  T *Arg = (T*)arg;                                             \
  _ ## T ## _start(Arg);                                        \
  T ## _delete(Arg);                                            \
  return NULL;                                                  \
}                                                               \
static inline void _ ## T ## _start(T *const Arg)


#endif 	    /* !ORWL_THREAD_H_ */
