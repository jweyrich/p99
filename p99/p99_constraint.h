/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is as follows:               */
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
#include "p99_errno.h"

P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_file);
P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_context);
P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_info);

#define P00_JMP_BUF_FILE P99_THREAD_LOCAL(p00_jmp_buf_file)
#define P00_JMP_BUF_CONTEXT P99_THREAD_LOCAL(p00_jmp_buf_context)
#define P00_JMP_BUF_INFO P99_THREAD_LOCAL(p00_jmp_buf_info)

P99_CONSTANT(int, p00_ilen10, sizeof(P99_STRINGIFY(LLONG_MIN)));

p99_inline
void p00_constraint_report(errno_t p00_cond, char const* p00_file, char const* p00_context, char const* p00_info) {
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
    char p00_str[p00_ilen10];
    sprintf(p00_str, "%d", p00_cond);
    fputs(p00_str, stderr);
  }
  if (!p00_cond && errno) p00_cond = errno;
  char const* errstr = strerror(p00_cond);
  if (errstr) {
    fputs(", library error: ", stderr);
    fputs(errstr, stderr);
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

#ifndef P99_CONSTRAINT_HANDLER
# define P99_CONSTRAINT_HANDLER report_handler_s
#endif

#if __STDC_WANT_LIB_EXT1__

typedef
void (*constraint_handler_t)(const char * restrict p00_msg,
                             void * restrict p00_ptr,
                             errno_t p00_err);

P99_DECLARE_ATOMIC(constraint_handler_t);

P99_WEAK(report_handler_s)
void report_handler_s(const char * restrict p00_msg,
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

p99_inline
void p99_constraint_handler(const char * restrict p00_msg,
                            void * restrict p00_ptr,
                            errno_t p00_err) {
  constraint_handler_t func = atomic_load(&p00_constraint_handler);
  if (func) func(p00_msg, p00_ptr, p00_err);
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
#endif

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


#define P99_CONSTRAINT_TRIGGER(E, I)                       \
p00_constraint_call((E), P99_STRINGIFY(__LINE__), __func__, I)

#define P00_CONSTRAINT_CALL3(F, I, C)         \
P99_CONSTRAINT_TRIGGER(F C, I)

#define P00_CONSTRAINT_CALL1(F) P00_CONSTRAINT_CALL3(F, P00_CONSTRAINT_INFO(F), ())

#define P00_CONSTRAINT_CALL0(F, ...) P00_CONSTRAINT_CALL3(F, P00_CONSTRAINT_INFO(F), (__VA_ARGS__))

#define P99_CONSTRAINT_CALL(...)                \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_CONSTRAINT_CALL1(__VA_ARGS__))             \
(P00_CONSTRAINT_CALL0(__VA_ARGS__))

#endif
