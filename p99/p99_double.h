/*
** p99_double.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Aug 23 22:58:44 2010 Jens Gustedt
** Last update Mon Aug 23 22:58:44 2010 Jens Gustedt
*/

#ifndef   	P99_DOUBLE_H_
# define   	P99_DOUBLE_H_

#include "p99_if.h"

/**
 ** @file
 ** @brief Build decimal or hexadecimal @c double constants with macros
 **/

/**
 ** @addtogroup double_constants  Decimal or hexadecimal double constants
 **
 ** Since not all substrings of a double constant form a valid
 ** preprocessor token, construction of @c double constants must be
 ** done carefully when done with macros. The macros here guarantee
 ** that this is done in a portable way.
 ** @{
 */

#define P99__DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP, ...)       \
  P99_IF_EMPTY(SIGN)(+)(SIGN)P99__SKIP_ P99_PASTE(              \
  P99_IF_EMPTY(INT)(0)(INT),                                    \
  .,                                                            \
  P99_IF_EMPTY(FRAC)(0)(FRAC),                                  \
  E,                                                            \
  P99_IF_EMPTY(ESIGN)(+)(ESIGN),                                \
  P99_IF_EMPTY(EXP)(0)(EXP),                                    \
  __VA_ARGS__)

#ifdef DOXYGEN
/**
 ** @brief construct a @c double constant in decimal representation.
 **
 ** @param SIGN the sign of the constant, (+)
 ** @param INT the integer part, (0)
 ** @param FRAC the fractional part, (0)
 ** @param ESIGN sign of the exponent, (+)
 ** @param EXP exponent in decimal, (0)
 **
 ** So if all parameters are empty such as
 ** @code
 ** P99_DEC_DOUBLE()
 ** P99_DEC_DOUBLE(,,,,)
 ** @endcode
 ** this produces @c +0.0E+0
 **/
#define P99_DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP)
#else
#define P99_DEC_DOUBLE(...)                     \
  P99_IF_DEC_GE(P99_NARG(__VA_ARGS__), 6)       \
  (P99__DEC_DOUBLE(__VA_ARGS__))                \
  (P99__DEC_DOUBLE(__VA_ARGS__,,,,,))
#endif


#define P99__HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP, ...)      \
  P99_IF_EMPTY(SIGN)(+)(SIGN)P99__SKIP_ P99_PASTE(                      \
  0x,                                                                   \
  P99_IF_EMPTY(HEXINT)(0)(HEXINT),                                      \
  .,                                                                    \
  P99_IF_EMPTY(HEXFRAC)(0)(HEXFRAC),                                    \
  P,                                                                    \
  P99_IF_EMPTY(ESIGN)(+)(ESIGN),                                        \
  P99_IF_EMPTY(BINEXP)(0)(BINEXP),                                      \
  __VA_ARGS__)

#ifdef DOXYGEN
/**
 ** @brief construct a @c double constant in hexadecimal representation.
 **
 ** @see P99_DEC_DOUBLE for the analog with decimal representation
 **/
#define P99_HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP)
#else
#define P99_HEX_DOUBLE(...)                     \
  P99_IF_DEC_GE(P99_NARG(__VA_ARGS__), 6)       \
  (P99__HEX_DOUBLE(__VA_ARGS__))                \
  (P99__HEX_DOUBLE(__VA_ARGS__,,,,,))
#endif

/**
 ** @}
 */


#endif 	    /* !P99_DOUBLE_H_ */
