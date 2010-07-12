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

#ifndef DOXYGEN
#undef declare_defarg
#define declare_defarg(A, B, C, D) define_defarg(A, B, C)
#undef DECLARE_DEFARG
#define DECLARE_DEFARG(A, ...) DEFINE_DEFARG(A, __VA_ARGS__)
#undef PROTOTYPE
#define PROTOTYPE(T, NAME, ...) enum { P99_PASTE2(_another_boring_enum_, NAME) }

#undef   	ORWL_POSIX_DEFAULT_H_
#define   	ORWL_POSIX_DEFAULT_H_ 1

#include "orwl_posix_default.h"
#endif
