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
#ifndef   	P99_BLOCK_H_
# define   	P99_BLOCK_H_

#include "p99_c99.h"

/**
 ** @file
 ** @brief Macros that implement controlling blocks.
 ** @see preprocessor_blocks
 **/


/**
 ** @addtogroup preprocessor_blocks Preprocessor BLocks
 ** @brief This defines some macros that can be used to ensure that
 ** certain operations are always executed before entering and after
 ** leaving a particular block.
 ** @{
 **/


/**
 ** @brief A meta-macro to protect a dependent block or statement by
 ** statement @a BEFORE that is executed before the block and @a AFTER
 ** that is executed afterward.
 **
 ** Preliminary exit of the block is possible with @c break or @c continue.
 **
 ** @warning @c return, @c exit() or other functions that don't return
 ** to the caller inside the dependent block will result in not
 ** executing @a AFTER, so be careful.
 **
 ** An example of a potential use is
 ** @code
 ** #define P99_INVARIANT(EXPR) P99_PROTECTED_BLOCK(assert((EXPR) && "failed on entry"), assert((EXPR) && "failed on leave"))
 ** @endcode
 **
 ** Such a construct may then be used like this
 ** @code
 ** P99_INVARIANT(x > 5) {
 **   // do something with x that changes it but ensures that
 **   // it will be strictly less than 5 at the end
 ** }
 ** @endcode
 **
 ** and this would trigger an assertion whenever the condition is not
 ** fulfilled when entering or leaving the block.
 **/
#define P99_PROTECTED_BLOCK(BEFORE, AFTER)                        \
for (int _one1_ = 1;                                              \
     /* be sure to execute BEFORE only at the first evaluation */ \
     (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                \
     /* run AFTER exactly once */                                 \
     ((void)(AFTER), _one1_ = 0))                                 \
  /* Ensure that a `break' will still execute AFTER */            \
  for (; _one1_; _one1_ = 0)

/**
 ** @brief A meta-macro to protect a dependent block or statement by
 ** a guard variable @a NAME of type @a T.
 **
 ** The guard variable is initialized with expression @a INITIAL and
 ** statement @a BEFORE and @a AFTER are executed before and after the
 ** block, respectively.
 **
 ** Preliminary exit of the block is possible with @c break or @c continue.
 **
 ** @warning @c return, @c exit() or other functions that don't return
 ** to the caller inside the dependent block will result in not
 ** executing @a AFTER, so be careful.
 **
 ** An example of a potential use would be a pointer to a mutex
 ** variable. It can be initialized with the address of a mutex as
 ** argument to @a INITIAL. @a BEFORE and @a AFTER may then lock resp
 ** unlock that mutex.
 **/
#define P99_GUARDED_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)          \
for (int _one1_ = 1; _one1_; _one1_ = 0)                            \
  for (T NAME = (INITIAL);                                          \
       /* be sure to execute BEFORE only at the first evaluation */ \
       (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                \
       /* run AFTER exactly once */                                 \
       ((void)(AFTER), _one1_ = 0))                                 \
    /* Ensure that a `break' will still execute AFTER */            \
    for (; _one1_; _one1_ = 0)

/**
 ** @brief Do nothing
 **
 ** This may be placed everywhere where a C statement can be placed.
 **/
#define P99_NOP ((void)0)

/**
 ** @brief Prefer the statements in the argument list over the
 ** statement or block that follows.
 **
 ** The argument list should consist of a possibly empty list of
 ** statements, exactly as you would put them inside a <code>{
 ** ... }</code>. There is no restriction on the statements that you
 ** may put in the list: @c break, @c continue, @c return or @c goto
 ** should work as expected, and the list may even contain commas.
 **
 ** The dependent statement or block will in general not be executed
 ** unless they contain a valid jump target that is jumped to. Such a
 ** jump target may be a normal label or a case label.
 **
 ** @code
 ** char *str = malloc(25);
 ** if (!str) goto CLEANUP;
 ** .
 ** .
 ** P99_PREFER(fprintf(stderr, "Happy: all allocation went well!\n");)
 **   CLEANUP: {
 **      // Do some repair work and exit gracefully
 **      fprintf(stderr, "Unhappy: something went wrong!\n");
 **   }
 ** @endcode
 ** @see P99_AVOID
 **/
#define P99_PREFER(...) if (1) { __VA_ARGS__ } else

/**
 ** @brief Only execute the depending statement or block if it is
 ** jumped into explicitly from the outer block.
 **
 ** This can be used to comment out code temporarily on source
 ** level. This macro is preferable over the common <code>if
 ** (0)</code> dialect that is used for the same purpose, since it has
 ** no problem with a dangling @c else.
 **
 ** This can also be used to handle some exceptional cases to which
 ** you want to jump to explicitly, either by a @c goto or as a @c
 ** switch @c case.
 **
 ** With this the example from ::P99_PREFER reads simply
 ** @code
 ** char *str = malloc(25);
 ** if (!str) goto CLEANUP;
 ** .
 ** .
 ** P99_AVOID
 **   CLEANUP: {
 **     // Do some repair work and exit gracefully
 **     fprintf(stderr, "Unhappy: something went wrong!\n");
 **   }
 ** @endcode
 ** @see P99_PREFER
 **/
#define P99_AVOID P99_PREFER(/* NOP */)

/**
 ** @brief Execute the statements in the argument list.
 **
 ** This is to have several statements executed in a place where
 ** syntactically only one statement (and not a <code>{ ... }</code>
 ** block) is allowed.
 **
 ** The argument list should consist of a possibly empty list of
 ** statements, exactly as you would put them inside a <code>{
 ** ... }</code>. There is no restriction on the statements that you
 ** may put in the list: @c break, @c continue, @c return or @c goto
 ** should work as expected, and the list may even contain commas.
 **
 ** Traditionally this would be done with a construction like
 ** @code
 ** do { __VA_ARGS__ } while(0)
 ** @endcode
 **
 ** That traditional construction changes the control flow in that @c
 ** break and @c continue statements would change their meaning inside
 ** the list.
 **
 ** @see P99_PREFER
 ** @see P99_AVOID
 **/
#define P99_BLOCK(...) P99_PREFER(__VA_ARGS__) P99_NOP

/**
 ** @brief An exclusive @c case for a @c switch statement
 **
 ** This @c case will @em only be executed when the @c switch value is
 ** triggered and not if we fall through from a previous case.
 **
 ** @code
 ** switch(errno) {
 **  case 0: break; // everything works fine
 **  P99_XCASE EINTR : {
 **    fprintf(stderr, "Autsch: call to schnoeck was interrupted!\n");
 **    // do something else in that case
 **  }
 **  P99_XCASE ENOMEM :
 **    fprintf(stderr, "Autsch: call to schnoeck didn't have enough memory!\n");
 **  P99_XDEFAULT : {
 **    fprintf(stderr, "AUTSCH: call to schnoeck failed with unhandled case!\n");
 **    perror("AUTSCH");
 **  }
 **  // common clean up code comes here
 **  errno = 0;
 ** }
 ** @endcode
 **/
#define P99_XCASE P99_AVOID case

/**
 ** @brief The default case analogous to ::P99_XCASE
 **/
#define P99_XDEFAULT P99_AVOID default

typedef enum p00_uncase_enum {
  p00_uncase = 0,
} p00_uncase_enum;


#define P00_UNCASE switch((p00_uncase_enum)0) P99_XCASE 0

/**
 ** @brief A command prefixed with this cannot be a case target from
 ** surrounding scopes.
 **
 ** Actually this might not produce errors but just spoofy warnings,
 ** but well then you have been warned...
 **/
#define P99_UNCASE P00_UNCASE :

/**
 ** @brief Handle and reset @c errno.
 **
 ** This is will inspect @c errno (which is expensive) exactly once.
 ** If @c errno is @c 0, it will do nothing as efficient as
 ** possible. Otherwise it will execute the dependent block much as a
 ** @c switch statement:
 **
 ** @code
 ** P99_HANDLE_ERRNO {
 **   fprintf(stderr, "The compiler should tell us that this fprintf here is unreachable.\n");
 **   P99_XCASE EINTR : {
 **     fprintf(stderr, "Autsch: call to schnoeck was interrupted!\n");
 **     // do something else in that case
 **   }
 **   P99_XCASE ENOMEM :
 **       fprintf(stderr, "Autsch: call to schnoeck didn't have enough memory!\n");
 **   P99_XDEFAULT : {
 **       fprintf(stderr, "AUTSCH: call to schnoeck failed with unhandled case!\n");
 **       perror("AUTSCH");
 **     }
 **   fprintf(stderr, "We are doing some common cleanup for the errno handling code.\n");
 **   }
 ** @endcode
 **/
#define P99_HANDLE_ERRNO                                                \
for (_Bool p00_handle_errno = 1; p00_handle_errno; p00_handle_errno = 0) \
  for (int const p99_errno = errno; p00_handle_errno; p00_handle_errno = 0) \
    if (!p99_errno) { } else                                            \
      for (; p00_handle_errno; errno = 0, p00_handle_errno = 0)         \
        switch (p99_errno) case 0:


enum p99_unwind {
  /**
   ** @brief The level of nesting ::P99_UNWIND_PROTECT
   **
   ** This will always be accessible as read-only constant and taking
   ** an address of it will produce an error.
   **/
  p99_unwind_level = 0,
  p00_unwind_top = 0,
  p00_unwind_bottom = 0,
  /**
   ** @brief The code an eventual call to ::P99_UNWIND
   **
   ** This will be @c 0 if no ::P99_UNWIND is in progress. Otherwise a
   ** positive number tells how many levels will at most be unwound. A
   ** negative value will cause the whole ::P99_UNWIND_PROTECT level
   ** to traversed. This same negative value is visible on all levels
   ** through the execution of the protected parts.
   **
   ** This will always be accessible as read-only constant and taking
   ** an address of it will produce an error.
   **/
  p99_unwind_code = 0,
  p99_unwind_return = INT_MAX,
};


typedef struct p00_jmp_buf p00_jmp_buf;

struct p00_jmp_buf {
  bool volatile returning;
  jmp_buf buf;
};

#define P00_JMP_BUF_INITIALIZER { .returning = 0 }

/**
 ** @brief Unwind execution from several levels of nesting inside a
 ** function.
 **
 ** This macro allows to safely unwind several levels of block
 ** statements.
 **
 ** @code
 ** P99_UNWIND_PROTECT {
 **   // do something
 **   while (cond0) {
 **     for (;cond1;) {
 **        if (cond2) P99_UNWIND(-1);
 **     }
 **   }
 **  P99_PROTECT :
 **   // do some cleanup here
 **   // if everything went well ::p99_unwind_code has value 0 otherwise it
 **   // receives a value from P99_UNWIND
 ** }
 ** @endcode
 **
 ** Here ::P99_UNWIND terminates the execution of the inner blocks and
 ** resumes execution at the special "label" ::P99_PROTECT, if
 ** present.
 **
 ** The argument to ::P99_UNWIND controls how many levels are
 ** unwound. If it is positive at most that number of level is
 ** unwound, but never more levels than there are on the call stack of
 ** the enclosing function. If it is negative, all levels on the stack
 ** of the enclosing function are unwound and during that process this
 ** negative value that is passed to ::P99_UNWIND is accessible
 ** through ::p99_unwind_code.
 **
 ** @warning Variables that are modified before the call to
 ** ::P99_UNWIND are only guaranteed to have their new value in the
 ** protected part if they are declared @c volatile.
 **
 ** @see "test-p99-block.c" for a more sophisticated example of nested
 ** ::P99_UNWIND_PROTECT.
 ** @see ::P99_UNWIND
 ** @see ::P99_UNWIND_RETURN for a replacement of @c return that
 **      respects the protected parts
 ** @see ::p99_unwind_code
 ** @see ::p99_unwind_level
 **/
#define P99_UNWIND_PROTECT                                              \
/* the control variable for the syntax */                               \
for (register _Bool p00 = 1; p00; p00 = 0)                              \
  for (auto p00_jmp_buf p00_unwind_return = P00_JMP_BUF_INITIALIZER;    \
       p00;                                                             \
       p00_unwind_return.returning ? longjmp(p00_unwind_return.buf, 1) : P99_NOP) \
    for (register p00_jmp_buf *const p00_unwind_bottom0 = p00_unwind_bottom ? p00_unwind_bottom : &p00_unwind_return; p00;) \
      for (register p00_jmp_buf *const p00_unwind_bottom = p00_unwind_bottom0; p00_unwind_bottom && p00;) \
        /* are we unwinding or not? */                                  \
        for (register _Bool p00_unw = 0; p00;)                          \
          /* The return code from the longjmp to which we apply the     \
             special convention concerning the value 0. */              \
          for (register int p00_code = 0;                               \
               p00;                                                     \
               /* An eventual continuation of the unwind process is     \
                  decided here, since here the p00_unwind_top variable  \
                  that is visible is that of the enclosing scope, but   \
                  the unwind code variable is ours.  If the enclosing   \
                  scope is the outer scope, p00_unwind_top is a         \
                  integer with value zero. So even then the P99_UNWIND  \
                  is syntactically correct, but fortunately the         \
                  underlying call to longjmp will not be issued. */     \
               (p00_unw                                                 \
                ? P99_UNWIND(p00_code < 0 ? p00_code : p00_code - 1)    \
                : P99_NOP))                                             \
            /* maintain the level of nesting of different calls to      \
               P99_UNWIND_PROTECT */                                    \
            for (register unsigned const p00_level = p99_unwind_level + 1; p00;) \
              for (register unsigned const p99_unwind_level = p00_level; p99_unwind_level && p00;) \
                /* the buffer variable for setjmp/longjump */           \
                for (auto jmp_buf p00_unwind_top; p00;)                 \
                  for (p00_code = setjmp(p00_unwind_top); p00;)         \
                    /* detect whether or not we are unwinding */        \
                    for (p00_unw = !!p00_code; p00;)                    \
                      for (register int const p99_unwind_code = p00_code; p00; (void)p99_unwind_code) \
                        for (;p00; p00 = 0)                             \
                          /* dispatch. cast the _Bool to int since      \
                             this is what happens anyhow and some       \
                             compilers will issue strange warnings. */  \
                          switch ((int)p00_unw) case 0:

p99_inline
void p00_unwind(void* top, unsigned level, int cond) {
  if (level && cond && top) longjmp(top, cond);
}

/**
 ** @brief Preliminary resume from one or several levels of nested
 ** ::P99_UNWIND_PROTECT
 **
 ** @param x If this is evaluates to @c 0 nothing is done. If the
 ** result is positive as most as many levels of ::P99_UNWIND_PROTECT
 ** will be unwound. Otherwise all levels on the stack will be unwound
 ** and the control variables ::p99_unwind_code of these will be set
 ** to the value of @a x.
 **/
/* This uses a very special trick, such that this a valid call to
   longjmp at the end. On the lowest level p00_unwind_top is an
   integer of value 0 that converts to a void* that can be passed to
   longjmp as a formal parameter just such that the syntax is
   correct. On higher recursion levels this will be the variable of
   type jmp_buf that sits on the stack. jmp_buf is guaranteed to be an
   array type, so the expression here evaluates to a pointer that then
   is passed to the function. */
#define P99_UNWIND(x) p00_unwind(p00_unwind_top, p99_unwind_level, (x))

/**
 ** @brief Return from the enclosing function after unwinding all
 ** levels of nested ::P99_UNWIND_PROTECT.
 **
 ** By this you can guarantee that all eventually existing
 ** ::P99_PROTECT parts of enclosing ::P99_UNWIND_PROTECT are properly
 ** executed. This is in some similar to the guarantee of C++ to call
 ** destructors before returning from a function.
 **
 ** There is one important difference, though: the @c return
 ** expression (if any) is evaluated @em after the protected parts are
 ** unwound. So if these modify parts of that @c return expression you
 ** might not get what you expect. So instead of
 **
 ** @code
 ** P99_UNWIND_RETURN expr;
 ** @endcode
 **
 ** You might want do something like
 ** @code
 ** volatile retType myret = expr;
 ** P99_UNWIND_RETURN myret;
 ** @endcode
 **/
#define P99_UNWIND_RETURN                               \
if (p00_unwind_bottom                                   \
    && !setjmp(p00_unwind_bottom->buf)) {               \
  /* assign before we unwind all the way down */        \
  p00_unwind_bottom->returning = 1;                     \
  P99_UNWIND(-p99_unwind_return);                       \
 } else return

/**
 ** @brief The pseudo label to which we jump when we unwind the stack
 ** with ::P99_UNWIND.
 **
 ** Each ::P99_UNWIND_PROTECT may contain at most one such label.
 **
 ** @see P99_UNWIND_PROTECT
 ** @see ::P99_UNWIND
 ** @see ::p99_unwind_code
 ** @see ::p99_unwind_level
 **/
#define P99_PROTECT case 1 : P00_UNCASE

/**
 ** @brief Add some default documentation and links to the following
 ** block macro.
 **/
#define P99_BLOCK_DOCUMENT                                                                      \
/*! @warning Restrictions on preliminary exits from the dependent block or statement apply. **/ \
/*! @see P99_PROTECTED_BLOCK **/                                                                \
/*! @see P99_GUARDED_BLOCK **/

/**
 ** @brief Assert runtime condition @a EXPR on entry and on leave of the
 ** dependent block.
 ** @headerfile p99_c99.h "p99_c99.h"
 **/
P99_BLOCK_DOCUMENT
#define P99_INVARIANT(EXPR)                                             \
P99_PROTECTED_BLOCK(assert((EXPR) && "failed on entry"), assert((EXPR) && "failed on leave"))


/** @}
 **/

#endif 	    /* !P99_BLOCK_H_ */
