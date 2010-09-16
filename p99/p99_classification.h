/*
** p99_classification.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Thu Sep 16 11:48:55 2010 Jens Gustedt
** Last update Thu Sep 16 11:48:55 2010 Jens Gustedt
*/

#ifndef   	P99_CLASSIFICATION_H_
# define   	P99_CLASSIFICATION_H_

#include "p99_map.h"

/**
 ** @file
 ** @brief classification of different types of token sequences
 **
 ** @warning this will not be handling imaginary types with the
 ** _Imaginary keyword since the handling of those has been revised by
 ** a <a
 ** href="http://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_207.htm">defect report>/a>
 **/

/**
 ** @addtogroup classification
 ** @brief The macros in this group help to classify different token
 ** sequences, in particular type specifiers that are composed of
 ** several tokens.
 ** @{
 **/



#define	P99__BUILTIN_TYPE_	P99__BUILTIN_TYPE_i
#define	P99__BUILTIN_TYPE_V	P99__BUILTIN_TYPE_v
#define P99__BUILTIN_TYPE_B	P99__BUILTIN_TYPE_b
#define P99__BUILTIN_TYPE_C	P99__BUILTIN_TYPE_c
#define P99__BUILTIN_TYPE_D	P99__BUILTIN_TYPE_d
#define P99__BUILTIN_TYPE_DC	P99__BUILTIN_TYPE_dc
#define P99__BUILTIN_TYPE_DI	P99__BUILTIN_TYPE_di
#define P99__BUILTIN_TYPE_Dc	P99__BUILTIN_TYPE_dc
/* #define P99__BUILTIN_TYPE_Di	P99__BUILTIN_TYPE_di */
#define P99__BUILTIN_TYPE_F	P99__BUILTIN_TYPE_f
#define P99__BUILTIN_TYPE_FC	P99__BUILTIN_TYPE_fc
#define P99__BUILTIN_TYPE_FI	P99__BUILTIN_TYPE_fi
#define P99__BUILTIN_TYPE_Fc	P99__BUILTIN_TYPE_fc
/* #define P99__BUILTIN_TYPE_Fi	P99__BUILTIN_TYPE_fi */
#define P99__BUILTIN_TYPE_H	P99__BUILTIN_TYPE_h
#define P99__BUILTIN_TYPE_HH	P99__BUILTIN_TYPE_hh
#define P99__BUILTIN_TYPE_Hh	P99__BUILTIN_TYPE_hh
#define P99__BUILTIN_TYPE_I	P99__BUILTIN_TYPE_i
#define P99__BUILTIN_TYPE_L	P99__BUILTIN_TYPE_l
#define P99__BUILTIN_TYPE_LD	P99__BUILTIN_TYPE_ld
#define P99__BUILTIN_TYPE_LDC	P99__BUILTIN_TYPE_ldc
#define P99__BUILTIN_TYPE_LDI	P99__BUILTIN_TYPE_ldi
#define P99__BUILTIN_TYPE_LDc	P99__BUILTIN_TYPE_ldc
/* #define P99__BUILTIN_TYPE_LDi	P99__BUILTIN_TYPE_ldi */
#define P99__BUILTIN_TYPE_LL	P99__BUILTIN_TYPE_ll
#define P99__BUILTIN_TYPE_Ld	P99__BUILTIN_TYPE_ld
#define P99__BUILTIN_TYPE_LdC	P99__BUILTIN_TYPE_ldc
/* #define P99__BUILTIN_TYPE_LdI	P99__BUILTIN_TYPE_ldi */
#define P99__BUILTIN_TYPE_Ldc	P99__BUILTIN_TYPE_ldc
/* #define P99__BUILTIN_TYPE_Ldi	P99__BUILTIN_TYPE_ldi */
#define P99__BUILTIN_TYPE_Ll	P99__BUILTIN_TYPE_ll
#define P99__BUILTIN_TYPE_U	P99__BUILTIN_TYPE_u
#define P99__BUILTIN_TYPE_UH	P99__BUILTIN_TYPE_uh
#define P99__BUILTIN_TYPE_UHH	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_UHh	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_UL	P99__BUILTIN_TYPE_ul
#define P99__BUILTIN_TYPE_ULL	P99__BUILTIN_TYPE_ull
#define P99__BUILTIN_TYPE_ULl	P99__BUILTIN_TYPE_ull
#define P99__BUILTIN_TYPE_Uh	P99__BUILTIN_TYPE_uh
#define P99__BUILTIN_TYPE_UhH	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_Uhh	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_Ul	P99__BUILTIN_TYPE_ul
#define P99__BUILTIN_TYPE_UlL	P99__BUILTIN_TYPE_ull
#define P99__BUILTIN_TYPE_Ull	P99__BUILTIN_TYPE_ull
#define P99__BUILTIN_TYPE_dC	P99__BUILTIN_TYPE_dc
/* #define P99__BUILTIN_TYPE_dI	P99__BUILTIN_TYPE_di */
#define P99__BUILTIN_TYPE_fC	P99__BUILTIN_TYPE_fc
/* #define P99__BUILTIN_TYPE_fI	P99__BUILTIN_TYPE_fi */
#define P99__BUILTIN_TYPE_hH	P99__BUILTIN_TYPE_hh
#define P99__BUILTIN_TYPE_lD	P99__BUILTIN_TYPE_ld
#define P99__BUILTIN_TYPE_lDC	P99__BUILTIN_TYPE_ldc
/* #define P99__BUILTIN_TYPE_lDI	P99__BUILTIN_TYPE_ldi */
#define P99__BUILTIN_TYPE_lDc	P99__BUILTIN_TYPE_ldc
/* #define P99__BUILTIN_TYPE_lDi	P99__BUILTIN_TYPE_ldi */
#define P99__BUILTIN_TYPE_lL	P99__BUILTIN_TYPE_ll
#define P99__BUILTIN_TYPE_ldC	P99__BUILTIN_TYPE_ldc
/* #define P99__BUILTIN_TYPE_ldI	P99__BUILTIN_TYPE_ldi */
#define P99__BUILTIN_TYPE_uH	P99__BUILTIN_TYPE_uh
#define P99__BUILTIN_TYPE_uHH	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_uHh	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_uL	P99__BUILTIN_TYPE_ul
#define P99__BUILTIN_TYPE_uLL	P99__BUILTIN_TYPE_ull
#define P99__BUILTIN_TYPE_uLl	P99__BUILTIN_TYPE_ull
#define P99__BUILTIN_TYPE_uhH	P99__BUILTIN_TYPE_uhh
#define P99__BUILTIN_TYPE_ulL	P99__BUILTIN_TYPE_ull


/**
 ** @brief Find a builtin type according to a code such as `ull' or `ld'
 **
 ** E.g ::P99_BUILTIN_TYPE(ull) should expand to @c unsigned @c long
 ** @c long.
 **/
#define  P99_BUILTIN_TYPE(CODE)  P99_PASTE2(P99__BUILTIN, P99_PASTE2(_TYPE_, CODE))

#define P99__TYPE_NORMALIZE_(CODE, CONS, VOLA, SEQ)     \
P99_IF_EMPTY(CODE)                                      \
  (P99_IF_EMPTY(SEQ)(void)(SEQ))                        \
  (P99_BUILTIN_TYPE(CODE)                               \
   CONS                                                 \
   VOLA                                                 \
  )

#define P99__TYPE_NORMALIZE(N, CODE, SEQ)               \
P99__TYPE_NORMALIZE_(CODE,                              \
                     P99__CONST_CLASSIFY_(N, SEQ),      \
                     P99__VOLATILE_CLASSIFY_(N, SEQ),   \
                     SEQ                                \
                     )
/**
 ** @brief Normalize a @c void type expression.
 **
 ** @param SEQ is a sequence of keywords that should form a qualified
 ** void type expression
 ** @return @c void, followed by the qualifiers
 **/
#define P99_VOID_NORMALIZE(SEQ)                                        \
  P99__TYPE_NORMALIZE(P99_VOID_QUAL_LEN(SEQ), P99__VOID_CLASSIFY(SEQ), SEQ)

/**
 ** @brief Normalize a Boolean type expression.
 **
 ** @param SEQ is a sequence of keywords that should form a qualified
 ** Boolean type expression
 ** @return @c _Bool followed by the qualifiers
 **/
#define P99_BOOL_NORMALIZE(SEQ)                                        \
  P99__TYPE_NORMALIZE(P99_BOOL_QUAL_LEN(SEQ), P99__BOOL_CLASSIFY(SEQ), SEQ)

/**
 ** @brief Normalize an integer type expression.
 **
 ** @param SEQ is a sequence of keywords that should form a qualified
 ** integer type expression.
 **
 ** This normalizes the type by removing redundancy, such as @c int.
 **
 ** @return the normalized unqualified type, followed by the qualifiers
 **/
#define P99_INTEGER_NORMALIZE(SEQ)                                        \
  P99__TYPE_NORMALIZE(P99_INTEGER_QUAL_LEN(SEQ), P99__INTEGER_CLASSIFY(SEQ), SEQ)

/**
 ** @brief Normalize an floating type expression, including @c
 ** _Complex types.
 **
 ** @param SEQ is a sequence of keywords that should form a qualified
 ** floating type expression.
 **
 ** This normalizes the type by reordering the keywords.
 **
 ** @return the normalized unqualified type, followed by the qualifiers
 **/
#define P99_FLOAT_NORMALIZE(SEQ)                                        \
  P99__TYPE_NORMALIZE(P99_FLOAT_QUAL_LEN(SEQ), P99__FLOAT_CLASSIFY(SEQ), SEQ)


/**
 ** @}
 **/



#endif 	    /* !P99_CLASSIFICATION_H_ */
