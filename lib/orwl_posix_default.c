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

#undef declare_defarg
#define declare_defarg(A, B, C, D) define_defarg(A, B, C)

declare_defarg(pthread_barrier_init, 1, pthread_barrierattr_t*, NULL);
declare_defarg(pthread_cond_init, 1, pthread_condattr_t*, NULL);
declare_defarg(pthread_mutex_init, 1, pthread_mutexattr_t*, NULL);
declare_defarg(pthread_rwlock_init, 1, pthread_rwlockattr_t*, NULL);
declare_defarg(pthread_spin_init, 1, int, PTHREAD_PROCESS_PRIVATE);

declare_defarg(strtoul, 2, int, 0);
declare_defarg(strtoul, 1, char **, NULL);
declare_defarg(strtoull, 2, int, 0);
declare_defarg(strtoull, 1, char **, NULL);
declare_defarg(strtouf, 1, char **, NULL);
declare_defarg(strtoud, 1, char **, NULL);
declare_defarg(strtould, 1, char **, NULL);

declare_defarg(accept, 2, socklen_t*, &TNULL(socklen_t));
declare_defarg(accept, 1, struct sockaddr *, NULL);
declare_defarg(socket, 2, int, 0);
declare_defarg(socket, 1, int, SOCK_STREAM);
