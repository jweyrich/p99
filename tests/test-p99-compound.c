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
/**
 ** @file
 ** @brief Test the behavior of compound literals
 **
 ** Compound literals may lead to the propagation of stack pointers
 ** outside there scope of validity. This little program tests the
 ** behavior of your compiler with respect to these, in particular if
 ** it detects the misbehavior.
 **
 ** All my compilers with just -Wall for the moment only detect
 ** ::myLessBuggyFunction, i.e an explicit temporary that is returned
 ** from a function.
 **
 ** Compound literal proliferation in myBuggyFunction is only detected
 ** by clang with the --analyze option.
 **
 ** valgrind detects this misshavior at run time
 **/


#include <stdio.h>
#include <string.h>

enum { len = 9 };


char const* myVerryBuggyFunction(void) {
  return memset((char[len + 1]){ 0 }, 'a', len);
}

char const* myStillBuggyFunction(void) {
  char tmp[len + 1] = { 0 };
  return memset(tmp, 'a', len);
}

char const* myLessBuggyFunction(void) {
  char tmp[len + 1] = { "aaaaaaaaa" };
  return tmp;
}

char const* myBuggyFunction(void) {
  return (char[len + 1]){ 0 };
}

int main(int argc, char*argv[]) {
  printf("return %u 'a's: %s\n", len, myVerryBuggyFunction());
  printf("return %u 'a's: %s\n", len, myStillBuggyFunction());
  printf("return %u 'a's: %s\n", len, myLessBuggyFunction());
  printf("return two temporaries? %p ?= %p\n",
         (void*)myBuggyFunction(),
         (void*)myBuggyFunction());
  return 0;
}