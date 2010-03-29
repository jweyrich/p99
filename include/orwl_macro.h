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

#define _INV(N) _variable_argument_list_must_be_divisible_by_ ## N

#define _NARG_64_1(...)                                                 \
_decimal_(__NARG_64(__VA_ARGS__,                                        \
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
_decimal_(__NARG_64(__VA_ARGS__,                                        \
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
_decimal_(__NARG_64(__VA_ARGS__,                                        \
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
_decimal_(__NARG_64(__VA_ARGS__,                                        \
                    _INV(4), _INV(4), _INV(4), 0xF, _INV(4), _INV(4), _INV(4), 0xE, \
                    _INV(4), _INV(4), _INV(4), 0xD, _INV(4), _INV(4), _INV(4), 0xC, \
                    _INV(4), _INV(4), _INV(4), 0xB, _INV(4), _INV(4), _INV(4), 0xA, \
                    _INV(4), _INV(4), _INV(4), 0x9, _INV(4), _INV(4), _INV(4), 0x8, \
                    _INV(4), _INV(4), _INV(4), 0x7, _INV(4), _INV(4), _INV(4), 0x6, \
                    _INV(4), _INV(4), _INV(4), 0x5, _INV(4), _INV(4), _INV(4), 0x4, \
                    _INV(4), _INV(4), _INV(4), 0x3, _INV(4), _INV(4), _INV(4), 0x2, \
                    _INV(4), _INV(4), _INV(4), 0x1, _INV(4), _INV(4), _INV(4), 0x0))

#define __NARG_64(...) _ARG_64(__VA_ARGS__)

#define _MODARG_(_X) _NARG_64_ ## _X

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


#define _decimal_0x0 0
#define _decimal_0x1 1
#define _decimal_0x2 2
#define _decimal_0x3 3
#define _decimal_0x4 4
#define _decimal_0x5 5
#define _decimal_0x6 6
#define _decimal_0x7 7
#define _decimal_0x8 8
#define _decimal_0x9 9
#define _decimal_0xA 10
#define _decimal_0xB 11
#define _decimal_0xC 12
#define _decimal_0xD 13
#define _decimal_0xE 14
#define _decimal_0xF 15
#define _decimal_0x00 0
#define _decimal_0x01 1
#define _decimal_0x02 2
#define _decimal_0x03 3
#define _decimal_0x04 4
#define _decimal_0x05 5
#define _decimal_0x06 6
#define _decimal_0x07 7
#define _decimal_0x08 8
#define _decimal_0x09 9
#define _decimal_0x0A 10
#define _decimal_0x0B 11
#define _decimal_0x0C 12
#define _decimal_0x0D 13
#define _decimal_0x0E 14
#define _decimal_0x0F 15
#define _decimal_0x10 16
#define _decimal_0x11 17
#define _decimal_0x12 18
#define _decimal_0x13 19
#define _decimal_0x14 20
#define _decimal_0x15 21
#define _decimal_0x16 22
#define _decimal_0x17 23
#define _decimal_0x18 24
#define _decimal_0x19 25
#define _decimal_0x1A 26
#define _decimal_0x1B 27
#define _decimal_0x1C 28
#define _decimal_0x1D 29
#define _decimal_0x1E 30
#define _decimal_0x1F 31
#define _decimal_0x20 32
#define _decimal_0x21 33
#define _decimal_0x22 34
#define _decimal_0x23 35
#define _decimal_0x24 36
#define _decimal_0x25 37
#define _decimal_0x26 38
#define _decimal_0x27 39
#define _decimal_0x28 40
#define _decimal_0x29 41
#define _decimal_0x2A 42
#define _decimal_0x2B 43
#define _decimal_0x2C 44
#define _decimal_0x2D 45
#define _decimal_0x2E 46
#define _decimal_0x2F 47
#define _decimal_0x30 48
#define _decimal_0x31 49
#define _decimal_0x32 50
#define _decimal_0x33 51
#define _decimal_0x34 52
#define _decimal_0x35 53
#define _decimal_0x36 54
#define _decimal_0x37 55
#define _decimal_0x38 56
#define _decimal_0x39 57
#define _decimal_0x3A 58
#define _decimal_0x3B 59
#define _decimal_0x3C 60
#define _decimal_0x3D 61
#define _decimal_0x3E 62
#define _decimal_0x3F 63
#define _decimal_0x40 64

#define __decimal_(HEX) _decimal_ ## HEX
#define _decimal_(HEX) __decimal_(HEX)

#define _hexadecimal_0 0x00
#define _hexadecimal_1 0x01
#define _hexadecimal_2 0x02
#define _hexadecimal_3 0x03
#define _hexadecimal_4 0x04
#define _hexadecimal_5 0x05
#define _hexadecimal_6 0x06
#define _hexadecimal_7 0x07
#define _hexadecimal_8 0x08
#define _hexadecimal_9 0x09
#define _hexadecimal_10 0x0A
#define _hexadecimal_11 0x0B
#define _hexadecimal_12 0x0C
#define _hexadecimal_13 0x0D
#define _hexadecimal_14 0x0E
#define _hexadecimal_15 0x0F
#define _hexadecimal_16 0x10
#define _hexadecimal_17 0x11
#define _hexadecimal_18 0x12
#define _hexadecimal_19 0x13
#define _hexadecimal_20 0x14
#define _hexadecimal_21 0x15
#define _hexadecimal_22 0x16
#define _hexadecimal_23 0x17
#define _hexadecimal_24 0x18
#define _hexadecimal_25 0x19
#define _hexadecimal_26 0x1A
#define _hexadecimal_27 0x1B
#define _hexadecimal_28 0x1C
#define _hexadecimal_29 0x1D
#define _hexadecimal_30 0x1E
#define _hexadecimal_31 0x1F
#define _hexadecimal_32 0x20
#define _hexadecimal_33 0x21
#define _hexadecimal_34 0x22
#define _hexadecimal_35 0x23
#define _hexadecimal_36 0x24
#define _hexadecimal_37 0x25
#define _hexadecimal_38 0x26
#define _hexadecimal_39 0x27
#define _hexadecimal_40 0x28
#define _hexadecimal_41 0x29
#define _hexadecimal_42 0x2A
#define _hexadecimal_43 0x2B
#define _hexadecimal_44 0x2C
#define _hexadecimal_45 0x2D
#define _hexadecimal_46 0x2E
#define _hexadecimal_47 0x2F
#define _hexadecimal_48 0x30
#define _hexadecimal_49 0x31
#define _hexadecimal_50 0x32
#define _hexadecimal_51 0x33
#define _hexadecimal_52 0x34
#define _hexadecimal_53 0x35
#define _hexadecimal_54 0x36
#define _hexadecimal_55 0x37
#define _hexadecimal_56 0x38
#define _hexadecimal_57 0x39
#define _hexadecimal_58 0x3A
#define _hexadecimal_59 0x3B
#define _hexadecimal_60 0x3C
#define _hexadecimal_61 0x3D
#define _hexadecimal_62 0x3E
#define _hexadecimal_63 0x3F
#define _hexadecimal_64 0x40

#define __hexadecimal_(DEC) _hexadecimal_ ## DEC
#define _hexadecimal_(DEC) __hexadecimal_(DEC)

#define _predecessor_1  0 
#define _predecessor_2  1 
#define _predecessor_3  2 
#define _predecessor_4  3 
#define _predecessor_5  4 
#define _predecessor_6  5 
#define _predecessor_7  6 
#define _predecessor_8  7 
#define _predecessor_9  8 
#define _predecessor_10 9 
#define _predecessor_11 10
#define _predecessor_12 11
#define _predecessor_13 12
#define _predecessor_14 13
#define _predecessor_15 14
#define _predecessor_16 15
#define _predecessor_17 16
#define _predecessor_18 17
#define _predecessor_19 18
#define _predecessor_20 19
#define _predecessor_21 20
#define _predecessor_22 21
#define _predecessor_23 22
#define _predecessor_24 23
#define _predecessor_25 24
#define _predecessor_26 25
#define _predecessor_27 26
#define _predecessor_28 27
#define _predecessor_29 28
#define _predecessor_30 29
#define _predecessor_31 30
#define _predecessor_32 31
#define _predecessor_33 32
#define _predecessor_34 33
#define _predecessor_35 34
#define _predecessor_36 35
#define _predecessor_37 36
#define _predecessor_38 37
#define _predecessor_39 38
#define _predecessor_40 39
#define _predecessor_41 40
#define _predecessor_42 41
#define _predecessor_43 42
#define _predecessor_44 43
#define _predecessor_45 44
#define _predecessor_46 45
#define _predecessor_47 46
#define _predecessor_48 47
#define _predecessor_49 48
#define _predecessor_50 49
#define _predecessor_51 50
#define _predecessor_52 51
#define _predecessor_53 52
#define _predecessor_54 53
#define _predecessor_55 54
#define _predecessor_56 55
#define _predecessor_57 56
#define _predecessor_58 57
#define _predecessor_59 58
#define _predecessor_60 59
#define _predecessor_61 60
#define _predecessor_62 61
#define _predecessor_63 62

#define __predecessor(N) _predecessor_ ## N
#define _predecessor(N) __predecessor(N)

#define _call_with_1_1(NAME, ...) (__VA_ARGS__)
#define _call_with_1_0(NAME, _ign) _call_with_1_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_2_2(NAME, ...) (__VA_ARGS__)
#define _call_with_2_1(NAME, ...) _call_with_2_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_2_0(NAME, _ign) _call_with_2_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_3_3(NAME, ...) (__VA_ARGS__)
#define _call_with_3_2(NAME, ...) _call_with_3_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_3_1(NAME, ...) _call_with_3_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_3_0(NAME, _ign) _call_with_3_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_4_4(NAME, ...) (__VA_ARGS__)
#define _call_with_4_3(NAME, ...) _call_with_4_4(NAME, __VA_ARGS__, NAME ## _defarg_ ## 3())
#define _call_with_4_2(NAME, ...) _call_with_4_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_4_1(NAME, ...) _call_with_4_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_4_0(NAME, _ign) _call_with_4_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_5_5(NAME, ...) (__VA_ARGS__)
#define _call_with_5_4(NAME, ...) _call_with_5_5(NAME, __VA_ARGS__, NAME ## _defarg_ ## 4())
#define _call_with_5_3(NAME, ...) _call_with_5_4(NAME, __VA_ARGS__, NAME ## _defarg_ ## 3())
#define _call_with_5_2(NAME, ...) _call_with_5_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_5_1(NAME, ...) _call_with_5_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_5_0(NAME, _ign) _call_with_5_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_6_6(NAME, ...) (__VA_ARGS__)
#define _call_with_6_5(NAME, ...) _call_with_6_6(NAME, __VA_ARGS__, NAME ## _defarg_ ## 5())
#define _call_with_6_4(NAME, ...) _call_with_6_5(NAME, __VA_ARGS__, NAME ## _defarg_ ## 4())
#define _call_with_6_3(NAME, ...) _call_with_6_4(NAME, __VA_ARGS__, NAME ## _defarg_ ## 3())
#define _call_with_6_2(NAME, ...) _call_with_6_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_6_1(NAME, ...) _call_with_6_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_6_0(NAME, _ign) _call_with_6_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_7_7(NAME, ...) (__VA_ARGS__)
#define _call_with_7_6(NAME, ...) _call_with_7_7(NAME, __VA_ARGS__, NAME ## _defarg_ ## 6())
#define _call_with_7_5(NAME, ...) _call_with_7_6(NAME, __VA_ARGS__, NAME ## _defarg_ ## 5())
#define _call_with_7_4(NAME, ...) _call_with_7_5(NAME, __VA_ARGS__, NAME ## _defarg_ ## 4())
#define _call_with_7_3(NAME, ...) _call_with_7_4(NAME, __VA_ARGS__, NAME ## _defarg_ ## 3())
#define _call_with_7_2(NAME, ...) _call_with_7_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_7_1(NAME, ...) _call_with_7_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_7_0(NAME, _ign) _call_with_7_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_8_8(NAME, ...) (__VA_ARGS__)
#define _call_with_8_7(NAME, ...) _call_with_8_8(NAME, __VA_ARGS__, NAME ## _defarg_ ## 7())
#define _call_with_8_6(NAME, ...) _call_with_8_7(NAME, __VA_ARGS__, NAME ## _defarg_ ## 6())
#define _call_with_8_5(NAME, ...) _call_with_8_6(NAME, __VA_ARGS__, NAME ## _defarg_ ## 5())
#define _call_with_8_4(NAME, ...) _call_with_8_5(NAME, __VA_ARGS__, NAME ## _defarg_ ## 4())
#define _call_with_8_3(NAME, ...) _call_with_8_4(NAME, __VA_ARGS__, NAME ## _defarg_ ## 3())
#define _call_with_8_2(NAME, ...) _call_with_8_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_8_1(NAME, ...) _call_with_8_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_8_0(NAME, _ign) _call_with_8_1(NAME, NAME ## _defarg_ ## 0())

#define _call_with_9_9(NAME, ...) (__VA_ARGS__)
#define _call_with_9_8(NAME, ...) _call_with_9_9(NAME, __VA_ARGS__, NAME ## _defarg_ ## 8())
#define _call_with_9_7(NAME, ...) _call_with_9_8(NAME, __VA_ARGS__, NAME ## _defarg_ ## 7())
#define _call_with_9_6(NAME, ...) _call_with_9_7(NAME, __VA_ARGS__, NAME ## _defarg_ ## 6())
#define _call_with_9_5(NAME, ...) _call_with_9_6(NAME, __VA_ARGS__, NAME ## _defarg_ ## 5())
#define _call_with_9_4(NAME, ...) _call_with_9_5(NAME, __VA_ARGS__, NAME ## _defarg_ ## 4())
#define _call_with_9_3(NAME, ...) _call_with_9_4(NAME, __VA_ARGS__, NAME ## _defarg_ ## 3())
#define _call_with_9_2(NAME, ...) _call_with_9_3(NAME, __VA_ARGS__, NAME ## _defarg_ ## 2())
#define _call_with_9_1(NAME, ...) _call_with_9_2(NAME, __VA_ARGS__, NAME ## _defarg_ ## 1())
#define _call_with_9_0(NAME, _ign) _call_with_9_1(NAME, NAME ## _defarg_ ## 0())

#define ___call_with(M, N) _call_with_ ## M ##  _ ## N
#define __call_with(M, N) ___call_with(M, N)

/**
 ** @brief Declare the value @a V and type @a T @a of the M th default
 ** argument for @a NAME.
 **
 ** @param NAME must have been defined as described for
 ** #DEFINE_FSYMB.
 **
 ** @param T Obviously, should coincide with the type of the corresponding
 ** argument that is given in the definition of @a NAME.
 **
 ** @param M The use of this makes only sense if you define it for all possible
 ** @a M to the end of @a NAME's argument list, e.g for 1 and 2 if you
 ** have a function with three arguments (arguments are numbered from
 ** zero upwards). Position @a M as zero, i.e all arguments are
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
inline T NAME ## _defarg_ ## M(void) { return (V); }                    \
enum _dummy_ ## NAME ## _defarg_ ## M { _dummy_ ## NAME ## _defarg_ ## M }

/**
 ** @brief Define the symbols that are declared through a
 ** corresponding call ::declare_defarg.
 **/
#define define_defarg(NAME, M, T)          \
T NAME ## _defarg_ ## M(void)

/** @internal **/
#define FSYMB_(NAME) NAME ## _fsymb_

/** @brief Mangle @a NAME for the use with ::DEFINE_FSYMB
 **
 ** This should only be used in declaration and definition of the
 ** function that is hidden behind the macro @a NAME.
 **/
#define FSYMB(NAME) FSYMB_(NAME)

/**
 ** @brief Provide a documentation section to a function defined with ::DEFINE_FSYMB.
 **/
#define FSYMB_DOCUMENTATION(NAME)                                       \
/*! @see DEFINE_FSYMB */                                                \
/*! @see declare_defarg */                                              \
/*! This is actually implemented as a macro that helps to provide default arguments to the real function. */

/**
 ** @brief Provide a documentation section to a function defined with ::VA_ARGS.
 **/

/* The construct of eating away an empty argument list with `, ## __VA_ARGS__'
   only works for some compilers, namely gcc, icc and IBM. Therefore
   the orwl library itself should not use default arguments for the
   zeroth argument. */
#ifndef NO_ZERO_DEFARG
# define _call_with(M, ...) __call_with(M, _predecessor(_NARG_64(x, ## __VA_ARGS__)))
# define DEFINE_FSYMB(NAME, M, ...)                               \
  FSYMB(NAME)_call_with(M, ## __VA_ARGS__)(NAME, ## __VA_ARGS__)
# define LEN_MODARG(X, ...) _MODARG_(X)(__VA_ARGS__), ## __VA_ARGS__
# define LEN_ARG(...) _MODARG_(1)(__VA_ARGS__), ## __VA_ARGS__
# define CALL_THE_FUNC(NAME, MAX, ...) NAME _call_with(MAX, ## __VA_ARGS__)(NAME, ## __VA_ARGS__)
#else
# define _call_with(M, ...) __call_with(M, _predecessor(_NARG_64(x, __VA_ARGS__)))
# define DEFINE_FSYMB(NAME, M, ...)                              \
  FSYMB(NAME)_call_with(M, __VA_ARGS__)(NAME, __VA_ARGS__)
# define LEN_MODARG(X, ...) _MODARG_(X)(__VA_ARGS__), __VA_ARGS__
# define LEN_ARG(...) _MODARG_(1)(__VA_ARGS__), __VA_ARGS__
# define CALL_THE_FUNC(NAME, MAX, ...) NAME _call_with(MAX, __VA_ARGS__)(NAME, __VA_ARGS__)
#endif

/**
 ** @def DEFINE_FSYMB
 ** @brief Define a replacement macro for functions that can provide
 ** default arguments to the underlying real function.
 **
 ** The easiest is to explain this with an example
 ** @code
 ** void FSYMB(size_t_init)(size_t *size, size_t def) {
 **   *size = def;
 ** }
 **
 ** #define size_t_init(...) DEFINE_FSYMB(size_t_init, 2, __VA_ARGS__)
 ** declare_defarg(size_t_init, 1, size_t, 0);
 ** @endcode
 **
 ** This declares a macro @c size_t_init that resolves to the call of
 ** a real function (hidden behind a mangled name provided by
 ** FSYMB(size_t_init)) to initialize a @c size_t. If invoked
 ** with two arguments, these should be a pointer to the variable and
 ** an initialization value. If this initialization value is omitted
 ** the default value of @c 0 is used. Valid use is
 ** @code
 ** size_t i;
 ** size_t_init(&i, 87);
 ** size_t_init(&i);
 ** @endcode
 ** @param NAME is the function to provide with default argument features.
 ** @param M is the number of arguments that a full call to @a NAME takes.
 ** @see declare_defarg
 **/

/**
 ** @def CALL_THE_FUNC
 ** @brief Define a replacement macro for functions that can provide
 ** default arguments to the underlying real function.
 **
 ** This is similar to ::DEFINE_FSYMB, only that it should be used for
 ** pre-existing function (e.g system functions).
 **
 ** The easiest is to explain this with an example. The system
 ** function @c pthread_mutex_init is defined as follows:
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
 ** #define pthread_mutex_init(...) CALL_THE_FUNC(pthread_mutex_init, 2, __VA_ARGS__)
 ** declare_defarg(pthread_mutex_init, 1, pthread_mutexattr_t*, NULL);
 ** @endcode
 **
 ** This declares a macro @c pthread_mutex_init that resolves to the call of
 ** a real function to initialize a @c pthread_mutexattr_t*. If invoked
 ** with two arguments, this should be the same types of value as to a
 ** direct call of @c pthread_mutex_init. If this initialization value is omitted
 ** the default value of @c NULL is used. Valid use is
 ** @code
 ** static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
 ** static pthread_mutexattr_t attr;
 ** .
 ** pthread_mutex_init(&mut, &attr); // full specification with attributes
 ** pthread_mutex_init(&mut, NULL);  // still a valid form
 ** pthread_mutex_init(&mut);        // easy variant for everyday use
 ** @endcode
 ** @param NAME is the function to provide with default argument features.
 ** @param M is the number of arguments that a full call to @a NAME takes.
 ** @see declare_defarg
 **/

/**
 ** @brief Define an (almost) all purpose initializer
 **/
#define INITIALIZER INIT1

#define INIT0 (0)
#define INIT1 { 0 }
#define INIT2 { { 0 } }

/**
 ** @brief Define a @c NULL initialized constant of type @a T
 **/
#define TNULL NULL1

#define NULL0(T) ((T)INIT0)
#define NULL1(T) ((T)INIT1)
#define NULL2(T) ((T)INIT2)

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

#endif 	    /* !ORWL_MACRO_H_ */

