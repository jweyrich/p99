/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2012-2013 Jens Gustedt, INRIA, France                       */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_CONSTRAINT_H
#define P99_CONSTRAINT_H
#include "p99_bitset.h"

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
# if _GNU_SOURCE && __GLIBC__
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


P99_PURE_FUNCTION
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
    fprintf(stderr, "strerror_r returned %d\n", p00_ret);
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
  P99_UNUSED(p00_ptr);
  p00_constraint_report(p00_err, 0, 0, p00_msg);
}

P99_CONST_FUNCTION
p99_inline
void p99_ignore_handler(const char * restrict p00_msg,
                        void * restrict p00_ptr,
                        errno_t p00_err) {
  P99_UNUSED(p00_msg);
  P99_UNUSED(p00_ptr);
  P99_UNUSED(p00_err);
}

noreturn
p99_inline
void p99_abort_handler(const char * restrict p00_msg,
                       void * restrict p00_ptr,
                       errno_t p00_err) {
  P99_UNUSED(p00_ptr);
  p00_constraint_report(p00_err, 0, 0, p00_msg);
  fputs("runtime constraint violation: ", stderr);
  abort();
}

noreturn
p99_inline
void p99_exit_handler(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err) {
  P99_UNUSED(p00_ptr);
  P99_UNUSED(p00_err);
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
noreturn
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
P99_CONST_FUNCTION
void ignore_handler_s(const char * restrict p00_msg,
                      void * restrict p00_ptr,
                      errno_t p00_err) {
  p99_ignore_handler(p00_msg, p00_ptr, p00_err);
}

P99_WEAK(abort_handler_s)
noreturn
void abort_handler_s(const char * restrict p00_msg,
                     void * restrict p00_ptr,
                     errno_t p00_err) {
  p99_abort_handler(p00_msg, p00_ptr, p00_err);
}

p99_inline
constraint_handler_t set_constraint_handler_s(constraint_handler_t p00_hand) {
  if (!p00_hand) p00_hand = P99_CONSTRAINT_HANDLER;
  return atomic_exchange(&p00_constraint_handler, p00_hand);
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
                    const void *p00_key, const void *p00_base,
                    rsize_t p00_nmemb, rsize_t p00_size,
                    int (*p00_compar)(const void *, const void *, void *),
                    void *p00_ctx) {
  if (p00_nmemb) {
    if (p00_nmemb > RSIZE_MAX || p00_size > RSIZE_MAX) {
      p00_constraint_call(ERANGE, p00_file, p00_context, "bsearch_s runtime constraint violation");
    } else if (p00_nmemb && (!p00_key || !p00_base || !p00_compar)) {
      p00_constraint_call(EINVAL, p00_file, p00_context, "bsearch_s runtime constraint violation");
    } else {
      register unsigned char const (*p00_dbase)[p00_size] = (void*)p00_base;
      /* bot and top will always be the maximal (minimal) index that
         is known to be smaller (larger) than the search item. This
         strategy here even works when p00_nmemb == (rsize_t)-1. The
         wonders of unsigned arithmetic... */
      for (register size_t p00_bot = -1, p00_top = p00_nmemb;
           (p00_top - p00_bot) != 1;) {
        /* unsigned arithmetic just wraps around, so this is ok            */
        register size_t p00_med = (p00_bot + p00_top) / 2;
        /* We always have the assertions                                   */
        /* assert((p00_bot == p00_top && p00_top == (rsize_t)-1)                       */
        /*        || (p00_top - p00_bot > 1));                                     */
        /* assert(p00_med < p00_top);                                              */
        /* assert(p00_bot < p00_med || (p00_bot == (rsize_t)-1 && p00_med != (rsize_t)-1); */
        int co = p00_compar(p00_key, p00_dbase + p00_med, p00_ctx);
        if (!co) return (void*)(p00_dbase + p00_med);
        else if (co < 0) p00_top = p00_med;
        else p00_bot = p00_med;
      }
    }
  }
  return 0;
}

/** @ingroup C11_library **/
#define bsearch_s(...) p00_bsearch_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

p99_inline
errno_t p00_getenv_s(char const* p00_file, char const* p00_context,
                     size_t * restrict p00_len,
                     char * restrict p00_value, rsize_t p00_maxsize,
                     const char * restrict p00_name) {
  errno_t p00_ret = 0;
  size_t p00_le = 0;
  if (p00_maxsize > RSIZE_MAX) {
    p00_ret = ERANGE;
    p00_constraint_call(p00_ret, p00_file, p00_context, "getenv_s runtime constraint violation");
  } else if (!p00_name || (p00_maxsize && !p00_value)) {
    p00_ret = EINVAL;
    p00_constraint_call(p00_ret, p00_file, p00_context, "getenv_s runtime constraint violation");
  } else {
    char const*const p00_found = getenv(p00_name);
    if (p00_found) {
      p00_le = strlen(p00_found);
      if (p00_le < p00_maxsize) strcpy(p00_value, p00_found);
    } else {
      if (p00_maxsize) p00_value[0] = '\0';
    }
  }
  if (p00_len) *p00_len = p00_le;
  return p00_ret;
}

/** @ingroup C11_library **/
#define getenv_s(...) p00_getenv_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

p99_inline
errno_t p00_tmpfile_s(char const* p00_file, char const* p00_context,
                      FILE * restrict * restrict p00_streamptr) {
  errno_t p00_ret = 0;
  if (!p00_streamptr) {
    p00_ret = EINVAL;
  } else {
    *p00_streamptr = tmpfile();
    if (!*p00_streamptr) {
      p00_ret = errno;
      errno = 0;
    }
  }
  if (p00_ret) p00_constraint_call(p00_ret, p00_file, p00_context, "tmpfile_s runtime constraint violation");
  return p00_ret;
}

/** @ingroup C11_library **/
#define tmpfile_s(...) p00_tmpfile_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

p99_inline
errno_t p00_tmpnam_s(char const* p00_file, char const* p00_context,
                     char *p00_s, rsize_t p00_maxsize) {
  errno_t p00_ret = 0;
  if (p00_maxsize > RSIZE_MAX) {
    p00_ret = ERANGE;
  } else if (!p00_s) {
    p00_ret = EINVAL;
  } else {
    char p00_b[L_tmpnam] = P99_INIT;
    char * p00_r = tmpnam(p00_b);
    char p00_p = p00_b[L_tmpnam - 1];
    p00_b[L_tmpnam - 1] = 0;
    size_t p00_l = strlen(p00_b);
    if (!p00_r || ((p00_l == (L_tmpnam - 1)) && p00_p) || (p00_l >= p00_maxsize)) {
      p00_ret = ERANGE;
      /* no name could be created or the result was too long */
      p00_s[0] = 0;
    } else {
      strcpy(p00_s, p00_r);
    }
  }
  if (p00_ret) p00_constraint_call(p00_ret, p00_file, p00_context, "tmpnam_s runtime constraint violation");
  return p00_ret;
}

/** @ingroup C11_library **/
#define tmpnam_s(...) p00_tmpnam_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

p99_inline
char *p00_gets_s(char const* p00_file, char const* p00_context,
                 char *p00_s, rsize_t p00_n) {
  errno_t p00_err = 0;
  char* p00_ret = 0;
  if (!p00_s) {
    p00_err = EINVAL;
  } else if (!p00_n) {
    p00_s[0] = '\0';
    p00_err = EINVAL;
  } else if (p00_n > RSIZE_MAX) {
    p00_s[0] = '\0';
    p00_err = ERANGE;
  } else {
    p00_ret = fgets(p00_s, p00_n - 1, stdin);
    if (p00_ret) {
      size_t p00_l = strlen(p00_s);
      if (p00_s[p00_l - 1] == '\n') {
        /* all went well, just zero out the EOL character */
        p00_s[p00_l - 1] = '\0';
      } else {
        /* If the last character has not been a newline the only
           possibility of valid input that the end of the stream has been
           encountered. */
        char p00_b[2];
        char* p00_r = fgets(p00_b, 2, stdin);
        if (p00_r) {
          p00_s[0] = '\0';
          p00_err = ENOBUFS;
        }
      }
    }
  }
  if (p00_err) {
    p00_constraint_call(p00_err, p00_file, p00_context, "gets_s runtime constraint violation");
    return 0;
  } else {
    return p00_ret;
  }
}

/** @ingroup C11_library **/
#define gets_s(...) p00_gets_s(P99_STRINGIFY(__LINE__), __func__, __VA_ARGS__)

P99_PURE_FUNCTION
p99_inline
bool p00_isin0(char p00_c,
               rsize_t p00_s2l, uint8_t const p00_s2[const restrict p00_s2l]) {
  if (p00_c)
    for (rsize_t p00_i = 0; p00_i < (p00_s2l - 1); ++p00_i)
      if (P99_UNLIKELY(p00_c == p00_s2[p00_i]))
        return true;
  return false;
}


P99_PURE_FUNCTION
p99_inline
rsize_t p99_span(rsize_t p00_s1l, uint8_t p00_s1[const restrict p00_s1l],
                 rsize_t p00_s2l, uint8_t const p00_s2[const restrict p00_s2l]) {
  for (rsize_t p00_ret = 0; p00_ret < p00_s1l; ++p00_ret) {
    if (!p00_s1[p00_ret] || !P00_ISIN(p00_s1[p00_ret], p00_s2l, p00_s2))
      return p00_ret;
  }
  return p00_s1l;
}

p99_inline
rsize_t p00_cskip(rsize_t p00_s1l, uint8_t p00_s1[const restrict p00_s1l],
                  rsize_t p00_s2l, uint8_t const p00_s2[const restrict p00_s2l]) {
  rsize_t p00_ret = 0;
  for (; p00_ret < p00_s1l && p00_s1[p00_ret]; ++p00_ret) {
    if (P00_ISIN(p00_s1[p00_ret], p00_s2l, p00_s2)) {
      p00_s1[p00_ret] = '\0';
      return p00_ret + 1;
    }
  }
  return p00_ret;
}

p99_inline
uint8_t *p00_strtok_inner(
  rsize_t * restrict p00_s1max,
  uint8_t p00_ret0[restrict (*p00_s1max)],
  rsize_t p00_s2max,
  const uint8_t p00_s2[const restrict p00_s2max],
  uint8_t ** restrict p00_ptr) {
  /* We have to copy this to a "veritable" pointer because of a bug in
     clang 3.0 that tells us "read-only variable is not assignable"
     for the function parameter. */
  register uint8_t * p00_ret = p00_ret0;
  /* Skip delimiters at the beginning of the string. */
  register size_t const p00_l = p99_span(*p00_s1max, p00_ret, p00_s2max, p00_s2);
  if (p00_l < *p00_s1max) {
    *p00_s1max -= p00_l;
    p00_ret += p00_l;

    register size_t const p00_k = p00_cskip(*p00_s1max, p00_ret, p00_s2max, p00_s2);
    /* The empty token is always returned as null pointer. */
    if (p00_k && ((p00_k > 1) || (*p00_s1max == 1) || !p00_ret[1])) {
      *p00_ptr = p00_ret + p00_k;
      *p00_s1max -=  p00_k;
      return p00_ret;
    }
  }
  return 0;
}

p99_inline
char *p00_strtok_s(char const* p00_file, char const* p00_context,
                   rsize_t * restrict p00_s1max,
                   uint8_t p00_s1[restrict (*p00_s1max)],
                   rsize_t p00_s2max,
                   const uint8_t p00_s2[const restrict p00_s2max],
                   uint8_t ** restrict p00_ptr) {
  errno_t p00_err = 0;
  uint8_t *p00_ret = 0;
  if (P99_UNLIKELY(!p00_s1max || !p00_s2 || !p00_ptr)) {
    p00_err = EINVAL;
  } else if (P99_UNLIKELY(*p00_s1max > RSIZE_MAX)) {
    p00_err = ERANGE;
  } else if (P99_UNLIKELY(!p00_s1 && !*p00_ptr)) {
    p00_err = EINVAL;
  }
  if (P99_UNLIKELY(p00_err)) {
    p00_constraint_call(p00_err, p00_file, p00_context, "strtok_s runtime constraint violation");
  } else {
    if (P99_LIKELY(*p00_s1max)) {
      /* If this is a new scan, initialize the machinery */
      if (p00_s1)
        p00_ret = p00_strtok_inner(p00_s1max, p00_s1, p00_s2max, p00_s2, p00_ptr);
      else
        p00_ret = p00_strtok_inner(p00_s1max, *p00_ptr, p00_s2max, p00_s2, p00_ptr);
    }
  }
  return (char*)p00_ret;
}

#define P00_STRSIZE(S)                                         \
P99_GENERIC(S,                                                 \
            sizeof(S),                                         \
            (char *, strlen(S)+1),                             \
            (char const*, strlen(S)+1),                        \
            (char volatile*, strlen(S)+1),                     \
            (char volatile const*, strlen(S)+1),               \
            (char *restrict, strlen(S)+1),                     \
            (char const*restrict, strlen(S)+1),                \
            (char volatile*restrict, strlen(S)+1),             \
            (char volatile const*restrict, strlen(S)+1)        \
            )


/** @ingroup C11_library **/
#define strtok_s(S1, S1MAX, S2, PTR)                           \
p00_strtok_s(P99_STRINGIFY(__LINE__), __func__,                \
             S1MAX, (uint8_t*restrict)(S1),                    \
             P00_STRSIZE(S2), (uint8_t const*restrict)(S2),    \
             (uint8_t**)PTR)

#define P00_SPAN_DECLARE(NAME, SET)                                                            \
P99_PURE_FUNCTION                                                                              \
p99_inline                                                                                     \
rsize_t P99_PASTE2(p99_span_, NAME)(rsize_t p00_s1l, uint8_t p00_s1[const restrict p00_s1l]) { \
  return p99_span(p00_s1l, p00_s1, sizeof(SET)-1, (uint8_t const*)(char const[]){ SET });      \
}                                                                                              \
p99_inline                                                                                     \
char* P99_PASTE2(p99_strtok_, NAME)(rsize_t * restrict p00_s1max,                              \
                                      char p00_s1[restrict (*p00_s1max)],                      \
                                      char ** restrict p00_ptr) {                              \
  return strtok_s(p00_s1, p00_s1max, SET, p00_ptr);                                            \
}                                                                                              \
P99_MACRO_END(P00_SPAN_DECLARE, NAME)



P00_SPAN_DECLARE(blank, P00_BLANK);
P00_SPAN_DECLARE(space, P00_SPACE);
P00_SPAN_DECLARE(lower, P00_LOWER);
P00_SPAN_DECLARE(upper, P00_UPPER);
P00_SPAN_DECLARE(digit, P00_DIGIT);
P00_SPAN_DECLARE(xdigit, P00_XDIGIT);
P00_SPAN_DECLARE(alpha, P00_ALPHA);
P00_SPAN_DECLARE(alnum, P00_ALNUM);


# endif

/**
 ** @}
 **/



#endif
