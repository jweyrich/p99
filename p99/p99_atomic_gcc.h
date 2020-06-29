/* This may look like nonsense, but it really is -*- mode: C; coding: utf-8 -*- */
/*                                                                              */
/* Except for parts copied from previous work and as explicitly stated below,   */
/* the author and copyright holder for this work is                             */
/* (C) copyright  2012, 2014 Jens Gustedt, INRIA, France                        */
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
#ifndef P99_ATOMIC_GCC_H
#define P99_ATOMIC_GCC_H 1

#ifndef P99_ATOMIC_H
# warning "never include this file directly, use p99_atomic.h, instead"
#endif

#include "p99_args.h"

/**
 ** @file
 **
 ** @brief Implement some of the atomic operations as fallback to gcc'
 ** builtins.
 **/


#if defined(__ATOMIC_RELAXED) && !defined(P99_GCC_USE_SYNC)
# include "p99_atomic_gcc_atomic.h"
#else
# include "p99_atomic_gcc_sync.h"
#endif

#endif
