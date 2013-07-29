/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2011-2012 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#include "p99_constraint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "p99_compiler.h"
#include "p99_generic.h"
#include "p99_atomic.h"

static_assert(true, "everything is alright");
// static_assert(false, "all goes wrong");

//_Alignas(double)
int a;

_Alignas(16) int b;

alignas(16) int c;

//alignas(double)
int d;

alignas(alignof(double)) int e;

// three tentative definitions of the same variable
unsigned char f[_Alignof(double)];
unsigned char f[__alignof(double)];
unsigned char f[__alignof__(double)];

int * g = P99_GENERIC(a, (void*)&d, (int, &a), (unsigned int, &a));
//int * h = _Generic(a, default: (void*)&d, int: &a, unsigned int: &a);

#define TYPESTR(T) (T, #T)

#define SIGNEDTYPE_(T, X) (unsigned T, (signed T)(X))

#define SSIZE_T(X)                                             \
P99_GENERIC(sizeof(1),                                         \
            ,                                                  \
            SIGNEDTYPE_(char, X),                              \
            SIGNEDTYPE_(short, X),                             \
            SIGNEDTYPE_(int, X),                               \
            SIGNEDTYPE_(long, X),                              \
            SIGNEDTYPE_(long long, X))

float funcf(float a, float b) {
  return a;
}
void funcf_2nd_arg_error(float);
double funcd(double a, double b) {
  return a;
}
void funcd_2nd_arg_error(double);

#define check(x, y)                                                  \
  (*(_Generic((x), float:  (float*){ 0 }, double: (double*){ 0 })    \
     = &_Generic((y), float:  (float){ y }, double: (double){ y })))

#define func(x, y)                                                            \
  _Generic((x),                                                               \
           float:  _Generic((y), float:  funcf, double: funcf_2nd_arg_error), \
           double: _Generic((y), double: funcd, float:  funcd_2nd_arg_error)  \
           )(x, check(x, y))

#define check2(x, y)                                                          \
  (*(P99_GENERIC((x), , (float,  (float*){ 0 }), (double, (double*){ 0 }))    \
     = &P99_GENERIC((y), , (float,  (float){ y }), (double, (double){ y }))))

#define func2(x, y)                                                                         \
  P99_GENERIC((x), ,                                                                        \
              (float,  P99_GENERIC((y), , (float,  funcf), (double, funcf_2nd_arg_error))), \
              (double, P99_GENERIC((y), , (double, funcd), (float,  funcd_2nd_arg_error)))  \
              )(x, check2(x, y))

noreturn
void stop(void) {
  abort();
}

thread_local int ret = EXIT_SUCCESS;

#define print_feature(X) if (p99_has_feature(X)) printf("has feature %s\n", #X)
#define print_attribute(X) if (p99_has_attribute(X)) printf("has attribute %s\n", #X)
#define print_extension(X) if (p99_has_extension(X)) printf("has extension %s\n", #X)
#define print_builtin(X) if (p99_has_builtin(X)) printf("has builtin %s\n", #X)

unsigned globA = P99_GENERIC(globA, ,(unsigned, 1));   // should work and not give diagnostic
//unsigned globB = P99_GENERIC(globB, ,(signed, 1));     // shouldn't work and give a diagnostic

int main(int argc, char* argv[]) {
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
  //print_feature(__typeof);
  print_feature(topeof);
  print_builtin(__sync_val_compare_and_swap);
  printf("list of default unsigned types is " P99_STRINGIFY(P00_UINT_TYPE_LIST) "\n");
  printf("list of atomic lock-free types is " P99_STRINGIFY(P99_ATOMIC_LOCK_FREE_TYPES) "\n");
  printf("atomic lock-free macros: %d %d %d %d %d %d %d\n",
         ATOMIC_BOOL_LOCK_FREE,
         ATOMIC_CHAR_LOCK_FREE,
         ATOMIC_SHORT_LOCK_FREE,
         ATOMIC_INT_LOCK_FREE,
         ATOMIC_LONG_LOCK_FREE,
         ATOMIC_LLONG_LOCK_FREE,
         ATOMIC_POINTER_LOCK_FREE
        );
  //P99_GENERIC(1.0, ,(int, P99_NOP)); // shouldn't work
  P99_GENERIC(1, ,(int, P99_NOP));   // should work and not give diagnostic
  func2(1.0f, 2.0f);  // should work, but doesn't
  //func2(1.0f, 2.0); // shouldn't work
  //func2(1.0,  2.0f);  // shouldn't work
  func2(1.0,  2.0); // should work
  //func2(1,  2.0); // shouldn't work
  /* func(1.0f, 2.0f);  // should work, but doesn't */
  /* func(1.0f, 2.0); // shouldn't work */
  /* func(1.0,  2.0f);  // shouldn't work */
  /* func(1.0,  2.0); // should work, but doesn't */
  if (P99_SIZE_INDICATOR(sizeof(int), 1, 2, 4, 8)) {
    printf("sizeof int is one of 1, 2, 4, 8\n");
  } else {
    printf("sizeof int is weird\n");
  }
  /* Test both branches of P99_SIZE_INDICATOR */
  if (!P99_SIZE_INDICATOR(sizeof(char[3]), 1, 2, 4, 8)
      && P99_SIZE_INDICATOR(sizeof(char[3]), 3)) {
    printf("sizeof char[3] seems ok\n");
  } else {
    printf("sizeof char[3] is weird\n");
  }
  /* Test the minmax value for P99_SIZE_INDICATOR */
  if (P99_SIZE_INDICATOR(UINT16_MAX, 1, 2, 4, 8)) {
    printf("UINT16_MAX (%" PRIu16 ") is weird\n", UINT16_MAX);
  } else {
    printf("UINT16_MAX (%" PRIu16 ") seems ok\n", UINT16_MAX);
  }
  printf("size_t is %s\n",
         P99_GENERIC(sizeof(1),
                     ,
                     TYPESTR(unsigned char),
                     TYPESTR(unsigned short),
                     TYPESTR(unsigned int),
                     TYPESTR(unsigned long),
                     TYPESTR(unsigned long long)
                    ));

  printf("ssize_t is %s\n",
         P99_GENERIC(SSIZE_T(1),
                     ,
                     TYPESTR(signed char),
                     TYPESTR(signed short),
                     TYPESTR(signed int),
                     TYPESTR(signed long),
                     TYPESTR(signed long long)
                    ));

  unsigned Arr[7] = { 0 };
  printf("array length is %zu, pointer is %zu\n",
         P99_OBJLEN(Arr, unsigned, signed, float, double),
         P99_OBJLEN(&Arr[0], unsigned, signed, float, double)
        );
  double Ard[7] = { 0 };
  printf("array length is %zu, pointer is %zu\n",
         P99_OBJLEN(Ard, unsigned, signed, float, double),
         P99_OBJLEN(&Ard[0], unsigned, signed, float, double)
        );
  char Arc[7] = { 0 };
  printf("array length is %zu, pointer is %zu\n",
         P99_OBJLEN(Arc, char, unsigned, signed),
         P99_OBJLEN(&Arc[0], char, unsigned, signed)
        );
  unsigned char Aruc[7] = { 0 };
  printf("array length is %zu, pointer is %zu\n",
         P99_OBJLEN(Aruc, unsigned char, signed),
         P99_OBJLEN(&Aruc[0], unsigned char, signed)
        );
  signed char Arsc[7] = { 0 };
  printf("array length is %zu, pointer is %zu\n",
         P99_OBJLEN(Arsc, signed char, unsigned),
         P99_OBJLEN(&Arsc[0], signed char, unsigned)
        );
  char volatile aChar = 'a';
  P99_PRINTF("print generic from float %s, long double complex %s, a pointer %s, int %s, uint128 %s (%s), _Bool %s and char %s\n",
             5.3f,
             CMPLXL(54, 1E-181),
             Ard,
             argc,
#ifdef P99X_UINT128_MAX
             (p99x_uint128)1020304050607 * 100000,
             (p99x_uint128)0x0,
#else
             UINT64_C(1020304050607) * 100000,
             UINT64_C(0),
#endif
             (bool)(argc > 1),
             aChar);

  double base[25] = { 0 };
  double * somewhere = base + 13;
  double * elsewhere = base + 25;
  P99_PRINTF("checking generic range test, pointers: base=%s, base1=%s, inside=%s, outside=%s\n",
             P99_IN_RANGE(base, base, 25 * sizeof base[0]),
             P99_IN_RANGE(base + 1, base, 25 * sizeof base[0]),
             P99_IN_RANGE(somewhere, base, 25 * sizeof base[0]),
             P99_IN_RANGE(elsewhere, base, 25 * sizeof base[0]));
  P99_PRINTF("checking generic range test, integers: base=%s, inside=%s, upper=%s, outside=%s\n",
             P99_IN_RANGE(1, 1u, 7),
             P99_IN_RANGE(5, 1u, 7),
             P99_IN_RANGE(8, 1u, 7),
             P99_IN_RANGE(28, 1, 7));
  P99_PRINTF("checking generic range test, floats: base=%s, inside=%s, upper=%s, outside=%s\n",
             P99_IN_RANGE(1.0, 1u, 7.f),
             P99_IN_RANGE(5.0, 1u, 7.f),
             P99_IN_RANGE(8.0, 1u, 7),
             P99_IN_RANGE(28.0, 1, 7));
  static char nullptr0[P99_IS_NULLPTR_CONSTANT(0)+1];
  static char nullptr1[P99_IS_NULLPTR_CONSTANT((void*)0)+1];
  enum p00_nullptr_enum { p00_nullptr };
  static char nullptr2[P99_IS_NULLPTR_CONSTANT(p00_nullptr)+1];
  static char nullptr3[P99_IS_NULLPTR_CONSTANT((enum p00_nullptr_enum)0)+1];
  static char nullptr4[P99_IS_NULLPTR_CONSTANT((void const*)0)+1];
  static char nullptr5[P99_IS_NULLPTR_CONSTANT((void*)1)+1];
  static void *const ptr = 0;
  static char nullptr6[P99_IS_NULLPTR_CONSTANT(ptr)+1];
  printf("Testing for null pointer constant.\n"
         "\tshould all be true:\t0 (%s),"
         " (void*)0 (%s),\n"
         "\t\t\t\tenumeration constant (%s),"
         " constant of enum type (%s)\n"
         "\tshould all be false:\t(void const*)0 (%s),"
         " (void*)1 (%s),"
         " void*const ptr (%s)\n",
         bool_getname(2 == sizeof nullptr0),
         bool_getname(2 == sizeof nullptr1),
         bool_getname(2 == sizeof nullptr2),
         bool_getname(2 == sizeof nullptr3),
         bool_getname(2 == sizeof nullptr4),
         bool_getname(2 == sizeof nullptr5),
         bool_getname(2 == sizeof nullptr6)
         );

  static char integral_constant0[P99_IS_INTEGRAL_CONSTANT(7)+1];
  enum p00_integral_constant_enum { p00_integral_constant = 1 };
  static char integral_constant1[P99_IS_INTEGRAL_CONSTANT(p00_integral_constant)+1];
  static char integral_constant2[P99_IS_INTEGRAL_CONSTANT((enum p00_integral_constant_enum)1)+1];
  static char integral_constant3[P99_IS_INTEGRAL_CONSTANT(7.0)+1];
  static char integral_constant4[P99_IS_INTEGRAL_CONSTANT(!!0.0)+1];
  static int const val = 0;
  static char integral_constant5[P99_IS_INTEGRAL_CONSTANT(val)+1];
  static char integral_constant6[P99_IS_INTEGRAL_CONSTANT((char*)1)+1];
  /* may produce a warning or error*/
  static char integral_constant7[P99_IS_INTEGRAL_CONSTANT((void*)0)+1];
  printf("Testing for integral constant.\n"
         "\tshould all be true:"
         "\t7 (%s),"
         "\n"
         "\t\t\t\tenumeration constant (%s),"
         "\tconstant of enum type (%s)\n"
         "\tshould all be false:"
         "\t7.0 (%s),"
         "\t!!0.0 (%s),"
         "\tint const val (%s),\n"
         "\t\t\t\t(char*)1 (%s),"
         "\t(void*)0 (%s),\n",
         bool_getname(2 == sizeof integral_constant0),
         bool_getname(2 == sizeof integral_constant1),
         bool_getname(2 == sizeof integral_constant2),
         bool_getname(2 == sizeof integral_constant3),
         bool_getname(2 == sizeof integral_constant4),
         bool_getname(2 == sizeof integral_constant5),
         bool_getname(2 == sizeof integral_constant6),
         bool_getname(2 == sizeof integral_constant7)
         );

  printf("NULL\t\t(" P99_STRINGIFY(NULL) ")\t\tis %sa null pointer.\n",
         P99_IS_NULLPTR(NULL) ? "" : "not ");
#undef NULL
#define NULL UINTPTR_C(0)

  printf("redefined to\t(" P99_STRINGIFY(NULL) ")\tis %sa null pointer.\n",
         P99_IS_NULLPTR(NULL) ? "" : "not ");

#undef NULL
#define NULL ((void*)0)

  printf("redefined to\t(" P99_STRINGIFY(NULL) ")\t\tis %sa null pointer.\n",
         P99_IS_NULLPTR(NULL) ? "" : "not ");

  char line[] = "this is a, long text -- it has some special charActers\nnewlines?";
  puts("--- end ---");

  do {
    printf("parsing: %s\n", line);
    char *ptr = 0;
    rsize_t s1max = sizeof line;
    for(char * tok;
        s1max && (tok = strtok_s(ptr ? 0 : line, &s1max, "A-\n? ,", &ptr));)
      puts(tok);
    tmpnam_s(line, sizeof line);
    puts(line);
  }  while(gets_s(line, sizeof line) && line[0]);

  return ret;
}
