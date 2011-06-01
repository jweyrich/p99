/* This may look like nonsense, but it really is -*- mode: C -*-             */
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

P99_DECLARE_STRUCT(o_rwl_once_cont);

struct o_rwl_once_cont {
  void (*const init)(void);
  bool volatile cond;
  pthread_mutex_t mut;
  pthread_t id;
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
extern struct o_rwl_once_cont P99_PASTE3(o_rwl_, T, _once)

#ifdef DOXYGEN
/**
 ** @def DEFINE_ONCE
 ** @brief Define the function that will be exactly called once by
 ** <code>INIT_ONCE(NAME)</code>.
 **
 ** The function has a prototype of <code>void
 ** (*someFunctionName)(void)</code>.
 **
 ** @a NAME can be any valid identifier, the real function name will
 ** be mangled such that this will not clash with an existing one.
 **
 ** The ... list (optional) can be used to give a list of dependencies
 ** from other ::INIT_ONCE functions.
 ** @code
 ** DEFINE_ONCE(toto) {
 **  // initialize some share ressource
 ** }
 **
 ** DEFINE_ONCE(tutu, toto) {
 **   // empty
 ** }
 ** @endcode
 **
 ** This will ensure that <code>INIT_ONCE(toto)</code> is always
 ** triggered by <code>INIT_ONCE(tutu)</code> and run before we run
 ** the function of @c tutu itself. As in this case many functions
 ** will be empty and serve just to ensure that all dynamic
 ** dependencies are initialized in the right order.
 ** @see DECLARE_ONCE
 ** @see INIT_ONCE
 **/
#define DEFINE_ONCE(NAME, ...)
#else
#define DEFINE_ONCE(...)                                       \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                            \
 (O_RWL_DEFINE_ONCE_0(__VA_ARGS__))                            \
 (O_RWL_DEFINE_ONCE_1(__VA_ARGS__))
#endif

#define O_RWL_DEFINE_ONCE_0(T)                                 \
static void P99_PASTE3(o_rwl_, T, _once_init)(void);           \
struct o_rwl_once_cont P99_PASTE3(o_rwl_, T, _once) = {        \
  .mut = PTHREAD_MUTEX_INITIALIZER,                            \
  .cond = false,                                               \
  .init = P99_PASTE3(o_rwl_, T, _once_init),                   \
};                                                             \
static void P99_PASTE3(o_rwl_, T, _once_init)(void)

#define O_RWL_ONCE_INIT(_0, T, _2) INIT_ONCE(T)

#define O_RWL_DEFINE_ONCE_1(T, ...)                                        \
static void P99_PASTE3(o_rwl_, T, _once_init0)(void);                      \
static void P99_PASTE3(o_rwl_, T, _once_init)(void) {                      \
  P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, O_RWL_ONCE_INIT, __VA_ARGS__); \
  P99_PASTE3(o_rwl_, T, _once_init0)();                                    \
 }                                                                         \
struct o_rwl_once_cont P99_PASTE3(o_rwl_, T, _once) = {                    \
  .mut = PTHREAD_MUTEX_INITIALIZER,                                        \
  .cond = false,                                                           \
  .init = P99_PASTE3(o_rwl_, T, _once_init),                               \
};                                                                         \
static void P99_PASTE3(o_rwl_, T, _once_init0)(void)


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
#define ORWL_CRITICAL                                                                   \
P00_BLK_START                                                                           \
 P00_BLK_DECL_STATIC(pthread_mutex_t, P99_LINEID(orwl_crit), PTHREAD_MUTEX_INITIALIZER) \
MUTUAL_EXCLUDE((*P99_LINEID(orwl_crit)))

/**
 ** @brief Utility function used by ::INIT_ONCE.
 **
 ** You should not use this directly.
 **/
inline
void orwl_once_init(o_rwl_once_cont* o, char const name[]) {
  if (pthread_equal(o->id, pthread_self())) {
    fprintf(stderr, "Initializing %s has a cycle!\n", name);
  } else
    MUTUAL_EXCLUDE(o->mut)
      if (!o->cond) {
        fprintf(stderr, "Initializing %s for once\n", name);
        o->id = pthread_self();
        o->init();
        o->cond = true;
      }
}

/**
 ** @brief Ensure that the function that was defined with
 ** ::DEFINE_ONCE has been called exactly once before further
 ** proceeding.
 **
 ** Such a call could be place at the beginning of a user function to
 ** ensure that a shared resource is always initialized before its
 ** use. A better strategy though would be to call ::INIT_ONCE from @c
 ** main, e.g., before any threads of the application are started.
 ** @see DECLARE_ONCE
 ** @see DEFINE_ONCE
 **/
#define INIT_ONCE(T)                                           \
if (P99_LIKELY(P99_PASTE3(o_rwl_, T, _once).cond))             \
  P99_NOP;                                                     \
 else orwl_once_init(&P99_PASTE3(o_rwl_, T, _once), # T);

#endif 	    /* !ORWL_ONCE_H_ */
