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
 ** @section introduction Macros and inline functions working together
 **
 ** In C, functions (whether @c inline or not) and macros fulfill
 ** different purposes. Their difference should not be seen as
 ** ideological as some seem to take it, and what is even more
 ** important, they may work nicely together.
 **
 ** Macros are text replacement that is done at compile time and they
 ** can do things like ::P99_SIGNED(EXPR) which is defined by P99. That
 ** macro takes an expression as an argument, and tells at compile time
 ** of whether or not the integral type of @c EXPR is signed or
 ** not. Additionally it guarantees that @c EXPR itself is not
 ** evaluated at run time (so there are no side effects), but that
 ** only its type is taken into consideration.
 **
 ** Such an example shows that macros are ideally used when the type
 ** of an expression is to be determined and you want to act
 ** accordingly to that type. On the other hand, the pitfall with
 ** macros is that their arguments may be evaluated several times,
 ** which is bad because of side effects.
 **
 ** Functions on the other hand are typed, which makes them more
 ** strict or, phrased negatively, less flexible. Consider the
 ** function
 **
 ** @code
 ** inline
 ** uintmax_t p99__abs_signed(intmax_t a) {
 **  return (a < 0) ? -(uintmax_t)a : (uintmax_t)a;
 ** }
 ** @endcode
 **
 ** It takes signed integer value @c a and computes its absolute
 ** value. Observe that the return type of this function is
 ** unsigned. This has to be so, since otherwise not all valid values
 ** could be necessarily be realized.
 **
 ** ::p99__abs_signed would not be a good candidate for a macro, since
 ** @c a is evaluated twice; once in the controlling expression and
 ** once for returning its value or its negation.
 **
 ** We may use this function with any integral type, but then the
 ** argument will be promoted to @c intmax_t. In case that the value
 ** @c A that is passed to the call is positive and greater than @c
 ** INTMAX_MAX = 2<sup>N-1</sup> -1, the result is probably not what
 ** we'd expect. (Here @c N is the width of @c uintmax_t). If the
 ** conversion doesn't result in the run time system throwing a range
 ** error (it would be permitted to do so), the argument @a a would
 ** receive the negative value @c -C where @c C is @c 2<sup>N</sup> -
 ** A.  The result of the function call would then be @c C and not @c
 ** A.
 **
 ** With the following macro we get rid of these restrictions by
 ** combining the macro and the function:
 **
 ** @code
 ** #define P99_ABS(EXPR) (P99_SIGNED(EXPR) ? p99__abs_signed(EXPR) : (EXPR))
 ** @endcode
 **
 ** This has the following properties
 **
 ** <ul>
 **     <li>For any integral type it returns the correct result.</li>
 **     <li>The argument @c EXPR is evaluated exactly once.</li>
 **     <li>Any recent and decent compiler will create
 **         @ref inline "optimal code" for that combined macro.
 **     </li>
 ** </ul>
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
 **/

/**
 ** @page conventions Programming conventions
 **
 **  - @ref prefixes
 **  - @ref variableInit
 **  - @ref temporaries
 **
 ** @section prefixes Defining identifiers
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
 ** @section variableInit Variable initialization
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
 ** @section temporaries Use of temporary lvalues
 **
 ** Often when programming utilities for C that are supposed to return
 ** a pointer to an array or structure the question arises who is
 ** allocating the space, the caller or the callee.
 **
 ** P99 goes a different way, in between, in that tries to take away
 ** that burden mostly from the programmer of both, caller and callee.
 ** Let us look at the hypothetical function
 ** @code
 ** char const* hostname(char buffer[], size_t len);
 ** @endcode
 **
 ** which could be defined as being similar to the POSIX @c
 ** gethostname function, only that it doesn't return an error
 ** indicator but a pointer to the name or @c NULL if it fails. An old
 ** time (and dangerous!) calling convention for such a function would
 ** perhaps have been to return a statically allocated buffer in case
 ** that the @c buffer argument is @a NULL.
 **
 ** P99 lets you define more convenient and less dangerous calling
 ** conventions:
 ** @ref defaults
 ** allows us to define a
 ** @ref hide "macro of the same name"
 ** that uses a
 ** @ref compound "compound litteral"
 ** if no argument is given to the same function.
 **
 ** @code
 ** #define hostname(...) P99_CALL_DEFARG(hostname, 2, __VA_ARGS__)
 ** #define hostname_defarg_0() (char[HOSTNAME_MAX]){ 0 }
 ** #define hostname_defarg_1() HOST_NAME_MAX
 ** @endcode
 **
 ** This defines three different macros. One that is used in the place
 ** the programmer places a call to @c hostname. The other two, @c
 ** hostname_defarg_0 and @c hostname_defarg_1, are used by the macro
 ** @c hostname when the respective arguments are left out.
 **
 ** Now @c hostname can be used in three different ways.
 ** <ol>
 ** <li>Such that
 ** the caller is responsible and obtains space on the heap:
 ** @code
 ** char const*const host = hostname(malloc(mylen), mylen);
 ** .
 ** free(host);
 ** @endcode
 ** </li>
 ** <li>Such that the caller initializes its own variable that has a
 ** storage class that fits best to the needs:
 ** @code
 ** char host[mylen];
 ** .
 ** hostname(host, mylen);
 ** @endcode
 ** </li>
 ** <li>
 ** Or such that the space is allocated on the stack of the current
 ** scope of the call:
 ** @code
 ** char const*const host = hostname();
 ** @endcode
 ** </li>
 ** </ol>
 ** @endcode
 **
 ** The later is then equivalent to
 ** @code
 ** char tmp[HOSTNAME_MAX] = { 0 };
 ** char const*const host = hostname(tmp, HOSTNAME_MAX);
 ** @endcode
 ** but without leaving a non-const access to the contents of @c tmp.
 **
 **
 ** It uses a temporary value that is only valid inside the block in
 ** which the @c get_hostname macro is expanded. The handling of this
 ** temporary is implicit, neither the caller nor the callee have to
 ** worry of allocating or deallocating it.  On the calling side this
 ** convention is simple to use without having the callee expose a
 ** static buffer.
 **/

/**
 ** @page utilities Implemented utilities
 **
 ** @section defaults Default arguments to functions
 **
 ** In section @ref temporaries we have seen a way to provide default
 ** arguments to functions by overloading them with macros. The
 ** general declaration pattern here is as follows
 **
 ** @code
 ** #define NAME(...) P99_CALL_DEFARG(NAME, N, __VA_ARGS__)
 ** @endcode
 **
 ** Where @c NAME becomes the name of a macro and where we also
 ** suppose that there is already a function of the same name @c NAME.
 **
 ** Other than this would be in C++, the default value for the
 ** ::hostname macro that we had given was itself produced by a macro,
 ** namely @c hostname_defarg_0. The convention here is as simple as that:
 **  - when called, ::P99_CALL_DEFARG replaces each argument M
 **    (counting starts at 0) that is not
 **    provided by the tokens
 **    @code
 **    NAME ## _defarg_ ## M ()
 **    @endcode
 **    that is a concatenation of @c NAME with the token @c _defarg_
 **    and the decimal number @c M
 **  - "not provided" here meaning either
 **     - leaving an empty place in an argument list
 **     - giving less arguments than @c N
 **  - to be valid C code this name must then either
 **      -# itself be a macro that is then expanded
 **      -# be a valid function call that can be interpreted by the
 **        compiler
 **
 ** As we have seen in the example (a) is computed in the context of
 ** the caller. This let us simply use a temporary (here a local
 ** compound literal) that was thus valid in that context.
 **
 ** To obtain the same behavior as for C++, namely to provide a
 ** default argument that is evaluated at the place of declaration and
 ** not at the place of the call we have to use (b), a function call.
 ** This will be as efficient as macro call if we use @ref inline for
 ** that purpose.
 **
 ** To ease the programming of this functional approach P99 provides
 ** some machinery. We need three things as in the following example:
 ** @code
 ** P99_PROTOTYPE(rand48_t *, rand48_t_init, rand48_t*, unsigned short, unsigned short, unsigned short);
 ** #define rand48_t_init(...) P99_CALL_DEFARG(rand48_t_init, 4, __VA_ARGS__)
 ** P99_DECLARE_DEFARG(rand48_t_init,
 **                    ,
 **                    useconds(),
 **                    getpid(),
 **                    (atomic_counter_inc(&rand48_counter), atomic_counter_getvalue(&rand48_counter))
 **                    );
 ** @endcode
 **
 ** That are
 **  - a "prototype" of the underlying function, such that P99
 **    knows of the name of the function, the return type and the types
 **    of the arguments.
 **  - the macro definition as we have already seen
 **  - a declaration of the default arguments.
 **
 ** Here in the example there is no default argument for position 0
 ** but one for positions 1 to 3. All three have the type @c
 ** unsigned. The above leads to the automatic generation of three @c
 ** inline function that looks something like:
 **
 ** @code
 ** inline
 ** pthread_mutexattr_t const*
 ** pthread_mutex_init_defarg_1(void) {
 **   return useconds();
 ** }
 ** inline
 ** pthread_mutexattr_t const*
 ** pthread_mutex_init_defarg_2(void) {
 **   return getpid();
 ** }
 ** inline
 ** pthread_mutexattr_t const*
 ** pthread_mutex_init_defarg_3(void) {
 **   return (atomic_counter_inc(&rand48_counter), atomic_counter_getvalue(&rand48_counter));
 ** }
 ** @endcode
 **
 ** This declaration and definition is placed in the context of the
 ** above declaration and not in the context of the caller. Thus
 ** the expression is evaluated in that context, and not in the
 ** context of the caller. In particular for the third function, this
 ** fixes the variable @c rand48_counter to the one that is visible at
 ** the point of declaration.
 **
 ** @section blocks Guarded blocks
 ** @section condi Preprocessor conditionals and loops
 ** @section alloc Allocation and initialization facilities
 **/

/**
 ** @page programming Macro programming with P99
 **/

/**
 ** @page c99 C99 features
 ** @section variadic Variadic macros
 ** @section inline Inline functions
 ** @section initializers Named initializers
 ** @section compound Compound literals
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
 ** that is initialized with all @c `a', a terminating @c '\0' and
 ** that is a valid objet until the program leaves the current block.
 ** @code
 ** char const*const hui = memset((char[256]){0}, 'a', 255);
 ** @endcode
 **
 ** It would be equivalent to the following
 ** @code
 ** char tmp[256] = { 0 };
 ** char const*const hui = memset(tmp, 'a', 255);
 ** @endcode
 **
 ** Using the compound literal here has the advantage that no other
 ** non-const reference to the temporary is exposed.
 **
 ** @section hide Macros that hide a function
 **
 ** Per se, this is not a new feature of C99 but had been present
 ** before. The preprocessor has two special rules, one that applies
 ** generally to macros and the other that applies on to functional macros:
 **
 **  -# If during expansion of a macro XXX the token XXX is found, it
 **    is not expanded. So there is no recursion in C macros.
 **  -# If a functional macro YYY is found without a following
 **    opening parenthesis it is not expanded.
 **
 ** Theses features can be used to define a macro and another
 ** identifier that have the same name. It is sometimes used to all
 ** for a test if some functionality is present on a platform. E.g on
 ** my computer I have
 **
 ** @code
 ** #define stdin stdin
 ** @endcode
 **
 ** This can be used as follows
 ** @code
 ** #ifdef stdin
 **   // Do something for an hosted environment
 **   // Use stdin as usual
 ** #else
 **   // Do something for an embedded environment
 **   // We don't have stdin at all, write to a log file or so.
 ** #endif
 ** @endcode
 **
 ** But we may equally use this technique for a function symbol. POSIX
 ** explicitly allows this for example for the functions in @em stdio.h
 **
 ** <center>
 ** <em>
 ** The following shall be declared as functions and may also be
 ** defined as macros.<br />
 ** Function prototypes shall be provided.
 ** </em>
 ** </center>
 **
 ** Lets have a look at some random function from there and suppose it
 ** would be given as follows:
 ** @code
 ** int putc(int, FILE *);
 ** #define putc(C, F) (is_it_special(C) ? do_something_clever(C, F) : putc(C, F) )
 ** @endcode
 **
 ** (Yes this evaluates @c C two times.)  With that, these uses of @c
 ** putc are still valid:
 ** @code
 ** // Use the macro and implicitly the function, relies on rule 1
 ** putc('A', stdout);
 **
 ** // Just use the function not the macro, relies on rule 2
 ** (putc)('A', stdout);
 **
 ** // Get the address of putc and store it in my_putc, relies on rule 2
 ** int (*my_putc)(int, FILE*) = &putc;
 ** @endcode
 **
 ** The example above with @c putc has a particular pitfall if we have
 ** the above definitions in a header file and then include this file
 ** at the place where we define the the function:
 **
 ** @code
 ** #include <stdio.h>
 **
 ** int putc(int, FILE *) {
 **   // do the right thing here
 ** }
 ** @endcode
 **
 ** This will simply explode since the preprocessor will expand the @i
 ** functional reference to @c putc. This can be explicitly avoided
 ** by undefining the macro before the definition, but for this the
 ** implementor of @c putc has to know that it is also a macro.
 **
 ** With P99, we use this technique to @em overload a function to
 ** provide it with @ref defaults. A macro defined in that way will
 ** avoid this pitfall: if it is called with the same number of
 ** arguments (or more) that all are non-empty, it will produce the
 ** same token sequence as if the macro had not been defined.
 **/



#include "p99_block.h"
#include "p99_choice.h"
#include "p99_defarg.h"
#include "p99_int.h"
#include "p99_new.h"
#include "p99_double.h"

#endif 	    /* !P99_H_ */
