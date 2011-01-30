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
#include "orwl_rand.h"
#include "orwl_posix.h"
#include "p99_posix_default.h"
#include "p99_c99_default.h"

P99_INSTANTIATE(rand48_t*, rand48_t_init, rand48_t*, unsigned short, unsigned short, unsigned short);
P99_INSTANTIATE(void, rand48_t_destroy, rand48_t*);
P99_INSTANTIATE(uint32_t, orwl_rand, rand48_t*);
P99_INSTANTIATE(double, orwl_drand, rand48_t*);
P99_INSTANTIATE(uint64_t, orwl_rand64, rand48_t*);

DEFINE_NEW_DELETE(rand48_t);

DEFINE_THREAD_VAR(rand48_t, seed_get);



uint64_t orwl_mix(uint64_t a, uint64_t b) {
  uint16_t a0 = a >> 0;
  uint16_t a1 = a >> 16;
  uint16_t a2 = a >> 32;
  uint16_t a3 = a >> 48;
  uint16_t b0 = b >> 0;
  uint16_t b1 = b >> 16;
  uint16_t b2 = b >> 32;
  uint16_t b3 = b >> 48;
  rand48_t tmp[4] = {
    {{a0, b0, b2}},
    {{b1, a1, b3}},
    {{b2, b1, a2}},
    {{a3, b3, b0}},
  };
  return
    (uint64_t)(orwl_rand(&tmp[0]) ^ orwl_rand(&tmp[1]))
    | ((uint64_t)(orwl_rand(&tmp[2]) ^ orwl_rand(&tmp[3])) << 32);
}

static uint64_t secret = P99_0(uint64_t);

static char const ENVVAR[] = "ORWL_SECRET";

DEFINE_ONCE_UPON(secret) {
  char const *str = getenv(ENVVAR);
  if (str) secret = strtou64(str);
}

uint64_t orwl_challenge(uint64_t a) {
  uint64_t ret = 0;
  INIT_ONCE_UPON(secret, secret);
  if (secret) {
    for (; !ret; ++a)
      ret = orwl_mix(a, secret);
  }
  return ret;
}
