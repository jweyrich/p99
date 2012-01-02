/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#include "p99_threads.h"


void * p00_thrd_create(void* context);

static mtx_t mut;
static cnd_t cond;
static size_t volatile count = 0;
static atomic_double D;
static atomic_uint U;

P99_DECLARE_STRUCT(tester);

struct tester {
  int a;
  double b;
};

P99_DECLARE_ATOMIC(tester, atomic_tester);
atomic_tester testvar;

P99_DECLARE_ATOMIC(int*, atomic_intp);
atomic_intp intp;

static
int task(void* arg) {
  int ret = 0;
  printf("arg is %p, %d, %s %s\n",
         arg,
         P99_TYPE_INTEGER(arg),
         bool_getname(atomic_is_lock_free(&D)),
         bool_getname(atomic_is_lock_free(&U)));
  atomic_fetch_add(&U, 1u);
  mtx_lock(&mut);
  ++count;
  ret = count;
  cnd_signal(&cond);
  mtx_unlock(&mut);
  atomic_fetch_add(&D, ret);
  atomic_fetch_add(&intp, 1);
  atomic_store(&testvar, (tester){ .a = ret });
  if (ret % 3) thrd_yield();
  if (ret % 2)
    return -1;
  else
    thrd_exit(ret);
}



int main(int argc, char *argv[]) {
  size_t n = argc < 2 ? 2 : strtoul(argv[1], 0, 0);
  mtx_init(&mut, mtx_plain);
  cnd_init(&cond);
  thrd_t id[n];
  int* intp2 = 0;
  atomic_compare_exchange_weak(&intp, &intp2, &argc);
  unsigned U2 = 3;
  atomic_compare_exchange_weak(&U, &U2, 2);
  printf("initial value of U is %u, should be 0\n", U2);
  atomic_compare_exchange_weak(&U, &U2, 1);
  printf("value of U is set to %u, should be 1\n", atomic_load(&U));
  for (size_t i = 0; i < n; ++i)
    thrd_create(&id[i], task, &id[i]);
  for (size_t i = 1; i < n; i += 2)
    thrd_detach(id[i]);
  int res = 0;
  for (size_t i = 0; i < n; i += 2)
    thrd_join(id[i], &res);
  mtx_lock(&mut);
  while (count < n) cnd_wait(&cond, &mut);
  mtx_unlock(&mut);
  printf("results are %d, %d, %zu and %g\n",
         res,
         atomic_load(&testvar).a,
         (size_t)(atomic_load(&intp) - &argc),
         atomic_load(&D));
}
