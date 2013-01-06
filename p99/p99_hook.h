/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                            */
/* (C) copyright  2013 Pierre-Nicolas Clauss                                  */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef     P99_HOOK_H_
# define    P99_HOOK_H_

/**
 ** @file
 ** @brief Compiler specific hook functions to execute at load/unload time.
 **
 ** P99_AT_LOAD functions are executed before main() for the main program and
 ** when loading for dynamic libraries. P99_AT_UNLOAD functions are executed
 ** before exiting for the main program and when unloading for dynamic libraries
 **/

#include "p99_compiler.h"
#include "p99_if.h"
#include "p99_list.h"

#if p99_has_attribute(constructor)
# define P99_AT_LOAD_DECLARE(...)                                                             \
__attribute__((constructor P99_IF_LE(P99_NARG(__VA_ARGS__), 1)()((P99_SKP(1, __VA_ARGS__))))) \
void P99_CHS(0, __VA_ARGS__)(void)
# define P99_AT_LOAD_DEFINE P99_AT_LOAD_DECLARE
#else
# define P99_AT_LOAD_DECLARE(...)                                                   \
void P99_CHS(0, __VA_ARGS__)(void);                                                 \
P99_PRAGMA(startup P99_FOR(, P99_NARG(__VA_ARGS__), P00_SER, P00_IDT, __VA_ARGS__)) \
P99_MACRO_END(P99_AT_LOAD_DECLARE)
# define P99_AT_LOAD_DEFINE(...) void P99_CHS(0, __VA_ARGS__)(void)
#endif

#if p99_has_attribute(destructor)
# define P99_AT_UNLOAD_DECLARE(...)                                                          \
__attribute__((destructor P99_IF_LE(P99_NARG(__VA_ARGS__), 1)()((P99_SKP(1, __VA_ARGS__))))) \
void P99_CHS(0, __VA_ARGS__)(void)
# define P99_AT_UNLOAD_DEFINE P99_AT_UNLOAD_DECLARE
#else
# define P99_AT_UNLOAD_DECLARE(...)                                              \
void P99_CHS(0, __VA_ARGS__)(void);                                              \
P99_PRAGMA(exit P99_FOR(, P99_NARG(__VA_ARGS__), P00_SER, P00_IDT, __VA_ARGS__)) \
P99_MACRO_END(P99_AT_UNLOAD_DECLARE)
# define P99_AT_UNLOAD_DEFINE(...) void P99_CHS(0, __VA_ARGS__)(void)
#endif

#endif      /* !P99_HOOK_H_ */
