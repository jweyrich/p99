#include <stdio.h>

#include "p99.h"

typedef struct {
  struct {
   double b;
   int    a;
  } x;
   char   f[];
} S;

int main(void) {
   printf("size -> %zu\n", P99_FSIZEOF(S, f, 10));
   return 0;
}
