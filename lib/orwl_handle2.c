/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_handle2.h"

P99_INSTANTIATE(orwl_handle2*, orwl_handle2_init, orwl_handle2*);
P99_INSTANTIATE(void, orwl_handle2_destroy, orwl_handle2*);

DEFINE_NEW_DELETE(orwl_handle2);

DEFINE_ORWL_REGISTER_ALIAS(orwl_acquire2, orwl_handle2);
DEFINE_ORWL_REGISTER_ALIAS(orwl_next2, orwl_handle2);
DEFINE_ORWL_REGISTER_ALIAS(orwl_forced_cancel2, orwl_handle2);
DEFINE_ORWL_REGISTER_ALIAS(orwl_disconnect2, orwl_handle2);

DEFINE_ORWL_TYPE_DYNAMIC(orwl_handle2,
                         ORWL_REGISTER_ALIAS(orwl_acquire2, orwl_handle2),
                         ORWL_REGISTER_ALIAS(orwl_next2, orwl_handle2),
                         ORWL_REGISTER_ALIAS(orwl_forced_cancel2, orwl_handle2),
                         ORWL_REGISTER_ALIAS(orwl_disconnect2, orwl_handle2)
                        );

static
orwl_mirror* mirror_location(orwl_handle2* rh2) {
  orwl_mirror* ret = 0;
  if (rh2) {
    bool par = (rh2->clock % 2);
    ret = rh2->pair[par].rq;
  }
  return ret;
}

static
void o_rwl_new_request2(orwl_mirror* location, orwl_handle2* rh2, rand48_t* seed, bool par) {
  if (rh2->state[par] != orwl_requested)
    rh2->state[par] = rh2->inclusive
      ? orwl_read_request(location, &rh2->pair[par], 1, seed)
      : orwl_write_request(location, &rh2->pair[par], 1, seed);
}

static
orwl_state o_rwl_request2(orwl_mirror* location, orwl_handle2* rh2, rand48_t* seed, bool inclusive) {
  rh2->state[0] = orwl_invalid;
  rh2->state[1] = orwl_invalid;
  bool par = (rh2->clock % 2);
  if (!mirror_location(rh2)) {
    rh2->inclusive = inclusive;
    o_rwl_new_request2(location, rh2, seed, par);
  }
  return rh2->state[par];
}

orwl_state orwl_write_request2(orwl_mirror* location, orwl_handle2* rh2, size_t size, rand48_t* seed) {
  orwl_state ret = orwl_invalid;
  if (location && rh2) {
    for (size_t i = 0; i < size; ++i) {
      ret = o_rwl_request2(location + i, rh2 + i, seed, false);
      if (ret != orwl_requested) break;
    }
  }
  return ret;
}

orwl_state orwl_read_request2(orwl_mirror* location, orwl_handle2* rh2, size_t size, rand48_t* seed) {
  orwl_state ret = orwl_invalid;
  if (location && rh2) {
    for (size_t i = 0; i < size; ++i) {
      ret = o_rwl_request2(location + i, rh2 + i, seed, true);
      if (ret != orwl_requested) break;
    }
  }
  return ret;
}

orwl_state orwl_release2(orwl_handle2* rh2, rand48_t* seed)  {
  return orwl_next2(rh2, 1u, seed);
}

orwl_state orwl_next2(orwl_handle2* rh0, size_t size, rand48_t* seed)  {
  orwl_state ret = orwl_state_amount;
  for (size_t i = 0; i < size; ++i) {
    orwl_handle2* rh2 = rh0 + i;
    bool par = (rh2->clock % 2);
    orwl_acquire2(rh2);
    if (rh2->state[par] == orwl_acquired) {
      rh2->state[par] =  mirror_location(rh2)
        ? orwl_release(&rh2->pair[par], 1u, seed)
        : orwl_invalid;
    }
    ++(rh2->clock);
    ret = (rh2->state[par] < ret ? rh2->state[par] : ret);
  }
  return ret;
}

orwl_state orwl_cancel2(orwl_handle2* rh2, rand48_t* seed) {
  return orwl_disconnect2(rh2, 1u, seed);
}

orwl_state orwl_forced_cancel2(orwl_handle2* rh2, rand48_t* seed) {
  if (mirror_location(rh2)) {
    rh2->inclusive = false;
    for (int par = 0; par < 2; ++par) {
      rh2->state[par] = orwl_cancel(&rh2->pair[par], seed);
    }
    return (rh2->state[0] <= rh2->state[1] ? rh2->state[0] : rh2->state[1]);
  } else return orwl_valid;
}

orwl_state orwl_acquire2(orwl_handle2* rh0, size_t size, rand48_t* seed) {
  orwl_state ret = orwl_state_amount;
  for (size_t i = 0; i < size; ++i) {
    orwl_handle2* rh2 = rh0 + i;
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
      o_rwl_new_request2(location, rh2, seed, !par);
      if (state[!par] != orwl_requested) state[par] = orwl_invalid;
    }
    ret = (rh2->state[par] < ret ? rh2->state[par] : ret);
  }
  return ret;
}

orwl_state orwl_disconnect2(orwl_handle2* rh0, size_t size, rand48_t* seed) {
  orwl_state ret = orwl_state_amount;
  for (size_t i = 0; i < size; ++i) {
    orwl_handle2* rh2 = rh0 + i;
    orwl_state *const state = rh2->state;
    orwl_handle *const pair = rh2->pair;
    for (unsigned par = 0; par < 2; ++par) {
      if (pair[par].rq) {
        switch (orwl_test(&pair[par], 1u)) {
        case orwl_requested:
          orwl_acquire(&pair[par], 1u);
        case orwl_acquired:
          orwl_release(&pair[par], 1u, seed);
        default: ;
        }
      }
      ret = (state[par] < ret ? state[par] : ret);
    }
  }
  return ret;
}

orwl_state orwl_test2(orwl_handle2* rh0, size_t size, rand48_t* seed) {
  orwl_state ret = orwl_state_amount;
  for (size_t i = 0; i < size; ++i) {
    orwl_handle2* rh2 = rh0 + i;
    bool par = (rh2->clock % 2);
    rh2->state[par] =  mirror_location(rh2)
      ? orwl_test(&rh2->pair[par], 1u)
      : orwl_invalid;
    ret = (rh2->state[par] < ret ? rh2->state[par] : ret);
  }
  return ret;
}

uint64_t* orwl_map2(orwl_handle2* rh2, size_t* data_len, rand48_t* seed) {
  uint64_t* ret = 0;
  if (orwl_acquire2(rh2, 1u, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    ret = orwl_map(&rh2->pair[par], data_len);
  } else {
    *data_len = 0;
  }
  return ret;
}

void* orwl_write_map2(orwl_handle2* rh2, size_t* data_len, rand48_t* seed) {
  void* ret = 0;
  if (orwl_acquire2(rh2, 1u, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    ret = orwl_write_map(&rh2->pair[par], data_len);
  } else {
    *data_len = 0;
  }
  return ret;
}

uint64_t const* orwl_mapro2(orwl_handle2* rh2, size_t* data_len, rand48_t* seed) {
  uint64_t const* ret = 0;
  if (orwl_acquire2(rh2, 1u, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    ret = orwl_mapro(&rh2->pair[par], data_len);
  } else {
    *data_len = 0;
  }
  return ret;
}

void const* orwl_read_map2(orwl_handle2* rh2, size_t* data_len, rand48_t* seed) {
  void const* ret = 0;
  if (orwl_acquire2(rh2, 1u, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    ret = orwl_read_map(&rh2->pair[par], data_len);
  } else {
    *data_len = 0;
  }
  return ret;
}

void orwl_resize2(orwl_handle2* rh2, size_t data_len, rand48_t* seed) {
  if (orwl_acquire2(rh2, 1u, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    orwl_resize(&rh2->pair[par], data_len);
  }
}

void orwl_truncate2(orwl_handle2* rh2, size_t data_len, rand48_t* seed) {
  if (orwl_acquire2(rh2, 1u, seed) == orwl_acquired) {
    bool par = (rh2->clock % 2);
    orwl_truncate(&rh2->pair[par], data_len);
  }
}

