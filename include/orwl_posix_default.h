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

#include "orwl_macro.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>

// pthread_barrier_init (3posix) - destroy and initialize a barrier object (ADVANCED REALTIME THREADS)
// not possible because of the count argument

 // pthread_cond_init (3posix) - destroy and initialize condition variables
#define pthread_cond_init(...) CALL_THE_FUNC(pthread_cond_init, 2, __VA_ARGS__)
declare_defarg(pthread_cond_init, 1, pthread_condattr_t*, NULL);

 // pthread_mutex_init (3posix) - destroy and initialize a mutex
#define pthread_mutex_init(...) CALL_THE_FUNC(pthread_mutex_init, 2, __VA_ARGS__)
declare_defarg(pthread_mutex_init, 1, pthread_mutexattr_t*, NULL);

 // pthread_rwlock_init (3posix) - destroy and initialize a read-write lock object
#define pthread_rwlock_init(...) CALL_THE_FUNC(pthread_rwlock_init, 2, __VA_ARGS__)
declare_defarg(pthread_rwlock_init, 1, pthread_rwlockattr_t*, NULL);

 // pthread_spin_init (3posix) - destroy or initialize a spin lock object (ADVANCED REALTIME THREADS)
#define pthread_spin_init(...) CALL_THE_FUNC(pthread_spin_init, 2, __VA_ARGS__)
declare_defarg(pthread_spin_init, 1, int, PTHREAD_PROCESS_PRIVATE);




#define strtoul(...) CALL_THE_FUNC(strtoul, 3, __VA_ARGS__)
declare_defarg(strtoul, 2, int, 0);
declare_defarg(strtoul, 1, char **, NULL);

#define strtoull(...) CALL_THE_FUNC(strtoull, 3, __VA_ARGS__)
declare_defarg(strtoull, 2, int, 0);
declare_defarg(strtoull, 1, char **, NULL);

#define strtouf(...) CALL_THE_FUNC(strtouf, 2, __VA_ARGS__)
declare_defarg(strtouf, 1, char **, NULL);

#define strtoud(...) CALL_THE_FUNC(strtoud, 2, __VA_ARGS__)
declare_defarg(strtoud, 1, char **, NULL);

#define strtould(...) CALL_THE_FUNC(strtould, 2, __VA_ARGS__)
declare_defarg(strtould, 1, char **, NULL);


// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
#define accept(...) CALL_THE_FUNC(accept, 3, __VA_ARGS__)
declare_defarg(accept, 2, socklen_t*, &TNULL(socklen_t));
declare_defarg(accept, 1, struct sockaddr *, NULL);

// int socket(int domain, int type, int protocol);
#define socket(...) CALL_THE_FUNC(socket, 3, __VA_ARGS__)
declare_defarg(socket, 2, int, 0);
declare_defarg(socket, 1, int, SOCK_STREAM);


#endif 	    /* !ORWL_POSIX_DEFAULT_H_ */
