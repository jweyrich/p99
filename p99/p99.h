/**
 ** p99.h
 **
 ** Made by Jens Gustedt
 ** Login   <gustedt@damogran.loria.fr>
 **
 ** Started on  Mon Aug 23 15:59:41 2010 Jens Gustedt
 ** Last update Mon Aug 23 15:59:41 2010 Jens Gustedt
 **/

#ifndef   	P99_H_
# define   	P99_H_

/**
 ** @file
 ** @brief Use this to include all of P99.
 **/

/**
 ** @mainpage P99 - Preprocessor macros and functions for c99
 **
 ** In C, functions (whether @c inline or not) and macros fulfill
 ** different purposes. Their difference should not be seen as
 ** ideological as some seem to take it, and what is even more
 ** important, they may work nicely together.
 **
 ** Macros are text replacement that is done at compile time and they
 ** can do things like ::P99_ISSIGNED(T) which is defined by P99. That
 ** macro takes a type as an argument, and gives you a compile time
 ** expression of whether or not the integral type @c T is signed or
 ** not. That is, macros are ideally used when the type of an
 ** expression is not known (at the definition) and you want to do
 ** something about it. On the other hand, the pitfall with macros is
 ** that their arguments may be evaluated several times, which is bad
 ** because of side effects.
 **
 ** Functions on the other hand are typed, which makes them more
 ** strict or, phrased negatively, less flexible. Consider the
 ** functions
 **
 ** @code
 ** inline uintmax_t absU(uintmax_t a) { return a; }
 ** inline uintmax_t absS(uintmax_t a) {
 **    return (-a < a) ? -a : a;
 ** }
 ** @endcode
 **
 ** The first implements the trivial @c abs function for an unsigned
 ** integral type. The second implements it for a signed type. (Yes,
 ** it takes an unsigned as argument, this is for purpose.)
 **
 ** We may use these two functions with any integral type. But, the
 ** return type will always be of the largest width and there is a
 ** certain difficulty on knowing how do choose between the two.
 **
 ** Now with the following macro
 **
 ** @code
 ** #define ABS(T, A) ((T)(P99_ISSIGNED(T) ? absS : absU)(A))
 ** @endcode
 **
 ** we have implemented a
 **
 ** <ul>
 **     <li>family of functions</li>
 **     <li>that works for any integral type</li>
 **     <li>that evaluates its argument only once</li>
 **     <li>for which any recent and decent compiler will create optimal code</li>
 ** </ul>
 **
 ** Well, admittedly doing this with @c abs is a bit artificial, but I
 ** hope you get the picture.
 **
 ** In that spirit, P99 aims to provide utilities that often combine
 ** macros and @c inline funcions and that are only possible with the
 ** @ref c99 "features"
 ** that come with C99 (read ANSI C as normalized in 1999)
 ** and that where absent in C89. The features include among others
 **  - @ref variadic
 **  - @ref inline
 **  - @ref initializers
 **  - @ref compound
 **  - @ref hide
 **
 ** With these it implements @ref utilities "utilities" that
 ** previously had not been possible to program in C (or C macros) or
 ** that were very difficult to implement
 **  - @ref defaults
 **  - @ref blocks
 **  - @ref condi
 **  - @ref alloc
 **
 ** P99 also provides numerous facilities for
 ** @ref programming "macro programming."
 **
 ** P99 is not a C library in the classical sense but merely a
 ** collection of include files:
 **  - There is no binary library that is to be linked to your
 **    executable. The few functions that are provided are small
 **    wrappers that are compiled directly into your code.
 **  - There is nothing to configure, P99 include files should work
 **    out of the box with any conforming C99 compiler.
 **
 **
 ** @section credits Credits and Rights
 ** @subsection author Author and Maintainer
 ** @author <a href="http://www.loria.fr/~gustedt/">Jens Gustedt</a>
 ** @date 2010
 **
 ** @subsection copyright Copyright
 ** Copyright &copy; 2010 Jens Gustedt, INRIA, France, http://www.inria.fr/
 **
 ** Permission to use, copy, modify, and distribute this software and
 ** its documentation under the terms as provided in the file LICENSE
 ** is hereby granted. No representations are made about the
 ** suitability of this software for any purpose. It is provided "as
 ** is" without express or implied warranty.
 **
 ** @section conventions Programming conventions
 **
 **  - @ref prefixes
 **  - @ref variableInit
 **  - @ref temporaries
 **
 ** @subsection prefixes Defining identifiers
 **
 ** Macro names that implement a functionality of P99 are generally
 ** uppercase. Exceptions from that rule are @ref hide.
 ** All other identifiers are lowercase.
 **
 ** P99 uses the common prefixes @c P99_ and @c p99_ for macros and
 ** other identifiers, respectively. If you include any of the P99
 ** files future version could use any new identifier with one of
 ** those prefixes, so don't define some of them your own.
 **
 ** The extended prefixes @c P99__ and @c p99__ (with two underscores)
 ** are used for identifiers that are only auxiliary and that need not
 ** be documented by themselves. Such identifiers are ignored in the
 ** doxygen documentation.
 **
 **
 ** @subsection variableInit Variable initialization
 **
 ** Where possible, P99 uses initializers to initialize variables. For
 ** each type @c T where such an initialization is possible, there
 ** should be a macro @c T_INITIALIZER that does a standard
 ** initialization. Such a macro should use the @ref initializers
 ** scheme.
 **
 ** @code
 ** typedef struct toto toto;
 ** struct toto { double a; unsigned b; };
 ** #define TOTO_INITIALIZER { .a = 0.0; .b = 0u }
 ** @endcode
 **
 ** Such initializers can be easily assembled together
 ** @code
 ** typedef struct tutu tutu;
 ** struct tutu { toto A; bool c; };
 ** #define TUTU_INITIALIZER(VAL) { .A = TOTO_INITIALIZER, .c = (VAL) }
 ** @endcode
 **
 ** As you can see in this example, INITIALIZER can be a `normal'
 ** macro or a function like macro.
 **
 ** For dynamic initialization we assume that an `init' function
 ** exists that takes a pointer as a first argument and returns
 ** exactly the same pointer
 ** @code
 ** tutu* tutu_init(tutu* t, bool val) {
 **   t->A = (toto)TOTO_INITIALIZER;
 **   t->c = val;
 **   return t;
 ** }
 ** @endcode
 **
 ** @subsection temporaries Use of temporary lvalues
 **
 ** Often when programming utilities for C that are supposed to return
 ** a pointer to an array or structure the question arises who is
 ** allocating the space, the caller or the callee.
 **
 ** P99 goes a different way, in between, in that tries to take away
 ** that burden mostly from the programmer of both, caller and callee.
 ** Let us look at the hypothetical function
 ** @code
 ** char const* get_hostname(char* buffer);
 ** @endcode
 **
 ** @ref defaults
 ** allows us to define a
 ** @ref hide "macro of the same name"
 ** that uses a
 ** @ref compound "compound litteral"
 ** if no argument is given to the same function.
 **
 ** @code
 ** P99_PROTOTYPE(char const*, get_hostname, char *);
 ** P99_DEFARG_DOCU(get_hostname)
 ** #define get_hostname(...) P99_CALL_DEFARG(get_hostname, 1, __VA_ARGS__)
 ** P99_DECLARE_DEFARG(get_hostname, (char[HOSTNAME_MAX]){ 0 });
 ** @endcode
 **
 ** Now @c get_hostname can be used in three different ways.
 ** <ol>
 ** <li>Such that
 ** the caller is responsible and obtains space on the heap:
 ** @code
 ** char const * host = get_hostname(malloc(HOSTNAME_MAX));
 ** @endcode
 ** </li>
 ** <li>Such that the caller initializes its own variable that has a
 ** storage class that fits best to the needs:
 ** @code
 ** char const host[HOSTNAME_MAX];
 ** .
 ** get_hostname(host);
 ** @endcode
 ** </li>
 ** <li>
 ** Or such that the space is allocated on the stack of the current
 ** scope of the call:
 ** @code
 ** char const * host = get_hostname();
 ** @endcode
 ** </li>
 ** </ol>
 ** The later has the advantage of being simple to use, but without
 ** the callee exposing a static buffer.
 **
 ** @section utilities Implemented utilities
 ** @subsection defaults Default arguments to functions
 ** @subsection blocks Guarded blocks
 ** @subsection condi Preprocessor conditionals and loops
 ** @subsection alloc Allocation and initialization facilities
 **
 ** @section programming Macro programming with P99
 **
 ** @section c99 C99 features
 ** @subsection variadic Variadic macros
 ** @subsection inline Inline functions
 ** @subsection initializers Named initializers
 ** @subsection compound Compound literals
 **
 ** A compound literal is syntactically given as a compound
 ** initializer and a cast such as
 ** @code
 ** (int[2]){ 4, 5}
 ** (T){ .d = 1, .a = 10 }.
 ** @endcode
 **
 ** It is best seen to define a temporary object of the requested
 ** type, initialized with the same rules as would a named variable of
 ** that type.
 **
 **  - Such unnamed temporary objects can be initialized on the fly, e.g
 **     as the arguments to functions, and they are alive to the end of
 **     the block in which they are defined.
 **  - They define an lvalue from which an address can be taken.
 **  - Unless the type of the cast is defined with `const' the content
 **     of such a variable is modifiable.
 **
 ** Example: The following returns the pointer to a character array
 ** that is initialized with all @c `a' and that is a valid objet until
 ** the program leaves the current block.
 ** @code
 ** char* hui = memset(&(char[256]){0}, 'a', 256)
 ** @endcode
 **
 ** It would be equivalent to the following
 ** @code
 ** char tmp[256] = { 0 };
 ** char* hui = memset(tmp, 'a', 256)
 ** @endcode
 **
 ** @subsection hide Macros that hide a function
 **/



#include "p99_block.h"
#include "p99_choice.h"
#include "p99_defarg.h"
#include "p99_int.h"
#include "p99_new.h"
#include "p99_double.h"

#endif 	    /* !P99_H_ */
