#include "p99_atomic.h"

#define DETECT_TYPE(T, X) P99_GENERIC((X), false, (T, true))
#define DETECT_QVAL(T, X) P99_GENERIC(P99_QVALUE(X), false, (T, true))

#define SAYIT(MAC)                                                      \
  printf(#MAC ":\t%d for double lvalue\n", MAC(double const, (double const){ 0 })); \
  printf(#MAC ":\t%d for double rvalue\n", MAC(double const, (double const)0)); \
  printf(#MAC ":\t%d for int lvalue\n", MAC(int const, (int const){ 0 })); \
  printf(#MAC ":\t%d for int rvalue\n", MAC(int const, (int const)0)); \
  printf(#MAC ":\t%d for int lvalue conversion\n", MAC(int const, +((int const){ 0 }))); \
  printf(#MAC ":\t%d for int rvalue, no conversion\n", MAC(int const, +((int const)0))); \
  printf(#MAC ":\t%d for short lvalue, potential promotion\n", MAC(short const, (short const){ 0 })); \
  printf(#MAC ":\t%d for short rvalue, potential promotion\n", MAC(short const, (short const)0)); \
  printf(#MAC ":\t%d for short lvalue conversion and potential promotion\n", MAC(short const, +((short const){ 0 }))); \
  printf(#MAC ":\t%d for short rvalue and potential promotion\n", MAC(short const, +((short const)0))); \
  printf(#MAC ":\t%d for pointer\n", MAC(int const*, (int const*){ 0 })); \
  printf(#MAC ":\t%d for array, potential array-to-pointer conversion\n", MAC(int const[2], (int const[2]){ 0 }))


int main(void) {
  printf("P99 compiler:\t%40s\n", P99_COMPILER_VERSION);
  SAYIT(DETECT_TYPE);
  SAYIT(DETECT_QVAL);
}
