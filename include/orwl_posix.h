/*
** orwl_posix.h
**
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
**
** Started on  Mon Mar 29 18:40:36 2010 Jens Gustedt
** Last update Mon Mar 29 18:40:36 2010 Jens Gustedt
*/

#ifndef   	ORWL_POSIX_H_
#define   	ORWL_POSIX_H_

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#elif _XOPEN_SOURCE < 600
# error "Need at least XOPEN specification 6 to compile this file"
#endif
#ifndef _BSD_SOURCE
# define _BSD_SOURCE 600
#endif
#include <features.h>
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

#endif 	    /* !ORWL_POSIX_H_ */
