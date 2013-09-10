#include "p99_int.h"

#define P99_TSIG_NAME(N, I) P99_PASTE4(p00_, N, _, I)

#define P00_TSIG_DECLARE(N, X, I) char (*P99_TSIG_NAME(N, I))[1 + (((0ULL+(X))>>(I*16))&0xFFFF)]
#define P99_TSIG_DECLARE(N, X) P00_TSIG_DECLARE(N, X, 0); P00_TSIG_DECLARE(N, X, 1); P00_TSIG_DECLARE(N, X, 2); P00_TSIG_DECLARE(N, X, 3)



#define P00_TSIG_VAL(X, N, I) (sizeof(*(X).P99_TSIG_NAME(N, I))-1ULL)

#define P00_TSIG_CHARS(X, N, I)                 \
[2*I]     = P00_TSIG_VAL(X, N, I) & 0xFF,       \
[2*I + 1] = (P00_TSIG_VAL(X, N, I)>>8) & 0xFF

#define P99_TSIG_CHARS(X, N)                    \
  P00_TSIG_CHARS(X, N, 0),                      \
  P00_TSIG_CHARS(X, N, 1),                      \
  P00_TSIG_CHARS(X, N, 2),                      \
  P00_TSIG_CHARS(X, N, 3)

#define P00_TSIG_VAL_(X, N, I) (P00_TSIG_VAL(X, N, I)<<(I*16))

#define P99_TSIG_VAL(X, N)                      \
 P00_TSIG_VAL_(X, N, 0)                         \
|P00_TSIG_VAL_(X, N, 1)                         \
|P00_TSIG_VAL_(X, N, 2)                         \
|P00_TSIG_VAL_(X, N, 3)

#define P99_TSIG_CONV(T, X, N)                  \
(((union {                                      \
  T const p00_m;                                \
  /* Ensure that there are trailing 0's */      \
  unsigned char const p00_c[16];                \
} const){                                       \
    .p00_c = {                                  \
      P99_TSIG_CHARS(X, N)                      \
    }                                           \
  }).p00_m)


union oi {
  struct {
    unsigned a;
  };
  P99_TSIG_DECLARE(type_line, __LINE__);
  P99_TSIG_DECLARE(type_sig,  0xDEADBEAFDEADBEAFULL);
  P99_TSIG_DECLARE(type_name, 0x6E6f696E7520696FULL);
  P99_TSIG_DECLARE(type_magic,0X400921FB54442D18ULL);
  P99_TSIG_DECLARE(type_int,  0XFFFFFFFF00000000ULL);
  P99_TSIG_DECLARE(type_end,  0X0102030405060708ULL);
};


int main(void) {
  union oi oiObj = { 0u };
  printf("coded line %llu, integer value %#llX, size %zu\n",
         P99_TSIG_VAL(oiObj, type_line),
         P99_TSIG_VAL(oiObj, type_sig),
         sizeof(union oi));
  printf("coded value %" PRIX64 "\n", P99_TSIG_CONV(uint64_t, oiObj, type_sig));
  // we need an additional 0 character
  typedef char string8[9];
  printf("coded name %16s\n", P99_TSIG_CONV(string8, oiObj, type_name));
  printf("coded magic %.12g\n", P99_TSIG_CONV(double, oiObj, type_magic));
  printf("coded int %d\n", P99_TSIG_CONV(int, oiObj, type_int));
  printf("coded end %" PRIX64 "\n", P99_TSIG_CONV(uint64_t, oiObj, type_end));
}
