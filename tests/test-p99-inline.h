/*
** test-p99-inline.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Thu Nov 25 20:26:09 2010 Jens Gustedt
** Last update Thu Nov 25 20:26:09 2010 Jens Gustedt
*/

#ifndef   	TEST_P99_INLINE_H_
# define   	TEST_P99_INLINE_H_

#include <stdio.h>

#define BODY { printf("%s\n", __func__); }

/* simple inline function, may or may not be emitted */
inline void p00_inl(void) BODY

/* static inline function, must not be emitted */
static inline void p00_sinl(void) BODY

/* extern inline function without other declaration, must not be emitted */
extern inline void p00_einl(void) BODY

/* extern inline function with other declaration, must be emitted */
extern inline void p00_einl_p(void) BODY

/* extern inline function with inline declaration, must be emitted */
extern inline void p00_einl_inl(void) BODY

/* inline function with other extern inline declaration, must be emitted */
inline void p00_inl_einl(void) BODY

/* inline function with other "bare" declaration, must be emitted */
inline void p00_inl_p(void) BODY

#endif 	    /* !TEST_P99_INLINE_H_ */
