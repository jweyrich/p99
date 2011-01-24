/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
/*
** orwl_posix.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Nov 20 14:59:49 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_posix.h"
#include "p99_id.h"

#define P00_DEFINE_SC_1_(NAME, X, I) LONG_MIN

#define P00_DEFINE_SC_1(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEQ, P00_DEFINE_SC_1_, __VA_ARGS__)

static long p00_sc_constant_[P00_POS_SC_MAX] = {
  P00_DEFINE_SC_1(P00_POSIX_SC_1),
  P00_DEFINE_SC_1(P00_POSIX_SC_2),
#ifdef _SC_XOPEN_THREADS
  P00_DEFINE_SC_1(P00_POSIX_SC_THREAD),
#endif
#ifdef _SC_XOPEN_VERSION
  P00_DEFINE_SC_1(P00_POSIX_SC_XOPEN),
#endif
#ifdef _SC_XBS5_ILP32_OFF32
  P00_DEFINE_SC_1(P00_POSIX_SC_XBS5),
#endif
#ifdef _SC_V6_ILP32_OFF32
  P00_DEFINE_SC_1(P00_POSIX_SC_V6),
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
  P00_DEFINE_SC_1(P00_POSIX_SC_6),
#endif
};

long const*const p00_sc_constant = p00_sc_constant_;

#define P00_DEFINE_SC_(NAME, X, I) long P99_PASTE2(P99_S, X)(void)

#define P00_DEFINE_SC(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_DEFINE_SC_, __VA_ARGS__);

P00_DEFINE_SC(P00_POSIX_SC_1);
P00_DEFINE_SC(P00_POSIX_SC_2);
#ifdef _SC_XOPEN_THREADS
P00_DEFINE_SC(P00_POSIX_SC_THREAD);
#endif
#ifdef _SC_XOPEN_VERSION
P00_DEFINE_SC(P00_POSIX_SC_XOPEN);
#endif
#ifdef _SC_XBS5_ILP32_OFF32
P00_DEFINE_SC(P00_POSIX_SC_XBS5);
#endif
#ifdef _SC_V6_ILP32_OFF32
P00_DEFINE_SC(P00_POSIX_SC_V6);
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
P00_DEFINE_SC(P00_POSIX_SC_6);
#endif

#define P00_DEFINE_SC_NAME_(NAME, X, I) "_S" P99_STRINGIFY(X)

#define P00_DEFINE_SC_NAME(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEQ, P00_DEFINE_SC_NAME_, __VA_ARGS__)

static char const*const p00_sc_name[P00_POS_SC_MAX] = {
  P00_DEFINE_SC_NAME(P00_POSIX_SC_1),
  P00_DEFINE_SC_NAME(P00_POSIX_SC_2),
#ifdef _SC_XOPEN_THREADS
  P00_DEFINE_SC_NAME(P00_POSIX_SC_THREAD),
#endif
#ifdef _SC_XOPEN_VERSION
  P00_DEFINE_SC_NAME(P00_POSIX_SC_XOPEN),
#endif
#ifdef _SC_XBS5_ILP32_OFF32
  P00_DEFINE_SC_NAME(P00_POSIX_SC_XBS5),
#endif
#ifdef _SC_V6_ILP32_OFF32
  P00_DEFINE_SC_NAME(P00_POSIX_SC_V6),
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
  P00_DEFINE_SC_NAME(P00_POSIX_SC_6),
#endif
};

static
long p00_sysconf(int name) {
  errno = 0;
  long ret = sysconf(name);
  if (ret < 0) {
    ret = LONG_MIN + 1;
    P99_HANDLE_ERRNO {
    P99_XDEFAULT:
      ret = -p99_errno;
    }
  }
  return ret;
}

#define P00_DEFINE_SC_INIT_(NAME, X, I)                                    \
p00_sc_constant_[P99_PASTE2(P00_POS_S, X)] = p00_sysconf(P99_PASTE(_S, X))

#define P00_DEFINE_SC_INIT(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_DEFINE_SC_INIT_, __VA_ARGS__)

void orwl_posix_init(void) {
  P00_DEFINE_SC_INIT(P00_POSIX_SC_1);
  P00_DEFINE_SC_INIT(P00_POSIX_SC_2);
#ifdef _SC_XOPEN_THREADS
  P00_DEFINE_SC_INIT(P00_POSIX_SC_THREAD);
#endif
#ifdef _SC_XOPEN_VERSION
  P00_DEFINE_SC_INIT(P00_POSIX_SC_XOPEN);
#endif
#ifdef _SC_XBS5_ILP32_OFF32
  P00_DEFINE_SC_INIT(P00_POSIX_SC_XBS5);
#endif
#ifdef _SC_V6_ILP32_OFF32
  P00_DEFINE_SC_INIT(P00_POSIX_SC_V6);
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
  P00_DEFINE_SC_INIT(P00_POSIX_SC_6);
#endif
}

void orwl_posix_print(void) {
  for (size_t i = 0; i < P00_POS_SC_MAX; ++i) {
    char const* name = p00_sc_name[i] ? p00_sc_name[i] + 4 : "unknown";
    if (p00_sc_constant[i] < 0) {
      switch (p00_sc_constant[i]) {
      case LONG_MIN + 1:
        fprintf(stderr, "POSIX feature %-29sis indefinite\n", name);
        break;
      case -EINVAL:
        fprintf(stderr, "POSIX feature %-29sis not supported\n", name);
        break;
      default:
        fprintf(stderr, "POSIX feature %s, error %ld: %s\n", name, -p00_sc_constant[i], strerror(-p00_sc_constant[i]));
      }
    } else {
      fprintf(stderr, "POSIX feature %-29sis %ld\n", name, p00_sc_constant[i]);
    }
  }
}
