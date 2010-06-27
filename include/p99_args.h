/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	P99_ARGS_H_
# define   	P99_ARGS_H_

#include "orwl_inline.h"
#include "p99_paste.h"

/**
 ** @brief Return the length of the variate argument list.
 **
 ** This supposes that the length is less than 64.
 **
 ** Idea is due to Laurent Deniau, CERN, and taken from a discussion
 ** in comp.lang.c.
 **
 ** @warning this also counts an empty argument list as having one (=
 ** the empty) argument
 ** @see NARG for a macro that returns 0 if the list is empty
 **/
#define P99__NARG(...) P99__NARG_1(__VA_ARGS__)

#define P99__IS__EQ__(...) ,

/**
 ** @brief Test if the argument list is empty.
 **
 ** This expands to token 1 if the list was empty and to token 0 if
 ** there was anything different from a comment in the list.
 **
 ** The implementation of this macro is kind of tricky and heavily
 ** uses the fact that a function macro (@c P99__IS__EQ__ in this case) is
 ** left untouched if it is not followed by a parenthesis. See
 ** http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 **
 ** @return tokens 0 or 1
 **/
#define IS_EMPTY(...)                                                   \
P99__ISEMPTY(                                                           \
             /* test if there is just one argument, eventually an empty \
                one */                                                  \
             HAS_COMMA(__VA_ARGS__),                                    \
             /* test if P99_IS__EQ__ together with the argument         \
                adds a comma */                                         \
             HAS_COMMA(P99__IS__EQ__ __VA_ARGS__),                      \
             /* test if the argument together with a parenthesis        \
                adds a comma */                                         \
             HAS_COMMA(__VA_ARGS__ (~)),                                \
             /* test if placing it between P99_IS__EQ__ and the         \
                parenthesis adds a comma */                             \
             HAS_COMMA(P99__IS__EQ__ __VA_ARGS__ (~))                   \
             )

#define P99__ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(P99__IS_EMPTY_CASE_, _0, _1, _2, _3))
#define P99__IS_EMPTY_CASE_0001 ,

#define P99__NARG_EMPTY_1(VAL) 0
#define P99__NARG_EMPTY_0(VAL) VAL

/**
 ** @brief Return the length of the variate argument list, an empty
 ** argument list accounting for 0 arguments.
 **
 ** This supposes that the length of the list is less than 64.
 **
 ** @see P99__NARG for a macro that accounts an empty list to be 1
 **/
#define NARG(...) NARG__1(IS_EMPTY(__VA_ARGS__), P99__NARG(__VA_ARGS__))
#define NARG__1(B, VAL) NARG__2(PASTE2(P99__NARG_EMPTY_, B), VAL)
#define NARG__2(B, VAL) B(VAL)

/**
 ** @brief Detect if two tokens are equal.
 **
 ** These must be alphanumerical tokens composed of [_a-zA-Z_] and to
 ** be able to test for equality of token @c X the macro @c
 ** P99__IS_X_EQ_X(...) must be defined to expand to a comma.
 **/
#define TOK_EQ(TOK, ...)  P99__TOK_EQ_(PASTE3(P99__IS_, TOK, _EQ_), __VA_ARGS__)
#define P99__TOK_EQ_(MAC, ...)  P99__TOK_EQ__(MAC, __VA_ARGS__)
#define P99__TOK_EQ__(MAC, ...) HAS_COMMA(PASTE2(P99__TOK_EQ_, P99__NARG(MAC ## __VA_ARGS__ (~) MAC ## __VA_ARGS__))(~))

#define P99__TOK_EQ_0(...) ~
#define P99__TOK_EQ_1(...) ~
#define P99__TOK_EQ_2(...) ,
#define P99__TOK_EQ_3(...) ~
#define P99__TOK_EQ_4(...) ~

#endif 	    /* !P99_ARGS_H_ */
