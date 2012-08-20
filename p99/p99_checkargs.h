/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holder for this work is as follows:              */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_CHECKARGS_H_
# define    P99_CHECKARGS_H_

/**
 ** @file
 ** @brief Macros to check arguments to functions, in particular of
 ** variably modified types.
 **/

#include "p99_map.h"
#include "p99_enum.h"
#include "p99_type.h"
#include P99_ADVANCE_ID

#define P00_CA_ASIZE(X) size_t const P99_PASTE2(p00_ca_asize_, X)
#define P00_CA_ASIZES(N, ...) P99_SEQ(P00_CA_ASIZE, __VA_ARGS__)

#define P00_CA_PSIZE(X) size_t const P99_PASTE2(p00_ca_psize_, X)
#define P00_CA_PSIZES(N, ...) P99_SEQ(P00_CA_PSIZE, __VA_ARGS__)

#define P00_CA_FSIZE(_0, _1, I) size_t P99_PASTE2(p00_ca_fsize_, I); P99_UNUSED(P99_PASTE2(p00_ca_fsize_, I))
#define P00_CA_FSIZES(N, ...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_CA_FSIZE, __VA_ARGS__)

#define P00_CA_TYPEDEF(X) typedef X
#define P00_CA_TYPEDEFS(N, ...) P99_SEP(P00_CA_TYPEDEF, P99_REVS(__VA_ARGS__))

#define P00_CA_FSIZEOF(_0, X, I) P99_PASTE2(p00_ca_fsize_, I) = sizeof(X)
#define P00_CA_FSIZEOFS(N, ...) P99_FOR(, N, P00_SEP, P00_CA_FSIZEOF, __VA_ARGS__)

#define P00_CA_MANGLE_JOIN(X) _, X
#define P00_CA_MANGLE_LIST_(...) P99_PASTE(__VA_ARGS__)
#define P00_CA_MANGLE_LIST(...) P00_CA_MANGLE_LIST_(P99_SEQ(P00_CA_MANGLE_JOIN, __VA_ARGS__))

#define P00_CA_MANGLE(NAME, ACHECKS, PCHECKS)   \
P99_PASTE(p00_ca_,                              \
          NAME,                                 \
          _p00_achecks,                         \
          P00_CA_MANGLE_LIST ACHECKS,           \
          _p00_pchecks,                         \
          P00_CA_MANGLE_LIST PCHECKS            \
          )

#define P00_CA_ACHECK(X)                                                \
if (P99_UNLIKELY                                                        \
    (                                                                   \
     /* passing in a larger array is ok */                              \
     (P99_PASTE2(p00_ca_fsize_, X) > P99_PASTE2(p00_ca_asize_, X))      \
     )                                                                  \
    ) {                                                                 \
  fprintf(stderr,                                                       \
          "%s, call from %s, size check failed for parameter number " P99_STRINGIFY(X) ", is %zu instead of %zu\n", \
          p00_proto,                                                    \
          p00_call,                                                     \
          P99_PASTE2(p00_ca_asize_, X),                                 \
          P99_PASTE2(p00_ca_fsize_, X)                                  \
          );                                                            \
  abort();                                                              \
 }

#define P00_CA_ACHECKS(N, ...) P99_SEP(P00_CA_ACHECK, __VA_ARGS__)

#define P00_CA_PCHECK(LIST, X, I)                                       \
size_t const P99_PASTE2(p00_ca_lsize_, I) = sizeof((P99_CHS(X, P00_ROBUST LIST))[0]); \
if (P99_UNLIKELY                                                        \
    (                                                                   \
     (P99_PASTE2(p00_ca_lsize_, I) != P99_PASTE2(p00_ca_psize_, X))     \
     )                                                                  \
    ) {                                                                 \
  fprintf(stderr,                                                       \
          "%s, call from %s, pointed to size check failed for parameter number " P99_STRINGIFY(X) ", is %zu instead of %zu\n", \
          p00_proto,                                                    \
          p00_call,                                                     \
          P99_PASTE2(p00_ca_psize_, X),                                 \
          P99_PASTE2(p00_ca_lsize_, I)                                  \
          );                                                            \
  abort();                                                              \
 }

#define P00_CA_PCHECKS(LIST, N, ...) P99_FOR(LIST, P99_NARG(__VA_ARGS__), P00_SEP, P00_CA_PCHECK, __VA_ARGS__)


#define P00_CA_WRAP_DECLARE(NAME, RET, TYPES, VARS, ACHECKS, PCHECKS)   \
  /* clang tries to be helpful here, but we know better */              \
P99_IF_COMPILER(CLANG, GCC diagnostic push)                             \
P99_IF_COMPILER(CLANG, GCC diagnostic ignored "-Wsizeof-array-argument") \
inline                                                                  \
RET P00_CA_MANGLE(NAME, ACHECKS, PCHECKS)                                      \
     (                                                                  \
      char const* p00_call,                                             \
      P99_IF_EMPTY ACHECKS()(P00_CA_ASIZES(P99_NARG ACHECKS, P00_ROBUST ACHECKS),) \
      P99_IF_EMPTY PCHECKS()(P00_CA_PSIZES(P99_NARG PCHECKS, P00_ROBUST PCHECKS),) \
      P00_ROBUST TYPES) {                                               \
  char const*const p00_proto = P99_STRINGIFY(NAME) P99_STRINGIFY(TYPES); \
  P99_UNUSED(p00_proto);                                                \
  P99_IF_EMPTY ACHECKS()                                                \
       (                                                                \
        P00_CA_FSIZES(P99_NARG VARS, P00_ROBUST VARS);                  \
       {                                                                \
         P00_CA_TYPEDEFS(P99_NARG TYPES, P00_ROBUST TYPES);             \
         P00_CA_FSIZEOFS(P99_NARG VARS, P00_ROBUST VARS);               \
       }                                                                \
        P00_CA_ACHECKS(P99_NARG ACHECKS, P00_ROBUST ACHECKS);           \
       )                                                                \
  P00_CA_PCHECKS(VARS, P99_NARG PCHECKS, P00_ROBUST PCHECKS);           \
  /* Do a type check without re-declaring the function */               \
  RET (*P99_PASTE2(p00_ca_ft_, NAME)) TYPES = NAME;                     \
  P99_UNUSED(P99_PASTE2(p00_ca_ft_, NAME));                             \
  P99_IF_EQ_1(P99_IS_TOK(void, RET))()(return)                          \
    NAME VARS;                                                          \
}                                                                       \
P99_IF_COMPILER(CLANG, GCC diagnostic pop)                              \
P99_MACRO_END(P00_CA_WRAP_RET_DECLARE)

#define P00_CA_WRAP_DECLARE2(NAME, RET, TYPES, VARS, ACHECKS, PCHECKS)  \
P00_CA_WRAP_DECLARE(NAME, RET, TYPES, VARS,                             \
                   P99_IF_EMPTY ACHECKS(())(ACHECKS),                   \
                   P99_IF_EMPTY PCHECKS(())(PCHECKS))


#define P99_CA_WRAP_DECLARE(NAME, RET, TYPES, VARS, ...)                \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                                     \
(P00_CA_WRAP_DECLARE(NAME, RET, TYPES, VARS, __VA_ARGS__, __VA_ARGS__)) \
(P00_CA_WRAP_DECLARE(NAME, RET, TYPES, VARS, __VA_ARGS__))

#define P00_CA_WRAP_DEFINE(NAME, RET, TYPES, VARS, ACHECKS, PCHECKS)    \
  P00_INSTANTIATE(RET,                                                  \
                  P00_CA_MANGLE(NAME, ACHECKS, PCHECKS),                \
                  char const* p00_call,                                 \
                  P99_IF_EMPTY ACHECKS()(P00_CA_ASIZES(P99_NARG ACHECKS, P00_ROBUST ACHECKS),) \
                  P99_IF_EMPTY PCHECKS()(P00_CA_PSIZES(P99_NARG PCHECKS, P00_ROBUST PCHECKS),) \
                  P00_ROBUST TYPES)

#define P00_CA_WRAP_DEFINE2(NAME, RET, TYPES, VARS, ACHECKS, PCHECKS)   \
P00_CA_WRAP_DEFINE(NAME, RET, TYPES, VARS,                              \
                   P99_IF_EMPTY ACHECKS(())(ACHECKS),                   \
                   P99_IF_EMPTY PCHECKS(())(PCHECKS))

#define P99_CA_WRAP_DEFINE(NAME, RET, TYPES, VARS, ...)                 \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                                     \
(P00_CA_WRAP_DEFINE(NAME, RET, TYPES, VARS, __VA_ARGS__, __VA_ARGS__))  \
(P00_CA_WRAP_DEFINE(NAME, RET, TYPES, VARS, __VA_ARGS__))

#define P00_CA_SIZEOF(LIST, X, _2) sizeof(P99_CHS(X, P00_ROBUST LIST))
#define P00_CA_SIZEOFS(LIST, N, ...) P99_FOR(LIST, N, P00_SEQ, P00_CA_SIZEOF, __VA_ARGS__)

#define P00_CA_PSIZEOF(LIST, X, _2) sizeof(P99_CHS(X, P00_ROBUST LIST)[0])
#define P00_CA_PSIZEOFS(LIST, N, ...) P99_FOR(LIST, N, P00_SEQ, P00_CA_PSIZEOF, __VA_ARGS__)


#ifndef P99_NO_CHECKARGS
#define P99_CA_CALL(NAME, ACHECKS, PCHECKS, ...)                        \
P00_CA_MANGLE(NAME, ACHECKS, PCHECKS)                                   \
(                                                                       \
 __FILE__ ":" P99_STRINGIFY(__LINE__) ": "                              \
 P99_STRINGIFY(NAME) P99_STRINGIFY((__VA_ARGS__)),                      \
 P99_IF_EMPTY ACHECKS()(P00_CA_SIZEOFS((__VA_ARGS__), P99_NARG ACHECKS, P00_ROBUST ACHECKS),) \
 P99_IF_EMPTY PCHECKS()(P00_CA_PSIZEOFS((__VA_ARGS__), P99_NARG PCHECKS, P00_ROBUST PCHECKS),) \
 __VA_ARGS__                                                            \
)
#else
#define P99_CA_CALL(NAME, ACHECKS, PCHECKS, ...)  NAME(__VA_ARGS__)
#endif

#endif
