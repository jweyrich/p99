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

/**
 ** @brief A command prefixed with this cannot be a case target from
 ** surrounding scopes.
 **
 ** Actually this might not produce errors but just spoofy warnings,
 ** but well then you have been warned...
 **/
#define P99_UNCASE switch((p00_uncase_enum)0) P99_XCASE 0:

/**
 ** @brief Handle and reset @c errno.
 **
 ** This is will inspect @c errno (which is expesive) exactly once.
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
for (int p00_handle_errno = 0; p00_handle_errno < 1; ++p00_handle_errno) \
  for (int const p00_errno = errno; p00_handle_errno < 1; ++p00_handle_errno) \
    if (!p00_errno) { } else                                            \
      for (; p00_handle_errno < 1; errno = 0, ++p00_handle_errno)       \
        switch (p00_errno) case 0:



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
