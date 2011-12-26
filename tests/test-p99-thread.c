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

static
int task(void* arg) {
  int ret = 0;
  printf("arg is %p\n", arg);
  mtx_lock(&mut);
  ++count;
  ret = count;
  cnd_signal(&cond);
  mtx_unlock(&mut);
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
  printf("result is %d\n", res);
}
