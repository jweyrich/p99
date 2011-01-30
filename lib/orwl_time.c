/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
/*
** orwl_time.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Jan 30 15:38:54 2011 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_time.h"
#include "p99_defarg.h"

P99_INSTANTIATE(struct timespec, seconds2timespec, double);
P99_INSTANTIATE(double, timespec2seconds, struct timespec);
P99_INSTANTIATE(struct timeval, seconds2timeval, double);
P99_INSTANTIATE(double, timeval2seconds, struct timeval);

P99_INSTANTIATE(struct timespec, useconds2timespec, uint64_t);
P99_INSTANTIATE(uint64_t, timespec2useconds, struct timespec);
P99_INSTANTIATE(struct timeval, useconds2timeval, uint64_t);
P99_INSTANTIATE(uint64_t, timeval2useconds, struct timeval);

P99_INSTANTIATE(struct timeval, timespec2timeval, struct timespec);
P99_INSTANTIATE(struct timespec, timeval2timespec, struct timeval);
P99_INSTANTIATE(double, useconds2seconds, uint64_t);
P99_INSTANTIATE(uint64_t, seconds2useconds, double t);

P99_INSTANTIATE(struct timespec, orwl_gettime);
P99_INSTANTIATE(uint64_t, useconds);
P99_INSTANTIATE(double, seconds);

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199309L)
struct timespec orwl_gettime(void) {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t;
}
#else
#warning only low resolution gettimeofday found
struct timespec orwl_gettime(void) {
  struct timeval t;
  gettimeofday(&t, P99_0(struct timezone*));
  return timeval2timespec(t);
}
#endif

#if !defined(NO_NANOSLEEP) && defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199309L)
void sleepfor(double t) {
  double start = seconds();
  while (t > 0.0) {
    struct timespec req = seconds2timespec(t);
    if (!nanosleep(&req, 0)) return;
    double now = seconds();
    double passed = now - start;
    if (passed >= t) return;
    t -= passed;
    start = now;
  }
}
#else
# warning only low resolution sleep found
void sleepfor(double t) {
  uint64_t const mega = 1000000;
  uint64_t start = useconds();
  uint64_t rem =  seconds2useconds(t);
  while (rem) {
    /* It seems that some acient systems couldn't use usleep for more
     * than a second. */
    if (rem >= mega) {
      uint64_t secs = rem / mega;
      uint64_t passed = sleep(secs);
      if (passed > secs) return;
      passed = (passed ? passed : secs) * mega;
      rem -= passed;
      start += passed;
    } else {
# if !defined(NO_USLEEP) && defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)
      if (!usleep((useconds_t)rem)) return;
#  else
#  warning usleep replaced with select hack
    /* This is a hack. For once we are not interested in the return
       value. we have to check ourselves, anyhow. */
      struct timeval req = useconds2timeval(rem);
      select(0, 0, 0, 0, &req);
# endif
      errno = 0;
      uint64_t now = useconds();
      uint64_t passed = now - start;
      if (passed >= rem) return;
      rem -= passed;
      start = now;
    }
  }
}
#endif
