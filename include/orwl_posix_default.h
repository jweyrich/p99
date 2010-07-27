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


#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
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

#include "p99_defarg.h"
#include "orwl_int.h"

// pthread_barrier_init (3posix) - destroy and initialize a barrier object (ADVANCED REALTIME THREADS)
// not possible because of the count argument

 // pthread_cond_init (3posix) - destroy and initialize condition variables
P99_PROTOTYPE(int, pthread_cond_init, pthread_cond_t*, pthread_condattr_t const*);
P99_DEFARG_DOCU(pthread_cond_init)
#define pthread_cond_init(...) P99_CALL_DEFARG(pthread_cond_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(pthread_cond_init, , NULL);

 // pthread_mutex_init (3posix) - destroy and initialize a mutex
P99_PROTOTYPE(int, pthread_mutex_init, pthread_mutex_t*, pthread_mutexattr_t const*);
P99_DEFARG_DOCU(pthread_mutex_init)
#define pthread_mutex_init(...) P99_CALL_DEFARG(pthread_mutex_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(pthread_mutex_init, , NULL);

 // pthread_rwlock_init (3posix) - destroy and initialize a read-write lock object
P99_PROTOTYPE(int, pthread_rwlock_init, pthread_rwlock_t*, pthread_rwlockattr_t const*);
P99_DEFARG_DOCU(pthread_rwlock_init)
#define pthread_rwlock_init(...) P99_CALL_DEFARG(pthread_rwlock_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(pthread_rwlock_init, , NULL);

 // pthread_spin_init (3posix) - destroy or initialize a spin lock object (ADVANCED REALTIME THREADS)
P99_PROTOTYPE(int, pthread_spin_init, pthread_spinlock_t*, int);
P99_DEFARG_DOCU(pthread_spin_init)
#define pthread_spin_init(...) P99_CALL_DEFARG(pthread_spin_init, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(pthread_spin_init, , PTHREAD_PROCESS_PRIVATE);

 // sem_init - initialize a sem_t object
P99_PROTOTYPE(int, sem_init, sem_t*, int, unsigned);
P99_DEFARG_DOCU(sem_init)
#define sem_init(...) P99_CALL_DEFARG(sem_init, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(sem_init, , PTHREAD_PROCESS_PRIVATE, 0u);

// int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
#if   	ORWL_POSIX_DEFAULT_H_ == 2
typedef void (*pthread_key_create_arg1_t)(void *);
#endif
P99_PROTOTYPE(int, pthread_key_create, pthread_key_t*, pthread_key_create_arg1_t);
P99_DEFARG_DOCU(pthread_key_create)
#define pthread_key_create(...) P99_CALL_DEFARG(pthread_key_create, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(pthread_key_create, , NULL);


P99_PROTOTYPE(long, strtol, char const*, char**, int);
P99_DEFARG_DOCU(strtol)
#define strtol(...) P99_CALL_DEFARG(strtol, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(strtol, , NULL, 0);

P99_PROTOTYPE(long long, strtoll, char const*, char**, int);
P99_DEFARG_DOCU(strtoll)
#define strtoll(...) P99_CALL_DEFARG(strtoll, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(strtoll, , NULL, 0);

P99_PROTOTYPE(ulong, strtoul, char const*, char**, int);
P99_DEFARG_DOCU(strtoul)
#define strtoul(...) P99_CALL_DEFARG(strtoul, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(strtoul, , NULL, 0);

P99_PROTOTYPE(unsigned long long, strtoull, char const*, char**, int);
P99_DEFARG_DOCU(strtoull)
#define strtoull(...) P99_CALL_DEFARG(strtoull, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(strtoull, , NULL, 0);

P99_PROTOTYPE(float, strtof, char const*, char**);
P99_DEFARG_DOCU(strtof)
#define strtof(...) P99_CALL_DEFARG(strtof, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(strtof, , NULL);

P99_PROTOTYPE(double, strtod, char const*, char**);
P99_DEFARG_DOCU(strtod)
#define strtod(...) P99_CALL_DEFARG(strtod, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(strtod, , NULL);

P99_PROTOTYPE(long double, strtold, char const*, char**);
P99_DEFARG_DOCU(strtold)
#define strtold(...) P99_CALL_DEFARG(strtold, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(strtold, , NULL);

P99_PROTOTYPE(void*, calloc, size_t, size_t);
P99_DEFARG_DOCU(calloc)
#define calloc(...) P99_CALL_DEFARG(calloc, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(calloc, , 1);

#define SOCKADDR_IN_INIIALIZER { .sin_family = AF_INET }

#define IN_ADDR_INITIALIZER  { .s_addr = 0 }


// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
P99_PROTOTYPE(int, accept, int, struct sockaddr *, socklen_t *);
P99_DEFARG_DOCU(accept)
#define accept(...) P99_CALL_DEFARG(accept, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(accept, , NULL, &LNULL(socklen_t));

// int socket(int domain, int type, int protocol);
P99_PROTOTYPE(int, socket, int, int, int);
P99_DEFARG_DOCU(socket)
#define socket(...) P99_CALL_DEFARG(socket, 3, __VA_ARGS__)
P99_DECLARE_DEFARG(socket, , SOCK_STREAM, 0);

P99_PROTOTYPE(char const*, inet_ntop, int, const void *, char *, socklen_t);
P99_DECLARE_DEFARG(inet_ntop, , , );
P99_DEFARG_DOCU(inet_ntop)
#define inet_ntop(...) P99_CALL_DEFARG(inet_ntop, 4, __VA_ARGS__)
#define inet_ntop_defarg_2() ((char[INET6_ADDRSTRLEN]){ 0 })
#define inet_ntop_defarg_3() (INET6_ADDRSTRLEN)


P99_PROTOTYPE(char*, realpath, const char*, char*);
P99_DEFARG_DOCU(realpath)
#define realpath(...) P99_CALL_DEFARG(realpath, 2, __VA_ARGS__)
P99_DECLARE_DEFARG(realpath, , NULL);


#endif 	    /* !ORWL_POSIX_DEFAULT_H_ */
