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
#ifndef ORWL_NOTIFIER_H
#define ORWL_NOTIFIER_H

#include "orwl_count.h"

/**
 ** @brief A restriction of ::orwl_count to set flag and notify
 ** other threads that an event has occurred.
 **
 ** @see orwl_notifier_set
 ** @see orwl_notifier_block
 ** @see orwl_notifier_verify
 **/
typedef orwl_count orwl_notifier;

#define ORWL_NOTIFIER_INITIALIZER ORWL_COUNT_INITIALIZER(1)

/**
 ** @memberof orwl_notifier
 **/
inline
orwl_notifier* orwl_notifier_init(orwl_notifier* notifier) {
  orwl_count_init(notifier, 1);
  return notifier;
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_destroy(orwl_notifier* notifier) {
  orwl_count_destroy(notifier);
}

/**
 ** @brief Verify if the notification flag has been set. Non blocking.
 ** @memberof orwl_notifier
 **/
inline
bool orwl_notifier_verify(orwl_notifier* notifier) {
  return !orwl_count_value(notifier);
}

/**
 ** @brief Set the notifier flag and wake up all potential waiters.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_set(orwl_notifier* notifier) {
  if (!orwl_notifier_verify(notifier)) {
    if (orwl_count_dec(notifier, 1)) {
      report(true, "duplicated notifier set for %p", (void*)notifier);
    }
  }
}

/**
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_unset(orwl_notifier* notifier) {
  if (orwl_notifier_verify(notifier)) {
    if (orwl_count_inc(notifier, 1)) {
      report(true, "duplicated notifier unset for %p", (void*)notifier);
    }
  }
}

/**
 ** @brief Block until the notification flag has been set.
 **
 ** Non blocking if the flag is already set.
 ** @memberof orwl_notifier
 **/
inline
void orwl_notifier_block(orwl_notifier* notifier) {
  orwl_count_wait(notifier);
}

#endif
