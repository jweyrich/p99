/*
** orwl_posix_default.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Mar 29 18:41:45 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_posix_default.h"
#include "p99_id.h"

#ifndef DOXYGEN
#undef declare_defarg
#define declare_defarg(A, B, C, D) define_defarg(A, B, C)
#undef P99_DECLARE_DEFARG
#define P99_DECLARE_DEFARG(A, ...) P99_DEFINE_DEFARG(A, __VA_ARGS__)
#undef P99_PROTOTYPE
#define P99_PROTOTYPE(T, NAME, ...) P99_MACRO_END(_another_boring, NAME)

#undef   	ORWL_POSIX_DEFAULT_H_
#define   	ORWL_POSIX_DEFAULT_H_ 1

#include "orwl_posix_default.h"
#endif
