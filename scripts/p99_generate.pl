#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

my $maxnumber = 64;
my $digit = "1";

# these are the normative keywords in C99
my @keywords_C99
    = qw( _Bool _Complex _Imaginary auto break case char
         const continue default do double else enum extern float for goto if
         inline int long register restrict return short signed sizeof static
         struct switch typedef union unsigned void volatile while );

print "/* This file is automat";
print "ically generated, do not chan";
print "ge manually. */\n";

print "#define P99__ARG(";
for (my $i = 1; $i <= $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print "\t_$i,";
    } else {
        print "\\\n\t_$i,";
    }
}
print "\\\n\t...) _$maxnumber\n";

for (my $m = 1; $m < 5; ++$m) {
    print "#define P99__NARG_$m(...) P99__ARG(__VA_ARGS__, ";
    for (my $i = ($maxnumber - 1); $i >= 0; --$i) {
        my $val = ($i % $m) ? "P99__INV($m)" : ($i / $m);
        if ($i % 8 != 7) {
            print "\t$val,";
        } else {
            print "\\\n\t$val,";
        }
    }
    print ")\n";
}

print "/*! \@brief Determine of the argument list has a comma, i.e at least two arguments.*/\n";
print "#define HAS_COMMA(...) P99__ARG(__VA_ARGS__,\\\n";
for (my $i = 2; $i < $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print "\t1,";
    } else {
        print "\\\n\t1,";
    }
}
print "\t0, ...)\n";


for (my $arg = 2; $arg < $maxnumber; ++$arg) {
    my $arg1 = ${arg} - 1;
    print "#define P99__SKP${arg}(_0, ...) P99__SKP${arg1}(__VA_ARGS__)\n";
}

for (my $arg = 2; $arg < $maxnumber; ++$arg) {
    my $arg1 = ${arg} - 1;
    print "#define P99__PRE${arg}(_0, ...) _0, P99__PRE${arg1}(__VA_ARGS__)\n";
}

print "#define P99__ASCENDING() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t$i,";
    } else {
        print "\\\n\t$i,";
    }
}
print STDOUT "\n";

print "#define P99__ALL_ZEROES() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t0,";
    } else {
        print "\\\n\t0,";
    }
}
print STDOUT "\n";

print "#define P99__ALL_ONES() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t1,";
    } else {
        print "\\\n\t1,";
    }
}
print STDOUT "\n";

for (my $m = 7; $m < $maxnumber; ++$m) {
    my $m1 = $m - 1;
    print "/*! \@brief Paste $m arguments at their boundary.*/\n";
    print "#define P99_PASTE$m(...) P99__PASTE$m(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))\n";
    print "#define P99__PASTE$m(L, ...) P99_PASTE2(P99_PASTE${m1}(__VA_ARGS__), L)\n";
}

print "#define P99__IS_${_}_EQ_${_}(...) ,\n"
    foreach (0.. $maxnumber, @keywords_C99);
print "/*! \@brief Test if the argument consists of exactly the token \@c ${_} */\
#define P99_IS_EQ_${_}(...) P99_TOK_EQ(${_}, __VA_ARGS__)\n"
    foreach (0.. $maxnumber, @keywords_C99);

printf "#define P99__DEC_PRED_%d %d\n", $_ + 1, $_
    foreach (0.. $maxnumber);
printf "#define P99__DEC_PRED_minus_%d minus_%d\n", $_, $_ + 1
    foreach (0.. $maxnumber);
printf "#define P99__minus_minus_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define P99__IS_%d_GE_0 ,\n", $_
    foreach (0.. $maxnumber);
printf "#define P99__dec_eval_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define P99__dec_eval_minus_%d %d\n", $_, -$_
    foreach (0.. $maxnumber);
print "#define DUPL${_}(...) __VA_ARGS__, DUPL", ($_ - 1), "(__VA_ARGS__)\n"
    foreach (2 .. $maxnumber);


for (my $i = 2; $i < $maxnumber; ++$i) {
    my $i1 = $i - 1;
    print "#define P99__FOR${i}(NAME, OP, FUNC, ...) \\\n",
    "\tOP(NAME, $i1, P99__FOR${i1}(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), $i1))\n";
}
