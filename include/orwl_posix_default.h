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
#define pthread_cond_init(...) CALL_WITH_DEFAULTS(pthread_cond_init, 2, __VA_ARGS__)
declare_defarg(pthread_cond_init, 1, pthread_condattr_t*, NULL);

 // pthread_mutex_init (3posix) - destroy and initialize a mutex
#define pthread_mutex_init(...) CALL_WITH_DEFAULTS(pthread_mutex_init, 2, __VA_ARGS__)
declare_defarg(pthread_mutex_init, 1, pthread_mutexattr_t*, NULL);

 // pthread_rwlock_init (3posix) - destroy and initialize a read-write lock object
#define pthread_rwlock_init(...) CALL_WITH_DEFAULTS(pthread_rwlock_init, 2, __VA_ARGS__)
declare_defarg(pthread_rwlock_init, 1, pthread_rwlockattr_t*, NULL);

 // pthread_spin_init (3posix) - destroy or initialize a spin lock object (ADVANCED REALTIME THREADS)
#define pthread_spin_init(...) CALL_WITH_DEFAULTS(pthread_spin_init, 2, __VA_ARGS__)
declare_defarg(pthread_spin_init, 1, int, PTHREAD_PROCESS_PRIVATE);

// int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
#define pthread_key_create(...) CALL_WITH_DEFAULTS(pthread_key_create, 2, __VA_ARGS__)
#if   	ORWL_POSIX_DEFAULT_H_ == 2
typedef void (*pthread_key_create_arg1_t)(void *);
#endif
declare_defarg(pthread_key_create, 1, pthread_key_create_arg1_t, NULL);


#define strtol(...) CALL_WITH_DEFAULTS(strtol, 3, __VA_ARGS__)
declare_defarg(strtol, 2, int, 0);
declare_defarg(strtol, 1, char **, NULL);

#define strtoll(...) CALL_WITH_DEFAULTS(strtoll, 3, __VA_ARGS__)
declare_defarg(strtoll, 2, int, 0);
declare_defarg(strtoll, 1, char **, NULL);

#define strtoul(...) CALL_WITH_DEFAULTS(strtoul, 3, __VA_ARGS__)
declare_defarg(strtoul, 2, int, 0);
declare_defarg(strtoul, 1, char **, NULL);

#define strtoull(...) CALL_WITH_DEFAULTS(strtoull, 3, __VA_ARGS__)
declare_defarg(strtoull, 2, int, 0);
declare_defarg(strtoull, 1, char **, NULL);

#define strtof(...) CALL_WITH_DEFAULTS(strtof, 2, __VA_ARGS__)
declare_defarg(strtof, 1, char **, NULL);

#define strtod(...) CALL_WITH_DEFAULTS(strtod, 2, __VA_ARGS__)
declare_defarg(strtod, 1, char **, NULL);

#define strtold(...) CALL_WITH_DEFAULTS(strtold, 2, __VA_ARGS__)
declare_defarg(strtold, 1, char **, NULL);


// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
#define accept(...) CALL_WITH_DEFAULTS(accept, 3, __VA_ARGS__)
declare_defarg(accept, 2, socklen_t*, &TNULL(socklen_t));
declare_defarg(accept, 1, struct sockaddr *, NULL);

// int socket(int domain, int type, int protocol);
#define socket(...) CALL_WITH_DEFAULTS(socket, 3, __VA_ARGS__)
declare_defarg(socket, 2, int, 0);
declare_defarg(socket, 1, int, SOCK_STREAM);

// int open(const char *pathname, int flags);
// int open(const char *pathname, int flags, mode_t mode);
#define open(...) CALL_WITH_DEFAULTS(open, 3, __VA_ARGS__)
declare_defarg(open, 2, mode_t,  S_IRWXU);
declare_defarg(open, 1, int, O_RDONLY);

// int openat (int __fd, __const char *__file, int __oflag, ...)
#define openat(...) CALL_WITH_DEFAULTS(openat, 4, __VA_ARGS__)
declare_defarg(openat, 3, mode_t,  S_IRWXU);
declare_defarg(openat, 2, int, O_RDONLY);

// int creat(const char *pathname, mode_t mode);
#define creat(...) CALL_WITH_DEFAULTS(creat, 2, __VA_ARGS__)
declare_defarg(creat, 1, mode_t, S_IRWXU);

// int open64(const char *pathname, int flags);
// int open64(const char *pathname, int flags, mode_t mode);
#define open64(...) CALL_WITH_DEFAULTS(open64, 3, __VA_ARGS__)
declare_defarg(open64, 2, mode_t,  S_IRWXU);
declare_defarg(open64, 1, int, O_RDONLY);

// int openat64 (int __fd, __const char *__file, int __oflag, ...)
#define openat64(...) CALL_WITH_DEFAULTS(openat64, 4, __VA_ARGS__)
declare_defarg(openat64, 3, mode_t,  S_IRWXU);
declare_defarg(openat64, 2, int, O_RDONLY);

// int creat(const char *pathname, mode_t mode);
#define creat64(...) CALL_WITH_DEFAULTS(creat64, 2, __VA_ARGS__)
declare_defarg(creat64, 1, mode_t, S_IRWXU);

#endif 	    /* !ORWL_POSIX_DEFAULT_H_ */
