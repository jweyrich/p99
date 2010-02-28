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
__NARG_64(__VA_ARGS__,                                                  \
          0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, \
          0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, \
          0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, \
          0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)

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
__NARG_64(__VA_ARGS__,                                                  \
          _INV(2), 0x1F, _INV(2), 0x1E, _INV(2), 0x1D, _INV(2), 0x1C,   \
          _INV(2), 0x1B, _INV(2), 0x1A, _INV(2), 0x19, _INV(2), 0x18,   \
          _INV(2), 0x17, _INV(2), 0x16, _INV(2), 0x15, _INV(2), 0x14,   \
          _INV(2), 0x13, _INV(2), 0x12, _INV(2), 0x11, _INV(2), 0x10,   \
          _INV(2), 0x0F, _INV(2), 0x0E, _INV(2), 0x0D, _INV(2), 0x0C,   \
          _INV(2), 0x0B, _INV(2), 0x0A, _INV(2), 0x09, _INV(2), 0x08,   \
          _INV(2), 0x07, _INV(2), 0x06, _INV(2), 0x05, _INV(2), 0x04,   \
          _INV(2), 0x03, _INV(2), 0x02, _INV(2), 0x01, _INV(2), 0x00)

/**
 ** @brief Return the number of triples of the variate argument list.
 **
 ** This supposes that the total length is less than 64.
 **
 ** If the length of the list is not divisible by 3, a compile time error occurs.
 **/
#define _NARG_64_3(...)                                                 \
__NARG_64(__VA_ARGS__,                                                  \
          0x16,                                                         \
          _INV(3), _INV(3), 0x15, _INV(3), _INV(3), 0x14, _INV(3), _INV(3), 0x13, \
          _INV(3), _INV(3), 0x12, _INV(3), _INV(3), 0x11, _INV(3), _INV(3), 0x10, \
          _INV(3), _INV(3), 0x0F, _INV(3), _INV(3), 0x0D, _INV(3), _INV(3), 0x0C, \
          _INV(3), _INV(3), 0x0B, _INV(3), _INV(3), 0x0A, _INV(3), _INV(3), 0x09, \
          _INV(3), _INV(3), 0x08, _INV(3), _INV(3), 0x07, _INV(3), _INV(3), 0x06, \
          _INV(3), _INV(3), 0x05, _INV(3), _INV(3), 0x04, _INV(3), _INV(3), 0x03, \
          _INV(3), _INV(3), 0x02, _INV(3), _INV(3), 0x01, _INV(3), _INV(3), 0x00)

/**
 ** @brief Return the number of quadruples of the variate argument list.
 **
 ** This supposes that the total length is less than 64.
 **
 ** If the length of the list is not divisible by 4, a compile time error occurs.
 **/
#define _NARG_64_4(...)                                                 \
__NARG_64(__VA_ARGS__,                                                  \
          _INV(4), _INV(4), _INV(4), 0xF, _INV(4), _INV(4), _INV(4), 0xE, \
          _INV(4), _INV(4), _INV(4), 0xD, _INV(4), _INV(4), _INV(4), 0xC, \
          _INV(4), _INV(4), _INV(4), 0xB, _INV(4), _INV(4), _INV(4), 0xA, \
          _INV(4), _INV(4), _INV(4), 0x9, _INV(4), _INV(4), _INV(4), 0x8, \
          _INV(4), _INV(4), _INV(4), 0x7, _INV(4), _INV(4), _INV(4), 0x6, \
          _INV(4), _INV(4), _INV(4), 0x5, _INV(4), _INV(4), _INV(4), 0x4, \
          _INV(4), _INV(4), _INV(4), 0x3, _INV(4), _INV(4), _INV(4), 0x2, \
          _INV(4), _INV(4), _INV(4), 0x1, _INV(4), _INV(4), _INV(4), 0x0)

#define __NARG_64(...) _ARG_64(__VA_ARGS__)

#define _MODARG_(_X) _NARG_64_ ## _X

/**
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It replaces @a X by
 ** the number of X-tuples in the following list of arguments.
 **/
#define LEN_MODARG(X, ...) _MODARG_(X)(__VA_ARGS__), __VA_ARGS__

/**
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It prefixes the
 ** list of arguments by an integer constant containing the length of
 ** the list.
 **/
#define LEN_ARG(...) _MODARG_(1)(__VA_ARGS__), __VA_ARGS__


/**
 ** @brief Helper macro to declare a variable length parameter list.
 **
 ** Inside the declared function @a X will of @c size_t and should
 ** hold the actual length of the list. It can be used as the argument
 ** to @c va_start.
 **
 ** Wrap your function into a macro that uses LEN_ARG. If used through
 ** that macro, the correct value for @a X will always be provided at
 ** compile time. 
 **/
#define VA_ARGS(X) size_t X, ...

#endif 	    /* !ORWL_MACRO_H_ */
