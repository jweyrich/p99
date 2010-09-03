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
# define   	ORWL_POSIX_DEFAULT_H_

#include "orwl_posix.h"
#include "p99_defarg.h"
#include P99_ADVANCE_ID
#include "orwl_int.h"

/**
 ** @brief Default arguments for pthread_barrier_init (3posix) - destroy and initialize a barrier object (ADVANCED REALTIME THREADS)
 **
 ** This macro hides the function pthread_barrier_init with return type @c int and signature
 ** (pthread_barrier_t *restrict, const pthread_barrierattr_t *restrict, unsigned)
 **/
P99_DEFARG_DOCU(pthread_barrier_init)
#define pthread_barrier_init(...) P99_CALL_DEFARG(pthread_barrier_init, 3, __VA_ARGS__)
#define pthread_barrier_init_defarg_1() NULL
#define pthread_barrier_init_defarg_2() (pthread_barrier_init must have a count argument)

/**
 ** @brief Default arguments for pthread_cond_init (3posix) - destroy and initialize condition variables
 **
 ** This macro hides the function pthread_cond_init with return type
 ** @c int and signature
 ** (pthread_cond_t*, pthread_condattr_t const*)
 **/
P99_DEFARG_DOCU(pthread_cond_init)
#define pthread_cond_init(...) P99_CALL_DEFARG(pthread_cond_init, 2, __VA_ARGS__)
#define pthread_cond_init_defarg_1() NULL

/**
 ** @brief Default arguments for pthread_mutex_init (3posix) - destroy and initialize a mutex
 **
 ** This macro hides the function pthread_mutex_init with return type @c int and signature
 ** (pthread_mutex_t*, pthread_mutexattr_t const*)
 **/
P99_DEFARG_DOCU(pthread_mutex_init)
#define pthread_mutex_init(...) P99_CALL_DEFARG(pthread_mutex_init, 2, __VA_ARGS__)
#define pthread_mutex_init_defarg_1 NULL

/**
 ** @brief Default arguments for pthread_rwlock_init (3posix) - destroy and initialize a read-write lock object
 **
 ** This macro hides the function pthread_rwlock_init with return type @c int and signature
 ** (pthread_rwlock_t*, pthread_rwlockattr_t const*)
 **/
P99_DEFARG_DOCU(pthread_rwlock_init)
#define pthread_rwlock_init(...) P99_CALL_DEFARG(pthread_rwlock_init, 2, __VA_ARGS__)
#define pthread_rwlock_init_defarg_1 NULL

/**
 ** @brief Default arguments for pthread_spin_init (3posix) - destroy or initialize a spin lock object (ADVANCED REALTIME THREADS)
 **
 ** This macro hides the function pthread_spin_init with return type @c int and signature
 ** (pthread_spinlock_t*, int)
 **/
P99_DEFARG_DOCU(pthread_spin_init)
#define pthread_spin_init(...) P99_CALL_DEFARG(pthread_spin_init, 2, __VA_ARGS__)
#define pthread_spin_init_defarg_1() PTHREAD_PROCESS_PRIVATE

/**
 ** @brief Default arguments for sem_init - initialize a sem_t object
 **
 ** This macro hides the function sem_init with return type @c int and signature
 ** (sem_t*, int, unsigned)
 **/
P99_DEFARG_DOCU(sem_init)
#define sem_init(...) P99_CALL_DEFARG(sem_init, 3, __VA_ARGS__)
#define sem_init_defarg_1() PTHREAD_PROCESS_PRIVATE
#define sem_init_defarg_2() 0

/**
 ** @brief Default arguments for int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
 **
 **/
P99_DEFARG_DOCU(pthread_key_create)
#define pthread_key_create(...) P99_CALL_DEFARG(pthread_key_create, 2, __VA_ARGS__)
#define pthread_key_create_defarg_1() NULL


/**
 ** @brief Default arguments for @c strtol
 **/
P99_DEFARG_DOCU(strtol)
#define strtol(...) P99_CALL_DEFARG(strtol, 3, __VA_ARGS__)
#define strtol_defarg_1() NULL
#define strtol_defarg_2() 0

/**
 ** @brief Default arguments for @c strtoll
 **/
P99_DEFARG_DOCU(strtoll)
#define strtoll(...) P99_CALL_DEFARG(strtoll, 3, __VA_ARGS__)
#define strtoll_defarg_1() NULL
#define strtoll_defarg_2() 0

/**
 ** @brief Default arguments for @c strtoul
 **/
P99_DEFARG_DOCU(strtoul)
#define strtoul(...) P99_CALL_DEFARG(strtoul, 3, __VA_ARGS__)
#define strtoul_defarg_1() NULL
#define strtoul_defarg_2() 0

/**
 ** @brief Default arguments for @c strtoull
 **/
P99_DEFARG_DOCU(strtoull)
#define strtoull(...) P99_CALL_DEFARG(strtoull, 3, __VA_ARGS__)
#define strtoull_defarg_1() NULL
#define strtoull_defarg_2() 0

/**
 ** @brief Default arguments for @c strtof
 **/
P99_DEFARG_DOCU(strtof)
#define strtof(...) P99_CALL_DEFARG(strtof, 2, __VA_ARGS__)
#define strtof_defarg_1() NULL

/**
 ** @brief Default arguments for @c strtod
 **/
P99_DEFARG_DOCU(strtod)
#define strtod(...) P99_CALL_DEFARG(strtod, 2, __VA_ARGS__)
#define strtod_defarg_1() NULL

/**
 ** @brief Default arguments for @c strtold
 **/
P99_DEFARG_DOCU(strtold)
#define strtold(...) P99_CALL_DEFARG(strtold, 2, __VA_ARGS__)
#define strtold_defarg_1() NULL

/**
 ** @brief Default arguments for @c calloc
 **/
P99_DEFARG_DOCU(calloc)
#define calloc(...) P99_CALL_DEFARG(calloc, 2, __VA_ARGS__)
#define calloc_defarg_1() 1u

#define SOCKADDR_IN_INIIALIZER { .sin_family = AF_INET }

#define IN_ADDR_INITIALIZER  { .s_addr = 0 }


/**
 ** @brief Default arguments for int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 **
 ** This macro hides the function accept with return type @c int and signature
 ** (int, struct sockaddr *, socklen_t *)
 **/
P99_DEFARG_DOCU(accept)
#define accept(...) P99_CALL_DEFARG(accept, 3, __VA_ARGS__)
#define accept_defarg_1() NULL
#define accept_defarg_2() &P99_LVAL(socklen_t)

/**
 ** @brief Default arguments for int socket(int domain, int type, int protocol);
 **
 ** This macro hides the function socket with return type @c int and signature
 ** (int, int, int)
 **/
P99_DEFARG_DOCU(socket)
#define socket(...) P99_CALL_DEFARG(socket, 3, __VA_ARGS__)
#define socket_defarg_1() SOCK_STREAM
#define socket_defarg_2() 0

/**
 ** @brief Default arguments for @c inet_ntop
 **/
P99_DEFARG_DOCU(inet_ntop)
#define inet_ntop(...) P99_CALL_DEFARG(inet_ntop, 4, __VA_ARGS__)
#define inet_ntop_defarg_2() (char[INET6_ADDRSTRLEN]){ 0 }
#define inet_ntop_defarg_3() INET6_ADDRSTRLEN


/**
 ** @brief Default arguments for @c realpath
 **/
P99_DEFARG_DOCU(realpath)
#define realpath(...) P99_CALL_DEFARG(realpath, 2, __VA_ARGS__)
#define realpath_defarg_1() NULL


#endif 	    /* !ORWL_POSIX_DEFAULT_H_ */
