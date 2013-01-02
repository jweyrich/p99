/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:            */
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
# define P99_AT_LOAD(X, ...) __attribute__((constructor P99_IF_EMPTY(__VA_ARGS__)()((__VA_ARGS__)))) void X(void)
#else
# define P99_AT_LOAD(X, ...) void X(void); P99_PRAGMA(startup X __VA_ARGS__) void X(void)
#endif

#if p99_has_attribute(destructor)
# define P99_AT_UNLOAD(X, ...) __attribute__((destructor P99_IF_EMPTY(__VA_ARGS__)()((__VA_ARGS__)))) void X(void)
#else
# define P99_AT_UNLOAD(X, ...) void X(void); P99_PRAGMA(exit X __VA_ARGS__) void X(void)
#endif

#endif      /* !P99_HOOK_H_ */
