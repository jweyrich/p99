/* This may look like nonsense, but it really is -*- mode: C -*-             */
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
#ifndef   	ORWL_TIME_H_
# define   	ORWL_TIME_H_

#include "orwl_posix.h"
#include "orwl_once.h"

/**
 ** @file "orwl_time.h"
 ** @brief Handle different representations of time.
 **
 ** We have 4 different representations of time values
 ** - @b seconds represented by @c double
 ** - @b useconds, micro-seconds represented by @c uint64_t
 ** - <code>struct timeval</code>, breaking time down to micro-seconds
 ** - <code>struct timespec</code>, breaking time down to nano-seconds
 **
 ** We implement all 12 functions that convert between these different
 ** types.
 **/

#define ORWL__DOCUMENT_TIME(S, T)                                                                                      \
/*! @brief Convert a time value from S to T. @see orwl_time.h for more information about time values and functions. */

ORWL__DOCUMENT_TIME(seconds, timespec)
inline
struct timespec seconds2timespec(double t) {
  double const giga = 1E+9;
  double const sec = trunc(t);
  const struct timespec ret = {
    .tv_sec = (time_t)sec,
    .tv_nsec = (time_t)((t - sec) * giga)
  };
  return ret;
}

ORWL__DOCUMENT_TIME(timespec, seconds)
inline
double timespec2seconds(struct timespec t) {
  double const nano = 1E-9;
  double const ret = t.tv_sec + nano * t.tv_nsec;
  return ret;
}

ORWL__DOCUMENT_TIME(seconds, timeval)
inline
struct timeval seconds2timeval(double t) {
  double const mega = 1E+6;
  double const sec = trunc(t);
  const struct timeval ret = {
    .tv_sec = (time_t)sec,
    .tv_usec = (time_t)((t - sec) * mega)
  };
  return ret;
}

ORWL__DOCUMENT_TIME(timeval, seconds)
inline
double timeval2seconds(struct timeval t) {
  double const micro = 1E-6;
  double const ret = t.tv_sec + micro * t.tv_usec;
  return ret;
}

ORWL__DOCUMENT_TIME(useconds, timespec)
inline
struct timespec useconds2timespec(uint64_t t) {
  enum { mega = 1000000u };
  uint64_t const sec = t / mega;
  uint64_t const rem = t % mega;
  const struct timespec ret = {
    .tv_sec = sec,
    .tv_nsec = rem * 1000uLL
  };
  return ret;
}

ORWL__DOCUMENT_TIME(timespec, useconds)
inline
uint64_t timespec2useconds(struct timespec t) {
  enum { mega = 1000000u };
  uint64_t const ret = t.tv_sec * mega + (t.tv_nsec / 1000);
  return ret;
}

ORWL__DOCUMENT_TIME(useconds, seconds)
inline
double useconds2seconds(uint64_t t) {
  double const micro = 1E-6;
  return t * micro;
}

ORWL__DOCUMENT_TIME(seconds, useconds)
inline
uint64_t seconds2useconds(double t) {
  double mega = 1E+6;
  return (uint64_t)(t * mega);
}

ORWL__DOCUMENT_TIME(useconds, timeval)
inline
struct timeval useconds2timeval(uint64_t t) {
  enum { mega = 1000000u };
  uint64_t const sec = t / mega;
  uint64_t const rem = t % mega;
  const struct timeval ret = {
    .tv_sec = sec,
    .tv_usec = rem
  };
  return ret;
}

ORWL__DOCUMENT_TIME(timeval, useconds)
inline
uint64_t timeval2useconds(struct timeval t) {
  enum { mega = 1000000u };
  uint64_t const ret = t.tv_sec * mega + t.tv_usec;
  return ret;
}

ORWL__DOCUMENT_TIME(timeval, timespec)
inline
struct timespec timeval2timespec(struct timeval t) {
  const struct timespec ret = {
    .tv_sec = t.tv_sec,
    .tv_nsec = t.tv_usec * 1000uLL
  };
  return ret;
}

ORWL__DOCUMENT_TIME(timespec, timeval)
inline
struct timeval timespec2timeval(struct timespec t) {
  long usec = t.tv_nsec / 1000;
  long rem = t.tv_nsec % 1000;
  const struct timeval ret = {
    .tv_sec = t.tv_sec,
    .tv_usec = (rem < 500 ? usec : usec + 1)
  };
  return ret;
}

/**
 ** @brief get the current time (since) Epoch in nanosecond precision.
 **
 ** If that is not available fall back to microsecond precision.
 ** @see orwl_time.h for more information about time values and functions.
 **/
#ifdef CLOCK_REALTIME
inline
struct timespec orwl_gettime(void) {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t;
}
#else
struct timespec orwl_gettime(void);
#endif

DECLARE_ONCE(orwl_gettime);


/**
 ** @brief Get the micro-seconds since epoch.
 ** @see orwl_time.h for more information about time values and functions.
 **/
inline
uint64_t useconds(void) {
  struct timespec t = orwl_gettime();
  return timespec2useconds(t);
}

/**
 ** @brief Get the seconds since epoch.
 ** @see orwl_time.h for more information about time values and functions.
 **/
inline
double seconds(void) {
  struct timespec t = orwl_gettime();
  return timespec2seconds(t);
}

/**
 ** @brief Let the calling thread rest for @a t seconds
 **
 ** The sleep time for this function includes the time that the thread
 ** is stopped by a signal.
 ** @see orwl_time.h for more information about time values and functions.
 **/
extern void sleepfor(double t);


#endif 	    /* !ORWL_TIME_H_ */
