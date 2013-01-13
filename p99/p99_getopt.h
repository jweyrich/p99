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

typedef int p00_getopt_process_type(void*, char const*);

struct p00_getopt {
  void* p00_o;
  p00_getopt_process_type* p00_f;
  char const* p00_a;
  char const* p00_d;
  char const* p00_t;
  char const* p00_n;
  char const* p00_v;
};

static_inline
int p00_getopt_comp(void const* p00_a, void const* p00_b) {
  struct p00_getopt const*const* p00_A = p00_a;
  struct p00_getopt const*const* p00_B = p00_b;
  if (p00_A && (*p00_A) && (*p00_A)->p00_a)
    if (p00_B && (*p00_B) && (*p00_B)->p00_a)
      return strcmp((*p00_A)->p00_a, (*p00_B)->p00_a);
    else
      return -1;
  else if (p00_B && (*p00_B) && (*p00_B)->p00_a)
    return 1;
  else
    return 0;
}

static_inline
int p00_getopt_subcomp(void const* p00_a, void const* p00_b) {
  struct p00_getopt const*const* p00_A = p00_a;
  struct p00_getopt const*const* p00_B = p00_b;
  if (p00_A && (*p00_A) && (*p00_A)->p00_a)
    if (p00_B && (*p00_B) && (*p00_B)->p00_a) {
      size_t p00_n = strlen((*p00_A)->p00_a);
      return strncmp((*p00_A)->p00_a, (*p00_B)->p00_a, p00_n);
    } else
      return -1;
  else if (p00_B && (*p00_B) && (*p00_B)->p00_a)
    return 1;
  else
    return 0;
}


#define P00_GETOPT_SIGNED(T)                                            \
static_inline                                                           \
int P99_PASTE2(p00_getopt_process_, T)(void* p00_o, char const* p00_c) { \
  T*p00_O = p00_o;                                                      \
  if (p00_c && p00_c[0]) {                                              \
    char* endptr = 0;                                                   \
    *p00_O = strtoll(p00_c, &endptr, 0);                                \
    if (endptr) return strlen(p00_c);                                   \
  }                                                                     \
  return -1;                                                            \
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
int P99_PASTE2(p00_getopt_process_, T)(void* p00_o, char const* p00_c) { \
  T*p00_O = p00_o;                                                      \
  if (p00_c && p00_c[0]) {                                              \
    char* endptr = 0;                                                   \
    *p00_O = strtoull(p00_c, &endptr, 0);                               \
    if (endptr) return strlen(p00_c);                                   \
  }                                                                     \
  return -1;                                                            \
}

P99_SER(P00_GETOPT_UNSIGNED,                    \
        uchar,                                  \
        ushort,                                 \
        unsigned,                               \
        ulong,                                  \
        ullong)

static_inline
int P99_PASTE2(p00_getopt_process_, char)(void* p00_o, char const*p00_c) {
  char*p00_O = p00_o;
  if (p00_c && p00_c[0]) {
    *p00_O = p00_c[0];
    return 1;
  }
  return -1;
}

static_inline
int P99_PASTE2(p00_getopt_process_, _Bool)(void* p00_o, char const*p00_c) {
  P99_UNUSED(p00_c);
  bool*p00_O = p00_o;
  *p00_O = !*p00_O;
  return 0;
}

static_inline
int P99_PASTE2(p00_getopt_process_, char_cptr)(void* p00_o, char const*p00_c) {
  char const**p00_O = p00_o;
  if (p00_c) {
    *p00_O = p00_c;
    return strlen(p00_c) + 1;
  }
  return -1;
}

#define P00_GETOPT_FLOAT(T)                                             \
static_inline                                                           \
int P99_PASTE2(p00_getopt_process_, T)(void* p00_o, char const* p00_c) { \
  T*p00_O = p00_o;                                                      \
  if (p00_c && p00_c[0]) {                                              \
    char* endptr = 0;                                                   \
    *p00_O = strtold(p00_c, &endptr);                                   \
    if (endptr) return strlen(p00_c);                                   \
  }                                                                     \
  return -1;                                                            \
}

P99_SER(P00_GETOPT_FLOAT,                       \
        float,                                  \
        double,                                 \
        ldouble)


#define P00_GETOPT_PROCESS_CHOOSE_(T) (T, P99_PASTE2(p00_getopt_process_, T))

#define P00_GETOPT_PROCESS_CHOOSE(...) P99_SEQ(P00_GETOPT_PROCESS_CHOOSE_, __VA_ARGS__)

#define P00_GETOPT_DECLARE(CHAR, T, NAME, DEF, ALIAS, DOC, ...)         \
  extern T NAME;                                                        \
  static struct p00_getopt const* p00_getopt_char_p00## CHAR            \
  = &(struct p00_getopt const){                                         \
    .p00_o =  &(NAME),                                                  \
    .p00_f = P99_GENERIC(NAME, 0, __VA_ARGS__),                         \
    .p00_a = (ALIAS),                                                   \
    .p00_d = (DOC),                                                     \
    .p00_t = #T,                                                        \
    .p00_n = #NAME,                                                     \
    .p00_v = #DEF,                                                      \
  }

#define P00_GETOPT_DECLARE_(...) P00_GETOPT_DECLARE(__VA_ARGS__)

#define P99_GETOPT_DECLARE(CHAR, T, ...)                                \
P00_GETOPT_DECLARE_(CHAR,                                               \
                    T,                                                  \
                    P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                 \
                    (__VA_ARGS__, 0, 0, 0)                              \
                    (P99_IF_LT(P99_NARG(__VA_ARGS__), 3)                \
                     (__VA_ARGS__, 0, 0)                                \
                     (P99_IF_LT(P99_NARG(__VA_ARGS__), 4)               \
                      (__VA_ARGS__, 0)                                  \
                      (__VA_ARGS__))),                                  \
                    P00_GETOPT_PROCESS_CHOOSE(P99_STD_REAL_TYPES, char_cptr))


#define P00_GETOPT_DEFINE(CHAR, T, NAME, DEF, ALIAS, DOC)       \
T NAME = (DEF)

#define P99_GETOPT_DEFINE(CHAR, T, ...)                 \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                     \
(P00_GETOPT_DEFINE(CHAR, T, __VA_ARGS__, 0, 0, 0))      \
(P99_IF_LT(P99_NARG(__VA_ARGS__), 3)                    \
 (P00_GETOPT_DEFINE(CHAR, T, __VA_ARGS__, 0, 0))        \
 (P99_IF_LT(P99_NARG(__VA_ARGS__), 4)                   \
  (P00_GETOPT_DEFINE(CHAR, T, __VA_ARGS__, 0))          \
  (P00_GETOPT_DEFINE(CHAR, T, __VA_ARGS__))))


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
  _p007, _p008, _p009, _p00_,                           \
  _p00AMPERSAND,                                        \
  _p00APOSTROPHE,                                       \
  _p00ASTERISK,                                         \
  _p00AT,                                               \
  _p00BACKSLASH,                                        \
  _p00BAR,                                              \
  _p00BRACELEFT,                                        \
  _p00BRACERIGHT,                                       \
  _p00BRACKETLEFT,                                      \
  _p00BRACKETRIGHT,                                     \
  _p00CARRET,                                           \
  _p00COLON,                                            \
  _p00COMMA,                                            \
  _p00DOLLAR,                                           \
  _p00EQUAL,                                            \
  _p00EXLAM,                                            \
  _p00GRAVE,                                            \
  _p00GREATER,                                          \
  _p00HASH,                                             \
  _p00HELP,                                             \
  _p00LESS,                                             \
  _p00PARENLEFT,                                        \
  _p00PARENRIGHT,                                       \
  _p00PERCENT,                                          \
  _p00PERIOD,                                           \
  _p00PLUS,                                             \
  _p00QUOTEDBL,                                         \
  _p00SEMICOLON,                                        \
  _p00SLASH,                                            \
  _p00TILDE


enum p00_getopt_enum {
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
  p00_getopt_enum_p00AMPERSAND = '&',
  p00_getopt_enum_p00APOSTROPHE = '\'',
  p00_getopt_enum_p00ASTERISK = '*',
  p00_getopt_enum_p00AT = '@',
  p00_getopt_enum_p00BACKSLASH = '\\',
  p00_getopt_enum_p00BAR = '|',
  p00_getopt_enum_p00BRACELEFT = '{',
  p00_getopt_enum_p00BRACERIGHT = '}',
  p00_getopt_enum_p00BRACKETLEFT = '[',
  p00_getopt_enum_p00BRACKETRIGHT = ']',
  p00_getopt_enum_p00CARRET = '^',
  p00_getopt_enum_p00COLON = ':',
  p00_getopt_enum_p00COMMA = ',',
  p00_getopt_enum_p00DOLLAR = '$',
  p00_getopt_enum_p00EQUAL = '=',
  p00_getopt_enum_p00EXLAM = '!',
  p00_getopt_enum_p00GRAVE = '`',
  p00_getopt_enum_p00GREATER = '>',
  p00_getopt_enum_p00HASH = '#',
  p00_getopt_enum_p00HELP = '?',
  p00_getopt_enum_p00LESS = '<',
  p00_getopt_enum_p00PARENLEFT = '(',
  p00_getopt_enum_p00PARENRIGHT = ')',
  p00_getopt_enum_p00PERCENT = '%',
  p00_getopt_enum_p00PERIOD = '.',
  p00_getopt_enum_p00PLUS = '+',
  p00_getopt_enum_p00QUOTEDBL = '"',
  p00_getopt_enum_p00SEMICOLON = ';',
  p00_getopt_enum_p00SLASH = '/',
  p00_getopt_enum_p00TILDE = '~',
};

P99_SEP(P00_GETOPT_STRUCT_DECL, P00_GETOPT_CHARS);

#define P00_GETOPT_HELP_COUNT_(CHAR)                                    \
do {                                                                    \
  struct p00_getopt const* p00_p = p00_getopt_char## CHAR;              \
  if (p00_p) {                                                          \
    if (p00_p->p00_a) p00_na = P99_GEN_MAX(p00_na, strlen(p00_p->p00_a)); \
    if (p00_p->p00_t) p00_nt = P99_GEN_MAX(p00_nt, strlen(p00_p->p00_t)); \
    if (p00_p->p00_n) p00_nn = P99_GEN_MAX(p00_nn, strlen(p00_p->p00_n)); \
    if (p00_p->p00_v) p00_nv = P99_GEN_MAX(p00_nv, strlen(p00_p->p00_v)); \
  }                                                                     \
} while(false)

#define P00_GETOPT_HELP_COUNT(...) P99_SEP(P00_GETOPT_HELP_COUNT_, __VA_ARGS__)

#define P00_GETOPT_HELP_(CHAR)                                          \
do {                                                                    \
  struct p00_getopt const* p00_p = p00_getopt_char## CHAR;              \
  if (p00_p) {                                                          \
    char const* p00_d = p00_p->p00_d ? p00_p->p00_d : "(not documented)"; \
    char const* p00_t = p00_p->p00_t ? p00_p->p00_t : "";               \
    char const* p00_n = p00_p->p00_n ? p00_p->p00_n : "";               \
    char const* p00_v = p00_p->p00_v ? p00_p->p00_v : "";               \
    if (p00_p->p00_a)                                                   \
      fprintf(stderr, "   -%c  --%-*s%-*s%-*s%-*s\t%s\n",               \
              p00_getopt_enum## CHAR,                                   \
              p00_na+2,                                                 \
              p00_p->p00_a,                                             \
              p00_nt+2,                                                 \
              p00_t,                                                    \
              p00_nn+2,                                                 \
              p00_n,                                                    \
              p00_nv+2,                                                 \
              p00_v,                                                    \
              p00_d);                                                   \
    else                                                                \
      fprintf(stderr, "   -%c    %-*s%-*s%-*s%-*s \t%s\n",           \
              p00_getopt_enum## CHAR,                                   \
              p00_na+2,                                                 \
              "",                                                       \
              p00_nt+2,                                                 \
              p00_t,                                                    \
              p00_nn+2,                                                 \
              p00_n,                                                    \
              p00_nv+2,                                                 \
              p00_v,                                                    \
              p00_d);                                                   \
  }                                                                     \
 } while(false)

#define P00_GETOPT_HELP(...) P99_SEP(P00_GETOPT_HELP_, __VA_ARGS__)

static_inline
int P99_PASTE2(p00_getopt_process_, help)(void* p00_o, char const*p00_c) {
  int p00_na = 0;
  int p00_nt = 0;
  int p00_nn = 0;
  int p00_nv = 0;
  P00_GETOPT_HELP_COUNT(P00_GETOPT_CHARS);
  if (p00_o) fprintf(stderr, "%s\toptions:\n", (char const*)p00_o);
  fprintf(stderr, "short  %-*s%-*s%-*s%-*s \t%s\n",
          p00_na+4,
          "long",
          p00_nt+2,
          "type",
          p00_nn+2,
          "name",
          p00_nv+2,
          "default",
          "");
  P00_GETOPT_HELP(P00_GETOPT_CHARS);
  exit(EXIT_FAILURE);
}

#define P00_GETOPT_INITIALIZE_(CHAR)                                    \
  case p00_getopt_enum## CHAR: {                                        \
    struct p00_getopt const* p00_p = p00_getopt_char## CHAR;            \
    if (p00_p) {                                                        \
      void* p00_o = p00_p->p00_o;                                       \
      p00_used = p00_p->p00_f(p00_o, p00_str);                          \
      if (p00_used >= 0) break;                                         \
    }                                                                   \
    p00_err0 = "unparsable argument";                                   \
    goto P00_REARANGE;                                                  \
  }

#define P00_GETOPT_INITIALIZE(...) P99_SER(P00_GETOPT_INITIALIZE_, __VA_ARGS__)

#define P00_GETOPT_ARRAY_(CHAR) [p00_getopt_enum## CHAR] = p00_getopt_char## CHAR

#define P00_GETOPT_ARRAY(...) P99_SEQ(P00_GETOPT_ARRAY_, __VA_ARGS__)

P99_MAIN_INTERCEPT(p99_getopt_initialize) {
  char const* p00_err0 = 0;
  char const* p00_err1 = 0;
  char const* p00_err2 = 0;
  /* Create a sorted array with all the aliases, such that we may then
     search for a matching key. */
  struct p00_getopt const* p00_A[CHAR_MAX] = {
    P00_GETOPT_ARRAY(P00_GETOPT_CHARS),
  };
  qsort(p00_A, CHAR_MAX, sizeof *p00_A, p00_getopt_comp);

  struct p00_getopt const** p00_up = p00_A;
  while (*p00_up) ++p00_up;

  bool p00_help = false;
  {
    /* Search for a matching alias in the array */
    struct p00_getopt const* p00_el = &(struct p00_getopt const){ .p00_a = "help", };
    struct p00_getopt const** p00_p = bsearch(&p00_el,
                                              p00_A,
                                              CHAR_MAX,
                                              sizeof *p00_A,
                                              p00_getopt_subcomp);
    if (p00_p && (*p00_p)) {
      /* Now search if there are several matches. */
      while (p00_p != p00_A && !p00_getopt_subcomp(&p00_el, p00_p - 1)) --p00_p;
      /* An exact match must always come first and is preferred.
         If the first is not an exact match, second shouldn't be
         a partial match. */
      p00_help = !p00_getopt_comp(&p00_el, p00_p);
    }
  }

  /* If -h is not taken install a help function on it. */
  struct p00_getopt const p00_h = {
    .p00_o = (*p00_argv)[0],
    .p00_f = p00_getopt_process_help,
    .p00_a = p00_help ? 0 : "help",
    .p00_d = "provide this help text",
  };
  if (!p00_help) {
    *p00_up = &p00_h;
    ++p00_up;
    qsort(p00_A, p00_up - p00_A, sizeof *p00_A, p00_getopt_comp);
  }
  if (!p00_getopt_char_p00HELP) p00_getopt_char_p00HELP = &p00_h;
  if (!p00_getopt_char_p00h) p00_getopt_char_p00h = &p00_h;

  /* Now comes the main processing loop. One character arguments may
     be aggregated into one option, that is why this loop looks a bit
     scary. */
  int p00_arg = 1;
  for (; p00_arg < *p00_argc && (*p00_argv)[p00_arg]; ++p00_arg) {
    /* All options must start with a dash, otherwise this finishes
       option processing. */
    if ((*p00_argv)[p00_arg][0] != '-') {
      goto P00_REARANGE;
    } else {
      /* A "--" without anything else finishes option processing. */
      if ((*p00_argv)[p00_arg][1] == '-' && !(*p00_argv)[p00_arg][2]) {
        ++p00_arg;
        goto P00_REARANGE;;
      }
      for (char const* p00_str = (*p00_argv)[p00_arg] + 1; p00_str && p00_str[0];) {
        int p00_used = 0;
        bool p00_extra = false;
        char p00_C = p00_str[0];
        ++p00_str;
        /* If there was nothing left in the option, take the next one
           as an argument. */
        if (!p00_str[0]) {
          p00_str = (*p00_argv)[p00_arg + 1];
          if (p00_str) p00_extra = true;
        }
        static char p00_err[2];
        p00_err[0] = p00_C;
        p00_err1 = p00_err;
        p00_err2 = p00_str;
        switch (p00_C) {
          /* The cases for the one-character options are hidden
             here. */
          P00_GETOPT_INITIALIZE(P00_GETOPT_CHARS)
            /* If the initial string was "--" this announces a long
               option. */
        case '-': {
            /* First split up the option string into option and
               argument, if possible. */
            char p00_al[strlen(p00_str) + 1];
            strcpy(p00_al, p00_str);
            char* p00_ar = strchr(p00_al, '=');
            if (p00_ar) {
              *p00_ar = 0;
              ++p00_ar;
            } else {
              /* If not, suppose that the argument has been given in
                 the next option. */
              p00_ar = (*p00_argv)[p00_arg + 1];
              p00_extra = true;
            }
            p00_err1 = p00_al;
            p00_err2 = p00_ar;
            /* Search for a matching alias in the array */
            struct p00_getopt const* p00_el = &(struct p00_getopt const){ .p00_a = p00_al, };
            struct p00_getopt const** p00_p = bsearch(&p00_el,
                                                      p00_A,
                                                      CHAR_MAX,
                                                      sizeof *p00_A,
                                                      p00_getopt_subcomp);
            if (p00_p && (*p00_p)) {
              /* Now search if there are several matches. */
              while (p00_p != p00_A && !p00_getopt_subcomp(&p00_el, p00_p - 1)) --p00_p;
              /* An exact match must always come first and is preferred.
                 If the first is not an exact match, second shouldn't be
                 a partial match. */
              if (!p00_getopt_comp(&p00_el, p00_p) || (p00_getopt_subcomp(&p00_el, p00_p+1) < 0)) {
                void* p00_o = (*p00_p)->p00_o;
                p00_used = (*p00_p)->p00_f(p00_o, p00_ar);
                if (p00_used >= 0) break;
                else p00_err0 = "unparsable argument";
              } else p00_err0 = "ambiguous option alias";
            } else p00_err0 = "no matching option alias";
            goto P00_REARANGE;
          }
        }
        /* If this used the option argument and the argument was found
           in the next element of argv, skip that. */
        if (p00_used > 0) {
          p00_arg += p00_extra;
          break;
        } else
          /* Otherwise if this was a flag and we tried to obtain from
             the next element in argv, check that element again, it
             might be an option, too. */
          if (p00_extra) break;
      }
    }
  }
 P00_REARANGE:
  if (p00_err0)
    fprintf(stderr, "Warning:%s for \"--%s\" with \"%s\"\n",
            p00_err0,
            p00_err1,
            p00_err2);
  /* At the end of the processing, shift all unused options down, such
     that they appear at front in the argv array. */
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
