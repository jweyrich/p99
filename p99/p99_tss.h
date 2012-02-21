/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_TSS_H
#define P99_TSS_H 1

#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#if defined(__GNUC__) || defined(P00_DOXYGEN)
# include "p99_atomic.h"
#endif

#include "p99_compiler.h"

/**
 ** @addtogroup threads C11 thread emulation on top of POSIX threads
 **
 ** This is a relatively straight forward implementation of the C11
 ** thread model on top of POSIX threads. The main difficulty in that
 ** is that the thread entry function signature differs between the
 ** two. C11 thread returns an <code>int</code> whereas POSIX returns
 ** a <code>void*</code>.
 **
 ** You find the thread management interfaces through the
 ** documentation of the type ::thrd_t.
 **
 ** @remark In addition to POSIX threads this implementation needs
 ** some C11 atomic operations for initialization via ::call_once and
 ** status communication.
 **
 ** @{
 **/

/**
 ** @addtogroup thread_macros
 ** @{
 **/

#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
# warning "definition of PTHREAD_DESTRUCTOR_ITERATIONS is missing"
/**
 ** @brief expands to an integer constant expression representing the
 ** maximum number of times that destructors will be called when a
 ** thread terminates
 ** @memberof tss_t
 **/
# define TSS_DTOR_ITERATIONS 1
#else
# define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS
#endif

/**
 ** @}
 **/


/**
 ** @addtogroup thread_types
 ** @{
 **/

/**
 ** @brief complete object type that holds an identifier for a
 ** thread-specific storage pointer
 **
 ** @remark This type is just a wrapper around a POSIX @c pthread_key_t.
 **
 ** @see P99_DECLARE_THREAD_LOCAL for a more comfortable interface to
 ** thread local variables
 **/
P99_ENC_DECLARE(pthread_key_t, tss_t);

/**
 ** @brief which is the function pointer type <code>void
 ** (*)(void*)</code>, used for a destructor for a thread-specific
 ** storage pointer
 ** @see tss_t
 **/
typedef void (*tss_dtor_t)(void*);

/**
 ** @}
 **/

/**
 ** @addtogroup thread_enum
 ** @{
 **/

/**
 ** @brief C11 thread function return values
 **/
enum thrd_status {
  /**
   ** @brief returned by a timed wait function to indicate that the time specified in the call was reached without acquiring the requested resource
   **/
  thrd_timedout = ETIMEDOUT,
  /**
   ** @brief returned by a function to indicate that the requested operation succeeded
   **/
  thrd_success = 0,
  /**
   ** @brief returned by a function to indicate that the requested
   ** operation failed because a resource requested by a test and
   ** return function is already in use
   **/
  thrd_busy = EBUSY,
  /**
   ** @brief returned by a function to indicate that the requested operation failed
   **/
  thrd_error = INT_MIN,
  /**
   ** @brief returned by a function to indicate that the requested
   ** operation failed because it was unable to allocate memory
   **/
  thrd_nomem = ENOMEM,
  /**
   ** @brief (extension) returned by ::thrd_sleep to indicate that the
   ** corresponding request has been interrupted by a signal
   **/
  thrd_intr = -1
};

/**
 ** @}
 **/

/**
 ** @memberof tss_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **
 ** If successful, sets the thread-specific storage pointed to by key
 ** to a value that uniquely identifies the newly created
 ** pointer. Otherwise, the thread-specific storage pointed to by key
 ** is set to an undefined value.
 **/
p99_inline
int tss_create(tss_t *p00_key, tss_dtor_t dtor) {
  return pthread_key_create(&P99_ENCP(p00_key), dtor) ? thrd_error : thrd_success;
}

/**
 ** @memberof tss_t
 **/
p99_inline
void tss_delete(tss_t p00_key) {
  (void)pthread_key_delete(P99_ENC(p00_key));
}

/**
 ** @memberof tss_t
 **
 ** @return the value for the current thread if successful, or @c 0 if
 ** unsuccessful.
 **/
p99_inline
void *tss_get(tss_t p00_key) {
  return pthread_getspecific(P99_ENC(p00_key));
}

/**
 ** @memberof tss_t
 ** @return ::thrd_success on success, or ::thrd_error if the request
 ** could not be honored.
 **/
p99_inline
int tss_set(tss_t p00_key, void *p00_val) {
  return pthread_setspecific(P99_ENC(p00_key), p00_val) ? thrd_error : thrd_success;
}

#if defined(thread_local) && !defined(P99_EMULATE_THREAD_LOCAL) && !defined(P00_DOXYGEN)

#define P99_DECLARE_THREAD_LOCAL(T, NAME)                      \
P99_WEAK(NAME)                                                 \
thread_local T NAME

#define P99_THREAD_LOCAL(NAME) (NAME)

#else
/**
 ** @brief A stub structure to hold a thread local variable if
 ** ::thread_local is not available.
 **
 ** Don't use this type directly but use ::P99_DECLARE_THREAD_LOCAL to
 ** declare a variable and ::P99_THREAD_LOCAL to access it.
 **
 ** A hypothetical example for the use of such a variable would be @c
 ** errno:
 **
 ** @code
 ** P99_DECLARE_THREAD_LOCAL(int, errno_loc);
 ** #define errno P99_THREAD_LOCAL(errno_loc)
 ** @endcode
 **
 ** With just these two lines @c errno always evaluates to an lvalue
 ** representing a thread local object. That is you can use it
 ** everywhere a normal variable of type @c int could be used:
 **
 ** @code
 ** if (errno == EINTR) ...
 ** errno = 0;
 ** my_func(&errno);
 ** @endcode
 **/
struct p99_tss {
  tss_t p00_val;
  bool volatile p00_done;
  atomic_flag p00_flg;
};

typedef struct p99_tss p99_tss;

/* This is an implementation to bootstrap the thread specific
   code. Once initialization functionalities that are better suited
   for application code are defined elsewhere. */
p99_inline
void p00_key_once_init(p99_tss * p00_key) {
  if (!p00_key->p00_done) {
    P99_SPIN_EXCLUDE(&p00_key->p00_flg) {
      if (!p00_key->p00_done) {
        int p00_ret = tss_create(&P99_ENCP(p00_key), free);
        if (p00_ret) {
          errno = p00_ret;
          perror("can't create thread specific key");
          abort();
        }
        p00_key->p00_done = true;
      }
    }
  }
}

p99_inline
void* p00_thread_local_get(p99_tss * p00_key, size_t p00_size) {
  p00_key_once_init(p00_key);
  void * p00_ret = tss_get(P99_ENCP(p00_key));
  if (P99_UNLIKELY(!p00_ret)) {
    p00_ret = calloc(1, p00_size);
    tss_set(P99_ENCP(p00_key), p00_ret);
  }
  return p00_ret;
}

/**
 ** @def P99_DECLARE_THREAD_LOCAL
 ** @brief declare a thread local variable @a NAME of type @a T
 **
 ** @remark such a variable must be declared in global scope
 **
 ** @see P99_THREAD_LOCAL to access the variable
 ** @see p99_tss for a complete description
 ** @memberof p99_tss
 **/
#define P99_DECLARE_THREAD_LOCAL(T, NAME)                      \
/** @see P99_THREAD_LOCAL to access the variable */            \
P99_WEAK(NAME)                                                 \
p99_tss NAME;                                                  \
typedef T P99_PASTE3(p00_, NAME, _type)

/**
 ** @def P99_THREAD_LOCAL
 ** @brief an lvalue expression that returns the thread local instance
 ** of variable @a NAME
 **
 ** @see P99_DECLARE_THREAD_LOCAL to declare the variable
 ** @see p99_tss for a complete description
 ** @memberof p99_tss
 **/
#define P99_THREAD_LOCAL(NAME) (*(P99_PASTE3(p00_, NAME, _type)*)p00_thread_local_get(&(NAME), sizeof(P99_PASTE3(p00_, NAME, _type))))

#endif


/**
 ** @}
 **/

#endif
