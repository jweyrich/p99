/*
** p99_map.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Jul 13 22:46:49 2010 Jens Gustedt
** Last update Tue Jul 13 22:46:49 2010 Jens Gustedt
*/

#ifndef   	P99_MAP_H_
# define   	P99_MAP_H_

#include "p99_if.h"
#include "p99_for.h"

/** @addtogroup list_processing
 ** @{
 **/



#define P99__ACCESSOR(NAME, X, I) (NAME)[I]
#define P99__VASSIGN(NAME, X, I) X = P99__ACCESSOR(NAME, X, I)
#define P99__STRLEN(NAME, X, I) strlen(X)
#define P99__TYPD(NAME, X, I) typedef X P99_PASTE2(NAME, I)

#define P99__STRCAT(NAME, I, REC, X) strcat(REC, X)



#define P99__STRLENS(N, ...) P99_FOR(,N, P99__SUM, P99__STRLEN, __VA_ARGS__)

/**
 ** @brief Return an expression that returns the sum of the lengths of
 ** all strings that are given as arguments.
 **/
#define P99_STRLENS(...) P99__STRLENS(P99_NARG(__VA_ARGS__),__VA_ARGS__)


#define P99__STRCATS(N, ...) P99_FOR(,N, P99__STRCAT, P99__IDT, __VA_ARGS__)

/**
 ** @brief Append all argument strings after @a TARG to @a TARG.
 **
 ** @a TARG must be compatible with @c char* and provide enough space
 ** to hold the concatenation of all strings. The remaining arguments
 ** must be compatible with @c const char*.
 **/
#define P99_STRCATS(TARG, ...) P99__STRCATS(P99_NARG(TARG, __VA_ARGS__), TARG, __VA_ARGS__)

/**
 ** @brief Concatenate all arguments.
 **
 ** All arguments must be strings for which the length can be computed
 ** at compile time.
 **
 ** @return a temporary string that is valid in the containing block
 ** of the expression holding the call to this macro.
 **
 ** @see P99_STRDUP for a variant that returns a @c malloc'ed string and
 ** thus can be called with any type of @c char* arguments.
 **/
#define P99_JOIN(...) P99_STRCATS((char[P99_STRLENS(__VA_ARGS__) + 1]){ 0 }, __VA_ARGS__)

/**
 ** @brief Concatenate all arguments.
 **
 ** @return a string that must be freed by @c free
 **/
#define P99_STRDUP(...) P99_STRCATS(memset(malloc(P99_STRLENS(__VA_ARGS__) + 1), 0, 1), __VA_ARGS__)

/**
 ** Repeat the parameter @a X @a N times.
 **/
#define P99_REPS(X, N) P99_FOR(X, N, P99__SEQ, P99__NAM, )

/**
 ** @brief Produce a list of length @a N that has the contents of 0,
 ** 1, , @a N-1
 **/
#define P99_POSS(N) P99_FOR(,N, P99__SEQ, P99__POS,)

/**
 ** Produce a list of length @a N that has the contents of @a X[0], @a
 ** X [1], ,
 ** @a X[@a N-1]
 **/
#define P99_ACCESSORS(X, N) P99_FOR(X, N, P99__SEQ, P99__ACCESSOR, )


/**
 ** @brief Vector-assign to a list
 **
 ** Produce a list of length @c N that has the contents of @c V0 = @a
 ** NAME[0], @c V1 = @a NAME[1], ..., @c VN-1 = @a NAME[@a N-1], where
 ** V0, etc are the remaining arguments.
 **/
#define P99_VASSIGNS(NAME, ...)                                             \
P99_IF_DEC_LT(P99_NARG(__VA_ARGS__),2)                                  \
(P99_IF_VOID(__VA_ARGS__)((void)0)(__VA_ARGS__ = (NAME)[0]))            \
  (P99_FOR(NAME, P99__NARG(__VA_ARGS__),P99__SEP, P99__VASSIGN, __VA_ARGS__))

#define P99__TYPEDEFS(NAME, N, ...)                     \
  P99_IF_VOID(__VA_ARGS__)                              \
  (enum { P99_PASTE3(NAME, _eat_the_semicolon_, N) })   \
  (P99_FOR(NAME, N, P99__SEP, P99__TYPD, __VA_ARGS__))

/**
 ** @brief Take each argument of the list and transform it into a
 ** @c typedef of name NAME_0, NAME_1, etc.
 **
 ** Because of syntax problems this can't be used for function or
 ** array type derivatives.
 **/
#define P99_TYPEDEFS(NAME, ...)                             \
P99__TYPEDEFS(NAME, P99_NARG(__VA_ARGS__), __VA_ARGS__)



/** @}
 **/


#endif 	    /* !P99_MAP_H_ */
