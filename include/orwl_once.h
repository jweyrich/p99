/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
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
#include "p99_paste.h"
#include "p99_block.h"

struct orwl__once_upon_cont {
  void (*const init)(void);
  pthread_mutex_t mut;
};

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

#define DECLARE_ONCE_UPON(T)                                    \
extern struct orwl__once_upon_cont P99_PASTE3(orwl__, T, _once)


#define DEFINE_ONCE_UPON(T)                                    \
static void P99_PASTE3(orwl__, T, _once_init)(void);           \
struct orwl__once_upon_cont P99_PASTE3(orwl__, T, _once) = {   \
  .mut = PTHREAD_MUTEX_INITIALIZER,                            \
  .init = P99_PASTE3(orwl__, T, _once_init),                   \
};                                                             \
static void P99_PASTE3(orwl__, T, _once_init)(void)


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
 **/
P99_BLOCK_DOCUMENT
#define MUTUAL_EXCLUDE(MUT)                                    \
P99_GUARDED_BLOCK(pthread_mutex_t*,                            \
      mut,                                                     \
      &(MUT),                                                  \
      pthread_mutex_lock(mut),                                 \
      pthread_mutex_unlock(mut))

/**
 ** @brief Ensure that the function that was defined with
 ** DEFINE_ONCE() has been called exactly once before further
 ** proceeding.
 **
 ** The condition to determine whether or not the function has been
 ** called is given by expression @a N. In most cases @a N will just
 ** be a variable. Be careful:
 **  - @a N should have  no side effects since it is evaluated at
 **    least twice.
 **  - @a N @b must be changed at the very end of the function, since
 **    otherwise new threads that arrive while the function is
 **    executed will continue processing too early.
 **/
#define INIT_ONCE_UPON(T, N)                                   \
do {                                                           \
  if (P99_UNLIKELY(!(N)))                                      \
    MUTUAL_EXCLUDE(P99_PASTE3(orwl__, T, _once).mut)           \
      if (!(N)) P99_PASTE3(orwl__, T, _once).init();           \
 } while(0)

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
# define DECLARE_ONCE_STATIC(NAME)                             \
extern                                                         \
__attribute__((constructor))                                   \
void P99_PASTE3(orwl__, NAME, _once_static)(void)

# define DEFINE_ONCE_STATIC(NAME)                              \
__attribute__((constructor))                                   \
void P99_PASTE3(orwl__, NAME, _once_static)(void)

# define INIT_ONCE_STATIC(NAME)
#else
# define DECLARE_ONCE_STATIC(NAME) DECLARE_ONCE(NAME)
# define DEFINE_ONCE_STATIC(NAME) DEFINE_ONCE(NAME)
# define INIT_ONCE_STATIC(NAME) DEFINE_ONCE(NAME)
#endif



#endif 	    /* !ORWL_ONCE_H_ */
