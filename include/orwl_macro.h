/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	ORWL_MACRO_H_
# define   	ORWL_MACRO_H_

#include "orwl_inline.h"
#include "orwl_default_arg.h"

/* This was inspired by BOOT's PP_CAT macro. Using such a thing avoid
   to define multiple levels of expansion for each macro. */
#define PASTE0()
#define PASTE1(_1) _1
#define PASTE2(_1, _2) _1 ## _2
#define _PASTE2(_1, _2) PASTE2(_1, _2)

#define __PASTE(F, N, ...) F ## N(__VA_ARGS__)
#define _PASTE(N, ...) __PASTE(PASTE, N, __VA_ARGS__)

/**
 ** @brief A left-to-right associative paste operator.
 **
 ** This macro avoids the ambiguity of the @c ## preprocessor operator
 ** which has no well defined associativity. With this macro here
 ** something like
 ** @code
 ** PASTE(0.1E, -, 1)
 ** @endcode
 ** is guaranteed to produce the token @c 0.1E-1, whereas the
 ** seemingly equivalent
 ** @code
 ** ETSAP(0.1E, -, 1)
 ** @endcode
 ** is not valid: the intermediate operation to paste tokens `-' and
 ** `1' would result in an invalid token and is thus rejected.
 **/
#define PASTE(...) _PASTE(_NARG(__VA_ARGS__), __VA_ARGS__)


#define ETSAP0()
#define ETSAP1(_1) _1
#define ETSAP2(_2, _1) _2 ## _1
#define _ETSAP2(_2, _1) ETSAP2(_2, _1)
#define ETSAP3(_3, _2, _1) _ETSAP2(_3, ETSAP2(_2, _1))
#define ETSAP4(_4, _3, _2, _1) _ETSAP2(_4, ETSAP3(_3, _2, _1))
#define ETSAP5(_5, _4, _3, _2, _1) _ETSAP2(_5, ETSAP4(_4, _3, _2, _1))
#define ETSAP6(_6, _5, _4, _3, _2, _1) _ETSAP2(_6, ETSAP5(_5, _4, _3, _2, _1))
#define ETSAP7(_7, _6, _5, _4, _3, _2, _1) _ETSAP2(_7, ETSAP6(_6, _5, _4, _3, _2, _1))

#define __ETSAP(F, N, ...) F ## N(__VA_ARGS__)
#define _ETSAP(N, ...) __ETSAP(ETSAP, N, __VA_ARGS__)

/**
 ** @brief A right-to-left associative paste operator.
 **
 ** @see PASTE
 **/
#define ETSAP(...) _ETSAP(_NARG(__VA_ARGS__), __VA_ARGS__)

#define _DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP, ...)   \
  IF_EMPTY(SIGN)(+)(SIGN)_SKIP_ PASTE(                  \
  IF_EMPTY(INT)(0)(INT),                                \
  .,                                                    \
  IF_EMPTY(FRAC)(0)(FRAC),                              \
  E,                                                    \
  IF_EMPTY(ESIGN)(+)(ESIGN),                            \
  IF_EMPTY(EXP)(0)(EXP),                                \
  __VA_ARGS__)

#define DEC_DOUBLE(...)                         \
  IF_DEC_GE(NARG(__VA_ARGS__), 6)               \
  (_DEC_DOUBLE(__VA_ARGS__))                    \
  (_DEC_DOUBLE(__VA_ARGS__,,,,,))


#define _HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP, ...)  \
  IF_EMPTY(SIGN)(+)(SIGN)_SKIP_ PASTE(                          \
  0x,                                                           \
  IF_EMPTY(HEXINT)(0)(HEXINT),                                  \
  .,                                                            \
  IF_EMPTY(HEXFRAC)(0)(HEXFRAC),                                \
  P,                                                            \
  IF_EMPTY(ESIGN)(+)(ESIGN),                                    \
  IF_EMPTY(BINEXP)(0)(BINEXP),                                  \
  __VA_ARGS__)

#define HEX_DOUBLE(...)                         \
  IF_DEC_GE(NARG(__VA_ARGS__), 6)               \
  (_HEX_DOUBLE(__VA_ARGS__))                    \
  (_HEX_DOUBLE(__VA_ARGS__,,,,,))


#define _INV(N) PASTE(_variable_argument_list_must_be_divisible_by_, N)

/**
 ** @brief Return the length of the variate argument list.
 **
 ** This supposes that the length is less than 64.
 **
 ** Idea is due to Laurent Deniau, CERN, and taken from a discussion
 ** in comp.lang.c.
 **
 ** @warning this also counts an empty argument list as having one (=
 ** the empty) argument
 ** @see NARG for a macro that returns 0 if the list is empty
 **/
#define _NARG(...) _NARG_1(__VA_ARGS__)

#define _IGNORE(...)
#define _IDENT(...) __VA_ARGS__
#define _SKIP_
#define _CLAUSE1(...) __VA_ARGS__ _IGNORE
#define _CLAUSE2(...) _IDENT

#define _IS__EQ__(...) ,
#define _IS_0_EQ_0(...) ,
#define _IS_1_EQ_1(...) ,
#define _IS_2_EQ_2(...) ,
#define _IS_3_EQ_3(...) ,
#define _IS_4_EQ_4(...) ,
#define _IS_5_EQ_5(...) ,
#define _IS_6_EQ_6(...) ,
#define _IS_7_EQ_7(...) ,
#define _IS_8_EQ_8(...) ,
#define _IS_9_EQ_9(...) ,
#define _IS_00_EQ_00(...) ,
#define _IS_10_EQ_10(...) ,
#define _IS_01_EQ_01(...) ,
#define _IS_11_EQ_11(...) ,
#define __IF_CLAUSE(A,B,C,...) C
#define _IF_CLAUSE(EXP) __IF_CLAUSE(EXP, _CLAUSE1, _CLAUSE2, ~)
#define _IF_NOT_CLAUSE(EXP) __IF_CLAUSE(EXP, _CLAUSE2, _CLAUSE1, ~)

/**
 ** @brief Test two words @a A and @a B if they are equal.
 **
 ** @a A and @a B must be just one word, i.e composed of
 ** alpha-numerical characters and underscores.
 **
 ** For such a test to work properly a corresponding macro @c _IS_A_EQ_A
 ** must exist for all @a A such that @a B may be the same word as @a
 ** A. E.g for the test
 **
 ** @code
 ** IF_EQ(0, X)(expand_when_equal)(expand_when_unequal)
 ** @endcode
 **
 ** the macro ::_IF_0_EQ_0 must exist. (Which it does in that case).
 **
 ** @see IF_DEC_EQ for equality of not too large decimal numbers
 **/
#define IF_EQ(A, B) _IF_CLAUSE(PASTE(_IS_,A,_EQ_,B)())

/**
 ** @brief Test two words @a A and @a B if they are unequal.
 **
 ** @see IF_EQ 
 **/
#define IF_NE(A, B) _IF_NOT_CLAUSE(PASTE(_IS_,A,_EQ_,B)())

/**
 ** @brief Test two decimal numbers @a A and @a B if they are equal.
 **/
#define IF_DEC_EQ(A, B) IF_EQ(0,_dec_minus(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if they are unequal.
 **/
#define IF_DEC_NE(A, B) IF_NE(0,_dec_minus(A,B))

#define IF_GE_0(A) _IF_CLAUSE(PASTE(_IS_,A,_GE_,0)())
#define IF_LT_0(A) _IF_NOT_CLAUSE(PASTE(_IS_,A,_GE_,0)())

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is greater
 ** or equal to @a B.
 **/
#define IF_DEC_GE(A, B) IF_GE_0(_dec_minus(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is less
 ** or equal to @a B.
 **/
#define IF_DEC_LE(A, B) IF_GE_0(_dec_minus(B,A))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** less than @a B.
 **/
#define IF_DEC_LT(A, B) IF_LT_0(_dec_minus(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** greater than @a B.
 **/
#define IF_DEC_GT(A, B) IF_LT_0(_dec_minus(B,A))

/**
 ** @brief Test if token N is the token 0.
 **/
#define IF_EQ_0(N) IF_EQ(0, N)
#define IS_EQ_0(N) IF_EQ(0, N)(1)(0)

/**
 ** @brief Test if token N is the token 1.
 **/
#define IF_EQ_1(N) IF_EQ(1, N)
#define IS_EQ_1(N) IF_EQ(1, N)(1)(0)

/**
 ** @brief Test if token N is the token 2.
 **/
#define IF_EQ_2(N) IF_EQ(2, N)
#define IS_EQ_2(N) IF_EQ(2, N)(1)(0)

/**
 ** @brief Test if token N is the token 3.
 **/
#define IF_EQ_3(N) IF_EQ(3, N)
#define IS_EQ_3(N) IF_EQ(3, N)(1)(0)

/**
 ** @brief Test if token N is the token 4.
 **/
#define IF_EQ_4(N) IF_EQ(4, N)
#define IS_EQ_4(N) IF_EQ(4, N)(1)(0)

/**
 ** @brief Test if the argument list is empty.
 **
 ** This expands to token 1 if the list was empty and to token 0 if
 ** there was anything different from a comment in the list.
 **
 ** The implementation of this macro is kind of tricky and heavily
 ** uses the fact that a function macro (@c _IS__EQ__ in this case)
 ** is left untouched if it is not followed by a parenthesis. By that
 ** the inner part of the macro will expand to a non-empty sequence
 ** with a different number of commas:
 **
 ** <DL>
 **  <DT>0</DT>
 **  <DD>if there was an non empty argument without commas and not
 **       starting with a parenthesis</DD>
 **  <DT>1</DT> <DD>if the argument was empty</DD>
 **  <DT>2</DT> <DD>if the argument had no comma but started with a parenthesis</DD>
 **  <DT>2N</DT> <DD>if the argument had N commas</DD>
 ** </DL>
 **
 ** So the case of exactly one comma is what we are looking for. For
 ** that case, ::_NARG returns the value 2, and then we may just
 ** test for the token 2.
 **
 ** @return tokens 0 or 1
 **/
#define IS_EMPTY(...) IS_EQ_2(_NARG(_IS__EQ__ __VA_ARGS__ (~) _IS__EQ__ __VA_ARGS__))

#define IF_EMPTY(...) IF_EQ_1(IS_EMPTY(__VA_ARGS__))

/**
 ** @brief Do a evaluation of the argument.
 **
 ** The result is the token 0 (seen here as `false') if the argument
 ** was empty or if it evaluated to the token 0. Otherwise returns the
 ** token 1.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_EVAL(A) IF_EMPTY(A)(0)(IF_EQ_0(A)(0)(1))


/**
 ** @brief Do a logical negation of the argument.
 **
 ** The result is the token 1 (seen here as `true') if the argument
 ** was empty or if it evaluated to the token 0. Otherwise returns the
 ** token 0.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_NOT(A)  IF_EQ_0(A)(1)(IS_EMPTY(A))

/**
 ** @brief Do a logical exclusive or of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_XOR(A, B) IF_EQ(LOGIC_NOT(A), LOGIC_EVAL(B))(1)(0)
#define _LOGIC_OR(A, B) IF_EQ(00, PASTE2(A, B))(0)(1)

/**
 ** @brief Do a logical inclusive or of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_OR(A, B) _LOGIC_OR(LOGIC_EVAL(A), LOGIC_EVAL(B))

#define _LOGIC_AND(A, B) IF_EQ(00, PASTE2(A, B))(1)(0)

/**
 ** @brief Do a logical and of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_AND(A, B) _LOGIC_AND(LOGIC_NOT(A), LOGIC_NOT(B))


/**
 ** @brief A preprocessor control structure
 **
 ** Use this as follows
 ** @code
 ** IF_ELSE(some_expression)(tokens_A)(tokens_B)
 ** @endcode
 ** This expands to tokens_B if the list expanded to the token 0 and
 ** to tokens_A in any other case.
 **
 ** Observe the parenthesis around tokens_A and tokens_B.
 **/
#define IF_ELSE(...) IF_EQ_1(IS_EQ_0(__VA_ARGS__))


/**
 ** @brief Return the length of the variate argument list, an empty
 ** argument list accounting for 0 arguments.
 **
 ** This supposes that the length of the list is less than 64.
 **
 ** @see _NARG for a macro that accounts an empty list to be 1
 **/
#define NARG(...) IF_EMPTY(__VA_ARGS__)(0)(_NARG(__VA_ARGS__))

#define _IS_void_EQ_void(...) ,

/**
 ** @brief Test if the argument consists of exactly the word @c void.
 **
 ** @see IF_VOID for a macro that test whether or not its argument is
 ** empty @b or if it consists of the word @c void.
 **/
#define IS_void(...)                                                    \
IS_EQ_2(_NARG(_IS_void_EQ_ ## __VA_ARGS__ (~) _IS_void_EQ_ ## __VA_ARGS__))

#define IF_void(...) IF_EQ_1(IS_void(__VA_ARGS__))


/**
 ** @brief Test whether or not its argument is empty @b or if it
 ** consists of the word @c void.
 **
 ** @see IF_void for a macro that tests if the argument is exactly the
 ** word @c void.
 **/
#define IS_VOID(...) IF_EMPTY(__VA_ARGS__)(1)(IS_void(__VA_ARGS__))

#define IF_VOID(...) IF_EQ_1(IS_VOID(__VA_ARGS__))

#define IS_COMMA(...)                                                   \
IF_EQ_2(NARG(__VA_ARGS__))                                              \
(LOGIC_AND(IS_EMPTY(CHS(0,__VA_ARGS__)),IS_EMPTY(CHS(1,__VA_ARGS__))))  \
(0)

#define _MODARG_(_X) PASTE(_NARG_,  _X)

/**
 ** @def LEN_MODARG
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It replaces @a X by
 ** the number of X-tuples in the following list of arguments.
 ** @see VA_ARGS
 **/

/**
 ** @def LEN_ARG
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It prefixes the
 ** list of arguments by an integer constant containing the length of
 ** the list.
 ** @see VA_ARGS
 **/


/**
 ** @brief Helper macro to declare a variable length parameter list.
 **
 ** Inside the declared function @a X will of @c size_t and should
 ** hold the actual length of the list. It can be used as the argument
 ** to @c va_start.
 **
 ** Wrap your function into a macro that uses LEN_ARG. If used through
 ** that macro, the correct value for @a X will always be provided at
 ** compile time. Declare such a function as this:
 ** @code
 ** unsigned FSYMB(toto)(unsigned a, VA_ARGS(number));
 ** #define toto(A, ...) FSYMB(toto)(A, LEN_ARG(toto, __VA_ARGS__))
 ** VA_TYPES(toto, unsigned);
 ** @endcode
 **
 ** In the definition of the function you then may use the @c va_start
 ** etc from stdarg.h to tread the argument list.
 ** @code
 ** unsigned FSYMB(toto)((unsigned a, VA_ARGS(number)) {
 **     unsigned ret = 0;
 **     va_list ap;
 **     va_start(ap, number);
 **     for (size_t i = 0; i < number; ++i) {
 **       ret += VA_MODARG(ap, toto);
 **     }
 **     va_end(ap);
 **     return ret % a;
 ** }
 ** @endcode
 ** In this toy example @c toto can be used as
 ** @code
 ** unsigned magic = toto(3, 1, 3, 5, 7);
 ** @endcode
 ** which will result in converting 1, 3, 5, 7 (the variable
 ** arguments) to @c unsigned, computing their sum, i.e 16u, and
 ** compute that value mod 3u (the fixed argument @a a). So @a magic
 ** should hold the value 1u thereafter.
 **
 ** @param X is the name of the `length' parameter that you want to
 ** use in the definition of the function. As in the example above it
 ** should be then used as the second argument to @c va_start and as a
 ** loop boudary when you actual handle the argument list. @a X is
 ** implicitly declared to have type @c size_t.
 **
 ** @see LEN_ARG
 ** @see LEN_MODARG
 ** @see VA_TYPES
 ** @see FSYMB
 **/
#define VA_ARGS(X) size_t X, ...

#define VA_ARGS_DOCUMENTATION(NAME)                                     \
                                                                        \
/*! @see VA_ARGS */                                                     \
/*! This is actually implemented as a macro that helps to provide the length of the variable length argument list to the function. */


#define _hex2dec_(HEX) PASTE(_hex2dec_, HEX)
#define _dec2hex_(DEC) PASTE(_dec2hex_, DEC)
#define _predecessor(N) PASTE(_predecessor_, N)
#define _itpredecessor_0(DEC) DEC
#define _dec2uni(DEC) PASTE(_dec2uni_, DEC)
#define _uni2dec(UN) ETSAP(_uni2, dec_, UN)
#define _uni_add(U,V) PASTE(U, V)

#define ____dec_add(U,V) _uni_add(U,V)
#define ___dec_add(D,E) ____dec_add(_dec2uni(D),_dec2uni(E))
#define __dec_add(D,E) _uni2dec(___dec_add(D,E))
#define _dec_add(D,E) __dec_add(D,E)

#define _predecessor_0 minus_1
#define _dec_eval(EDEC) PASTE(_dec_eval_, EDEC)
#define _dec_minus(D,E) PASTE(_itpredecessor_, E)(D)


#define _FSYMB(NAME) PASTE(NAME, _f, sy, mb, _)

/** @brief Mangle @a NAME 
 **
 ** This should only be used in declaration and definition of the
 ** function that is hidden behind the macro @a NAME.
 **/
#define FSYMB(NAME) _FSYMB(NAME)

/**
 ** @brief Provide a documentation section to a function defined with ::CALL_WITH_DEFAULTS
 **/
#define FSYMB_DOCUMENTATION(NAME)                                       \
/*! @see CALL_WITH_DEFAULTS */                                          \
/*! @see DECLARE_DEFARG */                                              \
/*! This is actually implemented as a macro that helps to provide default arguments to the real function. */

#define _wda_0(NAME, ...) __VA_ARGS__
#define ____call_wda(NAME, K, ...) PASTE(_wda_, K)(NAME, __VA_ARGS__)
#define ___call_wda(NAME, K, ...) ____call_wda(NAME, K, __VA_ARGS__)
#define __call_wda(NAME, M, N, ...) ___call_wda(NAME, _dec_minus(M, N), __VA_ARGS__)
#define _call_wda(NAME, M, ...) __call_wda(NAME, M, NARG(__VA_ARGS__), __VA_ARGS__)

#define _CALL_WITH_ALL_DEFAULTS(NAME, M) PASTE(_wda_, M)(NAME, PASTE(NAME,_defarg_0)())

# define CALL_WITH_DEFAULTS(NAME, M, ...)               \
NAME(IF_EMPTY(__VA_ARGS__)                              \
     (_CALL_WITH_ALL_DEFAULTS(NAME, _predecessor(M)))   \
     (_call_wda(NAME, M, __VA_ARGS__))                  \
     )

/**
 ** @def CALL_WITH_DEFAULTS
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
 ** The following two lines heal this.
 **
 ** @code
 ** #define pthread_mutex_init(...) CALL_WITH_DEFAULTS(pthread_mutex_init, 2, __VA_ARGS__)
 ** declare_defarg(pthread_mutex_init, 1, pthread_mutexattr_t*, NULL);
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
 ** @see declare_defarg
 **/


/**
 ** @define expand to a comma token
 **
 ** This sometimes needed for trick macros such as
 ** ::CALL_WITH_DEFAULTS_EVEN_EMPTY to ensure that a macro parameter
 ** inside a recursion is not empty.
 **/
#define _COMMA_ ,

/**
 ** @brief Define an (almost) all purpose initializer
 **/
#define INITIALIZER INIT1

#define INIT0 (0)
#define INIT1 { 0 }
#define INIT2 { { 0 } }

/**
 ** @brief Typed @c NULL
 **
 ** Define a @c NULL initialized constant of type @a T.
 **/
#define TNULL(T) NULL1(T)

#define NULL0(T) ((T)INIT0)
#define NULL1(T) ((T)INIT1)
#define NULL2(T) ((T)INIT2)

/**
 ** @brief Typed ones
 **
 ** Define a all-one-bits initialized constant of integer type @a T.
 **/
#define TONES(T) (~TNULL(T))

/**
 ** @brief Signedness of a type
 **
 ** Determine if @a T corresponds to a signed integer type or not.
 **/
#define ISSIGNED(T) (TONES(T) < TNULL(T))

/**
 ** @brief Typed max value
 **
 ** Define the largest value that integer type @a T may hold.
 **/
#define TMAX(T) (ISSIGNED(T) ? (TONES(T) >> 1) : TONES(T))

/**
 ** @brief Typed min value
 **
 ** Define the smallest value that integer type @a T may hold.
 **/
#define TMIN(T) (~TMAX(T))


/**
 ** @brief A meta-macro to protect a dependent block or statement by a
 ** statement that is executed before and one after.
 **
 ** Preliminary exit of the block is possible with @c break.
 **
 ** @warning @c continue and @c return inside the dependent block will
 ** not execute @a AFTER, so be careful.
 **/
#define BLOCK(BEFORE, AFTER)                                    \
for (int _one1_ = 1;                                            \
     /* be sure to execute BEFORE only at the first evaluation */       \
     (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);              \
     /* run AFTER exactly once */                               \
     ((void)(AFTER), _one1_ = 0))                               \
  /* Ensure that a `break' will still execute AFTER */          \
  for (; _one1_; _one1_ = 0)

#define SAVE_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)                     \
for (int _one1_ = 1; _one1_; _one1_ = 0)                                \
  for (T NAME = (INITIAL);                                              \
       /* be sure to execute BEFORE only at the first evaluation */     \
       (_one1_ ? ((void)(BEFORE), _one1_) : _one1_);                    \
       /* run AFTER exactly once */                                     \
       ((void)(AFTER), _one1_ = 0))                                     \
    /* Ensure that a `break' will still execute AFTER */                \
    for (; _one1_; _one1_ = 0)


#define DOCUMENT_BLOCK /*! @see BLOCK for restrictions on preliminary exits from the dependent block or statement. **/


#define ARRAYSIZE(A) (sizeof(A)/sizeof(A[0]))

#define ARRAY2SIZE(A) A, ARRAY2SIZE(A)

#define CHOOSE5(xT, cc, cs, ci, cl, cll)        \
((sizeof(xT) < sizeof(int))                     \
 ? ((sizeof(xT) < sizeof(short))                \
    ? cc                                        \
    : cs)                                       \
 : ((sizeof(xT) <= sizeof(long))                \
    ? ((sizeof(xT) == sizeof(int))              \
       ? ci                                     \
       : cl)                                    \
    : cll))

#define PRI(xT, F, ...)                         \
CHOOSE5(xT,                                     \
        "%" #__VA_ARGS__ "hh" #F,               \
        "%" #__VA_ARGS__ "h" #F,                \
        "%" #__VA_ARGS__ "" #F,                 \
        "%" #__VA_ARGS__ "l" #F,                \
        "%" #__VA_ARGS__ "ll" #F)

/**
 ** @brief Promote integer expression @a x to the width of @c
 ** uintmax_t but keep signedness if possible.
 **/
#define _J(x) (0 ? TNULL(uintmax_t) : (x))

/**
 ** @brief Promote integer expression @a x to the width of @c
 ** size_t but keep signedness if possible.
 **/
#define _Z(x) (0 ? TNULL(size_t) : (x))

#define _DOIT0(...)
#define _DOIT1(NAME, N, OP, FUNC, X, ...) FUNC(NAME, X, 0)

#define _IDT(NAME, X, N) X
#define _SOP(NAME, X, N) N
#define _CCA(NAME, X, N) X[N]
#define _IGN(NAME, X, N)
#define _STRLEN(NAME, X, N) strlen(X)

#define _SUM(NAME, X, N, REC) (X + REC)
#define _PROD(NAME, X, N, REC) (X * REC)
#define _SEQ(NAME, X, N, REC) X, REC
#define _SEP(NAME, X, N, REC) X; REC
#define _PES(NAME, X, N, REC) REC; X
#define _REV(NAME, X, N, REC) REC, X
#define _STRCAT(NAME, X, N, REC) strcat(X, REC)
#define _STRTAC(NAME, X, N, REC) _STRCAT(NAME, REC, N, X)

#define _SUMS(N, ...) PASTE(_DOIT, N)(,N, _SUM, _IDT, __VA_ARGS__,)

/**
 ** @brief Compute the right associative sum of all the arguments.
 **/
#define SUMS(...) _SUMS(NARG(__VA_ARGS__),__VA_ARGS__)

#define _STRLENS(N, ...) PASTE(_DOIT, N)(,N, _SUM, _STRLEN, __VA_ARGS__,)

/**
 ** @brief Return an expression that returns the sum of the lengths of
 ** all strings that are given as arguments.
 **/
#define STRLENS(...) _STRLENS(NARG(__VA_ARGS__),__VA_ARGS__)

#define _REVS(N, ...) PASTE(_DOIT, N)(,N, _REV, _IDT, __VA_ARGS__,)

/**
 ** @brief Revert the argument list
 **/
#define REVS(...) IF_DEC_LT(NARG(__VA_ARGS__),2)(__VA_ARGS__)(_REVS(NARG(__VA_ARGS__),__VA_ARGS__))

#define _STRCATS(N, ...) PASTE(_DOIT, N)(,N, _STRTAC, _IDT, __VA_ARGS__,)

/**
 ** @brief Append all argument strings after @a TARG to @a TARG.
 **
 ** @a TARG must be compatible with @c char* and provide enough space
 ** to hold the concatenation of all strings. The remaining arguments
 ** must be compatible with @c const char*.
 **/
#define STRCATS(TARG, ...) _STRCATS(NARG(TARG, __VA_ARGS__), REVS(TARG, __VA_ARGS__))

/**
 ** @brief Concatenate all arguments.
 **
 ** All arguments must be strings for which the length can be computed
 ** at compile time.
 **
 ** @return a temporary string that is valid in the containing block
 ** of the expression holding the call to this macro.
 **
 ** @see STRDUP for a variant that returns a @c malloc'ed string and
 ** thus can be called with any type of @c char* arguments.
 **/
#define JOIN(...) STRCATS((char[STRLENS(__VA_ARGS__) + 1]){ 0 }, __VA_ARGS__)

/**
 ** @brief Concatenate all arguments.
 **
 ** @return a string that must be freed by @c free
 **/
#define STRDUP(...) STRCATS(memset(malloc(STRLENS(__VA_ARGS__) + 1), 0, 1), __VA_ARGS__)

#define _DECLS(N, ...) PASTE(_DOIT, N)(,N, _SEP, _IDT, __VA_ARGS__,)

/**
 ** @brief Change the commas in the argument list into semicolons.
 **/
#define DECLS(...) _DECLS(NARG(__VA_ARGS__), __VA_ARGS__)

/**
 ** Repeat the parameter @a X @a N times.
 **/
#define REPS(X, N) PASTE(_DOIT, N)(, N, _SEQ, X _IGN,,)

/**
 ** @brief Produce a list of length @a N that has the contents of 0,
 ** 1, , @a N-1
 **/
#define POSS(N) REVS(PASTE(_DOIT, N)(,N, _SEQ, _SOP,,))

/**
 ** Produce a list of length @a N that has the contents of @a X[0], @a
 ** X [1], ,
 ** @a X[@a N-1]
 **/
#define ACCS(X, N) REVS(PASTE(_DOIT, N)(, N, _SEQ, _CCA, REPS(X, N),))

/**
 ** Cut the argument list at position @a N
 **/
#define SELS(N, ...) PASTE(_DOIT, N)(, N, _SEQ, _IDT, __VA_ARGS__,)

#define CHS(N, ...)                             \
IF_DEC_GE(N, NARG(__VA_ARGS__))                 \
()                                              \
(IF_DEC_LT(NARG(__VA_ARGS__), 2)                \
 (__VA_ARGS__)                                  \
 (SELS(1, REVS(SELS(N, __VA_ARGS__)))))

#define _ASG(NAME, X, N) _predecessor(N)] , X

#define _SAR(NAME, X, N, REC) X = REC

#define _ASGS(X, N, ...)                                                \
DECLS(                                                                  \
      SELS(_dec_minus(N,2),                                             \
           REVS(                                                        \
                SELS(_predecessor(N),                                   \
                     PASTE(_DOIT, N)(, N,                              \
                                      (X)[_SAR,                         \
                                        _ASG,                           \
                                        REVS(__VA_ARGS__),              \
                                        ] /* dummy closing bracket */   \
                                      )                                 \
                     )                                                  \
                )                                                       \
           )                                                            \
      )


/**
 ** @brief Produce a list of length @a N that has the contents of @c
 ** V0 = @a X[0], @c V1 @a = X [1], , @c VN-1 = @a X[@a N-1], where
 ** V0, etc are the remaining arguments.
 **/
#define ASGS(X, ...)                                    \
IF_DEC_LT(NARG(__VA_ARGS__),2)                          \
(IF_VOID(__VA_ARGS__)((void)0)(__VA_ARGS__ = (X)[0]))   \
(_ASGS(X, _NARG(,,__VA_ARGS__), ,,__VA_ARGS__))

#define _TYPD(NAME, X, N) typedef X PASTE(NAME, N)
#define _TYPN(NAME, X, N, REC) X, REC

#define _TYPEDEFS(NAME, N, ...)                                         \
  IF_VOID(__VA_ARGS__)                                  \
  (enum { PASTE3(NAME, _eat_the_semicolon_, N) })                         \
  (DECLS(REVS(PASTE(_DOIT, N)(NAME, N, _TYPN, _TYPD, REVS(__VA_ARGS__),))))

#define TYPEDEFS(NAME, ...)                             \
_TYPEDEFS(NAME, NARG(__VA_ARGS__), __VA_ARGS__)

#define _PROTOTYPE(RT, NAME, ...)                       \
  RT NAME(IF_EMPTY(__VA_ARGS__)(void)(__VA_ARGS__));    \
  typedef RT PASTE(NAME, _sigtype_ret);                \
  TYPEDEFS(PASTE(NAME, _sigtype_), __VA_ARGS__)

#define PROTOTYPE(...)                          \
IF_EQ_2(NARG(__VA_ARGS__))                      \
(_PROTOTYPE(__VA_ARGS__, void))                 \
(_PROTOTYPE(__VA_ARGS__))


#define _DAFD(NAME, X, N)                                       \
IF_EMPTY(X)                                                     \
()                                                              \
(                                                               \
 inline                                                         \
 PASTE3(NAME, _sigtype_, N) PASTE3(NAME, _defarg_, N)(void) {   \
   PASTE3(NAME, _sigtype_, N) ret = (X);                        \
   return ret;                                                  \
 }                                                              \
 )

#define _DAFE(NAME, X, N)                                       \
  IF_EMPTY(X)(enum { PASTE3(NAME, _boring_, N) })(PASTE3(NAME, _sigtype_, N) PASTE3(NAME, _defarg_, N)(void))

#define _DAFN(NAME, X, N, REC) X REC

#define _DECLARE_DEFARG(NAME, N, ...)                                   \
  DECLS(REVS(PASTE(_DOIT, N)(NAME, N, _DAFN, _DAFD, __VA_ARGS__,)))    \
enum { PASTE3(_, NAME, _defarg_dummy_enum_val_) }

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
 ** @see PROTOTYPE on how to declare a prototype of a function
 ** @a NAME that can be used with this
 **
 ** @see CALL_WITH_DEFAULTS on how to declare the macro @a NAME
 **
 ** @see rand48_t_init for a more sophisticated example with
 ** non-constant expressions.
 **/
#define DECLARE_DEFARG(NAME, ...) _DECLARE_DEFARG(NAME, NARG(__VA_ARGS__), REVS(__VA_ARGS__))

#define _DEFINE_DEFARG(NAME, N, ...)                                         \
  DECLS(REVS(PASTE(_DOIT, N)(NAME, N, _SEP, _DAFE, __VA_ARGS__,)))

/**
 ** @brief Define the symbols that are declared through a
 ** corresponding call ::DECLARE_DEFARG.
 **
 ** The argument list here should be exactly the same as for ::DECLARE_DEFARG.
 **/
#define DEFINE_DEFARG(NAME, ...) _DEFINE_DEFARG(NAME, NARG(__VA_ARGS__), REVS(__VA_ARGS__))

#define _EMP(B, X, N) B
#define _COU(B, X, N, REC) PASTE(B, REC)
#define _DEC_MUL(A, B) IF_EQ_0(A)(0)(_uni2dec(IF_EQ_1(A)(B)(PASTE(_DOIT, A)(B, A, _COU, _EMP, _ASCENDING(),))))
#define DEC_MUL(A, B) IF_DEC_LT(A, B)(_DEC_MUL(A, _dec2uni(B)))(_DEC_MUL(B, _dec2uni(A)))

#define _ExP(A, X, N) X
#define _MxL(A, X, N, REC) IF_DEC_LE(X, A)(N)(REC)
#define _DEC_X(N, A, B, ...) PASTE(_DOIT, N)(A, N, _MxL, _ExP, __VA_ARGS__,)


#define _ENP(B, X, N)
#define _MIL(B, X, N, REC) IF_DEC_GT(N, 1)(DEC_MUL(N, B))(B), REC
#define _DEC_DIV(N, A, B) PASTE(_DOIT, N)(B, N, _MIL, _ENP, _ASCENDING(),)
#define DEC_DIV(A, B) IF_DEC_LT(A, B)(0)(_DEC_X(A, A, B,_DEC_DIV(A, A, B)))

#define DEC_MOD(A, B) IF_DEC_LT(A, B)(A)(_dec_minus(A, DEC_MUL(B, DEC_DIV(A, B))))


/**
 ** @brief Declare the types that are going to be used with a
 ** ::LEN_ARG or ::LEN_MODARG parameter list.
 **/
#define VA_TYPES(NAME, ...)   TYPEDEFS(PASTE(NAME, _mod_type_), __VA_ARGS__)

#define _VA_MODARG(AP, NAME, M, ...) va_arg(AP, PASTE(NAME, _mod_type_, M))

/**
 ** @brief Obtain the next argument in the variable argument list of
 ** function @a NAME.
 **
 ** This takes three arguments, @a AP, @c NAME and  @c R.
 ** @a AP is the @c va_list that has been defined in the function.
 **
 ** @c NAME is the name of the function
 **
 ** @c R should be the position modulo the module M that was given to
 ** ::LEN_MODARG. @c R defaults to 0 if omitted.
 ** @see VA_ARGS
 **/
#define VA_MODARG(AP, ...) _VA_MODARG(AP, __VA_ARGS__, 0, ~)

#define _CAS1(NAME, X, N) (PASTE(NAME, _mod_type_0){ X }
#define _CAS2(NAME, X, N) (PASTE(NAME, _mod_type_, DEC_MOD(N, 2))){ X }
#define _CAS3(NAME, X, N) (PASTE(NAME, _mod_type_, DEC_MOD(N, 3))){ X }
#define _CAS4(NAME, X, N) (PASTE(NAME, _mod_type_, DEC_MOD(N, 4))){ X }
#define _CAS5(NAME, X, N) (PASTE(NAME, _mod_type_, DEC_MOD(N, 5))){ X }
#define _MODARG_LIST(NAME, F, N, ...) PASTE(_DOIT, N)(NAME, N, _REV, F, __VA_ARGS__,)

#define LEN_MODARG(NAME, M, ...) _MODARG_(M)(__VA_ARGS__), _MODARG_LIST(NAME, PASTE(_CAS, M), NARG(__VA_ARGS__), REVS(__VA_ARGS__))
#define LEN_ARG(NAME, ...) _MODARG_(1)(__VA_ARGS__), _MODARG_LIST(NAME, _CAS1, NARG(__VA_ARGS__), REVS(__VA_ARGS__))


#endif 	    /* !ORWL_MACRO_H_ */
