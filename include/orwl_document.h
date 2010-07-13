/*
** orwl_document.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Jul  4 16:23:52 2010 Jens Gustedt
** Last update Sun Jul  4 16:23:52 2010 Jens Gustedt
*/

#ifndef   	ORWL_DOCUMENT_H_
# define   	ORWL_DOCUMENT_H_

#define VA_ARGS_DOCUMENTATION(NAME)                                     \
                                                                        \
/*! @see VA_ARGS */                                                     \
/*! This is actually implemented as a macro that helps to provide the length of the variable length argument list to the function. */


#define DOCUMENT_BLOCK /*! @see BLOCK for restrictions on preliminary exits from the dependent block or statement. **/


/**
 ** @brief Add some indications to a @c _init documentation.
 */
#define DOCUMENT_INIT(T)                                                \
/*! @brief Initialize a variable of type T */                           \
/*! @see NEW needs a version of this that takes just the T* as argument. */

/**
 ** @brief Add some indications to a @c _destroy documentation.
 */
#define DOCUMENT_DESTROY(T) /*! @brief Destroy a variable of type T @see T ## _delete needs this. */


#endif 	    /* !ORWL_DOCUMENT_H_ */
