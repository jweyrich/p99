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
#ifndef     ORWL_HEADER_H_
# define    ORWL_HEADER_H_

#include "orwl_int.h"

enum { orwl_header_els = 3 };

typedef uint64_t orwl_header[orwl_header_els];

#define ORWL_HTON64(VAL)                                       \
(((uint64_t)htonl((uint32_t)(VAL)))                            \
 | (((uint64_t)htonl((uint32_t)((VAL) >> 32))) << 32))

#define ORWL_NTOH64(VAL)                                       \
(((uint64_t)ntohl((uint32_t)(VAL)))                            \
 | (((uint64_t)ntohl((uint32_t)((VAL) >> 32))) << 32))



inline
uint64_t orwl_hton64(uint64_t val) {
  return ORWL_HTON64(val);
}

inline
uint64_t orwl_ntoh64(uint64_t val) {
  return ORWL_NTOH64(val);
}

#define ORWL_HOSTORDER UINT64_C(0x0807060504030201)
#define ORWL_NETWORDER ORWL_HTON64(ORWL_HOSTORDER)

#define ORWL_HEADER_INITIALIZER(CHAL) { [0] = CHAL, [1] = 0, [2] = ORWL_NETWORDER }


#endif      /* !ORWL_HEADER_H_ */
