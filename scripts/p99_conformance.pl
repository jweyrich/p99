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

my @compilers = (
    "tcc" => [ [],
        ["VLA", "TOKEN_CONCAT_ITERATIVE", "TOKEN_CONCAT_HASH_HASH",
            "STATIC_PARAMETER", "CONST_PARAMETER", "VOLATILE_PARAMETER", "FOR_DECLARATION",
            "PRAGMA", "PREPRO_ARITH", "UNIVERSAL",
            "DIGRAPH", "TRIGRAPH", ] ],
    "pcc" => [["-std=c99"],
        ["DIGRAPH", "TRIGRAPH",
            "CONST_PARAMETER", "VOLATILE_PARAMETER", "UNIVERSAL"]],
    "clang" => [["-trigraphs", "-Wno-trigraphs"], ["UNIVERSAL"]],
    "opencc" => [["-std=c99", "-Wno-trigraphs"], ["UNIVERSAL"]],
    "icc" => [["-std=c99"], []],
    "gcc-4.1" => [["-std=c99", "-fextended-identifiers", "-Wno-trigraphs"], []],
    "gcc-4.2" => [["-std=c99", "-fextended-identifiers", "-Wno-trigraphs"], []],
    "gcc-4.3" => [["-std=c99", "-fextended-identifiers", "-Wno-trigraphs"], []],
    "gcc-4.4" => [["-std=c99", "-fextended-identifiers", "-Wno-trigraphs"], []],
);

my $compiler = $ARGV[0];
my $version = join("-", @ARGV);
my $hfile = join("-", @ARGV, "c99-support.html");
my %compilers = @compilers;

die "compiler ${compiler} unknown" if (!defined($compilers{$compiler}));
my $dat = $compilers{$compiler};
my @dat = @{$dat};
my @defines = map { "-DSKIP_" . $_ } @{$dat[1]};
my $file = "test-p99-conformance";
my $ofile = "${file}.o";
my @args = (
    "-Wall",
    "-c",
    "-o",
    "${ofile}",
    @{$dat[0]},
    @defines,
    "${file}.c",
);

print STDERR "running ${compiler} with @args\n";

system($compiler, @args);


open(my $NM, "nm ${ofile}|") or die "could not run nm";
open(my $HTML, "> c99-conformance-${version}.html") or die "could not open html for output";

my %symbol;

foreach my $line (<$NM>) {
    chomp $line;
    if ($line =~ m/^[0-9a-fA-F]*\s+([a-zA-Z])\s+(has_.+)/) {
            #if ("$1" ne "U") {
            $symbol{$2} = "$1";
        #}
    }
}

@features = (
    "Variably modified types" => [
     "has_VLA" => "variable length array",
     "has_VLA_function" => "variable length array as function parameters",
    ],
    "Temporary variables" => [
     "has_compound_literal" => "compound literal",
    ],
    "Preprocessor" => [
     "has_concat_of_floats_1E" => "concatenation of 1E and number",
     "has_concat_of_floats_1Ep" => "concatenation of floats 1E- and number",
     "has_concat_of_floats_1Ep3Em" => "concatenation of floats 1E- 3E-",
     "has_concat_of_floats_iterative" => "iterative concatenation of floats",
     "has_concat_of_hash_hash" => "concatenation of hash hash",
     "has_expands_args_before_concatenation" => "expands args before concatenation",
     "has_determines_macro_arguments_first" => "determines macro arguments first",
    ],
    "Initializers" => [
     "has_designated_array_initializer" => "designated array initializer",
     "has_designated_struct_initializer" => "designated struct initializer",
     "has_initializer_trailing_commas" => "initializer with trailing commas",
     "has_length_from_initializer" => "length deduced from initializer",
    ],
    "Trigraphs and digraphs" => [
     "has_digraph_stringify" => "stringify with %: digraph",
     "has_punctuation_digraph" => "punctuation digraphs",
     "has_trigraph_stringfy" => "stringfy with ??= trigraph",
     "has_punctuation_trigraph" => "punctuation trigraphs",
     "has_hash_hash_interpretedCorrectly" => "hash hash interpretedCorrectly",
    ],
    "Syntax" => [
     "has_enum_trailing_commas" => "enum allows trailing comma",
     "has_flexible_array" => "flexible array",
     "has_mixed_declaration" => "mixed statements and declaration",
     "has_for_declaration" => "for with variable declaration",
     "has_func_macro" => "macro __func__ for function name",
     "has_hexdouble" => "hexdouble",
     "has_cpp_comment" => "// comment",
    ],
    "Qualifiers" => [
     "has_restrict_keyword" => "restrict keyword",
     "has_restrict_parameter" => "restrict in array function parameter",
     "has_static_parameter" => "static in array function parameter",
     "has_volatile_parameter" => "volatile in array function parameter",
     "has_const_parameter" => "const in array function parameter",
     "has_idempotent_const" => "idempotent const",
     "has_idempotent_restrict" => "idempotent restrict",
     "has_idempotent_volatile" => "idempotent volatile",
    ],
    "Data types" => [
     "has_long_double" => "long double type",
     "has_long_long" => "long long type",
     "has_ullong_max" => "unsigned long long has at least 64 bit",
     "has_preprocessor_uintmax" => "preprocessor computes at least in long long",
     "has_Bool" => "_Bool type",
    ],
    "Character set support" => [
     "has_hex_character" => "hexadecimal character as '\\x1234'",
     "has_universal_character_4" => "universal character '\\u1234'",
     "has_universal_character_8" => "universal character '\\U12345678'",
     "has_universal_string_4" => "universal string \"\\u1234\"",
     "has_universal_string_8" => "universal string \"\\U12345678\"",
     "has_καθολικός_χαρακτήρ" => "universal characters as is in linker symbol",
     "has__u03ba_u03b1_u03b8_u03bf_u03bb_u03b9_u03ba_u03cc_u03c2__u03c7_u03b1_u03c1_u03b1_u03ba_u03c4_u03ae_u03c1"
     => "universal characters mangled in linker symbol",
    ],
    "External symbols and inline" => [
     "has_mandatory_symbol1" => "generates symbol for inline with explicit extern and definition",
     "has_mandatory_symbol2" => "generates symbol for inline with explicit extern, definition and reference",
     "has_mandatory_symbol3" => "generates symbol for inline with implicit extern, and reference",
     "has_mandatory_symbol4" => "generates symbol for inline with implicit extern, definition, and reference",
    ],
    );

my %nonfeatures = (
"has_undefined_symbol1" => "generates symbol for inline definition",
"has_undefined_symbol2" => "generates symbol for inline definition and reference",
);

my %optionals = (
"has_undefined_symbol1" => "generates undefined symbol for inline definition",
"has_undefined_symbol2" => "generates undefined symbol for inline definition and reference",
);

if (defined($symbol{"has_καθολικός_χαρακτήρ"})) {
        $symbol{"has__u03ba_u03b1_u03b8_u03bf_u03bb_u03b9_u03ba_u03cc_u03c2__u03c7_u03b1_u03c1_u03b1_u03ba_u03c4_u03ae_u03c1"} = " "
        if (!defined($symbol{"has__u03ba_u03b1_u03b8_u03bf_u03bb_u03b9_u03ba_u03cc_u03c2__u03c7_u03b1_u03c1_u03b1_u03ba_u03c4_u03ae_u03c1"}));
    }
    if (defined($symbol{"has__u03ba_u03b1_u03b8_u03bf_u03bb_u03b9_u03ba_u03cc_u03c2__u03c7_u03b1_u03c1_u03b1_u03ba_u03c4_u03ae_u03c1"})) {
            $symbol{"has_καθολικός_χαρακτήρ"} = " "
            if (!defined($symbol{"has_καθολικός_χαρακτήρ"}));
        }

%features = @features;

print $HTML <<"EOF";
<html>
<header>
<title>C99 Conformance test of @ARGV</title>
<style>
html { background-color : #FFF }
tr { frame : 2px solid black;}
th { background-color : #DDD }
.missing { background-color : #E11 }
.toomuch { background-color : #E1E }
.optional { background-color : #070 }
.alternative { color : #AAA }
.command { background-color : #AAA; text-style : sans-serif }
</style>
</header>
<body>
<h1>C99 Conformance test of @ARGV</h1>
<p>
This test is part of the <a href="http://p99.gforge.inria.fr/">P99</a>
project. It has been automatically produced by "$ENV{USER}" with the commandline
</p>
<quote class="command">
${compiler} @{args}
</quote>
<p>
This conformance test is only for the preprocessor and compiler, not
the library. Also it generally only tests that check whether or not
the code compiles, not whether the produced object is correct.
</p>
<p>
There is one exception to that, we try to detect automatically how the
compiler handles <code>inline</code> functions.  Some compilers don't
generate all symbols that they should, others generate too much.
</p>
<p>
This test most likely doesn't cover all aspects of C99 that could be
compile time tested, yet.  It also probably has errors. If you find
such an error please let me know.
</p>
<table>
EOF

for (my $i = 0; $i < scalar @features; $i += 2) {
    my $group = $features[$i];
    %group = @{$features{$group}};
    @group = keys %group;
    print $HTML "<tr><td></td><th>$group</th>\n";
    foreach my $symb (sort @group) {
        if (defined($symbol{$symb})) {
                if ("$symbol{$symb}" !~ m"[U ]") {
                        print $HTML "<tr><td>+</td><td>$group{$symb}</td><tr>\n";
                        delete $symbol{"$symb"};
                    } else {
                        if ("$symbol{$symb}" eq " ") 
                            {
                                print $HTML "<tr class=\"alternative\"><td>-</td><td>$group{$symb}</td><tr>\n";
                                delete $symbol{"$symb"};
                            }
                    }
            }
        else
            {
                print $HTML "<tr class=\"missing\"><td>-</td><td>$group{$symb}</td><tr>\n";
            }
    }
}

foreach my $symb (keys %symbol) {
    if ($symbol{$symb} ne "U") {
            my $str = defined($nonfeatures{$symb}) ? $nonfeatures{$symb} : $symb;
            print $HTML "<tr class=\"toomuch\"><td>!</td><td>$str</td><tr>\n";
        } else {
            my $str = defined($optionals{$symb}) ? $optionals{$symb} : $symb;
            print $HTML "<tr class=\"optional\"><td> </td><td>$str</td><tr>\n";
        }
}

print $HTML <<'EOF';
</table>
<hr />
<table>
<tr><th colspan=2>Legend<th></tr>
<tr class="missing"><td>-</td><td>missing</td><tr>
<tr class="toomuch"><td>!</td><td>faulty</td><tr>
<tr class="alternative"><td>-</td><td>alternative</td><tr>
<tr class="optional"><td> </td><td>optional</td><tr>
</table>
</body>
</html>
EOF
