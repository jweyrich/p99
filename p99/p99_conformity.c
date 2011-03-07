/*
** p99_conformity.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar  6 13:52:57 2011 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#ifndef SKIP_VA_ARGS_MACRO
#define FIRST(X, ...) X
#if FIRST(0, something)
# error "The preprocessor lacks variable argument list support. Run test with -DSKIP_VA_ARGS_MACRO to be able to see the other tests."
#endif
#endif

#ifndef SKIP_DIGRAPH

/* check for the four "language" digraphs */
double digraph <::> = <% 0 %>;

%: define HAS_HASH_DIGRAPH 1
enum { has_hash_digraph = !!HAS_HASH_DIGRAPH };
%: define DIGRAPH_STRINGIFY(X) %:X
char const digraph_string[] = DIGRAPH_STRINGIFY(digraph);
#if !defined(HAS_HASH_DIGRAPH)
# error "The preprocessor lacks digraph %: support. Run test with -DSKIP_DIGRAPH to be able to see the other tests."
#endif

%: define HAS_HASH_HASH_DIGRAPH_(a, b) a %:%: b
%: define HAS_HASH_HASH_DIGRAPH HAS_HASH_HASH_DIGRAPH_(0, 1)
enum { has_hash_hash_digraph = !!HAS_HASH_HASH_DIGRAPH};
/* This one here and the one in the trigraph section should be merged
   into one symbol. */
double hash_hash_interpretedCorrectly[has_hash_hash_digraph];
#if !defined(HAS_HASH_HASH_DIGRAPH) || (HAS_HASH_HASH_DIGRAPH != 1)
# error "The preprocessor lacks quadgraph %:%: support. Run test with -DSKIP_DIGRAPH to be able to see the other tests."
#endif

#endif


#ifndef SKIP_TRIGRAPH

/* check for the eight "language" trigraphs */
int trigraph ??(??) = ??< (0 ??' 1), (0 ??! 1), ??-0,  '??/0' ??>;
/* don't get confused by the syntax highlighting the ??' aint't too bad for that */

??= define HAS_HASH_TRIGRAPH 1
enum { has_hash_trigraph = !!HAS_HASH_TRIGRAPH };
??= define TRIGRAPH_STRINGIFY(X) ??=X
char const trigraph_string[] = TRIGRAPH_STRINGIFY(trigraph);
#if !defined(HAS_HASH_TRIGRAPH)
# error "The preprocessor lacks trigraph ??= support. Run test with -DSKIP_TRIGRAPH to be able to see the other tests."
#endif

??= define HAS_HASH_HASH_TRIGRAPH_(a, b) a ??=??= b
??= define HAS_HASH_HASH_TRIGRAPH HAS_HASH_HASH_TRIGRAPH_(0, 1)
enum { has_hash_hash_trigraph = !!HAS_HASH_HASH_TRIGRAPH};
/* This one here and the one in the digraph section should be merged
   into one symbol. */
double hash_hash_interpretedCorrectly[has_hash_hash_trigraph];
#if !defined(HAS_HASH_HASH_TRIGRAPH) || (HAS_HASH_HASH_TRIGRAPH != 1)
# error "The preprocessor lacks hexgraph ??=??= support. Run test with -DSKIP_TRIGRAPH to be able to see the other tests."
#endif

#endif

#ifndef SKIP_BOOL
enum { hasTrue = (_Bool)127, hasFalse = (_Bool)0 };
_Bool has_Bool[1];
_Bool has_Bool[hasTrue] = { hasFalse };
#endif

#ifndef SKIP_VLA
double VLA_function(unsigned n, double A[n][n]) {
  double ret = 0.0;
  unsigned i, j;
  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      ret += A[i][j];
  return ret;
}
_Bool has_VLA(unsigned n) {
  double VLA[n][n];
  return VLA_function(n, VLA) >= 0.0;
}


#endif

#ifndef SKIP_INLINE
typedef void (*func)(void);

/* This alone should not result in a generation of a symbol */
/* undefined_symbol but leave an undefined reference to such a function. */
inline
void
undefined_symbol(void) {
  /* empty */
}
func undefined_symbol_tester[] = { undefined_symbol };

/* This should result in a generation of a symbol mandatory_symbol1 */
inline
void
mandatory_symbol1(void) {
  /* empty */
}
extern inline
void
mandatory_symbol1(void);

/* This should result in a generation of a symbol mandatory_symbol2 */
inline
void
mandatory_symbol2(void) {
  /* empty */
}
func mandatory_symbol2_tester[] = { mandatory_symbol2 };
extern inline
void
mandatory_symbol2(void);

/* This should result in a generation of a symbol mandatory_symbol3 */
inline
void
mandatory_symbol3(void) {
  /* empty */
}
void
mandatory_symbol3(void);

/* This should result in a generation of a symbol mandatory_symbol4 */
inline
void
mandatory_symbol4(void) {
  /* empty */
}
func mandatory_symbol4_tester[] = { mandatory_symbol4 };
extern inline
void
mandatory_symbol4(void);

#endif

#ifndef SKIP_HEXDOUBLE
enum { has_hexdouble = (0x1P2 > 10) };
double hasHexdouble[has_hexdouble];
#endif

#ifndef SKIP_COMPOUND
unsigned has_compound_literal(void) {
  return (unsigned){ 0 };
}
#endif

#ifndef SKIP_INITIALIZERS
unsigned A0[4] = { [3] = 1u };
unsigned A1[] = { [3] = 1u };
unsigned gets_length_from_initializer[sizeof(A0) == sizeof(A1)];
struct {
  unsigned first;
  double second;
} Struct = { .second = 1, .first = 2 };
#endif

#ifndef SKIP_EXPANDS
#define GLUE2(A, B) A ## B
#define GLUE2_(A, B) GLUE2(A, B)
#define GLUE4(A, B, C, D) GLUE2_(GLUE2(A, B),GLUE2(C, D));
unsigned thousand = GLUE4(1, 0, 0, 0);

/* expand arguments after assigment, but before call */
#define COMMA ,
#define GLUE1(X) GLUE2(X)
unsigned determines_macro_arguments_first[GLUE1(0 COMMA 1)];

/* Expand args before ## concatenation */
#define EATEAT 0
#define EAT 1
enum { GLUE2_(eat, GLUE2(EAT, EAT)) = GLUE2(EAT, EAT),
       GLUE2_(eat, GLUE2_(EAT, EAT)) = GLUE2_(EAT, EAT)};
unsigned expands_args_before_concatenation[eat11] = { eat0 };
#endif

#ifndef SKIP_TRAILING_COMMA
enum { enumConstant, };
unsigned allows_trailing_commas[] = { enumConstant, };
#endif

#ifndef SKIP_FLEXIBLE
typedef struct {
  unsigned len;
  double arr[];
} flexible;
typedef union {
  flexible flex;
  char buffer[sizeof(flexible) + 10*sizeof(double)];
} flex10;
flex10 flexArray = { .flex.len = 10 };
#endif

#ifndef SKIP_STATIC_PARAMETER
void has_static_parameter(double A[static 10]){
 /* empty */
}
void has_const_parameter(double A[const 10]){
 /* empty */
}
void has_volatile_parameter(double A[volatile 10]){
 /* empty */
}
void has_restrict_parameter(double A[restrict 10]){
 /* empty */
}
#endif

#ifndef SKIP_COMMENTS
enum { has_cpp_comment = 2
       //* all of this is a comment to the end of the line */ 2
       - 1
};
unsigned allows_cpp_comment[has_cpp_comment];
#endif

#ifndef SKIP_MIXED
unsigned allows_arbitrary_declaration(void) {
  /* infinite recursion, but who cares */
  allows_arbitrary_declaration();
  unsigned a = 10;
  return a;
}
#endif

#ifndef SKIP_FOR_DECLARATION
unsigned allows_for_declaration(void) {
  unsigned a = 10;
  for (unsigned i = 0; i < a; ++i) {
    a -= i;
  }
  switch (0)
    for (unsigned var; 0;)
    default:
      switch((var = 0), 0)
      default: {
        return var;
      }
  return a;
}
#endif

#ifndef SKIP_IDEM
unsigned const const has_idempotent_qualifiers = 1;
#endif

#ifndef SKIP_PRAGMA
#pragma STDC FP_CONTRACT ON
#define PRAGMA(MESS) _Pragma(# MESS)
PRAGMA(STDC FP_CONTRACT OFF)
#endif

#ifndef SKIP_FUNC
void has_func_macro(char const** where) {
  *where = __func__;
 }
#endif

#ifndef SKIP_LONG_LONG
long long has_long_long = 1;
#endif

#ifndef SKIP_UNIVERSAL
int has_extended_character = L'\x0401';
int has_universal_character = L'\u0401';
int has_Universal_character = L'\U00000401';
int has_\u0401 = 1;
#endif
