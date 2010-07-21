/*
** p99_new.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Jul 21 14:30:16 2010 Jens Gustedt
** Last update Wed Jul 21 14:30:16 2010 Jens Gustedt
*/

#ifndef   	P99_NEW_H_
# define   	P99_NEW_H_

#include <stdlib.h>
#include <string.h>

#include "p99_compiler.h"

/**
 ** @addtogroup preprocessor_initialization Iniitalization facilities throught the preprocessor
 **
 ** @{
 **/

/**
 ** @brief Zero out all bits in the object that @a X points to.
 **
 ** @a X can be a pointer or an array
 ** @code
 ** T *A;
 ** T B[3];
 ** PZERO(A, 29);
 ** PZERO(B, 2);
 ** @endcode
 **
 ** Here the first call zeroes out 29 elements of type T starting at
 ** the memory where @c A points to. The second zeroes out the first
 ** two elements of the array @c B.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 **/
#define P99_PZERO(X, N) (memset((X), 0, sizeof(X[0]) * N))

/**
 ** @brief Zero out all bits in the object @a X.
 **
 ** @a X must be an object from which the address can be taken, such
 ** as a normal variable or an array.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 **/
#define P99_TZERO(X) (memset(&(X), 0, sizeof(X)))


/**
 ** @}
 **/


#define P99__NEW(T) P99_PASTE2(T, _init)((T*)malloc(sizeof(T)))

#define P99__NEW_ARGS(T, ...) P99_PASTE2(T, _init)((T*)malloc(sizeof(T)), __VA_ARGS__)


/**
 ** @addtogroup preprocessor_allocation Allocation facilities throught the preprocessor
 **
 ** @{
 **/

/**
 ** @brief Allocate an element of type @c T as given by the first
 ** argument and initialize it with the remaining arguments, if any.
 **
 ** This supposes that the argument list is of the form
 ** @code
 ** T [, OTHER_PARAMETERS]
 ** @endcode
 ** and that a function or macro named @c T_init exists where
 ** @c T_init is just the concatenation of the type name @c T and
 ** `_init'. Thus @c T must be a typename that consists of just one
 ** token: `signed' would work but `signed long' would not.
 **
 ** The @c T_init function must have a signature that is
 ** compatible to one of
 ** @code
 ** T* T_init(T* p);
 ** T* T_init(T*p, OTHER_PARAMETERS);
 ** @endcode
 **
 ** - it must accept a pointer to @c T as a first argument
 ** - it must be robust to the fact of receiving NULL through that
 **     first argument (indicating an error condition)
 ** - it should return the @i same pointer @c p
 ** - it may accept additional arguments which may then be passed
 **     through from the call to ::P99_NEW
 **
 ** @return value is a pointer to a newly allocated and initialized object of type
 ** @c T, or @c NULL if the allocation failed.
 **
 ** @see P99_CALL_DEFARG if you want to provide default arguments to
 ** the @c T_init function.
 **/
#define P99_NEW(...) P99_IF_DEC_LT(P99_NARG(__VA_ARGS__), 2)(P99__NEW(__VA_ARGS__))(P99__NEW_ARGS(__VA_ARGS__))

/**
 ** @}
 **/

#endif 	    /* !P99_NEW_H_ */
