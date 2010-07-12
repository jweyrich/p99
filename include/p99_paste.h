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

#include "p99_generated.h"

/* This was inspired by BOOT's PP_CAT macro. Using such a thing avoid
   to define multiple levels of expansion for each macro. */
#define P99_PASTE0()
#define P99_PASTE1(_1) _1

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the paste operation first, and then an evaluation
 ** of the result. Thus
 ** @code
 ** #define B(x) toto(x)
 ** P99_CAT2(A, B(6));
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
#define P99_CAT2(_1, _2) _1 ## _2

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the evaluation of the arguments first and then
 ** proceeds at the concatenation
 ** of the results. Thus
 ** @code
 ** #define B(x) toto(x)
 ** P99_CAT2(A, B(6));
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
#define P99_PASTE2(_1, _2) P99_CAT2(_1, _2)
#define P99_PASTE3(_1,	_2,	_3)	\
	P99_PASTE2(P99_PASTE2(_1,	_2), _3)
#define P99_PASTE4(_1,	_2,	_3,	_4)	\
	P99_PASTE2(P99_PASTE3(_1,	_2,	_3), _4)
#define P99_PASTE5(_1,	_2,	_3,	_4,	_5)	\
	P99_PASTE2(P99_PASTE4(_1,	_2,	_3,	_4), _5)
#define P99_PASTE6(_1,	_2,	_3,	_4,	_5,	_6)	\
	P99_PASTE2(P99_PASTE5(_1,	_2,	_3,	_4,	_5), _6)

#endif 	    /* !P99_PASTE_H_ */
