/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_INIT_H_
# define    P99_INIT_H_

#include "p99_if.h"

/**
 ** @file
 **
 ** @brief Implement a simple FILEID that changes each time this file
 ** is included somewhere
 **
 ** The FILEID here is a hexadecimal number with 4 digits. 34320
 ** different such numbers are produced by the algorithm until it
 ** wraps around.
 **/


#define P99_INIT_1
#define P99_INIT_2
#define P99_INIT_3

#define P99_INIT_NR P99_PASTE3(P99_INIT_3, P99_INIT_2, P99_INIT_1)

#define P00_INIT_FUNCTION(NR)  P99_IF_EMPTY(NR)(P99_PASTE2(p00_init_function_, P99_INIT_NR))(P99_PASTE2(p00_init_function_, NR))

#define P00_INIT_TRIGGER_FUNCTION_0(_0, _1, I)  static void (*P99_PASTE2(p00_init_function_, I))(void)
#define P00_INIT_TRIGGER_FUNCTION_1(_0, _1, I)  if (P99_PASTE2(p00_init_function_, I)) P99_PASTE2(p00_init_function_, I)()

P99_FOR(, P99_MAX_NUMBER, P00_SEP, P00_INIT_TRIGGER_FUNCTION_0, P99_REP(P99_MAX_NUMBER,));

#define P00_INIT_FUNCTION_(NAME, NR)                           \
P99_WEAK(NAME) void NAME(void);                                \
static void (*P00_INIT_FUNCTION(NR))(void) = NAME;             \
P99_WEAK(NAME) void NAME(void)

#define P99_INIT_FUNCTION(...) P99_IF_EQ(P99_NARG(__VA_ARGS__), 2)(P00_INIT_FUNCTION_(__VA_ARGS__))(P00_INIT_FUNCTION_(__VA_ARGS__,))


#if defined(P99_INTERCEPT_MAIN) || defined(P00_DOXYGEN)

int p99_init_main(int, char*[]);

P99_MAIN_INTERCEPT(p99_init_main) {
  P99_FOR(, P99_MAX_NUMBER, P00_SEP, P00_INIT_TRIGGER_FUNCTION_1, P99_REP(P99_MAX_NUMBER,));
}

#undef main
#define main p99_init_main

static inline void p99_init_trigger(void) {
  /* empty */
};

#  else

static inline void p99_init_trigger(void) {
  P99_FOR(, P99_MAX_NUMBER, P00_SEP, P00_INIT_TRIGGER_FUNCTION_1, P99_REP(P99_MAX_NUMBER,));
};

# endif


#endif

#if P99_PASTE2(P99_INIT_1, 0) == 0
# undef P99_INIT_1
# define P99_INIT_1 1
#elif P99_INIT_1 == 1
# undef P99_INIT_1
# define P99_INIT_1 2
#elif P99_INIT_1 == 2
# undef P99_INIT_1
# define P99_INIT_1 3
#elif P99_INIT_1 == 3
# undef P99_INIT_1
# define P99_INIT_1 4
#elif P99_INIT_1 == 4
# undef P99_INIT_1
# define P99_INIT_1 5
#elif P99_INIT_1 == 5
# undef P99_INIT_1
# define P99_INIT_1 6
#elif P99_INIT_1 == 6
# undef P99_INIT_1
# define P99_INIT_1 7
#elif P99_INIT_1 == 7
# undef P99_INIT_1
# define P99_INIT_1 8
#elif P99_INIT_1 == 8
# undef P99_INIT_1
# define P99_INIT_1 9
#elif P99_INIT_1 == 9
# undef P99_INIT_1
# define P99_INIT_1 0
# if P99_PASTE2(P99_INIT_2, 0) == 0
#  undef P99_INIT_2
#  define P99_INIT_2 1
# elif P99_INIT_2 == 1
#  undef P99_INIT_2
#  define P99_INIT_2 2
# elif P99_INIT_2 == 2
#  undef P99_INIT_2
#  define P99_INIT_2 3
# elif P99_INIT_2 == 3
#  undef P99_INIT_2
#  define P99_INIT_2 4
# elif P99_INIT_2 == 4
#  undef P99_INIT_2
#  define P99_INIT_2 5
# elif P99_INIT_2 == 5
#  undef P99_INIT_2
#  define P99_INIT_2 6
# elif P99_INIT_2 == 6
#  undef P99_INIT_2
#  define P99_INIT_2 7
# elif P99_INIT_2 == 7
#  undef P99_INIT_2
#  define P99_INIT_2 8
# elif P99_INIT_2 == 8
#  undef P99_INIT_2
#  define P99_INIT_2 9
# elif P99_INIT_2 == 9
#  undef P99_INIT_2
#  define P99_INIT_2 0
#  if P99_PASTE2(P99_INIT_3, 0) == 0
#   undef P99_INIT_3
#   define P99_INIT_3 1
#  elif P99_INIT_3 == 1
#   undef P99_INIT_3
#   define P99_INIT_3 2
#  elif P99_INIT_3 == 2
#   undef P99_INIT_3
#   define P99_INIT_3 3
#  elif P99_INIT_3 == 3
#   undef P99_INIT_3
#   define P99_INIT_3 4
#  elif P99_INIT_3 == 4
#   undef P99_INIT_3
#   define P99_INIT_3 5
#  elif P99_INIT_3 == 5
#   undef P99_INIT_3
#   define P99_INIT_3 6
#  elif P99_INIT_3 == 6
#   undef P99_INIT_3
#   define P99_INIT_3 7
#  elif P99_INIT_3 == 7
#   undef P99_INIT_3
#   define P99_INIT_3 8
#  elif P99_INIT_3 == 8
#   undef P99_INIT_3
#   define P99_INIT_3 9
#  elif P99_INIT_3 == 9
#   error "more than 999 init functions, compilation aborted"
#  endif
# endif
#endif

#if P99_INIT_NR > P99_MAX_NUMBER
# error "more init functions than supported through P99_MAX_NUMBER, compilation aborted"
#endif
