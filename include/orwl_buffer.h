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
/*
** orwl_buffer.h
**
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
**
** Started on  Tue Jul  5 08:25:17 2011 Jens Gustedt
** Last update Tue Jul  5 08:25:17 2011 Jens Gustedt
*/

#ifndef     ORWL_BUFFER_H_
# define    ORWL_BUFFER_H_

#include "p99.h"
#include <sys/uio.h>

P99_DECLARE_STRUCT(orwl_buffer);

struct orwl_buffer {
  uint64_t* data;
  size_t len;
};

#define ORWL_BUFFER_INITIALIZER(LEN, DATA) { .len = LEN, .data = DATA }


#ifndef DOXYGEN
inline
P99_PROTOTYPE(orwl_buffer*, orwl_buffer_init, orwl_buffer *, size_t, uint64_t*);
#define orwl_buffer_init(...) P99_CALL_DEFARG(orwl_buffer_init, 3, __VA_ARGS__)
#define orwl_buffer_init_defarg_1() P99_0(size_t)
#define orwl_buffer_init_defarg_2() P99_0(uint64_t*)
#endif

inline
orwl_buffer* orwl_buffer_init(orwl_buffer *buf, size_t len, uint64_t* data) {
  if (buf) {
    *buf = P99_LVAL(orwl_buffer, .len = len, .data = data);
  }
  return buf;
}

inline
void orwl_buffer_destroy(orwl_buffer *buf) {
  /* empty */
}

typedef struct iovec orwl_iovec;

inline
orwl_iovec* orwl_iovec_init(orwl_iovec *buf, size_t len, uint64_t* data) {
  if (buf) {
    *buf = P99_LVAL(orwl_iovec, .iov_len = len, .iov_base = data);
  }
  return buf;
}

inline
void orwl_iovec_destroy(orwl_iovec *buf) {
  /* empty */
}

inline
orwl_iovec orwl_buffer2iovec(orwl_buffer buf) {
  return P99_LVAL(orwl_iovec, .iov_len = (buf.len)*sizeof buf.data[0], .iov_base = buf.data);
}

inline
orwl_buffer orwl_iovec2buffer(orwl_iovec iovec) {
  static orwl_buffer const buf;
  return P99_LVAL(orwl_buffer, .len = iovec.iov_len / sizeof buf.data[0], .data = iovec.iov_base);
}



#endif      /* !ORWL_BUFFER_H_ */
