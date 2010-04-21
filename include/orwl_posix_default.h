/*
** orwl_posix_default.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Mar 29 18:40:36 2010 Jens Gustedt
** Last update Mon Mar 29 18:40:36 2010 Jens Gustedt
*/

#ifndef   	ORWL_POSIX_DEFAULT_H_
/*! @brief Control the include of "orwl_posix_default.h"
**
** The special value 1 will be used in a second include to do the
** instantiation.
**/
# define   	ORWL_POSIX_DEFAULT_H_ 0
#endif

#if   	ORWL_POSIX_DEFAULT_H_ < 2
#if   	ORWL_POSIX_DEFAULT_H_ == 0
# undef   	ORWL_POSIX_DEFAULT_H_
# define   	ORWL_POSIX_DEFAULT_H_ 2
#endif

#include "orwl_macro.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

// pthread_barrier_init (3posix) - destroy and initialize a barrier object (ADVANCED REALTIME THREADS)
// not possible because of the count argument

 // pthread_cond_init (3posix) - destroy and initialize condition variables
DEFARG_SIGNATURE(int, pthread_cond_init, pthread_cond_t*, pthread_condattr_t const*);
#define pthread_cond_init(...) CALL_WITH_DEFAULTS(pthread_cond_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_cond_init, , NULL);

 // pthread_mutex_init (3posix) - destroy and initialize a mutex
DEFARG_SIGNATURE(int, pthread_mutex_init, pthread_mutex_t*, pthread_mutexattr_t const*);
#define pthread_mutex_init(...) CALL_WITH_DEFAULTS(pthread_mutex_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_mutex_init, , NULL);

 // pthread_rwlock_init (3posix) - destroy and initialize a read-write lock object
DEFARG_SIGNATURE(int, pthread_rwlock_init, pthread_rwlock_t*, pthread_rwlockattr_t const*);
#define pthread_rwlock_init(...) CALL_WITH_DEFAULTS(pthread_rwlock_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_rwlock_init, , NULL);

 // pthread_spin_init (3posix) - destroy or initialize a spin lock object (ADVANCED REALTIME THREADS)
DEFARG_SIGNATURE(int, pthread_spin_init, pthread_spinlock_t*, int);
#define pthread_spin_init(...) CALL_WITH_DEFAULTS(pthread_spin_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_spin_init, , PTHREAD_PROCESS_PRIVATE);

// int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
#if   	ORWL_POSIX_DEFAULT_H_ == 2
typedef void (*pthread_key_create_arg1_t)(void *);
#endif
DEFARG_SIGNATURE(int, pthread_key_create, pthread_key_t*, pthread_key_create_arg1_t);
#define pthread_key_create(...) CALL_WITH_DEFAULTS(pthread_key_create, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_key_create, , NULL);


DEFARG_SIGNATURE(long, strtol, char const*, char**, int);
#define strtol(...) CALL_WITH_DEFAULTS(strtol, 3, __VA_ARGS__)
DECLARE_DEFARG(strtol, , NULL, 0);

DEFARG_SIGNATURE(long long, strtoll, char const*, char**, int);
#define strtoll(...) CALL_WITH_DEFAULTS(strtoll, 3, __VA_ARGS__)
DECLARE_DEFARG(strtoll, , NULL, 0);

DEFARG_SIGNATURE(ulong, strtoul, char const*, char**, int);
#define strtoul(...) CALL_WITH_DEFAULTS(strtoul, 3, __VA_ARGS__)
DECLARE_DEFARG(strtoul, , NULL, 0);

DEFARG_SIGNATURE(unsigned long long, strtoull, char const*, char**, int);
#define strtoull(...) CALL_WITH_DEFAULTS(strtoull, 3, __VA_ARGS__)
DECLARE_DEFARG(strtoull, , NULL, 0);

DEFARG_SIGNATURE(float, strtof, char const*, char**);
#define strtof(...) CALL_WITH_DEFAULTS(strtof, 2, __VA_ARGS__)
DECLARE_DEFARG(strtof, , NULL);

DEFARG_SIGNATURE(double, strtod, char const*, char**);
#define strtod(...) CALL_WITH_DEFAULTS(strtod, 2, __VA_ARGS__)
DECLARE_DEFARG(strtod, , NULL);

DEFARG_SIGNATURE(long double, strtold, char const*, char**);
#define strtold(...) CALL_WITH_DEFAULTS(strtold, 2, __VA_ARGS__)
DECLARE_DEFARG(strtold, , NULL);


// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
DEFARG_SIGNATURE(int, accept, int, struct sockaddr *, socklen_t *);
#define accept(...) CALL_WITH_DEFAULTS(accept, 3, __VA_ARGS__)
DECLARE_DEFARG(accept, , NULL, &TNULL(socklen_t));

// int socket(int domain, int type, int protocol);
DEFARG_SIGNATURE(int, socket, int, int, int);
#define socket(...) CALL_WITH_DEFAULTS(socket, 3, __VA_ARGS__)
DECLARE_DEFARG(socket, , SOCK_STREAM, 0);


#endif 	    /* !ORWL_POSIX_DEFAULT_H_ */
