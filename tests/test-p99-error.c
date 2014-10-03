/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2012 Jens Gustedt, INRIA, France                     */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include <math.h>
#include "p99_c99_throw.h"

int main(void) {
  char errbuf[10] = P99_INIT;
  P99_STRERROR(EINVAL, sizeof errbuf, errbuf);
  fprintf_throw(stdout, "Testing P99_STRERROR: %s\n", errbuf);
  char const* message = "This should be ok.\n";
  int len = fprintf_throw(stdout, "%s", message);
  char* buf = memcpy(malloc_throw(len), message, len);
  fprintf_throw(stdout, "We just printed %d characters, buffer contains: %s", len, buf);
  fclose_throw(stdout);
  P99_TRY {
    fprintf_throw(stdout, "This should be caught.\n");
  } P99_CATCH(int err) {
    if (err) {
      fprintf_throw(stderr, "This is how we catch %s: %s.\n", p99_errno_getname(err), strerror(err));
      p99_jmp_report(err);
    }
  }
  ulong volatile found = 0;
  ldouble volatile founld = 0;
  P99_TRY {
    found = strtoul_throw("-42 is a correct value: ULONG_MAX -  42", 0, 0);
    founld = strtold_throw("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFP45653 has even a long double overflow", 0);
  } P99_CATCH(int err) {
    fprintf_throw(stderr, "The values found are %lu and %La\n", found, founld);
    switch (err) {
    case 0: break;
    case INT_MAX: fprintf_throw(stderr, "overflow\n"); break;
    case INT_MIN: fprintf_throw(stderr, "underflow\n"); break;
    default:       p99_jmp_report(err);
    }
  }
  P99_TRY {
    founld = strtold_throw("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFP-45653 should underflow", 0);
  } P99_CATCH(int err) {
    fprintf_throw(stderr, "The value for underflow is %La\n", founld);
    if (err) p99_jmp_report(err);
  }
  fprintf_throw(stdout, "This should abort because the exception is not caught.\n");
}
