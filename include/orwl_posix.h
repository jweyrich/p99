/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	ORWL_POSIX_H_
#define   	ORWL_POSIX_H_

/**
 ** @file
 ** @brief Define feature macros and include all necessary files from
 ** C99 and POSIX.
 **/

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#elif _XOPEN_SOURCE < 600
# error "Need at least XOPEN specification 6 to compile this file"
#endif
#ifndef _BSD_SOURCE
# define _BSD_SOURCE 1
#endif

#ifdef __gnu_linux__
#include <features.h>
#endif

/*
  semaphores are an extension and must not be provided so we include
  them via our own file, if available. This then includes pthread.h
  and semaphore.h if available.
   */
#include "orwl_sem.h"
#include "orwl_barrier.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <nl_types.h>

#include "p99_for.h"

/** @brief Init some fast access variables to POSIX features */
extern void orwl_posix_init(void);
/** @brief Print a list of POSIX features and their values to @c
    stderr */
extern void orwl_posix_print(void);

extern long const*const p00_sc_constant;

/*
  Some lists of configuration parameters:
  - The first two are those which we'd expect on every POSIX
    system. They are split into because otherwise the list would be
    too long for the macro processing.
  - The others are optional features or extension depending on newer
    versions of the standard. They may or may not be present on a
    system.
*/

#define P00_POSIX_SC_1                                         \
C_2_C_BIND,                                                    \
C_2_C_DEV,                                                     \
C_2_CHAR_TERM,                                                 \
C_2_FORT_DEV,                                                  \
C_2_FORT_RUN,                                                  \
C_2_LOCALEDEF,                                                 \
C_2_PBS,                                                       \
C_2_PBS_ACCOUNTING,                                            \
C_2_PBS_CHECKPOINT,                                            \
C_2_PBS_LOCATE,                                                \
C_2_PBS_MESSAGE,                                               \
C_2_PBS_TRACK,                                                 \
C_2_SW_DEV,                                                    \
C_2_UPE,                                                       \
C_2_VERSION,                                                   \
C_ADVISORY_INFO,                                               \
C_AIO_LISTIO_MAX,                                              \
C_AIO_MAX,                                                     \
C_AIO_PRIO_DELTA_MAX,                                          \
C_ARG_MAX,                                                     \
C_ASYNCHRONOUS_IO,                                             \
C_ATEXIT_MAX,                                                  \
C_BARRIERS,                                                    \
C_BC_BASE_MAX,                                                 \
C_BC_DIM_MAX,                                                  \
C_BC_SCALE_MAX,                                                \
C_BC_STRING_MAX,                                               \
C_CHILD_MAX,                                                   \
C_CLK_TCK,                                                     \
C_CLOCK_SELECTION,                                             \
C_COLL_WEIGHTS_MAX,                                            \
C_CPUTIME,                                                     \
C_DELAYTIMER_MAX,                                              \
C_EXPR_NEST_MAX,                                               \
C_FSYNC,                                                       \
C_GETGR_R_SIZE_MAX,                                            \
C_GETPW_R_SIZE_MAX,                                            \
C_HOST_NAME_MAX,                                               \
C_IOV_MAX,                                                     \
C_IPV6,                                                        \
C_JOB_CONTROL,                                                 \
C_LINE_MAX,                                                    \
C_LOGIN_NAME_MAX,                                              \
C_MAPPED_FILES,                                                \
C_MEMLOCK,                                                     \
C_MEMLOCK_RANGE,                                               \
C_MEMORY_PROTECTION,                                           \
C_MESSAGE_PASSING,                                             \
C_MONOTONIC_CLOCK,                                             \
C_MQ_OPEN_MAX,                                                 \
C_MQ_PRIO_MAX,                                                 \
C_NGROUPS_MAX

#define P00_POSIX_SC_2                                         \
C_OPEN_MAX,                                                    \
C_PAGE_SIZE,                                                   \
C_PAGESIZE,                                                    \
C_PRIORITIZED_IO,                                              \
C_PRIORITY_SCHEDULING,                                         \
C_RAW_SOCKETS,                                                 \
C_RE_DUP_MAX,                                                  \
C_READER_WRITER_LOCKS,                                         \
C_REALTIME_SIGNALS,                                            \
C_REGEXP,                                                      \
C_RTSIG_MAX,                                                   \
C_SAVED_IDS,                                                   \
C_SEM_NSEMS_MAX,                                               \
C_SEM_VALUE_MAX,                                               \
C_SEMAPHORES,                                                  \
C_SHARED_MEMORY_OBJECTS,                                       \
C_SHELL,                                                       \
C_SIGQUEUE_MAX,                                                \
C_SPAWN,                                                       \
C_SPIN_LOCKS,                                                  \
C_SPORADIC_SERVER,                                             \
C_STREAM_MAX,                                                  \
C_SYMLOOP_MAX,                                                 \
C_SYNCHRONIZED_IO,                                             \
C_TIMEOUTS,                                                    \
C_TIMER_MAX,                                                   \
C_TIMERS,                                                      \
C_TRACE,                                                       \
C_TRACE_EVENT_FILTER,                                          \
C_TRACE_INHERIT,                                               \
C_TRACE_LOG,                                                   \
C_TTY_NAME_MAX,                                                \
C_TYPED_MEMORY_OBJECTS,                                        \
C_TZNAME_MAX,                                                  \
C_VERSION

#define P00_POSIX_SC_THREAD                                    \
C_THREAD_ATTR_STACKADDR,                                       \
C_THREAD_ATTR_STACKSIZE,                                       \
C_THREAD_CPUTIME,                                              \
C_THREAD_DESTRUCTOR_ITERATIONS,                                \
C_THREAD_KEYS_MAX,                                             \
C_THREAD_PRIO_INHERIT,                                         \
C_THREAD_PRIO_PROTECT,                                         \
C_THREAD_PRIORITY_SCHEDULING,                                  \
C_THREAD_PROCESS_SHARED,                                       \
C_THREAD_SAFE_FUNCTIONS,                                       \
C_THREAD_SPORADIC_SERVER,                                      \
C_THREAD_STACK_MIN,                                            \
C_THREAD_THREADS_MAX,                                          \
C_THREADS

#define P00_POSIX_SC_V6                                        \
C_V6_ILP32_OFF32,                                              \
C_V6_ILP32_OFFBIG,                                             \
C_V6_LP64_OFF64,                                               \
C_V6_LPBIG_OFFBIG

#define P00_POSIX_SC_XOPEN                                     \
C_XOPEN_CRYPT,                                                 \
C_XOPEN_ENH_I18N,                                              \
C_XOPEN_LEGACY,                                                \
C_XOPEN_REALTIME,                                              \
C_XOPEN_REALTIME_THREADS,                                      \
C_XOPEN_SHM,                                                   \
C_XOPEN_UNIX,                                                  \
C_XOPEN_VERSION

#define P00_POSIX_SC_XBS5                                      \
C_XBS5_ILP32_OFF32,                                            \
C_XBS5_ILP32_OFFBIG,                                           \
C_XBS5_LP64_OFF64,                                             \
C_XBS5_LPBIG_OFFBIG


#define P00_POSIX_SC_6                                         \
C_SS_REPL_MAX,                                                 \
C_TRACE_EVENT_NAME_MAX,                                        \
C_TRACE_NAME_MAX,                                              \
C_TRACE_SYS_MAX,                                               \
C_TRACE_USER_EVENT_MAX,                                        \
C_XOPEN_STREAMS

#define P00_DECLARE_SC_ENUM_(NAME, X, I) P99_PASTE2(P00_POS_S, X)

#define P00_DECLARE_SC_ENUM(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEQ, P00_DECLARE_SC_ENUM_, __VA_ARGS__)

enum {
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_1),
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_2),
#ifdef _SC_XOPEN_THREADS
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_THREAD),
#endif
#ifdef _SC_XOPEN_VERSION
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_XOPEN),
#endif
#ifdef _SC_XBS5_ILP32_OFF32
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_XBS5),
#endif
#ifdef _SC_V6_ILP32_OFF32
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_V6),
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
  P00_DECLARE_SC_ENUM(P00_POSIX_SC_6),
#endif
  P00_POS_SC_MAX
};

#define P00_DECLARE_SC_(NAME, X, I)                            \
/** @brief return the POSIX system config feature X */         \
inline long P99_PASTE2(P99_S, X)(void) {                       \
  switch (P99_PASTE2(_S, X)) {                                 \
    /* OPEN_MAX can be changed with setrlimit */               \
  case _SC_OPEN_MAX: return sysconf(_SC_OPEN_MAX);             \
  default: return p00_sc_constant[P99_PASTE2(P00_POS_S, X)];   \
  }                                                            \
}

#define P00_DECLARE_SC(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SER, P00_DECLARE_SC_, __VA_ARGS__);

P00_DECLARE_SC(P00_POSIX_SC_1)
P00_DECLARE_SC(P00_POSIX_SC_2)
#ifdef _SC_XOPEN_THREADS
  P00_DECLARE_SC(P00_POSIX_SC_THREAD)
#endif
#ifdef _SC_XOPEN_VERSION
  P00_DECLARE_SC(P00_POSIX_SC_XOPEN)
#endif
#ifdef _SC_XBS5_ILP32_OFF32
 P00_DECLARE_SC(P00_POSIX_SC_XBS5)
#endif
#ifdef _SC_V6_ILP32_OFF32
  P00_DECLARE_SC(P00_POSIX_SC_V6)
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
 P00_DECLARE_SC(P00_POSIX_SC_6)
#endif

#undef P00_DECLARE_SC_ENUM_
#undef P00_DECLARE_SC_ENUM
#undef P00_DECLARE_SC_
#undef P00_DECLARE_SC

#endif 	    /* !ORWL_POSIX_H_ */
