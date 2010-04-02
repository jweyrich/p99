#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

my $maxnumber = 64;
my $digit = "1";

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
    printf "#define _dec_eval_%d %d\n", $i, $i;
    printf "#define _dec_eval_minus_%d %d\n", $i, -$i;
}

for (my $i = 1; $i < $maxnumber; ++$i) {
    my $i0 = $i - 1;
    print "#define ___wda_${i}(NAME, N, ...) _wda_${i0}(NAME, __VA_ARGS__, NAME ## _defarg_ ## N())\n";
    print "#define __wda_${i}(NAME, N, ...) ___wda_${i}(NAME, N, __VA_ARGS__)\n";
    print "#define _wda_${i}(NAME, ...) __wda_${i}(NAME, _predecessor(_NARG_64(x, ## __VA_ARGS__)), __VA_ARGS__)\n";
    print "#define _wda_minus_${i}(NAME, ...) __VA_ARGS__\n";
}
