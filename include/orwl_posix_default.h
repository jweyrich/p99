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

struct rand48_t;

#ifndef __cplusplus
typedef struct rand48_t rand48_t;
#endif


#endif


#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "orwl_macro.h"

// pthread_barrier_init (3posix) - destroy and initialize a barrier object (ADVANCED REALTIME THREADS)
// not possible because of the count argument

 // pthread_cond_init (3posix) - destroy and initialize condition variables
PROTOTYPE(int, pthread_cond_init, pthread_cond_t*, pthread_condattr_t const*);
FSYMB_DOCUMENTATION(pthread_cond_init)
#define pthread_cond_init(...) CALL_WITH_DEFAULTS(pthread_cond_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_cond_init, , NULL);

 // pthread_mutex_init (3posix) - destroy and initialize a mutex
PROTOTYPE(int, pthread_mutex_init, pthread_mutex_t*, pthread_mutexattr_t const*);
FSYMB_DOCUMENTATION(pthread_mutex_init)
#define pthread_mutex_init(...) CALL_WITH_DEFAULTS(pthread_mutex_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_mutex_init, , NULL);

 // pthread_rwlock_init (3posix) - destroy and initialize a read-write lock object
PROTOTYPE(int, pthread_rwlock_init, pthread_rwlock_t*, pthread_rwlockattr_t const*);
FSYMB_DOCUMENTATION(pthread_rwlock_init)
#define pthread_rwlock_init(...) CALL_WITH_DEFAULTS(pthread_rwlock_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_rwlock_init, , NULL);

 // pthread_spin_init (3posix) - destroy or initialize a spin lock object (ADVANCED REALTIME THREADS)
PROTOTYPE(int, pthread_spin_init, pthread_spinlock_t*, int);
FSYMB_DOCUMENTATION(pthread_spin_init)
#define pthread_spin_init(...) CALL_WITH_DEFAULTS(pthread_spin_init, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_spin_init, , PTHREAD_PROCESS_PRIVATE);

// int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
#if   	ORWL_POSIX_DEFAULT_H_ == 2
typedef void (*pthread_key_create_arg1_t)(void *);
#endif
PROTOTYPE(int, pthread_key_create, pthread_key_t*, pthread_key_create_arg1_t);
FSYMB_DOCUMENTATION(pthread_key_create)
#define pthread_key_create(...) CALL_WITH_DEFAULTS(pthread_key_create, 2, __VA_ARGS__)
DECLARE_DEFARG(pthread_key_create, , NULL);


PROTOTYPE(long, strtol, char const*, char**, int);
FSYMB_DOCUMENTATION(strtol)
#define strtol(...) CALL_WITH_DEFAULTS(strtol, 3, __VA_ARGS__)
DECLARE_DEFARG(strtol, , NULL, 0);

PROTOTYPE(long long, strtoll, char const*, char**, int);
FSYMB_DOCUMENTATION(strtoll)
#define strtoll(...) CALL_WITH_DEFAULTS(strtoll, 3, __VA_ARGS__)
DECLARE_DEFARG(strtoll, , NULL, 0);

PROTOTYPE(ulong, strtoul, char const*, char**, int);
FSYMB_DOCUMENTATION(strtoul)
#define strtoul(...) CALL_WITH_DEFAULTS(strtoul, 3, __VA_ARGS__)
DECLARE_DEFARG(strtoul, , NULL, 0);

PROTOTYPE(unsigned long long, strtoull, char const*, char**, int);
FSYMB_DOCUMENTATION(strtoull)
#define strtoull(...) CALL_WITH_DEFAULTS(strtoull, 3, __VA_ARGS__)
DECLARE_DEFARG(strtoull, , NULL, 0);

PROTOTYPE(float, strtof, char const*, char**);
FSYMB_DOCUMENTATION(strtof)
#define strtof(...) CALL_WITH_DEFAULTS(strtof, 2, __VA_ARGS__)
DECLARE_DEFARG(strtof, , NULL);

PROTOTYPE(double, strtod, char const*, char**);
FSYMB_DOCUMENTATION(strtod)
#define strtod(...) CALL_WITH_DEFAULTS(strtod, 2, __VA_ARGS__)
DECLARE_DEFARG(strtod, , NULL);

PROTOTYPE(long double, strtold, char const*, char**);
FSYMB_DOCUMENTATION(strtold)
#define strtold(...) CALL_WITH_DEFAULTS(strtold, 2, __VA_ARGS__)
DECLARE_DEFARG(strtold, , NULL);

PROTOTYPE(void*, calloc, size_t, size_t);
FSYMB_DOCUMENTATION(calloc)
#define calloc(...) CALL_WITH_DEFAULTS(calloc, 2, __VA_ARGS__)
DECLARE_DEFARG(calloc, , 1);

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
PROTOTYPE(int, accept, int, struct sockaddr *, socklen_t *);
FSYMB_DOCUMENTATION(accept)
#define accept(...) CALL_WITH_DEFAULTS(accept, 3, __VA_ARGS__)
DECLARE_DEFARG(accept, , NULL, &TNULL(socklen_t));

// int socket(int domain, int type, int protocol);
PROTOTYPE(int, socket, int, int, int);
FSYMB_DOCUMENTATION(socket)
#define socket(...) CALL_WITH_DEFAULTS(socket, 3, __VA_ARGS__)
DECLARE_DEFARG(socket, , SOCK_STREAM, 0);

PROTOTYPE(char const*, inet_ntop, int, const void *, char *, socklen_t);
DECLARE_DEFARG(inet_ntop, , , );
FSYMB_DOCUMENTATION(inet_ntop)
#define inet_ntop(...) CALL_WITH_DEFAULTS(inet_ntop, 4, __VA_ARGS__)
#define inet_ntop_defarg_2() ((char[INET6_ADDRSTRLEN]){ 0 })
#define inet_ntop_defarg_3() (INET6_ADDRSTRLEN)


#endif 	    /* !ORWL_POSIX_DEFAULT_H_ */
