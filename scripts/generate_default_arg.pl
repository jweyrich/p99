#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

my $maxnumber = 64;
my $digit = "1";


print "/* This file is automat";
print "ically generated, do not chan";
print "ge manually. */\n";

print "#define _ARG(";
for (my $i = 1; $i <= $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print "\t_$i,";
    } else {
        print "\\\n\t_$i,";
    }
}
print "\\\n\t...) _$maxnumber\n";

for (my $m = 1; $m < 5; ++$m) {
    print "#define _NARG_$m(...) _ARG(__VA_ARGS__, ";
    for (my $i = ($maxnumber - 1); $i >= 0; --$i) {
        my $val = ($i % $m) ? "_INV($m)" : ($i / $m);
        if ($i % 8 != 7) {
            print "\t$val,";
        } else {
            print "\\\n\t$val,";
        }
    }
    print ")\n";
}

for (my $arg = 0; $arg < $maxnumber; ++$arg) {
    print "#define _CHS${arg}(";
    for (my $i = 0; $i <= $arg; ++$i) {
        if ($i % 8 != 1) {
            print "\t_$i,";
        } else {
            print "\\\n\t_$i,";
        }
    }
    print "\\\n\t...) _$arg\n";
}

print "#define _ASCENDING() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t$i,";
    } else {
        print "\\\n\t$i,";
    }
}
print STDOUT "\t", $maxnumber, "\n";

for(my $mod = 1; $mod < $maxnumber; ++$mod) {
    print "#define _MOD${mod}() ";
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
    print "#define _DIV${div}() ";
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
        print "#define PASTE$m(${li},\t_${m})\t\\\n\t_PASTE2(PASTE${m1}(${li}), _${m})\n";
        $li .= ",\t_${m}";
    }
}

printf "#define _uni2dec_%s %d\n", ${digit}x$_, $_
    foreach (0.. $maxnumber);
printf "#define _dec2uni_%d %s\n", $_, ${digit}x$_
    foreach (0.. $maxnumber);
printf "#define _predecessor_%d %d\n", $_ + 1, $_
    foreach (0.. $maxnumber);
printf "#define _itpredecessor_%d(DEC) _predecessor(_itpredecessor_%d(DEC))\n", $_ + 1, $_
    foreach (0.. $maxnumber);
printf "#define _predecessor_minus_%d minus_%d\n", $_, $_ + 1
    foreach (0.. $maxnumber);
printf "#define _minus_minus_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define _IS_%d_GE_0 ,\n", $_
    foreach (0.. $maxnumber);
printf "#define _dec_eval_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define _dec_eval_minus_%d %d\n", $_, -$_
    foreach (0.. $maxnumber);
print "#define REP${_}(X) ", "X ## " x ($_ - 1), "X\n"
    foreach (2 .. $maxnumber);


for (my $i = 2; $i < $maxnumber; ++$i) {
    my $i1 = $i - 1;
    print "#define _DOIT${i}(NAME, OP, FUNC, A, ...) \\\n",
    "\tOP(NAME, FUNC(NAME, A, $i1), $i1, _DOIT${i1}(NAME, OP, FUNC, __VA_ARGS__, ))\n";
}
