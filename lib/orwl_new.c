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
#include "orwl_new.h"

P99_INSTANTIATE(size_t, p00_blocks, size_t, size_t);
P99_INSTANTIATE(size_t, p00_vnew_size_len, size_t);
P99_INSTANTIATE(void*, p00_vnew, size_t);
P99_INSTANTIATE(size_t, p00_vlen, void const*);
P99_INSTANTIATE(void, p00_vdelete, void const*);
P99_INSTANTIATE(size_t const*, p00_vfind, void const*);
