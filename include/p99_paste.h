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
#define PASTE3(_1,	_2,	_3)	\
	PASTE2(PASTE2(_1,	_2), _3)
#define PASTE4(_1,	_2,	_3,	_4)	\
	PASTE2(PASTE3(_1,	_2,	_3), _4)
#define PASTE5(_1,	_2,	_3,	_4,	_5)	\
	PASTE2(PASTE4(_1,	_2,	_3,	_4), _5)
#define PASTE6(_1,	_2,	_3,	_4,	_5,	_6)	\
	PASTE2(PASTE5(_1,	_2,	_3,	_4,	_5), _6)

#endif 	    /* !P99_PASTE_H_ */
