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
#ifndef   	ORWL_ONCE_H_
# define   	ORWL_ONCE_H_

#include "orwl_posix.h"

P99_DECLARE_STRUCT(orwl__once_cont);

struct orwl__once_cont {
  void (*const init)(void);
  bool cond;
  pthread_mutex_t mut;
};


/**
 ** @brief Declare the symbols that are needed for the macro
 ** INIT_ONCE().
 **
 ** @param T should be unique for each use of this macro.
 ** @see INIT_ONCE
 ** @see DEFINE_ONCE
 **/
#define DECLARE_ONCE(T)                                        \
extern struct orwl__once_cont P99_PASTE3(orwl__, T, _once)

/**
 ** @brief Define the function that will be exactly called once by the
 ** macro INIT_ONCE().
 **
 ** The function has a prototype of <code>void (*name)(void)</code>.
 **
 ** @see DECLARE_ONCE
 **/
#define DEFINE_ONCE(T)                                         \
static void P99_PASTE3(orwl__, T, _once_init)(void);           \
struct orwl__once_cont P99_PASTE3(orwl__, T, _once) = {        \
  .mut = PTHREAD_MUTEX_INITIALIZER,                            \
  .cond = false,                                               \
  .init = P99_PASTE3(orwl__, T, _once_init),                   \
};                                                             \
static void P99_PASTE3(orwl__, T, _once_init)(void)

/**
 ** @brief Protect the following block or statement with @c
 ** pthread_mutex_t @a mut.
 ** @see ORWL_CRITICAL for a tool where you don't have to allocate the
 ** mutex yourself.
 **/
P99_BLOCK_DOCUMENT
#define MUTUAL_EXCLUDE(MUT)                                    \
P99_GUARDED_BLOCK(pthread_mutex_t*,                            \
      P99_FILEID(mut),                                         \
      &(MUT),                                                  \
      pthread_mutex_lock(P99_FILEID(mut)),                     \
      pthread_mutex_unlock(P99_FILEID(mut)))

/**
 ** @brief Protect the following block or statement as a critical
 ** section of the program.
 **
 ** Internally this places a <code>static pthread_mutex_t</code>
 ** here. Don't ask.
 **
 ** Such a critical section is only protected against threads that try
 ** to enter this same critical section. Threads may well be
 ** simultaneously be in different critical sections.
 ** @see MUTUAL_EXCLUDE to protect several critical sections against
 ** each other.
 **/
P99_BLOCK_DOCUMENT
#define ORWL_CRITICAL                                                       \
P00_BLK_START                                                               \
P00_BLK_DECL_STATIC(pthread_mutex_t, orwl__crit, PTHREAD_MUTEX_INITIALIZER) \
MUTUAL_EXCLUDE((*orwl__crit))

/**
 ** @brief Ensure that the function that was defined with
 ** DEFINE_ONCE() has been called exactly once before further
 ** proceeding.
 **/
#define INIT_ONCE(T)                                           \
do {                                                           \
  if (P99_UNLIKELY(!(P99_PASTE3(orwl__, T, _once).cond)))      \
    MUTUAL_EXCLUDE(P99_PASTE3(orwl__, T, _once).mut)           \
      if (!(P99_PASTE3(orwl__, T, _once).cond)) {              \
        fprintf(stderr, "Initializing %s for once\n", # T);    \
        P99_PASTE3(orwl__, T, _once).init();                   \
        P99_PASTE3(orwl__, T, _once).cond = true;              \
      }                                                        \
 } while(0)

/**
 ** @def DECLARE_ONCE_STATIC
 ** @brief Almost the same as ::DECLARE_ONCE, with the difference that
 ** the once-function might be called at any point before a call to
 ** ::INIT_ONCE_STATIC, even if no such call is issued at all
 **
 ** @see DECLARE_ONCE
 ** @see DEFINE_ONCE_STATIC
 ** @see INIT_ONCE_STATIC
 **/


/**
 ** @def DEFINE_ONCE_STATIC
 ** @brief Almost the same as ::DEFINE_ONCE, with the difference that
 ** the once-function might be called at any point before a call to
 ** ::INIT_ONCE_STATIC, even if no such call is issued at all
 **
 ** @see DECLARE_ONCE_STATIC
 ** @see DEFINE_ONCE
 ** @see INIT_ONCE_STATIC
 **/

/**
 ** @def INIT_ONCE_STATIC
 ** @brief Almost the same as ::INIT_ONCE, with the difference that
 ** the once-function might be called at any point before a call to
 ** ::INIT_ONCE_STATIC, even if no such call is issued at all
 **
 ** @see DECLARE_ONCE_STATIC
 ** @see DEFINE_ONCE_STATIC
 ** @see INIT_ONCE
 **/


#if (defined(__GNUC__) && (__GNUC__ > 3))
# define DEFINE_ONCE_STATIC(NAME)                              \
__attribute__((constructor))                                   \
DEFINE_ONCE(NAME)
#else
# define DEFINE_ONCE_STATIC(NAME) DEFINE_ONCE(NAME)
#endif

# define DECLARE_ONCE_STATIC(NAME) DECLARE_ONCE(NAME)
# define INIT_ONCE_STATIC(NAME) INIT_ONCE(NAME)


#endif 	    /* !ORWL_ONCE_H_ */
