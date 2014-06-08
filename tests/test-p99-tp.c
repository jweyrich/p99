#include "p99_threads.h"
#include "p99_tp.h"

/* Just to see in the debugger what the real type of this is. */
_Atomic(p00_tp_glue)* f(void) {
  return 0;
}

_Atomic(unsigned)* g(void) {
  return 0;
}

P99_DECLARE_STRUCT(p99_8);

struct p99_8 {
  unsigned char a[8];
};

P99_DECLARE_ATOMIC(p99_8);

_Atomic(p99_8)* h(void) {
  return 0;
}

P99_DECLARE_STRUCT(p99_16);

struct p99_16 {
  unsigned char a[16];
};

P99_DECLARE_ATOMIC(p99_16);

_Atomic(p99_16)* i(void) {
  return 0;
}

#define test_lockfree(T) printf(#T " is lock free:\t%s\n", bool_getname(atomic_is_lock_free(&(_Atomic(T))P99_INIT)))

typedef unsigned* up;
P99_TP_DECLARE(up);

int main(void) {
  f();
  g();
  h();
  i();
  test_lockfree(unsigned);
  test_lockfree(p00_tp_glue);
  test_lockfree(p99_8);
  test_lockfree(p99_16);
  P99_TP(up) tp = P99_TP_INITIALIZER(0);
  void * fut = &(unsigned){ 37 };
  P99_TP_TYPE_STATE(&tp) st = P99_TP_STATE_INITIALIZER(&tp, fut);
  void * val = P99_TP_STATE_GET(&st);
  printf("val is %p\n", val);
  return EXIT_SUCCESS;
}
