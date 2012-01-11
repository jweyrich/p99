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
#include "stdbool.h"
#include "stdlib.h"
#include "p99_compiler.h"
#include "p99_generic.h"

static_assert(true, "everything is alright");
// static_assert(false, "all goes wrong");

_Alignas(double) int a;

_Alignas(16) int b;

alignas(16) int c;

alignas(double) int d;

alignas(alignof(double)) int e;

// three tentative definitions of the same variable
unsigned char f[_Alignof(double)];
unsigned char f[__alignof(double)];
unsigned char f[__alignof__(double)];

int * g = P99_GENERIC(a, (void*)&d, (int, &a), (unsigned int, &a));
int * h = _Generic(a, default: (void*)&d, int: &a, unsigned int: &a);

noreturn
void stop(void) {
  abort();
}

thread_local int ret = EXIT_SUCCESS;

#define print_feature(X) if (p99_has_feature(X)) printf("has feature %s\n", #X)
#define print_attribute(X) if (p99_has_attribute(X)) printf("has attribute %s\n", #X)
#define print_extension(X) if (p99_has_extension(X)) printf("has extension %s\n", #X)
#define print_builtin(X) if (p99_has_builtin(X)) printf("has builtin %s\n", #X)

int main(void) {
  print_attribute(deprecated);
  print_attribute(weak);
  print_attribute(weakref);
  print_extension(c_generic_selections);
  print_feature(c_alignas);
  print_feature(c_alignof);
  print_feature(c_inline);
  print_feature(c_noreturn);
  print_feature(gnu_alignof);
  print_feature(gnu_thread_local);
  print_feature(statement_expression);
  print_feature(typeof);
  return ret;
}
