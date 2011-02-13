#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

use English;

open(PIPE, ">-");

print PIPE "#define P00_IGNORE(...)\n";

for (my $i = 1; $i < 1000; ++$i) {
    print PIPE "#define P00FIRST${i}(A, ...) A\n";
}

while (<STDIN>) {
    if (! m/# *[1-9][0-9]*/) {
        my $i = 0;
        s/[:]/,/go;
        s/_Pragma//go;
        #s|COMMENTSTART|/*|go;
        #s|COMMENTEND|*/|go;
        s|MACRODEFINE|#define |go;
        s|sizeof|P00_IGNORE|go;
        while (m/\(1 \?/) {
            s/\(1 \?/"P00FIRST".++$i."("/e;
        }
        while (m/\?/) {
            s/\?/"P00FIRST".++$i."("/e;
            $_ .= ")";
        }
        print PIPE;
    }
}
