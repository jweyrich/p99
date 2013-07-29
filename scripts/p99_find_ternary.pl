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
        s|MACRODEFINE|#define |go;
        s|sizeof|P00_IGNORE|go if (!defined($sizeof));
        s|__typeof__|P00_IGNORE|go;
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
