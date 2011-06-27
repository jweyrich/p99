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
#ifndef   	ORWL_INSTRUMENT_H_
# define   	ORWL_INSTRUMENT_H_

#include "orwl.h"

void print_statistics_instr(size_t id);
orwl_state orwl_write_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag);
orwl_state orwl_read_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag);
orwl_state orwl_acquire2_instr(orwl_handle2* rh2, size_t flag);
orwl_state orwl_release2_instr(orwl_handle2* rh2, size_t flag);
orwl_state orwl_cancel2_instr(orwl_handle2* rh2, size_t flag);
orwl_state orwl_test2_instr(orwl_handle2* rh2, size_t flag);
void* orwl_write_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag);
void const* orwl_read_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag);
void orwl_truncate2_instr(orwl_handle2* rh2, size_t data_len, size_t flag);

#endif
