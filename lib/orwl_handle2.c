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
#include "orwl_handle2.h"

orwl_handle2 *orwl_handle2_init(orwl_handle2 *rh2);
void orwl_handle2_destroy(orwl_handle2 *rh2);

DEFINE_NEW_DELETE(orwl_handle2);

DEFINE_ORWL_REGISTER_ALIAS(orwl_acquire2, orwl_handle2);
DEFINE_ORWL_REGISTER_ALIAS(orwl_release2, orwl_handle2);
DEFINE_ORWL_REGISTER_ALIAS(orwl_cancel2, orwl_handle2);


DEFINE_ORWL_TYPE_DYNAMIC(orwl_handle2,
                         ORWL_REGISTER_ALIAS(orwl_acquire2, orwl_handle2),
                         ORWL_REGISTER_ALIAS(orwl_release2, orwl_handle2),
                         ORWL_REGISTER_ALIAS(orwl_cancel2, orwl_handle2)
                         );

static
orwl_mirror* mirror_location(orwl_handle2* rh2) {
  orwl_mirror* ret = NULL;
  if (rh2) {
    bool par = (rh2->clock % 2);
    ret = rh2->pair[par].rq;
  }
  return ret;
}

static
void orwl__new_request2(orwl_mirror* location, orwl_handle2* rh2, rand48_t* seed, bool par) {
  if (rh2->state[par] != orwl_requested)
    rh2->state[par] = rh2->inclusive
      ? orwl_read_request(location, &rh2->pair[par], seed)
      : orwl_write_request(location, &rh2->pair[par], seed);
}

static
orwl_state orwl__request2(orwl_mirror* location, orwl_handle2* rh2, rand48_t* seed, bool inclusive) {
  rh2->state[0] = orwl_invalid;
  rh2->state[1] = orwl_invalid;
  bool par = (rh2->clock % 2);
  if (!mirror_location(rh2)) {
    rh2->inclusive = inclusive;
    orwl__new_request2(location, rh2, seed, par);
    //rh2->location = location;
  }
  return rh2->state[par];
}

orwl_state orwl_write_request2(orwl_mirror* location, orwl_handle2* rh2, rand48_t* seed) {
  return orwl__request2(location, rh2, seed, false);
}

orwl_state orwl_read_request2(orwl_mirror* location, orwl_handle2* rh2, rand48_t* seed) {
  return orwl__request2(location, rh2, seed, true);
}

orwl_state orwl_release2(orwl_handle2* rh2, rand48_t* seed)  {
  bool par = (rh2->clock % 2);
  orwl_acquire2(rh2);
  if (rh2->state[par] == orwl_acquired) {
    rh2->state[par] =  mirror_location(rh2)
      ? orwl_release(&rh2->pair[par], seed)
      : orwl_invalid;
  }
  ++(rh2->clock);
  return rh2->state[par];
}

orwl_state orwl_cancel2(orwl_handle2* rh2, rand48_t* seed) {
  if (mirror_location(rh2)) {
    rh2->inclusive = false;
    for (int par = 0; par < 2; ++par) {
      rh2->state[par] = orwl_cancel(&rh2->pair[par], seed);
    }
    return (rh2->state[0] <= rh2->state[1] ? rh2->state[0] : rh2->state[1]);
  } else return orwl_valid;
}

orwl_state orwl_acquire2(orwl_handle2* rh2, rand48_t* seed) {
  bool par = (rh2->clock % 2);
  orwl_state *const state = rh2->state;
  orwl_mirror *const location = mirror_location(rh2);
  switch (state[par]) {
  case orwl_requested:
    state[par] = location
      ? orwl_acquire(&rh2->pair[par])
      : orwl_invalid;
  case orwl_acquired: break;
  default: return orwl_invalid;
  }
  if (state[par] == orwl_acquired) {
    orwl__new_request2(location, rh2, seed, !par);
    if (state[!par] != orwl_requested) state[par] = orwl_invalid;
  }
  return state[par];
}

orwl_state orwl_test2(orwl_handle2* rh2, rand48_t* seed) {
  bool par = (rh2->clock % 2);
  rh2->state[par] =  mirror_location(rh2)
    ? orwl_test(&rh2->pair[par])
    : orwl_invalid;
  return rh2->state[par];
}

void orwl_map2(orwl_handle2* rh2, uint64_t** data, size_t* data_len, rand48_t* seed) {
  if (orwl_acquire2(rh2, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    orwl_map(&rh2->pair[par], data, data_len);
  }
}

void orwl_resize2(orwl_handle2* rh2, size_t data_len, rand48_t* seed) {
  if (orwl_acquire2(rh2, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    orwl_resize(&rh2->pair[par], data_len);
  }
}

