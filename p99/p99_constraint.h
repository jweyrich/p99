/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_CONSTRAINT_H
#define P99_CONSTRAINT_H
#include "p99_tss.h"
#include "p99_c99.h"
#include "p99_errno.h"

/**
 ** @addtogroup C11_library
 **
 ** @{
 **/

/**
 ** @brief calculates the length of the (untruncated) locale-specific
 ** message string that the strerror_s function maps to errnum.
 **/
p99_inline size_t strerrorlen_s(errno_t p00_errnum);

/**
 ** @brief computes the length of the string pointed to by s.
 **/
p99_inline size_t strnlen_s(const char *p00_s, size_t p00_maxsize);


/**
 ** @brief maps the number in errnum to a locale-specific message
 ** string.
 **/
p99_inline
errno_t strerror_s(char *p00_s, rsize_t p00_maxsize, errno_t p00_errnum);

P99_WEAK(exit_handler_s)
void exit_handler_s(const char * restrict p00_msg,
                    void * restrict p00_ptr,
                    errno_t p00_err);

/**
 ** @brief A pointer to the ::ignore_handler_s function shall be a
 ** suitable argument to the ::set_constraint_handler_s function.
 **
 ** The ::ignore_handler_s function simply returns to its caller.
 **/
P99_WEAK(ignore_handler_s)
void ignore_handler_s(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err);

/**
 ** @brief A pointer to the ::abort_handler_s function shall be a
 ** suitable argument to the ::set_constraint_handler_s function.
 **
 ** The ::abort_handler_s function writes a message on the standard
 ** error stream in an implementation-defined format. The message
 ** shall include the string pointed to by @a p00_msg.  The
 ** ::abort_handler_s function then calls the @c abort function.
 **
 ** @return The ::abort_handler_s function does not return to its
 ** caller.
 **/
P99_WEAK(abort_handler_s)
void abort_handler_s(const char * restrict p00_msg,
                     void * restrict p00_ptr,
                     errno_t p00_err);


typedef
void (*constraint_handler_t)(const char * restrict p00_msg,
                             void * restrict p00_ptr,
                             errno_t p00_err);

/**
 ** @brief sets the runtime-constraint handler to be handler.
 **/
p99_inline
constraint_handler_t set_constraint_handler_s(constraint_handler_t handler);


P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_file);
P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_context);
P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_info);

#define P00_JMP_BUF_FILE P99_THREAD_LOCAL(p00_jmp_buf_file)
#define P00_JMP_BUF_CONTEXT P99_THREAD_LOCAL(p00_jmp_buf_context)
#define P00_JMP_BUF_INFO P99_THREAD_LOCAL(p00_jmp_buf_info)

P99_CONSTANT(int, p00_ilen10, 256);

#if __STDC_WANT_LIB_EXT1__ && !defined(__STDC_LIB_EXT1__)
p99_inline
size_t strerrorlen_s(errno_t p00_errnum) {
  return strlen(strerror(p00_errnum));
}
#endif

#if (_XOPEN_SOURCE >= 600)
p99_inline
int p00_strerror(int p00_errname, size_t p00_maxsize, char p00_s[p00_maxsize]) {
# if _GNU_SOURCE
  char * p00_ret = strerror_r(p00_errname, p00_s, p00_maxsize);
  if (p00_ret != p00_s) {
    if (strlen(p00_ret) < p00_maxsize) {
      strcpy(p00_s, p00_ret);
    } else {
      errno = ERANGE;
      return -1;
    }
  }
  return 0;
# else
  return strerror_r(p00_errname, p00_s, p00_maxsize);
# endif
}
#elif __STDC_WANT_LIB_EXT1__ && defined(__STDC_LIB_EXT1__)
p99_inline
int p00_strerror(int p00_errname, size_t p00_maxsize, char p00_s[p00_maxsize]) {
  return strerror_s(p00_s, p00_maxsize, p00_errname);
}
#else
p99_inline
int p00_strerror(int p00_errname, size_t p00_maxsize, char p00_s[p00_maxsize]) {
  size_t p00_len = strerrorlen_s(p00_errname);
  size_t p00_ret = 0;
  if (p00_len > p00_maxsize - 1) {
    p00_len = p00_maxsize - 1;
    p00_ret = ERANGE;
  }
  strncpy(p00_s, strerror(p00_errname), p00_len);
  p00_s[p00_len] = 0;
  return p00_ret;
}
#endif

p99_inline
char const* p00_strerror_r(int p00_errname, size_t p00_maxsize, char p00_s[p00_maxsize]) {
  p00_strerror(p00_errname, p00_maxsize, p00_s);
  return p00_s;
}

#define P00_STRERROR2(E, STR) p00_strerror(E, sizeof(STR), STR)

#define P99_STRERROR_MAX 256

#define p00_strerror(...)                                               \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                                     \
(p00_strerror(__VA_ARGS__, P99_STRERROR_MAX, (char[P99_STRERROR_MAX]))) \
(P99_IF_LT(P99_NARG(__VA_ARGS__), 3)                                    \
 (P00_STRERROR2(__VA_ARGS__))                                           \
 (p00_strerror(__VA_ARGS__)))

#if __STDC_WANT_LIB_EXT1__ && !defined(__STDC_LIB_EXT1__)
P99_WEAK(p99_constraint_handler)
void p99_constraint_handler(const char * restrict p00_msg,
                            void * restrict p00_ptr,
                            errno_t p00_err);


p99_inline
size_t strnlen_s(const char *p00_s, size_t p00_maxsize) {
  size_t p00_ret = p00_maxsize;
  if (p00_s && p00_maxsize) {
    char const* p00_pos = memchr(p00_s, 0, p00_maxsize);
    if (p00_pos) p00_ret = p00_pos - p00_s;
  }
  return p00_ret;
}

p99_inline
errno_t strerror_s(char *p00_s, rsize_t p00_maxsize, errno_t p00_errnum) {
  if (!p00_maxsize || p00_maxsize > RSIZE_MAX)
    p99_constraint_handler(
      ", call to strerror_s, dynamic constraint violation",
      0,
      EINVAL);
  // strerror_r may set errno
  errno_t p00_back = errno;
  errno = 0;
  int p00_ret = p00_strerror(p00_errnum, p00_maxsize, p00_s);
  switch (p00_ret) {
  case 0: break;
  case ERANGE: {
    /* The output string has been shortend to fit into p00_s. */
    p00_ret = 0;
    if (p00_maxsize > 3) memset(p00_s + (p00_maxsize - 4), '.', 3);
    break;
  }
  case EINVAL: {
    /* p00_errnum didn't correspond to a valid error number. */
    p00_ret = snprintf(p00_s, p00_maxsize, "unknown <%d> error", p00_errnum);
    if (!p00_ret || (p00_ret < p00_maxsize)) {
      p00_ret = 0;
      if (p00_maxsize > 3) memset(p00_s + (p00_maxsize - 4), '.', 3);
    }
    break;
  }
  default:
    P99_FPRINTF(stderr, "strerror_r returned %s\n", p00_ret);
    break;
  }
  /* Unconditionally set errno to the previous value. */
  errno = p00_back;
  return p00_ret;
}
#endif

p99_inline
char const* p00_strerror_s(char *p00_s, rsize_t p00_maxsize, errno_t p00_errnum) {
  strerror_s(p00_s, p00_maxsize, p00_errnum);
  return p00_s;
}

# define P00_STRERROR(E, S, STR) p00_strerror_s(STR, S, E)
#else
# define P00_STRERROR(...) p00_strerror_r(__VA_ARGS__)
#endif

#define P00_STRERROR02(E, STR) P00_STRERROR(E, sizeof(STR), STR)

#define P99_STRERROR(...)                                                    \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                                          \
(P00_STRERROR(__VA_ARGS__, P99_STRERROR_MAX, (char[P99_STRERROR_MAX]){ 0 })) \
(P99_IF_LT(P99_NARG(__VA_ARGS__), 3)                                         \
 (P00_STRERROR02(__VA_ARGS__))                                               \
 (P00_STRERROR(__VA_ARGS__)))

p99_inline
void p00_constraint_report(errno_t p00_cond, char const* p00_file, char const* p00_context, char const* p00_info) {
  char p00_str[p00_ilen10] = P99_INIT;
  if (!p00_context) p00_context = P00_JMP_BUF_CONTEXT;
  if (!p00_context) p00_context = "<unknown function>";
  if (!p00_info) p00_info = P00_JMP_BUF_INFO;
  if (!p00_file) p00_file = P00_JMP_BUF_FILE;
  if (!p00_file) p00_file = "<unknown location>";
  fputs(p00_context, stderr);
  fputc(':', stderr);
  fputs(p00_file, stderr);
  fputs(": ", stderr);
  if (p00_info) {
    fputs(p00_info, stderr);
    fputs(", ", stderr);
  }
  fputs("exception ", stderr);
  {
    char const*const p00_errname = p99_errno_getname(p00_cond);
    if (p00_errname) {
      fputs(p00_errname, stderr);
      fputc('=', stderr);
    }
  }
  {
    sprintf(p00_str, "%d", p00_cond);
    fputs(p00_str, stderr);
  }
  if (!p00_cond && errno) p00_cond = errno;
  P99_STRERROR(p00_cond, p00_str);
  if (p00_str[0]) {
    fputs(", library error: ", stderr);
    fputs(p00_str, stderr);
  }
  fputc('\n', stderr);
}

p99_inline
void p99_report_handler(const char * restrict p00_msg,
                        void * restrict p00_ptr,
                        errno_t p00_err) {
  p00_constraint_report(p00_err, 0, 0, p00_msg);
}

p99_inline
void p99_ignore_handler(const char * restrict p00_msg,
                        void * restrict p00_ptr,
                        errno_t p00_err) {
  /* empty */
}

p99_inline
void p99_abort_handler(const char * restrict p00_msg,
                       void * restrict p00_ptr,
                       errno_t p00_err) {
  p00_constraint_report(p00_err, 0, 0, p00_msg);
  fputs("runtime constraint violation: ", stderr);
  abort();
}

p99_inline
void p99_exit_handler(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err) {
  p00_constraint_report(p00_err, 0, 0, p00_msg);
  fputs("runtime constraint violation: ", stderr);
  exit(EXIT_FAILURE);
}

#ifndef P99_CONSTRAINT_HANDLER
# define P99_CONSTRAINT_HANDLER exit_handler_s
#endif

#if __STDC_WANT_LIB_EXT1__

P99_DECLARE_ATOMIC(constraint_handler_t);

P99_WEAK(report_handler_s)
void report_handler_s(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err);

P99_WEAK(exit_handler_s)
void exit_handler_s(const char * restrict p00_msg,
                    void * restrict p00_ptr,
                    errno_t p00_err);

P99_WEAK(p00_constraint_handler)
_Atomic(constraint_handler_t) p00_constraint_handler = ATOMIC_VAR_INIT(P99_CONSTRAINT_HANDLER);

P99_WEAK(report_handler_s)
void report_handler_s(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err) {
  p99_report_handler(p00_msg, p00_ptr, p00_err);
}

P99_WEAK(exit_handler_s)
void exit_handler_s(const char * restrict p00_msg,
                    void * restrict p00_ptr,
                    errno_t p00_err) {
  p99_exit_handler(p00_msg, p00_ptr, p00_err);
}

P99_WEAK(p99_constraint_handler)
void p99_constraint_handler(const char * restrict p00_msg,
                            void * restrict p00_ptr,
                            errno_t p00_err) {
  constraint_handler_t p00_func = atomic_load(&p00_constraint_handler);
  if (p00_func) p00_func(p00_msg, p00_ptr, p00_err);
}


# ifndef __STDC_LIB_EXT1__

P99_WEAK(ignore_handler_s)
void ignore_handler_s(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err) {
  p99_ignore_handler(p00_msg, p00_ptr, p00_err);
}

P99_WEAK(abort_handler_s)
void abort_handler_s(const char * restrict p00_msg,
                     void * restrict p00_ptr,
                     errno_t p00_err) {
  p99_abort_handler(p00_msg, p00_ptr, p00_err);
}

p99_inline
constraint_handler_t set_constraint_handler_s(constraint_handler_t handler) {
  if (!handler) handler = P99_CONSTRAINT_HANDLER;
  return atomic_fetch_and_store(&p00_constraint_handler, handler);
}

# endif

p99_inline
errno_t p00_constraint_call(errno_t p00_cond, char const* p00_file, char const* p00_context, char const* p00_info) {
  if (p00_cond) {
    if (p00_file) P00_JMP_BUF_FILE = p00_file;
    if (p00_context) P00_JMP_BUF_CONTEXT = p00_context;
    p99_constraint_handler(p00_info, 0, p00_cond);
  }
  return p00_cond;
}

#define P00_CONSTRAINT_INFO(F) ", call to " #F ", dynamic constraint violation"


#define P99_CONSTRAINT_TRIGGER(E, I)                           \
p00_constraint_call((E), P99_STRINGIFY(__LINE__), __func__, I)

#define P00_CONSTRAINT_CALL3(F, I, C)                          \
P99_CONSTRAINT_TRIGGER(F C, I)

#define P00_CONSTRAINT_CALL1(F) P00_CONSTRAINT_CALL3(F, P00_CONSTRAINT_INFO(F), ())

#define P00_CONSTRAINT_CALL0(F, ...) P00_CONSTRAINT_CALL3(F, P00_CONSTRAINT_INFO(F), (__VA_ARGS__))

#define P99_CONSTRAINT_CALL(...)                               \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                            \
(P00_CONSTRAINT_CALL1(__VA_ARGS__))                            \
(P00_CONSTRAINT_CALL0(__VA_ARGS__))


# if !defined(__STDC_LIB_EXT1__) || defined(P00_DOXYGEN)

p99_inline
errno_t p00_memcpy_s(_Bool p00_overlap,
                     void * restrict p00_s1, rsize_t p00_s1max,
                     const void * restrict p00_s2, rsize_t p00_n) {
  errno_t p00_ret = 0;
  if (!p00_s1) { p00_ret = EINVAL; goto P00_SEVERE; }
  if (p00_s1max > RSIZE_MAX) { p00_ret = ERANGE; goto P00_SEVERE; }
  if (!p00_s2) { p00_ret = EINVAL; goto P00_ERR; }
  if (p00_n > RSIZE_MAX || p00_n > p00_s1max) { p00_ret = ERANGE; goto P00_ERR; }
  if (!p00_overlap) {
    unsigned char const* p00_c1 = p00_s1;
    unsigned char const* p00_c2 = p00_s2;
    /* Strictly speaking all this range check is UB if the regions don't
       overlap. */
    if ((p00_c1 <= p00_c2 && p00_c2 < (p00_c1 + p00_s1max))
        || (p00_c2 <= p00_c1 && p00_c1 < (p00_c2 + p00_n))) {
      p00_ret = EINVAL; goto P00_ERR;
    }
    memcpy(p00_s1, p00_s2, p00_n);
  } else {
    memmove(p00_s1, p00_s2, p00_n);
  }
  return 0;
P00_ERR:
  if (p00_s1max) memset(p00_s1, 0, p00_s1max);
P00_SEVERE:
  return p00_ret;
}

/** @ingroup C11_library **/
# define memcpy_s(S1, S1MAX, S2, N)  P99_CONSTRAINT_TRIGGER(p00_memcpy_s(false, (S1), (S1MAX), (S2), (N)), "memcpy_s runtime constraint violation")
/** @ingroup C11_library **/
# define memmove_s(S1, S1MAX, S2, N) P99_CONSTRAINT_TRIGGER(p00_memcpy_s(true, (S1), (S1MAX), (S2), (N)), "memmove_s runtime constraint violation")

p99_inline
errno_t p00_strcpy_s(void * restrict p00_s1, rsize_t p00_s1max,
                     const void * restrict p00_s2) {
  size_t p00_len = strnlen_s(p00_s2, p00_s1max) + 1;
  return p00_memcpy_s(false, p00_s1, p00_s1max, p00_s2, p00_len);
}

/** @ingroup C11_library **/
# define strcpy_s(S1, S1MAX, S2)  P99_CONSTRAINT_TRIGGER(p00_strcpy_s((S1), (S1MAX), (S2)), "strcpy_s runtime constraint violation")

p99_inline
errno_t p00_strncpy_s(char * restrict p00_s1,
                      rsize_t p00_s1max,
                      const char * restrict p00_s2,
                      rsize_t p00_n) {
  if (!p00_s1 || !p00_s1max || p00_s1max > RSIZE_MAX) return memcpy_s(p00_s1, p00_s1max, p00_s2, p00_n);
  if (!p00_s2 || p00_n > RSIZE_MAX) return memcpy_s(p00_s1, p00_s1max, p00_s2, p00_n);
  /* The maximum string length that we can copy is p00_n - 1. */
  size_t p00_len = strnlen_s(p00_s2, p00_n - 1) + 1;
  /* If the target can't hold the string, the standard demands to
     abort the copy operation. */
  if (p00_s1max < p00_len) return memcpy_s(p00_s1, p00_s1max, 0, 0);
  /* Now copy and force null termination. */
  size_t p00_ret = memcpy_s(p00_s1, p00_s1max, p00_s2, p00_len - 1);
  p00_s1[p00_len - 1] = 0;
  return p00_ret;
}

# define strncpy_s(S1, S1MAX, S2, N)  P99_CONSTRAINT_TRIGGER(p00_strncpy_s((S1), (S1MAX), (S2), (N)), "strncpy_s runtime constraint violation")

p99_inline
errno_t p00_strcat_s(char * restrict p00_s1,
                     rsize_t p00_s1max,
                     const char * restrict p00_s2) {
  if (!p00_s1 || !p00_s1max || p00_s1max > RSIZE_MAX) return memcpy_s(p00_s1, p00_s1max, p00_s2, 0);
  if (!p00_s2) return memcpy_s(p00_s1, p00_s1max, p00_s2, 0);
  size_t p00_l1 = strnlen_s(p00_s1, p00_s1max);
  /* Check if p00_s1 had been null terminated */
  if (p00_l1 >= p00_s1max) return memcpy_s(p00_s1, 1, 0, 0);
  else p00_s1max -= p00_l1;
  /* Only look into the string that could fit after the current
     contents of p00_s1. */
  size_t p00_l2 = strnlen_s(p00_s2, p00_s1max) + 1;
  if (p00_l2 > p00_s1max) return memcpy_s(p00_s1, 1, p00_s2, 0);
  size_t p00_ret = strncpy_s(p00_s1 + p00_l1, p00_s1max, p00_s2, p00_l2);
  if (p00_ret) p00_s1[0] = 0;
  return p00_ret;
}

/** @ingroup C11_library **/
# define strcat_s(S1, S1MAX, S2)  P99_CONSTRAINT_TRIGGER(p00_strcat_s((S1), (S1MAX), (S2)), "strcat_s runtime constraint violation")

p99_inline
errno_t p00_strncat_s(char * restrict p00_s1,
                      rsize_t p00_s1max,
                      const char * restrict p00_s2,
                      rsize_t p00_n) {
  if (!p00_s1 || !p00_s1max || p00_s1max > RSIZE_MAX) return memcpy_s(p00_s1, p00_s1max, p00_s2, 0);
  if (!p00_s2 || p00_n > RSIZE_MAX) return memcpy_s(p00_s1, p00_s1max, p00_s2, 0);
  size_t p00_l1 = strnlen_s(p00_s1, p00_s1max);
  /* Check if p00_s1 had been null terminated */
  if (p00_l1 >= p00_s1max) return memcpy_s(p00_s1, 1, 0, 0);
  else p00_s1max -= p00_l1;
  /* Only look into the string that could fit after the current
     contents of p00_s1. */
  size_t p00_l2 = strnlen_s(p00_s2, p00_n) + 1;
  if (p00_l2 > p00_s1max) return memcpy_s(p00_s1, 1, p00_s2, 0);
  size_t p00_ret = strncpy_s(p00_s1 + p00_l1, p00_s1max, p00_s2, p00_l2);
  if (p00_ret) p00_s1[0] = 0;
  return p00_ret;
}

/** @ingroup C11_library **/
# define strncat_s(S1, S1MAX, S2, N)  P99_CONSTRAINT_TRIGGER(p00_strncat_s((S1), (S1MAX), (S2), (N)), "strncat_s runtime constraint violation")

p99_inline
void *p00_bsearch_s(char const* p00_file, char const* p00_context,
                    const void *key, const void *base,
                    rsize_t nmemb, rsize_t size,
                    int (*compar)(const void *k, const void *y,
                                  void *context),
                    void *context) {
  if (nmemb) {
    if (nmemb > RSIZE_MAX || size > RSIZE_MAX) {
      p00_constraint_call(ERANGE, p00_file, p00_context, "bsearch_s runtime constraint violation");
    } else if (nmemb && (!key || !base || !compar)) {
      p00_constraint_call(EINVAL, p00_file, p00_context, "bsearch_s runtime constraint violation");
    } else {
      typedef unsigned char const dummy[size];
      register dummy const * dbase = (void*)base;
      /* bot and top will always be the maximal (minimal) index that
         is known to be smaller (larger) than the search item. This
         strategy here even works when nmemb == (rsize_t)-1. The
         wonders of unsigned arithmetic... */
      for (register size_t bot = -1, top = nmemb;
           (top - bot) != 1;) {
        /* unsigned arithmetic just wraps around, so this is ok            */
        register size_t med = (bot + top) / 2;
        /* We always have the assertions                                   */
        /* assert((bot == top && top == (rsize_t)-1)                       */
        /*        || (top - bot > 1));                                     */
        /* assert(med < top);                                              */
        /* assert(bot < med || (bot == (rsize_t)-1 && med != (rsize_t)-1); */
        int co = compar(key, dbase + med, context);
        if (!co) return (void*)(dbase + med);
        else if (co < 0) top = med;
        else bot = med;
      }
    }
  }
  return 0;
}

/** @ingroup C11_library **/
#define bsearch_s(...) p00_bsearch_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

p99_inline
errno_t p00_getenv_s(char const* p00_file, char const* p00_context,
                     size_t * restrict len,
                     char * restrict value, rsize_t maxsize,
                     const char * restrict name) {
  errno_t ret = 0;
  size_t le = 0;
  if (maxsize > RSIZE_MAX) {
    ret = ERANGE;
    p00_constraint_call(ret, p00_file, p00_context, "getenv_s runtime constraint violation");
  } else if (!name || (maxsize && !value)) {
    ret = EINVAL;
    p00_constraint_call(ret, p00_file, p00_context, "getenv_s runtime constraint violation");
  } else {
    char const*const found = getenv(name);
    if (found) {
      le = strlen(found);
      if (le < maxsize) strcpy(value, found);
    } else {
      if (maxsize) value[0] = '\0';
    }
  }
  if (len) *len = le;
  return ret;
}

/** @ingroup C11_library **/
#define getenv_s(...) p00_getenv_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

# endif

/**
 ** @}
 **/



#endif
