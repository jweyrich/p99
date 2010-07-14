/*
** p99_defarg.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Tue Jul 13 22:50:40 2010 Jens Gustedt
** Last update Tue Jul 13 22:50:40 2010 Jens Gustedt
*/

#ifndef   	P99_DEFARG_H_
# define   	P99_DEFARG_H_

#include "p99_map.h"

/**
 ** @addtogroup default_arguments Default arguments for functions
 **
 ** @brief As a C++ like feature, this series of macros can be used to
 ** provide default arguments to functions.
 **
 ** This goes in several parts.
 **
 **   - Use ::P99_PROTOTYPE to define a prototype for your function.
 **   - define a macro that just calls ::P99_CALL_DEFARG as a
 **       replacement for your function
 **   - declare the default arguments with ::P99_DECLARE_DEFARG
 **   - generate the necessary external symbols with
 **       ::P99_DEFINE_DEFARG 
 ** @{
 **/


#ifdef DOXYGEN
# define P99_CALL_DEFARG(NAME, M, ...) NAME(__VA_ARGS__)
#else
# define P99_CALL_DEFARG(NAME, M, ...)                          \
NAME(P99_IF_EQ(0,M)                                             \
     (__VA_ARGS__)                                              \
     (P99_IF_EMPTY(__VA_ARGS__)                                 \
      (P99__DEFARGS(NAME, M, P99_PASTE2(NAME,_defarg_0)()))     \
      (P99__DEFARGS(NAME, M, __VA_ARGS__))                      \
      )                                                         \
     )
#endif


/**
 ** @def P99_CALL_DEFARG
 ** @brief Define a replacement macro for functions that can provide
 ** default arguments to the underlying real function.
 **
 ** This macro may be used for `overloading' system functions or
 ** functions that you define by your own. The easiest is to explain
 ** this with an example. The system function @c pthread_mutex_init is
 ** defined as follows:
 **
 ** @code
 ** int pthread_mutex_init(pthread_mutex_t *mut, pthread_mutexattr_t* attr);
 ** @endcode
 **
 ** Here the second argument is used to eventually specify an
 ** `attribute' to the mutex @c mut. Most people don't use that
 ** functionality and therefore @c pthread_mutex_init accepts @c NULL
 ** as a replacement for @c attr. This might be annoying since the
 ** focus of the syntax is on the exception than on the main use: the
 ** programmer always has to remember this particular special case and
 ** give explicit @c NULL's.
 **
 ** The following lines heal this.
 **
 ** @code
 ** P99_PROTOTYPE(int, pthread_mutex_init, pthread_mutex_t*, pthread_mutexattr_t const*);
 ** #define pthread_mutex_init(...) P99_CALL_DEFARG(pthread_mutex_init, 2, __VA_ARGS__)
 ** P99_DECLARE_DEFARG(pthread_mutex_init, , NULL);
 ** @endcode
 **
 ** This declares a macro @c pthread_mutex_init that resolves to the call of
 ** a real function to initialize a @c pthread_mutexattr_t*. If invoked
 ** with two arguments or more, the macro expansion results just in
 ** the usual call to the function.
 **
 ** If this initialization value for argument 1 is omitted (arguments
 ** count from 0) the default value of @c NULL is used. Valid use is
 **
 ** @code
 ** static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
 ** static pthread_mutexattr_t attr;
 ** .
 ** pthread_mutex_init(&mut, &attr); // full specification with attributes
 ** pthread_mutex_init(&mut, NULL);  // still a valid form
 **
 ** // easy variant for everyday use, equivalent to the previous one
 ** pthread_mutex_init(&mut);
 **
 ** // Also function pointers can still be taken.
 ** int (*myFunc)(pthread_mutex_t *, pthread_mutexattr_t*) = pthread_mutex_init;
 ** @endcode
 ** @param NAME is the function to provide with default argument features.
 ** @param M is the number of arguments that a full call to @a NAME takes.
 ** @see P99_DECLARE_DEFARG
 ** @see P99_PROTOTYPE
 **
 ** This macro is more flexible than the corresponding C++ feature of
 ** default arguments. It also lets you omit middle arguments.
 **
 ** More technically, for arguments that are omitted this just
 ** requires that NAME_defarg_M is defined for function @a NAME
 ** and @a M and that it is callable without arguments. This may just
 ** be a function (as implicitly defined by #P99_DECLARE_DEFARG) or a
 ** macro. For the first case everything the function refers to must
 ** be declare at the point of its definition. For the second case,
 ** the macro is evaluate at the place of the call and could refer to
 ** local variables or anything you like.
 **/

/**
 ** @def P99_PROTOTYPE(RT, NAME, ...)
 ** @brief Define the prototype of function @a NAME.
 **
 ** @a RT is the return type of the function, and the remaining
 ** arguments list the types of the arguments. This is needed by
 ** #P99_DECLARE_DEFARG to determine the type of the functions that return
 ** default arguments.
 **
 ** @warning the list should only contain types and should not give
 ** names to the individual parameters.
 **/

#ifdef DOXYGEN
#define P99__PROTOTYPE(RT, NAME, ...)                                       \
/*! @remark This function might be hidden behind a macro :: ## NAME of the same name. */ \
RT NAME(__VA_ARGS__)
#define P99_PROTOTYPE(RT, NAME, ...) P99__PROTOTYPE(__VA_ARGS__)
#else
#define P99__PROTOTYPE(RT, NAME, ...)                           \
  RT NAME(P99_IF_EMPTY(__VA_ARGS__)(void)(__VA_ARGS__));        \
  typedef RT P99_CAT2(NAME, _prototype_ret);                    \
  P99_TYPEDEFS(P99_CAT2(NAME, _prototype_), __VA_ARGS__)

#define P99_PROTOTYPE(...)                          \
P99_IF_EQ_2(P99_NARG(__VA_ARGS__))              \
(P99__PROTOTYPE(__VA_ARGS__, void))             \
(P99__PROTOTYPE(__VA_ARGS__))
#endif


#define P99__EXPR_FUNCTION(NAME, X, N)                                  \
P99_IF_EMPTY(X)                                                         \
()                                                                      \
(                                                                       \
 inline                                                                 \
 P99_PASTE3(NAME, _prototype_, N) P99_PASTE3(NAME, _defarg_, N)(void) { \
   P99_PASTE3(NAME, _prototype_, N) ret = (X);                          \
   return ret;                                                          \
 }                                                                      \
)

#define P99__DAFE(NAME, X, N)                                           \
P99_IF_EMPTY(X)                                                         \
(enum { P99_PASTE3(NAME, _boring_, N) })                                \
(P99_PASTE3(NAME, _prototype_, N) P99_PASTE3(NAME, _defarg_, N)(void))

#define P99__DECLARE_DEFARG(NAME, N, ...)                       \
  P99_FOR(NAME, N, P99__SER, P99__EXPR_FUNCTION, __VA_ARGS__)       \
enum { P99_PASTE3(p99__, NAME, _defarg_dummy_enum_val_) }

#ifdef DOXYGEN
/**
 ** @brief Provide default arguments for macro @a NAME
 **
 ** Each element in the list must correspond to an expression that can
 ** be evaluated in the outer scope, just where this call is placed.
 ** In many cases this will be constant expressions such as @c 0 or @c
 ** NULL, but they must not necessarily be so.
 **
 ** An empty argument, i.e nothing but an eventual comment, produces
 ** nothing. So no default argument will be provided for the
 ** corresponding position in the parameter list of @a NAME.
 **
 ** @see P99_PROTOTYPE on how to declare a prototype of a function
 ** @a NAME that can be used with this
 **
 ** @see P99_CALL_DEFARG on how to declare the macro @a NAME
 **
 ** @see rand48_t_init for a more sophisticated example with
 ** non-constant expressions.
 **/
#define P99_DECLARE_DEFARG(NAME, ...)
#else
#define P99_DECLARE_DEFARG(NAME, ...) P99__DECLARE_DEFARG(NAME, P99_NARG(__VA_ARGS__), __VA_ARGS__)
#endif

#define P99__DEFINE_DEFARG(NAME, N, ...)                \
  P99_FOR(NAME, N, P99__SEP, P99__DAFE, __VA_ARGS__)

#ifdef DOXYGEN
/**
 ** @brief Define the symbols that are declared through a
 ** corresponding call ::P99_DECLARE_DEFARG.
 **
 ** The argument list here should be exactly the same as for ::P99_DECLARE_DEFARG.
 **/
#define P99_DEFINE_DEFARG(NAME, ...)
#else
#define P99_DEFINE_DEFARG(NAME, ...) P99__DEFINE_DEFARG(NAME, P99_NARG(__VA_ARGS__), __VA_ARGS__)
#endif


#define P99__DARG(NAME, X, N) P99_IF_EMPTY(X)(P99_PASTE3(NAME, _defarg_, N)())(X)
#define P99___DEFARGS(NAME, N, ...) P99_FOR(NAME, N, P99__SEQ, P99__DARG, __VA_ARGS__)
#define P99__DEFARGS(NAME, N, ...) P99___DEFARGS(NAME, N, P99_IF_DEC_LT(P99_NARG(__VA_ARGS__),N) (__VA_ARGS__, P99_REPS(,P99_DEC_MINUS(N,P99_NARG(__VA_ARGS__)))) (__VA_ARGS__))


/**
 ** @brief Provide a documentation section to a function defined with ::P99_CALL_DEFARG
 **/
#define P99_DEFARG_DOCU(NAME)                                       \
/*! @see P99_CALL_DEFARG */                                          \
/*! @see P99_DECLARE_DEFARG */                                              \
/*! @see NAME This is actually implemented as a macro that helps to provide default arguments to the real function. */



/** @}
 **/


#endif 	    /* !P99_DEFARG_H_ */
