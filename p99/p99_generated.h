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
/** @file
 ** @brief automatically generated macros to handle variadic macros.
 **
 ** You should probably never include this file directly but through
 ** other P99 header files that define macros that are of direct use.
 **/

/** @brief The version of the P99 collection indicated by the commit date.
 **
 ** This should appear as an RFC2822 style date as you encounter e.g in emails.
 ** If this doesn't contain a date but a bizarre `format' with $ signs
 ** you have a version that is directly taken from the git repostitory.
 **/
#define P99_VERSION_DATE "$Format:%cd$"

/** @brief The version of the P99 collection indicated by a unique commit ID.
 **
 ** This should appear as a long string of hexadecimal digits.
 ** If this contains but a bizarre "format" with $ signs
 ** you have a version that is directly taken from the git repostitory.
 **/
#define P99_VERSION_ID "$Format:%H$"

#define P99__VERSION_DATE "Thu, 16 Sep 2010 23:05:23 +0200"
#define P99__VERSION_ID "eb534f1d31aa3a23309b77734f874d521c3cb3a2"

/** @brief The maximal number of arguments the P99 macros can handle.
 **
 ** This limit applies also to the integer arithmetic that is
 ** performed by the macros in @ref preprocessor_arithmetic.
 **/
#define P99_MAX_NUMBER 121
#define P99__ARG(                                              \
	_1,	_2,	_3,	_4,	_5,	_6,	_7,	_8,                               \
	_9,	_10,	_11,	_12,	_13,	_14,	_15,	_16,                        \
	_17,	_18,	_19,	_20,	_21,	_22,	_23,	_24,                       \
	_25,	_26,	_27,	_28,	_29,	_30,	_31,	_32,                       \
	_33,	_34,	_35,	_36,	_37,	_38,	_39,	_40,                       \
	_41,	_42,	_43,	_44,	_45,	_46,	_47,	_48,                       \
	_49,	_50,	_51,	_52,	_53,	_54,	_55,	_56,                       \
	_57,	_58,	_59,	_60,	_61,	_62,	_63,	_64,                       \
	_65,	_66,	_67,	_68,	_69,	_70,	_71,	_72,                       \
	_73,	_74,	_75,	_76,	_77,	_78,	_79,	_80,                       \
	_81,	_82,	_83,	_84,	_85,	_86,	_87,	_88,                       \
	_89,	_90,	_91,	_92,	_93,	_94,	_95,	_96,                       \
	_97,	_98,	_99,	_100,	_101,	_102,	_103,	_104,                  \
	_105,	_106,	_107,	_108,	_109,	_110,	_111,	_112,               \
	_113,	_114,	_115,	_116,	_117,	_118,	_119,	_120,               \
	_121,                                                         \
	...) _121
#define P99__NARG_1(...) P99__ARG(__VA_ARGS__, 	120,           \
	119,	118,	117,	116,	115,	114,	113,	112,                       \
	111,	110,	109,	108,	107,	106,	105,	104,                       \
	103,	102,	101,	100,	99,	98,	97,	96,                           \
	95,	94,	93,	92,	91,	90,	89,	88,                               \
	87,	86,	85,	84,	83,	82,	81,	80,                               \
	79,	78,	77,	76,	75,	74,	73,	72,                               \
	71,	70,	69,	68,	67,	66,	65,	64,                               \
	63,	62,	61,	60,	59,	58,	57,	56,                               \
	55,	54,	53,	52,	51,	50,	49,	48,                               \
	47,	46,	45,	44,	43,	42,	41,	40,                               \
	39,	38,	37,	36,	35,	34,	33,	32,                               \
	31,	30,	29,	28,	27,	26,	25,	24,                               \
	23,	22,	21,	20,	19,	18,	17,	16,                               \
	15,	14,	13,	12,	11,	10,	9,	8,                                 \
	7,	6,	5,	4,	3,	2,	1,	0,)
#define P99__NARG_2(...) P99__ARG(__VA_ARGS__, 	60,                  \
	P99__INV(2),	59,	P99__INV(2),	58,	P99__INV(2),	57,	P99__INV(2),	56, \
	P99__INV(2),	55,	P99__INV(2),	54,	P99__INV(2),	53,	P99__INV(2),	52, \
	P99__INV(2),	51,	P99__INV(2),	50,	P99__INV(2),	49,	P99__INV(2),	48, \
	P99__INV(2),	47,	P99__INV(2),	46,	P99__INV(2),	45,	P99__INV(2),	44, \
	P99__INV(2),	43,	P99__INV(2),	42,	P99__INV(2),	41,	P99__INV(2),	40, \
	P99__INV(2),	39,	P99__INV(2),	38,	P99__INV(2),	37,	P99__INV(2),	36, \
	P99__INV(2),	35,	P99__INV(2),	34,	P99__INV(2),	33,	P99__INV(2),	32, \
	P99__INV(2),	31,	P99__INV(2),	30,	P99__INV(2),	29,	P99__INV(2),	28, \
	P99__INV(2),	27,	P99__INV(2),	26,	P99__INV(2),	25,	P99__INV(2),	24, \
	P99__INV(2),	23,	P99__INV(2),	22,	P99__INV(2),	21,	P99__INV(2),	20, \
	P99__INV(2),	19,	P99__INV(2),	18,	P99__INV(2),	17,	P99__INV(2),	16, \
	P99__INV(2),	15,	P99__INV(2),	14,	P99__INV(2),	13,	P99__INV(2),	12, \
	P99__INV(2),	11,	P99__INV(2),	10,	P99__INV(2),	9,	P99__INV(2),	8,   \
	P99__INV(2),	7,	P99__INV(2),	6,	P99__INV(2),	5,	P99__INV(2),	4,     \
	P99__INV(2),	3,	P99__INV(2),	2,	P99__INV(2),	1,	P99__INV(2),	0,)
#define P99__NARG_3(...) P99__ARG(__VA_ARGS__, 	40,                                    \
	P99__INV(3),	P99__INV(3),	39,	P99__INV(3),	P99__INV(3),	38,	P99__INV(3),	P99__INV(3), \
	37,	P99__INV(3),	P99__INV(3),	36,	P99__INV(3),	P99__INV(3),	35,	P99__INV(3),          \
	P99__INV(3),	34,	P99__INV(3),	P99__INV(3),	33,	P99__INV(3),	P99__INV(3),	32,          \
	P99__INV(3),	P99__INV(3),	31,	P99__INV(3),	P99__INV(3),	30,	P99__INV(3),	P99__INV(3), \
	29,	P99__INV(3),	P99__INV(3),	28,	P99__INV(3),	P99__INV(3),	27,	P99__INV(3),          \
	P99__INV(3),	26,	P99__INV(3),	P99__INV(3),	25,	P99__INV(3),	P99__INV(3),	24,          \
	P99__INV(3),	P99__INV(3),	23,	P99__INV(3),	P99__INV(3),	22,	P99__INV(3),	P99__INV(3), \
	21,	P99__INV(3),	P99__INV(3),	20,	P99__INV(3),	P99__INV(3),	19,	P99__INV(3),          \
	P99__INV(3),	18,	P99__INV(3),	P99__INV(3),	17,	P99__INV(3),	P99__INV(3),	16,          \
	P99__INV(3),	P99__INV(3),	15,	P99__INV(3),	P99__INV(3),	14,	P99__INV(3),	P99__INV(3), \
	13,	P99__INV(3),	P99__INV(3),	12,	P99__INV(3),	P99__INV(3),	11,	P99__INV(3),          \
	P99__INV(3),	10,	P99__INV(3),	P99__INV(3),	9,	P99__INV(3),	P99__INV(3),	8,            \
	P99__INV(3),	P99__INV(3),	7,	P99__INV(3),	P99__INV(3),	6,	P99__INV(3),	P99__INV(3),   \
	5,	P99__INV(3),	P99__INV(3),	4,	P99__INV(3),	P99__INV(3),	3,	P99__INV(3),             \
	P99__INV(3),	2,	P99__INV(3),	P99__INV(3),	1,	P99__INV(3),	P99__INV(3),	0,)
#define P99__NARG_4(...) P99__ARG(__VA_ARGS__, 	30,                                    \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	29,	P99__INV(4),	P99__INV(4),	P99__INV(4),	28, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	27,	P99__INV(4),	P99__INV(4),	P99__INV(4),	26, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	25,	P99__INV(4),	P99__INV(4),	P99__INV(4),	24, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	23,	P99__INV(4),	P99__INV(4),	P99__INV(4),	22, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	21,	P99__INV(4),	P99__INV(4),	P99__INV(4),	20, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	19,	P99__INV(4),	P99__INV(4),	P99__INV(4),	18, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	17,	P99__INV(4),	P99__INV(4),	P99__INV(4),	16, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	15,	P99__INV(4),	P99__INV(4),	P99__INV(4),	14, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	13,	P99__INV(4),	P99__INV(4),	P99__INV(4),	12, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	11,	P99__INV(4),	P99__INV(4),	P99__INV(4),	10, \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	9,	P99__INV(4),	P99__INV(4),	P99__INV(4),	8,   \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	7,	P99__INV(4),	P99__INV(4),	P99__INV(4),	6,   \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	5,	P99__INV(4),	P99__INV(4),	P99__INV(4),	4,   \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	3,	P99__INV(4),	P99__INV(4),	P99__INV(4),	2,   \
	P99__INV(4),	P99__INV(4),	P99__INV(4),	1,	P99__INV(4),	P99__INV(4),	P99__INV(4),	0,)
/** @ingroup basic_list_operations                                                 */
/** @brief Determine of the argument list has a comma, i.e at least two arguments. */
#define P99_HAS_COMMA(...) P99__ARG(__VA_ARGS__,               \
	1,	1,	1,	1,	1,	1,	1,                                          \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,	0, ...)
#define P99__SKP2(_0, ...) P99__SKP1(__VA_ARGS__)
#define P99__SKP3(_0, ...) P99__SKP2(__VA_ARGS__)
#define P99__SKP4(_0, ...) P99__SKP3(__VA_ARGS__)
#define P99__SKP5(_0, ...) P99__SKP4(__VA_ARGS__)
#define P99__SKP6(_0, ...) P99__SKP5(__VA_ARGS__)
#define P99__SKP7(_0, ...) P99__SKP6(__VA_ARGS__)
#define P99__SKP8(_0, ...) P99__SKP7(__VA_ARGS__)
#define P99__SKP9(_0, ...) P99__SKP8(__VA_ARGS__)
#define P99__SKP10(_0, ...) P99__SKP9(__VA_ARGS__)
#define P99__SKP11(_0, ...) P99__SKP10(__VA_ARGS__)
#define P99__SKP12(_0, ...) P99__SKP11(__VA_ARGS__)
#define P99__SKP13(_0, ...) P99__SKP12(__VA_ARGS__)
#define P99__SKP14(_0, ...) P99__SKP13(__VA_ARGS__)
#define P99__SKP15(_0, ...) P99__SKP14(__VA_ARGS__)
#define P99__SKP16(_0, ...) P99__SKP15(__VA_ARGS__)
#define P99__SKP17(_0, ...) P99__SKP16(__VA_ARGS__)
#define P99__SKP18(_0, ...) P99__SKP17(__VA_ARGS__)
#define P99__SKP19(_0, ...) P99__SKP18(__VA_ARGS__)
#define P99__SKP20(_0, ...) P99__SKP19(__VA_ARGS__)
#define P99__SKP21(_0, ...) P99__SKP20(__VA_ARGS__)
#define P99__SKP22(_0, ...) P99__SKP21(__VA_ARGS__)
#define P99__SKP23(_0, ...) P99__SKP22(__VA_ARGS__)
#define P99__SKP24(_0, ...) P99__SKP23(__VA_ARGS__)
#define P99__SKP25(_0, ...) P99__SKP24(__VA_ARGS__)
#define P99__SKP26(_0, ...) P99__SKP25(__VA_ARGS__)
#define P99__SKP27(_0, ...) P99__SKP26(__VA_ARGS__)
#define P99__SKP28(_0, ...) P99__SKP27(__VA_ARGS__)
#define P99__SKP29(_0, ...) P99__SKP28(__VA_ARGS__)
#define P99__SKP30(_0, ...) P99__SKP29(__VA_ARGS__)
#define P99__SKP31(_0, ...) P99__SKP30(__VA_ARGS__)
#define P99__SKP32(_0, ...) P99__SKP31(__VA_ARGS__)
#define P99__SKP33(_0, ...) P99__SKP32(__VA_ARGS__)
#define P99__SKP34(_0, ...) P99__SKP33(__VA_ARGS__)
#define P99__SKP35(_0, ...) P99__SKP34(__VA_ARGS__)
#define P99__SKP36(_0, ...) P99__SKP35(__VA_ARGS__)
#define P99__SKP37(_0, ...) P99__SKP36(__VA_ARGS__)
#define P99__SKP38(_0, ...) P99__SKP37(__VA_ARGS__)
#define P99__SKP39(_0, ...) P99__SKP38(__VA_ARGS__)
#define P99__SKP40(_0, ...) P99__SKP39(__VA_ARGS__)
#define P99__SKP41(_0, ...) P99__SKP40(__VA_ARGS__)
#define P99__SKP42(_0, ...) P99__SKP41(__VA_ARGS__)
#define P99__SKP43(_0, ...) P99__SKP42(__VA_ARGS__)
#define P99__SKP44(_0, ...) P99__SKP43(__VA_ARGS__)
#define P99__SKP45(_0, ...) P99__SKP44(__VA_ARGS__)
#define P99__SKP46(_0, ...) P99__SKP45(__VA_ARGS__)
#define P99__SKP47(_0, ...) P99__SKP46(__VA_ARGS__)
#define P99__SKP48(_0, ...) P99__SKP47(__VA_ARGS__)
#define P99__SKP49(_0, ...) P99__SKP48(__VA_ARGS__)
#define P99__SKP50(_0, ...) P99__SKP49(__VA_ARGS__)
#define P99__SKP51(_0, ...) P99__SKP50(__VA_ARGS__)
#define P99__SKP52(_0, ...) P99__SKP51(__VA_ARGS__)
#define P99__SKP53(_0, ...) P99__SKP52(__VA_ARGS__)
#define P99__SKP54(_0, ...) P99__SKP53(__VA_ARGS__)
#define P99__SKP55(_0, ...) P99__SKP54(__VA_ARGS__)
#define P99__SKP56(_0, ...) P99__SKP55(__VA_ARGS__)
#define P99__SKP57(_0, ...) P99__SKP56(__VA_ARGS__)
#define P99__SKP58(_0, ...) P99__SKP57(__VA_ARGS__)
#define P99__SKP59(_0, ...) P99__SKP58(__VA_ARGS__)
#define P99__SKP60(_0, ...) P99__SKP59(__VA_ARGS__)
#define P99__SKP61(_0, ...) P99__SKP60(__VA_ARGS__)
#define P99__SKP62(_0, ...) P99__SKP61(__VA_ARGS__)
#define P99__SKP63(_0, ...) P99__SKP62(__VA_ARGS__)
#define P99__SKP64(_0, ...) P99__SKP63(__VA_ARGS__)
#define P99__SKP65(_0, ...) P99__SKP64(__VA_ARGS__)
#define P99__SKP66(_0, ...) P99__SKP65(__VA_ARGS__)
#define P99__SKP67(_0, ...) P99__SKP66(__VA_ARGS__)
#define P99__SKP68(_0, ...) P99__SKP67(__VA_ARGS__)
#define P99__SKP69(_0, ...) P99__SKP68(__VA_ARGS__)
#define P99__SKP70(_0, ...) P99__SKP69(__VA_ARGS__)
#define P99__SKP71(_0, ...) P99__SKP70(__VA_ARGS__)
#define P99__SKP72(_0, ...) P99__SKP71(__VA_ARGS__)
#define P99__SKP73(_0, ...) P99__SKP72(__VA_ARGS__)
#define P99__SKP74(_0, ...) P99__SKP73(__VA_ARGS__)
#define P99__SKP75(_0, ...) P99__SKP74(__VA_ARGS__)
#define P99__SKP76(_0, ...) P99__SKP75(__VA_ARGS__)
#define P99__SKP77(_0, ...) P99__SKP76(__VA_ARGS__)
#define P99__SKP78(_0, ...) P99__SKP77(__VA_ARGS__)
#define P99__SKP79(_0, ...) P99__SKP78(__VA_ARGS__)
#define P99__SKP80(_0, ...) P99__SKP79(__VA_ARGS__)
#define P99__SKP81(_0, ...) P99__SKP80(__VA_ARGS__)
#define P99__SKP82(_0, ...) P99__SKP81(__VA_ARGS__)
#define P99__SKP83(_0, ...) P99__SKP82(__VA_ARGS__)
#define P99__SKP84(_0, ...) P99__SKP83(__VA_ARGS__)
#define P99__SKP85(_0, ...) P99__SKP84(__VA_ARGS__)
#define P99__SKP86(_0, ...) P99__SKP85(__VA_ARGS__)
#define P99__SKP87(_0, ...) P99__SKP86(__VA_ARGS__)
#define P99__SKP88(_0, ...) P99__SKP87(__VA_ARGS__)
#define P99__SKP89(_0, ...) P99__SKP88(__VA_ARGS__)
#define P99__SKP90(_0, ...) P99__SKP89(__VA_ARGS__)
#define P99__SKP91(_0, ...) P99__SKP90(__VA_ARGS__)
#define P99__SKP92(_0, ...) P99__SKP91(__VA_ARGS__)
#define P99__SKP93(_0, ...) P99__SKP92(__VA_ARGS__)
#define P99__SKP94(_0, ...) P99__SKP93(__VA_ARGS__)
#define P99__SKP95(_0, ...) P99__SKP94(__VA_ARGS__)
#define P99__SKP96(_0, ...) P99__SKP95(__VA_ARGS__)
#define P99__SKP97(_0, ...) P99__SKP96(__VA_ARGS__)
#define P99__SKP98(_0, ...) P99__SKP97(__VA_ARGS__)
#define P99__SKP99(_0, ...) P99__SKP98(__VA_ARGS__)
#define P99__SKP100(_0, ...) P99__SKP99(__VA_ARGS__)
#define P99__SKP101(_0, ...) P99__SKP100(__VA_ARGS__)
#define P99__SKP102(_0, ...) P99__SKP101(__VA_ARGS__)
#define P99__SKP103(_0, ...) P99__SKP102(__VA_ARGS__)
#define P99__SKP104(_0, ...) P99__SKP103(__VA_ARGS__)
#define P99__SKP105(_0, ...) P99__SKP104(__VA_ARGS__)
#define P99__SKP106(_0, ...) P99__SKP105(__VA_ARGS__)
#define P99__SKP107(_0, ...) P99__SKP106(__VA_ARGS__)
#define P99__SKP108(_0, ...) P99__SKP107(__VA_ARGS__)
#define P99__SKP109(_0, ...) P99__SKP108(__VA_ARGS__)
#define P99__SKP110(_0, ...) P99__SKP109(__VA_ARGS__)
#define P99__SKP111(_0, ...) P99__SKP110(__VA_ARGS__)
#define P99__SKP112(_0, ...) P99__SKP111(__VA_ARGS__)
#define P99__SKP113(_0, ...) P99__SKP112(__VA_ARGS__)
#define P99__SKP114(_0, ...) P99__SKP113(__VA_ARGS__)
#define P99__SKP115(_0, ...) P99__SKP114(__VA_ARGS__)
#define P99__SKP116(_0, ...) P99__SKP115(__VA_ARGS__)
#define P99__SKP117(_0, ...) P99__SKP116(__VA_ARGS__)
#define P99__SKP118(_0, ...) P99__SKP117(__VA_ARGS__)
#define P99__SKP119(_0, ...) P99__SKP118(__VA_ARGS__)
#define P99__SKP120(_0, ...) P99__SKP119(__VA_ARGS__)
#define P99__PRE2(_0, ...) _0, P99__PRE1(__VA_ARGS__)
#define P99__PRE3(_0, ...) _0, P99__PRE2(__VA_ARGS__)
#define P99__PRE4(_0, ...) _0, P99__PRE3(__VA_ARGS__)
#define P99__PRE5(_0, ...) _0, P99__PRE4(__VA_ARGS__)
#define P99__PRE6(_0, ...) _0, P99__PRE5(__VA_ARGS__)
#define P99__PRE7(_0, ...) _0, P99__PRE6(__VA_ARGS__)
#define P99__PRE8(_0, ...) _0, P99__PRE7(__VA_ARGS__)
#define P99__PRE9(_0, ...) _0, P99__PRE8(__VA_ARGS__)
#define P99__PRE10(_0, ...) _0, P99__PRE9(__VA_ARGS__)
#define P99__PRE11(_0, ...) _0, P99__PRE10(__VA_ARGS__)
#define P99__PRE12(_0, ...) _0, P99__PRE11(__VA_ARGS__)
#define P99__PRE13(_0, ...) _0, P99__PRE12(__VA_ARGS__)
#define P99__PRE14(_0, ...) _0, P99__PRE13(__VA_ARGS__)
#define P99__PRE15(_0, ...) _0, P99__PRE14(__VA_ARGS__)
#define P99__PRE16(_0, ...) _0, P99__PRE15(__VA_ARGS__)
#define P99__PRE17(_0, ...) _0, P99__PRE16(__VA_ARGS__)
#define P99__PRE18(_0, ...) _0, P99__PRE17(__VA_ARGS__)
#define P99__PRE19(_0, ...) _0, P99__PRE18(__VA_ARGS__)
#define P99__PRE20(_0, ...) _0, P99__PRE19(__VA_ARGS__)
#define P99__PRE21(_0, ...) _0, P99__PRE20(__VA_ARGS__)
#define P99__PRE22(_0, ...) _0, P99__PRE21(__VA_ARGS__)
#define P99__PRE23(_0, ...) _0, P99__PRE22(__VA_ARGS__)
#define P99__PRE24(_0, ...) _0, P99__PRE23(__VA_ARGS__)
#define P99__PRE25(_0, ...) _0, P99__PRE24(__VA_ARGS__)
#define P99__PRE26(_0, ...) _0, P99__PRE25(__VA_ARGS__)
#define P99__PRE27(_0, ...) _0, P99__PRE26(__VA_ARGS__)
#define P99__PRE28(_0, ...) _0, P99__PRE27(__VA_ARGS__)
#define P99__PRE29(_0, ...) _0, P99__PRE28(__VA_ARGS__)
#define P99__PRE30(_0, ...) _0, P99__PRE29(__VA_ARGS__)
#define P99__PRE31(_0, ...) _0, P99__PRE30(__VA_ARGS__)
#define P99__PRE32(_0, ...) _0, P99__PRE31(__VA_ARGS__)
#define P99__PRE33(_0, ...) _0, P99__PRE32(__VA_ARGS__)
#define P99__PRE34(_0, ...) _0, P99__PRE33(__VA_ARGS__)
#define P99__PRE35(_0, ...) _0, P99__PRE34(__VA_ARGS__)
#define P99__PRE36(_0, ...) _0, P99__PRE35(__VA_ARGS__)
#define P99__PRE37(_0, ...) _0, P99__PRE36(__VA_ARGS__)
#define P99__PRE38(_0, ...) _0, P99__PRE37(__VA_ARGS__)
#define P99__PRE39(_0, ...) _0, P99__PRE38(__VA_ARGS__)
#define P99__PRE40(_0, ...) _0, P99__PRE39(__VA_ARGS__)
#define P99__PRE41(_0, ...) _0, P99__PRE40(__VA_ARGS__)
#define P99__PRE42(_0, ...) _0, P99__PRE41(__VA_ARGS__)
#define P99__PRE43(_0, ...) _0, P99__PRE42(__VA_ARGS__)
#define P99__PRE44(_0, ...) _0, P99__PRE43(__VA_ARGS__)
#define P99__PRE45(_0, ...) _0, P99__PRE44(__VA_ARGS__)
#define P99__PRE46(_0, ...) _0, P99__PRE45(__VA_ARGS__)
#define P99__PRE47(_0, ...) _0, P99__PRE46(__VA_ARGS__)
#define P99__PRE48(_0, ...) _0, P99__PRE47(__VA_ARGS__)
#define P99__PRE49(_0, ...) _0, P99__PRE48(__VA_ARGS__)
#define P99__PRE50(_0, ...) _0, P99__PRE49(__VA_ARGS__)
#define P99__PRE51(_0, ...) _0, P99__PRE50(__VA_ARGS__)
#define P99__PRE52(_0, ...) _0, P99__PRE51(__VA_ARGS__)
#define P99__PRE53(_0, ...) _0, P99__PRE52(__VA_ARGS__)
#define P99__PRE54(_0, ...) _0, P99__PRE53(__VA_ARGS__)
#define P99__PRE55(_0, ...) _0, P99__PRE54(__VA_ARGS__)
#define P99__PRE56(_0, ...) _0, P99__PRE55(__VA_ARGS__)
#define P99__PRE57(_0, ...) _0, P99__PRE56(__VA_ARGS__)
#define P99__PRE58(_0, ...) _0, P99__PRE57(__VA_ARGS__)
#define P99__PRE59(_0, ...) _0, P99__PRE58(__VA_ARGS__)
#define P99__PRE60(_0, ...) _0, P99__PRE59(__VA_ARGS__)
#define P99__PRE61(_0, ...) _0, P99__PRE60(__VA_ARGS__)
#define P99__PRE62(_0, ...) _0, P99__PRE61(__VA_ARGS__)
#define P99__PRE63(_0, ...) _0, P99__PRE62(__VA_ARGS__)
#define P99__PRE64(_0, ...) _0, P99__PRE63(__VA_ARGS__)
#define P99__PRE65(_0, ...) _0, P99__PRE64(__VA_ARGS__)
#define P99__PRE66(_0, ...) _0, P99__PRE65(__VA_ARGS__)
#define P99__PRE67(_0, ...) _0, P99__PRE66(__VA_ARGS__)
#define P99__PRE68(_0, ...) _0, P99__PRE67(__VA_ARGS__)
#define P99__PRE69(_0, ...) _0, P99__PRE68(__VA_ARGS__)
#define P99__PRE70(_0, ...) _0, P99__PRE69(__VA_ARGS__)
#define P99__PRE71(_0, ...) _0, P99__PRE70(__VA_ARGS__)
#define P99__PRE72(_0, ...) _0, P99__PRE71(__VA_ARGS__)
#define P99__PRE73(_0, ...) _0, P99__PRE72(__VA_ARGS__)
#define P99__PRE74(_0, ...) _0, P99__PRE73(__VA_ARGS__)
#define P99__PRE75(_0, ...) _0, P99__PRE74(__VA_ARGS__)
#define P99__PRE76(_0, ...) _0, P99__PRE75(__VA_ARGS__)
#define P99__PRE77(_0, ...) _0, P99__PRE76(__VA_ARGS__)
#define P99__PRE78(_0, ...) _0, P99__PRE77(__VA_ARGS__)
#define P99__PRE79(_0, ...) _0, P99__PRE78(__VA_ARGS__)
#define P99__PRE80(_0, ...) _0, P99__PRE79(__VA_ARGS__)
#define P99__PRE81(_0, ...) _0, P99__PRE80(__VA_ARGS__)
#define P99__PRE82(_0, ...) _0, P99__PRE81(__VA_ARGS__)
#define P99__PRE83(_0, ...) _0, P99__PRE82(__VA_ARGS__)
#define P99__PRE84(_0, ...) _0, P99__PRE83(__VA_ARGS__)
#define P99__PRE85(_0, ...) _0, P99__PRE84(__VA_ARGS__)
#define P99__PRE86(_0, ...) _0, P99__PRE85(__VA_ARGS__)
#define P99__PRE87(_0, ...) _0, P99__PRE86(__VA_ARGS__)
#define P99__PRE88(_0, ...) _0, P99__PRE87(__VA_ARGS__)
#define P99__PRE89(_0, ...) _0, P99__PRE88(__VA_ARGS__)
#define P99__PRE90(_0, ...) _0, P99__PRE89(__VA_ARGS__)
#define P99__PRE91(_0, ...) _0, P99__PRE90(__VA_ARGS__)
#define P99__PRE92(_0, ...) _0, P99__PRE91(__VA_ARGS__)
#define P99__PRE93(_0, ...) _0, P99__PRE92(__VA_ARGS__)
#define P99__PRE94(_0, ...) _0, P99__PRE93(__VA_ARGS__)
#define P99__PRE95(_0, ...) _0, P99__PRE94(__VA_ARGS__)
#define P99__PRE96(_0, ...) _0, P99__PRE95(__VA_ARGS__)
#define P99__PRE97(_0, ...) _0, P99__PRE96(__VA_ARGS__)
#define P99__PRE98(_0, ...) _0, P99__PRE97(__VA_ARGS__)
#define P99__PRE99(_0, ...) _0, P99__PRE98(__VA_ARGS__)
#define P99__PRE100(_0, ...) _0, P99__PRE99(__VA_ARGS__)
#define P99__PRE101(_0, ...) _0, P99__PRE100(__VA_ARGS__)
#define P99__PRE102(_0, ...) _0, P99__PRE101(__VA_ARGS__)
#define P99__PRE103(_0, ...) _0, P99__PRE102(__VA_ARGS__)
#define P99__PRE104(_0, ...) _0, P99__PRE103(__VA_ARGS__)
#define P99__PRE105(_0, ...) _0, P99__PRE104(__VA_ARGS__)
#define P99__PRE106(_0, ...) _0, P99__PRE105(__VA_ARGS__)
#define P99__PRE107(_0, ...) _0, P99__PRE106(__VA_ARGS__)
#define P99__PRE108(_0, ...) _0, P99__PRE107(__VA_ARGS__)
#define P99__PRE109(_0, ...) _0, P99__PRE108(__VA_ARGS__)
#define P99__PRE110(_0, ...) _0, P99__PRE109(__VA_ARGS__)
#define P99__PRE111(_0, ...) _0, P99__PRE110(__VA_ARGS__)
#define P99__PRE112(_0, ...) _0, P99__PRE111(__VA_ARGS__)
#define P99__PRE113(_0, ...) _0, P99__PRE112(__VA_ARGS__)
#define P99__PRE114(_0, ...) _0, P99__PRE113(__VA_ARGS__)
#define P99__PRE115(_0, ...) _0, P99__PRE114(__VA_ARGS__)
#define P99__PRE116(_0, ...) _0, P99__PRE115(__VA_ARGS__)
#define P99__PRE117(_0, ...) _0, P99__PRE116(__VA_ARGS__)
#define P99__PRE118(_0, ...) _0, P99__PRE117(__VA_ARGS__)
#define P99__PRE119(_0, ...) _0, P99__PRE118(__VA_ARGS__)
#define P99__PRE120(_0, ...) _0, P99__PRE119(__VA_ARGS__)
#define P99__ASCENDING()                                       \
	0,	1,	2,	3,	4,	5,	6,	7,                                       \
	8,	9,	10,	11,	12,	13,	14,	15,                                 \
	16,	17,	18,	19,	20,	21,	22,	23,                               \
	24,	25,	26,	27,	28,	29,	30,	31,                               \
	32,	33,	34,	35,	36,	37,	38,	39,                               \
	40,	41,	42,	43,	44,	45,	46,	47,                               \
	48,	49,	50,	51,	52,	53,	54,	55,                               \
	56,	57,	58,	59,	60,	61,	62,	63,                               \
	64,	65,	66,	67,	68,	69,	70,	71,                               \
	72,	73,	74,	75,	76,	77,	78,	79,                               \
	80,	81,	82,	83,	84,	85,	86,	87,                               \
	88,	89,	90,	91,	92,	93,	94,	95,                               \
	96,	97,	98,	99,	100,	101,	102,	103,                           \
	104,	105,	106,	107,	108,	109,	110,	111,                       \
	112,	113,	114,	115,	116,	117,	118,	119,                       \
	120,
#define P99__ALL_ZEROES()                                      \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,	0,	0,	0,	0,	0,	0,	0,                                       \
	0,
#define P99__ALL_ONES()                                        \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,	1,	1,	1,	1,	1,	1,	1,                                       \
	1,
/** @brief Paste 7 arguments at their boundary.*/
#define P99_PASTE7(...) P99__PASTE7(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE7(L, ...) P99_PASTE2(P99_PASTE6(__VA_ARGS__), L)
/** @brief Paste 8 arguments at their boundary.*/
#define P99_PASTE8(...) P99__PASTE8(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE8(L, ...) P99_PASTE2(P99_PASTE7(__VA_ARGS__), L)
/** @brief Paste 9 arguments at their boundary.*/
#define P99_PASTE9(...) P99__PASTE9(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE9(L, ...) P99_PASTE2(P99_PASTE8(__VA_ARGS__), L)
/** @brief Paste 10 arguments at their boundary.*/
#define P99_PASTE10(...) P99__PASTE10(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE10(L, ...) P99_PASTE2(P99_PASTE9(__VA_ARGS__), L)
/** @brief Paste 11 arguments at their boundary.*/
#define P99_PASTE11(...) P99__PASTE11(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE11(L, ...) P99_PASTE2(P99_PASTE10(__VA_ARGS__), L)
/** @brief Paste 12 arguments at their boundary.*/
#define P99_PASTE12(...) P99__PASTE12(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE12(L, ...) P99_PASTE2(P99_PASTE11(__VA_ARGS__), L)
/** @brief Paste 13 arguments at their boundary.*/
#define P99_PASTE13(...) P99__PASTE13(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE13(L, ...) P99_PASTE2(P99_PASTE12(__VA_ARGS__), L)
/** @brief Paste 14 arguments at their boundary.*/
#define P99_PASTE14(...) P99__PASTE14(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE14(L, ...) P99_PASTE2(P99_PASTE13(__VA_ARGS__), L)
/** @brief Paste 15 arguments at their boundary.*/
#define P99_PASTE15(...) P99__PASTE15(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE15(L, ...) P99_PASTE2(P99_PASTE14(__VA_ARGS__), L)
/** @brief Paste 16 arguments at their boundary.*/
#define P99_PASTE16(...) P99__PASTE16(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE16(L, ...) P99_PASTE2(P99_PASTE15(__VA_ARGS__), L)
/** @brief Paste 17 arguments at their boundary.*/
#define P99_PASTE17(...) P99__PASTE17(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE17(L, ...) P99_PASTE2(P99_PASTE16(__VA_ARGS__), L)
/** @brief Paste 18 arguments at their boundary.*/
#define P99_PASTE18(...) P99__PASTE18(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE18(L, ...) P99_PASTE2(P99_PASTE17(__VA_ARGS__), L)
/** @brief Paste 19 arguments at their boundary.*/
#define P99_PASTE19(...) P99__PASTE19(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE19(L, ...) P99_PASTE2(P99_PASTE18(__VA_ARGS__), L)
/** @brief Paste 20 arguments at their boundary.*/
#define P99_PASTE20(...) P99__PASTE20(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE20(L, ...) P99_PASTE2(P99_PASTE19(__VA_ARGS__), L)
/** @brief Paste 21 arguments at their boundary.*/
#define P99_PASTE21(...) P99__PASTE21(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE21(L, ...) P99_PASTE2(P99_PASTE20(__VA_ARGS__), L)
/** @brief Paste 22 arguments at their boundary.*/
#define P99_PASTE22(...) P99__PASTE22(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE22(L, ...) P99_PASTE2(P99_PASTE21(__VA_ARGS__), L)
/** @brief Paste 23 arguments at their boundary.*/
#define P99_PASTE23(...) P99__PASTE23(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE23(L, ...) P99_PASTE2(P99_PASTE22(__VA_ARGS__), L)
/** @brief Paste 24 arguments at their boundary.*/
#define P99_PASTE24(...) P99__PASTE24(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE24(L, ...) P99_PASTE2(P99_PASTE23(__VA_ARGS__), L)
/** @brief Paste 25 arguments at their boundary.*/
#define P99_PASTE25(...) P99__PASTE25(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE25(L, ...) P99_PASTE2(P99_PASTE24(__VA_ARGS__), L)
/** @brief Paste 26 arguments at their boundary.*/
#define P99_PASTE26(...) P99__PASTE26(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE26(L, ...) P99_PASTE2(P99_PASTE25(__VA_ARGS__), L)
/** @brief Paste 27 arguments at their boundary.*/
#define P99_PASTE27(...) P99__PASTE27(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE27(L, ...) P99_PASTE2(P99_PASTE26(__VA_ARGS__), L)
/** @brief Paste 28 arguments at their boundary.*/
#define P99_PASTE28(...) P99__PASTE28(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE28(L, ...) P99_PASTE2(P99_PASTE27(__VA_ARGS__), L)
/** @brief Paste 29 arguments at their boundary.*/
#define P99_PASTE29(...) P99__PASTE29(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE29(L, ...) P99_PASTE2(P99_PASTE28(__VA_ARGS__), L)
/** @brief Paste 30 arguments at their boundary.*/
#define P99_PASTE30(...) P99__PASTE30(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE30(L, ...) P99_PASTE2(P99_PASTE29(__VA_ARGS__), L)
/** @brief Paste 31 arguments at their boundary.*/
#define P99_PASTE31(...) P99__PASTE31(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE31(L, ...) P99_PASTE2(P99_PASTE30(__VA_ARGS__), L)
/** @brief Paste 32 arguments at their boundary.*/
#define P99_PASTE32(...) P99__PASTE32(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE32(L, ...) P99_PASTE2(P99_PASTE31(__VA_ARGS__), L)
/** @brief Paste 33 arguments at their boundary.*/
#define P99_PASTE33(...) P99__PASTE33(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE33(L, ...) P99_PASTE2(P99_PASTE32(__VA_ARGS__), L)
/** @brief Paste 34 arguments at their boundary.*/
#define P99_PASTE34(...) P99__PASTE34(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE34(L, ...) P99_PASTE2(P99_PASTE33(__VA_ARGS__), L)
/** @brief Paste 35 arguments at their boundary.*/
#define P99_PASTE35(...) P99__PASTE35(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE35(L, ...) P99_PASTE2(P99_PASTE34(__VA_ARGS__), L)
/** @brief Paste 36 arguments at their boundary.*/
#define P99_PASTE36(...) P99__PASTE36(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE36(L, ...) P99_PASTE2(P99_PASTE35(__VA_ARGS__), L)
/** @brief Paste 37 arguments at their boundary.*/
#define P99_PASTE37(...) P99__PASTE37(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE37(L, ...) P99_PASTE2(P99_PASTE36(__VA_ARGS__), L)
/** @brief Paste 38 arguments at their boundary.*/
#define P99_PASTE38(...) P99__PASTE38(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE38(L, ...) P99_PASTE2(P99_PASTE37(__VA_ARGS__), L)
/** @brief Paste 39 arguments at their boundary.*/
#define P99_PASTE39(...) P99__PASTE39(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE39(L, ...) P99_PASTE2(P99_PASTE38(__VA_ARGS__), L)
/** @brief Paste 40 arguments at their boundary.*/
#define P99_PASTE40(...) P99__PASTE40(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE40(L, ...) P99_PASTE2(P99_PASTE39(__VA_ARGS__), L)
/** @brief Paste 41 arguments at their boundary.*/
#define P99_PASTE41(...) P99__PASTE41(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE41(L, ...) P99_PASTE2(P99_PASTE40(__VA_ARGS__), L)
/** @brief Paste 42 arguments at their boundary.*/
#define P99_PASTE42(...) P99__PASTE42(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE42(L, ...) P99_PASTE2(P99_PASTE41(__VA_ARGS__), L)
/** @brief Paste 43 arguments at their boundary.*/
#define P99_PASTE43(...) P99__PASTE43(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE43(L, ...) P99_PASTE2(P99_PASTE42(__VA_ARGS__), L)
/** @brief Paste 44 arguments at their boundary.*/
#define P99_PASTE44(...) P99__PASTE44(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE44(L, ...) P99_PASTE2(P99_PASTE43(__VA_ARGS__), L)
/** @brief Paste 45 arguments at their boundary.*/
#define P99_PASTE45(...) P99__PASTE45(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE45(L, ...) P99_PASTE2(P99_PASTE44(__VA_ARGS__), L)
/** @brief Paste 46 arguments at their boundary.*/
#define P99_PASTE46(...) P99__PASTE46(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE46(L, ...) P99_PASTE2(P99_PASTE45(__VA_ARGS__), L)
/** @brief Paste 47 arguments at their boundary.*/
#define P99_PASTE47(...) P99__PASTE47(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE47(L, ...) P99_PASTE2(P99_PASTE46(__VA_ARGS__), L)
/** @brief Paste 48 arguments at their boundary.*/
#define P99_PASTE48(...) P99__PASTE48(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE48(L, ...) P99_PASTE2(P99_PASTE47(__VA_ARGS__), L)
/** @brief Paste 49 arguments at their boundary.*/
#define P99_PASTE49(...) P99__PASTE49(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE49(L, ...) P99_PASTE2(P99_PASTE48(__VA_ARGS__), L)
/** @brief Paste 50 arguments at their boundary.*/
#define P99_PASTE50(...) P99__PASTE50(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE50(L, ...) P99_PASTE2(P99_PASTE49(__VA_ARGS__), L)
/** @brief Paste 51 arguments at their boundary.*/
#define P99_PASTE51(...) P99__PASTE51(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE51(L, ...) P99_PASTE2(P99_PASTE50(__VA_ARGS__), L)
/** @brief Paste 52 arguments at their boundary.*/
#define P99_PASTE52(...) P99__PASTE52(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE52(L, ...) P99_PASTE2(P99_PASTE51(__VA_ARGS__), L)
/** @brief Paste 53 arguments at their boundary.*/
#define P99_PASTE53(...) P99__PASTE53(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE53(L, ...) P99_PASTE2(P99_PASTE52(__VA_ARGS__), L)
/** @brief Paste 54 arguments at their boundary.*/
#define P99_PASTE54(...) P99__PASTE54(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE54(L, ...) P99_PASTE2(P99_PASTE53(__VA_ARGS__), L)
/** @brief Paste 55 arguments at their boundary.*/
#define P99_PASTE55(...) P99__PASTE55(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE55(L, ...) P99_PASTE2(P99_PASTE54(__VA_ARGS__), L)
/** @brief Paste 56 arguments at their boundary.*/
#define P99_PASTE56(...) P99__PASTE56(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE56(L, ...) P99_PASTE2(P99_PASTE55(__VA_ARGS__), L)
/** @brief Paste 57 arguments at their boundary.*/
#define P99_PASTE57(...) P99__PASTE57(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE57(L, ...) P99_PASTE2(P99_PASTE56(__VA_ARGS__), L)
/** @brief Paste 58 arguments at their boundary.*/
#define P99_PASTE58(...) P99__PASTE58(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE58(L, ...) P99_PASTE2(P99_PASTE57(__VA_ARGS__), L)
/** @brief Paste 59 arguments at their boundary.*/
#define P99_PASTE59(...) P99__PASTE59(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE59(L, ...) P99_PASTE2(P99_PASTE58(__VA_ARGS__), L)
/** @brief Paste 60 arguments at their boundary.*/
#define P99_PASTE60(...) P99__PASTE60(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE60(L, ...) P99_PASTE2(P99_PASTE59(__VA_ARGS__), L)
/** @brief Paste 61 arguments at their boundary.*/
#define P99_PASTE61(...) P99__PASTE61(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE61(L, ...) P99_PASTE2(P99_PASTE60(__VA_ARGS__), L)
/** @brief Paste 62 arguments at their boundary.*/
#define P99_PASTE62(...) P99__PASTE62(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE62(L, ...) P99_PASTE2(P99_PASTE61(__VA_ARGS__), L)
/** @brief Paste 63 arguments at their boundary.*/
#define P99_PASTE63(...) P99__PASTE63(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE63(L, ...) P99_PASTE2(P99_PASTE62(__VA_ARGS__), L)
/** @brief Paste 64 arguments at their boundary.*/
#define P99_PASTE64(...) P99__PASTE64(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE64(L, ...) P99_PASTE2(P99_PASTE63(__VA_ARGS__), L)
/** @brief Paste 65 arguments at their boundary.*/
#define P99_PASTE65(...) P99__PASTE65(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE65(L, ...) P99_PASTE2(P99_PASTE64(__VA_ARGS__), L)
/** @brief Paste 66 arguments at their boundary.*/
#define P99_PASTE66(...) P99__PASTE66(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE66(L, ...) P99_PASTE2(P99_PASTE65(__VA_ARGS__), L)
/** @brief Paste 67 arguments at their boundary.*/
#define P99_PASTE67(...) P99__PASTE67(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE67(L, ...) P99_PASTE2(P99_PASTE66(__VA_ARGS__), L)
/** @brief Paste 68 arguments at their boundary.*/
#define P99_PASTE68(...) P99__PASTE68(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE68(L, ...) P99_PASTE2(P99_PASTE67(__VA_ARGS__), L)
/** @brief Paste 69 arguments at their boundary.*/
#define P99_PASTE69(...) P99__PASTE69(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE69(L, ...) P99_PASTE2(P99_PASTE68(__VA_ARGS__), L)
/** @brief Paste 70 arguments at their boundary.*/
#define P99_PASTE70(...) P99__PASTE70(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE70(L, ...) P99_PASTE2(P99_PASTE69(__VA_ARGS__), L)
/** @brief Paste 71 arguments at their boundary.*/
#define P99_PASTE71(...) P99__PASTE71(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE71(L, ...) P99_PASTE2(P99_PASTE70(__VA_ARGS__), L)
/** @brief Paste 72 arguments at their boundary.*/
#define P99_PASTE72(...) P99__PASTE72(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE72(L, ...) P99_PASTE2(P99_PASTE71(__VA_ARGS__), L)
/** @brief Paste 73 arguments at their boundary.*/
#define P99_PASTE73(...) P99__PASTE73(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE73(L, ...) P99_PASTE2(P99_PASTE72(__VA_ARGS__), L)
/** @brief Paste 74 arguments at their boundary.*/
#define P99_PASTE74(...) P99__PASTE74(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE74(L, ...) P99_PASTE2(P99_PASTE73(__VA_ARGS__), L)
/** @brief Paste 75 arguments at their boundary.*/
#define P99_PASTE75(...) P99__PASTE75(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE75(L, ...) P99_PASTE2(P99_PASTE74(__VA_ARGS__), L)
/** @brief Paste 76 arguments at their boundary.*/
#define P99_PASTE76(...) P99__PASTE76(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE76(L, ...) P99_PASTE2(P99_PASTE75(__VA_ARGS__), L)
/** @brief Paste 77 arguments at their boundary.*/
#define P99_PASTE77(...) P99__PASTE77(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE77(L, ...) P99_PASTE2(P99_PASTE76(__VA_ARGS__), L)
/** @brief Paste 78 arguments at their boundary.*/
#define P99_PASTE78(...) P99__PASTE78(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE78(L, ...) P99_PASTE2(P99_PASTE77(__VA_ARGS__), L)
/** @brief Paste 79 arguments at their boundary.*/
#define P99_PASTE79(...) P99__PASTE79(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE79(L, ...) P99_PASTE2(P99_PASTE78(__VA_ARGS__), L)
/** @brief Paste 80 arguments at their boundary.*/
#define P99_PASTE80(...) P99__PASTE80(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE80(L, ...) P99_PASTE2(P99_PASTE79(__VA_ARGS__), L)
/** @brief Paste 81 arguments at their boundary.*/
#define P99_PASTE81(...) P99__PASTE81(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE81(L, ...) P99_PASTE2(P99_PASTE80(__VA_ARGS__), L)
/** @brief Paste 82 arguments at their boundary.*/
#define P99_PASTE82(...) P99__PASTE82(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE82(L, ...) P99_PASTE2(P99_PASTE81(__VA_ARGS__), L)
/** @brief Paste 83 arguments at their boundary.*/
#define P99_PASTE83(...) P99__PASTE83(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE83(L, ...) P99_PASTE2(P99_PASTE82(__VA_ARGS__), L)
/** @brief Paste 84 arguments at their boundary.*/
#define P99_PASTE84(...) P99__PASTE84(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE84(L, ...) P99_PASTE2(P99_PASTE83(__VA_ARGS__), L)
/** @brief Paste 85 arguments at their boundary.*/
#define P99_PASTE85(...) P99__PASTE85(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE85(L, ...) P99_PASTE2(P99_PASTE84(__VA_ARGS__), L)
/** @brief Paste 86 arguments at their boundary.*/
#define P99_PASTE86(...) P99__PASTE86(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE86(L, ...) P99_PASTE2(P99_PASTE85(__VA_ARGS__), L)
/** @brief Paste 87 arguments at their boundary.*/
#define P99_PASTE87(...) P99__PASTE87(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE87(L, ...) P99_PASTE2(P99_PASTE86(__VA_ARGS__), L)
/** @brief Paste 88 arguments at their boundary.*/
#define P99_PASTE88(...) P99__PASTE88(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE88(L, ...) P99_PASTE2(P99_PASTE87(__VA_ARGS__), L)
/** @brief Paste 89 arguments at their boundary.*/
#define P99_PASTE89(...) P99__PASTE89(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE89(L, ...) P99_PASTE2(P99_PASTE88(__VA_ARGS__), L)
/** @brief Paste 90 arguments at their boundary.*/
#define P99_PASTE90(...) P99__PASTE90(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE90(L, ...) P99_PASTE2(P99_PASTE89(__VA_ARGS__), L)
/** @brief Paste 91 arguments at their boundary.*/
#define P99_PASTE91(...) P99__PASTE91(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE91(L, ...) P99_PASTE2(P99_PASTE90(__VA_ARGS__), L)
/** @brief Paste 92 arguments at their boundary.*/
#define P99_PASTE92(...) P99__PASTE92(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE92(L, ...) P99_PASTE2(P99_PASTE91(__VA_ARGS__), L)
/** @brief Paste 93 arguments at their boundary.*/
#define P99_PASTE93(...) P99__PASTE93(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE93(L, ...) P99_PASTE2(P99_PASTE92(__VA_ARGS__), L)
/** @brief Paste 94 arguments at their boundary.*/
#define P99_PASTE94(...) P99__PASTE94(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE94(L, ...) P99_PASTE2(P99_PASTE93(__VA_ARGS__), L)
/** @brief Paste 95 arguments at their boundary.*/
#define P99_PASTE95(...) P99__PASTE95(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE95(L, ...) P99_PASTE2(P99_PASTE94(__VA_ARGS__), L)
/** @brief Paste 96 arguments at their boundary.*/
#define P99_PASTE96(...) P99__PASTE96(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE96(L, ...) P99_PASTE2(P99_PASTE95(__VA_ARGS__), L)
/** @brief Paste 97 arguments at their boundary.*/
#define P99_PASTE97(...) P99__PASTE97(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE97(L, ...) P99_PASTE2(P99_PASTE96(__VA_ARGS__), L)
/** @brief Paste 98 arguments at their boundary.*/
#define P99_PASTE98(...) P99__PASTE98(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE98(L, ...) P99_PASTE2(P99_PASTE97(__VA_ARGS__), L)
/** @brief Paste 99 arguments at their boundary.*/
#define P99_PASTE99(...) P99__PASTE99(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE99(L, ...) P99_PASTE2(P99_PASTE98(__VA_ARGS__), L)
/** @brief Paste 100 arguments at their boundary.*/
#define P99_PASTE100(...) P99__PASTE100(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE100(L, ...) P99_PASTE2(P99_PASTE99(__VA_ARGS__), L)
/** @brief Paste 101 arguments at their boundary.*/
#define P99_PASTE101(...) P99__PASTE101(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE101(L, ...) P99_PASTE2(P99_PASTE100(__VA_ARGS__), L)
/** @brief Paste 102 arguments at their boundary.*/
#define P99_PASTE102(...) P99__PASTE102(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE102(L, ...) P99_PASTE2(P99_PASTE101(__VA_ARGS__), L)
/** @brief Paste 103 arguments at their boundary.*/
#define P99_PASTE103(...) P99__PASTE103(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE103(L, ...) P99_PASTE2(P99_PASTE102(__VA_ARGS__), L)
/** @brief Paste 104 arguments at their boundary.*/
#define P99_PASTE104(...) P99__PASTE104(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE104(L, ...) P99_PASTE2(P99_PASTE103(__VA_ARGS__), L)
/** @brief Paste 105 arguments at their boundary.*/
#define P99_PASTE105(...) P99__PASTE105(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE105(L, ...) P99_PASTE2(P99_PASTE104(__VA_ARGS__), L)
/** @brief Paste 106 arguments at their boundary.*/
#define P99_PASTE106(...) P99__PASTE106(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE106(L, ...) P99_PASTE2(P99_PASTE105(__VA_ARGS__), L)
/** @brief Paste 107 arguments at their boundary.*/
#define P99_PASTE107(...) P99__PASTE107(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE107(L, ...) P99_PASTE2(P99_PASTE106(__VA_ARGS__), L)
/** @brief Paste 108 arguments at their boundary.*/
#define P99_PASTE108(...) P99__PASTE108(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE108(L, ...) P99_PASTE2(P99_PASTE107(__VA_ARGS__), L)
/** @brief Paste 109 arguments at their boundary.*/
#define P99_PASTE109(...) P99__PASTE109(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE109(L, ...) P99_PASTE2(P99_PASTE108(__VA_ARGS__), L)
/** @brief Paste 110 arguments at their boundary.*/
#define P99_PASTE110(...) P99__PASTE110(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE110(L, ...) P99_PASTE2(P99_PASTE109(__VA_ARGS__), L)
/** @brief Paste 111 arguments at their boundary.*/
#define P99_PASTE111(...) P99__PASTE111(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE111(L, ...) P99_PASTE2(P99_PASTE110(__VA_ARGS__), L)
/** @brief Paste 112 arguments at their boundary.*/
#define P99_PASTE112(...) P99__PASTE112(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE112(L, ...) P99_PASTE2(P99_PASTE111(__VA_ARGS__), L)
/** @brief Paste 113 arguments at their boundary.*/
#define P99_PASTE113(...) P99__PASTE113(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE113(L, ...) P99_PASTE2(P99_PASTE112(__VA_ARGS__), L)
/** @brief Paste 114 arguments at their boundary.*/
#define P99_PASTE114(...) P99__PASTE114(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE114(L, ...) P99_PASTE2(P99_PASTE113(__VA_ARGS__), L)
/** @brief Paste 115 arguments at their boundary.*/
#define P99_PASTE115(...) P99__PASTE115(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE115(L, ...) P99_PASTE2(P99_PASTE114(__VA_ARGS__), L)
/** @brief Paste 116 arguments at their boundary.*/
#define P99_PASTE116(...) P99__PASTE116(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE116(L, ...) P99_PASTE2(P99_PASTE115(__VA_ARGS__), L)
/** @brief Paste 117 arguments at their boundary.*/
#define P99_PASTE117(...) P99__PASTE117(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE117(L, ...) P99_PASTE2(P99_PASTE116(__VA_ARGS__), L)
/** @brief Paste 118 arguments at their boundary.*/
#define P99_PASTE118(...) P99__PASTE118(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE118(L, ...) P99_PASTE2(P99_PASTE117(__VA_ARGS__), L)
/** @brief Paste 119 arguments at their boundary.*/
#define P99_PASTE119(...) P99__PASTE119(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE119(L, ...) P99_PASTE2(P99_PASTE118(__VA_ARGS__), L)
/** @brief Paste 120 arguments at their boundary.*/
#define P99_PASTE120(...) P99__PASTE120(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))
#define P99__PASTE120(L, ...) P99_PASTE2(P99_PASTE119(__VA_ARGS__), L)
/**
 ** @addtogroup preprocessor_text
 **
 ** @brief These macros allow the handling of small texts and tokens.
 **
 ** For the following tokens, the macro ::P99_TOK_EQ will work.
 ** - decimal numbers: 0 ... 121
 ** - keywords:
 **     @c _Bool
 **     @c _Complex
 **     @c _Imaginary
 **     @c auto
 **     @c break
 **     @c case
 **     @c char
 **     @c const
 **     @c continue
 **     @c default
 **     @c do
 **     @c double
 **     @c else
 **     @c enum
 **     @c extern
 **     @c float
 **     @c for
 **     @c goto
 **     @c if
 **     @c inline
 **     @c int
 **     @c long
 **     @c register
 **     @c restrict
 **     @c return
 **     @c short
 **     @c signed
 **     @c sizeof
 **     @c static
 **     @c struct
 **     @c switch
 **     @c typedef
 **     @c union
 **     @c unsigned
 **     @c void
 **     @c volatile
 **     @c while
 ** - typedefs:
 **     @c clock_t
 **     @c div_t
 **     @c double_t
 **     @c fenv_t
 **     @c fexcept_t
 **     @c float_t
 **     @c fpos_t
 **     @c imaxdiv_t
 **     @c int16_t
 **     @c int32_t
 **     @c int64_t
 **     @c int8_t
 **     @c intfast16_t
 **     @c intfast32_t
 **     @c intfast64_t
 **     @c intfast8_t
 **     @c intleast16_t
 **     @c intleast32_t
 **     @c intleast64_t
 **     @c intleast8_t
 **     @c intmax_t
 **     @c intptr_t
 **     @c ldiv_t
 **     @c lldiv_t
 **     @c mbstate_t
 **     @c off_t
 **     @c ptrdiff_t
 **     @c sig_atomic_t
 **     @c size_t
 **     @c ssize_t
 **     @c time_t
 **     @c uint16_t
 **     @c uint32_t
 **     @c uint64_t
 **     @c uint8_t
 **     @c uintfast16_t
 **     @c uintfast32_t
 **     @c uintfast64_t
 **     @c uintfast8_t
 **     @c uintleast16_t
 **     @c uintleast32_t
 **     @c uintleast64_t
 **     @c uintleast8_t
 **     @c uintmax_t
 **     @c uintptr_t
 **     @c wchar_t
 **     @c wctrans_t
 **     @c wctype_t
 **     @c wint_t
 ** - functions or functional macros:
 **     @c _Exit
 **     @c abort
 **     @c abs
 **     @c acos
 **     @c asctime
 **     @c asin
 **     @c assert
 **     @c atan
 **     @c atan2
 **     @c atexit
 **     @c atof
 **     @c atof
 **     @c atoi
 **     @c atol
 **     @c bsearch
 **     @c btowc
 **     @c cabs
 **     @c cacos
 **     @c cacosh
 **     @c calloc
 **     @c carg
 **     @c casin
 **     @c casinh
 **     @c catan
 **     @c catanh
 **     @c ccos
 **     @c ccosh
 **     @c ceil
 **     @c cexp
 **     @c cimag
 **     @c clearerr
 **     @c clock
 **     @c clog
 **     @c conj
 **     @c cos
 **     @c cosh
 **     @c cpow
 **     @c cproj
 **     @c creal
 **     @c csin
 **     @c csinh
 **     @c csqrt
 **     @c ctan
 **     @c ctanh
 **     @c ctime
 **     @c difftime
 **     @c div
 **     @c exit
 **     @c exp
 **     @c fabs
 **     @c fclose
 **     @c fdopen
 **     @c feof
 **     @c ferror
 **     @c fflush
 **     @c fgetc
 **     @c fgetpos
 **     @c fgets
 **     @c fgetwc
 **     @c fgetws
 **     @c floor
 **     @c fopen
 **     @c fprintf
 **     @c fprintf
 **     @c fputc
 **     @c fputchar
 **     @c fputs
 **     @c fputwc
 **     @c fputws
 **     @c fread
 **     @c free
 **     @c freopen
 **     @c frexp
 **     @c fscanf
 **     @c fscanf
 **     @c fseek
 **     @c fsetpos
 **     @c ftell
 **     @c fwide
 **     @c fwprintf
 **     @c fwrite
 **     @c fwscanf
 **     @c getaddrinfo
 **     @c getc
 **     @c getchar
 **     @c getenv
 **     @c getnameinfo
 **     @c gets
 **     @c getwc
 **     @c getwchar
 **     @c gmtime
 **     @c imaxabs
 **     @c imaxdiv
 **     @c isalnum
 **     @c isalpha
 **     @c isblank
 **     @c iscntrl
 **     @c isdigit
 **     @c isgraph
 **     @c islower
 **     @c isprint
 **     @c ispunct
 **     @c isspace
 **     @c isupper
 **     @c iswalnum
 **     @c iswalpha
 **     @c iswcntrl
 **     @c iswctype
 **     @c iswdigit
 **     @c iswgraph
 **     @c iswlower
 **     @c iswprint
 **     @c iswpunct
 **     @c iswspace
 **     @c iswupper
 **     @c iswxdigit
 **     @c isxdigit
 **     @c labs
 **     @c ldexp
 **     @c ldiv
 **     @c ldiv
 **     @c localeconv
 **     @c localtime
 **     @c log
 **     @c log10
 **     @c longjmp
 **     @c malloc
 **     @c mbrlen
 **     @c mbrtowc
 **     @c mbsinit
 **     @c mbsrtowcs
 **     @c memchr
 **     @c memcmp
 **     @c memcpy
 **     @c memmove
 **     @c memset
 **     @c mktime
 **     @c modf
 **     @c offsetof
 **     @c perror
 **     @c pow
 **     @c printf
 **     @c putc
 **     @c putchar
 **     @c puts
 **     @c putwc
 **     @c putwchar
 **     @c qsort
 **     @c raise
 **     @c rand
 **     @c realloc
 **     @c remove
 **     @c rename
 **     @c rewind
 **     @c scanf
 **     @c setbuf
 **     @c setjmp
 **     @c setlocale
 **     @c setvbuf
 **     @c sin
 **     @c sinh
 **     @c snprintf
 **     @c sprintf
 **     @c sqrt
 **     @c srand
 **     @c sscanf
 **     @c strcat
 **     @c strchr
 **     @c strcmp
 **     @c strcoll
 **     @c strcpy
 **     @c strcspn
 **     @c strerror
 **     @c strftime
 **     @c strlen
 **     @c strncat
 **     @c strncmp
 **     @c strncpy
 **     @c strpbrk
 **     @c strrchr
 **     @c strspn
 **     @c strstr
 **     @c strtod
 **     @c strtoimax
 **     @c strtok
 **     @c strtol
 **     @c strtoul
 **     @c strtoumax
 **     @c strxfrm
 **     @c swprintf
 **     @c swscanf
 **     @c system
 **     @c tan
 **     @c tanh
 **     @c time
 **     @c tmpfile
 **     @c tmpnam
 **     @c tolower
 **     @c toupper
 **     @c towctrans
 **     @c towlower
 **     @c towupper
 **     @c ungetc
 **     @c ungetwc
 **     @c va_arg
 **     @c va_copy
 **     @c va_end
 **     @c va_start
 **     @c vfprintf
 **     @c vfscanf
 **     @c vfwprintf
 **     @c vprintf
 **     @c vscanf
 **     @c vsprintf
 **     @c vsscanf
 **     @c vswprintf
 **     @c vwprintf
 **     @c wcrtomb
 **     @c wcscat
 **     @c wcschr
 **     @c wcscmp
 **     @c wcscoll
 **     @c wcscpy
 **     @c wcscspn
 **     @c wcsftime
 **     @c wcslen
 **     @c wcsncat
 **     @c wcsncmp
 **     @c wcsncpy
 **     @c wcspbrk
 **     @c wcsrchr
 **     @c wcsrtombs
 **     @c wcsspn
 **     @c wcsstr
 **     @c wcstod
 **     @c wcstoimax
 **     @c wcstok
 **     @c wcstol
 **     @c wcstoul
 **     @c wcstoumax
 **     @c wcsxfrm
 **     @c wctob
 **     @c wctrans
 **     @c wctype
 **     @c wmemchr
 **     @c wmemcmp
 **     @c wmemcpy
 **     @c wmemmove
 **     @c wmemset
 **     @c wprintf
 **     @c wscanf
 **
 ** @warning Non-functional macros cannot be captured by this mechanism since they will
 ** be expanded before we may actually handle them. This concerns in particular
 ** the following tokens that are defined to be macros:
 **     @c NULL
 **     @c and
 **     @c and_eq
 **     @c bitand
 **     @c bitor
 **     @c bool
 **     @c compl
 **     @c false
 **     @c not
 **     @c not_eq
 **     @c offsetof
 **     @c or
 **     @c or_eq
 **     @c true
 **     @c xor
 **     @c xor_eq
 ** @{
 **/
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
#define P99__IS_10_EQ_10(...) ,
#define P99__IS_11_EQ_11(...) ,
#define P99__IS_12_EQ_12(...) ,
#define P99__IS_13_EQ_13(...) ,
#define P99__IS_14_EQ_14(...) ,
#define P99__IS_15_EQ_15(...) ,
#define P99__IS_16_EQ_16(...) ,
#define P99__IS_17_EQ_17(...) ,
#define P99__IS_18_EQ_18(...) ,
#define P99__IS_19_EQ_19(...) ,
#define P99__IS_20_EQ_20(...) ,
#define P99__IS_21_EQ_21(...) ,
#define P99__IS_22_EQ_22(...) ,
#define P99__IS_23_EQ_23(...) ,
#define P99__IS_24_EQ_24(...) ,
#define P99__IS_25_EQ_25(...) ,
#define P99__IS_26_EQ_26(...) ,
#define P99__IS_27_EQ_27(...) ,
#define P99__IS_28_EQ_28(...) ,
#define P99__IS_29_EQ_29(...) ,
#define P99__IS_30_EQ_30(...) ,
#define P99__IS_31_EQ_31(...) ,
#define P99__IS_32_EQ_32(...) ,
#define P99__IS_33_EQ_33(...) ,
#define P99__IS_34_EQ_34(...) ,
#define P99__IS_35_EQ_35(...) ,
#define P99__IS_36_EQ_36(...) ,
#define P99__IS_37_EQ_37(...) ,
#define P99__IS_38_EQ_38(...) ,
#define P99__IS_39_EQ_39(...) ,
#define P99__IS_40_EQ_40(...) ,
#define P99__IS_41_EQ_41(...) ,
#define P99__IS_42_EQ_42(...) ,
#define P99__IS_43_EQ_43(...) ,
#define P99__IS_44_EQ_44(...) ,
#define P99__IS_45_EQ_45(...) ,
#define P99__IS_46_EQ_46(...) ,
#define P99__IS_47_EQ_47(...) ,
#define P99__IS_48_EQ_48(...) ,
#define P99__IS_49_EQ_49(...) ,
#define P99__IS_50_EQ_50(...) ,
#define P99__IS_51_EQ_51(...) ,
#define P99__IS_52_EQ_52(...) ,
#define P99__IS_53_EQ_53(...) ,
#define P99__IS_54_EQ_54(...) ,
#define P99__IS_55_EQ_55(...) ,
#define P99__IS_56_EQ_56(...) ,
#define P99__IS_57_EQ_57(...) ,
#define P99__IS_58_EQ_58(...) ,
#define P99__IS_59_EQ_59(...) ,
#define P99__IS_60_EQ_60(...) ,
#define P99__IS_61_EQ_61(...) ,
#define P99__IS_62_EQ_62(...) ,
#define P99__IS_63_EQ_63(...) ,
#define P99__IS_64_EQ_64(...) ,
#define P99__IS_65_EQ_65(...) ,
#define P99__IS_66_EQ_66(...) ,
#define P99__IS_67_EQ_67(...) ,
#define P99__IS_68_EQ_68(...) ,
#define P99__IS_69_EQ_69(...) ,
#define P99__IS_70_EQ_70(...) ,
#define P99__IS_71_EQ_71(...) ,
#define P99__IS_72_EQ_72(...) ,
#define P99__IS_73_EQ_73(...) ,
#define P99__IS_74_EQ_74(...) ,
#define P99__IS_75_EQ_75(...) ,
#define P99__IS_76_EQ_76(...) ,
#define P99__IS_77_EQ_77(...) ,
#define P99__IS_78_EQ_78(...) ,
#define P99__IS_79_EQ_79(...) ,
#define P99__IS_80_EQ_80(...) ,
#define P99__IS_81_EQ_81(...) ,
#define P99__IS_82_EQ_82(...) ,
#define P99__IS_83_EQ_83(...) ,
#define P99__IS_84_EQ_84(...) ,
#define P99__IS_85_EQ_85(...) ,
#define P99__IS_86_EQ_86(...) ,
#define P99__IS_87_EQ_87(...) ,
#define P99__IS_88_EQ_88(...) ,
#define P99__IS_89_EQ_89(...) ,
#define P99__IS_90_EQ_90(...) ,
#define P99__IS_91_EQ_91(...) ,
#define P99__IS_92_EQ_92(...) ,
#define P99__IS_93_EQ_93(...) ,
#define P99__IS_94_EQ_94(...) ,
#define P99__IS_95_EQ_95(...) ,
#define P99__IS_96_EQ_96(...) ,
#define P99__IS_97_EQ_97(...) ,
#define P99__IS_98_EQ_98(...) ,
#define P99__IS_99_EQ_99(...) ,
#define P99__IS_100_EQ_100(...) ,
#define P99__IS_101_EQ_101(...) ,
#define P99__IS_102_EQ_102(...) ,
#define P99__IS_103_EQ_103(...) ,
#define P99__IS_104_EQ_104(...) ,
#define P99__IS_105_EQ_105(...) ,
#define P99__IS_106_EQ_106(...) ,
#define P99__IS_107_EQ_107(...) ,
#define P99__IS_108_EQ_108(...) ,
#define P99__IS_109_EQ_109(...) ,
#define P99__IS_110_EQ_110(...) ,
#define P99__IS_111_EQ_111(...) ,
#define P99__IS_112_EQ_112(...) ,
#define P99__IS_113_EQ_113(...) ,
#define P99__IS_114_EQ_114(...) ,
#define P99__IS_115_EQ_115(...) ,
#define P99__IS_116_EQ_116(...) ,
#define P99__IS_117_EQ_117(...) ,
#define P99__IS_118_EQ_118(...) ,
#define P99__IS_119_EQ_119(...) ,
#define P99__IS_120_EQ_120(...) ,
#define P99__IS_121_EQ_121(...) ,
#define P99__IS__Bool_EQ__Bool(...) ,
#define P99__IS__Complex_EQ__Complex(...) ,
#define P99__IS__Exit_EQ__Exit(...) ,
#define P99__IS__Imaginary_EQ__Imaginary(...) ,
#define P99__IS_abort_EQ_abort(...) ,
#define P99__IS_abs_EQ_abs(...) ,
#define P99__IS_acos_EQ_acos(...) ,
#define P99__IS_asctime_EQ_asctime(...) ,
#define P99__IS_asin_EQ_asin(...) ,
#define P99__IS_assert_EQ_assert(...) ,
#define P99__IS_atan_EQ_atan(...) ,
#define P99__IS_atan2_EQ_atan2(...) ,
#define P99__IS_atexit_EQ_atexit(...) ,
#define P99__IS_atof_EQ_atof(...) ,
#define P99__IS_atof_EQ_atof(...) ,
#define P99__IS_atoi_EQ_atoi(...) ,
#define P99__IS_atol_EQ_atol(...) ,
#define P99__IS_auto_EQ_auto(...) ,
#define P99__IS_break_EQ_break(...) ,
#define P99__IS_bsearch_EQ_bsearch(...) ,
#define P99__IS_btowc_EQ_btowc(...) ,
#define P99__IS_cabs_EQ_cabs(...) ,
#define P99__IS_cacos_EQ_cacos(...) ,
#define P99__IS_cacosh_EQ_cacosh(...) ,
#define P99__IS_calloc_EQ_calloc(...) ,
#define P99__IS_carg_EQ_carg(...) ,
#define P99__IS_case_EQ_case(...) ,
#define P99__IS_casin_EQ_casin(...) ,
#define P99__IS_casinh_EQ_casinh(...) ,
#define P99__IS_catan_EQ_catan(...) ,
#define P99__IS_catanh_EQ_catanh(...) ,
#define P99__IS_ccos_EQ_ccos(...) ,
#define P99__IS_ccosh_EQ_ccosh(...) ,
#define P99__IS_ceil_EQ_ceil(...) ,
#define P99__IS_cexp_EQ_cexp(...) ,
#define P99__IS_char_EQ_char(...) ,
#define P99__IS_cimag_EQ_cimag(...) ,
#define P99__IS_clearerr_EQ_clearerr(...) ,
#define P99__IS_clock_EQ_clock(...) ,
#define P99__IS_clock_t_EQ_clock_t(...) ,
#define P99__IS_clog_EQ_clog(...) ,
#define P99__IS_conj_EQ_conj(...) ,
#define P99__IS_const_EQ_const(...) ,
#define P99__IS_continue_EQ_continue(...) ,
#define P99__IS_cos_EQ_cos(...) ,
#define P99__IS_cosh_EQ_cosh(...) ,
#define P99__IS_cpow_EQ_cpow(...) ,
#define P99__IS_cproj_EQ_cproj(...) ,
#define P99__IS_creal_EQ_creal(...) ,
#define P99__IS_csin_EQ_csin(...) ,
#define P99__IS_csinh_EQ_csinh(...) ,
#define P99__IS_csqrt_EQ_csqrt(...) ,
#define P99__IS_ctan_EQ_ctan(...) ,
#define P99__IS_ctanh_EQ_ctanh(...) ,
#define P99__IS_ctime_EQ_ctime(...) ,
#define P99__IS_default_EQ_default(...) ,
#define P99__IS_difftime_EQ_difftime(...) ,
#define P99__IS_div_EQ_div(...) ,
#define P99__IS_div_t_EQ_div_t(...) ,
#define P99__IS_do_EQ_do(...) ,
#define P99__IS_double_EQ_double(...) ,
#define P99__IS_double_t_EQ_double_t(...) ,
#define P99__IS_else_EQ_else(...) ,
#define P99__IS_enum_EQ_enum(...) ,
#define P99__IS_exit_EQ_exit(...) ,
#define P99__IS_exp_EQ_exp(...) ,
#define P99__IS_extern_EQ_extern(...) ,
#define P99__IS_fabs_EQ_fabs(...) ,
#define P99__IS_fclose_EQ_fclose(...) ,
#define P99__IS_fdopen_EQ_fdopen(...) ,
#define P99__IS_fenv_t_EQ_fenv_t(...) ,
#define P99__IS_feof_EQ_feof(...) ,
#define P99__IS_ferror_EQ_ferror(...) ,
#define P99__IS_fexcept_t_EQ_fexcept_t(...) ,
#define P99__IS_fflush_EQ_fflush(...) ,
#define P99__IS_fgetc_EQ_fgetc(...) ,
#define P99__IS_fgetpos_EQ_fgetpos(...) ,
#define P99__IS_fgets_EQ_fgets(...) ,
#define P99__IS_fgetwc_EQ_fgetwc(...) ,
#define P99__IS_fgetws_EQ_fgetws(...) ,
#define P99__IS_float_EQ_float(...) ,
#define P99__IS_float_t_EQ_float_t(...) ,
#define P99__IS_floor_EQ_floor(...) ,
#define P99__IS_fopen_EQ_fopen(...) ,
#define P99__IS_for_EQ_for(...) ,
#define P99__IS_fpos_t_EQ_fpos_t(...) ,
#define P99__IS_fprintf_EQ_fprintf(...) ,
#define P99__IS_fprintf_EQ_fprintf(...) ,
#define P99__IS_fputc_EQ_fputc(...) ,
#define P99__IS_fputchar_EQ_fputchar(...) ,
#define P99__IS_fputs_EQ_fputs(...) ,
#define P99__IS_fputwc_EQ_fputwc(...) ,
#define P99__IS_fputws_EQ_fputws(...) ,
#define P99__IS_fread_EQ_fread(...) ,
#define P99__IS_free_EQ_free(...) ,
#define P99__IS_freopen_EQ_freopen(...) ,
#define P99__IS_frexp_EQ_frexp(...) ,
#define P99__IS_fscanf_EQ_fscanf(...) ,
#define P99__IS_fscanf_EQ_fscanf(...) ,
#define P99__IS_fseek_EQ_fseek(...) ,
#define P99__IS_fsetpos_EQ_fsetpos(...) ,
#define P99__IS_ftell_EQ_ftell(...) ,
#define P99__IS_fwide_EQ_fwide(...) ,
#define P99__IS_fwprintf_EQ_fwprintf(...) ,
#define P99__IS_fwrite_EQ_fwrite(...) ,
#define P99__IS_fwscanf_EQ_fwscanf(...) ,
#define P99__IS_getaddrinfo_EQ_getaddrinfo(...) ,
#define P99__IS_getc_EQ_getc(...) ,
#define P99__IS_getchar_EQ_getchar(...) ,
#define P99__IS_getenv_EQ_getenv(...) ,
#define P99__IS_getnameinfo_EQ_getnameinfo(...) ,
#define P99__IS_gets_EQ_gets(...) ,
#define P99__IS_getwc_EQ_getwc(...) ,
#define P99__IS_getwchar_EQ_getwchar(...) ,
#define P99__IS_gmtime_EQ_gmtime(...) ,
#define P99__IS_goto_EQ_goto(...) ,
#define P99__IS_if_EQ_if(...) ,
#define P99__IS_imaxabs_EQ_imaxabs(...) ,
#define P99__IS_imaxdiv_EQ_imaxdiv(...) ,
#define P99__IS_imaxdiv_t_EQ_imaxdiv_t(...) ,
#define P99__IS_inline_EQ_inline(...) ,
#define P99__IS_int_EQ_int(...) ,
#define P99__IS_int16_t_EQ_int16_t(...) ,
#define P99__IS_int32_t_EQ_int32_t(...) ,
#define P99__IS_int64_t_EQ_int64_t(...) ,
#define P99__IS_int8_t_EQ_int8_t(...) ,
#define P99__IS_intfast16_t_EQ_intfast16_t(...) ,
#define P99__IS_intfast32_t_EQ_intfast32_t(...) ,
#define P99__IS_intfast64_t_EQ_intfast64_t(...) ,
#define P99__IS_intfast8_t_EQ_intfast8_t(...) ,
#define P99__IS_intleast16_t_EQ_intleast16_t(...) ,
#define P99__IS_intleast32_t_EQ_intleast32_t(...) ,
#define P99__IS_intleast64_t_EQ_intleast64_t(...) ,
#define P99__IS_intleast8_t_EQ_intleast8_t(...) ,
#define P99__IS_intmax_t_EQ_intmax_t(...) ,
#define P99__IS_intptr_t_EQ_intptr_t(...) ,
#define P99__IS_isalnum_EQ_isalnum(...) ,
#define P99__IS_isalpha_EQ_isalpha(...) ,
#define P99__IS_isblank_EQ_isblank(...) ,
#define P99__IS_iscntrl_EQ_iscntrl(...) ,
#define P99__IS_isdigit_EQ_isdigit(...) ,
#define P99__IS_isgraph_EQ_isgraph(...) ,
#define P99__IS_islower_EQ_islower(...) ,
#define P99__IS_isprint_EQ_isprint(...) ,
#define P99__IS_ispunct_EQ_ispunct(...) ,
#define P99__IS_isspace_EQ_isspace(...) ,
#define P99__IS_isupper_EQ_isupper(...) ,
#define P99__IS_iswalnum_EQ_iswalnum(...) ,
#define P99__IS_iswalpha_EQ_iswalpha(...) ,
#define P99__IS_iswcntrl_EQ_iswcntrl(...) ,
#define P99__IS_iswctype_EQ_iswctype(...) ,
#define P99__IS_iswdigit_EQ_iswdigit(...) ,
#define P99__IS_iswgraph_EQ_iswgraph(...) ,
#define P99__IS_iswlower_EQ_iswlower(...) ,
#define P99__IS_iswprint_EQ_iswprint(...) ,
#define P99__IS_iswpunct_EQ_iswpunct(...) ,
#define P99__IS_iswspace_EQ_iswspace(...) ,
#define P99__IS_iswupper_EQ_iswupper(...) ,
#define P99__IS_iswxdigit_EQ_iswxdigit(...) ,
#define P99__IS_isxdigit_EQ_isxdigit(...) ,
#define P99__IS_labs_EQ_labs(...) ,
#define P99__IS_ldexp_EQ_ldexp(...) ,
#define P99__IS_ldiv_EQ_ldiv(...) ,
#define P99__IS_ldiv_EQ_ldiv(...) ,
#define P99__IS_ldiv_t_EQ_ldiv_t(...) ,
#define P99__IS_lldiv_t_EQ_lldiv_t(...) ,
#define P99__IS_localeconv_EQ_localeconv(...) ,
#define P99__IS_localtime_EQ_localtime(...) ,
#define P99__IS_log_EQ_log(...) ,
#define P99__IS_log10_EQ_log10(...) ,
#define P99__IS_long_EQ_long(...) ,
#define P99__IS_longjmp_EQ_longjmp(...) ,
#define P99__IS_malloc_EQ_malloc(...) ,
#define P99__IS_mbrlen_EQ_mbrlen(...) ,
#define P99__IS_mbrtowc_EQ_mbrtowc(...) ,
#define P99__IS_mbsinit_EQ_mbsinit(...) ,
#define P99__IS_mbsrtowcs_EQ_mbsrtowcs(...) ,
#define P99__IS_mbstate_t_EQ_mbstate_t(...) ,
#define P99__IS_memchr_EQ_memchr(...) ,
#define P99__IS_memcmp_EQ_memcmp(...) ,
#define P99__IS_memcpy_EQ_memcpy(...) ,
#define P99__IS_memmove_EQ_memmove(...) ,
#define P99__IS_memset_EQ_memset(...) ,
#define P99__IS_mktime_EQ_mktime(...) ,
#define P99__IS_modf_EQ_modf(...) ,
#define P99__IS_off_t_EQ_off_t(...) ,
#define P99__IS_offsetof_EQ_offsetof(...) ,
#define P99__IS_perror_EQ_perror(...) ,
#define P99__IS_pow_EQ_pow(...) ,
#define P99__IS_printf_EQ_printf(...) ,
#define P99__IS_ptrdiff_t_EQ_ptrdiff_t(...) ,
#define P99__IS_putc_EQ_putc(...) ,
#define P99__IS_putchar_EQ_putchar(...) ,
#define P99__IS_puts_EQ_puts(...) ,
#define P99__IS_putwc_EQ_putwc(...) ,
#define P99__IS_putwchar_EQ_putwchar(...) ,
#define P99__IS_qsort_EQ_qsort(...) ,
#define P99__IS_raise_EQ_raise(...) ,
#define P99__IS_rand_EQ_rand(...) ,
#define P99__IS_realloc_EQ_realloc(...) ,
#define P99__IS_register_EQ_register(...) ,
#define P99__IS_remove_EQ_remove(...) ,
#define P99__IS_rename_EQ_rename(...) ,
#define P99__IS_restrict_EQ_restrict(...) ,
#define P99__IS_return_EQ_return(...) ,
#define P99__IS_rewind_EQ_rewind(...) ,
#define P99__IS_scanf_EQ_scanf(...) ,
#define P99__IS_setbuf_EQ_setbuf(...) ,
#define P99__IS_setjmp_EQ_setjmp(...) ,
#define P99__IS_setlocale_EQ_setlocale(...) ,
#define P99__IS_setvbuf_EQ_setvbuf(...) ,
#define P99__IS_short_EQ_short(...) ,
#define P99__IS_sig_atomic_t_EQ_sig_atomic_t(...) ,
#define P99__IS_signed_EQ_signed(...) ,
#define P99__IS_sin_EQ_sin(...) ,
#define P99__IS_sinh_EQ_sinh(...) ,
#define P99__IS_size_t_EQ_size_t(...) ,
#define P99__IS_sizeof_EQ_sizeof(...) ,
#define P99__IS_snprintf_EQ_snprintf(...) ,
#define P99__IS_sprintf_EQ_sprintf(...) ,
#define P99__IS_sqrt_EQ_sqrt(...) ,
#define P99__IS_srand_EQ_srand(...) ,
#define P99__IS_sscanf_EQ_sscanf(...) ,
#define P99__IS_ssize_t_EQ_ssize_t(...) ,
#define P99__IS_static_EQ_static(...) ,
#define P99__IS_strcat_EQ_strcat(...) ,
#define P99__IS_strchr_EQ_strchr(...) ,
#define P99__IS_strcmp_EQ_strcmp(...) ,
#define P99__IS_strcoll_EQ_strcoll(...) ,
#define P99__IS_strcpy_EQ_strcpy(...) ,
#define P99__IS_strcspn_EQ_strcspn(...) ,
#define P99__IS_strerror_EQ_strerror(...) ,
#define P99__IS_strftime_EQ_strftime(...) ,
#define P99__IS_strlen_EQ_strlen(...) ,
#define P99__IS_strncat_EQ_strncat(...) ,
#define P99__IS_strncmp_EQ_strncmp(...) ,
#define P99__IS_strncpy_EQ_strncpy(...) ,
#define P99__IS_strpbrk_EQ_strpbrk(...) ,
#define P99__IS_strrchr_EQ_strrchr(...) ,
#define P99__IS_strspn_EQ_strspn(...) ,
#define P99__IS_strstr_EQ_strstr(...) ,
#define P99__IS_strtod_EQ_strtod(...) ,
#define P99__IS_strtoimax_EQ_strtoimax(...) ,
#define P99__IS_strtok_EQ_strtok(...) ,
#define P99__IS_strtol_EQ_strtol(...) ,
#define P99__IS_strtoul_EQ_strtoul(...) ,
#define P99__IS_strtoumax_EQ_strtoumax(...) ,
#define P99__IS_struct_EQ_struct(...) ,
#define P99__IS_strxfrm_EQ_strxfrm(...) ,
#define P99__IS_switch_EQ_switch(...) ,
#define P99__IS_swprintf_EQ_swprintf(...) ,
#define P99__IS_swscanf_EQ_swscanf(...) ,
#define P99__IS_system_EQ_system(...) ,
#define P99__IS_tan_EQ_tan(...) ,
#define P99__IS_tanh_EQ_tanh(...) ,
#define P99__IS_time_EQ_time(...) ,
#define P99__IS_time_t_EQ_time_t(...) ,
#define P99__IS_tmpfile_EQ_tmpfile(...) ,
#define P99__IS_tmpnam_EQ_tmpnam(...) ,
#define P99__IS_tolower_EQ_tolower(...) ,
#define P99__IS_toupper_EQ_toupper(...) ,
#define P99__IS_towctrans_EQ_towctrans(...) ,
#define P99__IS_towlower_EQ_towlower(...) ,
#define P99__IS_towupper_EQ_towupper(...) ,
#define P99__IS_typedef_EQ_typedef(...) ,
#define P99__IS_uint16_t_EQ_uint16_t(...) ,
#define P99__IS_uint32_t_EQ_uint32_t(...) ,
#define P99__IS_uint64_t_EQ_uint64_t(...) ,
#define P99__IS_uint8_t_EQ_uint8_t(...) ,
#define P99__IS_uintfast16_t_EQ_uintfast16_t(...) ,
#define P99__IS_uintfast32_t_EQ_uintfast32_t(...) ,
#define P99__IS_uintfast64_t_EQ_uintfast64_t(...) ,
#define P99__IS_uintfast8_t_EQ_uintfast8_t(...) ,
#define P99__IS_uintleast16_t_EQ_uintleast16_t(...) ,
#define P99__IS_uintleast32_t_EQ_uintleast32_t(...) ,
#define P99__IS_uintleast64_t_EQ_uintleast64_t(...) ,
#define P99__IS_uintleast8_t_EQ_uintleast8_t(...) ,
#define P99__IS_uintmax_t_EQ_uintmax_t(...) ,
#define P99__IS_uintptr_t_EQ_uintptr_t(...) ,
#define P99__IS_ungetc_EQ_ungetc(...) ,
#define P99__IS_ungetwc_EQ_ungetwc(...) ,
#define P99__IS_union_EQ_union(...) ,
#define P99__IS_unsigned_EQ_unsigned(...) ,
#define P99__IS_va_arg_EQ_va_arg(...) ,
#define P99__IS_va_copy_EQ_va_copy(...) ,
#define P99__IS_va_end_EQ_va_end(...) ,
#define P99__IS_va_start_EQ_va_start(...) ,
#define P99__IS_vfprintf_EQ_vfprintf(...) ,
#define P99__IS_vfscanf_EQ_vfscanf(...) ,
#define P99__IS_vfwprintf_EQ_vfwprintf(...) ,
#define P99__IS_void_EQ_void(...) ,
#define P99__IS_volatile_EQ_volatile(...) ,
#define P99__IS_vprintf_EQ_vprintf(...) ,
#define P99__IS_vscanf_EQ_vscanf(...) ,
#define P99__IS_vsprintf_EQ_vsprintf(...) ,
#define P99__IS_vsscanf_EQ_vsscanf(...) ,
#define P99__IS_vswprintf_EQ_vswprintf(...) ,
#define P99__IS_vwprintf_EQ_vwprintf(...) ,
#define P99__IS_wchar_t_EQ_wchar_t(...) ,
#define P99__IS_wcrtomb_EQ_wcrtomb(...) ,
#define P99__IS_wcscat_EQ_wcscat(...) ,
#define P99__IS_wcschr_EQ_wcschr(...) ,
#define P99__IS_wcscmp_EQ_wcscmp(...) ,
#define P99__IS_wcscoll_EQ_wcscoll(...) ,
#define P99__IS_wcscpy_EQ_wcscpy(...) ,
#define P99__IS_wcscspn_EQ_wcscspn(...) ,
#define P99__IS_wcsftime_EQ_wcsftime(...) ,
#define P99__IS_wcslen_EQ_wcslen(...) ,
#define P99__IS_wcsncat_EQ_wcsncat(...) ,
#define P99__IS_wcsncmp_EQ_wcsncmp(...) ,
#define P99__IS_wcsncpy_EQ_wcsncpy(...) ,
#define P99__IS_wcspbrk_EQ_wcspbrk(...) ,
#define P99__IS_wcsrchr_EQ_wcsrchr(...) ,
#define P99__IS_wcsrtombs_EQ_wcsrtombs(...) ,
#define P99__IS_wcsspn_EQ_wcsspn(...) ,
#define P99__IS_wcsstr_EQ_wcsstr(...) ,
#define P99__IS_wcstod_EQ_wcstod(...) ,
#define P99__IS_wcstoimax_EQ_wcstoimax(...) ,
#define P99__IS_wcstok_EQ_wcstok(...) ,
#define P99__IS_wcstol_EQ_wcstol(...) ,
#define P99__IS_wcstoul_EQ_wcstoul(...) ,
#define P99__IS_wcstoumax_EQ_wcstoumax(...) ,
#define P99__IS_wcsxfrm_EQ_wcsxfrm(...) ,
#define P99__IS_wctob_EQ_wctob(...) ,
#define P99__IS_wctrans_EQ_wctrans(...) ,
#define P99__IS_wctrans_t_EQ_wctrans_t(...) ,
#define P99__IS_wctype_EQ_wctype(...) ,
#define P99__IS_wctype_t_EQ_wctype_t(...) ,
#define P99__IS_while_EQ_while(...) ,
#define P99__IS_wint_t_EQ_wint_t(...) ,
#define P99__IS_wmemchr_EQ_wmemchr(...) ,
#define P99__IS_wmemcmp_EQ_wmemcmp(...) ,
#define P99__IS_wmemcpy_EQ_wmemcpy(...) ,
#define P99__IS_wmemmove_EQ_wmemmove(...) ,
#define P99__IS_wmemset_EQ_wmemset(...) ,
#define P99__IS_wprintf_EQ_wprintf(...) ,
#define P99__IS_wscanf_EQ_wscanf(...) ,
#define P99__TOK_0_STARTS_0 ,
#define P99__TOK_1_STARTS_1 ,
#define P99__TOK_2_STARTS_2 ,
#define P99__TOK_3_STARTS_3 ,
#define P99__TOK_4_STARTS_4 ,
#define P99__TOK_5_STARTS_5 ,
#define P99__TOK_6_STARTS_6 ,
#define P99__TOK_7_STARTS_7 ,
#define P99__TOK_8_STARTS_8 ,
#define P99__TOK_9_STARTS_9 ,
#define P99__TOK_10_STARTS_10 ,
#define P99__TOK_11_STARTS_11 ,
#define P99__TOK_12_STARTS_12 ,
#define P99__TOK_13_STARTS_13 ,
#define P99__TOK_14_STARTS_14 ,
#define P99__TOK_15_STARTS_15 ,
#define P99__TOK_16_STARTS_16 ,
#define P99__TOK_17_STARTS_17 ,
#define P99__TOK_18_STARTS_18 ,
#define P99__TOK_19_STARTS_19 ,
#define P99__TOK_20_STARTS_20 ,
#define P99__TOK_21_STARTS_21 ,
#define P99__TOK_22_STARTS_22 ,
#define P99__TOK_23_STARTS_23 ,
#define P99__TOK_24_STARTS_24 ,
#define P99__TOK_25_STARTS_25 ,
#define P99__TOK_26_STARTS_26 ,
#define P99__TOK_27_STARTS_27 ,
#define P99__TOK_28_STARTS_28 ,
#define P99__TOK_29_STARTS_29 ,
#define P99__TOK_30_STARTS_30 ,
#define P99__TOK_31_STARTS_31 ,
#define P99__TOK_32_STARTS_32 ,
#define P99__TOK_33_STARTS_33 ,
#define P99__TOK_34_STARTS_34 ,
#define P99__TOK_35_STARTS_35 ,
#define P99__TOK_36_STARTS_36 ,
#define P99__TOK_37_STARTS_37 ,
#define P99__TOK_38_STARTS_38 ,
#define P99__TOK_39_STARTS_39 ,
#define P99__TOK_40_STARTS_40 ,
#define P99__TOK_41_STARTS_41 ,
#define P99__TOK_42_STARTS_42 ,
#define P99__TOK_43_STARTS_43 ,
#define P99__TOK_44_STARTS_44 ,
#define P99__TOK_45_STARTS_45 ,
#define P99__TOK_46_STARTS_46 ,
#define P99__TOK_47_STARTS_47 ,
#define P99__TOK_48_STARTS_48 ,
#define P99__TOK_49_STARTS_49 ,
#define P99__TOK_50_STARTS_50 ,
#define P99__TOK_51_STARTS_51 ,
#define P99__TOK_52_STARTS_52 ,
#define P99__TOK_53_STARTS_53 ,
#define P99__TOK_54_STARTS_54 ,
#define P99__TOK_55_STARTS_55 ,
#define P99__TOK_56_STARTS_56 ,
#define P99__TOK_57_STARTS_57 ,
#define P99__TOK_58_STARTS_58 ,
#define P99__TOK_59_STARTS_59 ,
#define P99__TOK_60_STARTS_60 ,
#define P99__TOK_61_STARTS_61 ,
#define P99__TOK_62_STARTS_62 ,
#define P99__TOK_63_STARTS_63 ,
#define P99__TOK_64_STARTS_64 ,
#define P99__TOK_65_STARTS_65 ,
#define P99__TOK_66_STARTS_66 ,
#define P99__TOK_67_STARTS_67 ,
#define P99__TOK_68_STARTS_68 ,
#define P99__TOK_69_STARTS_69 ,
#define P99__TOK_70_STARTS_70 ,
#define P99__TOK_71_STARTS_71 ,
#define P99__TOK_72_STARTS_72 ,
#define P99__TOK_73_STARTS_73 ,
#define P99__TOK_74_STARTS_74 ,
#define P99__TOK_75_STARTS_75 ,
#define P99__TOK_76_STARTS_76 ,
#define P99__TOK_77_STARTS_77 ,
#define P99__TOK_78_STARTS_78 ,
#define P99__TOK_79_STARTS_79 ,
#define P99__TOK_80_STARTS_80 ,
#define P99__TOK_81_STARTS_81 ,
#define P99__TOK_82_STARTS_82 ,
#define P99__TOK_83_STARTS_83 ,
#define P99__TOK_84_STARTS_84 ,
#define P99__TOK_85_STARTS_85 ,
#define P99__TOK_86_STARTS_86 ,
#define P99__TOK_87_STARTS_87 ,
#define P99__TOK_88_STARTS_88 ,
#define P99__TOK_89_STARTS_89 ,
#define P99__TOK_90_STARTS_90 ,
#define P99__TOK_91_STARTS_91 ,
#define P99__TOK_92_STARTS_92 ,
#define P99__TOK_93_STARTS_93 ,
#define P99__TOK_94_STARTS_94 ,
#define P99__TOK_95_STARTS_95 ,
#define P99__TOK_96_STARTS_96 ,
#define P99__TOK_97_STARTS_97 ,
#define P99__TOK_98_STARTS_98 ,
#define P99__TOK_99_STARTS_99 ,
#define P99__TOK_100_STARTS_100 ,
#define P99__TOK_101_STARTS_101 ,
#define P99__TOK_102_STARTS_102 ,
#define P99__TOK_103_STARTS_103 ,
#define P99__TOK_104_STARTS_104 ,
#define P99__TOK_105_STARTS_105 ,
#define P99__TOK_106_STARTS_106 ,
#define P99__TOK_107_STARTS_107 ,
#define P99__TOK_108_STARTS_108 ,
#define P99__TOK_109_STARTS_109 ,
#define P99__TOK_110_STARTS_110 ,
#define P99__TOK_111_STARTS_111 ,
#define P99__TOK_112_STARTS_112 ,
#define P99__TOK_113_STARTS_113 ,
#define P99__TOK_114_STARTS_114 ,
#define P99__TOK_115_STARTS_115 ,
#define P99__TOK_116_STARTS_116 ,
#define P99__TOK_117_STARTS_117 ,
#define P99__TOK_118_STARTS_118 ,
#define P99__TOK_119_STARTS_119 ,
#define P99__TOK_120_STARTS_120 ,
#define P99__TOK_121_STARTS_121 ,
#define P99__TOK__Bool_STARTS__Bool ,
#define P99__TOK__Complex_STARTS__Complex ,
#define P99__TOK__Exit_STARTS__Exit ,
#define P99__TOK__Imaginary_STARTS__Imaginary ,
#define P99__TOK_abort_STARTS_abort ,
#define P99__TOK_abs_STARTS_abs ,
#define P99__TOK_acos_STARTS_acos ,
#define P99__TOK_asctime_STARTS_asctime ,
#define P99__TOK_asin_STARTS_asin ,
#define P99__TOK_assert_STARTS_assert ,
#define P99__TOK_atan_STARTS_atan ,
#define P99__TOK_atan2_STARTS_atan2 ,
#define P99__TOK_atexit_STARTS_atexit ,
#define P99__TOK_atof_STARTS_atof ,
#define P99__TOK_atof_STARTS_atof ,
#define P99__TOK_atoi_STARTS_atoi ,
#define P99__TOK_atol_STARTS_atol ,
#define P99__TOK_auto_STARTS_auto ,
#define P99__TOK_break_STARTS_break ,
#define P99__TOK_bsearch_STARTS_bsearch ,
#define P99__TOK_btowc_STARTS_btowc ,
#define P99__TOK_cabs_STARTS_cabs ,
#define P99__TOK_cacos_STARTS_cacos ,
#define P99__TOK_cacosh_STARTS_cacosh ,
#define P99__TOK_calloc_STARTS_calloc ,
#define P99__TOK_carg_STARTS_carg ,
#define P99__TOK_case_STARTS_case ,
#define P99__TOK_casin_STARTS_casin ,
#define P99__TOK_casinh_STARTS_casinh ,
#define P99__TOK_catan_STARTS_catan ,
#define P99__TOK_catanh_STARTS_catanh ,
#define P99__TOK_ccos_STARTS_ccos ,
#define P99__TOK_ccosh_STARTS_ccosh ,
#define P99__TOK_ceil_STARTS_ceil ,
#define P99__TOK_cexp_STARTS_cexp ,
#define P99__TOK_char_STARTS_char ,
#define P99__TOK_cimag_STARTS_cimag ,
#define P99__TOK_clearerr_STARTS_clearerr ,
#define P99__TOK_clock_STARTS_clock ,
#define P99__TOK_clock_t_STARTS_clock_t ,
#define P99__TOK_clog_STARTS_clog ,
#define P99__TOK_conj_STARTS_conj ,
#define P99__TOK_const_STARTS_const ,
#define P99__TOK_continue_STARTS_continue ,
#define P99__TOK_cos_STARTS_cos ,
#define P99__TOK_cosh_STARTS_cosh ,
#define P99__TOK_cpow_STARTS_cpow ,
#define P99__TOK_cproj_STARTS_cproj ,
#define P99__TOK_creal_STARTS_creal ,
#define P99__TOK_csin_STARTS_csin ,
#define P99__TOK_csinh_STARTS_csinh ,
#define P99__TOK_csqrt_STARTS_csqrt ,
#define P99__TOK_ctan_STARTS_ctan ,
#define P99__TOK_ctanh_STARTS_ctanh ,
#define P99__TOK_ctime_STARTS_ctime ,
#define P99__TOK_default_STARTS_default ,
#define P99__TOK_difftime_STARTS_difftime ,
#define P99__TOK_div_STARTS_div ,
#define P99__TOK_div_t_STARTS_div_t ,
#define P99__TOK_do_STARTS_do ,
#define P99__TOK_double_STARTS_double ,
#define P99__TOK_double_t_STARTS_double_t ,
#define P99__TOK_else_STARTS_else ,
#define P99__TOK_enum_STARTS_enum ,
#define P99__TOK_exit_STARTS_exit ,
#define P99__TOK_exp_STARTS_exp ,
#define P99__TOK_extern_STARTS_extern ,
#define P99__TOK_fabs_STARTS_fabs ,
#define P99__TOK_fclose_STARTS_fclose ,
#define P99__TOK_fdopen_STARTS_fdopen ,
#define P99__TOK_fenv_t_STARTS_fenv_t ,
#define P99__TOK_feof_STARTS_feof ,
#define P99__TOK_ferror_STARTS_ferror ,
#define P99__TOK_fexcept_t_STARTS_fexcept_t ,
#define P99__TOK_fflush_STARTS_fflush ,
#define P99__TOK_fgetc_STARTS_fgetc ,
#define P99__TOK_fgetpos_STARTS_fgetpos ,
#define P99__TOK_fgets_STARTS_fgets ,
#define P99__TOK_fgetwc_STARTS_fgetwc ,
#define P99__TOK_fgetws_STARTS_fgetws ,
#define P99__TOK_float_STARTS_float ,
#define P99__TOK_float_t_STARTS_float_t ,
#define P99__TOK_floor_STARTS_floor ,
#define P99__TOK_fopen_STARTS_fopen ,
#define P99__TOK_for_STARTS_for ,
#define P99__TOK_fpos_t_STARTS_fpos_t ,
#define P99__TOK_fprintf_STARTS_fprintf ,
#define P99__TOK_fprintf_STARTS_fprintf ,
#define P99__TOK_fputc_STARTS_fputc ,
#define P99__TOK_fputchar_STARTS_fputchar ,
#define P99__TOK_fputs_STARTS_fputs ,
#define P99__TOK_fputwc_STARTS_fputwc ,
#define P99__TOK_fputws_STARTS_fputws ,
#define P99__TOK_fread_STARTS_fread ,
#define P99__TOK_free_STARTS_free ,
#define P99__TOK_freopen_STARTS_freopen ,
#define P99__TOK_frexp_STARTS_frexp ,
#define P99__TOK_fscanf_STARTS_fscanf ,
#define P99__TOK_fscanf_STARTS_fscanf ,
#define P99__TOK_fseek_STARTS_fseek ,
#define P99__TOK_fsetpos_STARTS_fsetpos ,
#define P99__TOK_ftell_STARTS_ftell ,
#define P99__TOK_fwide_STARTS_fwide ,
#define P99__TOK_fwprintf_STARTS_fwprintf ,
#define P99__TOK_fwrite_STARTS_fwrite ,
#define P99__TOK_fwscanf_STARTS_fwscanf ,
#define P99__TOK_getaddrinfo_STARTS_getaddrinfo ,
#define P99__TOK_getc_STARTS_getc ,
#define P99__TOK_getchar_STARTS_getchar ,
#define P99__TOK_getenv_STARTS_getenv ,
#define P99__TOK_getnameinfo_STARTS_getnameinfo ,
#define P99__TOK_gets_STARTS_gets ,
#define P99__TOK_getwc_STARTS_getwc ,
#define P99__TOK_getwchar_STARTS_getwchar ,
#define P99__TOK_gmtime_STARTS_gmtime ,
#define P99__TOK_goto_STARTS_goto ,
#define P99__TOK_if_STARTS_if ,
#define P99__TOK_imaxabs_STARTS_imaxabs ,
#define P99__TOK_imaxdiv_STARTS_imaxdiv ,
#define P99__TOK_imaxdiv_t_STARTS_imaxdiv_t ,
#define P99__TOK_inline_STARTS_inline ,
#define P99__TOK_int_STARTS_int ,
#define P99__TOK_int16_t_STARTS_int16_t ,
#define P99__TOK_int32_t_STARTS_int32_t ,
#define P99__TOK_int64_t_STARTS_int64_t ,
#define P99__TOK_int8_t_STARTS_int8_t ,
#define P99__TOK_intfast16_t_STARTS_intfast16_t ,
#define P99__TOK_intfast32_t_STARTS_intfast32_t ,
#define P99__TOK_intfast64_t_STARTS_intfast64_t ,
#define P99__TOK_intfast8_t_STARTS_intfast8_t ,
#define P99__TOK_intleast16_t_STARTS_intleast16_t ,
#define P99__TOK_intleast32_t_STARTS_intleast32_t ,
#define P99__TOK_intleast64_t_STARTS_intleast64_t ,
#define P99__TOK_intleast8_t_STARTS_intleast8_t ,
#define P99__TOK_intmax_t_STARTS_intmax_t ,
#define P99__TOK_intptr_t_STARTS_intptr_t ,
#define P99__TOK_isalnum_STARTS_isalnum ,
#define P99__TOK_isalpha_STARTS_isalpha ,
#define P99__TOK_isblank_STARTS_isblank ,
#define P99__TOK_iscntrl_STARTS_iscntrl ,
#define P99__TOK_isdigit_STARTS_isdigit ,
#define P99__TOK_isgraph_STARTS_isgraph ,
#define P99__TOK_islower_STARTS_islower ,
#define P99__TOK_isprint_STARTS_isprint ,
#define P99__TOK_ispunct_STARTS_ispunct ,
#define P99__TOK_isspace_STARTS_isspace ,
#define P99__TOK_isupper_STARTS_isupper ,
#define P99__TOK_iswalnum_STARTS_iswalnum ,
#define P99__TOK_iswalpha_STARTS_iswalpha ,
#define P99__TOK_iswcntrl_STARTS_iswcntrl ,
#define P99__TOK_iswctype_STARTS_iswctype ,
#define P99__TOK_iswdigit_STARTS_iswdigit ,
#define P99__TOK_iswgraph_STARTS_iswgraph ,
#define P99__TOK_iswlower_STARTS_iswlower ,
#define P99__TOK_iswprint_STARTS_iswprint ,
#define P99__TOK_iswpunct_STARTS_iswpunct ,
#define P99__TOK_iswspace_STARTS_iswspace ,
#define P99__TOK_iswupper_STARTS_iswupper ,
#define P99__TOK_iswxdigit_STARTS_iswxdigit ,
#define P99__TOK_isxdigit_STARTS_isxdigit ,
#define P99__TOK_labs_STARTS_labs ,
#define P99__TOK_ldexp_STARTS_ldexp ,
#define P99__TOK_ldiv_STARTS_ldiv ,
#define P99__TOK_ldiv_STARTS_ldiv ,
#define P99__TOK_ldiv_t_STARTS_ldiv_t ,
#define P99__TOK_lldiv_t_STARTS_lldiv_t ,
#define P99__TOK_localeconv_STARTS_localeconv ,
#define P99__TOK_localtime_STARTS_localtime ,
#define P99__TOK_log_STARTS_log ,
#define P99__TOK_log10_STARTS_log10 ,
#define P99__TOK_long_STARTS_long ,
#define P99__TOK_longjmp_STARTS_longjmp ,
#define P99__TOK_malloc_STARTS_malloc ,
#define P99__TOK_mbrlen_STARTS_mbrlen ,
#define P99__TOK_mbrtowc_STARTS_mbrtowc ,
#define P99__TOK_mbsinit_STARTS_mbsinit ,
#define P99__TOK_mbsrtowcs_STARTS_mbsrtowcs ,
#define P99__TOK_mbstate_t_STARTS_mbstate_t ,
#define P99__TOK_memchr_STARTS_memchr ,
#define P99__TOK_memcmp_STARTS_memcmp ,
#define P99__TOK_memcpy_STARTS_memcpy ,
#define P99__TOK_memmove_STARTS_memmove ,
#define P99__TOK_memset_STARTS_memset ,
#define P99__TOK_mktime_STARTS_mktime ,
#define P99__TOK_modf_STARTS_modf ,
#define P99__TOK_off_t_STARTS_off_t ,
#define P99__TOK_offsetof_STARTS_offsetof ,
#define P99__TOK_perror_STARTS_perror ,
#define P99__TOK_pow_STARTS_pow ,
#define P99__TOK_printf_STARTS_printf ,
#define P99__TOK_ptrdiff_t_STARTS_ptrdiff_t ,
#define P99__TOK_putc_STARTS_putc ,
#define P99__TOK_putchar_STARTS_putchar ,
#define P99__TOK_puts_STARTS_puts ,
#define P99__TOK_putwc_STARTS_putwc ,
#define P99__TOK_putwchar_STARTS_putwchar ,
#define P99__TOK_qsort_STARTS_qsort ,
#define P99__TOK_raise_STARTS_raise ,
#define P99__TOK_rand_STARTS_rand ,
#define P99__TOK_realloc_STARTS_realloc ,
#define P99__TOK_register_STARTS_register ,
#define P99__TOK_remove_STARTS_remove ,
#define P99__TOK_rename_STARTS_rename ,
#define P99__TOK_restrict_STARTS_restrict ,
#define P99__TOK_return_STARTS_return ,
#define P99__TOK_rewind_STARTS_rewind ,
#define P99__TOK_scanf_STARTS_scanf ,
#define P99__TOK_setbuf_STARTS_setbuf ,
#define P99__TOK_setjmp_STARTS_setjmp ,
#define P99__TOK_setlocale_STARTS_setlocale ,
#define P99__TOK_setvbuf_STARTS_setvbuf ,
#define P99__TOK_short_STARTS_short ,
#define P99__TOK_sig_atomic_t_STARTS_sig_atomic_t ,
#define P99__TOK_signed_STARTS_signed ,
#define P99__TOK_sin_STARTS_sin ,
#define P99__TOK_sinh_STARTS_sinh ,
#define P99__TOK_size_t_STARTS_size_t ,
#define P99__TOK_sizeof_STARTS_sizeof ,
#define P99__TOK_snprintf_STARTS_snprintf ,
#define P99__TOK_sprintf_STARTS_sprintf ,
#define P99__TOK_sqrt_STARTS_sqrt ,
#define P99__TOK_srand_STARTS_srand ,
#define P99__TOK_sscanf_STARTS_sscanf ,
#define P99__TOK_ssize_t_STARTS_ssize_t ,
#define P99__TOK_static_STARTS_static ,
#define P99__TOK_strcat_STARTS_strcat ,
#define P99__TOK_strchr_STARTS_strchr ,
#define P99__TOK_strcmp_STARTS_strcmp ,
#define P99__TOK_strcoll_STARTS_strcoll ,
#define P99__TOK_strcpy_STARTS_strcpy ,
#define P99__TOK_strcspn_STARTS_strcspn ,
#define P99__TOK_strerror_STARTS_strerror ,
#define P99__TOK_strftime_STARTS_strftime ,
#define P99__TOK_strlen_STARTS_strlen ,
#define P99__TOK_strncat_STARTS_strncat ,
#define P99__TOK_strncmp_STARTS_strncmp ,
#define P99__TOK_strncpy_STARTS_strncpy ,
#define P99__TOK_strpbrk_STARTS_strpbrk ,
#define P99__TOK_strrchr_STARTS_strrchr ,
#define P99__TOK_strspn_STARTS_strspn ,
#define P99__TOK_strstr_STARTS_strstr ,
#define P99__TOK_strtod_STARTS_strtod ,
#define P99__TOK_strtoimax_STARTS_strtoimax ,
#define P99__TOK_strtok_STARTS_strtok ,
#define P99__TOK_strtol_STARTS_strtol ,
#define P99__TOK_strtoul_STARTS_strtoul ,
#define P99__TOK_strtoumax_STARTS_strtoumax ,
#define P99__TOK_struct_STARTS_struct ,
#define P99__TOK_strxfrm_STARTS_strxfrm ,
#define P99__TOK_switch_STARTS_switch ,
#define P99__TOK_swprintf_STARTS_swprintf ,
#define P99__TOK_swscanf_STARTS_swscanf ,
#define P99__TOK_system_STARTS_system ,
#define P99__TOK_tan_STARTS_tan ,
#define P99__TOK_tanh_STARTS_tanh ,
#define P99__TOK_time_STARTS_time ,
#define P99__TOK_time_t_STARTS_time_t ,
#define P99__TOK_tmpfile_STARTS_tmpfile ,
#define P99__TOK_tmpnam_STARTS_tmpnam ,
#define P99__TOK_tolower_STARTS_tolower ,
#define P99__TOK_toupper_STARTS_toupper ,
#define P99__TOK_towctrans_STARTS_towctrans ,
#define P99__TOK_towlower_STARTS_towlower ,
#define P99__TOK_towupper_STARTS_towupper ,
#define P99__TOK_typedef_STARTS_typedef ,
#define P99__TOK_uint16_t_STARTS_uint16_t ,
#define P99__TOK_uint32_t_STARTS_uint32_t ,
#define P99__TOK_uint64_t_STARTS_uint64_t ,
#define P99__TOK_uint8_t_STARTS_uint8_t ,
#define P99__TOK_uintfast16_t_STARTS_uintfast16_t ,
#define P99__TOK_uintfast32_t_STARTS_uintfast32_t ,
#define P99__TOK_uintfast64_t_STARTS_uintfast64_t ,
#define P99__TOK_uintfast8_t_STARTS_uintfast8_t ,
#define P99__TOK_uintleast16_t_STARTS_uintleast16_t ,
#define P99__TOK_uintleast32_t_STARTS_uintleast32_t ,
#define P99__TOK_uintleast64_t_STARTS_uintleast64_t ,
#define P99__TOK_uintleast8_t_STARTS_uintleast8_t ,
#define P99__TOK_uintmax_t_STARTS_uintmax_t ,
#define P99__TOK_uintptr_t_STARTS_uintptr_t ,
#define P99__TOK_ungetc_STARTS_ungetc ,
#define P99__TOK_ungetwc_STARTS_ungetwc ,
#define P99__TOK_union_STARTS_union ,
#define P99__TOK_unsigned_STARTS_unsigned ,
#define P99__TOK_va_arg_STARTS_va_arg ,
#define P99__TOK_va_copy_STARTS_va_copy ,
#define P99__TOK_va_end_STARTS_va_end ,
#define P99__TOK_va_start_STARTS_va_start ,
#define P99__TOK_vfprintf_STARTS_vfprintf ,
#define P99__TOK_vfscanf_STARTS_vfscanf ,
#define P99__TOK_vfwprintf_STARTS_vfwprintf ,
#define P99__TOK_void_STARTS_void ,
#define P99__TOK_volatile_STARTS_volatile ,
#define P99__TOK_vprintf_STARTS_vprintf ,
#define P99__TOK_vscanf_STARTS_vscanf ,
#define P99__TOK_vsprintf_STARTS_vsprintf ,
#define P99__TOK_vsscanf_STARTS_vsscanf ,
#define P99__TOK_vswprintf_STARTS_vswprintf ,
#define P99__TOK_vwprintf_STARTS_vwprintf ,
#define P99__TOK_wchar_t_STARTS_wchar_t ,
#define P99__TOK_wcrtomb_STARTS_wcrtomb ,
#define P99__TOK_wcscat_STARTS_wcscat ,
#define P99__TOK_wcschr_STARTS_wcschr ,
#define P99__TOK_wcscmp_STARTS_wcscmp ,
#define P99__TOK_wcscoll_STARTS_wcscoll ,
#define P99__TOK_wcscpy_STARTS_wcscpy ,
#define P99__TOK_wcscspn_STARTS_wcscspn ,
#define P99__TOK_wcsftime_STARTS_wcsftime ,
#define P99__TOK_wcslen_STARTS_wcslen ,
#define P99__TOK_wcsncat_STARTS_wcsncat ,
#define P99__TOK_wcsncmp_STARTS_wcsncmp ,
#define P99__TOK_wcsncpy_STARTS_wcsncpy ,
#define P99__TOK_wcspbrk_STARTS_wcspbrk ,
#define P99__TOK_wcsrchr_STARTS_wcsrchr ,
#define P99__TOK_wcsrtombs_STARTS_wcsrtombs ,
#define P99__TOK_wcsspn_STARTS_wcsspn ,
#define P99__TOK_wcsstr_STARTS_wcsstr ,
#define P99__TOK_wcstod_STARTS_wcstod ,
#define P99__TOK_wcstoimax_STARTS_wcstoimax ,
#define P99__TOK_wcstok_STARTS_wcstok ,
#define P99__TOK_wcstol_STARTS_wcstol ,
#define P99__TOK_wcstoul_STARTS_wcstoul ,
#define P99__TOK_wcstoumax_STARTS_wcstoumax ,
#define P99__TOK_wcsxfrm_STARTS_wcsxfrm ,
#define P99__TOK_wctob_STARTS_wctob ,
#define P99__TOK_wctrans_STARTS_wctrans ,
#define P99__TOK_wctrans_t_STARTS_wctrans_t ,
#define P99__TOK_wctype_STARTS_wctype ,
#define P99__TOK_wctype_t_STARTS_wctype_t ,
#define P99__TOK_while_STARTS_while ,
#define P99__TOK_wint_t_STARTS_wint_t ,
#define P99__TOK_wmemchr_STARTS_wmemchr ,
#define P99__TOK_wmemcmp_STARTS_wmemcmp ,
#define P99__TOK_wmemcpy_STARTS_wmemcpy ,
#define P99__TOK_wmemmove_STARTS_wmemmove ,
#define P99__TOK_wmemset_STARTS_wmemset ,
#define P99__TOK_wprintf_STARTS_wprintf ,
#define P99__TOK_wscanf_STARTS_wscanf ,
/*! @brief Test if the argument consists of exactly the token @c 0 */
#define P99_IS_EQ_0(...) P99_TOK_EQ(0, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 1 */
#define P99_IS_EQ_1(...) P99_TOK_EQ(1, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 2 */
#define P99_IS_EQ_2(...) P99_TOK_EQ(2, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 3 */
#define P99_IS_EQ_3(...) P99_TOK_EQ(3, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 4 */
#define P99_IS_EQ_4(...) P99_TOK_EQ(4, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 5 */
#define P99_IS_EQ_5(...) P99_TOK_EQ(5, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 6 */
#define P99_IS_EQ_6(...) P99_TOK_EQ(6, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 7 */
#define P99_IS_EQ_7(...) P99_TOK_EQ(7, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 8 */
#define P99_IS_EQ_8(...) P99_TOK_EQ(8, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 9 */
#define P99_IS_EQ_9(...) P99_TOK_EQ(9, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 10 */
#define P99_IS_EQ_10(...) P99_TOK_EQ(10, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 11 */
#define P99_IS_EQ_11(...) P99_TOK_EQ(11, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 12 */
#define P99_IS_EQ_12(...) P99_TOK_EQ(12, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 13 */
#define P99_IS_EQ_13(...) P99_TOK_EQ(13, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 14 */
#define P99_IS_EQ_14(...) P99_TOK_EQ(14, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 15 */
#define P99_IS_EQ_15(...) P99_TOK_EQ(15, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 16 */
#define P99_IS_EQ_16(...) P99_TOK_EQ(16, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 17 */
#define P99_IS_EQ_17(...) P99_TOK_EQ(17, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 18 */
#define P99_IS_EQ_18(...) P99_TOK_EQ(18, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 19 */
#define P99_IS_EQ_19(...) P99_TOK_EQ(19, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 20 */
#define P99_IS_EQ_20(...) P99_TOK_EQ(20, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 21 */
#define P99_IS_EQ_21(...) P99_TOK_EQ(21, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 22 */
#define P99_IS_EQ_22(...) P99_TOK_EQ(22, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 23 */
#define P99_IS_EQ_23(...) P99_TOK_EQ(23, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 24 */
#define P99_IS_EQ_24(...) P99_TOK_EQ(24, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 25 */
#define P99_IS_EQ_25(...) P99_TOK_EQ(25, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 26 */
#define P99_IS_EQ_26(...) P99_TOK_EQ(26, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 27 */
#define P99_IS_EQ_27(...) P99_TOK_EQ(27, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 28 */
#define P99_IS_EQ_28(...) P99_TOK_EQ(28, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 29 */
#define P99_IS_EQ_29(...) P99_TOK_EQ(29, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 30 */
#define P99_IS_EQ_30(...) P99_TOK_EQ(30, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 31 */
#define P99_IS_EQ_31(...) P99_TOK_EQ(31, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 32 */
#define P99_IS_EQ_32(...) P99_TOK_EQ(32, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 33 */
#define P99_IS_EQ_33(...) P99_TOK_EQ(33, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 34 */
#define P99_IS_EQ_34(...) P99_TOK_EQ(34, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 35 */
#define P99_IS_EQ_35(...) P99_TOK_EQ(35, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 36 */
#define P99_IS_EQ_36(...) P99_TOK_EQ(36, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 37 */
#define P99_IS_EQ_37(...) P99_TOK_EQ(37, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 38 */
#define P99_IS_EQ_38(...) P99_TOK_EQ(38, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 39 */
#define P99_IS_EQ_39(...) P99_TOK_EQ(39, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 40 */
#define P99_IS_EQ_40(...) P99_TOK_EQ(40, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 41 */
#define P99_IS_EQ_41(...) P99_TOK_EQ(41, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 42 */
#define P99_IS_EQ_42(...) P99_TOK_EQ(42, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 43 */
#define P99_IS_EQ_43(...) P99_TOK_EQ(43, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 44 */
#define P99_IS_EQ_44(...) P99_TOK_EQ(44, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 45 */
#define P99_IS_EQ_45(...) P99_TOK_EQ(45, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 46 */
#define P99_IS_EQ_46(...) P99_TOK_EQ(46, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 47 */
#define P99_IS_EQ_47(...) P99_TOK_EQ(47, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 48 */
#define P99_IS_EQ_48(...) P99_TOK_EQ(48, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 49 */
#define P99_IS_EQ_49(...) P99_TOK_EQ(49, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 50 */
#define P99_IS_EQ_50(...) P99_TOK_EQ(50, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 51 */
#define P99_IS_EQ_51(...) P99_TOK_EQ(51, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 52 */
#define P99_IS_EQ_52(...) P99_TOK_EQ(52, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 53 */
#define P99_IS_EQ_53(...) P99_TOK_EQ(53, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 54 */
#define P99_IS_EQ_54(...) P99_TOK_EQ(54, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 55 */
#define P99_IS_EQ_55(...) P99_TOK_EQ(55, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 56 */
#define P99_IS_EQ_56(...) P99_TOK_EQ(56, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 57 */
#define P99_IS_EQ_57(...) P99_TOK_EQ(57, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 58 */
#define P99_IS_EQ_58(...) P99_TOK_EQ(58, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 59 */
#define P99_IS_EQ_59(...) P99_TOK_EQ(59, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 60 */
#define P99_IS_EQ_60(...) P99_TOK_EQ(60, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 61 */
#define P99_IS_EQ_61(...) P99_TOK_EQ(61, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 62 */
#define P99_IS_EQ_62(...) P99_TOK_EQ(62, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 63 */
#define P99_IS_EQ_63(...) P99_TOK_EQ(63, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 64 */
#define P99_IS_EQ_64(...) P99_TOK_EQ(64, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 65 */
#define P99_IS_EQ_65(...) P99_TOK_EQ(65, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 66 */
#define P99_IS_EQ_66(...) P99_TOK_EQ(66, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 67 */
#define P99_IS_EQ_67(...) P99_TOK_EQ(67, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 68 */
#define P99_IS_EQ_68(...) P99_TOK_EQ(68, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 69 */
#define P99_IS_EQ_69(...) P99_TOK_EQ(69, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 70 */
#define P99_IS_EQ_70(...) P99_TOK_EQ(70, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 71 */
#define P99_IS_EQ_71(...) P99_TOK_EQ(71, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 72 */
#define P99_IS_EQ_72(...) P99_TOK_EQ(72, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 73 */
#define P99_IS_EQ_73(...) P99_TOK_EQ(73, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 74 */
#define P99_IS_EQ_74(...) P99_TOK_EQ(74, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 75 */
#define P99_IS_EQ_75(...) P99_TOK_EQ(75, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 76 */
#define P99_IS_EQ_76(...) P99_TOK_EQ(76, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 77 */
#define P99_IS_EQ_77(...) P99_TOK_EQ(77, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 78 */
#define P99_IS_EQ_78(...) P99_TOK_EQ(78, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 79 */
#define P99_IS_EQ_79(...) P99_TOK_EQ(79, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 80 */
#define P99_IS_EQ_80(...) P99_TOK_EQ(80, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 81 */
#define P99_IS_EQ_81(...) P99_TOK_EQ(81, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 82 */
#define P99_IS_EQ_82(...) P99_TOK_EQ(82, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 83 */
#define P99_IS_EQ_83(...) P99_TOK_EQ(83, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 84 */
#define P99_IS_EQ_84(...) P99_TOK_EQ(84, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 85 */
#define P99_IS_EQ_85(...) P99_TOK_EQ(85, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 86 */
#define P99_IS_EQ_86(...) P99_TOK_EQ(86, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 87 */
#define P99_IS_EQ_87(...) P99_TOK_EQ(87, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 88 */
#define P99_IS_EQ_88(...) P99_TOK_EQ(88, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 89 */
#define P99_IS_EQ_89(...) P99_TOK_EQ(89, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 90 */
#define P99_IS_EQ_90(...) P99_TOK_EQ(90, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 91 */
#define P99_IS_EQ_91(...) P99_TOK_EQ(91, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 92 */
#define P99_IS_EQ_92(...) P99_TOK_EQ(92, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 93 */
#define P99_IS_EQ_93(...) P99_TOK_EQ(93, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 94 */
#define P99_IS_EQ_94(...) P99_TOK_EQ(94, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 95 */
#define P99_IS_EQ_95(...) P99_TOK_EQ(95, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 96 */
#define P99_IS_EQ_96(...) P99_TOK_EQ(96, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 97 */
#define P99_IS_EQ_97(...) P99_TOK_EQ(97, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 98 */
#define P99_IS_EQ_98(...) P99_TOK_EQ(98, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 99 */
#define P99_IS_EQ_99(...) P99_TOK_EQ(99, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 100 */
#define P99_IS_EQ_100(...) P99_TOK_EQ(100, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 101 */
#define P99_IS_EQ_101(...) P99_TOK_EQ(101, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 102 */
#define P99_IS_EQ_102(...) P99_TOK_EQ(102, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 103 */
#define P99_IS_EQ_103(...) P99_TOK_EQ(103, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 104 */
#define P99_IS_EQ_104(...) P99_TOK_EQ(104, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 105 */
#define P99_IS_EQ_105(...) P99_TOK_EQ(105, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 106 */
#define P99_IS_EQ_106(...) P99_TOK_EQ(106, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 107 */
#define P99_IS_EQ_107(...) P99_TOK_EQ(107, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 108 */
#define P99_IS_EQ_108(...) P99_TOK_EQ(108, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 109 */
#define P99_IS_EQ_109(...) P99_TOK_EQ(109, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 110 */
#define P99_IS_EQ_110(...) P99_TOK_EQ(110, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 111 */
#define P99_IS_EQ_111(...) P99_TOK_EQ(111, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 112 */
#define P99_IS_EQ_112(...) P99_TOK_EQ(112, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 113 */
#define P99_IS_EQ_113(...) P99_TOK_EQ(113, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 114 */
#define P99_IS_EQ_114(...) P99_TOK_EQ(114, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 115 */
#define P99_IS_EQ_115(...) P99_TOK_EQ(115, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 116 */
#define P99_IS_EQ_116(...) P99_TOK_EQ(116, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 117 */
#define P99_IS_EQ_117(...) P99_TOK_EQ(117, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 118 */
#define P99_IS_EQ_118(...) P99_TOK_EQ(118, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 119 */
#define P99_IS_EQ_119(...) P99_TOK_EQ(119, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 120 */
#define P99_IS_EQ_120(...) P99_TOK_EQ(120, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c 121 */
#define P99_IS_EQ_121(...) P99_TOK_EQ(121, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c _Bool */
#define P99_IS_EQ__Bool(...) P99_TOK_EQ(_Bool, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c _Complex */
#define P99_IS_EQ__Complex(...) P99_TOK_EQ(_Complex, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c _Exit */
#define P99_IS_EQ__Exit(...) P99_TOK_EQ(_Exit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c _Imaginary */
#define P99_IS_EQ__Imaginary(...) P99_TOK_EQ(_Imaginary, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c abort */
#define P99_IS_EQ_abort(...) P99_TOK_EQ(abort, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c abs */
#define P99_IS_EQ_abs(...) P99_TOK_EQ(abs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c acos */
#define P99_IS_EQ_acos(...) P99_TOK_EQ(acos, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c asctime */
#define P99_IS_EQ_asctime(...) P99_TOK_EQ(asctime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c asin */
#define P99_IS_EQ_asin(...) P99_TOK_EQ(asin, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c assert */
#define P99_IS_EQ_assert(...) P99_TOK_EQ(assert, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atan */
#define P99_IS_EQ_atan(...) P99_TOK_EQ(atan, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atan2 */
#define P99_IS_EQ_atan2(...) P99_TOK_EQ(atan2, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atexit */
#define P99_IS_EQ_atexit(...) P99_TOK_EQ(atexit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atof */
#define P99_IS_EQ_atof(...) P99_TOK_EQ(atof, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atof */
#define P99_IS_EQ_atof(...) P99_TOK_EQ(atof, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atoi */
#define P99_IS_EQ_atoi(...) P99_TOK_EQ(atoi, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c atol */
#define P99_IS_EQ_atol(...) P99_TOK_EQ(atol, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c auto */
#define P99_IS_EQ_auto(...) P99_TOK_EQ(auto, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c break */
#define P99_IS_EQ_break(...) P99_TOK_EQ(break, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c bsearch */
#define P99_IS_EQ_bsearch(...) P99_TOK_EQ(bsearch, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c btowc */
#define P99_IS_EQ_btowc(...) P99_TOK_EQ(btowc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cabs */
#define P99_IS_EQ_cabs(...) P99_TOK_EQ(cabs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cacos */
#define P99_IS_EQ_cacos(...) P99_TOK_EQ(cacos, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cacosh */
#define P99_IS_EQ_cacosh(...) P99_TOK_EQ(cacosh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c calloc */
#define P99_IS_EQ_calloc(...) P99_TOK_EQ(calloc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c carg */
#define P99_IS_EQ_carg(...) P99_TOK_EQ(carg, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c case */
#define P99_IS_EQ_case(...) P99_TOK_EQ(case, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c casin */
#define P99_IS_EQ_casin(...) P99_TOK_EQ(casin, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c casinh */
#define P99_IS_EQ_casinh(...) P99_TOK_EQ(casinh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c catan */
#define P99_IS_EQ_catan(...) P99_TOK_EQ(catan, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c catanh */
#define P99_IS_EQ_catanh(...) P99_TOK_EQ(catanh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ccos */
#define P99_IS_EQ_ccos(...) P99_TOK_EQ(ccos, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ccosh */
#define P99_IS_EQ_ccosh(...) P99_TOK_EQ(ccosh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ceil */
#define P99_IS_EQ_ceil(...) P99_TOK_EQ(ceil, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cexp */
#define P99_IS_EQ_cexp(...) P99_TOK_EQ(cexp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c char */
#define P99_IS_EQ_char(...) P99_TOK_EQ(char, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cimag */
#define P99_IS_EQ_cimag(...) P99_TOK_EQ(cimag, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c clearerr */
#define P99_IS_EQ_clearerr(...) P99_TOK_EQ(clearerr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c clock */
#define P99_IS_EQ_clock(...) P99_TOK_EQ(clock, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c clock_t */
#define P99_IS_EQ_clock_t(...) P99_TOK_EQ(clock_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c clog */
#define P99_IS_EQ_clog(...) P99_TOK_EQ(clog, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c conj */
#define P99_IS_EQ_conj(...) P99_TOK_EQ(conj, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c const */
#define P99_IS_EQ_const(...) P99_TOK_EQ(const, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c continue */
#define P99_IS_EQ_continue(...) P99_TOK_EQ(continue, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cos */
#define P99_IS_EQ_cos(...) P99_TOK_EQ(cos, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cosh */
#define P99_IS_EQ_cosh(...) P99_TOK_EQ(cosh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cpow */
#define P99_IS_EQ_cpow(...) P99_TOK_EQ(cpow, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c cproj */
#define P99_IS_EQ_cproj(...) P99_TOK_EQ(cproj, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c creal */
#define P99_IS_EQ_creal(...) P99_TOK_EQ(creal, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c csin */
#define P99_IS_EQ_csin(...) P99_TOK_EQ(csin, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c csinh */
#define P99_IS_EQ_csinh(...) P99_TOK_EQ(csinh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c csqrt */
#define P99_IS_EQ_csqrt(...) P99_TOK_EQ(csqrt, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ctan */
#define P99_IS_EQ_ctan(...) P99_TOK_EQ(ctan, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ctanh */
#define P99_IS_EQ_ctanh(...) P99_TOK_EQ(ctanh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ctime */
#define P99_IS_EQ_ctime(...) P99_TOK_EQ(ctime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c default */
#define P99_IS_EQ_default(...) P99_TOK_EQ(default, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c difftime */
#define P99_IS_EQ_difftime(...) P99_TOK_EQ(difftime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c div */
#define P99_IS_EQ_div(...) P99_TOK_EQ(div, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c div_t */
#define P99_IS_EQ_div_t(...) P99_TOK_EQ(div_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c do */
#define P99_IS_EQ_do(...) P99_TOK_EQ(do, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c double */
#define P99_IS_EQ_double(...) P99_TOK_EQ(double, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c double_t */
#define P99_IS_EQ_double_t(...) P99_TOK_EQ(double_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c else */
#define P99_IS_EQ_else(...) P99_TOK_EQ(else, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c enum */
#define P99_IS_EQ_enum(...) P99_TOK_EQ(enum, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c exit */
#define P99_IS_EQ_exit(...) P99_TOK_EQ(exit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c exp */
#define P99_IS_EQ_exp(...) P99_TOK_EQ(exp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c extern */
#define P99_IS_EQ_extern(...) P99_TOK_EQ(extern, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fabs */
#define P99_IS_EQ_fabs(...) P99_TOK_EQ(fabs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fclose */
#define P99_IS_EQ_fclose(...) P99_TOK_EQ(fclose, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fdopen */
#define P99_IS_EQ_fdopen(...) P99_TOK_EQ(fdopen, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fenv_t */
#define P99_IS_EQ_fenv_t(...) P99_TOK_EQ(fenv_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c feof */
#define P99_IS_EQ_feof(...) P99_TOK_EQ(feof, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ferror */
#define P99_IS_EQ_ferror(...) P99_TOK_EQ(ferror, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fexcept_t */
#define P99_IS_EQ_fexcept_t(...) P99_TOK_EQ(fexcept_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fflush */
#define P99_IS_EQ_fflush(...) P99_TOK_EQ(fflush, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fgetc */
#define P99_IS_EQ_fgetc(...) P99_TOK_EQ(fgetc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fgetpos */
#define P99_IS_EQ_fgetpos(...) P99_TOK_EQ(fgetpos, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fgets */
#define P99_IS_EQ_fgets(...) P99_TOK_EQ(fgets, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fgetwc */
#define P99_IS_EQ_fgetwc(...) P99_TOK_EQ(fgetwc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fgetws */
#define P99_IS_EQ_fgetws(...) P99_TOK_EQ(fgetws, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c float */
#define P99_IS_EQ_float(...) P99_TOK_EQ(float, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c float_t */
#define P99_IS_EQ_float_t(...) P99_TOK_EQ(float_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c floor */
#define P99_IS_EQ_floor(...) P99_TOK_EQ(floor, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fopen */
#define P99_IS_EQ_fopen(...) P99_TOK_EQ(fopen, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c for */
#define P99_IS_EQ_for(...) P99_TOK_EQ(for, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fpos_t */
#define P99_IS_EQ_fpos_t(...) P99_TOK_EQ(fpos_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fprintf */
#define P99_IS_EQ_fprintf(...) P99_TOK_EQ(fprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fprintf */
#define P99_IS_EQ_fprintf(...) P99_TOK_EQ(fprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fputc */
#define P99_IS_EQ_fputc(...) P99_TOK_EQ(fputc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fputchar */
#define P99_IS_EQ_fputchar(...) P99_TOK_EQ(fputchar, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fputs */
#define P99_IS_EQ_fputs(...) P99_TOK_EQ(fputs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fputwc */
#define P99_IS_EQ_fputwc(...) P99_TOK_EQ(fputwc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fputws */
#define P99_IS_EQ_fputws(...) P99_TOK_EQ(fputws, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fread */
#define P99_IS_EQ_fread(...) P99_TOK_EQ(fread, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c free */
#define P99_IS_EQ_free(...) P99_TOK_EQ(free, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c freopen */
#define P99_IS_EQ_freopen(...) P99_TOK_EQ(freopen, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c frexp */
#define P99_IS_EQ_frexp(...) P99_TOK_EQ(frexp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fscanf */
#define P99_IS_EQ_fscanf(...) P99_TOK_EQ(fscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fscanf */
#define P99_IS_EQ_fscanf(...) P99_TOK_EQ(fscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fseek */
#define P99_IS_EQ_fseek(...) P99_TOK_EQ(fseek, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fsetpos */
#define P99_IS_EQ_fsetpos(...) P99_TOK_EQ(fsetpos, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ftell */
#define P99_IS_EQ_ftell(...) P99_TOK_EQ(ftell, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fwide */
#define P99_IS_EQ_fwide(...) P99_TOK_EQ(fwide, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fwprintf */
#define P99_IS_EQ_fwprintf(...) P99_TOK_EQ(fwprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fwrite */
#define P99_IS_EQ_fwrite(...) P99_TOK_EQ(fwrite, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c fwscanf */
#define P99_IS_EQ_fwscanf(...) P99_TOK_EQ(fwscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getaddrinfo */
#define P99_IS_EQ_getaddrinfo(...) P99_TOK_EQ(getaddrinfo, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getc */
#define P99_IS_EQ_getc(...) P99_TOK_EQ(getc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getchar */
#define P99_IS_EQ_getchar(...) P99_TOK_EQ(getchar, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getenv */
#define P99_IS_EQ_getenv(...) P99_TOK_EQ(getenv, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getnameinfo */
#define P99_IS_EQ_getnameinfo(...) P99_TOK_EQ(getnameinfo, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c gets */
#define P99_IS_EQ_gets(...) P99_TOK_EQ(gets, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getwc */
#define P99_IS_EQ_getwc(...) P99_TOK_EQ(getwc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c getwchar */
#define P99_IS_EQ_getwchar(...) P99_TOK_EQ(getwchar, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c gmtime */
#define P99_IS_EQ_gmtime(...) P99_TOK_EQ(gmtime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c goto */
#define P99_IS_EQ_goto(...) P99_TOK_EQ(goto, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c if */
#define P99_IS_EQ_if(...) P99_TOK_EQ(if, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c imaxabs */
#define P99_IS_EQ_imaxabs(...) P99_TOK_EQ(imaxabs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c imaxdiv */
#define P99_IS_EQ_imaxdiv(...) P99_TOK_EQ(imaxdiv, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c imaxdiv_t */
#define P99_IS_EQ_imaxdiv_t(...) P99_TOK_EQ(imaxdiv_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c inline */
#define P99_IS_EQ_inline(...) P99_TOK_EQ(inline, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c int */
#define P99_IS_EQ_int(...) P99_TOK_EQ(int, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c int16_t */
#define P99_IS_EQ_int16_t(...) P99_TOK_EQ(int16_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c int32_t */
#define P99_IS_EQ_int32_t(...) P99_TOK_EQ(int32_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c int64_t */
#define P99_IS_EQ_int64_t(...) P99_TOK_EQ(int64_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c int8_t */
#define P99_IS_EQ_int8_t(...) P99_TOK_EQ(int8_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intfast16_t */
#define P99_IS_EQ_intfast16_t(...) P99_TOK_EQ(intfast16_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intfast32_t */
#define P99_IS_EQ_intfast32_t(...) P99_TOK_EQ(intfast32_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intfast64_t */
#define P99_IS_EQ_intfast64_t(...) P99_TOK_EQ(intfast64_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intfast8_t */
#define P99_IS_EQ_intfast8_t(...) P99_TOK_EQ(intfast8_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intleast16_t */
#define P99_IS_EQ_intleast16_t(...) P99_TOK_EQ(intleast16_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intleast32_t */
#define P99_IS_EQ_intleast32_t(...) P99_TOK_EQ(intleast32_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intleast64_t */
#define P99_IS_EQ_intleast64_t(...) P99_TOK_EQ(intleast64_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intleast8_t */
#define P99_IS_EQ_intleast8_t(...) P99_TOK_EQ(intleast8_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intmax_t */
#define P99_IS_EQ_intmax_t(...) P99_TOK_EQ(intmax_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c intptr_t */
#define P99_IS_EQ_intptr_t(...) P99_TOK_EQ(intptr_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isalnum */
#define P99_IS_EQ_isalnum(...) P99_TOK_EQ(isalnum, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isalpha */
#define P99_IS_EQ_isalpha(...) P99_TOK_EQ(isalpha, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isblank */
#define P99_IS_EQ_isblank(...) P99_TOK_EQ(isblank, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iscntrl */
#define P99_IS_EQ_iscntrl(...) P99_TOK_EQ(iscntrl, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isdigit */
#define P99_IS_EQ_isdigit(...) P99_TOK_EQ(isdigit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isgraph */
#define P99_IS_EQ_isgraph(...) P99_TOK_EQ(isgraph, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c islower */
#define P99_IS_EQ_islower(...) P99_TOK_EQ(islower, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isprint */
#define P99_IS_EQ_isprint(...) P99_TOK_EQ(isprint, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ispunct */
#define P99_IS_EQ_ispunct(...) P99_TOK_EQ(ispunct, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isspace */
#define P99_IS_EQ_isspace(...) P99_TOK_EQ(isspace, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isupper */
#define P99_IS_EQ_isupper(...) P99_TOK_EQ(isupper, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswalnum */
#define P99_IS_EQ_iswalnum(...) P99_TOK_EQ(iswalnum, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswalpha */
#define P99_IS_EQ_iswalpha(...) P99_TOK_EQ(iswalpha, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswcntrl */
#define P99_IS_EQ_iswcntrl(...) P99_TOK_EQ(iswcntrl, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswctype */
#define P99_IS_EQ_iswctype(...) P99_TOK_EQ(iswctype, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswdigit */
#define P99_IS_EQ_iswdigit(...) P99_TOK_EQ(iswdigit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswgraph */
#define P99_IS_EQ_iswgraph(...) P99_TOK_EQ(iswgraph, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswlower */
#define P99_IS_EQ_iswlower(...) P99_TOK_EQ(iswlower, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswprint */
#define P99_IS_EQ_iswprint(...) P99_TOK_EQ(iswprint, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswpunct */
#define P99_IS_EQ_iswpunct(...) P99_TOK_EQ(iswpunct, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswspace */
#define P99_IS_EQ_iswspace(...) P99_TOK_EQ(iswspace, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswupper */
#define P99_IS_EQ_iswupper(...) P99_TOK_EQ(iswupper, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c iswxdigit */
#define P99_IS_EQ_iswxdigit(...) P99_TOK_EQ(iswxdigit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c isxdigit */
#define P99_IS_EQ_isxdigit(...) P99_TOK_EQ(isxdigit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c labs */
#define P99_IS_EQ_labs(...) P99_TOK_EQ(labs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ldexp */
#define P99_IS_EQ_ldexp(...) P99_TOK_EQ(ldexp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ldiv */
#define P99_IS_EQ_ldiv(...) P99_TOK_EQ(ldiv, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ldiv */
#define P99_IS_EQ_ldiv(...) P99_TOK_EQ(ldiv, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ldiv_t */
#define P99_IS_EQ_ldiv_t(...) P99_TOK_EQ(ldiv_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c lldiv_t */
#define P99_IS_EQ_lldiv_t(...) P99_TOK_EQ(lldiv_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c localeconv */
#define P99_IS_EQ_localeconv(...) P99_TOK_EQ(localeconv, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c localtime */
#define P99_IS_EQ_localtime(...) P99_TOK_EQ(localtime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c log */
#define P99_IS_EQ_log(...) P99_TOK_EQ(log, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c log10 */
#define P99_IS_EQ_log10(...) P99_TOK_EQ(log10, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c long */
#define P99_IS_EQ_long(...) P99_TOK_EQ(long, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c longjmp */
#define P99_IS_EQ_longjmp(...) P99_TOK_EQ(longjmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c malloc */
#define P99_IS_EQ_malloc(...) P99_TOK_EQ(malloc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c mbrlen */
#define P99_IS_EQ_mbrlen(...) P99_TOK_EQ(mbrlen, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c mbrtowc */
#define P99_IS_EQ_mbrtowc(...) P99_TOK_EQ(mbrtowc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c mbsinit */
#define P99_IS_EQ_mbsinit(...) P99_TOK_EQ(mbsinit, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c mbsrtowcs */
#define P99_IS_EQ_mbsrtowcs(...) P99_TOK_EQ(mbsrtowcs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c mbstate_t */
#define P99_IS_EQ_mbstate_t(...) P99_TOK_EQ(mbstate_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c memchr */
#define P99_IS_EQ_memchr(...) P99_TOK_EQ(memchr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c memcmp */
#define P99_IS_EQ_memcmp(...) P99_TOK_EQ(memcmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c memcpy */
#define P99_IS_EQ_memcpy(...) P99_TOK_EQ(memcpy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c memmove */
#define P99_IS_EQ_memmove(...) P99_TOK_EQ(memmove, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c memset */
#define P99_IS_EQ_memset(...) P99_TOK_EQ(memset, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c mktime */
#define P99_IS_EQ_mktime(...) P99_TOK_EQ(mktime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c modf */
#define P99_IS_EQ_modf(...) P99_TOK_EQ(modf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c off_t */
#define P99_IS_EQ_off_t(...) P99_TOK_EQ(off_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c offsetof */
#define P99_IS_EQ_offsetof(...) P99_TOK_EQ(offsetof, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c perror */
#define P99_IS_EQ_perror(...) P99_TOK_EQ(perror, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c pow */
#define P99_IS_EQ_pow(...) P99_TOK_EQ(pow, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c printf */
#define P99_IS_EQ_printf(...) P99_TOK_EQ(printf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ptrdiff_t */
#define P99_IS_EQ_ptrdiff_t(...) P99_TOK_EQ(ptrdiff_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c putc */
#define P99_IS_EQ_putc(...) P99_TOK_EQ(putc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c putchar */
#define P99_IS_EQ_putchar(...) P99_TOK_EQ(putchar, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c puts */
#define P99_IS_EQ_puts(...) P99_TOK_EQ(puts, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c putwc */
#define P99_IS_EQ_putwc(...) P99_TOK_EQ(putwc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c putwchar */
#define P99_IS_EQ_putwchar(...) P99_TOK_EQ(putwchar, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c qsort */
#define P99_IS_EQ_qsort(...) P99_TOK_EQ(qsort, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c raise */
#define P99_IS_EQ_raise(...) P99_TOK_EQ(raise, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c rand */
#define P99_IS_EQ_rand(...) P99_TOK_EQ(rand, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c realloc */
#define P99_IS_EQ_realloc(...) P99_TOK_EQ(realloc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c register */
#define P99_IS_EQ_register(...) P99_TOK_EQ(register, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c remove */
#define P99_IS_EQ_remove(...) P99_TOK_EQ(remove, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c rename */
#define P99_IS_EQ_rename(...) P99_TOK_EQ(rename, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c restrict */
#define P99_IS_EQ_restrict(...) P99_TOK_EQ(restrict, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c return */
#define P99_IS_EQ_return(...) P99_TOK_EQ(return, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c rewind */
#define P99_IS_EQ_rewind(...) P99_TOK_EQ(rewind, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c scanf */
#define P99_IS_EQ_scanf(...) P99_TOK_EQ(scanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c setbuf */
#define P99_IS_EQ_setbuf(...) P99_TOK_EQ(setbuf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c setjmp */
#define P99_IS_EQ_setjmp(...) P99_TOK_EQ(setjmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c setlocale */
#define P99_IS_EQ_setlocale(...) P99_TOK_EQ(setlocale, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c setvbuf */
#define P99_IS_EQ_setvbuf(...) P99_TOK_EQ(setvbuf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c short */
#define P99_IS_EQ_short(...) P99_TOK_EQ(short, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sig_atomic_t */
#define P99_IS_EQ_sig_atomic_t(...) P99_TOK_EQ(sig_atomic_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c signed */
#define P99_IS_EQ_signed(...) P99_TOK_EQ(signed, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sin */
#define P99_IS_EQ_sin(...) P99_TOK_EQ(sin, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sinh */
#define P99_IS_EQ_sinh(...) P99_TOK_EQ(sinh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c size_t */
#define P99_IS_EQ_size_t(...) P99_TOK_EQ(size_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sizeof */
#define P99_IS_EQ_sizeof(...) P99_TOK_EQ(sizeof, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c snprintf */
#define P99_IS_EQ_snprintf(...) P99_TOK_EQ(snprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sprintf */
#define P99_IS_EQ_sprintf(...) P99_TOK_EQ(sprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sqrt */
#define P99_IS_EQ_sqrt(...) P99_TOK_EQ(sqrt, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c srand */
#define P99_IS_EQ_srand(...) P99_TOK_EQ(srand, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c sscanf */
#define P99_IS_EQ_sscanf(...) P99_TOK_EQ(sscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ssize_t */
#define P99_IS_EQ_ssize_t(...) P99_TOK_EQ(ssize_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c static */
#define P99_IS_EQ_static(...) P99_TOK_EQ(static, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strcat */
#define P99_IS_EQ_strcat(...) P99_TOK_EQ(strcat, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strchr */
#define P99_IS_EQ_strchr(...) P99_TOK_EQ(strchr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strcmp */
#define P99_IS_EQ_strcmp(...) P99_TOK_EQ(strcmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strcoll */
#define P99_IS_EQ_strcoll(...) P99_TOK_EQ(strcoll, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strcpy */
#define P99_IS_EQ_strcpy(...) P99_TOK_EQ(strcpy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strcspn */
#define P99_IS_EQ_strcspn(...) P99_TOK_EQ(strcspn, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strerror */
#define P99_IS_EQ_strerror(...) P99_TOK_EQ(strerror, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strftime */
#define P99_IS_EQ_strftime(...) P99_TOK_EQ(strftime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strlen */
#define P99_IS_EQ_strlen(...) P99_TOK_EQ(strlen, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strncat */
#define P99_IS_EQ_strncat(...) P99_TOK_EQ(strncat, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strncmp */
#define P99_IS_EQ_strncmp(...) P99_TOK_EQ(strncmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strncpy */
#define P99_IS_EQ_strncpy(...) P99_TOK_EQ(strncpy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strpbrk */
#define P99_IS_EQ_strpbrk(...) P99_TOK_EQ(strpbrk, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strrchr */
#define P99_IS_EQ_strrchr(...) P99_TOK_EQ(strrchr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strspn */
#define P99_IS_EQ_strspn(...) P99_TOK_EQ(strspn, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strstr */
#define P99_IS_EQ_strstr(...) P99_TOK_EQ(strstr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strtod */
#define P99_IS_EQ_strtod(...) P99_TOK_EQ(strtod, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strtoimax */
#define P99_IS_EQ_strtoimax(...) P99_TOK_EQ(strtoimax, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strtok */
#define P99_IS_EQ_strtok(...) P99_TOK_EQ(strtok, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strtol */
#define P99_IS_EQ_strtol(...) P99_TOK_EQ(strtol, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strtoul */
#define P99_IS_EQ_strtoul(...) P99_TOK_EQ(strtoul, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strtoumax */
#define P99_IS_EQ_strtoumax(...) P99_TOK_EQ(strtoumax, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c struct */
#define P99_IS_EQ_struct(...) P99_TOK_EQ(struct, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c strxfrm */
#define P99_IS_EQ_strxfrm(...) P99_TOK_EQ(strxfrm, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c switch */
#define P99_IS_EQ_switch(...) P99_TOK_EQ(switch, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c swprintf */
#define P99_IS_EQ_swprintf(...) P99_TOK_EQ(swprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c swscanf */
#define P99_IS_EQ_swscanf(...) P99_TOK_EQ(swscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c system */
#define P99_IS_EQ_system(...) P99_TOK_EQ(system, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c tan */
#define P99_IS_EQ_tan(...) P99_TOK_EQ(tan, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c tanh */
#define P99_IS_EQ_tanh(...) P99_TOK_EQ(tanh, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c time */
#define P99_IS_EQ_time(...) P99_TOK_EQ(time, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c time_t */
#define P99_IS_EQ_time_t(...) P99_TOK_EQ(time_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c tmpfile */
#define P99_IS_EQ_tmpfile(...) P99_TOK_EQ(tmpfile, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c tmpnam */
#define P99_IS_EQ_tmpnam(...) P99_TOK_EQ(tmpnam, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c tolower */
#define P99_IS_EQ_tolower(...) P99_TOK_EQ(tolower, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c toupper */
#define P99_IS_EQ_toupper(...) P99_TOK_EQ(toupper, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c towctrans */
#define P99_IS_EQ_towctrans(...) P99_TOK_EQ(towctrans, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c towlower */
#define P99_IS_EQ_towlower(...) P99_TOK_EQ(towlower, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c towupper */
#define P99_IS_EQ_towupper(...) P99_TOK_EQ(towupper, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c typedef */
#define P99_IS_EQ_typedef(...) P99_TOK_EQ(typedef, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uint16_t */
#define P99_IS_EQ_uint16_t(...) P99_TOK_EQ(uint16_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uint32_t */
#define P99_IS_EQ_uint32_t(...) P99_TOK_EQ(uint32_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uint64_t */
#define P99_IS_EQ_uint64_t(...) P99_TOK_EQ(uint64_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uint8_t */
#define P99_IS_EQ_uint8_t(...) P99_TOK_EQ(uint8_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintfast16_t */
#define P99_IS_EQ_uintfast16_t(...) P99_TOK_EQ(uintfast16_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintfast32_t */
#define P99_IS_EQ_uintfast32_t(...) P99_TOK_EQ(uintfast32_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintfast64_t */
#define P99_IS_EQ_uintfast64_t(...) P99_TOK_EQ(uintfast64_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintfast8_t */
#define P99_IS_EQ_uintfast8_t(...) P99_TOK_EQ(uintfast8_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintleast16_t */
#define P99_IS_EQ_uintleast16_t(...) P99_TOK_EQ(uintleast16_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintleast32_t */
#define P99_IS_EQ_uintleast32_t(...) P99_TOK_EQ(uintleast32_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintleast64_t */
#define P99_IS_EQ_uintleast64_t(...) P99_TOK_EQ(uintleast64_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintleast8_t */
#define P99_IS_EQ_uintleast8_t(...) P99_TOK_EQ(uintleast8_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintmax_t */
#define P99_IS_EQ_uintmax_t(...) P99_TOK_EQ(uintmax_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c uintptr_t */
#define P99_IS_EQ_uintptr_t(...) P99_TOK_EQ(uintptr_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ungetc */
#define P99_IS_EQ_ungetc(...) P99_TOK_EQ(ungetc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c ungetwc */
#define P99_IS_EQ_ungetwc(...) P99_TOK_EQ(ungetwc, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c union */
#define P99_IS_EQ_union(...) P99_TOK_EQ(union, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c unsigned */
#define P99_IS_EQ_unsigned(...) P99_TOK_EQ(unsigned, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c va_arg */
#define P99_IS_EQ_va_arg(...) P99_TOK_EQ(va_arg, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c va_copy */
#define P99_IS_EQ_va_copy(...) P99_TOK_EQ(va_copy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c va_end */
#define P99_IS_EQ_va_end(...) P99_TOK_EQ(va_end, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c va_start */
#define P99_IS_EQ_va_start(...) P99_TOK_EQ(va_start, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vfprintf */
#define P99_IS_EQ_vfprintf(...) P99_TOK_EQ(vfprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vfscanf */
#define P99_IS_EQ_vfscanf(...) P99_TOK_EQ(vfscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vfwprintf */
#define P99_IS_EQ_vfwprintf(...) P99_TOK_EQ(vfwprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c void */
#define P99_IS_EQ_void(...) P99_TOK_EQ(void, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c volatile */
#define P99_IS_EQ_volatile(...) P99_TOK_EQ(volatile, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vprintf */
#define P99_IS_EQ_vprintf(...) P99_TOK_EQ(vprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vscanf */
#define P99_IS_EQ_vscanf(...) P99_TOK_EQ(vscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vsprintf */
#define P99_IS_EQ_vsprintf(...) P99_TOK_EQ(vsprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vsscanf */
#define P99_IS_EQ_vsscanf(...) P99_TOK_EQ(vsscanf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vswprintf */
#define P99_IS_EQ_vswprintf(...) P99_TOK_EQ(vswprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c vwprintf */
#define P99_IS_EQ_vwprintf(...) P99_TOK_EQ(vwprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wchar_t */
#define P99_IS_EQ_wchar_t(...) P99_TOK_EQ(wchar_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcrtomb */
#define P99_IS_EQ_wcrtomb(...) P99_TOK_EQ(wcrtomb, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcscat */
#define P99_IS_EQ_wcscat(...) P99_TOK_EQ(wcscat, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcschr */
#define P99_IS_EQ_wcschr(...) P99_TOK_EQ(wcschr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcscmp */
#define P99_IS_EQ_wcscmp(...) P99_TOK_EQ(wcscmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcscoll */
#define P99_IS_EQ_wcscoll(...) P99_TOK_EQ(wcscoll, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcscpy */
#define P99_IS_EQ_wcscpy(...) P99_TOK_EQ(wcscpy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcscspn */
#define P99_IS_EQ_wcscspn(...) P99_TOK_EQ(wcscspn, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsftime */
#define P99_IS_EQ_wcsftime(...) P99_TOK_EQ(wcsftime, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcslen */
#define P99_IS_EQ_wcslen(...) P99_TOK_EQ(wcslen, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsncat */
#define P99_IS_EQ_wcsncat(...) P99_TOK_EQ(wcsncat, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsncmp */
#define P99_IS_EQ_wcsncmp(...) P99_TOK_EQ(wcsncmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsncpy */
#define P99_IS_EQ_wcsncpy(...) P99_TOK_EQ(wcsncpy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcspbrk */
#define P99_IS_EQ_wcspbrk(...) P99_TOK_EQ(wcspbrk, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsrchr */
#define P99_IS_EQ_wcsrchr(...) P99_TOK_EQ(wcsrchr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsrtombs */
#define P99_IS_EQ_wcsrtombs(...) P99_TOK_EQ(wcsrtombs, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsspn */
#define P99_IS_EQ_wcsspn(...) P99_TOK_EQ(wcsspn, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsstr */
#define P99_IS_EQ_wcsstr(...) P99_TOK_EQ(wcsstr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcstod */
#define P99_IS_EQ_wcstod(...) P99_TOK_EQ(wcstod, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcstoimax */
#define P99_IS_EQ_wcstoimax(...) P99_TOK_EQ(wcstoimax, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcstok */
#define P99_IS_EQ_wcstok(...) P99_TOK_EQ(wcstok, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcstol */
#define P99_IS_EQ_wcstol(...) P99_TOK_EQ(wcstol, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcstoul */
#define P99_IS_EQ_wcstoul(...) P99_TOK_EQ(wcstoul, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcstoumax */
#define P99_IS_EQ_wcstoumax(...) P99_TOK_EQ(wcstoumax, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wcsxfrm */
#define P99_IS_EQ_wcsxfrm(...) P99_TOK_EQ(wcsxfrm, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wctob */
#define P99_IS_EQ_wctob(...) P99_TOK_EQ(wctob, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wctrans */
#define P99_IS_EQ_wctrans(...) P99_TOK_EQ(wctrans, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wctrans_t */
#define P99_IS_EQ_wctrans_t(...) P99_TOK_EQ(wctrans_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wctype */
#define P99_IS_EQ_wctype(...) P99_TOK_EQ(wctype, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wctype_t */
#define P99_IS_EQ_wctype_t(...) P99_TOK_EQ(wctype_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c while */
#define P99_IS_EQ_while(...) P99_TOK_EQ(while, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wint_t */
#define P99_IS_EQ_wint_t(...) P99_TOK_EQ(wint_t, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wmemchr */
#define P99_IS_EQ_wmemchr(...) P99_TOK_EQ(wmemchr, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wmemcmp */
#define P99_IS_EQ_wmemcmp(...) P99_TOK_EQ(wmemcmp, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wmemcpy */
#define P99_IS_EQ_wmemcpy(...) P99_TOK_EQ(wmemcpy, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wmemmove */
#define P99_IS_EQ_wmemmove(...) P99_TOK_EQ(wmemmove, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wmemset */
#define P99_IS_EQ_wmemset(...) P99_TOK_EQ(wmemset, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wprintf */
#define P99_IS_EQ_wprintf(...) P99_TOK_EQ(wprintf, __VA_ARGS__)
/*! @brief Test if the argument consists of exactly the token @c wscanf */
#define P99_IS_EQ_wscanf(...) P99_TOK_EQ(wscanf, __VA_ARGS__)
#define P99__EAT_0 
#define P99__EAT_1 
#define P99__EAT_2 
#define P99__EAT_3 
#define P99__EAT_4 
#define P99__EAT_5 
#define P99__EAT_6 
#define P99__EAT_7 
#define P99__EAT_8 
#define P99__EAT_9 
#define P99__EAT_10 
#define P99__EAT_11 
#define P99__EAT_12 
#define P99__EAT_13 
#define P99__EAT_14 
#define P99__EAT_15 
#define P99__EAT_16 
#define P99__EAT_17 
#define P99__EAT_18 
#define P99__EAT_19 
#define P99__EAT_20 
#define P99__EAT_21 
#define P99__EAT_22 
#define P99__EAT_23 
#define P99__EAT_24 
#define P99__EAT_25 
#define P99__EAT_26 
#define P99__EAT_27 
#define P99__EAT_28 
#define P99__EAT_29 
#define P99__EAT_30 
#define P99__EAT_31 
#define P99__EAT_32 
#define P99__EAT_33 
#define P99__EAT_34 
#define P99__EAT_35 
#define P99__EAT_36 
#define P99__EAT_37 
#define P99__EAT_38 
#define P99__EAT_39 
#define P99__EAT_40 
#define P99__EAT_41 
#define P99__EAT_42 
#define P99__EAT_43 
#define P99__EAT_44 
#define P99__EAT_45 
#define P99__EAT_46 
#define P99__EAT_47 
#define P99__EAT_48 
#define P99__EAT_49 
#define P99__EAT_50 
#define P99__EAT_51 
#define P99__EAT_52 
#define P99__EAT_53 
#define P99__EAT_54 
#define P99__EAT_55 
#define P99__EAT_56 
#define P99__EAT_57 
#define P99__EAT_58 
#define P99__EAT_59 
#define P99__EAT_60 
#define P99__EAT_61 
#define P99__EAT_62 
#define P99__EAT_63 
#define P99__EAT_64 
#define P99__EAT_65 
#define P99__EAT_66 
#define P99__EAT_67 
#define P99__EAT_68 
#define P99__EAT_69 
#define P99__EAT_70 
#define P99__EAT_71 
#define P99__EAT_72 
#define P99__EAT_73 
#define P99__EAT_74 
#define P99__EAT_75 
#define P99__EAT_76 
#define P99__EAT_77 
#define P99__EAT_78 
#define P99__EAT_79 
#define P99__EAT_80 
#define P99__EAT_81 
#define P99__EAT_82 
#define P99__EAT_83 
#define P99__EAT_84 
#define P99__EAT_85 
#define P99__EAT_86 
#define P99__EAT_87 
#define P99__EAT_88 
#define P99__EAT_89 
#define P99__EAT_90 
#define P99__EAT_91 
#define P99__EAT_92 
#define P99__EAT_93 
#define P99__EAT_94 
#define P99__EAT_95 
#define P99__EAT_96 
#define P99__EAT_97 
#define P99__EAT_98 
#define P99__EAT_99 
#define P99__EAT_100 
#define P99__EAT_101 
#define P99__EAT_102 
#define P99__EAT_103 
#define P99__EAT_104 
#define P99__EAT_105 
#define P99__EAT_106 
#define P99__EAT_107 
#define P99__EAT_108 
#define P99__EAT_109 
#define P99__EAT_110 
#define P99__EAT_111 
#define P99__EAT_112 
#define P99__EAT_113 
#define P99__EAT_114 
#define P99__EAT_115 
#define P99__EAT_116 
#define P99__EAT_117 
#define P99__EAT_118 
#define P99__EAT_119 
#define P99__EAT_120 
#define P99__EAT_121 
#define P99__EAT__Bool 
#define P99__EAT__Complex 
#define P99__EAT__Exit 
#define P99__EAT__Imaginary 
#define P99__EAT_abort 
#define P99__EAT_abs 
#define P99__EAT_acos 
#define P99__EAT_asctime 
#define P99__EAT_asin 
#define P99__EAT_assert 
#define P99__EAT_atan 
#define P99__EAT_atan2 
#define P99__EAT_atexit 
#define P99__EAT_atof 
#define P99__EAT_atof 
#define P99__EAT_atoi 
#define P99__EAT_atol 
#define P99__EAT_auto 
#define P99__EAT_break 
#define P99__EAT_bsearch 
#define P99__EAT_btowc 
#define P99__EAT_cabs 
#define P99__EAT_cacos 
#define P99__EAT_cacosh 
#define P99__EAT_calloc 
#define P99__EAT_carg 
#define P99__EAT_case 
#define P99__EAT_casin 
#define P99__EAT_casinh 
#define P99__EAT_catan 
#define P99__EAT_catanh 
#define P99__EAT_ccos 
#define P99__EAT_ccosh 
#define P99__EAT_ceil 
#define P99__EAT_cexp 
#define P99__EAT_char 
#define P99__EAT_cimag 
#define P99__EAT_clearerr 
#define P99__EAT_clock 
#define P99__EAT_clock_t 
#define P99__EAT_clog 
#define P99__EAT_conj 
#define P99__EAT_const 
#define P99__EAT_continue 
#define P99__EAT_cos 
#define P99__EAT_cosh 
#define P99__EAT_cpow 
#define P99__EAT_cproj 
#define P99__EAT_creal 
#define P99__EAT_csin 
#define P99__EAT_csinh 
#define P99__EAT_csqrt 
#define P99__EAT_ctan 
#define P99__EAT_ctanh 
#define P99__EAT_ctime 
#define P99__EAT_default 
#define P99__EAT_difftime 
#define P99__EAT_div 
#define P99__EAT_div_t 
#define P99__EAT_do 
#define P99__EAT_double 
#define P99__EAT_double_t 
#define P99__EAT_else 
#define P99__EAT_enum 
#define P99__EAT_exit 
#define P99__EAT_exp 
#define P99__EAT_extern 
#define P99__EAT_fabs 
#define P99__EAT_fclose 
#define P99__EAT_fdopen 
#define P99__EAT_fenv_t 
#define P99__EAT_feof 
#define P99__EAT_ferror 
#define P99__EAT_fexcept_t 
#define P99__EAT_fflush 
#define P99__EAT_fgetc 
#define P99__EAT_fgetpos 
#define P99__EAT_fgets 
#define P99__EAT_fgetwc 
#define P99__EAT_fgetws 
#define P99__EAT_float 
#define P99__EAT_float_t 
#define P99__EAT_floor 
#define P99__EAT_fopen 
#define P99__EAT_for 
#define P99__EAT_fpos_t 
#define P99__EAT_fprintf 
#define P99__EAT_fprintf 
#define P99__EAT_fputc 
#define P99__EAT_fputchar 
#define P99__EAT_fputs 
#define P99__EAT_fputwc 
#define P99__EAT_fputws 
#define P99__EAT_fread 
#define P99__EAT_free 
#define P99__EAT_freopen 
#define P99__EAT_frexp 
#define P99__EAT_fscanf 
#define P99__EAT_fscanf 
#define P99__EAT_fseek 
#define P99__EAT_fsetpos 
#define P99__EAT_ftell 
#define P99__EAT_fwide 
#define P99__EAT_fwprintf 
#define P99__EAT_fwrite 
#define P99__EAT_fwscanf 
#define P99__EAT_getaddrinfo 
#define P99__EAT_getc 
#define P99__EAT_getchar 
#define P99__EAT_getenv 
#define P99__EAT_getnameinfo 
#define P99__EAT_gets 
#define P99__EAT_getwc 
#define P99__EAT_getwchar 
#define P99__EAT_gmtime 
#define P99__EAT_goto 
#define P99__EAT_if 
#define P99__EAT_imaxabs 
#define P99__EAT_imaxdiv 
#define P99__EAT_imaxdiv_t 
#define P99__EAT_inline 
#define P99__EAT_int 
#define P99__EAT_int16_t 
#define P99__EAT_int32_t 
#define P99__EAT_int64_t 
#define P99__EAT_int8_t 
#define P99__EAT_intfast16_t 
#define P99__EAT_intfast32_t 
#define P99__EAT_intfast64_t 
#define P99__EAT_intfast8_t 
#define P99__EAT_intleast16_t 
#define P99__EAT_intleast32_t 
#define P99__EAT_intleast64_t 
#define P99__EAT_intleast8_t 
#define P99__EAT_intmax_t 
#define P99__EAT_intptr_t 
#define P99__EAT_isalnum 
#define P99__EAT_isalpha 
#define P99__EAT_isblank 
#define P99__EAT_iscntrl 
#define P99__EAT_isdigit 
#define P99__EAT_isgraph 
#define P99__EAT_islower 
#define P99__EAT_isprint 
#define P99__EAT_ispunct 
#define P99__EAT_isspace 
#define P99__EAT_isupper 
#define P99__EAT_iswalnum 
#define P99__EAT_iswalpha 
#define P99__EAT_iswcntrl 
#define P99__EAT_iswctype 
#define P99__EAT_iswdigit 
#define P99__EAT_iswgraph 
#define P99__EAT_iswlower 
#define P99__EAT_iswprint 
#define P99__EAT_iswpunct 
#define P99__EAT_iswspace 
#define P99__EAT_iswupper 
#define P99__EAT_iswxdigit 
#define P99__EAT_isxdigit 
#define P99__EAT_labs 
#define P99__EAT_ldexp 
#define P99__EAT_ldiv 
#define P99__EAT_ldiv 
#define P99__EAT_ldiv_t 
#define P99__EAT_lldiv_t 
#define P99__EAT_localeconv 
#define P99__EAT_localtime 
#define P99__EAT_log 
#define P99__EAT_log10 
#define P99__EAT_long 
#define P99__EAT_longjmp 
#define P99__EAT_malloc 
#define P99__EAT_mbrlen 
#define P99__EAT_mbrtowc 
#define P99__EAT_mbsinit 
#define P99__EAT_mbsrtowcs 
#define P99__EAT_mbstate_t 
#define P99__EAT_memchr 
#define P99__EAT_memcmp 
#define P99__EAT_memcpy 
#define P99__EAT_memmove 
#define P99__EAT_memset 
#define P99__EAT_mktime 
#define P99__EAT_modf 
#define P99__EAT_off_t 
#define P99__EAT_offsetof 
#define P99__EAT_perror 
#define P99__EAT_pow 
#define P99__EAT_printf 
#define P99__EAT_ptrdiff_t 
#define P99__EAT_putc 
#define P99__EAT_putchar 
#define P99__EAT_puts 
#define P99__EAT_putwc 
#define P99__EAT_putwchar 
#define P99__EAT_qsort 
#define P99__EAT_raise 
#define P99__EAT_rand 
#define P99__EAT_realloc 
#define P99__EAT_register 
#define P99__EAT_remove 
#define P99__EAT_rename 
#define P99__EAT_restrict 
#define P99__EAT_return 
#define P99__EAT_rewind 
#define P99__EAT_scanf 
#define P99__EAT_setbuf 
#define P99__EAT_setjmp 
#define P99__EAT_setlocale 
#define P99__EAT_setvbuf 
#define P99__EAT_short 
#define P99__EAT_sig_atomic_t 
#define P99__EAT_signed 
#define P99__EAT_sin 
#define P99__EAT_sinh 
#define P99__EAT_size_t 
#define P99__EAT_sizeof 
#define P99__EAT_snprintf 
#define P99__EAT_sprintf 
#define P99__EAT_sqrt 
#define P99__EAT_srand 
#define P99__EAT_sscanf 
#define P99__EAT_ssize_t 
#define P99__EAT_static 
#define P99__EAT_strcat 
#define P99__EAT_strchr 
#define P99__EAT_strcmp 
#define P99__EAT_strcoll 
#define P99__EAT_strcpy 
#define P99__EAT_strcspn 
#define P99__EAT_strerror 
#define P99__EAT_strftime 
#define P99__EAT_strlen 
#define P99__EAT_strncat 
#define P99__EAT_strncmp 
#define P99__EAT_strncpy 
#define P99__EAT_strpbrk 
#define P99__EAT_strrchr 
#define P99__EAT_strspn 
#define P99__EAT_strstr 
#define P99__EAT_strtod 
#define P99__EAT_strtoimax 
#define P99__EAT_strtok 
#define P99__EAT_strtol 
#define P99__EAT_strtoul 
#define P99__EAT_strtoumax 
#define P99__EAT_struct 
#define P99__EAT_strxfrm 
#define P99__EAT_switch 
#define P99__EAT_swprintf 
#define P99__EAT_swscanf 
#define P99__EAT_system 
#define P99__EAT_tan 
#define P99__EAT_tanh 
#define P99__EAT_time 
#define P99__EAT_time_t 
#define P99__EAT_tmpfile 
#define P99__EAT_tmpnam 
#define P99__EAT_tolower 
#define P99__EAT_toupper 
#define P99__EAT_towctrans 
#define P99__EAT_towlower 
#define P99__EAT_towupper 
#define P99__EAT_typedef 
#define P99__EAT_uint16_t 
#define P99__EAT_uint32_t 
#define P99__EAT_uint64_t 
#define P99__EAT_uint8_t 
#define P99__EAT_uintfast16_t 
#define P99__EAT_uintfast32_t 
#define P99__EAT_uintfast64_t 
#define P99__EAT_uintfast8_t 
#define P99__EAT_uintleast16_t 
#define P99__EAT_uintleast32_t 
#define P99__EAT_uintleast64_t 
#define P99__EAT_uintleast8_t 
#define P99__EAT_uintmax_t 
#define P99__EAT_uintptr_t 
#define P99__EAT_ungetc 
#define P99__EAT_ungetwc 
#define P99__EAT_union 
#define P99__EAT_unsigned 
#define P99__EAT_va_arg 
#define P99__EAT_va_copy 
#define P99__EAT_va_end 
#define P99__EAT_va_start 
#define P99__EAT_vfprintf 
#define P99__EAT_vfscanf 
#define P99__EAT_vfwprintf 
#define P99__EAT_void 
#define P99__EAT_volatile 
#define P99__EAT_vprintf 
#define P99__EAT_vscanf 
#define P99__EAT_vsprintf 
#define P99__EAT_vsscanf 
#define P99__EAT_vswprintf 
#define P99__EAT_vwprintf 
#define P99__EAT_wchar_t 
#define P99__EAT_wcrtomb 
#define P99__EAT_wcscat 
#define P99__EAT_wcschr 
#define P99__EAT_wcscmp 
#define P99__EAT_wcscoll 
#define P99__EAT_wcscpy 
#define P99__EAT_wcscspn 
#define P99__EAT_wcsftime 
#define P99__EAT_wcslen 
#define P99__EAT_wcsncat 
#define P99__EAT_wcsncmp 
#define P99__EAT_wcsncpy 
#define P99__EAT_wcspbrk 
#define P99__EAT_wcsrchr 
#define P99__EAT_wcsrtombs 
#define P99__EAT_wcsspn 
#define P99__EAT_wcsstr 
#define P99__EAT_wcstod 
#define P99__EAT_wcstoimax 
#define P99__EAT_wcstok 
#define P99__EAT_wcstol 
#define P99__EAT_wcstoul 
#define P99__EAT_wcstoumax 
#define P99__EAT_wcsxfrm 
#define P99__EAT_wctob 
#define P99__EAT_wctrans 
#define P99__EAT_wctrans_t 
#define P99__EAT_wctype 
#define P99__EAT_wctype_t 
#define P99__EAT_while 
#define P99__EAT_wint_t 
#define P99__EAT_wmemchr 
#define P99__EAT_wmemcmp 
#define P99__EAT_wmemcpy 
#define P99__EAT_wmemmove 
#define P99__EAT_wmemset 
#define P99__EAT_wprintf 
#define P99__EAT_wscanf 
#define P99__TOKEN_0 ,
#define P99__TOKEN_1 ,
#define P99__TOKEN_2 ,
#define P99__TOKEN_3 ,
#define P99__TOKEN_4 ,
#define P99__TOKEN_5 ,
#define P99__TOKEN_6 ,
#define P99__TOKEN_7 ,
#define P99__TOKEN_8 ,
#define P99__TOKEN_9 ,
#define P99__TOKEN_10 ,
#define P99__TOKEN_11 ,
#define P99__TOKEN_12 ,
#define P99__TOKEN_13 ,
#define P99__TOKEN_14 ,
#define P99__TOKEN_15 ,
#define P99__TOKEN_16 ,
#define P99__TOKEN_17 ,
#define P99__TOKEN_18 ,
#define P99__TOKEN_19 ,
#define P99__TOKEN_20 ,
#define P99__TOKEN_21 ,
#define P99__TOKEN_22 ,
#define P99__TOKEN_23 ,
#define P99__TOKEN_24 ,
#define P99__TOKEN_25 ,
#define P99__TOKEN_26 ,
#define P99__TOKEN_27 ,
#define P99__TOKEN_28 ,
#define P99__TOKEN_29 ,
#define P99__TOKEN_30 ,
#define P99__TOKEN_31 ,
#define P99__TOKEN_32 ,
#define P99__TOKEN_33 ,
#define P99__TOKEN_34 ,
#define P99__TOKEN_35 ,
#define P99__TOKEN_36 ,
#define P99__TOKEN_37 ,
#define P99__TOKEN_38 ,
#define P99__TOKEN_39 ,
#define P99__TOKEN_40 ,
#define P99__TOKEN_41 ,
#define P99__TOKEN_42 ,
#define P99__TOKEN_43 ,
#define P99__TOKEN_44 ,
#define P99__TOKEN_45 ,
#define P99__TOKEN_46 ,
#define P99__TOKEN_47 ,
#define P99__TOKEN_48 ,
#define P99__TOKEN_49 ,
#define P99__TOKEN_50 ,
#define P99__TOKEN_51 ,
#define P99__TOKEN_52 ,
#define P99__TOKEN_53 ,
#define P99__TOKEN_54 ,
#define P99__TOKEN_55 ,
#define P99__TOKEN_56 ,
#define P99__TOKEN_57 ,
#define P99__TOKEN_58 ,
#define P99__TOKEN_59 ,
#define P99__TOKEN_60 ,
#define P99__TOKEN_61 ,
#define P99__TOKEN_62 ,
#define P99__TOKEN_63 ,
#define P99__TOKEN_64 ,
#define P99__TOKEN_65 ,
#define P99__TOKEN_66 ,
#define P99__TOKEN_67 ,
#define P99__TOKEN_68 ,
#define P99__TOKEN_69 ,
#define P99__TOKEN_70 ,
#define P99__TOKEN_71 ,
#define P99__TOKEN_72 ,
#define P99__TOKEN_73 ,
#define P99__TOKEN_74 ,
#define P99__TOKEN_75 ,
#define P99__TOKEN_76 ,
#define P99__TOKEN_77 ,
#define P99__TOKEN_78 ,
#define P99__TOKEN_79 ,
#define P99__TOKEN_80 ,
#define P99__TOKEN_81 ,
#define P99__TOKEN_82 ,
#define P99__TOKEN_83 ,
#define P99__TOKEN_84 ,
#define P99__TOKEN_85 ,
#define P99__TOKEN_86 ,
#define P99__TOKEN_87 ,
#define P99__TOKEN_88 ,
#define P99__TOKEN_89 ,
#define P99__TOKEN_90 ,
#define P99__TOKEN_91 ,
#define P99__TOKEN_92 ,
#define P99__TOKEN_93 ,
#define P99__TOKEN_94 ,
#define P99__TOKEN_95 ,
#define P99__TOKEN_96 ,
#define P99__TOKEN_97 ,
#define P99__TOKEN_98 ,
#define P99__TOKEN_99 ,
#define P99__TOKEN_100 ,
#define P99__TOKEN_101 ,
#define P99__TOKEN_102 ,
#define P99__TOKEN_103 ,
#define P99__TOKEN_104 ,
#define P99__TOKEN_105 ,
#define P99__TOKEN_106 ,
#define P99__TOKEN_107 ,
#define P99__TOKEN_108 ,
#define P99__TOKEN_109 ,
#define P99__TOKEN_110 ,
#define P99__TOKEN_111 ,
#define P99__TOKEN_112 ,
#define P99__TOKEN_113 ,
#define P99__TOKEN_114 ,
#define P99__TOKEN_115 ,
#define P99__TOKEN_116 ,
#define P99__TOKEN_117 ,
#define P99__TOKEN_118 ,
#define P99__TOKEN_119 ,
#define P99__TOKEN_120 ,
#define P99__TOKEN_121 ,
#define P99__TOKEN__Bool ,
#define P99__TOKEN__Complex ,
#define P99__TOKEN__Exit ,
#define P99__TOKEN__Imaginary ,
#define P99__TOKEN_abort ,
#define P99__TOKEN_abs ,
#define P99__TOKEN_acos ,
#define P99__TOKEN_asctime ,
#define P99__TOKEN_asin ,
#define P99__TOKEN_assert ,
#define P99__TOKEN_atan ,
#define P99__TOKEN_atan2 ,
#define P99__TOKEN_atexit ,
#define P99__TOKEN_atof ,
#define P99__TOKEN_atof ,
#define P99__TOKEN_atoi ,
#define P99__TOKEN_atol ,
#define P99__TOKEN_auto ,
#define P99__TOKEN_break ,
#define P99__TOKEN_bsearch ,
#define P99__TOKEN_btowc ,
#define P99__TOKEN_cabs ,
#define P99__TOKEN_cacos ,
#define P99__TOKEN_cacosh ,
#define P99__TOKEN_calloc ,
#define P99__TOKEN_carg ,
#define P99__TOKEN_case ,
#define P99__TOKEN_casin ,
#define P99__TOKEN_casinh ,
#define P99__TOKEN_catan ,
#define P99__TOKEN_catanh ,
#define P99__TOKEN_ccos ,
#define P99__TOKEN_ccosh ,
#define P99__TOKEN_ceil ,
#define P99__TOKEN_cexp ,
#define P99__TOKEN_char ,
#define P99__TOKEN_cimag ,
#define P99__TOKEN_clearerr ,
#define P99__TOKEN_clock ,
#define P99__TOKEN_clock_t ,
#define P99__TOKEN_clog ,
#define P99__TOKEN_conj ,
#define P99__TOKEN_const ,
#define P99__TOKEN_continue ,
#define P99__TOKEN_cos ,
#define P99__TOKEN_cosh ,
#define P99__TOKEN_cpow ,
#define P99__TOKEN_cproj ,
#define P99__TOKEN_creal ,
#define P99__TOKEN_csin ,
#define P99__TOKEN_csinh ,
#define P99__TOKEN_csqrt ,
#define P99__TOKEN_ctan ,
#define P99__TOKEN_ctanh ,
#define P99__TOKEN_ctime ,
#define P99__TOKEN_default ,
#define P99__TOKEN_difftime ,
#define P99__TOKEN_div ,
#define P99__TOKEN_div_t ,
#define P99__TOKEN_do ,
#define P99__TOKEN_double ,
#define P99__TOKEN_double_t ,
#define P99__TOKEN_else ,
#define P99__TOKEN_enum ,
#define P99__TOKEN_exit ,
#define P99__TOKEN_exp ,
#define P99__TOKEN_extern ,
#define P99__TOKEN_fabs ,
#define P99__TOKEN_fclose ,
#define P99__TOKEN_fdopen ,
#define P99__TOKEN_fenv_t ,
#define P99__TOKEN_feof ,
#define P99__TOKEN_ferror ,
#define P99__TOKEN_fexcept_t ,
#define P99__TOKEN_fflush ,
#define P99__TOKEN_fgetc ,
#define P99__TOKEN_fgetpos ,
#define P99__TOKEN_fgets ,
#define P99__TOKEN_fgetwc ,
#define P99__TOKEN_fgetws ,
#define P99__TOKEN_float ,
#define P99__TOKEN_float_t ,
#define P99__TOKEN_floor ,
#define P99__TOKEN_fopen ,
#define P99__TOKEN_for ,
#define P99__TOKEN_fpos_t ,
#define P99__TOKEN_fprintf ,
#define P99__TOKEN_fprintf ,
#define P99__TOKEN_fputc ,
#define P99__TOKEN_fputchar ,
#define P99__TOKEN_fputs ,
#define P99__TOKEN_fputwc ,
#define P99__TOKEN_fputws ,
#define P99__TOKEN_fread ,
#define P99__TOKEN_free ,
#define P99__TOKEN_freopen ,
#define P99__TOKEN_frexp ,
#define P99__TOKEN_fscanf ,
#define P99__TOKEN_fscanf ,
#define P99__TOKEN_fseek ,
#define P99__TOKEN_fsetpos ,
#define P99__TOKEN_ftell ,
#define P99__TOKEN_fwide ,
#define P99__TOKEN_fwprintf ,
#define P99__TOKEN_fwrite ,
#define P99__TOKEN_fwscanf ,
#define P99__TOKEN_getaddrinfo ,
#define P99__TOKEN_getc ,
#define P99__TOKEN_getchar ,
#define P99__TOKEN_getenv ,
#define P99__TOKEN_getnameinfo ,
#define P99__TOKEN_gets ,
#define P99__TOKEN_getwc ,
#define P99__TOKEN_getwchar ,
#define P99__TOKEN_gmtime ,
#define P99__TOKEN_goto ,
#define P99__TOKEN_if ,
#define P99__TOKEN_imaxabs ,
#define P99__TOKEN_imaxdiv ,
#define P99__TOKEN_imaxdiv_t ,
#define P99__TOKEN_inline ,
#define P99__TOKEN_int ,
#define P99__TOKEN_int16_t ,
#define P99__TOKEN_int32_t ,
#define P99__TOKEN_int64_t ,
#define P99__TOKEN_int8_t ,
#define P99__TOKEN_intfast16_t ,
#define P99__TOKEN_intfast32_t ,
#define P99__TOKEN_intfast64_t ,
#define P99__TOKEN_intfast8_t ,
#define P99__TOKEN_intleast16_t ,
#define P99__TOKEN_intleast32_t ,
#define P99__TOKEN_intleast64_t ,
#define P99__TOKEN_intleast8_t ,
#define P99__TOKEN_intmax_t ,
#define P99__TOKEN_intptr_t ,
#define P99__TOKEN_isalnum ,
#define P99__TOKEN_isalpha ,
#define P99__TOKEN_isblank ,
#define P99__TOKEN_iscntrl ,
#define P99__TOKEN_isdigit ,
#define P99__TOKEN_isgraph ,
#define P99__TOKEN_islower ,
#define P99__TOKEN_isprint ,
#define P99__TOKEN_ispunct ,
#define P99__TOKEN_isspace ,
#define P99__TOKEN_isupper ,
#define P99__TOKEN_iswalnum ,
#define P99__TOKEN_iswalpha ,
#define P99__TOKEN_iswcntrl ,
#define P99__TOKEN_iswctype ,
#define P99__TOKEN_iswdigit ,
#define P99__TOKEN_iswgraph ,
#define P99__TOKEN_iswlower ,
#define P99__TOKEN_iswprint ,
#define P99__TOKEN_iswpunct ,
#define P99__TOKEN_iswspace ,
#define P99__TOKEN_iswupper ,
#define P99__TOKEN_iswxdigit ,
#define P99__TOKEN_isxdigit ,
#define P99__TOKEN_labs ,
#define P99__TOKEN_ldexp ,
#define P99__TOKEN_ldiv ,
#define P99__TOKEN_ldiv ,
#define P99__TOKEN_ldiv_t ,
#define P99__TOKEN_lldiv_t ,
#define P99__TOKEN_localeconv ,
#define P99__TOKEN_localtime ,
#define P99__TOKEN_log ,
#define P99__TOKEN_log10 ,
#define P99__TOKEN_long ,
#define P99__TOKEN_longjmp ,
#define P99__TOKEN_malloc ,
#define P99__TOKEN_mbrlen ,
#define P99__TOKEN_mbrtowc ,
#define P99__TOKEN_mbsinit ,
#define P99__TOKEN_mbsrtowcs ,
#define P99__TOKEN_mbstate_t ,
#define P99__TOKEN_memchr ,
#define P99__TOKEN_memcmp ,
#define P99__TOKEN_memcpy ,
#define P99__TOKEN_memmove ,
#define P99__TOKEN_memset ,
#define P99__TOKEN_mktime ,
#define P99__TOKEN_modf ,
#define P99__TOKEN_off_t ,
#define P99__TOKEN_offsetof ,
#define P99__TOKEN_perror ,
#define P99__TOKEN_pow ,
#define P99__TOKEN_printf ,
#define P99__TOKEN_ptrdiff_t ,
#define P99__TOKEN_putc ,
#define P99__TOKEN_putchar ,
#define P99__TOKEN_puts ,
#define P99__TOKEN_putwc ,
#define P99__TOKEN_putwchar ,
#define P99__TOKEN_qsort ,
#define P99__TOKEN_raise ,
#define P99__TOKEN_rand ,
#define P99__TOKEN_realloc ,
#define P99__TOKEN_register ,
#define P99__TOKEN_remove ,
#define P99__TOKEN_rename ,
#define P99__TOKEN_restrict ,
#define P99__TOKEN_return ,
#define P99__TOKEN_rewind ,
#define P99__TOKEN_scanf ,
#define P99__TOKEN_setbuf ,
#define P99__TOKEN_setjmp ,
#define P99__TOKEN_setlocale ,
#define P99__TOKEN_setvbuf ,
#define P99__TOKEN_short ,
#define P99__TOKEN_sig_atomic_t ,
#define P99__TOKEN_signed ,
#define P99__TOKEN_sin ,
#define P99__TOKEN_sinh ,
#define P99__TOKEN_size_t ,
#define P99__TOKEN_sizeof ,
#define P99__TOKEN_snprintf ,
#define P99__TOKEN_sprintf ,
#define P99__TOKEN_sqrt ,
#define P99__TOKEN_srand ,
#define P99__TOKEN_sscanf ,
#define P99__TOKEN_ssize_t ,
#define P99__TOKEN_static ,
#define P99__TOKEN_strcat ,
#define P99__TOKEN_strchr ,
#define P99__TOKEN_strcmp ,
#define P99__TOKEN_strcoll ,
#define P99__TOKEN_strcpy ,
#define P99__TOKEN_strcspn ,
#define P99__TOKEN_strerror ,
#define P99__TOKEN_strftime ,
#define P99__TOKEN_strlen ,
#define P99__TOKEN_strncat ,
#define P99__TOKEN_strncmp ,
#define P99__TOKEN_strncpy ,
#define P99__TOKEN_strpbrk ,
#define P99__TOKEN_strrchr ,
#define P99__TOKEN_strspn ,
#define P99__TOKEN_strstr ,
#define P99__TOKEN_strtod ,
#define P99__TOKEN_strtoimax ,
#define P99__TOKEN_strtok ,
#define P99__TOKEN_strtol ,
#define P99__TOKEN_strtoul ,
#define P99__TOKEN_strtoumax ,
#define P99__TOKEN_struct ,
#define P99__TOKEN_strxfrm ,
#define P99__TOKEN_switch ,
#define P99__TOKEN_swprintf ,
#define P99__TOKEN_swscanf ,
#define P99__TOKEN_system ,
#define P99__TOKEN_tan ,
#define P99__TOKEN_tanh ,
#define P99__TOKEN_time ,
#define P99__TOKEN_time_t ,
#define P99__TOKEN_tmpfile ,
#define P99__TOKEN_tmpnam ,
#define P99__TOKEN_tolower ,
#define P99__TOKEN_toupper ,
#define P99__TOKEN_towctrans ,
#define P99__TOKEN_towlower ,
#define P99__TOKEN_towupper ,
#define P99__TOKEN_typedef ,
#define P99__TOKEN_uint16_t ,
#define P99__TOKEN_uint32_t ,
#define P99__TOKEN_uint64_t ,
#define P99__TOKEN_uint8_t ,
#define P99__TOKEN_uintfast16_t ,
#define P99__TOKEN_uintfast32_t ,
#define P99__TOKEN_uintfast64_t ,
#define P99__TOKEN_uintfast8_t ,
#define P99__TOKEN_uintleast16_t ,
#define P99__TOKEN_uintleast32_t ,
#define P99__TOKEN_uintleast64_t ,
#define P99__TOKEN_uintleast8_t ,
#define P99__TOKEN_uintmax_t ,
#define P99__TOKEN_uintptr_t ,
#define P99__TOKEN_ungetc ,
#define P99__TOKEN_ungetwc ,
#define P99__TOKEN_union ,
#define P99__TOKEN_unsigned ,
#define P99__TOKEN_va_arg ,
#define P99__TOKEN_va_copy ,
#define P99__TOKEN_va_end ,
#define P99__TOKEN_va_start ,
#define P99__TOKEN_vfprintf ,
#define P99__TOKEN_vfscanf ,
#define P99__TOKEN_vfwprintf ,
#define P99__TOKEN_void ,
#define P99__TOKEN_volatile ,
#define P99__TOKEN_vprintf ,
#define P99__TOKEN_vscanf ,
#define P99__TOKEN_vsprintf ,
#define P99__TOKEN_vsscanf ,
#define P99__TOKEN_vswprintf ,
#define P99__TOKEN_vwprintf ,
#define P99__TOKEN_wchar_t ,
#define P99__TOKEN_wcrtomb ,
#define P99__TOKEN_wcscat ,
#define P99__TOKEN_wcschr ,
#define P99__TOKEN_wcscmp ,
#define P99__TOKEN_wcscoll ,
#define P99__TOKEN_wcscpy ,
#define P99__TOKEN_wcscspn ,
#define P99__TOKEN_wcsftime ,
#define P99__TOKEN_wcslen ,
#define P99__TOKEN_wcsncat ,
#define P99__TOKEN_wcsncmp ,
#define P99__TOKEN_wcsncpy ,
#define P99__TOKEN_wcspbrk ,
#define P99__TOKEN_wcsrchr ,
#define P99__TOKEN_wcsrtombs ,
#define P99__TOKEN_wcsspn ,
#define P99__TOKEN_wcsstr ,
#define P99__TOKEN_wcstod ,
#define P99__TOKEN_wcstoimax ,
#define P99__TOKEN_wcstok ,
#define P99__TOKEN_wcstol ,
#define P99__TOKEN_wcstoul ,
#define P99__TOKEN_wcstoumax ,
#define P99__TOKEN_wcsxfrm ,
#define P99__TOKEN_wctob ,
#define P99__TOKEN_wctrans ,
#define P99__TOKEN_wctrans_t ,
#define P99__TOKEN_wctype ,
#define P99__TOKEN_wctype_t ,
#define P99__TOKEN_while ,
#define P99__TOKEN_wint_t ,
#define P99__TOKEN_wmemchr ,
#define P99__TOKEN_wmemcmp ,
#define P99__TOKEN_wmemcpy ,
#define P99__TOKEN_wmemmove ,
#define P99__TOKEN_wmemset ,
#define P99__TOKEN_wprintf ,
#define P99__TOKEN_wscanf ,
#define P99__QUALIFIER_const ,
#define P99__QUALIFIER_restrict ,
#define P99__QUALIFIER_volatile ,
#define P99__VOID_void ,
#define P99__BOOL__Bool ,
#define P99__BOOL_bool ,
#define P99__INTEGER_char ,
#define P99__INTEGER_int ,
#define P99__INTEGER_long ,
#define P99__INTEGER_short ,
#define P99__INTEGER_signed ,
#define P99__INTEGER_unsigned ,
#define P99__REAL_double ,
#define P99__REAL_float ,
#define P99__REAL_long ,
#define P99__FLOAT__Complex ,
#define P99__FLOAT__Imaginary ,
#define P99__FLOAT_double ,
#define P99__FLOAT_float ,
#define P99__FLOAT_long ,
#define P99__TYPE__Bool ,
#define P99__TYPE__Complex ,
#define P99__TYPE__Imaginary ,
#define P99__TYPE_char ,
#define P99__TYPE_const ,
#define P99__TYPE_double ,
#define P99__TYPE_float ,
#define P99__TYPE_int ,
#define P99__TYPE_long ,
#define P99__TYPE_restrict ,
#define P99__TYPE_short ,
#define P99__TYPE_signed ,
#define P99__TYPE_unsigned ,
#define P99__TYPE_void ,
#define P99__TYPE_volatile ,
#define P99__VOID_QUAL_void ,
#define P99__VOID_QUAL_const ,
#define P99__VOID_QUAL_restrict ,
#define P99__VOID_QUAL_volatile ,
#define P99__BOOL_QUAL__Bool ,
#define P99__BOOL_QUAL_bool ,
#define P99__BOOL_QUAL_const ,
#define P99__BOOL_QUAL_restrict ,
#define P99__BOOL_QUAL_volatile ,
#define P99__INTEGER_QUAL_char ,
#define P99__INTEGER_QUAL_int ,
#define P99__INTEGER_QUAL_long ,
#define P99__INTEGER_QUAL_short ,
#define P99__INTEGER_QUAL_signed ,
#define P99__INTEGER_QUAL_unsigned ,
#define P99__INTEGER_QUAL_const ,
#define P99__INTEGER_QUAL_restrict ,
#define P99__INTEGER_QUAL_volatile ,
#define P99__REAL_QUAL_double ,
#define P99__REAL_QUAL_float ,
#define P99__REAL_QUAL_long ,
#define P99__REAL_QUAL_const ,
#define P99__REAL_QUAL_restrict ,
#define P99__REAL_QUAL_volatile ,
#define P99__FLOAT_QUAL__Complex ,
#define P99__FLOAT_QUAL__Imaginary ,
#define P99__FLOAT_QUAL_double ,
#define P99__FLOAT_QUAL_float ,
#define P99__FLOAT_QUAL_long ,
#define P99__FLOAT_QUAL_const ,
#define P99__FLOAT_QUAL_restrict ,
#define P99__FLOAT_QUAL_volatile ,
#define P99__TYPE_QUAL__Bool ,
#define P99__TYPE_QUAL__Complex ,
#define P99__TYPE_QUAL__Imaginary ,
#define P99__TYPE_QUAL_char ,
#define P99__TYPE_QUAL_const ,
#define P99__TYPE_QUAL_double ,
#define P99__TYPE_QUAL_float ,
#define P99__TYPE_QUAL_int ,
#define P99__TYPE_QUAL_long ,
#define P99__TYPE_QUAL_restrict ,
#define P99__TYPE_QUAL_short ,
#define P99__TYPE_QUAL_signed ,
#define P99__TYPE_QUAL_unsigned ,
#define P99__TYPE_QUAL_void ,
#define P99__TYPE_QUAL_volatile ,
#define P99__TYPE_QUAL_const ,
#define P99__TYPE_QUAL_restrict ,
#define P99__TYPE_QUAL_volatile ,
#define P99__KEYWORD__Bool ,
#define P99__KEYWORD__Complex ,
#define P99__KEYWORD__Imaginary ,
#define P99__KEYWORD_auto ,
#define P99__KEYWORD_break ,
#define P99__KEYWORD_case ,
#define P99__KEYWORD_char ,
#define P99__KEYWORD_const ,
#define P99__KEYWORD_continue ,
#define P99__KEYWORD_default ,
#define P99__KEYWORD_do ,
#define P99__KEYWORD_double ,
#define P99__KEYWORD_else ,
#define P99__KEYWORD_enum ,
#define P99__KEYWORD_extern ,
#define P99__KEYWORD_float ,
#define P99__KEYWORD_for ,
#define P99__KEYWORD_goto ,
#define P99__KEYWORD_if ,
#define P99__KEYWORD_inline ,
#define P99__KEYWORD_int ,
#define P99__KEYWORD_long ,
#define P99__KEYWORD_register ,
#define P99__KEYWORD_restrict ,
#define P99__KEYWORD_return ,
#define P99__KEYWORD_short ,
#define P99__KEYWORD_signed ,
#define P99__KEYWORD_sizeof ,
#define P99__KEYWORD_static ,
#define P99__KEYWORD_struct ,
#define P99__KEYWORD_switch ,
#define P99__KEYWORD_typedef ,
#define P99__KEYWORD_union ,
#define P99__KEYWORD_unsigned ,
#define P99__KEYWORD_void ,
#define P99__KEYWORD_volatile ,
#define P99__KEYWORD_while ,
#define P99__RESERVED__Bool ,
#define P99__RESERVED__Complex ,
#define P99__RESERVED__Exit ,
#define P99__RESERVED__Imaginary ,
#define P99__RESERVED_abort ,
#define P99__RESERVED_abs ,
#define P99__RESERVED_acos ,
#define P99__RESERVED_asctime ,
#define P99__RESERVED_asin ,
#define P99__RESERVED_assert ,
#define P99__RESERVED_atan ,
#define P99__RESERVED_atan2 ,
#define P99__RESERVED_atexit ,
#define P99__RESERVED_atof ,
#define P99__RESERVED_atof ,
#define P99__RESERVED_atoi ,
#define P99__RESERVED_atol ,
#define P99__RESERVED_auto ,
#define P99__RESERVED_break ,
#define P99__RESERVED_bsearch ,
#define P99__RESERVED_btowc ,
#define P99__RESERVED_cabs ,
#define P99__RESERVED_cacos ,
#define P99__RESERVED_cacosh ,
#define P99__RESERVED_calloc ,
#define P99__RESERVED_carg ,
#define P99__RESERVED_case ,
#define P99__RESERVED_casin ,
#define P99__RESERVED_casinh ,
#define P99__RESERVED_catan ,
#define P99__RESERVED_catanh ,
#define P99__RESERVED_ccos ,
#define P99__RESERVED_ccosh ,
#define P99__RESERVED_ceil ,
#define P99__RESERVED_cexp ,
#define P99__RESERVED_char ,
#define P99__RESERVED_cimag ,
#define P99__RESERVED_clearerr ,
#define P99__RESERVED_clock ,
#define P99__RESERVED_clock_t ,
#define P99__RESERVED_clog ,
#define P99__RESERVED_conj ,
#define P99__RESERVED_const ,
#define P99__RESERVED_continue ,
#define P99__RESERVED_cos ,
#define P99__RESERVED_cosh ,
#define P99__RESERVED_cpow ,
#define P99__RESERVED_cproj ,
#define P99__RESERVED_creal ,
#define P99__RESERVED_csin ,
#define P99__RESERVED_csinh ,
#define P99__RESERVED_csqrt ,
#define P99__RESERVED_ctan ,
#define P99__RESERVED_ctanh ,
#define P99__RESERVED_ctime ,
#define P99__RESERVED_default ,
#define P99__RESERVED_difftime ,
#define P99__RESERVED_div ,
#define P99__RESERVED_div_t ,
#define P99__RESERVED_do ,
#define P99__RESERVED_double ,
#define P99__RESERVED_double_t ,
#define P99__RESERVED_else ,
#define P99__RESERVED_enum ,
#define P99__RESERVED_exit ,
#define P99__RESERVED_exp ,
#define P99__RESERVED_extern ,
#define P99__RESERVED_fabs ,
#define P99__RESERVED_fclose ,
#define P99__RESERVED_fdopen ,
#define P99__RESERVED_fenv_t ,
#define P99__RESERVED_feof ,
#define P99__RESERVED_ferror ,
#define P99__RESERVED_fexcept_t ,
#define P99__RESERVED_fflush ,
#define P99__RESERVED_fgetc ,
#define P99__RESERVED_fgetpos ,
#define P99__RESERVED_fgets ,
#define P99__RESERVED_fgetwc ,
#define P99__RESERVED_fgetws ,
#define P99__RESERVED_float ,
#define P99__RESERVED_float_t ,
#define P99__RESERVED_floor ,
#define P99__RESERVED_fopen ,
#define P99__RESERVED_for ,
#define P99__RESERVED_fpos_t ,
#define P99__RESERVED_fprintf ,
#define P99__RESERVED_fprintf ,
#define P99__RESERVED_fputc ,
#define P99__RESERVED_fputchar ,
#define P99__RESERVED_fputs ,
#define P99__RESERVED_fputwc ,
#define P99__RESERVED_fputws ,
#define P99__RESERVED_fread ,
#define P99__RESERVED_free ,
#define P99__RESERVED_freopen ,
#define P99__RESERVED_frexp ,
#define P99__RESERVED_fscanf ,
#define P99__RESERVED_fscanf ,
#define P99__RESERVED_fseek ,
#define P99__RESERVED_fsetpos ,
#define P99__RESERVED_ftell ,
#define P99__RESERVED_fwide ,
#define P99__RESERVED_fwprintf ,
#define P99__RESERVED_fwrite ,
#define P99__RESERVED_fwscanf ,
#define P99__RESERVED_getaddrinfo ,
#define P99__RESERVED_getc ,
#define P99__RESERVED_getchar ,
#define P99__RESERVED_getenv ,
#define P99__RESERVED_getnameinfo ,
#define P99__RESERVED_gets ,
#define P99__RESERVED_getwc ,
#define P99__RESERVED_getwchar ,
#define P99__RESERVED_gmtime ,
#define P99__RESERVED_goto ,
#define P99__RESERVED_if ,
#define P99__RESERVED_imaxabs ,
#define P99__RESERVED_imaxdiv ,
#define P99__RESERVED_imaxdiv_t ,
#define P99__RESERVED_inline ,
#define P99__RESERVED_int ,
#define P99__RESERVED_int16_t ,
#define P99__RESERVED_int32_t ,
#define P99__RESERVED_int64_t ,
#define P99__RESERVED_int8_t ,
#define P99__RESERVED_intfast16_t ,
#define P99__RESERVED_intfast32_t ,
#define P99__RESERVED_intfast64_t ,
#define P99__RESERVED_intfast8_t ,
#define P99__RESERVED_intleast16_t ,
#define P99__RESERVED_intleast32_t ,
#define P99__RESERVED_intleast64_t ,
#define P99__RESERVED_intleast8_t ,
#define P99__RESERVED_intmax_t ,
#define P99__RESERVED_intptr_t ,
#define P99__RESERVED_isalnum ,
#define P99__RESERVED_isalpha ,
#define P99__RESERVED_isblank ,
#define P99__RESERVED_iscntrl ,
#define P99__RESERVED_isdigit ,
#define P99__RESERVED_isgraph ,
#define P99__RESERVED_islower ,
#define P99__RESERVED_isprint ,
#define P99__RESERVED_ispunct ,
#define P99__RESERVED_isspace ,
#define P99__RESERVED_isupper ,
#define P99__RESERVED_iswalnum ,
#define P99__RESERVED_iswalpha ,
#define P99__RESERVED_iswcntrl ,
#define P99__RESERVED_iswctype ,
#define P99__RESERVED_iswdigit ,
#define P99__RESERVED_iswgraph ,
#define P99__RESERVED_iswlower ,
#define P99__RESERVED_iswprint ,
#define P99__RESERVED_iswpunct ,
#define P99__RESERVED_iswspace ,
#define P99__RESERVED_iswupper ,
#define P99__RESERVED_iswxdigit ,
#define P99__RESERVED_isxdigit ,
#define P99__RESERVED_labs ,
#define P99__RESERVED_ldexp ,
#define P99__RESERVED_ldiv ,
#define P99__RESERVED_ldiv ,
#define P99__RESERVED_ldiv_t ,
#define P99__RESERVED_lldiv_t ,
#define P99__RESERVED_localeconv ,
#define P99__RESERVED_localtime ,
#define P99__RESERVED_log ,
#define P99__RESERVED_log10 ,
#define P99__RESERVED_long ,
#define P99__RESERVED_longjmp ,
#define P99__RESERVED_malloc ,
#define P99__RESERVED_mbrlen ,
#define P99__RESERVED_mbrtowc ,
#define P99__RESERVED_mbsinit ,
#define P99__RESERVED_mbsrtowcs ,
#define P99__RESERVED_mbstate_t ,
#define P99__RESERVED_memchr ,
#define P99__RESERVED_memcmp ,
#define P99__RESERVED_memcpy ,
#define P99__RESERVED_memmove ,
#define P99__RESERVED_memset ,
#define P99__RESERVED_mktime ,
#define P99__RESERVED_modf ,
#define P99__RESERVED_off_t ,
#define P99__RESERVED_offsetof ,
#define P99__RESERVED_perror ,
#define P99__RESERVED_pow ,
#define P99__RESERVED_printf ,
#define P99__RESERVED_ptrdiff_t ,
#define P99__RESERVED_putc ,
#define P99__RESERVED_putchar ,
#define P99__RESERVED_puts ,
#define P99__RESERVED_putwc ,
#define P99__RESERVED_putwchar ,
#define P99__RESERVED_qsort ,
#define P99__RESERVED_raise ,
#define P99__RESERVED_rand ,
#define P99__RESERVED_realloc ,
#define P99__RESERVED_register ,
#define P99__RESERVED_remove ,
#define P99__RESERVED_rename ,
#define P99__RESERVED_restrict ,
#define P99__RESERVED_return ,
#define P99__RESERVED_rewind ,
#define P99__RESERVED_scanf ,
#define P99__RESERVED_setbuf ,
#define P99__RESERVED_setjmp ,
#define P99__RESERVED_setlocale ,
#define P99__RESERVED_setvbuf ,
#define P99__RESERVED_short ,
#define P99__RESERVED_sig_atomic_t ,
#define P99__RESERVED_signed ,
#define P99__RESERVED_sin ,
#define P99__RESERVED_sinh ,
#define P99__RESERVED_size_t ,
#define P99__RESERVED_sizeof ,
#define P99__RESERVED_snprintf ,
#define P99__RESERVED_sprintf ,
#define P99__RESERVED_sqrt ,
#define P99__RESERVED_srand ,
#define P99__RESERVED_sscanf ,
#define P99__RESERVED_ssize_t ,
#define P99__RESERVED_static ,
#define P99__RESERVED_strcat ,
#define P99__RESERVED_strchr ,
#define P99__RESERVED_strcmp ,
#define P99__RESERVED_strcoll ,
#define P99__RESERVED_strcpy ,
#define P99__RESERVED_strcspn ,
#define P99__RESERVED_strerror ,
#define P99__RESERVED_strftime ,
#define P99__RESERVED_strlen ,
#define P99__RESERVED_strncat ,
#define P99__RESERVED_strncmp ,
#define P99__RESERVED_strncpy ,
#define P99__RESERVED_strpbrk ,
#define P99__RESERVED_strrchr ,
#define P99__RESERVED_strspn ,
#define P99__RESERVED_strstr ,
#define P99__RESERVED_strtod ,
#define P99__RESERVED_strtoimax ,
#define P99__RESERVED_strtok ,
#define P99__RESERVED_strtol ,
#define P99__RESERVED_strtoul ,
#define P99__RESERVED_strtoumax ,
#define P99__RESERVED_struct ,
#define P99__RESERVED_strxfrm ,
#define P99__RESERVED_switch ,
#define P99__RESERVED_swprintf ,
#define P99__RESERVED_swscanf ,
#define P99__RESERVED_system ,
#define P99__RESERVED_tan ,
#define P99__RESERVED_tanh ,
#define P99__RESERVED_time ,
#define P99__RESERVED_time_t ,
#define P99__RESERVED_tmpfile ,
#define P99__RESERVED_tmpnam ,
#define P99__RESERVED_tolower ,
#define P99__RESERVED_toupper ,
#define P99__RESERVED_towctrans ,
#define P99__RESERVED_towlower ,
#define P99__RESERVED_towupper ,
#define P99__RESERVED_typedef ,
#define P99__RESERVED_uint16_t ,
#define P99__RESERVED_uint32_t ,
#define P99__RESERVED_uint64_t ,
#define P99__RESERVED_uint8_t ,
#define P99__RESERVED_uintfast16_t ,
#define P99__RESERVED_uintfast32_t ,
#define P99__RESERVED_uintfast64_t ,
#define P99__RESERVED_uintfast8_t ,
#define P99__RESERVED_uintleast16_t ,
#define P99__RESERVED_uintleast32_t ,
#define P99__RESERVED_uintleast64_t ,
#define P99__RESERVED_uintleast8_t ,
#define P99__RESERVED_uintmax_t ,
#define P99__RESERVED_uintptr_t ,
#define P99__RESERVED_ungetc ,
#define P99__RESERVED_ungetwc ,
#define P99__RESERVED_union ,
#define P99__RESERVED_unsigned ,
#define P99__RESERVED_va_arg ,
#define P99__RESERVED_va_copy ,
#define P99__RESERVED_va_end ,
#define P99__RESERVED_va_start ,
#define P99__RESERVED_vfprintf ,
#define P99__RESERVED_vfscanf ,
#define P99__RESERVED_vfwprintf ,
#define P99__RESERVED_void ,
#define P99__RESERVED_volatile ,
#define P99__RESERVED_vprintf ,
#define P99__RESERVED_vscanf ,
#define P99__RESERVED_vsprintf ,
#define P99__RESERVED_vsscanf ,
#define P99__RESERVED_vswprintf ,
#define P99__RESERVED_vwprintf ,
#define P99__RESERVED_wchar_t ,
#define P99__RESERVED_wcrtomb ,
#define P99__RESERVED_wcscat ,
#define P99__RESERVED_wcschr ,
#define P99__RESERVED_wcscmp ,
#define P99__RESERVED_wcscoll ,
#define P99__RESERVED_wcscpy ,
#define P99__RESERVED_wcscspn ,
#define P99__RESERVED_wcsftime ,
#define P99__RESERVED_wcslen ,
#define P99__RESERVED_wcsncat ,
#define P99__RESERVED_wcsncmp ,
#define P99__RESERVED_wcsncpy ,
#define P99__RESERVED_wcspbrk ,
#define P99__RESERVED_wcsrchr ,
#define P99__RESERVED_wcsrtombs ,
#define P99__RESERVED_wcsspn ,
#define P99__RESERVED_wcsstr ,
#define P99__RESERVED_wcstod ,
#define P99__RESERVED_wcstoimax ,
#define P99__RESERVED_wcstok ,
#define P99__RESERVED_wcstol ,
#define P99__RESERVED_wcstoul ,
#define P99__RESERVED_wcstoumax ,
#define P99__RESERVED_wcsxfrm ,
#define P99__RESERVED_wctob ,
#define P99__RESERVED_wctrans ,
#define P99__RESERVED_wctrans_t ,
#define P99__RESERVED_wctype ,
#define P99__RESERVED_wctype_t ,
#define P99__RESERVED_while ,
#define P99__RESERVED_wint_t ,
#define P99__RESERVED_wmemchr ,
#define P99__RESERVED_wmemcmp ,
#define P99__RESERVED_wmemcpy ,
#define P99__RESERVED_wmemmove ,
#define P99__RESERVED_wmemset ,
#define P99__RESERVED_wprintf ,
#define P99__RESERVED_wscanf ,
#define P99__CONST7(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(8)(7)
#define P99__CONST6(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST7(P99__EAT_FIRST(SEQ)))(6)
#define P99__CONST5(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST6(P99__EAT_FIRST(SEQ)))(5)
#define P99__CONST4(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST5(P99__EAT_FIRST(SEQ)))(4)
#define P99__CONST3(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST4(P99__EAT_FIRST(SEQ)))(3)
#define P99__CONST2(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST3(P99__EAT_FIRST(SEQ)))(2)
#define P99__CONST1(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST2(P99__EAT_FIRST(SEQ)))(1)
#define P99__CONST_CLASSIFY_7(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ) P99__CONST_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__CONST_CLASSIFY_6(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ) P99__CONST_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__CONST_CLASSIFY_5(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ) P99__CONST_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__CONST_CLASSIFY_4(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ) P99__CONST_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__CONST_CLASSIFY_3(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ) P99__CONST_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__CONST_CLASSIFY_2(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ) P99__CONST_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__CONST_CLASSIFY_1(SEQ) P99_PASTE2(P99__CONST_CLASSIFY_, SEQ)
#define P99__CONST_CLASSIFY___(CODE) P99_IF_ELSE(P99__CONST_VALIDATE(CODE))(P99_PASTE2(P99__CONST_CLASSIFY__, CODE))(INVALID_CONST_TYPE_EXPRESSION[CODE])
#define P99__CONST_CLASSIFY__(N, ...) P99__CONST_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__CONST_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__CONST_CLASSIFY__(N, P99_PASTE2(P99__CONST_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind CONST */
#define P99__CONST_CLASSIFY(SEQ) P99__CONST_CLASSIFY_(P99_CONST_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind CONST */
#define P99__CONST_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__CONST_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind CONST */
#define P99_CONST_LEN(SEQ) P99_IF_ELSE(P99_IS_CONST_TOK(SEQ))(P99__CONST1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind CONST */
#define P99_IS_CONST_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__CONST_, SEQ))
#define P99__VOLATILE7(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(8)(7)
#define P99__VOLATILE6(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE7(P99__EAT_FIRST(SEQ)))(6)
#define P99__VOLATILE5(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE6(P99__EAT_FIRST(SEQ)))(5)
#define P99__VOLATILE4(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE5(P99__EAT_FIRST(SEQ)))(4)
#define P99__VOLATILE3(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE4(P99__EAT_FIRST(SEQ)))(3)
#define P99__VOLATILE2(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE3(P99__EAT_FIRST(SEQ)))(2)
#define P99__VOLATILE1(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE2(P99__EAT_FIRST(SEQ)))(1)
#define P99__VOLATILE_CLASSIFY_7(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ) P99__VOLATILE_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__VOLATILE_CLASSIFY_6(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ) P99__VOLATILE_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__VOLATILE_CLASSIFY_5(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ) P99__VOLATILE_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__VOLATILE_CLASSIFY_4(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ) P99__VOLATILE_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__VOLATILE_CLASSIFY_3(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ) P99__VOLATILE_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__VOLATILE_CLASSIFY_2(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ) P99__VOLATILE_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__VOLATILE_CLASSIFY_1(SEQ) P99_PASTE2(P99__VOLATILE_CLASSIFY_, SEQ)
#define P99__VOLATILE_CLASSIFY___(CODE) P99_IF_ELSE(P99__VOLATILE_VALIDATE(CODE))(P99_PASTE2(P99__VOLATILE_CLASSIFY__, CODE))(INVALID_VOLATILE_TYPE_EXPRESSION[CODE])
#define P99__VOLATILE_CLASSIFY__(N, ...) P99__VOLATILE_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__VOLATILE_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__VOLATILE_CLASSIFY__(N, P99_PASTE2(P99__VOLATILE_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind VOLATILE */
#define P99__VOLATILE_CLASSIFY(SEQ) P99__VOLATILE_CLASSIFY_(P99_VOLATILE_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind VOLATILE */
#define P99__VOLATILE_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__VOLATILE_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind VOLATILE */
#define P99_VOLATILE_LEN(SEQ) P99_IF_ELSE(P99_IS_VOLATILE_TOK(SEQ))(P99__VOLATILE1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind VOLATILE */
#define P99_IS_VOLATILE_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__VOLATILE_, SEQ))
#define P99__RESTRICT7(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(8)(7)
#define P99__RESTRICT6(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT7(P99__EAT_FIRST(SEQ)))(6)
#define P99__RESTRICT5(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT6(P99__EAT_FIRST(SEQ)))(5)
#define P99__RESTRICT4(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT5(P99__EAT_FIRST(SEQ)))(4)
#define P99__RESTRICT3(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT4(P99__EAT_FIRST(SEQ)))(3)
#define P99__RESTRICT2(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT3(P99__EAT_FIRST(SEQ)))(2)
#define P99__RESTRICT1(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT2(P99__EAT_FIRST(SEQ)))(1)
#define P99__RESTRICT_CLASSIFY_7(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ) P99__RESTRICT_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__RESTRICT_CLASSIFY_6(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ) P99__RESTRICT_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__RESTRICT_CLASSIFY_5(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ) P99__RESTRICT_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__RESTRICT_CLASSIFY_4(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ) P99__RESTRICT_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__RESTRICT_CLASSIFY_3(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ) P99__RESTRICT_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__RESTRICT_CLASSIFY_2(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ) P99__RESTRICT_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__RESTRICT_CLASSIFY_1(SEQ) P99_PASTE2(P99__RESTRICT_CLASSIFY_, SEQ)
#define P99__RESTRICT_CLASSIFY___(CODE) P99_IF_ELSE(P99__RESTRICT_VALIDATE(CODE))(P99_PASTE2(P99__RESTRICT_CLASSIFY__, CODE))(INVALID_RESTRICT_TYPE_EXPRESSION[CODE])
#define P99__RESTRICT_CLASSIFY__(N, ...) P99__RESTRICT_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__RESTRICT_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__RESTRICT_CLASSIFY__(N, P99_PASTE2(P99__RESTRICT_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind RESTRICT */
#define P99__RESTRICT_CLASSIFY(SEQ) P99__RESTRICT_CLASSIFY_(P99_RESTRICT_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind RESTRICT */
#define P99__RESTRICT_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__RESTRICT_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind RESTRICT */
#define P99_RESTRICT_LEN(SEQ) P99_IF_ELSE(P99_IS_RESTRICT_TOK(SEQ))(P99__RESTRICT1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind RESTRICT */
#define P99_IS_RESTRICT_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__RESTRICT_, SEQ))
#define P99__BOOL7(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(8)(7)
#define P99__BOOL6(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__BOOL5(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__BOOL4(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__BOOL3(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__BOOL2(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__BOOL1(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__BOOL_CLASSIFY_7(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ) P99__BOOL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__BOOL_CLASSIFY_6(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ) P99__BOOL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__BOOL_CLASSIFY_5(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ) P99__BOOL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__BOOL_CLASSIFY_4(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ) P99__BOOL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__BOOL_CLASSIFY_3(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ) P99__BOOL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__BOOL_CLASSIFY_2(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ) P99__BOOL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__BOOL_CLASSIFY_1(SEQ) P99_PASTE2(P99__BOOL_CLASSIFY_, SEQ)
#define P99__BOOL_CLASSIFY___(CODE) P99_IF_ELSE(P99__BOOL_VALIDATE(CODE))(P99_PASTE2(P99__BOOL_CLASSIFY__, CODE))(INVALID_BOOL_TYPE_EXPRESSION[CODE])
#define P99__BOOL_CLASSIFY__(N, ...) P99__BOOL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__BOOL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__BOOL_CLASSIFY__(N, P99_PASTE2(P99__BOOL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind BOOL */
#define P99__BOOL_CLASSIFY(SEQ) P99__BOOL_CLASSIFY_(P99_BOOL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind BOOL */
#define P99__BOOL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__BOOL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind BOOL */
#define P99_BOOL_LEN(SEQ) P99_IF_ELSE(P99_IS_BOOL_TOK(SEQ))(P99__BOOL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind BOOL */
#define P99_IS_BOOL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__BOOL_, SEQ))
#define P99__VOID7(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(8)(7)
#define P99__VOID6(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID7(P99__EAT_FIRST(SEQ)))(6)
#define P99__VOID5(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID6(P99__EAT_FIRST(SEQ)))(5)
#define P99__VOID4(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID5(P99__EAT_FIRST(SEQ)))(4)
#define P99__VOID3(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID4(P99__EAT_FIRST(SEQ)))(3)
#define P99__VOID2(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID3(P99__EAT_FIRST(SEQ)))(2)
#define P99__VOID1(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID2(P99__EAT_FIRST(SEQ)))(1)
#define P99__VOID_CLASSIFY_7(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ) P99__VOID_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__VOID_CLASSIFY_6(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ) P99__VOID_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__VOID_CLASSIFY_5(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ) P99__VOID_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__VOID_CLASSIFY_4(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ) P99__VOID_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__VOID_CLASSIFY_3(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ) P99__VOID_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__VOID_CLASSIFY_2(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ) P99__VOID_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__VOID_CLASSIFY_1(SEQ) P99_PASTE2(P99__VOID_CLASSIFY_, SEQ)
#define P99__VOID_CLASSIFY___(CODE) P99_IF_ELSE(P99__VOID_VALIDATE(CODE))(P99_PASTE2(P99__VOID_CLASSIFY__, CODE))(INVALID_VOID_TYPE_EXPRESSION[CODE])
#define P99__VOID_CLASSIFY__(N, ...) P99__VOID_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__VOID_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__VOID_CLASSIFY__(N, P99_PASTE2(P99__VOID_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind VOID */
#define P99__VOID_CLASSIFY(SEQ) P99__VOID_CLASSIFY_(P99_VOID_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind VOID */
#define P99__VOID_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__VOID_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind VOID */
#define P99_VOID_LEN(SEQ) P99_IF_ELSE(P99_IS_VOID_TOK(SEQ))(P99__VOID1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind VOID */
#define P99_IS_VOID_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__VOID_, SEQ))
#define P99__INTEGER7(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(8)(7)
#define P99__INTEGER6(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER7(P99__EAT_FIRST(SEQ)))(6)
#define P99__INTEGER5(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER6(P99__EAT_FIRST(SEQ)))(5)
#define P99__INTEGER4(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER5(P99__EAT_FIRST(SEQ)))(4)
#define P99__INTEGER3(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER4(P99__EAT_FIRST(SEQ)))(3)
#define P99__INTEGER2(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER3(P99__EAT_FIRST(SEQ)))(2)
#define P99__INTEGER1(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER2(P99__EAT_FIRST(SEQ)))(1)
#define P99__INTEGER_CLASSIFY_7(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ) P99__INTEGER_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_CLASSIFY_6(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ) P99__INTEGER_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_CLASSIFY_5(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ) P99__INTEGER_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_CLASSIFY_4(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ) P99__INTEGER_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_CLASSIFY_3(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ) P99__INTEGER_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_CLASSIFY_2(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ) P99__INTEGER_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_CLASSIFY_1(SEQ) P99_PASTE2(P99__INTEGER_CLASSIFY_, SEQ)
#define P99__INTEGER_CLASSIFY___(CODE) P99_IF_ELSE(P99__INTEGER_VALIDATE(CODE))(P99_PASTE2(P99__INTEGER_CLASSIFY__, CODE))(INVALID_INTEGER_TYPE_EXPRESSION[CODE])
#define P99__INTEGER_CLASSIFY__(N, ...) P99__INTEGER_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__INTEGER_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__INTEGER_CLASSIFY__(N, P99_PASTE2(P99__INTEGER_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind INTEGER */
#define P99__INTEGER_CLASSIFY(SEQ) P99__INTEGER_CLASSIFY_(P99_INTEGER_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind INTEGER */
#define P99__INTEGER_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__INTEGER_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind INTEGER */
#define P99_INTEGER_LEN(SEQ) P99_IF_ELSE(P99_IS_INTEGER_TOK(SEQ))(P99__INTEGER1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind INTEGER */
#define P99_IS_INTEGER_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__INTEGER_, SEQ))
#define P99__REAL7(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(8)(7)
#define P99__REAL6(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__REAL5(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__REAL4(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__REAL3(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__REAL2(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__REAL1(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__REAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ) P99__REAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__REAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ) P99__REAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__REAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ) P99__REAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__REAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ) P99__REAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__REAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ) P99__REAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__REAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ) P99__REAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__REAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__REAL_CLASSIFY_, SEQ)
#define P99__REAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__REAL_VALIDATE(CODE))(P99_PASTE2(P99__REAL_CLASSIFY__, CODE))(INVALID_REAL_TYPE_EXPRESSION[CODE])
#define P99__REAL_CLASSIFY__(N, ...) P99__REAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__REAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__REAL_CLASSIFY__(N, P99_PASTE2(P99__REAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind REAL */
#define P99__REAL_CLASSIFY(SEQ) P99__REAL_CLASSIFY_(P99_REAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind REAL */
#define P99__REAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__REAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind REAL */
#define P99_REAL_LEN(SEQ) P99_IF_ELSE(P99_IS_REAL_TOK(SEQ))(P99__REAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind REAL */
#define P99_IS_REAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__REAL_, SEQ))
#define P99__FLOAT7(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(8)(7)
#define P99__FLOAT6(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT7(P99__EAT_FIRST(SEQ)))(6)
#define P99__FLOAT5(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT6(P99__EAT_FIRST(SEQ)))(5)
#define P99__FLOAT4(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT5(P99__EAT_FIRST(SEQ)))(4)
#define P99__FLOAT3(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT4(P99__EAT_FIRST(SEQ)))(3)
#define P99__FLOAT2(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT3(P99__EAT_FIRST(SEQ)))(2)
#define P99__FLOAT1(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT2(P99__EAT_FIRST(SEQ)))(1)
#define P99__FLOAT_CLASSIFY_7(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ) P99__FLOAT_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_CLASSIFY_6(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ) P99__FLOAT_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_CLASSIFY_5(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ) P99__FLOAT_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_CLASSIFY_4(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ) P99__FLOAT_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_CLASSIFY_3(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ) P99__FLOAT_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_CLASSIFY_2(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ) P99__FLOAT_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_CLASSIFY_1(SEQ) P99_PASTE2(P99__FLOAT_CLASSIFY_, SEQ)
#define P99__FLOAT_CLASSIFY___(CODE) P99_IF_ELSE(P99__FLOAT_VALIDATE(CODE))(P99_PASTE2(P99__FLOAT_CLASSIFY__, CODE))(INVALID_FLOAT_TYPE_EXPRESSION[CODE])
#define P99__FLOAT_CLASSIFY__(N, ...) P99__FLOAT_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__FLOAT_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__FLOAT_CLASSIFY__(N, P99_PASTE2(P99__FLOAT_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind FLOAT */
#define P99__FLOAT_CLASSIFY(SEQ) P99__FLOAT_CLASSIFY_(P99_FLOAT_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind FLOAT */
#define P99__FLOAT_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__FLOAT_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind FLOAT */
#define P99_FLOAT_LEN(SEQ) P99_IF_ELSE(P99_IS_FLOAT_TOK(SEQ))(P99__FLOAT1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind FLOAT */
#define P99_IS_FLOAT_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__FLOAT_, SEQ))
#define P99__TYPE7(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(8)(7)
#define P99__TYPE6(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE7(P99__EAT_FIRST(SEQ)))(6)
#define P99__TYPE5(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE6(P99__EAT_FIRST(SEQ)))(5)
#define P99__TYPE4(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE5(P99__EAT_FIRST(SEQ)))(4)
#define P99__TYPE3(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE4(P99__EAT_FIRST(SEQ)))(3)
#define P99__TYPE2(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE3(P99__EAT_FIRST(SEQ)))(2)
#define P99__TYPE1(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE2(P99__EAT_FIRST(SEQ)))(1)
#define P99__TYPE_CLASSIFY_7(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ) P99__TYPE_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__TYPE_CLASSIFY_6(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ) P99__TYPE_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__TYPE_CLASSIFY_5(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ) P99__TYPE_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__TYPE_CLASSIFY_4(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ) P99__TYPE_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__TYPE_CLASSIFY_3(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ) P99__TYPE_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__TYPE_CLASSIFY_2(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ) P99__TYPE_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__TYPE_CLASSIFY_1(SEQ) P99_PASTE2(P99__TYPE_CLASSIFY_, SEQ)
#define P99__TYPE_CLASSIFY___(CODE) P99_IF_ELSE(P99__TYPE_VALIDATE(CODE))(P99_PASTE2(P99__TYPE_CLASSIFY__, CODE))(INVALID_TYPE_TYPE_EXPRESSION[CODE])
#define P99__TYPE_CLASSIFY__(N, ...) P99__TYPE_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__TYPE_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__TYPE_CLASSIFY__(N, P99_PASTE2(P99__TYPE_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind TYPE */
#define P99__TYPE_CLASSIFY(SEQ) P99__TYPE_CLASSIFY_(P99_TYPE_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind TYPE */
#define P99__TYPE_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__TYPE_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind TYPE */
#define P99_TYPE_LEN(SEQ) P99_IF_ELSE(P99_IS_TYPE_TOK(SEQ))(P99__TYPE1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind TYPE */
#define P99_IS_TYPE_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__TYPE_, SEQ))
#define P99__BOOL_QUAL7(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(8)(7)
#define P99__BOOL_QUAL6(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__BOOL_QUAL5(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__BOOL_QUAL4(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__BOOL_QUAL3(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__BOOL_QUAL2(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__BOOL_QUAL1(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__BOOL_QUAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ) P99__BOOL_QUAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__BOOL_QUAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ) P99__BOOL_QUAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__BOOL_QUAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ) P99__BOOL_QUAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__BOOL_QUAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ) P99__BOOL_QUAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__BOOL_QUAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ) P99__BOOL_QUAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__BOOL_QUAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ) P99__BOOL_QUAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__BOOL_QUAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, SEQ)
#define P99__BOOL_QUAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__BOOL_QUAL_VALIDATE(CODE))(P99_PASTE2(P99__BOOL_QUAL_CLASSIFY__, CODE))(INVALID_BOOL_QUAL_TYPE_EXPRESSION[CODE])
#define P99__BOOL_QUAL_CLASSIFY__(N, ...) P99__BOOL_QUAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__BOOL_QUAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__BOOL_QUAL_CLASSIFY__(N, P99_PASTE2(P99__BOOL_QUAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind BOOL_QUAL */
#define P99__BOOL_QUAL_CLASSIFY(SEQ) P99__BOOL_QUAL_CLASSIFY_(P99_BOOL_QUAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind BOOL_QUAL */
#define P99__BOOL_QUAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__BOOL_QUAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind BOOL_QUAL */
#define P99_BOOL_QUAL_LEN(SEQ) P99_IF_ELSE(P99_IS_BOOL_QUAL_TOK(SEQ))(P99__BOOL_QUAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind BOOL_QUAL */
#define P99_IS_BOOL_QUAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__BOOL_QUAL_, SEQ))
#define P99__VOID_QUAL7(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(8)(7)
#define P99__VOID_QUAL6(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__VOID_QUAL5(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__VOID_QUAL4(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__VOID_QUAL3(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__VOID_QUAL2(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__VOID_QUAL1(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__VOID_QUAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ) P99__VOID_QUAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__VOID_QUAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ) P99__VOID_QUAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__VOID_QUAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ) P99__VOID_QUAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__VOID_QUAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ) P99__VOID_QUAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__VOID_QUAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ) P99__VOID_QUAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__VOID_QUAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ) P99__VOID_QUAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__VOID_QUAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, SEQ)
#define P99__VOID_QUAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__VOID_QUAL_VALIDATE(CODE))(P99_PASTE2(P99__VOID_QUAL_CLASSIFY__, CODE))(INVALID_VOID_QUAL_TYPE_EXPRESSION[CODE])
#define P99__VOID_QUAL_CLASSIFY__(N, ...) P99__VOID_QUAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__VOID_QUAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__VOID_QUAL_CLASSIFY__(N, P99_PASTE2(P99__VOID_QUAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind VOID_QUAL */
#define P99__VOID_QUAL_CLASSIFY(SEQ) P99__VOID_QUAL_CLASSIFY_(P99_VOID_QUAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind VOID_QUAL */
#define P99__VOID_QUAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__VOID_QUAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind VOID_QUAL */
#define P99_VOID_QUAL_LEN(SEQ) P99_IF_ELSE(P99_IS_VOID_QUAL_TOK(SEQ))(P99__VOID_QUAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind VOID_QUAL */
#define P99_IS_VOID_QUAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__VOID_QUAL_, SEQ))
#define P99__INTEGER_QUAL7(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(8)(7)
#define P99__INTEGER_QUAL6(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__INTEGER_QUAL5(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__INTEGER_QUAL4(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__INTEGER_QUAL3(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__INTEGER_QUAL2(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__INTEGER_QUAL1(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__INTEGER_QUAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ) P99__INTEGER_QUAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_QUAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ) P99__INTEGER_QUAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_QUAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ) P99__INTEGER_QUAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_QUAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ) P99__INTEGER_QUAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_QUAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ) P99__INTEGER_QUAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_QUAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ) P99__INTEGER_QUAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__INTEGER_QUAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, SEQ)
#define P99__INTEGER_QUAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__INTEGER_QUAL_VALIDATE(CODE))(P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY__, CODE))(INVALID_INTEGER_QUAL_TYPE_EXPRESSION[CODE])
#define P99__INTEGER_QUAL_CLASSIFY__(N, ...) P99__INTEGER_QUAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__INTEGER_QUAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__INTEGER_QUAL_CLASSIFY__(N, P99_PASTE2(P99__INTEGER_QUAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind INTEGER_QUAL */
#define P99__INTEGER_QUAL_CLASSIFY(SEQ) P99__INTEGER_QUAL_CLASSIFY_(P99_INTEGER_QUAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind INTEGER_QUAL */
#define P99__INTEGER_QUAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__INTEGER_QUAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind INTEGER_QUAL */
#define P99_INTEGER_QUAL_LEN(SEQ) P99_IF_ELSE(P99_IS_INTEGER_QUAL_TOK(SEQ))(P99__INTEGER_QUAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind INTEGER_QUAL */
#define P99_IS_INTEGER_QUAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__INTEGER_QUAL_, SEQ))
#define P99__REAL_QUAL7(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(8)(7)
#define P99__REAL_QUAL6(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__REAL_QUAL5(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__REAL_QUAL4(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__REAL_QUAL3(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__REAL_QUAL2(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__REAL_QUAL1(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__REAL_QUAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ) P99__REAL_QUAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__REAL_QUAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ) P99__REAL_QUAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__REAL_QUAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ) P99__REAL_QUAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__REAL_QUAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ) P99__REAL_QUAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__REAL_QUAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ) P99__REAL_QUAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__REAL_QUAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ) P99__REAL_QUAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__REAL_QUAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, SEQ)
#define P99__REAL_QUAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__REAL_QUAL_VALIDATE(CODE))(P99_PASTE2(P99__REAL_QUAL_CLASSIFY__, CODE))(INVALID_REAL_QUAL_TYPE_EXPRESSION[CODE])
#define P99__REAL_QUAL_CLASSIFY__(N, ...) P99__REAL_QUAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__REAL_QUAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__REAL_QUAL_CLASSIFY__(N, P99_PASTE2(P99__REAL_QUAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind REAL_QUAL */
#define P99__REAL_QUAL_CLASSIFY(SEQ) P99__REAL_QUAL_CLASSIFY_(P99_REAL_QUAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind REAL_QUAL */
#define P99__REAL_QUAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__REAL_QUAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind REAL_QUAL */
#define P99_REAL_QUAL_LEN(SEQ) P99_IF_ELSE(P99_IS_REAL_QUAL_TOK(SEQ))(P99__REAL_QUAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind REAL_QUAL */
#define P99_IS_REAL_QUAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__REAL_QUAL_, SEQ))
#define P99__FLOAT_QUAL7(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(8)(7)
#define P99__FLOAT_QUAL6(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__FLOAT_QUAL5(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__FLOAT_QUAL4(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__FLOAT_QUAL3(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__FLOAT_QUAL2(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__FLOAT_QUAL1(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__FLOAT_QUAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ) P99__FLOAT_QUAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_QUAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ) P99__FLOAT_QUAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_QUAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ) P99__FLOAT_QUAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_QUAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ) P99__FLOAT_QUAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_QUAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ) P99__FLOAT_QUAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_QUAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ) P99__FLOAT_QUAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__FLOAT_QUAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, SEQ)
#define P99__FLOAT_QUAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__FLOAT_QUAL_VALIDATE(CODE))(P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY__, CODE))(INVALID_FLOAT_QUAL_TYPE_EXPRESSION[CODE])
#define P99__FLOAT_QUAL_CLASSIFY__(N, ...) P99__FLOAT_QUAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__FLOAT_QUAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__FLOAT_QUAL_CLASSIFY__(N, P99_PASTE2(P99__FLOAT_QUAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind FLOAT_QUAL */
#define P99__FLOAT_QUAL_CLASSIFY(SEQ) P99__FLOAT_QUAL_CLASSIFY_(P99_FLOAT_QUAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind FLOAT_QUAL */
#define P99__FLOAT_QUAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__FLOAT_QUAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind FLOAT_QUAL */
#define P99_FLOAT_QUAL_LEN(SEQ) P99_IF_ELSE(P99_IS_FLOAT_QUAL_TOK(SEQ))(P99__FLOAT_QUAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind FLOAT_QUAL */
#define P99_IS_FLOAT_QUAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__FLOAT_QUAL_, SEQ))
#define P99__TYPE_QUAL7(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(8)(7)
#define P99__TYPE_QUAL6(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL7(P99__EAT_FIRST(SEQ)))(6)
#define P99__TYPE_QUAL5(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL6(P99__EAT_FIRST(SEQ)))(5)
#define P99__TYPE_QUAL4(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL5(P99__EAT_FIRST(SEQ)))(4)
#define P99__TYPE_QUAL3(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL4(P99__EAT_FIRST(SEQ)))(3)
#define P99__TYPE_QUAL2(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL3(P99__EAT_FIRST(SEQ)))(2)
#define P99__TYPE_QUAL1(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL2(P99__EAT_FIRST(SEQ)))(1)
#define P99__TYPE_QUAL_CLASSIFY_7(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ) P99__TYPE_QUAL_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__TYPE_QUAL_CLASSIFY_6(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ) P99__TYPE_QUAL_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__TYPE_QUAL_CLASSIFY_5(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ) P99__TYPE_QUAL_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__TYPE_QUAL_CLASSIFY_4(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ) P99__TYPE_QUAL_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__TYPE_QUAL_CLASSIFY_3(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ) P99__TYPE_QUAL_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__TYPE_QUAL_CLASSIFY_2(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ) P99__TYPE_QUAL_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__TYPE_QUAL_CLASSIFY_1(SEQ) P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, SEQ)
#define P99__TYPE_QUAL_CLASSIFY___(CODE) P99_IF_ELSE(P99__TYPE_QUAL_VALIDATE(CODE))(P99_PASTE2(P99__TYPE_QUAL_CLASSIFY__, CODE))(INVALID_TYPE_QUAL_TYPE_EXPRESSION[CODE])
#define P99__TYPE_QUAL_CLASSIFY__(N, ...) P99__TYPE_QUAL_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__TYPE_QUAL_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__TYPE_QUAL_CLASSIFY__(N, P99_PASTE2(P99__TYPE_QUAL_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind TYPE_QUAL */
#define P99__TYPE_QUAL_CLASSIFY(SEQ) P99__TYPE_QUAL_CLASSIFY_(P99_TYPE_QUAL_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind TYPE_QUAL */
#define P99__TYPE_QUAL_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__TYPE_QUAL_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind TYPE_QUAL */
#define P99_TYPE_QUAL_LEN(SEQ) P99_IF_ELSE(P99_IS_TYPE_QUAL_TOK(SEQ))(P99__TYPE_QUAL1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind TYPE_QUAL */
#define P99_IS_TYPE_QUAL_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__TYPE_QUAL_, SEQ))
#define P99__KEYWORD7(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(8)(7)
#define P99__KEYWORD6(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD7(P99__EAT_FIRST(SEQ)))(6)
#define P99__KEYWORD5(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD6(P99__EAT_FIRST(SEQ)))(5)
#define P99__KEYWORD4(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD5(P99__EAT_FIRST(SEQ)))(4)
#define P99__KEYWORD3(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD4(P99__EAT_FIRST(SEQ)))(3)
#define P99__KEYWORD2(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD3(P99__EAT_FIRST(SEQ)))(2)
#define P99__KEYWORD1(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD2(P99__EAT_FIRST(SEQ)))(1)
#define P99__KEYWORD_CLASSIFY_7(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ) P99__KEYWORD_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__KEYWORD_CLASSIFY_6(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ) P99__KEYWORD_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__KEYWORD_CLASSIFY_5(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ) P99__KEYWORD_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__KEYWORD_CLASSIFY_4(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ) P99__KEYWORD_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__KEYWORD_CLASSIFY_3(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ) P99__KEYWORD_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__KEYWORD_CLASSIFY_2(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ) P99__KEYWORD_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__KEYWORD_CLASSIFY_1(SEQ) P99_PASTE2(P99__KEYWORD_CLASSIFY_, SEQ)
#define P99__KEYWORD_CLASSIFY___(CODE) P99_IF_ELSE(P99__KEYWORD_VALIDATE(CODE))(P99_PASTE2(P99__KEYWORD_CLASSIFY__, CODE))(INVALID_KEYWORD_TYPE_EXPRESSION[CODE])
#define P99__KEYWORD_CLASSIFY__(N, ...) P99__KEYWORD_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__KEYWORD_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__KEYWORD_CLASSIFY__(N, P99_PASTE2(P99__KEYWORD_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind KEYWORD */
#define P99__KEYWORD_CLASSIFY(SEQ) P99__KEYWORD_CLASSIFY_(P99_KEYWORD_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind KEYWORD */
#define P99__KEYWORD_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__KEYWORD_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind KEYWORD */
#define P99_KEYWORD_LEN(SEQ) P99_IF_ELSE(P99_IS_KEYWORD_TOK(SEQ))(P99__KEYWORD1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind KEYWORD */
#define P99_IS_KEYWORD_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__KEYWORD_, SEQ))
#define P99__RESERVED7(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(8)(7)
#define P99__RESERVED6(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED7(P99__EAT_FIRST(SEQ)))(6)
#define P99__RESERVED5(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED6(P99__EAT_FIRST(SEQ)))(5)
#define P99__RESERVED4(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED5(P99__EAT_FIRST(SEQ)))(4)
#define P99__RESERVED3(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED4(P99__EAT_FIRST(SEQ)))(3)
#define P99__RESERVED2(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED3(P99__EAT_FIRST(SEQ)))(2)
#define P99__RESERVED1(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED2(P99__EAT_FIRST(SEQ)))(1)
#define P99__RESERVED_CLASSIFY_7(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ) P99__RESERVED_CLASSIFY_6(P99__EAT_FIRST(SEQ))
#define P99__RESERVED_CLASSIFY_6(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ) P99__RESERVED_CLASSIFY_5(P99__EAT_FIRST(SEQ))
#define P99__RESERVED_CLASSIFY_5(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ) P99__RESERVED_CLASSIFY_4(P99__EAT_FIRST(SEQ))
#define P99__RESERVED_CLASSIFY_4(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ) P99__RESERVED_CLASSIFY_3(P99__EAT_FIRST(SEQ))
#define P99__RESERVED_CLASSIFY_3(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ) P99__RESERVED_CLASSIFY_2(P99__EAT_FIRST(SEQ))
#define P99__RESERVED_CLASSIFY_2(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ) P99__RESERVED_CLASSIFY_1(P99__EAT_FIRST(SEQ))
#define P99__RESERVED_CLASSIFY_1(SEQ) P99_PASTE2(P99__RESERVED_CLASSIFY_, SEQ)
#define P99__RESERVED_CLASSIFY___(CODE) P99_IF_ELSE(P99__RESERVED_VALIDATE(CODE))(P99_PASTE2(P99__RESERVED_CLASSIFY__, CODE))(INVALID_RESERVED_TYPE_EXPRESSION[CODE])
#define P99__RESERVED_CLASSIFY__(N, ...) P99__RESERVED_CLASSIFY___(P99__NARG(__VA_ARGS__))
#define P99__RESERVED_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P99__RESERVED_CLASSIFY__(N, P99_PASTE2(P99__RESERVED_CLASSIFY_, N)(SEQ)))
/*! @brief Classify the tokens in sequence @a SEQ that all are of kind RESERVED */
#define P99__RESERVED_CLASSIFY(SEQ) P99__RESERVED_CLASSIFY_(P99_RESERVED_QUAL_LEN(SEQ), SEQ)
/*! @brief Check if @a CODE corresponds to a valid combination of tokens of kind RESERVED */
#define P99__RESERVED_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P99__RESERVED_VALIDATE__, CODE)))
/*! @brief Count the number of tokens in sequence @a SEQ that all are of kind RESERVED */
#define P99_RESERVED_LEN(SEQ) P99_IF_ELSE(P99_IS_RESERVED_TOK(SEQ))(P99__RESERVED1(P99__EAT_FIRST(SEQ)))(0)
/*! @brief Test if the token sequence @a SEQ starts with a token of kind RESERVED */
#define P99_IS_RESERVED_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P99__RESERVED_, SEQ))
#define P99__CONST_CLASSIFY_const	/* 1 */	,
#define P99__CONST_CLASSIFY__1	
#define P99__CONST_VALIDATE__1	,
#define P99__CONST_CLASSIFY__2	const
#define P99__CONST_VALIDATE__2	,
#define P99__CONST_CLASSIFY__3	const
#define P99__CONST_VALIDATE__3	,
#define P99__CONST_CLASSIFY__4	const
#define P99__CONST_VALIDATE__4	,
#define P99__CONST_CLASSIFY__5	const
#define P99__CONST_VALIDATE__5	,
#define P99__CONST_CLASSIFY__6	const
#define P99__CONST_VALIDATE__6	,
#define P99__CONST_CLASSIFY__7	const
#define P99__CONST_VALIDATE__7	,
#define P99__VOLATILE_CLASSIFY_volatile	/* 1 */	,
#define P99__VOLATILE_CLASSIFY__1	
#define P99__VOLATILE_VALIDATE__1	,
#define P99__VOLATILE_CLASSIFY__2	volatile
#define P99__VOLATILE_VALIDATE__2	,
#define P99__VOLATILE_CLASSIFY__3	volatile
#define P99__VOLATILE_VALIDATE__3	,
#define P99__VOLATILE_CLASSIFY__4	volatile
#define P99__VOLATILE_VALIDATE__4	,
#define P99__VOLATILE_CLASSIFY__5	volatile
#define P99__VOLATILE_VALIDATE__5	,
#define P99__VOLATILE_CLASSIFY__6	volatile
#define P99__VOLATILE_VALIDATE__6	,
#define P99__VOLATILE_CLASSIFY__7	volatile
#define P99__VOLATILE_VALIDATE__7	,
#define P99__VOID_CLASSIFY_void	/* 1 */	,
#define P99__VOID_CLASSIFY__1	
#define P99__VOID_VALIDATE__1	,
#define P99__VOID_CLASSIFY__2	v
#define P99__VOID_VALIDATE__2	,
#define P99__BOOL_CLASSIFY__Bool	/* 1 */	,
#define P99__BOOL_CLASSIFY__1	
#define P99__BOOL_VALIDATE__1	,
#define P99__BOOL_CLASSIFY__2	b
#define P99__BOOL_VALIDATE__2	,
#define P99__INTEGER_CLASSIFY_long	/* 1 */	,
#define P99__INTEGER_CLASSIFY_short	/* 4 */	,,,,
#define P99__INTEGER_CLASSIFY_char	/* 8 */	,,,,,,,,
#define P99__INTEGER_CLASSIFY_signed	/* 16 */	,,,,,,,,,,,,,,,,
#define P99__INTEGER_CLASSIFY_unsigned	/* 32 */	,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define P99__INTEGER_CLASSIFY_int	/* 64 */	,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define P99__INTEGER_CLASSIFY__2	l
#define P99__INTEGER_VALIDATE__2	,
#define P99__INTEGER_CLASSIFY__3	ll
#define P99__INTEGER_VALIDATE__3	,
#define P99__INTEGER_CLASSIFY__5	h
#define P99__INTEGER_VALIDATE__5	,
#define P99__INTEGER_CLASSIFY__9	c
#define P99__INTEGER_VALIDATE__9	,
#define P99__INTEGER_CLASSIFY__17	i
#define P99__INTEGER_VALIDATE__17	,
#define P99__INTEGER_CLASSIFY__18	l
#define P99__INTEGER_VALIDATE__18	,
#define P99__INTEGER_CLASSIFY__19	ll
#define P99__INTEGER_VALIDATE__19	,
#define P99__INTEGER_CLASSIFY__21	h
#define P99__INTEGER_VALIDATE__21	,
#define P99__INTEGER_CLASSIFY__25	hh
#define P99__INTEGER_VALIDATE__25	,
#define P99__INTEGER_CLASSIFY__33	u
#define P99__INTEGER_VALIDATE__33	,
#define P99__INTEGER_CLASSIFY__34	ul
#define P99__INTEGER_VALIDATE__34	,
#define P99__INTEGER_CLASSIFY__35	ull
#define P99__INTEGER_VALIDATE__35	,
#define P99__INTEGER_CLASSIFY__37	uh
#define P99__INTEGER_VALIDATE__37	,
#define P99__INTEGER_CLASSIFY__41	uhh
#define P99__INTEGER_VALIDATE__41	,
#define P99__INTEGER_CLASSIFY__65	i
#define P99__INTEGER_VALIDATE__65	,
#define P99__INTEGER_CLASSIFY__66	l
#define P99__INTEGER_VALIDATE__66	,
#define P99__INTEGER_CLASSIFY__67	ll
#define P99__INTEGER_VALIDATE__67	,
#define P99__INTEGER_CLASSIFY__69	h
#define P99__INTEGER_VALIDATE__69	,
#define P99__INTEGER_CLASSIFY__81	i
#define P99__INTEGER_VALIDATE__81	,
#define P99__INTEGER_CLASSIFY__82	l
#define P99__INTEGER_VALIDATE__82	,
#define P99__INTEGER_CLASSIFY__83	ll
#define P99__INTEGER_VALIDATE__83	,
#define P99__INTEGER_CLASSIFY__85	h
#define P99__INTEGER_VALIDATE__85	,
#define P99__INTEGER_CLASSIFY__97	u
#define P99__INTEGER_VALIDATE__97	,
#define P99__INTEGER_CLASSIFY__98	ul
#define P99__INTEGER_VALIDATE__98	,
#define P99__INTEGER_CLASSIFY__99	ull
#define P99__INTEGER_VALIDATE__99	,
#define P99__INTEGER_CLASSIFY__101	uh
#define P99__INTEGER_VALIDATE__101	,
#define P99__FLOAT_CLASSIFY_long	/* 1 */	,
#define P99__FLOAT_CLASSIFY_double	/* 4 */	,,,,
#define P99__FLOAT_CLASSIFY_float	/* 16 */	,,,,,,,,,,,,,,,,
#define P99__FLOAT_CLASSIFY__Complex	/* 32 */	,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define P99__FLOAT_CLASSIFY__5	d
#define P99__FLOAT_VALIDATE__5	,
#define P99__FLOAT_CLASSIFY__6	ld
#define P99__FLOAT_VALIDATE__6	,
#define P99__FLOAT_CLASSIFY__17	f
#define P99__FLOAT_VALIDATE__17	,
#define P99__FLOAT_CLASSIFY__37	dc
#define P99__FLOAT_VALIDATE__37	,
#define P99__FLOAT_CLASSIFY__38	ldc
#define P99__FLOAT_VALIDATE__38	,
#define P99__FLOAT_CLASSIFY__49	fc
#define P99__FLOAT_VALIDATE__49	,
#define P99__BUILTIN_TYPE_b	_Bool
#define P99__BUILTIN_TYPE_c	char
#define P99__BUILTIN_TYPE_d	double
#define P99__BUILTIN_TYPE_dc	double _Complex
#define P99__BUILTIN_TYPE_f	float
#define P99__BUILTIN_TYPE_fc	float _Complex
#define P99__BUILTIN_TYPE_h	short
#define P99__BUILTIN_TYPE_hh	signed char
#define P99__BUILTIN_TYPE_i	signed
#define P99__BUILTIN_TYPE_l	signed long
#define P99__BUILTIN_TYPE_ld	long double
#define P99__BUILTIN_TYPE_ldc	long double _Complex
#define P99__BUILTIN_TYPE_ll	signed long long
#define P99__BUILTIN_TYPE_u	unsigned
#define P99__BUILTIN_TYPE_uh	unsigned short
#define P99__BUILTIN_TYPE_uhh	unsigned char
#define P99__BUILTIN_TYPE_ul	unsigned long
#define P99__BUILTIN_TYPE_ull	unsigned long long
#define P99__BUILTIN_TYPE_v	void
/**
 ** @}
 **/
#define P99__DEC_PRED_1 0
#define P99__DEC_PRED_2 1
#define P99__DEC_PRED_3 2
#define P99__DEC_PRED_4 3
#define P99__DEC_PRED_5 4
#define P99__DEC_PRED_6 5
#define P99__DEC_PRED_7 6
#define P99__DEC_PRED_8 7
#define P99__DEC_PRED_9 8
#define P99__DEC_PRED_10 9
#define P99__DEC_PRED_11 10
#define P99__DEC_PRED_12 11
#define P99__DEC_PRED_13 12
#define P99__DEC_PRED_14 13
#define P99__DEC_PRED_15 14
#define P99__DEC_PRED_16 15
#define P99__DEC_PRED_17 16
#define P99__DEC_PRED_18 17
#define P99__DEC_PRED_19 18
#define P99__DEC_PRED_20 19
#define P99__DEC_PRED_21 20
#define P99__DEC_PRED_22 21
#define P99__DEC_PRED_23 22
#define P99__DEC_PRED_24 23
#define P99__DEC_PRED_25 24
#define P99__DEC_PRED_26 25
#define P99__DEC_PRED_27 26
#define P99__DEC_PRED_28 27
#define P99__DEC_PRED_29 28
#define P99__DEC_PRED_30 29
#define P99__DEC_PRED_31 30
#define P99__DEC_PRED_32 31
#define P99__DEC_PRED_33 32
#define P99__DEC_PRED_34 33
#define P99__DEC_PRED_35 34
#define P99__DEC_PRED_36 35
#define P99__DEC_PRED_37 36
#define P99__DEC_PRED_38 37
#define P99__DEC_PRED_39 38
#define P99__DEC_PRED_40 39
#define P99__DEC_PRED_41 40
#define P99__DEC_PRED_42 41
#define P99__DEC_PRED_43 42
#define P99__DEC_PRED_44 43
#define P99__DEC_PRED_45 44
#define P99__DEC_PRED_46 45
#define P99__DEC_PRED_47 46
#define P99__DEC_PRED_48 47
#define P99__DEC_PRED_49 48
#define P99__DEC_PRED_50 49
#define P99__DEC_PRED_51 50
#define P99__DEC_PRED_52 51
#define P99__DEC_PRED_53 52
#define P99__DEC_PRED_54 53
#define P99__DEC_PRED_55 54
#define P99__DEC_PRED_56 55
#define P99__DEC_PRED_57 56
#define P99__DEC_PRED_58 57
#define P99__DEC_PRED_59 58
#define P99__DEC_PRED_60 59
#define P99__DEC_PRED_61 60
#define P99__DEC_PRED_62 61
#define P99__DEC_PRED_63 62
#define P99__DEC_PRED_64 63
#define P99__DEC_PRED_65 64
#define P99__DEC_PRED_66 65
#define P99__DEC_PRED_67 66
#define P99__DEC_PRED_68 67
#define P99__DEC_PRED_69 68
#define P99__DEC_PRED_70 69
#define P99__DEC_PRED_71 70
#define P99__DEC_PRED_72 71
#define P99__DEC_PRED_73 72
#define P99__DEC_PRED_74 73
#define P99__DEC_PRED_75 74
#define P99__DEC_PRED_76 75
#define P99__DEC_PRED_77 76
#define P99__DEC_PRED_78 77
#define P99__DEC_PRED_79 78
#define P99__DEC_PRED_80 79
#define P99__DEC_PRED_81 80
#define P99__DEC_PRED_82 81
#define P99__DEC_PRED_83 82
#define P99__DEC_PRED_84 83
#define P99__DEC_PRED_85 84
#define P99__DEC_PRED_86 85
#define P99__DEC_PRED_87 86
#define P99__DEC_PRED_88 87
#define P99__DEC_PRED_89 88
#define P99__DEC_PRED_90 89
#define P99__DEC_PRED_91 90
#define P99__DEC_PRED_92 91
#define P99__DEC_PRED_93 92
#define P99__DEC_PRED_94 93
#define P99__DEC_PRED_95 94
#define P99__DEC_PRED_96 95
#define P99__DEC_PRED_97 96
#define P99__DEC_PRED_98 97
#define P99__DEC_PRED_99 98
#define P99__DEC_PRED_100 99
#define P99__DEC_PRED_101 100
#define P99__DEC_PRED_102 101
#define P99__DEC_PRED_103 102
#define P99__DEC_PRED_104 103
#define P99__DEC_PRED_105 104
#define P99__DEC_PRED_106 105
#define P99__DEC_PRED_107 106
#define P99__DEC_PRED_108 107
#define P99__DEC_PRED_109 108
#define P99__DEC_PRED_110 109
#define P99__DEC_PRED_111 110
#define P99__DEC_PRED_112 111
#define P99__DEC_PRED_113 112
#define P99__DEC_PRED_114 113
#define P99__DEC_PRED_115 114
#define P99__DEC_PRED_116 115
#define P99__DEC_PRED_117 116
#define P99__DEC_PRED_118 117
#define P99__DEC_PRED_119 118
#define P99__DEC_PRED_120 119
#define P99__DEC_PRED_121 120
#define P99__DEC_PRED_122 121
#define P99__DEC_PRED_minus_0 minus_1
#define P99__DEC_PRED_minus_1 minus_2
#define P99__DEC_PRED_minus_2 minus_3
#define P99__DEC_PRED_minus_3 minus_4
#define P99__DEC_PRED_minus_4 minus_5
#define P99__DEC_PRED_minus_5 minus_6
#define P99__DEC_PRED_minus_6 minus_7
#define P99__DEC_PRED_minus_7 minus_8
#define P99__DEC_PRED_minus_8 minus_9
#define P99__DEC_PRED_minus_9 minus_10
#define P99__DEC_PRED_minus_10 minus_11
#define P99__DEC_PRED_minus_11 minus_12
#define P99__DEC_PRED_minus_12 minus_13
#define P99__DEC_PRED_minus_13 minus_14
#define P99__DEC_PRED_minus_14 minus_15
#define P99__DEC_PRED_minus_15 minus_16
#define P99__DEC_PRED_minus_16 minus_17
#define P99__DEC_PRED_minus_17 minus_18
#define P99__DEC_PRED_minus_18 minus_19
#define P99__DEC_PRED_minus_19 minus_20
#define P99__DEC_PRED_minus_20 minus_21
#define P99__DEC_PRED_minus_21 minus_22
#define P99__DEC_PRED_minus_22 minus_23
#define P99__DEC_PRED_minus_23 minus_24
#define P99__DEC_PRED_minus_24 minus_25
#define P99__DEC_PRED_minus_25 minus_26
#define P99__DEC_PRED_minus_26 minus_27
#define P99__DEC_PRED_minus_27 minus_28
#define P99__DEC_PRED_minus_28 minus_29
#define P99__DEC_PRED_minus_29 minus_30
#define P99__DEC_PRED_minus_30 minus_31
#define P99__DEC_PRED_minus_31 minus_32
#define P99__DEC_PRED_minus_32 minus_33
#define P99__DEC_PRED_minus_33 minus_34
#define P99__DEC_PRED_minus_34 minus_35
#define P99__DEC_PRED_minus_35 minus_36
#define P99__DEC_PRED_minus_36 minus_37
#define P99__DEC_PRED_minus_37 minus_38
#define P99__DEC_PRED_minus_38 minus_39
#define P99__DEC_PRED_minus_39 minus_40
#define P99__DEC_PRED_minus_40 minus_41
#define P99__DEC_PRED_minus_41 minus_42
#define P99__DEC_PRED_minus_42 minus_43
#define P99__DEC_PRED_minus_43 minus_44
#define P99__DEC_PRED_minus_44 minus_45
#define P99__DEC_PRED_minus_45 minus_46
#define P99__DEC_PRED_minus_46 minus_47
#define P99__DEC_PRED_minus_47 minus_48
#define P99__DEC_PRED_minus_48 minus_49
#define P99__DEC_PRED_minus_49 minus_50
#define P99__DEC_PRED_minus_50 minus_51
#define P99__DEC_PRED_minus_51 minus_52
#define P99__DEC_PRED_minus_52 minus_53
#define P99__DEC_PRED_minus_53 minus_54
#define P99__DEC_PRED_minus_54 minus_55
#define P99__DEC_PRED_minus_55 minus_56
#define P99__DEC_PRED_minus_56 minus_57
#define P99__DEC_PRED_minus_57 minus_58
#define P99__DEC_PRED_minus_58 minus_59
#define P99__DEC_PRED_minus_59 minus_60
#define P99__DEC_PRED_minus_60 minus_61
#define P99__DEC_PRED_minus_61 minus_62
#define P99__DEC_PRED_minus_62 minus_63
#define P99__DEC_PRED_minus_63 minus_64
#define P99__DEC_PRED_minus_64 minus_65
#define P99__DEC_PRED_minus_65 minus_66
#define P99__DEC_PRED_minus_66 minus_67
#define P99__DEC_PRED_minus_67 minus_68
#define P99__DEC_PRED_minus_68 minus_69
#define P99__DEC_PRED_minus_69 minus_70
#define P99__DEC_PRED_minus_70 minus_71
#define P99__DEC_PRED_minus_71 minus_72
#define P99__DEC_PRED_minus_72 minus_73
#define P99__DEC_PRED_minus_73 minus_74
#define P99__DEC_PRED_minus_74 minus_75
#define P99__DEC_PRED_minus_75 minus_76
#define P99__DEC_PRED_minus_76 minus_77
#define P99__DEC_PRED_minus_77 minus_78
#define P99__DEC_PRED_minus_78 minus_79
#define P99__DEC_PRED_minus_79 minus_80
#define P99__DEC_PRED_minus_80 minus_81
#define P99__DEC_PRED_minus_81 minus_82
#define P99__DEC_PRED_minus_82 minus_83
#define P99__DEC_PRED_minus_83 minus_84
#define P99__DEC_PRED_minus_84 minus_85
#define P99__DEC_PRED_minus_85 minus_86
#define P99__DEC_PRED_minus_86 minus_87
#define P99__DEC_PRED_minus_87 minus_88
#define P99__DEC_PRED_minus_88 minus_89
#define P99__DEC_PRED_minus_89 minus_90
#define P99__DEC_PRED_minus_90 minus_91
#define P99__DEC_PRED_minus_91 minus_92
#define P99__DEC_PRED_minus_92 minus_93
#define P99__DEC_PRED_minus_93 minus_94
#define P99__DEC_PRED_minus_94 minus_95
#define P99__DEC_PRED_minus_95 minus_96
#define P99__DEC_PRED_minus_96 minus_97
#define P99__DEC_PRED_minus_97 minus_98
#define P99__DEC_PRED_minus_98 minus_99
#define P99__DEC_PRED_minus_99 minus_100
#define P99__DEC_PRED_minus_100 minus_101
#define P99__DEC_PRED_minus_101 minus_102
#define P99__DEC_PRED_minus_102 minus_103
#define P99__DEC_PRED_minus_103 minus_104
#define P99__DEC_PRED_minus_104 minus_105
#define P99__DEC_PRED_minus_105 minus_106
#define P99__DEC_PRED_minus_106 minus_107
#define P99__DEC_PRED_minus_107 minus_108
#define P99__DEC_PRED_minus_108 minus_109
#define P99__DEC_PRED_minus_109 minus_110
#define P99__DEC_PRED_minus_110 minus_111
#define P99__DEC_PRED_minus_111 minus_112
#define P99__DEC_PRED_minus_112 minus_113
#define P99__DEC_PRED_minus_113 minus_114
#define P99__DEC_PRED_minus_114 minus_115
#define P99__DEC_PRED_minus_115 minus_116
#define P99__DEC_PRED_minus_116 minus_117
#define P99__DEC_PRED_minus_117 minus_118
#define P99__DEC_PRED_minus_118 minus_119
#define P99__DEC_PRED_minus_119 minus_120
#define P99__DEC_PRED_minus_120 minus_121
#define P99__DEC_PRED_minus_121 minus_122
#define P99__minus_minus_0 0
#define P99__minus_minus_1 1
#define P99__minus_minus_2 2
#define P99__minus_minus_3 3
#define P99__minus_minus_4 4
#define P99__minus_minus_5 5
#define P99__minus_minus_6 6
#define P99__minus_minus_7 7
#define P99__minus_minus_8 8
#define P99__minus_minus_9 9
#define P99__minus_minus_10 10
#define P99__minus_minus_11 11
#define P99__minus_minus_12 12
#define P99__minus_minus_13 13
#define P99__minus_minus_14 14
#define P99__minus_minus_15 15
#define P99__minus_minus_16 16
#define P99__minus_minus_17 17
#define P99__minus_minus_18 18
#define P99__minus_minus_19 19
#define P99__minus_minus_20 20
#define P99__minus_minus_21 21
#define P99__minus_minus_22 22
#define P99__minus_minus_23 23
#define P99__minus_minus_24 24
#define P99__minus_minus_25 25
#define P99__minus_minus_26 26
#define P99__minus_minus_27 27
#define P99__minus_minus_28 28
#define P99__minus_minus_29 29
#define P99__minus_minus_30 30
#define P99__minus_minus_31 31
#define P99__minus_minus_32 32
#define P99__minus_minus_33 33
#define P99__minus_minus_34 34
#define P99__minus_minus_35 35
#define P99__minus_minus_36 36
#define P99__minus_minus_37 37
#define P99__minus_minus_38 38
#define P99__minus_minus_39 39
#define P99__minus_minus_40 40
#define P99__minus_minus_41 41
#define P99__minus_minus_42 42
#define P99__minus_minus_43 43
#define P99__minus_minus_44 44
#define P99__minus_minus_45 45
#define P99__minus_minus_46 46
#define P99__minus_minus_47 47
#define P99__minus_minus_48 48
#define P99__minus_minus_49 49
#define P99__minus_minus_50 50
#define P99__minus_minus_51 51
#define P99__minus_minus_52 52
#define P99__minus_minus_53 53
#define P99__minus_minus_54 54
#define P99__minus_minus_55 55
#define P99__minus_minus_56 56
#define P99__minus_minus_57 57
#define P99__minus_minus_58 58
#define P99__minus_minus_59 59
#define P99__minus_minus_60 60
#define P99__minus_minus_61 61
#define P99__minus_minus_62 62
#define P99__minus_minus_63 63
#define P99__minus_minus_64 64
#define P99__minus_minus_65 65
#define P99__minus_minus_66 66
#define P99__minus_minus_67 67
#define P99__minus_minus_68 68
#define P99__minus_minus_69 69
#define P99__minus_minus_70 70
#define P99__minus_minus_71 71
#define P99__minus_minus_72 72
#define P99__minus_minus_73 73
#define P99__minus_minus_74 74
#define P99__minus_minus_75 75
#define P99__minus_minus_76 76
#define P99__minus_minus_77 77
#define P99__minus_minus_78 78
#define P99__minus_minus_79 79
#define P99__minus_minus_80 80
#define P99__minus_minus_81 81
#define P99__minus_minus_82 82
#define P99__minus_minus_83 83
#define P99__minus_minus_84 84
#define P99__minus_minus_85 85
#define P99__minus_minus_86 86
#define P99__minus_minus_87 87
#define P99__minus_minus_88 88
#define P99__minus_minus_89 89
#define P99__minus_minus_90 90
#define P99__minus_minus_91 91
#define P99__minus_minus_92 92
#define P99__minus_minus_93 93
#define P99__minus_minus_94 94
#define P99__minus_minus_95 95
#define P99__minus_minus_96 96
#define P99__minus_minus_97 97
#define P99__minus_minus_98 98
#define P99__minus_minus_99 99
#define P99__minus_minus_100 100
#define P99__minus_minus_101 101
#define P99__minus_minus_102 102
#define P99__minus_minus_103 103
#define P99__minus_minus_104 104
#define P99__minus_minus_105 105
#define P99__minus_minus_106 106
#define P99__minus_minus_107 107
#define P99__minus_minus_108 108
#define P99__minus_minus_109 109
#define P99__minus_minus_110 110
#define P99__minus_minus_111 111
#define P99__minus_minus_112 112
#define P99__minus_minus_113 113
#define P99__minus_minus_114 114
#define P99__minus_minus_115 115
#define P99__minus_minus_116 116
#define P99__minus_minus_117 117
#define P99__minus_minus_118 118
#define P99__minus_minus_119 119
#define P99__minus_minus_120 120
#define P99__minus_minus_121 121
#define P99__IS_0_GE_0 ,
#define P99__IS_1_GE_0 ,
#define P99__IS_2_GE_0 ,
#define P99__IS_3_GE_0 ,
#define P99__IS_4_GE_0 ,
#define P99__IS_5_GE_0 ,
#define P99__IS_6_GE_0 ,
#define P99__IS_7_GE_0 ,
#define P99__IS_8_GE_0 ,
#define P99__IS_9_GE_0 ,
#define P99__IS_10_GE_0 ,
#define P99__IS_11_GE_0 ,
#define P99__IS_12_GE_0 ,
#define P99__IS_13_GE_0 ,
#define P99__IS_14_GE_0 ,
#define P99__IS_15_GE_0 ,
#define P99__IS_16_GE_0 ,
#define P99__IS_17_GE_0 ,
#define P99__IS_18_GE_0 ,
#define P99__IS_19_GE_0 ,
#define P99__IS_20_GE_0 ,
#define P99__IS_21_GE_0 ,
#define P99__IS_22_GE_0 ,
#define P99__IS_23_GE_0 ,
#define P99__IS_24_GE_0 ,
#define P99__IS_25_GE_0 ,
#define P99__IS_26_GE_0 ,
#define P99__IS_27_GE_0 ,
#define P99__IS_28_GE_0 ,
#define P99__IS_29_GE_0 ,
#define P99__IS_30_GE_0 ,
#define P99__IS_31_GE_0 ,
#define P99__IS_32_GE_0 ,
#define P99__IS_33_GE_0 ,
#define P99__IS_34_GE_0 ,
#define P99__IS_35_GE_0 ,
#define P99__IS_36_GE_0 ,
#define P99__IS_37_GE_0 ,
#define P99__IS_38_GE_0 ,
#define P99__IS_39_GE_0 ,
#define P99__IS_40_GE_0 ,
#define P99__IS_41_GE_0 ,
#define P99__IS_42_GE_0 ,
#define P99__IS_43_GE_0 ,
#define P99__IS_44_GE_0 ,
#define P99__IS_45_GE_0 ,
#define P99__IS_46_GE_0 ,
#define P99__IS_47_GE_0 ,
#define P99__IS_48_GE_0 ,
#define P99__IS_49_GE_0 ,
#define P99__IS_50_GE_0 ,
#define P99__IS_51_GE_0 ,
#define P99__IS_52_GE_0 ,
#define P99__IS_53_GE_0 ,
#define P99__IS_54_GE_0 ,
#define P99__IS_55_GE_0 ,
#define P99__IS_56_GE_0 ,
#define P99__IS_57_GE_0 ,
#define P99__IS_58_GE_0 ,
#define P99__IS_59_GE_0 ,
#define P99__IS_60_GE_0 ,
#define P99__IS_61_GE_0 ,
#define P99__IS_62_GE_0 ,
#define P99__IS_63_GE_0 ,
#define P99__IS_64_GE_0 ,
#define P99__IS_65_GE_0 ,
#define P99__IS_66_GE_0 ,
#define P99__IS_67_GE_0 ,
#define P99__IS_68_GE_0 ,
#define P99__IS_69_GE_0 ,
#define P99__IS_70_GE_0 ,
#define P99__IS_71_GE_0 ,
#define P99__IS_72_GE_0 ,
#define P99__IS_73_GE_0 ,
#define P99__IS_74_GE_0 ,
#define P99__IS_75_GE_0 ,
#define P99__IS_76_GE_0 ,
#define P99__IS_77_GE_0 ,
#define P99__IS_78_GE_0 ,
#define P99__IS_79_GE_0 ,
#define P99__IS_80_GE_0 ,
#define P99__IS_81_GE_0 ,
#define P99__IS_82_GE_0 ,
#define P99__IS_83_GE_0 ,
#define P99__IS_84_GE_0 ,
#define P99__IS_85_GE_0 ,
#define P99__IS_86_GE_0 ,
#define P99__IS_87_GE_0 ,
#define P99__IS_88_GE_0 ,
#define P99__IS_89_GE_0 ,
#define P99__IS_90_GE_0 ,
#define P99__IS_91_GE_0 ,
#define P99__IS_92_GE_0 ,
#define P99__IS_93_GE_0 ,
#define P99__IS_94_GE_0 ,
#define P99__IS_95_GE_0 ,
#define P99__IS_96_GE_0 ,
#define P99__IS_97_GE_0 ,
#define P99__IS_98_GE_0 ,
#define P99__IS_99_GE_0 ,
#define P99__IS_100_GE_0 ,
#define P99__IS_101_GE_0 ,
#define P99__IS_102_GE_0 ,
#define P99__IS_103_GE_0 ,
#define P99__IS_104_GE_0 ,
#define P99__IS_105_GE_0 ,
#define P99__IS_106_GE_0 ,
#define P99__IS_107_GE_0 ,
#define P99__IS_108_GE_0 ,
#define P99__IS_109_GE_0 ,
#define P99__IS_110_GE_0 ,
#define P99__IS_111_GE_0 ,
#define P99__IS_112_GE_0 ,
#define P99__IS_113_GE_0 ,
#define P99__IS_114_GE_0 ,
#define P99__IS_115_GE_0 ,
#define P99__IS_116_GE_0 ,
#define P99__IS_117_GE_0 ,
#define P99__IS_118_GE_0 ,
#define P99__IS_119_GE_0 ,
#define P99__IS_120_GE_0 ,
#define P99__IS_121_GE_0 ,
#define P99__dec_eval_0 0
#define P99__dec_eval_1 1
#define P99__dec_eval_2 2
#define P99__dec_eval_3 3
#define P99__dec_eval_4 4
#define P99__dec_eval_5 5
#define P99__dec_eval_6 6
#define P99__dec_eval_7 7
#define P99__dec_eval_8 8
#define P99__dec_eval_9 9
#define P99__dec_eval_10 10
#define P99__dec_eval_11 11
#define P99__dec_eval_12 12
#define P99__dec_eval_13 13
#define P99__dec_eval_14 14
#define P99__dec_eval_15 15
#define P99__dec_eval_16 16
#define P99__dec_eval_17 17
#define P99__dec_eval_18 18
#define P99__dec_eval_19 19
#define P99__dec_eval_20 20
#define P99__dec_eval_21 21
#define P99__dec_eval_22 22
#define P99__dec_eval_23 23
#define P99__dec_eval_24 24
#define P99__dec_eval_25 25
#define P99__dec_eval_26 26
#define P99__dec_eval_27 27
#define P99__dec_eval_28 28
#define P99__dec_eval_29 29
#define P99__dec_eval_30 30
#define P99__dec_eval_31 31
#define P99__dec_eval_32 32
#define P99__dec_eval_33 33
#define P99__dec_eval_34 34
#define P99__dec_eval_35 35
#define P99__dec_eval_36 36
#define P99__dec_eval_37 37
#define P99__dec_eval_38 38
#define P99__dec_eval_39 39
#define P99__dec_eval_40 40
#define P99__dec_eval_41 41
#define P99__dec_eval_42 42
#define P99__dec_eval_43 43
#define P99__dec_eval_44 44
#define P99__dec_eval_45 45
#define P99__dec_eval_46 46
#define P99__dec_eval_47 47
#define P99__dec_eval_48 48
#define P99__dec_eval_49 49
#define P99__dec_eval_50 50
#define P99__dec_eval_51 51
#define P99__dec_eval_52 52
#define P99__dec_eval_53 53
#define P99__dec_eval_54 54
#define P99__dec_eval_55 55
#define P99__dec_eval_56 56
#define P99__dec_eval_57 57
#define P99__dec_eval_58 58
#define P99__dec_eval_59 59
#define P99__dec_eval_60 60
#define P99__dec_eval_61 61
#define P99__dec_eval_62 62
#define P99__dec_eval_63 63
#define P99__dec_eval_64 64
#define P99__dec_eval_65 65
#define P99__dec_eval_66 66
#define P99__dec_eval_67 67
#define P99__dec_eval_68 68
#define P99__dec_eval_69 69
#define P99__dec_eval_70 70
#define P99__dec_eval_71 71
#define P99__dec_eval_72 72
#define P99__dec_eval_73 73
#define P99__dec_eval_74 74
#define P99__dec_eval_75 75
#define P99__dec_eval_76 76
#define P99__dec_eval_77 77
#define P99__dec_eval_78 78
#define P99__dec_eval_79 79
#define P99__dec_eval_80 80
#define P99__dec_eval_81 81
#define P99__dec_eval_82 82
#define P99__dec_eval_83 83
#define P99__dec_eval_84 84
#define P99__dec_eval_85 85
#define P99__dec_eval_86 86
#define P99__dec_eval_87 87
#define P99__dec_eval_88 88
#define P99__dec_eval_89 89
#define P99__dec_eval_90 90
#define P99__dec_eval_91 91
#define P99__dec_eval_92 92
#define P99__dec_eval_93 93
#define P99__dec_eval_94 94
#define P99__dec_eval_95 95
#define P99__dec_eval_96 96
#define P99__dec_eval_97 97
#define P99__dec_eval_98 98
#define P99__dec_eval_99 99
#define P99__dec_eval_100 100
#define P99__dec_eval_101 101
#define P99__dec_eval_102 102
#define P99__dec_eval_103 103
#define P99__dec_eval_104 104
#define P99__dec_eval_105 105
#define P99__dec_eval_106 106
#define P99__dec_eval_107 107
#define P99__dec_eval_108 108
#define P99__dec_eval_109 109
#define P99__dec_eval_110 110
#define P99__dec_eval_111 111
#define P99__dec_eval_112 112
#define P99__dec_eval_113 113
#define P99__dec_eval_114 114
#define P99__dec_eval_115 115
#define P99__dec_eval_116 116
#define P99__dec_eval_117 117
#define P99__dec_eval_118 118
#define P99__dec_eval_119 119
#define P99__dec_eval_120 120
#define P99__dec_eval_121 121
#define P99__dec_eval_minus_0 0
#define P99__dec_eval_minus_1 -1
#define P99__dec_eval_minus_2 -2
#define P99__dec_eval_minus_3 -3
#define P99__dec_eval_minus_4 -4
#define P99__dec_eval_minus_5 -5
#define P99__dec_eval_minus_6 -6
#define P99__dec_eval_minus_7 -7
#define P99__dec_eval_minus_8 -8
#define P99__dec_eval_minus_9 -9
#define P99__dec_eval_minus_10 -10
#define P99__dec_eval_minus_11 -11
#define P99__dec_eval_minus_12 -12
#define P99__dec_eval_minus_13 -13
#define P99__dec_eval_minus_14 -14
#define P99__dec_eval_minus_15 -15
#define P99__dec_eval_minus_16 -16
#define P99__dec_eval_minus_17 -17
#define P99__dec_eval_minus_18 -18
#define P99__dec_eval_minus_19 -19
#define P99__dec_eval_minus_20 -20
#define P99__dec_eval_minus_21 -21
#define P99__dec_eval_minus_22 -22
#define P99__dec_eval_minus_23 -23
#define P99__dec_eval_minus_24 -24
#define P99__dec_eval_minus_25 -25
#define P99__dec_eval_minus_26 -26
#define P99__dec_eval_minus_27 -27
#define P99__dec_eval_minus_28 -28
#define P99__dec_eval_minus_29 -29
#define P99__dec_eval_minus_30 -30
#define P99__dec_eval_minus_31 -31
#define P99__dec_eval_minus_32 -32
#define P99__dec_eval_minus_33 -33
#define P99__dec_eval_minus_34 -34
#define P99__dec_eval_minus_35 -35
#define P99__dec_eval_minus_36 -36
#define P99__dec_eval_minus_37 -37
#define P99__dec_eval_minus_38 -38
#define P99__dec_eval_minus_39 -39
#define P99__dec_eval_minus_40 -40
#define P99__dec_eval_minus_41 -41
#define P99__dec_eval_minus_42 -42
#define P99__dec_eval_minus_43 -43
#define P99__dec_eval_minus_44 -44
#define P99__dec_eval_minus_45 -45
#define P99__dec_eval_minus_46 -46
#define P99__dec_eval_minus_47 -47
#define P99__dec_eval_minus_48 -48
#define P99__dec_eval_minus_49 -49
#define P99__dec_eval_minus_50 -50
#define P99__dec_eval_minus_51 -51
#define P99__dec_eval_minus_52 -52
#define P99__dec_eval_minus_53 -53
#define P99__dec_eval_minus_54 -54
#define P99__dec_eval_minus_55 -55
#define P99__dec_eval_minus_56 -56
#define P99__dec_eval_minus_57 -57
#define P99__dec_eval_minus_58 -58
#define P99__dec_eval_minus_59 -59
#define P99__dec_eval_minus_60 -60
#define P99__dec_eval_minus_61 -61
#define P99__dec_eval_minus_62 -62
#define P99__dec_eval_minus_63 -63
#define P99__dec_eval_minus_64 -64
#define P99__dec_eval_minus_65 -65
#define P99__dec_eval_minus_66 -66
#define P99__dec_eval_minus_67 -67
#define P99__dec_eval_minus_68 -68
#define P99__dec_eval_minus_69 -69
#define P99__dec_eval_minus_70 -70
#define P99__dec_eval_minus_71 -71
#define P99__dec_eval_minus_72 -72
#define P99__dec_eval_minus_73 -73
#define P99__dec_eval_minus_74 -74
#define P99__dec_eval_minus_75 -75
#define P99__dec_eval_minus_76 -76
#define P99__dec_eval_minus_77 -77
#define P99__dec_eval_minus_78 -78
#define P99__dec_eval_minus_79 -79
#define P99__dec_eval_minus_80 -80
#define P99__dec_eval_minus_81 -81
#define P99__dec_eval_minus_82 -82
#define P99__dec_eval_minus_83 -83
#define P99__dec_eval_minus_84 -84
#define P99__dec_eval_minus_85 -85
#define P99__dec_eval_minus_86 -86
#define P99__dec_eval_minus_87 -87
#define P99__dec_eval_minus_88 -88
#define P99__dec_eval_minus_89 -89
#define P99__dec_eval_minus_90 -90
#define P99__dec_eval_minus_91 -91
#define P99__dec_eval_minus_92 -92
#define P99__dec_eval_minus_93 -93
#define P99__dec_eval_minus_94 -94
#define P99__dec_eval_minus_95 -95
#define P99__dec_eval_minus_96 -96
#define P99__dec_eval_minus_97 -97
#define P99__dec_eval_minus_98 -98
#define P99__dec_eval_minus_99 -99
#define P99__dec_eval_minus_100 -100
#define P99__dec_eval_minus_101 -101
#define P99__dec_eval_minus_102 -102
#define P99__dec_eval_minus_103 -103
#define P99__dec_eval_minus_104 -104
#define P99__dec_eval_minus_105 -105
#define P99__dec_eval_minus_106 -106
#define P99__dec_eval_minus_107 -107
#define P99__dec_eval_minus_108 -108
#define P99__dec_eval_minus_109 -109
#define P99__dec_eval_minus_110 -110
#define P99__dec_eval_minus_111 -111
#define P99__dec_eval_minus_112 -112
#define P99__dec_eval_minus_113 -113
#define P99__dec_eval_minus_114 -114
#define P99__dec_eval_minus_115 -115
#define P99__dec_eval_minus_116 -116
#define P99__dec_eval_minus_117 -117
#define P99__dec_eval_minus_118 -118
#define P99__dec_eval_minus_119 -119
#define P99__dec_eval_minus_120 -120
#define P99__dec_eval_minus_121 -121
#define P99_DUPL2(...) __VA_ARGS__, P99_DUPL1(__VA_ARGS__)
#define P99_DUPL3(...) __VA_ARGS__, P99_DUPL2(__VA_ARGS__)
#define P99_DUPL4(...) __VA_ARGS__, P99_DUPL3(__VA_ARGS__)
#define P99_DUPL5(...) __VA_ARGS__, P99_DUPL4(__VA_ARGS__)
#define P99_DUPL6(...) __VA_ARGS__, P99_DUPL5(__VA_ARGS__)
#define P99_DUPL7(...) __VA_ARGS__, P99_DUPL6(__VA_ARGS__)
#define P99_DUPL8(...) __VA_ARGS__, P99_DUPL7(__VA_ARGS__)
#define P99_DUPL9(...) __VA_ARGS__, P99_DUPL8(__VA_ARGS__)
#define P99_DUPL10(...) __VA_ARGS__, P99_DUPL9(__VA_ARGS__)
#define P99_DUPL11(...) __VA_ARGS__, P99_DUPL10(__VA_ARGS__)
#define P99_DUPL12(...) __VA_ARGS__, P99_DUPL11(__VA_ARGS__)
#define P99_DUPL13(...) __VA_ARGS__, P99_DUPL12(__VA_ARGS__)
#define P99_DUPL14(...) __VA_ARGS__, P99_DUPL13(__VA_ARGS__)
#define P99_DUPL15(...) __VA_ARGS__, P99_DUPL14(__VA_ARGS__)
#define P99_DUPL16(...) __VA_ARGS__, P99_DUPL15(__VA_ARGS__)
#define P99_DUPL17(...) __VA_ARGS__, P99_DUPL16(__VA_ARGS__)
#define P99_DUPL18(...) __VA_ARGS__, P99_DUPL17(__VA_ARGS__)
#define P99_DUPL19(...) __VA_ARGS__, P99_DUPL18(__VA_ARGS__)
#define P99_DUPL20(...) __VA_ARGS__, P99_DUPL19(__VA_ARGS__)
#define P99_DUPL21(...) __VA_ARGS__, P99_DUPL20(__VA_ARGS__)
#define P99_DUPL22(...) __VA_ARGS__, P99_DUPL21(__VA_ARGS__)
#define P99_DUPL23(...) __VA_ARGS__, P99_DUPL22(__VA_ARGS__)
#define P99_DUPL24(...) __VA_ARGS__, P99_DUPL23(__VA_ARGS__)
#define P99_DUPL25(...) __VA_ARGS__, P99_DUPL24(__VA_ARGS__)
#define P99_DUPL26(...) __VA_ARGS__, P99_DUPL25(__VA_ARGS__)
#define P99_DUPL27(...) __VA_ARGS__, P99_DUPL26(__VA_ARGS__)
#define P99_DUPL28(...) __VA_ARGS__, P99_DUPL27(__VA_ARGS__)
#define P99_DUPL29(...) __VA_ARGS__, P99_DUPL28(__VA_ARGS__)
#define P99_DUPL30(...) __VA_ARGS__, P99_DUPL29(__VA_ARGS__)
#define P99_DUPL31(...) __VA_ARGS__, P99_DUPL30(__VA_ARGS__)
#define P99_DUPL32(...) __VA_ARGS__, P99_DUPL31(__VA_ARGS__)
#define P99_DUPL33(...) __VA_ARGS__, P99_DUPL32(__VA_ARGS__)
#define P99_DUPL34(...) __VA_ARGS__, P99_DUPL33(__VA_ARGS__)
#define P99_DUPL35(...) __VA_ARGS__, P99_DUPL34(__VA_ARGS__)
#define P99_DUPL36(...) __VA_ARGS__, P99_DUPL35(__VA_ARGS__)
#define P99_DUPL37(...) __VA_ARGS__, P99_DUPL36(__VA_ARGS__)
#define P99_DUPL38(...) __VA_ARGS__, P99_DUPL37(__VA_ARGS__)
#define P99_DUPL39(...) __VA_ARGS__, P99_DUPL38(__VA_ARGS__)
#define P99_DUPL40(...) __VA_ARGS__, P99_DUPL39(__VA_ARGS__)
#define P99_DUPL41(...) __VA_ARGS__, P99_DUPL40(__VA_ARGS__)
#define P99_DUPL42(...) __VA_ARGS__, P99_DUPL41(__VA_ARGS__)
#define P99_DUPL43(...) __VA_ARGS__, P99_DUPL42(__VA_ARGS__)
#define P99_DUPL44(...) __VA_ARGS__, P99_DUPL43(__VA_ARGS__)
#define P99_DUPL45(...) __VA_ARGS__, P99_DUPL44(__VA_ARGS__)
#define P99_DUPL46(...) __VA_ARGS__, P99_DUPL45(__VA_ARGS__)
#define P99_DUPL47(...) __VA_ARGS__, P99_DUPL46(__VA_ARGS__)
#define P99_DUPL48(...) __VA_ARGS__, P99_DUPL47(__VA_ARGS__)
#define P99_DUPL49(...) __VA_ARGS__, P99_DUPL48(__VA_ARGS__)
#define P99_DUPL50(...) __VA_ARGS__, P99_DUPL49(__VA_ARGS__)
#define P99_DUPL51(...) __VA_ARGS__, P99_DUPL50(__VA_ARGS__)
#define P99_DUPL52(...) __VA_ARGS__, P99_DUPL51(__VA_ARGS__)
#define P99_DUPL53(...) __VA_ARGS__, P99_DUPL52(__VA_ARGS__)
#define P99_DUPL54(...) __VA_ARGS__, P99_DUPL53(__VA_ARGS__)
#define P99_DUPL55(...) __VA_ARGS__, P99_DUPL54(__VA_ARGS__)
#define P99_DUPL56(...) __VA_ARGS__, P99_DUPL55(__VA_ARGS__)
#define P99_DUPL57(...) __VA_ARGS__, P99_DUPL56(__VA_ARGS__)
#define P99_DUPL58(...) __VA_ARGS__, P99_DUPL57(__VA_ARGS__)
#define P99_DUPL59(...) __VA_ARGS__, P99_DUPL58(__VA_ARGS__)
#define P99_DUPL60(...) __VA_ARGS__, P99_DUPL59(__VA_ARGS__)
#define P99_DUPL61(...) __VA_ARGS__, P99_DUPL60(__VA_ARGS__)
#define P99_DUPL62(...) __VA_ARGS__, P99_DUPL61(__VA_ARGS__)
#define P99_DUPL63(...) __VA_ARGS__, P99_DUPL62(__VA_ARGS__)
#define P99_DUPL64(...) __VA_ARGS__, P99_DUPL63(__VA_ARGS__)
#define P99_DUPL65(...) __VA_ARGS__, P99_DUPL64(__VA_ARGS__)
#define P99_DUPL66(...) __VA_ARGS__, P99_DUPL65(__VA_ARGS__)
#define P99_DUPL67(...) __VA_ARGS__, P99_DUPL66(__VA_ARGS__)
#define P99_DUPL68(...) __VA_ARGS__, P99_DUPL67(__VA_ARGS__)
#define P99_DUPL69(...) __VA_ARGS__, P99_DUPL68(__VA_ARGS__)
#define P99_DUPL70(...) __VA_ARGS__, P99_DUPL69(__VA_ARGS__)
#define P99_DUPL71(...) __VA_ARGS__, P99_DUPL70(__VA_ARGS__)
#define P99_DUPL72(...) __VA_ARGS__, P99_DUPL71(__VA_ARGS__)
#define P99_DUPL73(...) __VA_ARGS__, P99_DUPL72(__VA_ARGS__)
#define P99_DUPL74(...) __VA_ARGS__, P99_DUPL73(__VA_ARGS__)
#define P99_DUPL75(...) __VA_ARGS__, P99_DUPL74(__VA_ARGS__)
#define P99_DUPL76(...) __VA_ARGS__, P99_DUPL75(__VA_ARGS__)
#define P99_DUPL77(...) __VA_ARGS__, P99_DUPL76(__VA_ARGS__)
#define P99_DUPL78(...) __VA_ARGS__, P99_DUPL77(__VA_ARGS__)
#define P99_DUPL79(...) __VA_ARGS__, P99_DUPL78(__VA_ARGS__)
#define P99_DUPL80(...) __VA_ARGS__, P99_DUPL79(__VA_ARGS__)
#define P99_DUPL81(...) __VA_ARGS__, P99_DUPL80(__VA_ARGS__)
#define P99_DUPL82(...) __VA_ARGS__, P99_DUPL81(__VA_ARGS__)
#define P99_DUPL83(...) __VA_ARGS__, P99_DUPL82(__VA_ARGS__)
#define P99_DUPL84(...) __VA_ARGS__, P99_DUPL83(__VA_ARGS__)
#define P99_DUPL85(...) __VA_ARGS__, P99_DUPL84(__VA_ARGS__)
#define P99_DUPL86(...) __VA_ARGS__, P99_DUPL85(__VA_ARGS__)
#define P99_DUPL87(...) __VA_ARGS__, P99_DUPL86(__VA_ARGS__)
#define P99_DUPL88(...) __VA_ARGS__, P99_DUPL87(__VA_ARGS__)
#define P99_DUPL89(...) __VA_ARGS__, P99_DUPL88(__VA_ARGS__)
#define P99_DUPL90(...) __VA_ARGS__, P99_DUPL89(__VA_ARGS__)
#define P99_DUPL91(...) __VA_ARGS__, P99_DUPL90(__VA_ARGS__)
#define P99_DUPL92(...) __VA_ARGS__, P99_DUPL91(__VA_ARGS__)
#define P99_DUPL93(...) __VA_ARGS__, P99_DUPL92(__VA_ARGS__)
#define P99_DUPL94(...) __VA_ARGS__, P99_DUPL93(__VA_ARGS__)
#define P99_DUPL95(...) __VA_ARGS__, P99_DUPL94(__VA_ARGS__)
#define P99_DUPL96(...) __VA_ARGS__, P99_DUPL95(__VA_ARGS__)
#define P99_DUPL97(...) __VA_ARGS__, P99_DUPL96(__VA_ARGS__)
#define P99_DUPL98(...) __VA_ARGS__, P99_DUPL97(__VA_ARGS__)
#define P99_DUPL99(...) __VA_ARGS__, P99_DUPL98(__VA_ARGS__)
#define P99_DUPL100(...) __VA_ARGS__, P99_DUPL99(__VA_ARGS__)
#define P99_DUPL101(...) __VA_ARGS__, P99_DUPL100(__VA_ARGS__)
#define P99_DUPL102(...) __VA_ARGS__, P99_DUPL101(__VA_ARGS__)
#define P99_DUPL103(...) __VA_ARGS__, P99_DUPL102(__VA_ARGS__)
#define P99_DUPL104(...) __VA_ARGS__, P99_DUPL103(__VA_ARGS__)
#define P99_DUPL105(...) __VA_ARGS__, P99_DUPL104(__VA_ARGS__)
#define P99_DUPL106(...) __VA_ARGS__, P99_DUPL105(__VA_ARGS__)
#define P99_DUPL107(...) __VA_ARGS__, P99_DUPL106(__VA_ARGS__)
#define P99_DUPL108(...) __VA_ARGS__, P99_DUPL107(__VA_ARGS__)
#define P99_DUPL109(...) __VA_ARGS__, P99_DUPL108(__VA_ARGS__)
#define P99_DUPL110(...) __VA_ARGS__, P99_DUPL109(__VA_ARGS__)
#define P99_DUPL111(...) __VA_ARGS__, P99_DUPL110(__VA_ARGS__)
#define P99_DUPL112(...) __VA_ARGS__, P99_DUPL111(__VA_ARGS__)
#define P99_DUPL113(...) __VA_ARGS__, P99_DUPL112(__VA_ARGS__)
#define P99_DUPL114(...) __VA_ARGS__, P99_DUPL113(__VA_ARGS__)
#define P99_DUPL115(...) __VA_ARGS__, P99_DUPL114(__VA_ARGS__)
#define P99_DUPL116(...) __VA_ARGS__, P99_DUPL115(__VA_ARGS__)
#define P99_DUPL117(...) __VA_ARGS__, P99_DUPL116(__VA_ARGS__)
#define P99_DUPL118(...) __VA_ARGS__, P99_DUPL117(__VA_ARGS__)
#define P99_DUPL119(...) __VA_ARGS__, P99_DUPL118(__VA_ARGS__)
#define P99_DUPL120(...) __VA_ARGS__, P99_DUPL119(__VA_ARGS__)
#define P99_DUPL121(...) __VA_ARGS__, P99_DUPL120(__VA_ARGS__)
#define P99__FOR2(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 1, P99__FOR1(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 1))
#define P99__FOR3(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 2, P99__FOR2(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 2))
#define P99__FOR4(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 3, P99__FOR3(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 3))
#define P99__FOR5(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 4, P99__FOR4(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 4))
#define P99__FOR6(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 5, P99__FOR5(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 5))
#define P99__FOR7(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 6, P99__FOR6(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 6))
#define P99__FOR8(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 7, P99__FOR7(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 7))
#define P99__FOR9(NAME, OP, FUNC, ...)                                                                     \
	OP(NAME, 8, P99__FOR8(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 8))
#define P99__FOR10(NAME, OP, FUNC, ...)                                                                    \
	OP(NAME, 9, P99__FOR9(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 9))
#define P99__FOR11(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 10, P99__FOR10(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 10))
#define P99__FOR12(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 11, P99__FOR11(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 11))
#define P99__FOR13(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 12, P99__FOR12(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 12))
#define P99__FOR14(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 13, P99__FOR13(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 13))
#define P99__FOR15(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 14, P99__FOR14(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 14))
#define P99__FOR16(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 15, P99__FOR15(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 15))
#define P99__FOR17(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 16, P99__FOR16(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 16))
#define P99__FOR18(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 17, P99__FOR17(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 17))
#define P99__FOR19(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 18, P99__FOR18(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 18))
#define P99__FOR20(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 19, P99__FOR19(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 19))
#define P99__FOR21(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 20, P99__FOR20(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 20))
#define P99__FOR22(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 21, P99__FOR21(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 21))
#define P99__FOR23(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 22, P99__FOR22(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 22))
#define P99__FOR24(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 23, P99__FOR23(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 23))
#define P99__FOR25(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 24, P99__FOR24(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 24))
#define P99__FOR26(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 25, P99__FOR25(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 25))
#define P99__FOR27(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 26, P99__FOR26(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 26))
#define P99__FOR28(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 27, P99__FOR27(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 27))
#define P99__FOR29(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 28, P99__FOR28(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 28))
#define P99__FOR30(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 29, P99__FOR29(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 29))
#define P99__FOR31(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 30, P99__FOR30(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 30))
#define P99__FOR32(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 31, P99__FOR31(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 31))
#define P99__FOR33(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 32, P99__FOR32(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 32))
#define P99__FOR34(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 33, P99__FOR33(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 33))
#define P99__FOR35(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 34, P99__FOR34(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 34))
#define P99__FOR36(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 35, P99__FOR35(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 35))
#define P99__FOR37(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 36, P99__FOR36(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 36))
#define P99__FOR38(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 37, P99__FOR37(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 37))
#define P99__FOR39(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 38, P99__FOR38(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 38))
#define P99__FOR40(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 39, P99__FOR39(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 39))
#define P99__FOR41(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 40, P99__FOR40(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 40))
#define P99__FOR42(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 41, P99__FOR41(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 41))
#define P99__FOR43(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 42, P99__FOR42(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 42))
#define P99__FOR44(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 43, P99__FOR43(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 43))
#define P99__FOR45(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 44, P99__FOR44(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 44))
#define P99__FOR46(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 45, P99__FOR45(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 45))
#define P99__FOR47(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 46, P99__FOR46(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 46))
#define P99__FOR48(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 47, P99__FOR47(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 47))
#define P99__FOR49(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 48, P99__FOR48(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 48))
#define P99__FOR50(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 49, P99__FOR49(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 49))
#define P99__FOR51(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 50, P99__FOR50(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 50))
#define P99__FOR52(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 51, P99__FOR51(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 51))
#define P99__FOR53(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 52, P99__FOR52(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 52))
#define P99__FOR54(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 53, P99__FOR53(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 53))
#define P99__FOR55(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 54, P99__FOR54(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 54))
#define P99__FOR56(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 55, P99__FOR55(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 55))
#define P99__FOR57(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 56, P99__FOR56(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 56))
#define P99__FOR58(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 57, P99__FOR57(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 57))
#define P99__FOR59(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 58, P99__FOR58(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 58))
#define P99__FOR60(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 59, P99__FOR59(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 59))
#define P99__FOR61(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 60, P99__FOR60(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 60))
#define P99__FOR62(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 61, P99__FOR61(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 61))
#define P99__FOR63(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 62, P99__FOR62(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 62))
#define P99__FOR64(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 63, P99__FOR63(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 63))
#define P99__FOR65(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 64, P99__FOR64(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 64))
#define P99__FOR66(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 65, P99__FOR65(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 65))
#define P99__FOR67(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 66, P99__FOR66(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 66))
#define P99__FOR68(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 67, P99__FOR67(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 67))
#define P99__FOR69(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 68, P99__FOR68(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 68))
#define P99__FOR70(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 69, P99__FOR69(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 69))
#define P99__FOR71(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 70, P99__FOR70(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 70))
#define P99__FOR72(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 71, P99__FOR71(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 71))
#define P99__FOR73(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 72, P99__FOR72(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 72))
#define P99__FOR74(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 73, P99__FOR73(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 73))
#define P99__FOR75(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 74, P99__FOR74(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 74))
#define P99__FOR76(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 75, P99__FOR75(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 75))
#define P99__FOR77(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 76, P99__FOR76(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 76))
#define P99__FOR78(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 77, P99__FOR77(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 77))
#define P99__FOR79(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 78, P99__FOR78(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 78))
#define P99__FOR80(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 79, P99__FOR79(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 79))
#define P99__FOR81(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 80, P99__FOR80(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 80))
#define P99__FOR82(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 81, P99__FOR81(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 81))
#define P99__FOR83(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 82, P99__FOR82(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 82))
#define P99__FOR84(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 83, P99__FOR83(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 83))
#define P99__FOR85(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 84, P99__FOR84(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 84))
#define P99__FOR86(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 85, P99__FOR85(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 85))
#define P99__FOR87(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 86, P99__FOR86(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 86))
#define P99__FOR88(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 87, P99__FOR87(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 87))
#define P99__FOR89(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 88, P99__FOR88(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 88))
#define P99__FOR90(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 89, P99__FOR89(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 89))
#define P99__FOR91(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 90, P99__FOR90(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 90))
#define P99__FOR92(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 91, P99__FOR91(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 91))
#define P99__FOR93(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 92, P99__FOR92(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 92))
#define P99__FOR94(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 93, P99__FOR93(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 93))
#define P99__FOR95(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 94, P99__FOR94(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 94))
#define P99__FOR96(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 95, P99__FOR95(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 95))
#define P99__FOR97(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 96, P99__FOR96(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 96))
#define P99__FOR98(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 97, P99__FOR97(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 97))
#define P99__FOR99(NAME, OP, FUNC, ...)                                                                       \
	OP(NAME, 98, P99__FOR98(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 98))
#define P99__FOR100(NAME, OP, FUNC, ...)                                                                      \
	OP(NAME, 99, P99__FOR99(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 99))
#define P99__FOR101(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 100, P99__FOR100(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 100))
#define P99__FOR102(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 101, P99__FOR101(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 101))
#define P99__FOR103(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 102, P99__FOR102(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 102))
#define P99__FOR104(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 103, P99__FOR103(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 103))
#define P99__FOR105(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 104, P99__FOR104(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 104))
#define P99__FOR106(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 105, P99__FOR105(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 105))
#define P99__FOR107(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 106, P99__FOR106(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 106))
#define P99__FOR108(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 107, P99__FOR107(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 107))
#define P99__FOR109(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 108, P99__FOR108(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 108))
#define P99__FOR110(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 109, P99__FOR109(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 109))
#define P99__FOR111(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 110, P99__FOR110(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 110))
#define P99__FOR112(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 111, P99__FOR111(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 111))
#define P99__FOR113(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 112, P99__FOR112(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 112))
#define P99__FOR114(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 113, P99__FOR113(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 113))
#define P99__FOR115(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 114, P99__FOR114(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 114))
#define P99__FOR116(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 115, P99__FOR115(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 115))
#define P99__FOR117(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 116, P99__FOR116(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 116))
#define P99__FOR118(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 117, P99__FOR117(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 117))
#define P99__FOR119(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 118, P99__FOR118(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 118))
#define P99__FOR120(NAME, OP, FUNC, ...)                                                                         \
	OP(NAME, 119, P99__FOR119(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), 119))

/* The preprocessor always computes with the precision of uintmax_t */
/* so for the preprocessor this is equivalent to UINITMAX_MAX       */
#define P99__UNSIGNED_MAX ~0u

#define P99__S0 0x01
#define P99__S1 0x02
#define P99__S2 0x04
#define P99__S3 0x08
#define P99__S4 0x10
#define P99__S5 0x20
#define P99__S6 0x40



/* This has to be such ugly #if/#else to ensure that the            */
/* preprocessor never sees a constant that is too large.            */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 64
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x0U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 65
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x10000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 66
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x30000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 67
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x70000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 68
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 69
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1F0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 70
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3F0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 71
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7F0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 72
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 73
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x100FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1FF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 74
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x300FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3FF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 75
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x700FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7FF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 76
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 77
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x1F00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1FFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 78
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x3F00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3FFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 79
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x7F00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7FFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 80
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 81
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0x10000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1FFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 82
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0x30000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3FFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 83
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0x70000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7FFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 84
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0xF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 85
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0x1F0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1FFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 86
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0x3F0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3FFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 87
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0x7F0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7FFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 88
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00U
#  define P99__B4 0xFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 89
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x100FF00FF00FF00FF00FF00U
#  define P99__B4 0x1FF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1FFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 90
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x300FF00FF00FF00FF00FF00U
#  define P99__B4 0x3FF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3FFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 91
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x700FF00FF00FF00FF00FF00U
#  define P99__B4 0x7FF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7FFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 92
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 93
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x1F00FF00FF00FF00FF00FF00U
#  define P99__B4 0x1FFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 94
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x3F00FF00FF00FF00FF00FF00U
#  define P99__B4 0x3FFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 95
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x7F00FF00FF00FF00FF00FF00U
#  define P99__B4 0x7FFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 96
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 97
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x100000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 98
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x300000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 99
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x700000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 100
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 101
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1F00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 102
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3F00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 103
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7F00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 104
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 105
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x100FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1FF00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 106
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x300FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3FF00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 107
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x700FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7FF00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 108
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 109
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x1F00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1FFF00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 110
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x3F00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3FFF00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 111
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x7F00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7FFF00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 112
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 113
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x10000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1FFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 114
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x30000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3FFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 115
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x70000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7FFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 116
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 117
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x1F0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1FFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 118
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x3F0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3FFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 119
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x7F0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7FFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 120
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 121
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x100FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x1FF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1FFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 122
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x300FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x3FF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3FFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 123
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x700FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x7FF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7FFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 124
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xF00FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 125
#  define P99_UINTMAX_MAX 0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x10F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x1F00FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x1FFF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x1FFFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x1FFFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 126
#  define P99_UINTMAX_MAX 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x30F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x3F00FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x3FFF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x3FFFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x3FFFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 127
#  define P99_UINTMAX_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0x2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0x4CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0x70F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0x7F00FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0x7FFF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0x7FFFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0x7FFFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */
#ifndef P99_UINTMAX_MAX
# if P99__UNSIGNED_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#  define P99_UINTMAX_WIDTH 128
#  define P99_UINTMAX_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#  define P99__B0 0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU
#  define P99__B1 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCU
#  define P99__B2 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0U
#  define P99__B3 0xFF00FF00FF00FF00FF00FF00FF00FF00U
#  define P99__B4 0xFFFF0000FFFF0000FFFF0000FFFF0000U
#  define P99__B5 0xFFFFFFFF00000000FFFFFFFF00000000U
#  define P99__B6 0xFFFFFFFFFFFFFFFF0000000000000000U
# endif /* P99__UNSIGNED_MAX */
#endif /* P99_UINTMAX_MAX */

//! @addtogroup preprocessor_macros
//! @{


#define P99__preprocessor_macros

//! @addtogroup meta_programming
//! @{


#define P99__meta_programming

//! @addtogroup preprocessor_conditionals
//! @{


#define P99__preprocessor_conditionals

//! @}

//! @addtogroup preprocessor_for
//! @{


#define P99__preprocessor_for

//! @}

//! @addtogroup preprocessor_blocks
//! @{


#define P99__preprocessor_blocks

//! @}

//! @addtogroup double_constants
//! @{


#define P99__double_constants

//! @}

//! @addtogroup classification
//! @{


#define P99__classification

//! @}

//! @}

//! @addtogroup list_processing
//! @{


#define P99__list_processing

//! @addtogroup basic_list_operations
//! @{


#define P99__basic_list_operations

//! @}

//! @addtogroup statement_lists
//! @{


#define P99__statement_lists

//! @}

//! @}

//! @addtogroup preprocessor_operators
//! @{


#define P99__preprocessor_operators

//! @addtogroup preprocessor_logic
//! @{


#define P99__preprocessor_logic

//! @}

//! @addtogroup preprocessor_arithmetic
//! @{


#define P99__preprocessor_arithmetic

//! @}

//! @addtogroup preprocessor_text
//! @{


#define P99__preprocessor_text

//! @}

//! @}

//! @addtogroup code_augmenting
//! @{


#define P99__code_augmenting

//! @addtogroup preprocessor_allocation
//! @{


#define P99__preprocessor_allocation

//! @}

//! @addtogroup preprocessor_initialization
//! @{


#define P99__preprocessor_initialization

//! @}

//! @addtogroup default_arguments
//! @{


#define P99__default_arguments

//! @}

//! @addtogroup variadic
//! @{


#define P99__variadic

//! @}

//! @}

//! @}
