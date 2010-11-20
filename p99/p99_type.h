/*
** p99_type.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Nov 20 09:09:33 2010 Jens Gustedt
** Last update Sat Nov 20 09:09:33 2010 Jens Gustedt
*/

#ifndef   	P99_TYPE_H_
# define   	P99_TYPE_H_

/**
 ** @brief forward declaration of a @c struct @a NAME
 **
 ** This combines a @c struct forward declaration and a @c typedef
 ** that should allow you to use @c struct @a NAME and just @a NAME
 ** interchangeably.
 **
 ** This declaration should also be suitable to be included into a C++
 ** source.
 **/
#define DECLARE_STRUCT(NAME) typedef struct NAME NAME


/**
 ** @brief forward declaration of a @c union @a NAME
 **
 ** This combines a @c union forward declaration and a @c typedef
 ** that should allow you to use @c union @a NAME and just @a NAME
 ** interchangeably.
 **
 ** This declaration should also be suitable to be included into a C++
 ** source.
 **/
#define DECLARE_UNION(NAME) typedef union NAME NAME


#endif 	    /* !P99_TYPE_H_ */
