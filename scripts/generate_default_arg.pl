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


print "#define _ASCENDING() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t$i,";
    } else {
        print "\\\n\t$i,";
    }
}
print STDOUT "\t", $maxnumber, "\n";



{
    my $li = "_1,\t_2";
    for (my $m = 3; $m < 20; ++$m) {
        my $m1 = $m - 1;
        print "#define PASTE$m(${li},\t_${m})\t\\\n\t_PASTE2(PASTE${m1}(${li}), _${m})\n";
        $li .= ",\t_${m}";
    }
}


for (my $i = 0; $i < 0x10 && $i < $maxnumber; ++$i) {
    printf "#define _hex2dec_0x%X %d\n", $i, $i;
}

for (my $i = 0; $i < 0x100 && $i < $maxnumber; ++$i) {
    printf "#define _hex2dec_0x%02X %d\n", $i, $i;
}

for (my $i = 0; $i < 0x1000 && $i < $maxnumber; ++$i) {
    printf "#define _hex2dec_0x%03X %d\n", $i, $i;
}

for (my $i = 0; $i < 0x1000 && $i < $maxnumber; ++$i) {
    printf "#define _dec2hex_%d %03X\n", $i, $i;
}

for (my $i = 0; $i < $maxnumber; ++$i) {
    printf "#define _uni2dec_%s %d\n", ${digit}x$i, $i;
    printf "#define _dec2uni_%d %s\n", $i, ${digit}x$i;
    printf "#define _predecessor_%d %d\n", $i + 1, $i;
    printf "#define _itpredecessor_%d(DEC) _predecessor(_itpredecessor_%d(DEC))\n", $i + 1, $i;
    printf "#define _predecessor_minus_%d minus_%d\n", $i, $i + 1;
    printf "#define _minus_minus_%d %d\n", $i, $i;
    printf "#define _IS_%d_GE_0 ,\n", $i;
    printf "#define _dec_eval_%d %d\n", $i, $i;
    printf "#define _dec_eval_minus_%d %d\n", $i, -$i;
}

for (my $i = 1; $i < $maxnumber; ++$i) {
    my $i0 = $i - 1;
    print "#define __wda_${i}(NAME, N, ...) _wda_${i0}(NAME, __VA_ARGS__, PASTE3(NAME, _defarg_, N)())\n";
    print "#define _wda_${i}(NAME, ...) __wda_${i}(NAME, _NARG(__VA_ARGS__), __VA_ARGS__)\n";
    print "#define _wda_minus_${i}(NAME, ...) __VA_ARGS__\n";
}

for (my $i = 2; $i < $maxnumber; ++$i) {
    print "#define __DOIT${i}(NAME, N, OP, FUNC, A, ...) OP(NAME, FUNC(NAME, A, N), N, PASTE(_DOIT, N)(NAME, N, OP, FUNC, __VA_ARGS__, ))\n";
    print "#define _DOIT${i}(NAME, N, OP, FUNC, A, ...) __DOIT${i}(NAME, _predecessor(N), OP, FUNC, A, __VA_ARGS__)\n";
}
