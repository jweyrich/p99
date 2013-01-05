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
 ** @brief Implement initialization functions that are executed early.
 **
 ** This is similar to gcc's constructor attribute.
 **
 **/


#define P99_INIT_1
#define P99_INIT_2
#define P99_INIT_3

#define P99_INIT_NR P99_PASTE3(P99_INIT_3, P99_INIT_2, P99_INIT_1)

#define P00_INIT_FUNCTION(NR)  P99_IF_EMPTY(NR)(P99_PASTE2(p00_init_function_, P99_INIT_NR))(P99_PASTE2(p00_init_function_, NR))

#define P00_INIT_FUNC_VAR_S(_0, _1, I)          \
static void (*const P99_PASTE2(p00_init_function_, I))(void); \
static void * P99_PASTE2(p00_init_var_, I)

P99_FOR(, P99_MAX_NUMBER, P00_SEP, P00_INIT_FUNC_VAR_S, P99_REP(P99_MAX_NUMBER,));

p99_inline
void p00_init_function_dummy(void) { }

/* gcc is particularly bad in optimizing this code, go figure */
#if P99_COMPILER & P99_COMPILER_GNU
# define P00_INIT_TRIGGER_FUNCTION_1(_0, _1, I)                          \
if (__builtin_constant_p(!!P99_PASTE2(p00_init_function_, I)) && !!P99_PASTE2(p00_init_function_, I)) \
    P99_PASTE2(p00_init_function_, I)()
#else
# define P00_INIT_TRIGGER_FUNCTION_1(_0, _1, I)                          \
if (P99_PASTE2(p00_init_function_, I)) P99_PASTE2(p00_init_function_, I)()
#endif

#define P00_INIT_TRIGGER_FUNCTION_2(_0, _1, I) (void)P99_PASTE2(p00_init_var_, I)

#define P00_INIT_FUNCTION_(NAME, NR)                    \
void NAME(void);                                        \
static void (*const P00_INIT_FUNCTION(NR))(void) = NAME

#define P99_INIT_FUNCTION_DECLARE(...) P99_IF_EQ(P99_NARG(__VA_ARGS__), 2)(P00_INIT_FUNCTION_(__VA_ARGS__))(P00_INIT_FUNCTION_(__VA_ARGS__,))


#if defined(P99_INTERCEPT_MAIN) || defined(P00_DOXYGEN)

#define P99_MAIN_INTERCEPT(NAME)                                        \
int NAME(int, char*[]);                                                 \
P99_WEAK(P99_PASTE2(p00_init_func_, NAME))                              \
 void P99_PASTE2(p00_init_func_, NAME)(int*, char***);                  \
P99_WEAK(main)                                                          \
int main(int p00_argc, char**p00_argv) {                                \
  fprintf(stderr, "%s: intercepting " P99_STRINGIFY(NAME) "\n", __func__); \
  P99_PASTE2(p00_init_func_, NAME)(&p00_argc, &p00_argv);               \
  return NAME(p00_argc, p00_argv);                                      \
}                                                                       \
P99_WEAK(P99_PASTE2(p00_init_func_, NAME))                              \
void P99_PASTE2(p00_init_func_, NAME)(int * p00_argc, char***p00_argv)

#define P99_INIT_TRIGGER(NAME, ARGC, ARGV) P99_NOP

#  else

#define P99_MAIN_INTERCEPT(NAME)                                        \
P99_WEAK(P99_PASTE2(p00_init_func_, NAME))                              \
void P99_PASTE2(p00_init_func_, NAME)(int * p00_argc, char***p00_argv)

#define P99_INIT_TRIGGER(NAME, ARGC, ARGV) P99_PASTE2(p00_init_func_, NAME)((ARGC), (ARGV))

# endif

P99_MAIN_INTERCEPT(p99_init_main) {
  P99_FOR(, P99_MAX_NUMBER, P00_SEP, P00_INIT_TRIGGER_FUNCTION_1, P99_REP(P99_MAX_NUMBER,));
  P99_FOR(, P99_MAX_NUMBER, P00_SEP, P00_INIT_TRIGGER_FUNCTION_2, P99_REP(P99_MAX_NUMBER,));
}

# if defined(P99_INTERCEPT_MAIN)
#  undef main
#  define main p99_init_main
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
