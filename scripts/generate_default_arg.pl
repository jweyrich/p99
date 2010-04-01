#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

my $maxnumber = 64;
my $maxargs = 32;

for (my $i = 0; $i < 0x10 && $i < $maxnumber; ++$i) {
    printf "#define _decimal_0x%X %d\n", $i, $i;
}

for (my $i = 0; $i < 0x100 && $i < $maxnumber; ++$i) {
    printf "#define _decimal_0x%02X %d\n", $i, $i;
}

for (my $i = 0; $i < 0x1000 && $i < $maxnumber; ++$i) {
    printf "#define _decimal_0x%03X %d\n", $i, $i;
}

for (my $i = 0; $i < 0x1000 && $i < $maxnumber; ++$i) {
    printf "#define _hexadecimal_%d %03X\n", $i, $i;
}

for (my $i = 0; $i < $maxnumber; ++$i) {
    printf "#define _predecessor_%d %d\n", $i + 1, $i;
}

for (my $i = 1; $i < $maxargs; ++$i) {
    print "#define _call_with_${i}_0(NAME, _ign) _call_with_${i}_1(NAME, NAME ## _defarg_ ## 0())\n";
    for (my $j = 1; $j < $i; ++$j) {
        my $j1 = $j + 1;
        print "#define _call_with_${i}_${j}(NAME, ...) _call_with_${i}_${j1}(NAME, __VA_ARGS__, NAME ## _defarg_ ## ${j}())\n";
    }
    print "#define _call_with_${i}_${i}(NAME, ...) (__VA_ARGS__)\n";
}
