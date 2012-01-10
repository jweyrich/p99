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

static_assert(true, "everything is alright");
// static_assert(false, "all goes wrong");

_Alignas(double) int a;

_Alignas(16) int b;

alignas(16) int c;

alignas(double) int d;

alignas(alignof(double)) int e;

noreturn
void stop(void) {
  abort();
}

thread_local int ret = EXIT_SUCCESS;

int main(void) {
  return ret;
}
