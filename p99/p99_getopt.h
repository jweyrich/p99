/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_GETOPT_H_
# define    P99_GETOPT_H_

/**
 ** @file
 ** @brief Providing macros that handle variadic macro argument lists.
 **/

#include "p99_init.h"
#include "p99_c99_default.h"

typedef size_t p00_getopt_process_type(void*, char const*);

struct p00_getopt {
  void* p00_o;
  p00_getopt_process_type* p00_f;
};

#define P00_GETOPT_SIGNED(T)                                            \
static_inline                                                              \
size_t P99_PASTE2(p00_getopt_process_, T)(void* p00_o, char const* p00_c) { \
  T*p00_O = p00_o;                                                      \
  if (p00_c && p00_c[0]) {                                              \
    char* endptr = 0;                                                   \
    *p00_O = strtoll(p00_c, &endptr, 0);                                \
    if (endptr) return strlen(p00_c);                                   \
  }                                                                     \
  return 0;                                                             \
}

P99_SER(P00_GETOPT_SIGNED,                      \
        schar,                                  \
        short,                                  \
        int,                                    \
        long,                                   \
        llong                                   \
)

#define P00_GETOPT_UNSIGNED(T)                                          \
static_inline                                                              \
size_t P99_PASTE2(p00_getopt_process_, T)(void* p00_o, char const* p00_c) { \
  T*p00_O = p00_o;                                                      \
  if (p00_c && p00_c[0]) {                                              \
    char* endptr = 0;                                                   \
    *p00_O = strtoull(p00_c, &endptr, 0);                               \
    if (endptr) return strlen(p00_c);                                   \
  }                                                                     \
  return 0;                                                             \
}

P99_SER(P00_GETOPT_UNSIGNED,                    \
        _Bool,                                  \
        uchar,                                  \
        ushort,                                 \
        unsigned,                               \
        ulong,                                  \
        ullong)

static_inline
size_t P99_PASTE2(p00_getopt_process_, char)(void* p00_o, char const*p00_c) {
  char*p00_O = p00_o;
  if (p00_c && p00_c[0]) {
    *p00_O = p00_c[0];
    return 1;
  }
  return 0;
}

static_inline
size_t P99_PASTE2(p00_getopt_process_, char_cptr)(void* p00_o, char const*p00_c) {
  char const**p00_O = p00_o;
  if (p00_c) {
    *p00_O = p00_c;
    return strlen(p00_c);
  }
  return 0;
}

#define P00_GETOPT_FLOAT(T)                                             \
static_inline                                                              \
size_t P99_PASTE2(p00_getopt_process_, T)(void* p00_o, char const* p00_c) { \
  T*p00_O = p00_o;                                                      \
  if (p00_c && p00_c[0]) {                                              \
    char* endptr = 0;                                                   \
    *p00_O = strtold(p00_c, &endptr);                                   \
    if (endptr) return strlen(p00_c);                                   \
  }                                                                     \
  return 0;                                                             \
}

P99_SER(P00_GETOPT_FLOAT,                       \
        float,                                  \
        double,                                 \
        ldouble)


#define P00_GETOPT_PROCESS_CHOOSE_(T) (T, P99_PASTE2(p00_getopt_process_, T))

#define P00_GETOPT_PROCESS_CHOOSE(...) P99_SEQ(P00_GETOPT_PROCESS_CHOOSE_, __VA_ARGS__)

#define P00_GETOPT_DECLARE(CHAR, T, NAME, DEF, ...)                     \
  extern T NAME;                                                        \
  static struct p00_getopt const* p00_getopt_char_p00## CHAR            \
  = &(struct p00_getopt const){                                         \
    .p00_o =  &(NAME),                                                  \
    .p00_f = P99_GENERIC(NAME, 0, __VA_ARGS__)                          \
  }

#define P00_GETOPT_DECLARE_(...) P00_GETOPT_DECLARE(__VA_ARGS__)

#define P99_GETOPT_DECLARE(CHAR, T, ...)                                \
P00_GETOPT_DECLARE_(CHAR,                                               \
                    T,                                                  \
                   P99_IF_LT(P99_NARG(__VA_ARGS__), 2)(__VA_ARGS__, 0)(__VA_ARGS__), \
                   P00_GETOPT_PROCESS_CHOOSE(P99_STD_REAL_TYPES, char_cptr))


#define P00_GETOPT_DEFINE(CHAR, T, NAME, DEF)   \
T NAME = (DEF)

#define P99_GETOPT_DEFINE(CHAR, T, ...)         \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_GETOPT_DEFINE(CHAR, T, __VA_ARGS__, 0))    \
(P00_GETOPT_DEFINE(CHAR, T, __VA_ARGS__))


#define P00_GETOPT_STRUCT_DECL(CHAR) static struct p00_getopt const* p00_getopt_char## CHAR

#define P00_GETOPT_CHARS                                \
_p00A, _p00B, _p00C, _p00D, _p00E, _p00F, _p00G,        \
  _p00H, _p00I, _p00J, _p00K, _p00L, _p00M, _p00N,      \
  _p00O, _p00P, _p00Q, _p00R, _p00S, _p00T, _p00U,      \
  _p00V, _p00W, _p00X, _p00Y, _p00Z,                    \
  _p00a, _p00b, _p00c, _p00d, _p00e, _p00f, _p00g,      \
  _p00h, _p00i, _p00j, _p00k, _p00l, _p00m, _p00n,      \
  _p00o, _p00p, _p00q, _p00r, _p00s, _p00t, _p00u,      \
  _p00v, _p00w, _p00x, _p00y, _p00z,                    \
  _p000, _p001, _p002, _p003, _p004, _p005, _p006,      \
  _p007, _p008, _p009, _p00_


enum {
  p00_getopt_enum_p00A = 'A',
  p00_getopt_enum_p00B = 'B',
  p00_getopt_enum_p00C = 'C',
  p00_getopt_enum_p00D = 'D',
  p00_getopt_enum_p00E = 'E',
  p00_getopt_enum_p00F = 'F',
  p00_getopt_enum_p00G = 'G',
  p00_getopt_enum_p00H = 'H',
  p00_getopt_enum_p00I = 'I',
  p00_getopt_enum_p00J = 'J',
  p00_getopt_enum_p00K = 'K',
  p00_getopt_enum_p00L = 'L',
  p00_getopt_enum_p00M = 'M',
  p00_getopt_enum_p00N = 'N',
  p00_getopt_enum_p00O = 'O',
  p00_getopt_enum_p00P = 'P',
  p00_getopt_enum_p00Q = 'Q',
  p00_getopt_enum_p00R = 'R',
  p00_getopt_enum_p00S = 'S',
  p00_getopt_enum_p00T = 'T',
  p00_getopt_enum_p00U = 'U',
  p00_getopt_enum_p00V = 'V',
  p00_getopt_enum_p00W = 'W',
  p00_getopt_enum_p00X = 'X',
  p00_getopt_enum_p00Y = 'Y',
  p00_getopt_enum_p00Z = 'Z',
  p00_getopt_enum_p00a = 'a',
  p00_getopt_enum_p00b = 'b',
  p00_getopt_enum_p00c = 'c',
  p00_getopt_enum_p00d = 'd',
  p00_getopt_enum_p00e = 'e',
  p00_getopt_enum_p00f = 'f',
  p00_getopt_enum_p00g = 'g',
  p00_getopt_enum_p00h = 'h',
  p00_getopt_enum_p00i = 'i',
  p00_getopt_enum_p00j = 'j',
  p00_getopt_enum_p00k = 'k',
  p00_getopt_enum_p00l = 'l',
  p00_getopt_enum_p00m = 'm',
  p00_getopt_enum_p00n = 'n',
  p00_getopt_enum_p00o = 'o',
  p00_getopt_enum_p00p = 'p',
  p00_getopt_enum_p00q = 'q',
  p00_getopt_enum_p00r = 'r',
  p00_getopt_enum_p00s = 's',
  p00_getopt_enum_p00t = 't',
  p00_getopt_enum_p00u = 'u',
  p00_getopt_enum_p00v = 'v',
  p00_getopt_enum_p00w = 'w',
  p00_getopt_enum_p00x = 'x',
  p00_getopt_enum_p00y = 'y',
  p00_getopt_enum_p00z = 'z',
  p00_getopt_enum_p000 = '0',
  p00_getopt_enum_p001 = '1',
  p00_getopt_enum_p002 = '2',
  p00_getopt_enum_p003 = '3',
  p00_getopt_enum_p004 = '4',
  p00_getopt_enum_p005 = '5',
  p00_getopt_enum_p006 = '6',
  p00_getopt_enum_p007 = '7',
  p00_getopt_enum_p008 = '8',
  p00_getopt_enum_p009 = '9',
  p00_getopt_enum_p00_ = '_',
};


P99_SEP(P00_GETOPT_STRUCT_DECL, P00_GETOPT_CHARS);

#define P00_GETOPT_INITIALIZE_(CHAR)                                    \
  case p00_getopt_enum## CHAR: {                                        \
    struct p00_getopt const* p00_p = p00_getopt_char## CHAR;            \
    if (p00_p) {                                                        \
      void* p00_o = p00_p->p00_o;                                       \
      p00_used = p00_p->p00_f(p00_o, p00_str);                          \
      break;                                                            \
    } else goto P00_REARANGE;                                           \
  }

#define P00_GETOPT_INITIALIZE(...) P99_SER(P00_GETOPT_INITIALIZE_, __VA_ARGS__)

P99_MAIN_INTERCEPT(p99_getopt_initialize) {
  int p00_arg = 1;
  for (; p00_arg < *p00_argc && (*p00_argv)[p00_arg]; ++p00_arg) {
    if ((*p00_argv)[p00_arg][0] != '-') {
      break;
    } else {
      if ((*p00_argv)[p00_arg][1] == '-' && !(*p00_argv)[p00_arg][1]) {
        ++p00_arg;
        break;
      }
      for (char const* p00_str = (*p00_argv)[p00_arg] + 1; p00_str && p00_str[0];) {
        size_t p00_used = 0;
        bool p00_extra = 0;
        char p00_C = p00_str[0];
        ++p00_str;
        if (!p00_str[0]) {
          p00_str = (*p00_argv)[p00_arg + 1];
          if (p00_str) p00_extra = true;
        }
        switch (p00_C) {
          P00_GETOPT_INITIALIZE(P00_GETOPT_CHARS)
        default: goto P00_REARANGE;
        }
        if (p00_used) {
          p00_arg += p00_extra;
          break;
        }
      }
    }
  }
 P00_REARANGE:
  *p00_argc -= (p00_arg - 1);
  for (int p00_n = 1; p00_n < *p00_argc; ++p00_n) {
    (*p00_argv)[p00_n] = (*p00_argv)[p00_n + (p00_arg - 1)];
  }
}

# if defined(P99_INTERCEPT_MAIN)
#  undef main
#  define main p99_getopt_initialize
# endif


#endif      /* !P99_GETOPT_H_ */
