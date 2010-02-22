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

/**
 ** @brief Declare the symbols that are needed for the macro
 ** INIT_ONCE().
 **
 ** @param T should be a type.
 **/
#define DECLARE_ONCE(T)                         \
extern pthread_once_t _ ## T ## _once;          \
void _ ## T ## _once_init(void)


/**
 ** @brief Define the function that will be exactly called once by the
 ** macro INIT_ONCE().
 **
 ** The function has a prototype of void (*name)(void).
 **
 ** @param T should be a type.
 **/
#define DEFINE_ONCE(T)                                  \
pthread_once_t _ ## T ## _once = PTHREAD_ONCE_INIT;     \
void _ ## T ## _once_init(void)

/**
 ** @brief Ensure that the function that was defined with
 ** DEFINE_ONCE() has been called exactly once before further
 ** proceeding.
 **
 ** @param T should be a type.
 **/
#define INIT_ONCE(T)                                    \
do {                                                    \
  pthread_once(&_ ## T ## _once, _ ## T ## _once_init); \
 } while(0)


#endif 	    /* !ORWL_ONCE_H_ */
