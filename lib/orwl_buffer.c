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
#include "orwl_buffer.h"

P99_INSTANTIATE(orwl_buffer*, orwl_buffer_init, orwl_buffer *, size_t, uint64_t*);
P99_INSTANTIATE(void, orwl_buffer_destroy, orwl_buffer*);
P99_INSTANTIATE(orwl_iovec*, orwl_iovec_init, orwl_iovec *, size_t, uint64_t*);
P99_INSTANTIATE(void, orwl_iovec_destroy, orwl_iovec*);
P99_INSTANTIATE(orwl_iovec, orwl_buffer2iovec, orwl_buffer buf);
P99_INSTANTIATE(orwl_buffer, orwl_iovec2buffer, orwl_iovec iovec);
