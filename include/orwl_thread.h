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
#ifndef   	ORWL_THREAD_H_
# define   	ORWL_THREAD_H_

#include "orwl_once.h"
#include "orwl_int.h"
#include "orwl_macro.h"
#include "orwl_time.h"
#include "p99_defarg.h"

/**
 ** @brief A default (global!) version of something like a
 ** thread id.
 **
 ** This is used by #report and #progress, unless you define a local
 ** variable with the same name.
 **
 ** This is constant. If you want to use different values per thread
 ** you should declare it on the stack of the thread.
 **/
extern size_t const orwl_mynum;

/**
 ** @brief A default (global!) version of something like the
 ** total number of threads.
 **
 ** This is used by #report and #progress, unless you define a local
 ** variable with the same name.
 **
 ** This variable here is shared among all threads. So you should
 ** probably only assign this variable once and then evaluate it.
 **/
extern size_t orwl_np;

/**
 ** @brief A default (global!) version of something like a phase
 ** your program is in.
 **
 ** This is used by #report and #progress, unless you define a local
 ** variable with the same name.
 **
 ** This variable here is shared among all threads. So you should
 ** rarely assign this variable and then mostly evaluate it.
 **/
extern size_t orwl_phase;

/**
 ** @brief Internally use by report().
 **/
#ifdef __GNUC__
__attribute__((format(printf, 4, 5)))
#endif
extern void orwl_report(size_t mynum, size_t np, size_t phase, char const* format, ...);

/**
 ** @brief Internally use by progress().
 **/
#ifdef __GNUC__
__attribute__((format(printf, 5, 6)))
#endif
extern void orwl_progress(size_t t, size_t mynum, size_t np, size_t phase, char const* format, ...);

/**
 ** @brief Report to @c stderr if @a expr is fulfilled.
 **
 ** The ... arguments to that function should just be as for the @c
 ** printf family, i.e a format followed with a list of arguments.
 **
 ** The output is prefixed with an identification of the thread. This
 ** is either the return of @c pthread_self or the value of a local
 ** variable #orwl_mynum. Then a phase id is shown, either the value
 ** of the global variable #orwl_phase or of a local variable with the
 ** same name.
 **/
#define report(exp, ...)                                       \
do {                                                           \
  if ((bool)exp) {                                             \
    orwl_report(orwl_mynum, orwl_np, orwl_phase, __VA_ARGS__); \
  }                                                            \
 } while(0)


/**
 ** @brief Report progress to @c stderr if @a expr is fulfilled. @a t
 ** is supposed to be a progress count.
 **
 ** @a t modulo 4 is used to print a spinning \.
 **/
#define progress(exp, t, ...)                                       \
do {                                                                \
  if ((bool)exp) {                                                  \
    orwl_progress(t, orwl_mynum, orwl_np, orwl_phase, __VA_ARGS__); \
  }                                                                 \
 } while(0)


typedef void *(*start_routine_t)(void*);


extern pthread_attr_t const*const pthread_attr_detached;
extern pthread_attr_t const*const pthread_attr_joinable;
extern pthread_mutexattr_t const*const pthread_mutexattr_thread;
extern pthread_mutexattr_t const*const pthread_mutexattr_process;
extern pthread_condattr_t const*const pthread_condattr_thread;
extern pthread_condattr_t const*const pthread_condattr_process;
extern pthread_rwlockattr_t const*const pthread_rwlockattr_thread;
extern pthread_rwlockattr_t const*const pthread_rwlockattr_process;

DECLARE_ONCE(orwl_thread);
DECLARE_ONCE(pthread_mutex_t);
DECLARE_ONCE(pthread_cond_t);
DECLARE_ONCE(pthread_rwlock_t);

/**
 ** @brief Internal interface to pthread_create() for joinable threads.
 **/
extern int orwl_pthread_create_joinable(pthread_t *restrict thread,
                                        start_routine_t start_routine,
                                        void *restrict arg);

/**
 ** @brief Internal interface to pthread_create() for detached threads.
 **/
extern int orwl_pthread_create_detached(start_routine_t start_routine,
                                        void *restrict arg);

/**
 ** @brief Wait for all threads that have been created detached.
 **/
extern void orwl_pthread_wait_detached(void);

#ifdef DOXYGEN
/**
 ** @brief Define the function that is to be executed by a thread
 ** creation for type @a T.
 **
 ** For more details see also ::DECLARE_THREAD. With the example from there we
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
 ** worker_create_joinable(&work, &id);
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
 ** worker_create_joinable(&work, &id);
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
 ** detached state. In such a case you don't have
 ** much control on when the thread is finished so you @b must
 ** use calls to @c worker_new to allocate the memory.
 ** After your thread has terminated, @c worker_delete will
 ** automatically be called to free the space that had been allocated.
 ** @code
 ** worker *work = worker_new();
 ** worker->val = 37;
 ** worker_create_detached(work);
 ** ... do something in parallel ...
 ** ... maybe return from the calling function ...
 ** ... at the very end of your main, wait for all detached threads ...
 ** orwl_pthread_wait_detached();
 ** @endcode
 **/
#define DEFINE_THREAD(T)

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
 ** This declares functions that are accessible by the program and
 ** can be seen as declared as
 ** @code
 ** int worker_create(worker* arg, pthread_t *id);
 ** int worker_create_joinable(worker* arg, pthread_t *id);
 ** worker *worker_join(pthread_t id);
 ** int worker_create_detached(worker* arg);
 ** void worker_start(worker*const arg);
 ** @endcode
 ** Here the semantics are similar to pthread_create()  and
 ** pthread_join() with the following differences:
 **
 ** - The argument that the thread receives and returns is of type @a T*
 ** - The return value of @c worker_join will normally be the same as
 **   the input value @c arg for the corresponding call to @c worker_create.
 ** - The worker thread callback function itself is @c worker_start
 **    and must be created with the macro ::DEFINE_THREAD.
 ** - @c worker_create may be called with @c id set to a null pointer,
 **    this then is equivalent to calling @c worker_create_detached. The
 **    thread is then created detached, as obviously you don't know
 **    any thread id to join to. All threads that are created this way
 **    should be awaited for at the end of the main program. Use
 **    ::orwl_pthread_wait_detached() for that purpose.
 **/
#define DECLARE_THREAD(T)                                                                                 \
/*! @note This is the callback for each thread of type T */                                               \
/*! @see DEFINE_THREAD for details on how to launch threads */                                            \
/*! @memberof T */                                                                                        \
/*! @param Arg the T object for this call */                                                              \
void P99_PASTE2(T, _start)(T *const Arg);                                                                 \
/*! @brief Launch a thread for type T */                                                                  \
/*! @memberof T */                                                                                        \
/*! @param arg the T object for this call that will be passed to \ref T ## _start */                      \
/*! @param id if not a null pointer, the thread can be joined on this @a id */                            \
int P99_PASTE2(T, _create)(T* arg, pthread_t *id);                                                        \
/*! @brief Launch a joinable thread for type T */                                                         \
/*! @memberof T */                                                                                        \
/*! @param arg the T object for this call that will be passed to \ref T ## _start */                      \
/*! @param id must not be a null pointer, the thread can be joined on this @a id */                       \
/*! @warning The arguments @a arg and @a id remain under the responsability of the caller. */             \
int P99_PASTE2(T, _create_joinable)(T* arg, pthread_t *id);                                               \
/*! @brief Join a callback procedure of type T that has been launched with \ref T ## _create_joinable. */ \
/*! @memberof T */                                                                                        \
/*! @param id the thread that is to be joined */                                                          \
/*! @return the original argument for which this thread was created */                                    \
T* P99_PASTE2(T, _join)(pthread_t id);                                                                    \
/*! @brief Launch a detached thread for type T */                                                         \
/*! @memberof T */                                                                                        \
/*! @param arg the T object for this call that will be passed to \ref T ## _start */                      \
/*! @warning The argument @a arg should not be modified by the caller, hereafter. */                      \
int P99_PASTE2(T, _create_detached)(T* arg)
#else
#define DECLARE_THREAD(T)                                                         \
void P99_PASTE2(T, _start)(T* Arg);                                               \
inline T * P99_PASTE2(T, _join)(pthread_t id) {                                   \
  void *ret = 0;                                                                  \
  pthread_join(id, &ret);                                                         \
  return ret;                                                                     \
}                                                                                 \
inline void *P99_PASTE2(T, _start_joinable)(void* arg) {                          \
  T *Arg = (T*)arg;                                                               \
  P99_PASTE2(T, _start)(Arg);                                                     \
  return arg;                                                                     \
}                                                                                 \
inline void *P99_PASTE2(T, _start_detached)(void* arg) {                          \
  T *Arg = (T*)arg;                                                               \
  P99_PASTE2(T, _start)(Arg);                                                     \
  P99_PASTE2(T, _delete)(Arg);                                                    \
  return 0;                                                                       \
}                                                                                 \
inline int P99_PASTE2(T, _create_joinable)(T* arg, pthread_t *id) {               \
  return orwl_pthread_create_joinable(id, P99_PASTE2(T, _start_joinable), arg);   \
}                                                                                 \
inline int P99_PASTE2(T, _create_detached)(T* arg) {                              \
  return orwl_pthread_create_detached(P99_PASTE2(T, _start_detached), arg);       \
}                                                                                 \
inline int P99_PASTE2(T, _create)(T* arg, pthread_t *id) {                        \
  if (id)                                                                         \
    return orwl_pthread_create_joinable(id, P99_PASTE2(T, _start_joinable), arg); \
  else                                                                            \
    return orwl_pthread_create_detached(P99_PASTE2(T, _start_detached), arg);     \
}                                                                                 \
P99_MACRO_END(declare_thread)
#define DEFINE_THREAD(T)                                               \
P99_INSTANTIATE(T*, P99_PASTE2(T, _join), pthread_t);                  \
P99_INSTANTIATE(int, P99_PASTE2(T, _create), T*, pthread_t*);          \
P99_INSTANTIATE(int, P99_PASTE2(T, _create_joinable), T*, pthread_t*); \
P99_INSTANTIATE(int, P99_PASTE2(T, _create_detached), T*);             \
P99_INSTANTIATE(void*, P99_PASTE2(T, _start_joinable), void*);         \
P99_INSTANTIATE(void*, P99_PASTE2(T, _start_detached), void*);         \
void P99_PASTE2(T, _start)(T *const Arg)
#endif

inline pthread_t* pthread_t_init(pthread_t *id) {
  if (!id) return 0;
  *id = P99_LVAL(pthread_t const);
  return id;
}
inline void pthread_t_destroy(pthread_t *id) {
  /* special care for bogus warning given by icc */
  (void)id;
}

DECLARE_NEW_DELETE(pthread_t);



/**
 ** @brief Relax the @c sem_t @a SEM during execution of a dependent
 ** block or statement.
 **/
P99_BLOCK_DOCUMENT
#define SEM_RELAX(SEM)                                         \
P99_GUARDED_BLOCK(                                             \
           sem_t*,                                             \
           sem,                                                \
           &(SEM),                                             \
           sem_post(sem),                                      \
           sem_wait_nointr(sem))

/**
 ** @brief Request one token from @c sem_t @a SEM during execution of a dependent
 ** block or statement.
 **
 ** Execution will block until the value of @a SEM is positive, i.e a
 ** token is available on @a SEM. The token is re-injected into @a SEM
 ** after execution of the dependent block or statement.
 **
 ** @see sem_wait_nointr
 ** @see sem_post
 ** @see MUTUAL_EXCLUDE
 **/
P99_BLOCK_DOCUMENT
#define SEM_CRITICAL(SEM)                                      \
P99_GUARDED_BLOCK(                                             \
           sem_t*,                                             \
           sem,                                                \
           &(SEM),                                             \
           sem_wait_nointr(sem),                               \
           sem_post(sem))

inline
char const* pthread2str(char *buf, pthread_t id) {
  uchar *p = (uchar*)&id;
  for (unsigned i = 0; i < sizeof(pthread_t); ++i) {
    snprintf(buf + 2*i, 3, "%02" PRIX8, p[i]);
  }
  return buf;
}


#define PTHREAD2STR(ID) pthread2str((char[1 + sizeof(pthread_t) * 2]){0}, ID)

#if defined(__GNUC__) && !defined(__GNUC_NO_TLS__)

#define DECLARE_THREAD_VAR(T, NAME)                                                          \
T* P99_PASTE2(NAME, _init)(void);                                                            \
/* guarantee that the thread variable is properly initialized */                             \
/* var is first in the struct to have a quick return on the fast path */                     \
typedef struct P99_PASTE2(NAME, _type) { T var; bool initialized; } P99_PASTE2(NAME, _type); \
extern __thread P99_PASTE2(NAME, _type) P99_PASTE2(NAME, _var);                              \
/* The initialization is not inlined to take it out of the optimizers                        \
   view */                                                                                   \
/* In the function itself everything is done to privilege the fast                           \
   path */                                                                                   \
inline T* NAME(void) {                                                                       \
  register P99_PASTE2(NAME, _type)*const ret = &P99_PASTE2(NAME, _var);                      \
  register bool*const initialized = &ret->initialized;                                       \
  register T*const var = &ret->var;                                                          \
  if (P99_LIKELY(*initialized))                                                              \
    return var;                                                                              \
  else                                                                                       \
    return P99_PASTE2(NAME, _init)();                                                        \
}                                                                                            \
inline void P99_PASTE2(NAME, _clear)(void) {                                                 \
}                                                                                            \
P99_MACRO_END(DECLARE_THREAD_VAR)

#define DEFINE_THREAD_VAR(T, NAME)                                                  \
__thread P99_PASTE2(NAME, _type) P99_PASTE2(NAME, _var) = { .initialized = false }; \
 P99_INSTANTIATE(void, P99_PASTE2(NAME, _clear));                                   \
T* P99_PASTE2(NAME, _init)(void) {                                                  \
  register P99_PASTE2(NAME, _type)*const ret = &P99_PASTE2(NAME, _var);             \
  register bool*const initialized = &ret->initialized;                              \
  register T* const var = &ret->var;                                                \
  P99_PASTE2(T, _init)(var);                                                        \
  *initialized = true;                                                              \
  return var;                                                                       \
}                                                                                   \
P99_INSTANTIATE(T*, NAME)

#endif


/* The default implementation of thread local storage */
#ifndef DECLARE_THREAD_VAR

#define P00_DECLARE_THREAD_VAR(T, NAME, KEY)                   \
extern pthread_key_t KEY;                                      \
DECLARE_ONCE_STATIC(KEY);                                      \
inline T* NAME(void) {                                         \
  INIT_ONCE_STATIC(KEY);                                       \
  T* ret = pthread_getspecific(KEY);                           \
  if (P99_UNLIKELY(!ret)) {                                    \
    ret = P99_NEW(T);                                          \
    (void)pthread_setspecific(KEY, ret);                       \
  }                                                            \
  return ret;                                                  \
}                                                              \
inline void P99_PASTE2(NAME, _clear)(void) {                   \
  INIT_ONCE_STATIC(KEY);                                       \
  T* ret = pthread_getspecific(KEY);                           \
  if (P99_LIKELY(!!ret)) {                                     \
    (void)pthread_setspecific(KEY, P99_0(void*));              \
    P99_PASTE2(T, _delete)(ret);                               \
  }                                                            \
}                                                              \
extern pthread_key_t KEY


#define DECLARE_THREAD_VAR(T, NAME)                            \
/*! An accessor function to a thread local variable */         \
inline T* NAME(void);                                          \
P00_DECLARE_THREAD_VAR(T, NAME, P99_PASTE3(p00_, NAME, _key))


#define P00__DEFINE_THREAD_VAR(T, NAME, KEY)                                 \
pthread_key_t KEY;                                                           \
DEFINE_ONCE_STATIC(KEY) {                                                    \
  (void) pthread_key_create(&KEY, (void (*)(void *))P99_PASTE2(T, _delete)); \
}                                                                            \
P99_INSTANTIATE(void, P99_PASTE2(NAME, _clear));                             \
P99_INSTANTIATE(T*, NAME)

#define P00_DEFINE_THREAD_VAR(T, NAME, KEY) P00__DEFINE_THREAD_VAR(T, NAME, KEY)

#define DEFINE_THREAD_VAR(T, NAME) P00_DEFINE_THREAD_VAR(T, NAME, P99_PASTE3(p00_, NAME, _key))


#endif /* end of default implementation */

#endif 	    /* !ORWL_THREAD_H_ */
