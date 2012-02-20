/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2012 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef P99_TRY_H
#define P99_TRY_H
#include "p99_tss.h"
#include "p99_typenames.h"


P99_POINTER_TYPE(p00_jmp_buf0);
P99_DECLARE_ATOMIC(p00_jmp_buf0_ptr);

P99_DECLARE_THREAD_LOCAL(_Atomic(p00_jmp_buf0_ptr), p00_jmp_buf_top);

#define P00_JMP_BUF_TOP P99_THREAD_LOCAL(p00_jmp_buf_top)

P99_DECLARE_THREAD_LOCAL(char_cptr, p00_jmp_buf_file);

#define P00_JMP_BUF_FILE P99_THREAD_LOCAL(p00_jmp_buf_file)

p99_inline
void p00_jmp_skip(p00_jmp_buf0 * p00_des) {
  _Atomic(p00_jmp_buf0_ptr)* p00_head = &P00_JMP_BUF_TOP;
  p00_jmp_buf0 * p00_ret = 0;
  do {
    p00_ret = P99_LIFO_POP(p00_head);
  } while (p00_ret && (p00_ret != p00_des));
}

p99_inline
void p00_jmp_push(p00_jmp_buf0 * p00_des) {
  P99_LIFO_PUSH(&P00_JMP_BUF_TOP, p00_des);
}

enum { p00_ilen10 = sizeof(P99_STRINGIFY(LLONG_MIN)) };

p99_inline
noreturn
void p00_jmp_abort(int p00_cond, char const* p00_file) {
  char p00_str[p00_ilen10];
  sprintf(p00_str, "%d", p00_cond);
  if (!p00_file) p00_file = P00_JMP_BUF_FILE;
  if (!p00_file) p00_file = "<unknown location>";
  fputs(p00_file, stderr);
  fputs(": uncaught exception ", stderr);
  fputs(p00_str, stderr);
  if (!errno && p00_cond) errno = p00_cond;
  if (errno) perror(", could be");
  else fputs("\n", stderr);
  abort();
}

p99_inline
noreturn
void p00_jmp_throw(int p00_cond, p00_jmp_buf0 * p00_top, char const* p00_file) {
  if (p00_file) P00_JMP_BUF_FILE = p00_file;
  if (!p00_top) p00_top = P99_LIFO_TOP(&P00_JMP_BUF_TOP);
  if (P99_LIKELY(p00_top)) p00_longjmp(p00_top, p00_cond);
  else p00_jmp_abort(p00_cond, p00_file);
}


/**
 ** @addtogroup try Try, catch and finally blocks
 **
 ** @{
 **/


/**
 ** @brief Stop execution and the current point and signal an
 ** exception of value @a X to the next ::P99_TRY clause that is
 ** located on the call stack, if any.
 **
 ** @remark If there is no such try clause on the call stack, @c abort
 ** is called.
 **
 ** @param X should be an integer value that fits into an @c int. It
 ** @b must be non-zero. Otherwise the arbitrary value @c 1 as for @c
 ** setjmp / @c longjmp is transferred.
 **
 ** A good convention for the values to throw is using system wide
 ** error numbers such as @c ERANGE. But any other convention that
 ** fits the needs of an application can be used.
 **/
#define P99_THROW(X) p00_jmp_throw((X), p00_unwind_top, __FILE__ ":" P99_STRINGIFY(__LINE__))


/**
 ** @brief Stop execution and the current point inside a ::P99_FINALLY
 ** or ::P99_CATCH clause and propagate the same exception that let
 ** here to the next level.
 **/
#define P99_RETHROW p00_jmp_throw(p00_code, p00_unwind_top, 0)


/**
 ** @brief Create a block that can catch exceptions
 **
 ** @remark This must be used with either ::P99_CATCH or ::P99_FINALLY
 ** following immediately after the depending block.
 **
 ** The simplest use of this feature is together with ::P99_FINALLY
 ** @code
 ** unsigned char*volatile buffer = 0;
 ** P99_TRY {
 **   buffer = malloc(bignumber);
 **   if (!buffer) P99_THROW(thrd_nomem);
 **   // do something complicated with buffer
 **   favorite_func(buffer);
 ** } P99_FINALLY {
 **   free(buffer);
 ** }
 ** @endcode
 **
 ** This will ensure that the buffer allocated in @c buffer will
 ** always be freed, regardless what error conditions the code will
 ** encounter. In particular this will work, even if an exception is
 ** thrown from below the call to @c favorite_func.
 **
 ** If no exception occurs, the ::P99_FINALLY clause is executed
 ** anyhow. Then execution continues after the clause, just as normal
 ** code.
 **
 ** If an exception occurs, the clause is executed (an in this case
 ** the call to @c free is issued). But afterwards execution will not
 ** continue as normal but jump to the next ::P99_FINALLY or
 ** ::P99_CATCH block on the call stack.
 **
 ** An alternative way is to use ::P99_CATCH and to handle different
 ** exceptions explicitly.
 **
 ** @code
 ** unsigned char*volatile buffer = 0;
 ** P99_TRY {
 **   buffer = malloc(bignumber);
 **   if (!buffer) P99_THROW(thrd_nomem);
 **   // do something complicated with buffer
 ** } P99_CATCH(int code) {
 **   switch(code) {
 **     case thrd_nomem: perror("we had an allocation error"); break;
 **     case thrd_timedout: perror("we were timed out"); break;
 **   }
 **   free(buffer);
 **   if (code) P99_RETHROW;
 ** }
 ** @endcode
 **
 ** The difference here is that we receive the error code through the
 ** variable @c code and we can thus take different action for
 ** different exceptional conditions. If it weren't for the
 ** ::P99_RETHROW, the unrolling of the call stack would stop at this
 ** point and execution would continue after the catch block. The
 ** exception would be considered to be caught.
 **
 ** Here, since there is a ::P99_RETHROW, execution will jump to the
 ** next ::P99_FINALLY or ::P99_CATCH block on the call stack. In fact
 ** a catch block of
 **
 ** @code
 ** P99_CATCH(int code) {
 **   // do something here and then
 **   if (code) P99_RETHROW;
 ** }
 ** @endcode
 **
 ** Would be equivalent to
 **
 ** @code
 ** P99_FINALLY {
 **   // do something here
 ** }
 ** @endcode
 **
 ** only that this wouldn't give access to @c code.
 **/
#define P99_TRY                                                \
P99_UNWIND_PROTECT                                             \
/* one phase for the try, one for the finally */               \
for (register unsigned p00_pha = 0; p00_pha < 2u; ++p00_pha)   \
  /* Restrict the first phase to the try */                    \
  if (!p00_pha)                                                \
    P00_BLK_START                                              \
      P00_BLK_BEFORE(p00_jmp_push(p00_unwind_top))             \
      do

#define P00_FINALLY                                                 \
while (0); else if (0) { default: P99_NOP; } else case 1:           \
P00_BLK_START                                                       \
/* make sure that this phase is executed at most once */            \
P00_BLK_BEFORE(p00_pha = 2u)                                        \
/* unwind the lifo to the current point */                          \
P00_BLK_BEFORE(p00_jmp_skip(p00_unwind_top))                        \
/* Hide the top most jump buffer, such that throw or rethrow inside \
   the catch or finally blocks goes to the next level. */           \
P00_BLK_DECL(p00_jmp_buf0*, p00_unwind_top, p00_unwind_prev)


/**
 ** @brief Designate a block that is executed regardless of the
 ** exceptions that were encountered in the corresponding try block
 **
 ** @remark If an exception had been caught to arrive to the finally
 ** clause, the unwind of ::P99_TRY and ::P99_UNWIND_PROTECT blocks
 ** will continue. Otherwise execution will resume normally after the
 ** ::P99_FINALLY clause.
 **
 ** @see P99_TRY
 ** @see P99_CATCH
 **/
#define P99_FINALLY                                            \
P00_FINALLY                                                    \
P00_BLK_AFTER(p00_unw ? P99_RETHROW : P99_NOP)

/**
 ** @brief Designate a block that is executed regardless of the
 ** exceptions that were encountered in the corresponding try block
 **
 ** @remark The argument to ::P99_CATCH should be a declaration of an
 ** integer variable, most likely an @c int.
 **
 ** @remark With a ::P99_CATCH clause, an exception is considered to
 ** be caught. If you want to unwind the call stack even further you
 ** have to use ::P99_RETHROW.
 **
 ** @remark The corresponding code will @c 0 if and only if no
 ** exception occured.
 **
 ** @see P99_TRY
 ** @see P99_THROW
 ** @see P99_RETHROW
 **/
#define P99_CATCH(...)                                         \
P00_FINALLY                                                    \
P00_BLK_BEFORE(__VA_ARGS__ = p00_code)

/**
 ** @}
 **/


#endif
