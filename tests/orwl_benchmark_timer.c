/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl.h"

int main(int argc, char **argv) {
  const size_t max = 1E+7;
  const struct timespec start = orwl_gettime();
  for (size_t i = 0; i < max; ++i) {
    orwl_gettime();
  }
  const struct timespec end = orwl_gettime();
  printf("Average time for orwl_gettime(): %f ns\n",
	 timespec2useconds(timespec_diff(start,end)) / (double)max * 1000);
  return(0);
}
