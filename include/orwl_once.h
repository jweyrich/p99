/*
** orwl_once.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Feb 22 21:20:33 2010 Jens Gustedt
** Last update Mon Feb 22 21:20:33 2010 Jens Gustedt
*/

#ifndef   	ORWL_ONCE_H_
# define   	ORWL_ONCE_H_

#include "orwl_posix_default.h"
#include <pthread.h>

struct p99__once_upon_cont {
  void (*const init)(void);
  pthread_mutex_t mut;
};

struct p99__once_cont {
  void (*const init)(void);
  int cond;
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
#define DECLARE_ONCE(T)                         \
extern struct p99__once_cont PASTE3(p99__, T, _once)

#define DECLARE_ONCE_UPON(T)                    \
extern struct p99__once_upon_cont PASTE3(p99__, T, _once)


#define DEFINE_ONCE_UPON(T)                             \
static void PASTE3(p99__, T, _once_init)(void);         \
struct p99__once_upon_cont PASTE3(p99__, T, _once) = {  \
  .mut = PTHREAD_MUTEX_INITIALIZER,                     \
  .init = PASTE3(p99__, T, _once_init),                 \
};                                                      \
static void PASTE3(p99__, T, _once_init)(void)


/**
 ** @brief Define the function that will be exactly called once by the
 ** macro INIT_ONCE().
 **
 ** The function has a prototype of void (*name)(void).
 **
 ** @see DECLARE_ONCE
 **/
#define DEFINE_ONCE(T)                                  \
static void PASTE3(p99__, T, _once_init)(void);         \
struct p99__once_cont PASTE3(p99__, T, _once) = {       \
  .mut = PTHREAD_MUTEX_INITIALIZER,                     \
  .cond = 0,                                            \
  .init = PASTE3(p99__, T, _once_init),                 \
};                                                      \
static void PASTE3(p99__, T, _once_init)(void)

/**
 ** @brief Protect the following block or statement with @c
 ** pthread_mutex_t @a mut.
 **/
DOCUMENT_BLOCK
#define MUTUAL_EXCLUDE(MUT)                     \
SAVE_BLOCK(pthread_mutex_t*,                    \
      mut,                                      \
      &(MUT),                                   \
      pthread_mutex_lock(mut),                  \
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
#define INIT_ONCE_UPON(T, N)                    \
do {                                            \
  if (branch_expect(!(N), false))               \
    MUTUAL_EXCLUDE(PASTE3(p99__, T, _once).mut) \
      if (!(N)) PASTE3(p99__, T, _once).init(); \
 } while(0)

/**
 ** @brief Ensure that the function that was defined with
 ** DEFINE_ONCE() has been called exactly once before further
 ** proceeding.
 **/
#define INIT_ONCE(T)                                            \
do {                                                            \
  if (branch_expect(!(PASTE3(p99__, T, _once).cond), false))    \
    MUTUAL_EXCLUDE(PASTE3(p99__, T, _once).mut)                 \
      if (!(PASTE3(p99__, T, _once).cond)) {                    \
        PASTE3(p99__, T, _once).init();                         \
        PASTE3(p99__, T, _once).cond = 1;                       \
      }                                                         \
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
# define DECLARE_ONCE_STATIC(NAME)              \
extern                                          \
__attribute__((constructor))                    \
void PASTE3(p99__, NAME, _once_static)(void)

# define DEFINE_ONCE_STATIC(NAME)               \
__attribute__((constructor))                    \
void PASTE3(p99__, NAME, _once_static)(void)

# define INIT_ONCE_STATIC(NAME)
#else
# define DECLARE_ONCE_STATIC(NAME) DECLARE_ONCE(NAME)
# define DEFINE_ONCE_STATIC(NAME) DEFINE_ONCE(NAME)
# define INIT_ONCE_STATIC(NAME) DEFINE_ONCE(NAME)
#endif



#endif 	    /* !ORWL_ONCE_H_ */
