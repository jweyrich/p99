/* This may look like nonsense, but it really is -*- mode: C; coding: utf-8 -*- */
/*                                                                              */
/* Except for parts copied from previous work and as explicitly stated below,   */
/* the authors and copyright holders for this work are as follows:              */
/* (C) copyright  2013 Jens Gustedt, INRIA, France                              */
/* (C) copyright  2013 Pierre-Nicolas Clauss                                    */
/*                                                                              */
/* This file is free software; it is part of the P99 project.                   */
/*                                                                              */
/* Licensed under the Apache License, Version 2.0 (the "License");              */
/* you may not use this file except in compliance with the License.             */
/* You may obtain a copy of the License at                                      */
/*                                                                              */
/*     http://www.apache.org/licenses/LICENSE-2.0                               */
/*                                                                              */
/* Unless required by applicable law or agreed to in writing, software          */
/* distributed under the License is distributed on an "AS IS" BASIS,            */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     */
/* See the License for the specific language governing permissions and          */
/* limitations under the License.                                               */
/*                                                                              */
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

/**
 ** @addtogroup startup_hooks Hooks executed at startup
 ** @{
 **/


/**
 ** @def P99_AT_LOAD_DECLARE(...)
 **
 ** Declare a function that is to be executed at startup time.
 ** @see P99_INIT_FUNCTION_DECLARE for the preferred more general function
 ** @see P99_INIT_VARIABLE for such a function that is called with a @c void* argument
 ** @see P99_AT_LOAD_DEFINE to define the corresponding function
 **/

/**
 ** @def P99_AT_LOAD_DEFINE(...)
 **
 ** Define the function that has been declared with ::P99_AT_LOAD_DECLARE.
 **/

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

/**
 ** @def P99_AT_UNLOAD_DECLARE(...)
 **
 ** Declare a function that is to be executed at program exit or
 ** library unload.  For program exit this function is more or less
 ** superfluous, use the C99 and C11 feature @c atexit, or the C11
 ** ::at_quick_exit, instead.
 **
 ** @see P99_AT_UNLOAD_DEFINE to define the corresponding function
 **/

/**
 ** @def P99_AT_UNLOAD_DEFINE(...)
 **
 ** Define the function that has been declared with ::P99_AT_UNLOAD_DECLARE.
 **/

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


/**
 ** @}
 **/


#endif      /* !P99_HOOK_H_ */
