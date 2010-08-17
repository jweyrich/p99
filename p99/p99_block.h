/*
** p99_block.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Jul 14 10:07:54 2010 Jens Gustedt
** Last update Wed Jul 14 10:07:54 2010 Jens Gustedt
*/

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
 ** that is executed afterwards.
 **
 ** Preliminary exit of the block is possible with @c break.
 **
 ** @warning @c continue and @c return inside the dependent block will
 ** not execute @a AFTER, so be careful.
 **
 ** An example of a potential use is
 ** @code
 ** #define INVARIANT(EXPR) P99_PROTECTED_BLOCK(assert((EXPR) && "failed on entry"), assert((EXPR) && "failed on leave"))
 ** @endcode
 **
 ** Such a construct may then be used like this
 ** @code
 ** INVARIANT(x > 5) {
 **   // do something with x that changes it but ensures that
 **   // it will be strictly less than 5 at the end
 ** }
 ** @endcode
 **
 ** and this would trigger an assertion whenever the condition is not
 ** fulfilled when entering or leaving the block.
 **/
#define P99_PROTECTED_BLOCK(BEFORE, AFTER)                              \
for (int _one1_ = 1;                                                    \
     /* be sure to execute BEFORE only at the first evaluation */       \
     (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                      \
     /* run AFTER exactly once */                                       \
     ((void)(AFTER), _one1_ = 0))                                       \
  /* Ensure that a `break' will still execute AFTER */                  \
  for (; _one1_; _one1_ = 0)

/**
 ** @brief A meta-macro to protect a dependent block or statement by
 ** a guard variable @a NAME of type @a T.
 **
 ** The guard variable is initialized with expression @a INITIAL and
 ** statement @a BEFORE and @a AFTER are executed before and after the
 ** block, respectively.
 **
 ** Preliminary exit of the block is possible with @c break.
 **
 ** @warning @c continue and @c return inside the dependent block will
 ** not execute @a AFTER, so be careful.
 **
 ** An example of a potential use would be a pointer to a mutex
 ** variable. It can be initialized with the address of a mutex as
 ** argument to @a INITIAL. @a BEFORE and @a AFTER may then lock resp
 ** unlock that mutex.
 **/
#define P99_GUARDED_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)              \
for (int _one1_ = 1; _one1_; _one1_ = 0)                                \
  for (T NAME = (INITIAL);                                              \
       /* be sure to execute BEFORE only at the first evaluation */     \
       (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                    \
       /* run AFTER exactly once */                                     \
       ((void)(AFTER), _one1_ = 0))                                     \
    /* Ensure that a `break' will still execute AFTER */                \
    for (; _one1_; _one1_ = 0)


/**
 ** @brief Add some default documentation and links to the following
 ** block macro.
 **/
#define P99_BLOCK_DOCUMENT                                              \
/*! @warning Restrictions on preliminary exits from the dependent block or statement apply. **/ \
/*! @see P99_PROTECTED_BLOCK **/                                        \
/*! @see P99_GUARDED_BLOCK **/

/** @}
 **/

#endif 	    /* !P99_BLOCK_H_ */
