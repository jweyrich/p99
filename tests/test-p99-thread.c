/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2011-2013 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_threads.h"
#include "p99_atomic.h"
#include "p99_generic.h"
#include "p99_clib.h"
#include "p99_fifo.h"

/* declarations of these functions would usually go in a .h file */
#include "p99_init.h"
P99_INIT_FUNCTION_DECLARE(toto);
#include "p99_init.h"
P99_INIT_FUNCTION_DECLARE(tutu, 95);
#include "p99_init.h"
P99_INIT_FUNCTION_DECLARE(tata);
#include "p99_init.h"
unsigned teteVar;
void tete(void*);
P99_INIT_VARIABLE(teteVar, tete);

/* implementations of these functions would usually go in the .c */
void toto(void) {
  fprintf(stderr, "this is %s\n", __func__);
}
void tutu(void) {
  fprintf(stderr, "this is %s\n", __func__);
}
void tata(void) {
  fprintf(stderr, "this is %s\n", __func__);
}
void tete(void* arg) {
  fprintf(stderr, "this is %s\n", __func__);
  unsigned* teteVar = arg;
  *teteVar = 42;
  fprintf(stderr, "teteVar is %u\n", *teteVar);
}


void * p00_thrd_create(void* context);

static mtx_t mut;
static cnd_t cond;
static size_t volatile count = 0;
static atomic_double D;
static atomic_uint U;

P99_DECLARE_STRUCT(tester);
P99_POINTER_TYPE(tester);
P99_FIFO_DECLARE(tester_ptr);

struct tester {
  int a;
  double b;
  tester* p99_lifo;
};

P99_DECLARE_ATOMIC(tester, atomic_tester);
atomic_tester testvar;

P99_DECLARE_ATOMIC(int*, atomic_intp);

P99_DECLARE_ATOMIC(int*, atomic_intp2);
atomic_intp intp;

P99_FIFO(tester_ptr) ober_tester = P99_FIFO_INITIALIZER(0, 0);

static
void aqe(void) {
  fprintf(stderr, "quick exit!:");
  tester_ptr unter_tester = P99_FIFO_CLEAR(&ober_tester);
  while (unter_tester) {
    tester_ptr next = unter_tester->p99_lifo;
    fprintf(stderr, " %d (%p)", unter_tester ? unter_tester->a : INT_MAX, (void*)unter_tester);
    free(unter_tester);
    unter_tester = next;
  }
  fputc('\n', stderr);
}

static
void ate0(void) {
  fprintf(stderr, "thread exit 0!\n");
}

static
void ate1(void) {
  tester_ptr unter_tester = P99_FIFO_POP(&ober_tester);
  fprintf(stderr, "thread exit 1! %d (%p)\n", unter_tester ? unter_tester->a : INT_MAX, (void*)unter_tester);
  free(unter_tester);
}

static
int real_task(atomic_intp* arg) {
  int ret = 0;
  printf("arg is %p, %d, %s %s\n",
         (void*)arg,
         P99_TYPE_FLOATING(atomic_load(arg)),
         bool_getname(atomic_is_lock_free(&D)),
         bool_getname(atomic_is_lock_free(&U)));
  (void)atomic_fetch_add(&U, 1u);
  mtx_lock(&mut);
  ++count;
  ret = P99_GEN_MAX(count, 23);
  if (count%7) cnd_signal(&cond);
  else cnd_broadcast(&cond);
  mtx_unlock(&mut);
  {
    double d = atomic_load(&D);
    while (!atomic_compare_exchange_weak(&D, &d, d+ret)) {
      /* empty */
    }
  }
  (void)atomic_fetch_add(&intp, 1);
  int * point = atomic_fetch_add(arg, 1);
  *point = ret;
  tester b = atomic_load(&testvar);
  if (!atomic_compare_exchange_weak(&testvar, &b, (tester) { .a = ret }))
  printf("store didn't succeeded\n");
  at_thrd_exit(ate0);
  switch (ret % 3) {
  case 0: thrd_yield(); break;
  case 1: at_quick_exit(aqe); break;
  case 2: at_thrd_exit(ate1); break;
  }
  tester_ptr unter_tester = P99_MALLOC(tester);
  *unter_tester = (tester) { .a = ret, };
  P99_FIFO_APPEND(&ober_tester, unter_tester);
  if (ret % 2)
    return -1;
  else
    thrd_exit(ret);
}

static
int task(void* arg) {
  return real_task(arg);
}

static
void cleanup(void) {
  cnd_destroy(&cond);
  mtx_destroy(&mut);
}

int main(int argc, char *argv[]) {
  size_t n = argc < 2 ? 2 : strtoul(argv[1], 0, 0);
  mtx_init(&mut, mtx_plain);
  cnd_init(&cond);
  at_quick_exit(cleanup);
  atexit(cleanup);
  thrd_t (*id)[n] = P99_MALLOC(*id);
  int* intp2 = 0;
  (void)atomic_compare_exchange_weak(&intp, &intp2, &argc);
  unsigned U2 = 3;
  (void)atomic_compare_exchange_weak(&U, &U2, 2);
  printf("initial value of U is %u, should be 0\n", U2);
  (void)atomic_compare_exchange_weak(&U, &U2, 1);
  printf("value of U is set to %u, should be 1\n", atomic_load(&U));
  int (*arr)[n+1] = P99_MALLOC(*arr);
  atomic_intp arrp = intp; //ATOMIC_VAR_INIT(0);
  atomic_store(&arrp, &(*arr)[0]);
  real_task(&arrp);
  for (size_t i = 0; i < n; ++i)
    thrd_create(&(*id)[i], task, &arrp);
  for (size_t i = 1; i < n; i += 2)
    thrd_detach((*id)[i]);
  mtx_lock(&mut);
  while (count < n/2) {
    printf("start of wait %zu\n", count);
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    if (ts.tv_nsec + 100000000 < 1000000000) ts.tv_nsec += 100000000;
    cnd_timedwait(&cond, &mut, &ts);
  }
  mtx_unlock(&mut);
  int res = 0;
  for (size_t i = 0; i < n; i += 2)
    thrd_join((*id)[i], &res);
  if (!(n % 5)) thrd_exit(1);
  printf("results are %d, %d, %zu and %g\n",
         res,
         atomic_load(&testvar).a,
         (size_t)(atomic_load(&intp) - &argc),
         atomic_load(&D));
  size_t len = 0;
  getenv_s(&len, 0, 0, "PATH");
  char path[len + 1];
  getenv_s(0, path, len + 1, "PATH");
  P99_PRINTF("environment PATH is %s\n", (char const*)path);
  quick_exit(EXIT_SUCCESS);
}
