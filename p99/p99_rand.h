/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_RAND_H
#define P99_RAND_H
#include "p99_threads.h"
#include "p99_new.h"

/** @file
 **
 ** @brief An implementation of George Marsaglia's xorshift PRG
 ** with a period of about 2^160
 **/

/* From: George Marsaglia (geo@stat.fsu.edu)    */
/* Subject: Re: good C random number generator  */
/* Newsgroups: comp.lang.c                      */
/* Date: 2003-05-13 08:55:05 PST                */

/* Here is an example with k=5, period about 2^160, one of the fastest
   long period RNGs, returns more than 120 million random 32-bit
   integers/second (1.8MHz CPU), seems to pass all tests: */

P99_CONSTANT(int, p99_seed160_len, 6);
typedef uint32_t p99_seed160[p99_seed160_len];

p99_inline
uint32_t p00_xorshift(p99_seed160 * p00_s) {
  /* Use the 6th element as the running index, and compute the
     necessary derived indices mod 5 */
  ++(*p00_s)[5];
  register uint32_t const p00_0 = ((*p00_s)[5]) % 5;
  register uint32_t const p00_4 = p00_0 == 0 ? 4 : p00_0-1;
  register uint32_t const p00_2 = (p00_0 == 4) ? 1 : ((p00_0 == 3) ? 0 : p00_0+2);

  /* Load the part of the state variable that we need. */
  register uint32_t const p00_x = (*p00_s)[p00_0];
  register uint32_t const p00_y = (*p00_s)[p00_2];
  register uint32_t const p00_v = (*p00_s)[p00_4];

  register uint32_t t = (p00_x^(p00_x>>7));
  t = (p00_v^(p00_v<<6))^(t^(t<<13));

  /* Change just one place in the state variables. */
  (*p00_s)[p00_4] = t;

  /* Multiply with an odd number */
  return (2*p00_y + 1) * t;
}

struct p00_rand160 {
  once_flag p00_flag;
  p99_seed160 p00_seed[2];
};

P99_DECLARE_THREAD_LOCAL(struct p00_rand160, p00_seed160);

#define P00_BIGPRIME                                           \
UINT64_C(10007814641597694113),                                \
UINT64_C(10015183610531627897),                                \
UINT64_C(10089390291074425231),                                \
UINT64_C(10117275823191396191),                                \
UINT64_C(10215588060907623179),                                \
UINT64_C(10389441874873414061),                                \
UINT64_C(10602620510410479149),                                \
UINT64_C(10690793455755991027),                                \
UINT64_C(10793439684376201283),                                \
UINT64_C(10969167379420052431),                                \
UINT64_C(11050883422537956197),                                \
UINT64_C(11110712460003231287),                                \
UINT64_C(11342254921836171103),                                \
UINT64_C(11388752923558666351),                                \
UINT64_C(11682943258734137249),                                \
UINT64_C(11697622259988783581),                                \
UINT64_C(11731205802697935733),                                \
UINT64_C(11799524298339249581),                                \
UINT64_C(11862963432241722239),                                \
UINT64_C(11903172551239393097),                                \
UINT64_C(11937270107112816793),                                \
UINT64_C(11962673972948293321),                                \
UINT64_C(12001454206321043837),                                \
UINT64_C(12066567988703197129),                                \
UINT64_C(12090279337620046961),                                \
UINT64_C(12320220036580238077),                                \
UINT64_C(12409336420886496139),                                \
UINT64_C(12441355201341188273),                                \
UINT64_C(12444926949253327381),                                \
UINT64_C(12464345941324672183),                                \
UINT64_C(12469071720100075169),                                \
UINT64_C(12541090550165194183),                                \
UINT64_C(12558029109792848393),                                \
UINT64_C(12629045726855953273),                                \
UINT64_C(12845265184712635099),                                \
UINT64_C(12904165305535099733),                                \
UINT64_C(12949777725323667257),                                \
UINT64_C(13014679980528315931),                                \
UINT64_C(13027415158206533719),                                \
UINT64_C(13073256976558063279),                                \
UINT64_C(13153853421202095331),                                \
UINT64_C(13381351390611252151),                                \
UINT64_C(13825543164380321747),                                \
UINT64_C(13917403769616578911),                                \
UINT64_C(13929904570285153753),                                \
UINT64_C(14140216129016347127),                                \
UINT64_C(14184039880925065139),                                \
UINT64_C(14271570646513640671),                                \
UINT64_C(14446852824178510313),                                \
UINT64_C(14666997777683867003),                                \
UINT64_C(14692182605128454039),                                \
UINT64_C(14771388299183335229),                                \
UINT64_C(15059759696658839237),                                \
UINT64_C(15091316017205268437),                                \
UINT64_C(15180626055737690959),                                \
UINT64_C(15339437418814258573),                                \
UINT64_C(15601183704045111169),                                \
UINT64_C(15983170142123181797),                                \
UINT64_C(16545722488476290101),                                \
UINT64_C(16580005714519352107),                                \
UINT64_C(16589193612296178103),                                \
UINT64_C(16620171437104037921),                                \
UINT64_C(16713706803710883721),                                \
UINT64_C(16826711990149094791),                                \
UINT64_C(16886322755473635461),                                \
UINT64_C(16950140700365130619),                                \
UINT64_C(16952043704640877837),                                \
UINT64_C(17020386452131177189),                                \
UINT64_C(17020824347466355583),                                \
UINT64_C(17044414685455096133),                                \
UINT64_C(17089443963053420461),                                \
UINT64_C(17165479835541081871),                                \
UINT64_C(17185820514261086599),                                \
UINT64_C(17191892745505804273),                                \
UINT64_C(17613198595545138731),                                \
UINT64_C(17653178755564367203),                                \
UINT64_C(17653752776501147281),                                \
UINT64_C(17728444844615762171),                                \
UINT64_C(17744620187156425403),                                \
UINT64_C(17816722786105806973),                                \
UINT64_C(17831662963314755641),                                \
UINT64_C(17886004545482299117),                                \
UINT64_C(17920151933265509833),                                \
UINT64_C(17969552600607433963),                                \
UINT64_C(18066918898331824901),                                \
UINT64_C(18094173144238831753),                                \
UINT64_C(18134240252569387847),                                \
UINT64_C(18219113917191524677),                                \
UINT64_C(18298168206731166317),                                \
UINT64_C(18319234190200763803)

P99_WEAK(p99_rand160_init)
void p99_rand160_init(void* p00_p) {
  p99_seed160 * p00_s = p00_p
                        ? p00_p
                        : &P99_THREAD_LOCAL(p00_seed160).p00_seed;
  struct timespec p00_ts;
  timespec_get(&p00_ts, TIME_UTC);
  p99_seed160 p00_st = {
    /* local to this initialization call */
    (uintptr_t)p00_st,
    /* unique to this thread */
    (uintptr_t)p00_s,
    /* unique to this reallocation in the process */
    (uintptr_t)__func__,
    /* unique in time */
    (uint32_t)p00_ts.tv_sec,
    (uint32_t)p00_ts.tv_nsec,
    /* index, unique to this thread */
    (uintptr_t)p00_s,
  };
  /* mix things up a bit */
  for (unsigned p00_i = 0; p00_i < 32; ++p00_i) p00_xorshift(&p00_st);
  /* Now create two different state vectors that are de-phased. We
     can be in any state of the xorshift generator, so there are
     2^160 different initializations. */
  for (unsigned p00_j = 0; p00_j < 2; ++p00_j) {
    for (unsigned p00_i = 0; p00_i < p99_seed160_len; ++p00_i)
      (*p00_s)[p00_i] = p00_xorshift(&p00_st);
    ++p00_s;
  }
}

p99_inline
p99_seed160 * p99_seed160_get(void) {
  struct p00_rand160 * p00_loc = &P99_THREAD_LOCAL(p00_seed160);
  p99_call_once(&p00_loc->p00_flag, p99_rand160_init, &p00_loc->p00_seed);
  return &p00_loc->p00_seed[0];
}

P99_WEAK(p00_bigprime)
uint64_t const p00_bigprime[] = { P00_BIGPRIME };

enum { p00_bigprime_len = P99_ALEN(p00_bigprime) };

/**
 ** @brief a folded variant of the xorshift pseudo random generator
 **
 ** This implements a set of about 2^160 pseudo random generators,
 ** each with a period of about 2^160. The main idea is to have two
 ** different xorshift generators run in sync giving two 32 bit
 ** quantities and then to mangle up their results such that the
 ** individual bits can not be traced.
 **
 ** @warning This is not guaranteed to be crytographically secure.
 **/
P99_WEAK(p99_rand160)
uint64_t p99_rand160(void) {
  register p99_seed160 * p00_seed = p99_seed160_get();
  uint64_t p00_0 = p00_xorshift(&p00_seed[0]);
  uint64_t p00_1 = p00_xorshift(&p00_seed[1]);
  uint64_t p00_0r = p00_0 % p00_bigprime_len;
  uint64_t p00_0d = p00_0 / p00_bigprime_len;
  /* Use pare of the bits to choose a big number */
  p00_1 *= p00_bigprime[p00_0r];
  /* Use the rest of the bits to add more randomness */
  return (p00_0d ^ p00_1);
}

#endif
