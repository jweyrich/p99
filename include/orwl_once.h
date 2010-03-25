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

#include <pthread.h>

struct once_uponn_cont {
  void (*const init)(void);
  pthread_mutex_t mut;
};

struct once_cont {
  void (*const init)(void);
  int cond;
  pthread_mutex_t mut;
};


/**
 ** @brief Declare the symbols that are needed for the macro
 ** INIT_ONCE().
 **
 ** @param T should be a type.
 **/
#define DECLARE_ONCE(T)                         \
extern struct once_cont _ ## T ## _once

#define DECLARE_ONCE_UPON(T)                    \
extern struct once_uponn_cont _ ## T ## _once


#define DEFINE_ONCE_UPON(T)                     \
static void _ ## T ## _once_init(void);         \
struct once_uponn_cont _ ## T ## _once = {      \
  .mut = PTHREAD_MUTEX_INITIALIZER,             \
  .init = _ ## T ## _once_init,                 \
};                                              \
static void _ ## T ## _once_init(void)


/**
 ** @brief Define the function that will be exactly called once by the
 ** macro INIT_ONCE().
 **
 ** The function has a prototype of void (*name)(void).
 **
 ** @param T should be a type.
 **/
#define DEFINE_ONCE(T)                          \
static void _ ## T ## _once_init(void);         \
struct once_cont _ ## T ## _once = {            \
  .mut = PTHREAD_MUTEX_INITIALIZER,             \
  .cond = 0,                                    \
  .init = _ ## T ## _once_init,                 \
};                                              \
static void _ ## T ## _once_init(void)

/**
 ** @brief Protect the following block or statement with @c
 ** pthread_mutex_t @a mut.
 **/
#define CRITICAL(mut)                                   \
for (int _crit = 1;                                     \
     (pthread_mutex_lock(&(mut)), _crit);               \
     _crit = 0)                                         \
  for (; _crit;                                         \
       (pthread_mutex_unlock(&(mut)), _crit = 0))

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
 **
 ** @param T should be a type.
 **/
#define INIT_ONCE_UPON(T, N)                    \
do {                                            \
  if (!(N))                                     \
    CRITICAL(_ ## T ## _once.mut)               \
      if (!(N)) _ ## T ## _once.init();         \
 } while(0)

/**
 ** @brief Ensure that the function that was defined with
 ** DEFINE_ONCE() has been called exactly once before further
 ** proceeding.
 **
 ** @param T should be a type.
 **/
#define INIT_ONCE(T)                            \
do {                                            \
  if (!(_ ## T ## _once.cond))                  \
    CRITICAL(_ ## T ## _once.mut)               \
      if (!(_ ## T ## _once.cond)) {            \
        _ ## T ## _once.init();                 \
        _ ## T ## _once.cond = 1;               \
      }                                         \
 } while(0)


#endif 	    /* !ORWL_ONCE_H_ */
