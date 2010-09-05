/*
** p99_posix_default.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Mar 29 18:40:36 2010 Jens Gustedt
** Last update Mon Mar 29 18:40:36 2010 Jens Gustedt
*/

#ifndef   	P99_POSIX_DEFAULT_H_
# define   	P99_POSIX_DEFAULT_H_

#include "p99_defarg.h"
#include P99_ADVANCE_ID


#define P99__POSIX_DEFARG_DOCU(NAME, RET, ...)                          \
/*! @brief Default arguments for POSIX function NAME */                 \
/*! This macro hides the function @code RET NAME(__VA_ARGS__) @endcode */ \
/*! @see P99_CALL_DEFARG */
P99_MACRO_END(POSIX_DEFARG_DOCU);


P99__POSIX_DEFARG_DOCU(pthread_barrier_init, int, pthread_barrier_t *restrict objp, const pthread_barrierattr_t *restrict attr, unsigned count)
/*! @param objp is required */
/*! @param attr defaults to 0, such that is @a objp is initialized with default velues */
/*! @param count is required */
#define pthread_barrier_init(...) P99_CALL_DEFARG(pthread_barrier_init, 3, __VA_ARGS__)
#define pthread_barrier_init_defarg_1() 0
#define pthread_barrier_init_defarg_2() (pthread_barrier_init must have a count argument)

P99__POSIX_DEFARG_DOCU(pthread_cond_init, int, pthread_cond_t* objp, pthread_condattr_t const* attr)
/*! @param objp is required */
/*! @param attr defaults to 0, such that is @a objp is initialized with default velues */
#define pthread_cond_init(...) P99_CALL_DEFARG(pthread_cond_init, 2, __VA_ARGS__)
#define pthread_cond_init_defarg_1() 0

P99__POSIX_DEFARG_DOCU(pthread_mutex_init, int, pthread_mutex_t* objp, pthread_mutexattr_t const* attr)
/*! @param objp is required */
/*! @param attr defaults to 0, such that is @a objp is initialized with default velues */
#define pthread_mutex_init(...) P99_CALL_DEFARG(pthread_mutex_init, 2, __VA_ARGS__)
#define pthread_mutex_init_defarg_1 0

P99__POSIX_DEFARG_DOCU(pthread_rwlock_init, int, pthread_rwlock_t* objp, pthread_rwlockattr_t const* attr)
/*! @param objp is required */
/*! @param attr defaults to 0, such that is @a objp is initialized with default velues */
#define pthread_rwlock_init(...) P99_CALL_DEFARG(pthread_rwlock_init, 2, __VA_ARGS__)
#define pthread_rwlock_init_defarg_1 0

P99__POSIX_DEFARG_DOCU(pthread_spin_init, int, pthread_spinlock_t* objp, int pshared)
/*! @param objp is required */
/*! @param pshared defaults to PTHREAD_PROCESS_PRIVATE, such that @a objp is not shared between different processes  */
#define pthread_spin_init(...) P99_CALL_DEFARG(pthread_spin_init, 2, __VA_ARGS__)
#define pthread_spin_init_defarg_1() PTHREAD_PROCESS_PRIVATE

P99__POSIX_DEFARG_DOCU(sem_init, int, sem_t* objp, int pshared, unsigned count)
/*! @param objp is required */
/*! @param pshared defaults to PTHREAD_PROCESS_PRIVATE, such that @a objp is not shared between different processes  */
/*! @param count is required */
#define sem_init(...) P99_CALL_DEFARG(sem_init, 3, __VA_ARGS__)
#define sem_init_defarg_1() PTHREAD_PROCESS_PRIVATE
#define sem_init_defarg_2() 0

P99__POSIX_DEFARG_DOCU(pthread_key_create, int, pthread_key_t *key, void (*destr_function) (void *))
/*! @param objp is required */
/*! @param destr_function defaults to 0 */
#define pthread_key_create(...) P99_CALL_DEFARG(pthread_key_create, 2, __VA_ARGS__)
#define pthread_key_create_defarg_1() 0

#define P99__C99_DEFARG_DOCU(NAME, RET, ...)                            \
/*! @brief Default arguments for C99 function NAME */                   \
/*! This macro hides the function @code RET NAME(__VA_ARGS__) @endcode */ \
/*! @see P99_CALL_DEFARG */
P99_MACRO_END(C99_DEFARG_DOCU);


P99__C99_DEFARG_DOCU(wcstol, long, wchar_t const *nptr, wchar_t **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define wcstol(...) P99_CALL_DEFARG(wcstol, 3, __VA_ARGS__)
#define wcstol_defarg_1() 0
#define wcstol_defarg_2() 0

P99__C99_DEFARG_DOCU(wcstoll, long long, wchar_t const *nptr, wchar_t **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define wcstoll(...) P99_CALL_DEFARG(wcstoll, 3, __VA_ARGS__)
#define wcstoll_defarg_1() 0
#define wcstoll_defarg_2() 0

P99__C99_DEFARG_DOCU(wcstoimax, intmax_t, wchar_t const *nptr, wchar_t **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define wcstoimax(...) P99_CALL_DEFARG(wcstoimax, 3, __VA_ARGS__)
#define wcstoimax_defarg_1() 0
#define wcstoimax_defarg_2() 0

P99__C99_DEFARG_DOCU(wcstoul, unsigned long, wchar_t const *nptr, wchar_t **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define wcstoul(...) P99_CALL_DEFARG(wcstoul, 3, __VA_ARGS__)
#define wcstoul_defarg_1() 0
#define wcstoul_defarg_2() 0

P99__C99_DEFARG_DOCU(wcstoull, unsigned long long, wchar_t const *nptr, wchar_t **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define wcstoull(...) P99_CALL_DEFARG(wcstoull, 3, __VA_ARGS__)
#define wcstoull_defarg_1() 0
#define wcstoull_defarg_2() 0

P99__C99_DEFARG_DOCU(wcstoumax, uintmax_t, wchar_t const *nptr, wchar_t **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define wcstoumax(...) P99_CALL_DEFARG(wcstoumax, 3, __VA_ARGS__)
#define wcstoumax_defarg_1() 0
#define wcstoumax_defarg_2() 0

P99__C99_DEFARG_DOCU(wcstof, float, wchar_t const *nptr, wchar_t **endptr)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
#define wcstof(...) P99_CALL_DEFARG(wcstof, 2, __VA_ARGS__)
#define wcstof_defarg_1() 0

P99__C99_DEFARG_DOCU(wcstod, double, wchar_t const *nptr, wchar_t **endptr)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
#define wcstod(...) P99_CALL_DEFARG(wcstod, 2, __VA_ARGS__)
#define wcstod_defarg_1() 0

P99__C99_DEFARG_DOCU(wcstold, long double, wchar_t const *nptr, wchar_t **endptr)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
#define wcstold(...) P99_CALL_DEFARG(wcstold, 2, __VA_ARGS__)
#define wcstold_defarg_1() 0

P99__C99_DEFARG_DOCU(strerror, char*, int errnum)
/*! @param errnum defaults to errno */
#define strerror(...) P99_CALL_DEFARG(strerror, 1, __VA_ARGS__)
#define strerror_defarg_0() 0

P99__C99_DEFARG_DOCU(strtol, long, char const *nptr, char **endptr, int base)
/** This macro is preferable to the @c atol function. The default
 ** arguments make it functionally equivalent but add two features:
 **  - overflow and underflow handling
 **  - flexible base, namely octal, decimal or hexadecimal conversion
 **/
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define strtol(...) P99_CALL_DEFARG(strtol, 3, __VA_ARGS__)
#define strtol_defarg_1() 0
#define strtol_defarg_2() 0

P99__C99_DEFARG_DOCU(strtoll, long long, char const *nptr, char **endptr, int base)
/** This macro is preferable to the @c atoll function. The default
 ** arguments make it functionally equivalent but add two features:
 **  - overflow and underflow handling
 **  - flexible base, namely octal, decimal or hexadecimal conversion
 **/
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define strtoll(...) P99_CALL_DEFARG(strtoll, 3, __VA_ARGS__)
#define strtoll_defarg_1() 0
#define strtoll_defarg_2() 0

P99__C99_DEFARG_DOCU(strtoimax, intmax_t, char const *nptr, char **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define strtoimax(...) P99_CALL_DEFARG(strtoimax, 3, __VA_ARGS__)
#define strtoimax_defarg_1() 0
#define strtoimax_defarg_2() 0

P99__C99_DEFARG_DOCU(strtoul, unsigned long, char const *nptr, char **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define strtoul(...) P99_CALL_DEFARG(strtoul, 3, __VA_ARGS__)
#define strtoul_defarg_1() 0
#define strtoul_defarg_2() 0

P99__C99_DEFARG_DOCU(strtoull, unsigned long long, char const *nptr, char **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define strtoull(...) P99_CALL_DEFARG(strtoull, 3, __VA_ARGS__)
#define strtoull_defarg_1() 0
#define strtoull_defarg_2() 0

P99__C99_DEFARG_DOCU(strtoumax, uintmax_t, char const *nptr, char **endptr, int base)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define strtoumax(...) P99_CALL_DEFARG(strtoumax, 3, __VA_ARGS__)
#define strtoumax_defarg_1() 0
#define strtoumax_defarg_2() 0

P99__C99_DEFARG_DOCU(strtof, float, char const *nptr, char **endptr)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
#define strtof(...) P99_CALL_DEFARG(strtof, 2, __VA_ARGS__)
#define strtof_defarg_1() 0

P99__C99_DEFARG_DOCU(strtod, double, char const *nptr, char **endptr)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
#define strtod(...) P99_CALL_DEFARG(strtod, 2, __VA_ARGS__)
#define strtod_defarg_1() 0

P99__C99_DEFARG_DOCU(strtold, long double, char const *nptr, char **endptr)
/*! @param nptr is required */
/*! @param endptr defaults to 0, meaning that the successfully parsed string is not kept track */
#define strtold(...) P99_CALL_DEFARG(strtold, 2, __VA_ARGS__)
#define strtold_defarg_1() 0

P99__C99_DEFARG_DOCU(calloc, void*, size_t rec_num, size_t rec_size)
/** @param rec_num is required
 ** @param rec_size defaults to 1, thus @c calloc(rec_num) is
 ** equivalent to @c malloc(rec_num) only that it also initializes the
 ** allocated space to all zeros
 **/
#define calloc(...) P99_CALL_DEFARG(calloc, 2, __VA_ARGS__)
#define calloc_defarg_1() 1u

P99__C99_DEFARG_DOCU(longjmp, void, jmp_buf env, int val)
/** @param jmp_buf is required
 ** @param val defaults to 1
 **/
#define longjmp(...) P99_CALL_DEFARG(longjmp, 2, __VA_ARGS__)
#define longjmp_defarg_1() 1

#define SOCKADDR_IN_INIIALIZER { .sin_family = AF_INET }

#define IN_ADDR_INITIALIZER  { .s_addr = 0 }

P99__POSIX_DEFARG_DOCU(accept, int, int sockfd, struct sockaddr *addr, socklen_t *addrlen)
/**
 ** @param  sockfd must be providedd
 ** @param  addr defaults to 0
 ** @param  addrlen defaults to the address of a temporary of type @c socklen_t
 **/
#define accept(...) P99_CALL_DEFARG(accept, 3, __VA_ARGS__)
#define accept_defarg_1() 0
#define accept_defarg_2() &P99_LVAL(socklen_t)

P99__POSIX_DEFARG_DOCU(socket, int, int domain, int type, int protocol)
/**
 ** @param  domain is required
 ** @param  type defaults to SOCK_STREAM
 ** @param  protocol defaults to 0
 **/
#define socket(...) P99_CALL_DEFARG(socket, 3, __VA_ARGS__)
#define socket_defarg_1() SOCK_STREAM
#define socket_defarg_2() 0

P99__POSIX_DEFARG_DOCU(inet_ntop, char const*, int af, const void *src, char *dst, socklen_t size)
/**
 ** @param  af is required
 ** @param  src is required
 ** @param  dst defaults to the address of a temporary that is then returned
 ** @param  size defaults to INET6_ADDRSTRLEN
 **/
#define inet_ntop(...) P99_CALL_DEFARG(inet_ntop, 4, __VA_ARGS__)
#define inet_ntop_defarg_2() (char[INET6_ADDRSTRLEN]){ 0 }
#define inet_ntop_defarg_3() INET6_ADDRSTRLEN


P99__POSIX_DEFARG_DOCU(realpath, char*, char const *path, char *resolved_path)
/**
 ** @param  path is required
 ** @param  resolved_path defaults to the address of a temporary that is returned in case of success
 ** @note to have realpath allocate its return value with @c malloc, @a resolved_path should be set to 0
 **/
#define realpath(...) P99_CALL_DEFARG(realpath, 2, __VA_ARGS__)
#define realpath_defarg_1() (char[PATH_MAX]){ 0 }


#endif 	    /* !P99_POSIX_DEFAULT_H_ */
