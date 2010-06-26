/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	P99_PASTE_H_
# define   	P99_PASTE_H_

#include "orwl_inline.h"
#include "orwl_default_arg.h"

/* This was inspired by BOOT's PP_CAT macro. Using such a thing avoid
   to define multiple levels of expansion for each macro. */
#define PASTE0()
#define PASTE1(_1) _1

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the paste operation first, and then an evaluation
 ** of the result. Thus
 ** @code
 ** #define B(x) toto(x)
 ** CAT2(A, B(6));
 ** @endcode
 ** will result in
 ** @code
 ** AB(6);
 ** @endcode
 ** and not in
 ** @code
 ** Atoto(6)
 ** @endcode
 **/
#define CAT2(_1, _2) _1 ## _2

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the evaluation of the arguments first and then
 ** proceeds at the concatenation
 ** of the results. Thus
 ** @code
 ** #define B(x) toto(x)
 ** CAT2(A, B(6));
 ** @endcode
 ** will result in
 ** @code
 ** Atoto(6)
 ** @endcode
 ** and not in
 ** @code
 ** AB(6);
 ** @endcode
 **/
#define PASTE2(_1, _2) CAT2(_1, _2)

#define P99___PASTE(F, N, ...) F ## N(__VA_ARGS__)
#define P99__PASTE(N, ...) P99___PASTE(PASTE, N, __VA_ARGS__)


/** @addtogroup list_processing List processing macros
 ** @brief We provide here a series of macros that take a list of
 ** arguments of arbitrary length and that transform each element in
 ** that list in some way.
 ** @{
 **/


/**
 ** @brief A left-to-right associative paste operator.
 **
 ** This macro avoids the ambiguity of the @c ## preprocessor operator
 ** which has no well defined associativity. With this macro here
 ** something like
 ** @code
 ** PASTE(0.1E, -, 1)
 ** @endcode
 ** is guaranteed to produce the token @c 0.1E-1, whereas the
 ** seemingly equivalent
 ** @code
 ** ETSAP(0.1E, -, 1)
 ** @endcode
 ** is not valid: the intermediate operation to paste tokens `-' and
 ** `1' would result in an invalid token and is thus rejected.
 **
 ** This macro does the evaluation of the arguments first and
 ** then proceeds at the concatenation of the results.
 ** @pre the argumentlist should not be empty.
 **/
#define PASTE(...) P99__PASTE(P99__NARG(__VA_ARGS__), __VA_ARGS__)

/** @}
 **/


#endif 	    /* !P99_PASTE_H_ */
