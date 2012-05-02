#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#
# Except of parts copied from previous work and as explicitly stated below,
# the author and copyright holder for this work is
# all rights reserved,  2011 Jens Gustedt, INRIA, France
#
# This file is part of the P99 project. You received this file as as
# part of a confidential agreement and you may generally not
# redistribute it and/or modify it, unless under the terms as given in
# the file LICENSE.  It is distributed without any warranty; without
# even the implied warranty of merchantability or fitness for a
# particular purpose.
#

use English;
use Getopt::Long;

my $sizeof;

my $result = GetOptions ("sizeof!"        => \${sizeof},      # flag
    );

my %name;

my $fspecial = "p99_special.c";
open(SPECIAL, ">$fspecial");

print SPECIAL << 'DOCUMENTATION';


/** @file
 ** @brief This file just contains automatically produced information for doxygen.
 **
 ** It will usually be overwritten for any new release.
 **/

/**
 ** @page safety Safety of macro programming and pitfalls
 **
 ** Often we may hear arguments that function-like macros obfuscate
 ** the code, are dangerous, not maintainable, in short: the big
 ** evil.
 **
 ** I am convinced that this is just one of the many urban
 ** legends. Macros as such are not less safe than, say, the use of
 ** pointers or of a high quality sharpened kitchen knife.  And they
 ** belong to C as much as the <code>for</code> or the <code>++</code>
 ** operator.
 **
 ** Macros can make your code easier to read and to maintain,
 ** more precise and more efficient than would be possible by writing
 ** code "directly" without macros. It is all about how you write
 ** them, if you do that yourself, and how you use them.
 **
 ** As one example look at the following code:
 ** @code
 ** if (P99_ARE_ORDERED(<, a, b, c, d, e, f)) {
 **   // all are in order, do something special
 ** } else {
 **   // handle the base case
 ** }
 ** @endcode
 **  - This is readable: you have all the variables that are to be
 **    checked by the condition at one glance.
 **  - This is maintainable: just change a variable name in one place,
 **    deleted one or add one without problems.
 **  - This is precise and describes exactly what it does.
 **  - This is efficient since its replacement is exactly the
 **    expression that is needed for that task:
 ** @code
 ** ((a) < (b)) && ((b) < (c)) && ((c) < (d)) && ((d) < (e)) && ((e) < (f))
 ** @endcode
 ** which by itself is not very readable, is difficult to maintain and is
 ** not very explicit in its intentions.
 **
 ** In the example, part of the efficiency also comes from the fact
 ** that it is <em>not a function</em>. This is interesting if, for example,
 ** some of the variables in the list are of integer type and
 ** others are of floating type: promotions from integer type to floating
 ** type will only be performed where necessary in a comparison of two
 ** adjacent variables. In such a way we can profit of the exact
 ** comparison for integer types and can avoid the problems of
 ** rounding an integer to the next representable double. (Think of
 ** comparing <code>e = UINT64_MAX - 1</code> and <code>e =
 ** UINT64_MAX</code>.)
 **
 ** But, macros have pitfalls, and <em>one</em> important pitfall
 ** in particular: you don't see from a macro call if the arguments are
 ** evaluated multiple times or not. So if you have the habit of
 ** programming with side effects, you really have to be careful.
 **
 ** The simplest solution is avoidance: don't use expressions with
 ** side effects such as <code>++</code> as arguments to macros or
 ** functions. Really, don't do that.
 **
 ** If you are programming macros, you have to be more careful since
 ** you can't assume that everybody knows what she or he is doing. For
 ** P99 we have an automatic suite of scripts that tests if any of the
 ** macros that start with "P99_" evaluate their arguments multiple
 ** times. This is a bit tricky, special care has to be taken for
 ** macros that use the ternary operator <code>?:</code> and the @c
 ** sizeof operator:
 **
 **  - For the ternary operator, P99 has many cases where the condition is
 **    constant and only one branch is evaluated for its type.
 **  - <code>sizeof</code> is also special because its argument is evaluated
 **    mostly for its type and not for its value. There is
 **    one exception to that rule, namely variable length arrays, VLA.
 **
 ** The scripts mentioned above help us detect these and other special cases
 ** and the documentation of the corresponding P99 macros is then
 ** annotated with warnings and remarks that document the special
 ** behavior of these macros.
 **
DOCUMENTATION

open(PIPE, "|c99 -E - ");

my @files = (@ARGV);

my %specials;

# use @ARGV to run over all arguments
while (<>) {
    if (m/^#[ ]*define P99_PASTE/) {
        if (! m/^#[ ]*define P99_PASTE[0-6]\(/) {
            next;
        }
    }
    if (m/^[ ]*#[ ]*define[ ]+(P99\w*)\(([^)]*)\)/) {
        my $name = $1;
        my $args = $2;
        if (defined($specials{$name})) {
            my @specials = @{$specials{$name}};
            if ($specials[0] =~ /^[0-9]+$/) {
                unshift(@specials, $args);
            } elsif ($args !~ /^\.\.\.$/) {
                print STDERR "macro $name: overwriting previous list $specials[0] by $args\n";
                $specials[0] = $args;
            } else {
                print STDERR "macro $name: not replacing $specials[0] by $args\n";
            }
            $specials{$name} = [ @specials ];
        }
        my @args = split(",", $2);
        $args = @args;
        my $va_arg = 0;
        $va_arg = 1 if (@args && $args[$args - 1] =~ m/[.][.][.]/);
        $name{$name} = $args + ($va_arg ? 2 : 0);
    } elsif (m/^[ 	]*P00_DOCUMENT_[_A-Z0-9]*_ARGUMENT\((\w*)[ 	]*,[ 	]*([0-9]*)\)/) {
        $specials{$1} = [] if (!defined($specials{$1}));
        push @{$specials{$1}}, $2;
        if (m/^[ 	]*P00_DOCUMENT_NUMBER_ARGUMENT\((\w*)[ 	]*,[ 	]*([0-9]*)\)/) {
            print PIPE "#define $1_ARG_$2 2\n";
        }
    }
}

my @specials = sort keys %specials;

foreach my $key (@specials) {
    my @args = @{$specials{$key}};
    my $args = shift @args;
    my @names = split("[ ,]+", $args);
    if ($names[$#names] =~ /\.\.\./) {
        pop @names;
        push(@names, "__VA_ARG__[0]", "__VA_ARG__[1]", "__VA_ARG__[2]", "__VA_ARG__[3]", "__VA_ARG__[4]", "__VA_ARG__[5]");
    }
    my %args;
    map { $args{$_} = 1; } @args;
    @args = sort keys %args;
    my @list;
    foreach $arg (@args) {
        if (defined($names[$arg])) {
            push(@list, $names[$arg]);
        } else {
            push(@list, $arg);
        }
    }
    print SPECIAL " ** \@see $key($args) for argument",
    $#list > 1 ? "s " : " ";
    print SPECIAL join(", ",
                      map { "<code>$_</code>" }
                      @list);
    print SPECIAL ".\n";
}

print SPECIAL " **/\n";

print PIPE << 'HEADER0';
#define P00_DOCUMENT_DECLARATION_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N declaration
#define P00_DOCUMENT_MACRO_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N macro
#define P00_DOCUMENT_MULTIPLE_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N multiple
#define P00_DOCUMENT_STATEMENT_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N statement
#define P00_DOCUMENT_IDENTIFIER_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N identifier
#define P00_DOCUMENT_DESIGNATOR_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N designator
#define P00_DOCUMENT_TYPE_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N type
#define P00_DOCUMENT_TYPE_IDENTIFIER_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N type
#define P00_DOCUMENT_PERMITTED_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N permitted
HEADER0

    if (!defined($sizeof)) {
        print PIPE << 'HEADER1';
#define P00_DOCUMENT_WARN_VLA_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N VLA
HEADER1
}

foreach my $file (@files) {
    print PIPE "#include \"${file}\"\n";
}


foreach my $name (sort(keys(%name))) {
    my $str = "$name: $name(";
    for (my $i = 0; $i < $name{$name}; ++$i) {
        $str .= "${name}_ARG_${i}, ";
    }
    $str =~ s/, $//;
    print PIPE "$str)\n";
}
