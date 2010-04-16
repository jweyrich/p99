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
#define PASTE2(_1, _2) _1 ## _2
#define PASTE3(_1, _2, _3) _1 ## _2 ## _3
#define PASTE4(_1, _2, _3, _4) _1 ## _2 ## _3 ## _4
#define PASTE5(_1, _2, _3, _4, _5) _1 ## _2 ## _3 ## _4 ## _5
#define PASTE6(_1, _2, _3, _4, _5, _6) _1 ## _2 ## _3 ## _4 ## _5 ## _6

/**
 ** @brief Return the 64th argument.
 **/
#define _ARG_64(                                                        \
  _01, _02, _03, _04, _05, _06, _07, _08, _09, _0A, _0B, _0C, _0D, _0E, _0F, _10, \
  _11, _12, _13, _14, _15, _16, _17, _18, _19, _1A, _1B, _1C, _1D, _1E, _1F, _20, \
  _21, _22, _23, _24, _25, _26, _27, _28, _29, _2A, _2B, _2C, _2D, _2E, _2F, _30, \
  _31, _32, _33, _34, _35, _36, _37, _38, _39, _3A, _3B, _3C, _3D, _3E, _3F, _40, \
  ...)                                                                  \
  _40

#define _INV(N) PASTE2(_variable_argument_list_must_be_divisible_by_, N)

#define _NARG_64_1(...)                                                 \
_hex2dec_(__NARG_64(__VA_ARGS__,                                        \
                    0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, \
                    0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, \
                    0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, \
                    0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00))

/**
 ** @brief Return the length of the variate argument list.
 **
 ** This supposes that the length is less than 64.
 **
 ** Idea is due to Laurent Deniau, CERN, and taken from a discussion
 ** in comp.lang.c.
 **/
#define _NARG_64(...) _NARG_64_1(__VA_ARGS__)

/**
 ** @brief Return the number of pairs of the variate argument list.
 **
 ** This supposes that the total length is less than 64.
 **
 ** If the length of the list is odd, a compile time error occurs.
 **/
#define _NARG_64_2(...)                                                 \
_hex2dec_(__NARG_64(__VA_ARGS__,                                        \
                    _INV(2), 0x1F, _INV(2), 0x1E, _INV(2), 0x1D, _INV(2), 0x1C, \
                    _INV(2), 0x1B, _INV(2), 0x1A, _INV(2), 0x19, _INV(2), 0x18, \
                    _INV(2), 0x17, _INV(2), 0x16, _INV(2), 0x15, _INV(2), 0x14, \
                    _INV(2), 0x13, _INV(2), 0x12, _INV(2), 0x11, _INV(2), 0x10, \
                    _INV(2), 0x0F, _INV(2), 0x0E, _INV(2), 0x0D, _INV(2), 0x0C, \
                    _INV(2), 0x0B, _INV(2), 0x0A, _INV(2), 0x09, _INV(2), 0x08, \
                    _INV(2), 0x07, _INV(2), 0x06, _INV(2), 0x05, _INV(2), 0x04, \
                    _INV(2), 0x03, _INV(2), 0x02, _INV(2), 0x01, _INV(2), 0x00))

/**
 ** @brief Return the number of triples of the variate argument list.
 **
 ** This supposes that the total length is less than 64.
 **
 ** If the length of the list is not divisible by 3, a compile time error occurs.
 **/
#define _NARG_64_3(...)                                                 \
_hex2dec_(__NARG_64(__VA_ARGS__,                                        \
                    0x16,                                               \
                    _INV(3), _INV(3), 0x15, _INV(3), _INV(3), 0x14, _INV(3), _INV(3), 0x13, \
                    _INV(3), _INV(3), 0x12, _INV(3), _INV(3), 0x11, _INV(3), _INV(3), 0x10, \
                    _INV(3), _INV(3), 0x0F, _INV(3), _INV(3), 0x0D, _INV(3), _INV(3), 0x0C, \
                    _INV(3), _INV(3), 0x0B, _INV(3), _INV(3), 0x0A, _INV(3), _INV(3), 0x09, \
                    _INV(3), _INV(3), 0x08, _INV(3), _INV(3), 0x07, _INV(3), _INV(3), 0x06, \
                    _INV(3), _INV(3), 0x05, _INV(3), _INV(3), 0x04, _INV(3), _INV(3), 0x03, \
                    _INV(3), _INV(3), 0x02, _INV(3), _INV(3), 0x01, _INV(3), _INV(3), 0x00))

/**
 ** @brief Return the number of quadruples of the variate argument list.
 **
 ** This supposes that the total length is less than 64.
 **
 ** If the length of the list is not divisible by 4, a compile time error occurs.
 **/
#define _NARG_64_4(...)                                                 \
_hex2dec_(__NARG_64(__VA_ARGS__,                                        \
                    _INV(4), _INV(4), _INV(4), 0xF, _INV(4), _INV(4), _INV(4), 0xE, \
                    _INV(4), _INV(4), _INV(4), 0xD, _INV(4), _INV(4), _INV(4), 0xC, \
                    _INV(4), _INV(4), _INV(4), 0xB, _INV(4), _INV(4), _INV(4), 0xA, \
                    _INV(4), _INV(4), _INV(4), 0x9, _INV(4), _INV(4), _INV(4), 0x8, \
                    _INV(4), _INV(4), _INV(4), 0x7, _INV(4), _INV(4), _INV(4), 0x6, \
                    _INV(4), _INV(4), _INV(4), 0x5, _INV(4), _INV(4), _INV(4), 0x4, \
                    _INV(4), _INV(4), _INV(4), 0x3, _INV(4), _INV(4), _INV(4), 0x2, \
                    _INV(4), _INV(4), _INV(4), 0x1, _INV(4), _INV(4), _INV(4), 0x0))

#define __NARG_64(...) _ARG_64(__VA_ARGS__)

#define _MODARG_(_X) PASTE2(_NARG_64_,  _X)

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
 ** #define toto(A, ...) FSYMB(toto)(A, LEN_ARG(__VA_ARGS__))
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
 **       ret += va_arg(ap, unsigned);
 **     }
 **     va_end(ap);
 **     return ret % a;
 ** }
 ** @endcode
 ** In this toy example @c toto can be used as
 ** @code
 ** unsigned magic = toto(3, 1, 3, 5, 7);
 ** @endcode
 ** which will result in computing the sum of 1, 3, 5, 7 (the variable
 ** arguments), i.e 16, and compute that value mod 3 (the fixed
 ** argument @a a). So @a magic should hold the value 1 thereafter.
 **
 ** @param X is the name of the `length' parameter that you want to
 ** use in the definition of the function. As in the example above it
 ** should be then used as the second argument to @c va_start and as a
 ** loop boudary when you actual handle the argument list. @a X is
 ** implicitly declared to have type @c size_t.
 **
 ** @see LEN_ARG
 ** @see LEN_MODARG
 ** @see FSYMB
 **/
#define VA_ARGS(X) size_t X, ...

#define VA_ARGS_DOCUMENTATION(NAME)                                     \
                                                                        \
/*! @see VA_ARGS */                                                     \
/*! This is actually implemented as a macro that helps to provide the length of the variable length argument list to the function. */


#define _hex2dec_(HEX) PASTE2(_hex2dec_, HEX)
#define _dec2hex_(DEC) PASTE2(_dec2hex_, DEC)
#define _predecessor(N) PASTE2(_predecessor_, N)
#define _itpredecessor_0(DEC) DEC
#define _dec2uni(DEC) PASTE2(_dec2uni_, DEC)
#define _uni2dec(UN) PASTE2(_uni2dec_, UN)
#define _uni_add(U,V) PASTE2(U, V)

#define ____dec_add(U,V) _uni_add(U,V)
#define ___dec_add(D,E) ____dec_add(_dec2uni(D),_dec2uni(E))
#define __dec_add(D,E) _uni2dec(___dec_add(D,E))
#define _dec_add(D,E) __dec_add(D,E)

#define _predecessor_0 minus_1
#define _dec_eval(EDEC) PASTE2(_dec_eval_, EDEC)
#define _dec_minus(D,E) PASTE2(_itpredecessor_, E)(D)

#define __PASTE(_N, ...) PASTE2(PASTE, _N)(__VA_ARGS__)
#define _PASTE(...) __PASTE(_predecessor(_NARG_64(~, __VA_ARGS__)), __VA_ARGS__)
#define PASTE(...) _PASTE(__VA_ARGS__)

/**
 ** @brief Declare the value of the @a M th default argument for @a
 ** NAME to be of value @a V and of type @a T .
 **
 ** @param NAME must have been defined as described for
 ** #CALL_WITH_DEFAULTS.
 **
 ** @param T Obviously, should coincide with the type of the
 ** corresponding argument that is given in the definition of the real
 ** function @a NAME.
 **
 ** @param M The position of the argument in the call of @a
 ** NAME. Positions are counted from 0. The use of this makes only
 ** sense if you define it for all possible @a M to the end of @a
 ** NAME's argument list, e.g for 1 and 2 if you have a function with
 ** three arguments. Position @a M as zero, i.e all arguments are
 ** provided by default, is based on an extension of C99 that is non
 ** standard. Avoid it if you can.
 **
 ** @param V is not limited to a constant but may be any type of
 ** expression with return type @a T that can be evaluated at the
 ** point of definition, using any global function, constant or
 ** variable. Such an evaluation that takes place at run time whenever
 ** a default argument must be provided. So you'd better be very
 ** careful with side effects.
 **
 ** Use a corresponding ::define_defarg in a .c file to ensure
 ** that all functions are realized.
 **/

#define declare_defarg(NAME, M, T, V)                                   \
/*! @brief Default initializer for argument M **/                       \
/*! @return the expression `V' as evaluated at the place of the definition. **/ \
/*! @see NAME **/                                                       \
  inline T PASTE(NAME, _defarg_, M)(void) { return (V); }               \
  enum PASTE(_dummy_, NAME, _defarg_, M) { PASTE(_dummy_, NAME, _defarg_, M) }

/**
 ** @brief Define the symbols that are declared through a
 ** corresponding call ::declare_defarg.
 **/
#define define_defarg(NAME, M, T) T PASTE(NAME, _defarg_, M)(void)

/** @internal **/
#define FSYMB_(NAME) PASTE2(NAME, _fsymb_)

/** @brief Mangle @a NAME 
 **
 ** This should only be used in declaration and definition of the
 ** function that is hidden behind the macro @a NAME.
 **/
#define FSYMB(NAME) FSYMB_(NAME)

/**
 ** @brief Provide a documentation section to a function defined with ::CALL_WITH_DEFAULTS
 **/
#define FSYMB_DOCUMENTATION(NAME)                                       \
/*! @see CALL_WITH_DEFAULTS */                                                \
/*! @see declare_defarg */                                              \
/*! This is actually implemented as a macro that helps to provide default arguments to the real function. */

/* The construct of eating away an empty argument list with `, ## __VA_ARGS__'
   only works for some compilers, namely gcc, icc and IBM. Therefore
   the orwl library itself should not use default arguments for the
   zeroth argument. */
#ifndef NO_ZERO_DEFARG
#define _wda_0(NAME, ...) __VA_ARGS__
#define ____call_wda(NAME, K, ...) PASTE2(_wda_, K)(NAME, ## __VA_ARGS__)
#define ___call_wda(NAME, K, ...) ____call_wda(NAME, K, ## __VA_ARGS__)
#define __call_wda(NAME, M, N, ...) ___call_wda(NAME, _dec_minus(M, N), ## __VA_ARGS__)
#define _call_wda(NAME, M, ...) __call_wda(NAME, M, _predecessor(_NARG_64(~, ## __VA_ARGS__)), ## __VA_ARGS__)
# define LEN_MODARG(X, ...) _MODARG_(X)(__VA_ARGS__), ## __VA_ARGS__
# define LEN_ARG(...) _MODARG_(1)(__VA_ARGS__), ## __VA_ARGS__
#else
#define _wda_0(NAME, ...) __VA_ARGS__
#define ____call_wda(NAME, K, ...) PASTE2(_wda_, K)(NAME, __VA_ARGS__)
#define ___call_wda(NAME, K, ...) ____call_wda(NAME, K, __VA_ARGS__)
#define __call_wda(NAME, M, N, ...) ___call_wda(NAME, _dec_minus(M, N), __VA_ARGS__)
#define _call_wda(NAME, M, ...) __call_wda(NAME, M, _predecessor(_NARG_64(~, __VA_ARGS__)), __VA_ARGS__)
# define LEN_MODARG(X, ...) _MODARG_(X)(__VA_ARGS__), __VA_ARGS__
# define LEN_ARG(...) _MODARG_(1)(__VA_ARGS__), __VA_ARGS__
#endif

# define CALL_WITH_DEFAULTS(NAME, M, ...) NAME(_call_wda(NAME, M, __VA_ARGS__))

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
 ** @warning Don't use this if you have give a default for the 0th
 ** argument of a function. Because of the weird rules for empty
 ** argument lists of C99 and the preprocessor a more complicated set
 ** of macros, ::CALL_WITH_DEFAULTS_EVEN_EMPTY, must be used.
 **/


#define __cexpr_empty(...) (!# __VA_ARGS__[1])

/** preprocessor macro that results in a compiler expression that is
 ** true iff the past argument consists of a token that is exactly
 ** one character wide. */
#define _cexpr_empty(...) __cexpr_empty(__VA_ARGS__)

/**
 ** @define expand to a comma token
 **
 ** This sometimes needed for trick macros such as
 ** ::CALL_WITH_DEFAULTS_EVEN_EMPTY to ensure that a macro parameter
 ** inside a recursion is not empty.
 **/
#define _COMMA_ ,

/** preprocessor macro that results in a compiler expression of type
 ** @a T that corresponds to the variadic argument. The variadic
 ** argument must already start with a comma. @a DUMMY is a dummy
 ** variable of type @a T whoes value might be copied once, when not
 ** optimizing, but will be otherwise ignored.
 **/
#define _non_empty_first(T, DUMMY, ...) (((T[2]){ DUMMY __VA_ARGS__ })[1])

/** preprocessor macro that results in a compiler expression of type
 ** @a T that evaluates either to the variadic argument, if it is
 ** non-empty, or to the default argument for @a NAME if none such
 ** argument is given. The variadic argument must already start with a
 ** comma. @a DUMMY is a dummy variable of type @a T whoes value might
 ** be copied once, when not optimizing, but will be otherwise
 ** ignored.
 **
 ** The variadic argument or the default function will be evaluated
 ** only if necessary and at most once. 
 **/
#define __if_empty_default(NAME, T, DUMMY, ...) (_cexpr_empty(__VA_ARGS__) ? PASTE2(NAME, _defarg_0)() : _non_empty_first(T, DUMMY, __VA_ARGS__))

#define _IGNORE(...)
#define _IDENT(...) __VA_ARGS__
#define _SKIP_ 
#define _CLAUSE1(...) __VA_ARGS__ _IGNORE
#define _CLAUSE2(...) _IDENT

/**
 ** @brief Preprocessor conditional
 **
 ** @code
 ** _if_more_ignore(arg list)(arg list is short)(arg list is long)
 ** @endcode
 **
 **/
#define _if_more_ignore(...)                                            \
  _ARG_64(__VA_ARGS__,                                                  \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, \
          _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2, _CLAUSE2,             \
                                                                        \
          _CLAUSE1, _CLAUSE1, _CLAUSE1,                                 \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, \
          _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1, _CLAUSE1)

/**
 ** @brief Construct a macro that can even replace argument 0 by a default.
 **
 ** This is similar to a macro constructed with ::CALL_WITH_DEFAULTS but
 ** for the subtle difference of the ::_COMMA_ macro as given here.
 ** @code
 ** #define ftester(...) ftester(CALL_WITH_DEFAULTS_EVEN_EMPTY(ftester, 3, _COMMA_ __VA_ARGS__))
 ** @endcode
 **
 ** In difference to ::CALL_WITH_DEFAULTS @c ftester may now even be called
 ** with an empty argument list:
 **
 ** @code
 ** ftester();
 ** ftester(a);
 ** ftester(a, b);
 ** ftester(a, b, c);
 ** @endcode
 **
 ** The resulting code for the first two cases resembles more to an
 ** insult than to reasonable code. Basically it ensures the that the
 ** default function for @c tester and argument 0 gets called exactly
 ** when its needed. On the other hand the lack of any expression at
 ** all as an argument still results in something that the compiler
 ** should be able to digest. Don't ask.
 **
 ** The later two cases should result in something, simpler, where
 ** calls to the default functions are just substituted for missing
 ** arguments. In particular the last, when @c ftester is called with
 ** exactly the 3 arguments that it requires, the result should just
 ** be the identity.
 **
 ** But defining things becomes a bit more complicated than with
 ** ::CALL_WITH_DEFAULTS:
 ** - The default argument for position 0 @b must be declared and
 **   defined with ::declare_defarg_0 and ::define_defarg_0
 **   respectively.
 ** - Functions with just one argument even need more special care
 **   since otherwise the real function definition will explode. Use
 **   something like
 **
 ** @code
 ** #define ftaster(...) CALL_WITH_DEFAULTS_EVEN_EMPTY(ftaster, 1, _COMMA_ __VA_ARGS__)
 **
 ** void ftaster _SKIP_ (int A) {
 **   fprintf(stderr, "ftaster has %d\n", A);
 ** }
 **
 ** declare_defarg_0(ftaster, int, -1);
 ** @endcode
 **
 ** So in addition to the ::_COMMA_ inside the @c define, there must
 ** also be a ::_SKIP_ in the function definition itself.
 **/
#define CALL_WITH_DEFAULTS_EVEN_EMPTY(NAME, M, ...)                     \
  NAME(                                                                 \
_if_more_ignore(__VA_ARGS__)                                            \
(_call_wda(NAME, M, __if_empty_default(NAME, PASTE2(NAME, _def0dummy_t), PASTE2(NAME, _def0dummy), __VA_ARGS__))) \
(_call_wda(NAME, M __VA_ARGS__))                                        \
)

#define declare_defarg_0(NAME, T, V)                    \
typedef T PASTE2(NAME, _def0dummy_t);                  \
PASTE2(NAME, _def0dummy_t) PASTE2(NAME, _def0dummy);  \
declare_defarg(NAME, 0, T, V)

#define define_defarg_0(NAME, T) define_defarg(NAME, 0, T)


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

#define _DOIT1(N, OP, FUNC, ...) FUNC(__VA_ARGS__)

#define _IDENT(...) __VA_ARGS__
#define _STRLEN(...) strlen(__VA_ARGS__)

#define _SUM(X, REC) (X + REC)
#define _SEQ(X, REC) X, REC
#define _REV(X, REC) REC, X
#define _STRCAT(X, REC) strcat(X, REC)
#define _STRTAC(X, REC) strcat(REC, X)

#define _SUMS(N, ...) PASTE2(_DOIT, N)(N, _SUM, _IDENT, __VA_ARGS__)
#define SUMS(...) _SUMS(_NARG_64(__VA_ARGS__),__VA_ARGS__)

#define _STRLENS(N, ...) PASTE2(_DOIT, N)(N, _SUM, _STRLEN, __VA_ARGS__)
#define STRLENS(...) _STRLENS(_NARG_64(__VA_ARGS__),__VA_ARGS__)

#define _REVS(N, ...) PASTE2(_DOIT, N)(N, _REV, _IDENT, __VA_ARGS__)
#define REVS(...) _REVS(_NARG_64(__VA_ARGS__),__VA_ARGS__)

#define _STRCATS(N, ...) PASTE2(_DOIT, N)(N, _STRTAC, _IDENT, __VA_ARGS__)
#define STRCATS(...) _STRCATS(_NARG_64(__VA_ARGS__), REVS(__VA_ARGS__))

#define JOIN(...) STRCATS((char[SUMS(STRLENS(__VA_ARGS__)) + 1]){ 0 }, __VA_ARGS__)


#endif 	    /* !ORWL_MACRO_H_ */

