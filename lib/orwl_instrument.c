/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */

/* Ensure that deprecated function don't issue a warning, here */
#include "orwl_posix.h"
#undef P99_DEPRECATED
#define P99_DEPRECATED(...) __VA_ARGS__

#include "orwl_instrument.h"


void print_statistics_instr(size_t id) {
  printf("Nothing is done\n");
}

orwl_state orwl_write_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  return orwl_write_request2(location, rh2);
}

orwl_state orwl_read_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  return orwl_read_request2(location, rh2);
}

orwl_state orwl_acquire2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_acquire2(rh2);
}

orwl_state orwl_release2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_release2(rh2);
}

orwl_state orwl_disconnect2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_disconnect2(rh2);
}

orwl_state orwl_forced_cancel2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_forced_cancel2(rh2);
}

orwl_state orwl_cancel2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_cancel2(rh2);
}

orwl_state orwl_test2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_test2(rh2);
}

void* orwl_write_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_write_map2(rh2);
}

void const* orwl_read_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_read_map2(rh2);
}

void orwl_truncate2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_truncate2(rh2, data_len);
}
