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

print "#define HAS_COMMA(...) P99__ARG(__VA_ARGS__,\\\n";
for (my $i = 2; $i < $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print "\t1,";
    } else {
        print "\\\n\t1,";
    }
}
print "\t0, ...)\n";


for (my $arg = 0; $arg < $maxnumber; ++$arg) {
    print "#define P99__SKP${arg}(";
    for (my $i = 0; $i < $arg; ++$i) {
        if ($i % 8 != 1) {
            print "\t_$i,";
        } else {
            print "\\\n\t_$i,";
        }
    }
    print "\\\n\t...) __VA_ARGS__\n";
}

for (my $arg = 1; $arg < $maxnumber; ++$arg) {
    print "#define P99__PRE${arg}(";
    for (my $i = 0; $i < $arg; ++$i) {
        if ($i % 8 != 1) {
            print "\t_$i,";
        } else {
            print "\\\n\t_$i,";
        }
    }
    print "\\\n\t...) _0";
    for (my $i = 1; $i < $arg; ++$i) {
        if ($i % 8 != 1) {
            print ",\t_$i";
        } else {
            print ",\\\n\t_$i";
        }
    }
    print "\n";
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

for(my $mod = 1; $mod < $maxnumber; ++$mod) {
    print "#define P99__MOD${mod}() ";
    for (my $i = 0; $i < $maxnumber; ++$i) {
        if ($i % ${mod} != 0) {
            printf "\t%d,", $i % ${mod};
        } else {
            printf "\\\n\t%d,", $i % ${mod};
        }
    }
    print STDOUT "\t", $maxnumber % ${mod}, "\n";
}

for(my $div = 1; $div < $maxnumber; ++$div) {
    print "#define P99__DIV${div}() ";
    for (my $i = 0; $i < $maxnumber; ++$i) {
        if ($i % ${div} != 0) {
            printf "\t%d,", $i / ${div};
        } else {
            printf "\\\n\t%d,", $i / ${div};
        }
    }
    print STDOUT "\t", int($maxnumber / ${div}), "\n";
}


{
    my $li = "_1,\t_2";
    for (my $m = 3; $m < $maxnumber; ++$m) {
        my $m1 = $m - 1;
        print "#define PASTE$m(${li},\t_${m})\t\\\n\tPASTE2(PASTE${m1}(${li}), _${m})\n";
        $li .= ",\t_${m}";
    }
}

print "#define P99__IS_${_}_EQ_${_}(...) ,\n"
    foreach (0.. $maxnumber, @keywords_C99);
print "#define IS_EQ_${_}(...) TOK_EQ(${_}, __VA_ARGS__)\n"
    foreach (0.. $maxnumber, @keywords_C99);

printf "#define P99__uni2dec_%s %d\n", ${digit}x$_, $_
    foreach (0.. $maxnumber);
printf "#define P99__dec2uni_%d %s\n", $_, ${digit}x$_
    foreach (0.. $maxnumber);
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
print "#define REP${_}(X) ", "X ## " x ($_ - 1), "X\n"
    foreach (2 .. $maxnumber);


for (my $i = 2; $i < $maxnumber; ++$i) {
    my $i1 = $i - 1;
    print "#define P99__FOR${i}(NAME, OP, FUNC, ...) \\\n",
    "\tOP(NAME, $i1, P99__FOR${i1}(NAME, OP, FUNC, ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, LAST(__VA_ARGS__), $i1))\n";
}
