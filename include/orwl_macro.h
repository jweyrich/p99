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

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the paste operation first, and then an evaluation
 ** of the result. Thus
 ** @code
 ** #define B(x) toto(x)
 ** CAT2(A, B(6));
 ** @endcode
 ** will result in
 ** @code
 ** AB(6);
 ** @endcode
 ** and not in
 ** @code
 ** Atoto(6)
 ** @endcode
 **/
#define CAT2(_1, _2) _1 ## _2

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the evaluation of the arguments first and then
 ** proceeds at the concatenation
 ** of the results. Thus
 ** @code
 ** #define B(x) toto(x)
 ** CAT2(A, B(6));
 ** @endcode
 ** will result in
 ** @code
 ** Atoto(6)
 ** @endcode
 ** and not in
 ** @code
 ** AB(6);
 ** @endcode
 **/
#define PASTE2(_1, _2) CAT2(_1, _2)

#define P99___PASTE(F, N, ...) F ## N(__VA_ARGS__)
#define P99__PASTE(N, ...) P99___PASTE(PASTE, N, __VA_ARGS__)

#define P99__DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP, ...)   \
  IF_EMPTY(SIGN)(+)(SIGN)P99__SKIP_ PASTE(                  \
  IF_EMPTY(INT)(0)(INT),                                \
  .,                                                    \
  IF_EMPTY(FRAC)(0)(FRAC),                              \
  E,                                                    \
  IF_EMPTY(ESIGN)(+)(ESIGN),                            \
  IF_EMPTY(EXP)(0)(EXP),                                \
  __VA_ARGS__)

#ifdef DOXYGEN
/** 
 ** @brief construct a @c double constant in decimal representation.
 **
 ** @param SIGN the sign of the constant, (+)
 ** @param INT the integer part, (0)
 ** @param FRAC the fractional part, (0)
 ** @param ESIGN sign of the exponent, (+)
 ** @param EXP exponent in decimal, (0)
 **
 ** So if all parameters are empty such as
 ** @code
 ** DEC_DOUBLE()
 ** DEC(,,,,)
 ** @endcode
 ** this produces @c +0.0E+0
 **/
#define DEC_DOUBLE(SIGN, INT, FRAC, ESIGN, EXP)
#else
#define DEC_DOUBLE(...)                         \
  IF_DEC_GE(NARG(__VA_ARGS__), 6)               \
  (P99__DEC_DOUBLE(__VA_ARGS__))                    \
  (P99__DEC_DOUBLE(__VA_ARGS__,,,,,))
#endif


#define P99__HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP, ...)  \
  IF_EMPTY(SIGN)(+)(SIGN)P99__SKIP_ PASTE(                          \
  0x,                                                           \
  IF_EMPTY(HEXINT)(0)(HEXINT),                                  \
  .,                                                            \
  IF_EMPTY(HEXFRAC)(0)(HEXFRAC),                                \
  P,                                                            \
  IF_EMPTY(ESIGN)(+)(ESIGN),                                    \
  IF_EMPTY(BINEXP)(0)(BINEXP),                                  \
  __VA_ARGS__)

#ifdef DOXYGEN
/**
 ** @brief construct a @c double constant in hexadecimal representation.
 **
 ** @see DEC_DOUBLE for the analog with decimal representation
 **/
#define HEX_DOUBLE(SIGN, HEXINT, HEXFRAC, ESIGN, BINEXP)
#else
#define HEX_DOUBLE(...)                         \
  IF_DEC_GE(NARG(__VA_ARGS__), 6)               \
  (P99__HEX_DOUBLE(__VA_ARGS__))                    \
  (P99__HEX_DOUBLE(__VA_ARGS__,,,,,))
#endif

#define P99__INV(N) PASTE(P99__variable_argument_list_must_be_divisible_by_, N)

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
#define P99__NARG(...) P99__NARG_1(__VA_ARGS__)

#define P99__IGNORE(...)
#define P99__IDENT(...) __VA_ARGS__
#define P99__SKIP_
#define P99__CLAUSE1(...) __VA_ARGS__ P99__IGNORE
#define P99__CLAUSE2(...) P99__IDENT

#define P99__IS__EQ__(...) ,
#define P99__IS_0_EQ_0(...) ,
#define P99__IS_1_EQ_1(...) ,
#define P99__IS_2_EQ_2(...) ,
#define P99__IS_3_EQ_3(...) ,
#define P99__IS_4_EQ_4(...) ,
#define P99__IS_5_EQ_5(...) ,
#define P99__IS_6_EQ_6(...) ,
#define P99__IS_7_EQ_7(...) ,
#define P99__IS_8_EQ_8(...) ,
#define P99__IS_9_EQ_9(...) ,
#define P99__IS_00_EQ_00(...) ,
#define P99__IS_10_EQ_10(...) ,
#define P99__IS_01_EQ_01(...) ,
#define P99__IS_11_EQ_11(...) ,
#define P99___IF_CLAUSE(A,B,C,...) C
#define P99__IF_CLAUSE(EXP) P99___IF_CLAUSE(EXP, P99__CLAUSE1, P99__CLAUSE2, ~)
#define P99__IF_NOT_CLAUSE(EXP) P99___IF_CLAUSE(EXP, P99__CLAUSE2, P99__CLAUSE1, ~)

/**
 ** @brief Test two words @a A and @a B if they are equal.
 **
 ** @a A and @a B must be just one word, i.e composed of
 ** alpha-numerical characters and underscores.
 **
 ** For such a test to work properly a corresponding macro @c P99__IS_A_EQ_A
 ** must exist for all @a A such that @a B may be the same word as @a
 ** A. E.g for the test
 **
 ** @code
 ** IF_EQ(0, X)(expand_when_equal)(expand_when_unequal)
 ** @endcode
 **
 ** the macro ::P99__IF_0_EQ_0 must exist. (Which it does in that case).
 **
 ** @see IF_DEC_EQ for equality of not too large decimal numbers
 **/
#define IF_EQ(A, B) P99__IF_CLAUSE(PASTE(P99__IS_,A,_EQ_,B)())

/**
 ** @brief Test two words @a A and @a B if they are unequal.
 **
 ** @see IF_EQ 
 **/
#define IF_NE(A, B) P99__IF_NOT_CLAUSE(PASTE(P99__IS_,A,_EQ_,B)())

/**
 ** @brief Test two decimal numbers @a A and @a B if they are equal.
 **/
#define IF_DEC_EQ(A, B) IF_EQ(0,P99__dec_minus(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if they are unequal.
 **/
#define IF_DEC_NE(A, B) IF_NE(0,P99__dec_minus(A,B))

#define IF_GE_0(A) P99__IF_CLAUSE(PASTE(P99__IS_,A,_GE_,0)())
#define IF_LT_0(A) P99__IF_NOT_CLAUSE(PASTE(P99__IS_,A,_GE_,0)())

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is greater
 ** or equal to @a B.
 **/
#define IF_DEC_GE(A, B) IF_GE_0(P99__dec_minus(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is less
 ** or equal to @a B.
 **/
#define IF_DEC_LE(A, B) IF_GE_0(P99__dec_minus(B,A))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** less than @a B.
 **/
#define IF_DEC_LT(A, B) IF_LT_0(P99__dec_minus(A,B))

/**
 ** @brief Test two decimal numbers @a A and @a B if @a A is strictly
 ** greater than @a B.
 **/
#define IF_DEC_GT(A, B) IF_LT_0(P99__dec_minus(B,A))

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
 ** uses the fact that a function macro (@c P99__IS__EQ__ in this case) is
 ** left untouched if it is not followed by a parenthesis. See
 ** http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 **
 ** @return tokens 0 or 1
 **/
#define IS_EMPTY(...)                                                   \
  P99__ISEMPTY(                                                             \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          HAS_COMMA(__VA_ARGS__),                                       \
          /* test if _IS__EQ__ together with the argument               \
             adds a comma */                                            \
    HAS_COMMA(P99__IS__EQ__ __VA_ARGS__),                                \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          HAS_COMMA(__VA_ARGS__ (~)),                                   \
          /* test if placing it between _IS__EQ__ and the               \
             parenthesis adds a comma */                                \
          HAS_COMMA(P99__IS__EQ__ __VA_ARGS__ (~))                          \
          )

#define P99__ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(P99__IS_EMPTY_CASE_, _0, _1, _2, _3))
#define P99__IS_EMPTY_CASE_0001 ,

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
#define P99__LOGIC_OR(A, B) IF_EQ(00, CAT2(A, B))(0)(1)

/**
 ** @brief Do a logical inclusive or of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_OR(A, B) P99__LOGIC_OR(LOGIC_EVAL(A), LOGIC_EVAL(B))

#define P99__LOGIC_AND(A, B) IF_EQ(00, CAT2(A, B))(1)(0)

/**
 ** @brief Do a logical and of the arguments.
 **
 ** @see LOGIC_EVAL for how the individual arguments are considered to
 ** be truth values.
 **
 ** @return tokens 0 or 1
 **/
#define LOGIC_AND(A, B) P99__LOGIC_AND(LOGIC_NOT(A), LOGIC_NOT(B))


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
 ** @see P99__NARG for a macro that accounts an empty list to be 1
 **/
#define NARG(...) IF_EMPTY(P99___VA_ARGS__)(0)(P99__NARG(__VA_ARGS__))

#define P99__IS_void_EQ_void(...) ,

/**
 ** @brief Test if the argument consists of exactly the word @c void.
 **
 ** @see IF_VOID for a macro that test whether or not its argument is
 ** empty @b or if it consists of the word @c void.
 **/
#define IS_void(...)                                                    \
IS_EQ_2(P99__NARG(P99__IS_void_EQ_ ## __VA_ARGS__ (~) P99__IS_void_EQ_ ## __VA_ARGS__))

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

#define HAS_COMMA(...) IF_DEC_GT(P99__NARG(__VA_ARGS__), 1)(1)(0)

#define IS_COMMA(...)                                                   \
IF_EQ_2(NARG(__VA_ARGS__))                                              \
(LOGIC_AND(IS_EMPTY(CHS(0,__VA_ARGS__)),IS_EMPTY(CHS(1,__VA_ARGS__))))  \
(0)

#define P99__MODARG_(_X) PASTE(P99__NARG_,  _X)

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
#define VA_ARGS(X) size_t X /*!< the number of arguments that follow */, ...

#define VA_ARGS_DOCUMENTATION(NAME)                                     \
                                                                        \
/*! @see VA_ARGS */                                                     \
/*! This is actually implemented as a macro that helps to provide the length of the variable length argument list to the function. */

/**
 ** @brief Macro that expands to the predecessor of decimal constant
 ** #a N
 **/
#define P99___DEC_PRED(P, N) P ## N
#define P99__DEC_PRED(N) P99___DEC_PRED(P99__DEC_PRED_ , N)
#define DEC_PRED(N) P99__DEC_PRED(N)
#define P99__itpredecessor_0(DEC) DEC
#define P99__dec2uni(DEC) PASTE(P99__dec2uni_, DEC)
#define P99__uni2dec(UN) PASTE(P99__uni2dec_, UN)
#define P99__uni_add(U,V) PASTE(U, V)

#define P99_____dec_add(U,V) P99__uni_add(U,V)
#define P99____dec_add(D,E) P99_____dec_add(P99__dec2uni(D),P99__dec2uni(E))
#define P99___dec_add(D,E) P99__uni2dec(P99____dec_add(D,E))
#define P99__dec_add(D,E) P99___dec_add(D,E)

#define P99__DEC_PRED_0 minus_1
#define P99__dec_eval(EDEC) PASTE(P99__dec_eval_, EDEC)
#define P99__dec_minus(D,E) PASTE(P99__itpredecessor_, E)(D)


#define P99__FSYMB(NAME) PASTE(NAME, _f, sy, mb, _)

/** @brief Mangle @a NAME 
 **
 ** This should only be used in declaration and definition of the
 ** function that is hidden behind the macro @a NAME.
 **/
#define FSYMB(NAME) P99__FSYMB(NAME)

/**
 ** @brief Provide a documentation section to a function defined with ::CALL_WITH_DEFAULTS
 **/
#define FSYMB_DOCUMENTATION(NAME)                                       \
/*! @see CALL_WITH_DEFAULTS */                                          \
/*! @see DECLARE_DEFARG */                                              \
/*! @see NAME This is actually implemented as a macro that helps to provide default arguments to the real function. */

#ifdef DOXYGEN
# define CALL_WITH_DEFAULTS(NAME, M, ...) NAME(__VA_ARGS__)
#else
# define CALL_WITH_DEFAULTS(NAME, M, ...)               \
NAME(IF_EQ(0,M)                                         \
     (__VA_ARGS__)                                      \
     (IF_EMPTY(__VA_ARGS__)                             \
      (P99__DEFARGS(NAME, M, PASTE(NAME,_defarg_0)()))      \
      (P99__DEFARGS(NAME, M, __VA_ARGS__))                  \
      )                                                 \
     )
#endif


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
 ** The following lines heal this.
 **
 ** @code
 ** PROTOTYPE(int, pthread_mutex_init, pthread_mutex_t*, pthread_mutexattr_t const*);
 ** #define pthread_mutex_init(...) CALL_WITH_DEFAULTS(pthread_mutex_init, 2, __VA_ARGS__)
 ** DECLARE_DEFARG(pthread_mutex_init, , NULL);
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
 ** @see DECLARE_DEFARG
 ** @see PROTOTYPE
 **
 ** This macro is more flexible than the corresponding C++ feature of
 ** default arguments. It also lets you omit middle arguments.
 **
 ** More technically, for arguments that are omitted this just
 ** requires that NAME_defarg_M is defined for function @a NAME
 ** and @a M and that it is callable without arguments. This may just
 ** be a function (as implicitly defined by #DECLARE_DEFARG) or a
 ** macro. For the first case everything the function refers to must
 ** be declare at the point of its definition. For the second case,
 ** the macro is evaluate at the place of the call and could refer to
 ** local variables or anything you like.
 **/

/**
 ** @def PROTOTYPE(RT, NAME, ...)
 ** @brief Define the prototype of function @a NAME.
 **
 ** @a RT is the return type of the function, and the remaining
 ** arguments list the types of the arguments. This is needed by
 ** #DECLARE_DEFARG to determine the type of the functions that return
 ** default arguments.
 **
 **/

/**
 ** @define expand to a comma token
 **
 ** This sometimes needed for trick macros such as
 ** ::CALL_WITH_DEFAULTS_EVEN_EMPTY to ensure that a macro parameter
 ** inside a recursion is not empty.
 **/
#define P99__COMMA_ ,

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
 ** @brief Invalidated pointer
 **
 ** Define a all-one-bits initialized constant of pointer type @a T.
 **/
#define TGARB(T) ((T)~TNULL(uintptr_t))

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
#define TMAX(T) (~TMIN(T))

/**
 ** @brief Typed min value
 **
 ** Define the smallest value that integer type @a T may hold.
 **/
#define TMIN(T) (((T)ISSIGNED(T)) << ((sizeof(T)*CHAR_BIT)-1))


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
#define P99__J(x) (0 ? TNULL(uintmax_t) : (x))

/**
 ** @brief Promote integer expression @a x to the width of @c
 ** size_t but keep signedness if possible.
 **/
#define P99__Z(x) (0 ? TNULL(size_t) : (x))


/** @addtogroup list_processing List processing macros
 ** @brief We provide here a series of macros that take a list of
 ** arguments of arbitrary length and that transform each element in
 ** that list in some way.
 ** @{
 **/


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
 **
 ** This macro does the evaluation of the arguments first and
 ** then proceeds at the concatenation of the results.
 ** @pre the argumentlist should not be empty.
 **/
#define PASTE(...) P99__PASTE(P99__NARG(__VA_ARGS__), __VA_ARGS__)

#define LAST(...) CHS(DEC_PRED(P99__NARG(__VA_ARGS__)), __VA_ARGS__,)
#define ALLBUTLAST(...) PASTE2(P99__PRE,DEC_PRED(P99__NARG(__VA_ARGS__)))(__VA_ARGS__,)
#define P99__FOR1(NAME, OP, FUNC, ...) FUNC(NAME, P99__PRE1(__VA_ARGS__,), 0)
#define FOR(NAME, N, OP, FUNC, ...) PASTE2(P99__FOR, N)(NAME, OP, FUNC, __VA_ARGS__)

#ifndef DOXYGEN
#define REP0(...)
#define REP1(X) X
#endif
/**
 ** @brief Concatenate the token @a X @a N times.
 **/
#define REP(N, X) P99__REP(N, X)
#define P99__REP(N, X) REP ## N(X)


#define P99__IGN(NAME, X, N)
#define P99__IDT(NAME, X, N) X
#define P99__POS(NAME, X, N) N
#define P99__NAM(NAME, X, N) NAME

#define P99__ACCESSOR(NAME, X, N) (NAME)[N]
#define P99__VASSIGN(NAME, X, N) X = P99__ACCESSOR(NAME, X, N)
#define P99__STRLEN(NAME, X, N) strlen(X)
#define P99__TYPD(NAME, X, N) typedef X PASTE2(NAME, N)

#define P99__SUM(NAME, N, X, Y) ((X) + (Y))
#define P99__PROD(NAME, N, X, Y) ((X) * (Y))
#define P99__QUOT(NAME, N, X, Y) ((X) / (Y))
#define P99__XOR(NAME, N, X, Y) ((X) ^ (Y))
#define P99__BOR(NAME, N, X, Y) ((X) | (Y))
#define P99__BAND(NAME, N, X, Y) ((X) & (Y))
#define P99__OR(NAME, N, X, Y) ((X) || (Y))
#define P99__AND(NAME, N, X, Y) ((X) && (Y))

#define P99__SEQ(NAME, N, REC, X) REC, X
#define P99__SEP(NAME, N, REC, X) REC; X
#define P99__SER(NAME, N, REC, X) REC X
#define P99__REV(NAME, N, REC, X) X, REC
#define P99__STRCAT(NAME, N, REC, X) strcat(REC, X)

/**
 ** @brief Compute the right associative operation @a OP of all the arguments.
 **
 ** Here @a OP should receive four arguments <code>NAME, X, N,
 ** REC</code> out of which @c NAME and @c N are ignored and @c X and
 ** @c REC should be interpreted as the left and right hand of the
 ** operator action, respectively.
 **
 ** @a M is the length of the list that follows it.
 **/
#define BIGOP(OP, M, ...) FOR( , M, OP, P99__IDT, __VA_ARGS__,)

/**
 ** @brief Compute the right associative sum of all the arguments.
 **/
#define SUMS(...) BIGOP(P99__SUM, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative product of all the arguments.
 **/
#define PRODS(...) BIGOP(P99__PROD, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative quotient of all the arguments.
 **/
#define QUOTS(...) BIGOP(P99__QUOT, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative bitwise exclusive or of all the arguments.
 **/
#define XORS(...) BIGOP(P99__XOR, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative bitwise or of all the arguments.
 **/
#define BORS(...) BIGOP(P99__BOR, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative bitwise and of all the arguments.
 **/
#define BANDS(...) BIGOP(P99__BAND, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative logical or of all the arguments.
 **/
#define ORS(...) BIGOP(P99__OR, (NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Compute the right associative logical and of all the arguments.
 **/
#define ANDS(...) BIGOP(P99__AND, (NARG(__VA_ARGS__),__VA_ARGS__)


#define P99__STRLENS(N, ...) FOR(,N, P99__SUM, P99__STRLEN, __VA_ARGS__)

/**
 ** @brief Return an expression that returns the sum of the lengths of
 ** all strings that are given as arguments.
 **/
#define STRLENS(...) P99__STRLENS(NARG(__VA_ARGS__),__VA_ARGS__)

#define P99__REVS(N, ...) FOR(,N, P99__REV, P99__IDT, __VA_ARGS__)

/**
 ** @brief Revert the argument list
 **/
#define REVS(...) IF_DEC_LT(NARG(__VA_ARGS__),2)(__VA_ARGS__)(P99__REVS(NARG(__VA_ARGS__),__VA_ARGS__))

#define P99__STRCATS(N, ...) FOR(,N, P99__STRCAT, P99__IDT, __VA_ARGS__)

/**
 ** @brief Append all argument strings after @a TARG to @a TARG.
 **
 ** @a TARG must be compatible with @c char* and provide enough space
 ** to hold the concatenation of all strings. The remaining arguments
 ** must be compatible with @c const char*.
 **/
#define STRCATS(TARG, ...) P99__STRCATS(NARG(TARG, __VA_ARGS__), TARG, __VA_ARGS__)

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

/**
 ** Repeat the parameter @a X @a N times.
 **/
#define REPS(X, N) FOR(X, N, P99__SEQ, P99__NAM, )

/**
 ** @brief Produce a list of length @a N that has the contents of 0,
 ** 1, , @a N-1
 **/
#define POSS(N) FOR(,N, P99__SEQ, P99__POS,)

/**
 ** Produce a list of length @a N that has the contents of @a X[0], @a
 ** X [1], ,
 ** @a X[@a N-1]
 **/
#define ACCESSORS(X, N) FOR(X, N, P99__SEQ, P99__ACCESSOR, )

/**
 ** Cut the argument list at position @a N
 **/
#define SELS(N, ...) PASTE2(P99__PRE, N)(__VA_ARGS__)


/**
 ** @brief Skip @a N elements in the remaining argument list.
 **/
#define SKP(N, ...) PASTE2(P99__SKP, N)(__VA_ARGS__)

/**
 ** @brief Get the sublist of length @a L starting at the @a
 ** N<sup>th</sup> element in the remaining argument list.
 **
 ** Counting of elements starts at 0.
 **/
#define SUB(N, L, ...) P99__SUB(L, SKP(N, __VA_ARGS__))
#define P99__SUB(L, ...) SELS(L, __VA_ARGS__)

/**
 ** @brief Choose the @a N<sup>th</sup> element in the remaining argument
 ** list.
 **
 ** Counting of elements starts at 0.
 **/
#define CHS(N, ...) SUB(N, 1, __VA_ARGS__)

/**
 ** @brief Vector-assign to a list
 **
 ** Produce a list of length @c N that has the contents of @c V0 = @a
 ** NAME[0], @c V1 = @a NAME[1], ..., @c VN-1 = @a NAME[@a N-1], where
 ** V0, etc are the remaining arguments.
 **/
#define VASSIGNS(NAME, ...)                                     \
IF_DEC_LT(NARG(__VA_ARGS__),2)                                  \
(IF_VOID(__VA_ARGS__)((void)0)(__VA_ARGS__ = (NAME)[0]))        \
  (FOR(NAME, P99__NARG(__VA_ARGS__),P99__SEP, P99__VASSIGN, __VA_ARGS__))

#define P99__TYPEDEFS(NAME, N, ...)                         \
  IF_VOID(__VA_ARGS__)                                  \
  (enum { PASTE3(NAME, _eat_the_semicolon_, N) })       \
  (FOR(NAME, N, P99__SEP, P99__TYPD, __VA_ARGS__))

/**
 ** @brief Take each argument of the list and transform it into a
 ** @c typedef of name NAME_0, NAME_1, etc.
 **
 ** Because of syntax problems this can't be used for function or
 ** array type derivatives.
 **/
#define TYPEDEFS(NAME, ...)                             \
P99__TYPEDEFS(NAME, NARG(__VA_ARGS__), __VA_ARGS__)



/** @}
 **/

#ifdef DOXYGEN
#define P99__PROTOTYPE(RT, NAME, ...)                                       \
/*! @remark This function might be hidden behind a macro :: ## NAME of the same name. */ \
RT NAME(__VA_ARGS__)
#define PROTOTYPE(RT, NAME, ...) P99__PROTOTYPE(__VA_ARGS__)
#else
#define P99__PROTOTYPE(RT, NAME, ...)                       \
  RT NAME(IF_EMPTY(__VA_ARGS__)(void)(__VA_ARGS__));    \
  typedef RT CAT2(NAME, _prototype_ret);                \
  TYPEDEFS(CAT2(NAME, _prototype_), __VA_ARGS__)

#define PROTOTYPE(...)                          \
IF_EQ_2(NARG(__VA_ARGS__))                      \
(P99__PROTOTYPE(__VA_ARGS__, void))                 \
(P99__PROTOTYPE(__VA_ARGS__))
#endif


#define P99__EXPR_FUNCTION(NAME, X, N)                                       \
IF_EMPTY(X)                                                     \
()                                                              \
(                                                               \
 inline                                                         \
 PASTE3(NAME, _prototype_, N) PASTE3(NAME, _defarg_, N)(void) { \
   PASTE3(NAME, _prototype_, N) ret = (X);                      \
   return ret;                                                  \
 }                                                              \
)

#define P99__DAFE(NAME, X, N)                                       \
IF_EMPTY(X)                                                     \
(enum { PASTE3(NAME, _boring_, N) })                            \
(PASTE3(NAME, _prototype_, N) PASTE3(NAME, _defarg_, N)(void))

#define P99__DECLARE_DEFARG(NAME, N, ...)                   \
  FOR(NAME, N, P99__SER, P99__EXPR_FUNCTION, __VA_ARGS__)        \
enum { PASTE3(_, NAME, _defarg_dummy_enum_val_) }

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
 ** @see PROTOTYPE on how to declare a prototype of a function
 ** @a NAME that can be used with this
 **
 ** @see CALL_WITH_DEFAULTS on how to declare the macro @a NAME
 **
 ** @see rand48_t_init for a more sophisticated example with
 ** non-constant expressions.
 **/
#define DECLARE_DEFARG(NAME, ...)
#else
#define DECLARE_DEFARG(NAME, ...) P99__DECLARE_DEFARG(NAME, NARG(__VA_ARGS__), __VA_ARGS__)
#endif

#define P99__DEFINE_DEFARG(NAME, N, ...)                                         \
  FOR(NAME, N, P99__SEP, P99__DAFE, __VA_ARGS__)

#ifdef DOXYGEN
/**
 ** @brief Define the symbols that are declared through a
 ** corresponding call ::DECLARE_DEFARG.
 **
 ** The argument list here should be exactly the same as for ::DECLARE_DEFARG.
 **/
#define DEFINE_DEFARG(NAME, ...)
#else
#define DEFINE_DEFARG(NAME, ...) P99__DEFINE_DEFARG(NAME, NARG(__VA_ARGS__), __VA_ARGS__)
#endif


#define P99__DARG(NAME, X, N) IF_EMPTY(X)(PASTE3(NAME, _defarg_, N)())(X)
#define P99___DEFARGS(NAME, N, ...) FOR(NAME, N, P99__SEQ, P99__DARG, __VA_ARGS__)
#define P99__DEFARGS(NAME, N, ...) P99___DEFARGS(NAME, N, IF_DEC_LT(NARG(__VA_ARGS__),N) (__VA_ARGS__, REPS(,P99__dec_minus(N,NARG(__VA_ARGS__)))) (__VA_ARGS__))

#define P99__DEC_MUL(A, B) IF_EQ_0(A)(0)(P99__uni2dec(REP(A, B)))

/**
 ** @brief Generate the product of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning The result must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define DEC_MUL(A, B) IF_DEC_LT(A, B)(P99__DEC_MUL(A, P99__dec2uni(B)))(P99__DEC_MUL(B, P99__dec2uni(A)))
/**
 ** @brief Generate the quotient of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning Both arguments must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define DEC_DIV(A, B) CHS(A, P99__DIV ## B())
/**
 ** @brief Generate the modulus of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning Both arguments must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define DEC_MOD(A, B) CHS(A, P99__MOD ## B())

/**
 ** @brief Declare the types that are going to be used with a
 ** ::LEN_ARG or ::LEN_MODARG parameter list.
 **/
#define VA_TYPES(NAME, ...)   TYPEDEFS(PASTE(NAME, _mod_type_), __VA_ARGS__)

#define P99__VA_MODARG(AP, NAME, M, ...) va_arg(AP, PASTE(NAME, _mod_type_, M))

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
#define VA_MODARG(AP, ...) P99__VA_MODARG(AP, __VA_ARGS__, 0, ~)

#define P99__CAS1(NAME, X, N) (PASTE2(NAME, _mod_type_0){ X }
#define P99__CAS2(NAME, X, N) (PASTE3(NAME, _mod_type_, DEC_MOD(N, 2))){ X }
#define P99__CAS3(NAME, X, N) (PASTE3(NAME, _mod_type_, DEC_MOD(N, 3))){ X }
#define P99__CAS4(NAME, X, N) (PASTE3(NAME, _mod_type_, DEC_MOD(N, 4))){ X }
#define P99__CAS5(NAME, X, N) (PASTE3(NAME, _mod_type_, DEC_MOD(N, 5))){ X }
#define P99__MODARG_LIST(NAME, F, N, ...) FOR(NAME, N, P99__SEQ, F, __VA_ARGS__)

#define LEN_MODARG(NAME, M, ...) P99__MODARG_(M)(__VA_ARGS__), P99__MODARG_LIST(NAME, PASTE2(P99__CAS, M), NARG(__VA_ARGS__), __VA_ARGS__)
#define LEN_ARG(NAME, ...) P99__MODARG_(1)(__VA_ARGS__), P99__MODARG_LIST(NAME, P99__CAS1, NARG(__VA_ARGS__), __VA_ARGS__)


/**
 ** @def branch_expect
 ** @brief Provide a compiler hint concerning the likelihood of a
 ** certain value in an expression @a EXP.
 **
 ** With this you indicate that in the overwhelming number of cases
 ** the integer expression @a EXP will evaluate to @a VAL.
 **
 ** Currently this is only implemented for gcc.
 **/
#if (defined(__GNUC__) && (__GNUC__ > 3))
#define branch_expect(EXP, VAL) __builtin_expect((EXP), (VAL))
#else
#define branch_expect(EXP, VAL) (EXP)
#endif


/**
 ** @brief Add some indications to a @c _init documentation.
 */
#define DOCUMENT_INIT(T)                                                \
/*! @brief Initialize a variable of type T */                           \
/*! @see NEW needs a version of this that takes just the T* as argument. */

/**
 ** @brief Add some indications to a @c _destroy documentation.
 */
#define DOCUMENT_DESTROY(T) /*! @brief Destroy a variable of type T @see T ## _delete needs this. */



#endif 	    /* !ORWL_MACRO_H_ */
