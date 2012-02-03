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

my $prefix = "";

my $result = GetOptions ("prefix=s"        => \${prefix},      # flag
    );

my %name;

open(PIPE, "c99 -E -xc - |");

while (<PIPE>) {
    s/(?:L?"(?:[^"\\]|[\\].)*+")//go;
    while (m/(\bP99_[a-zA-Z_]*ARG_[0-9]*\b)/) {
        my $name = $1;
        $name{$name} = 0 if (!defined($name{$name}));
        ++$name{$name};
        s/(\bP99_[a-zA-Z_]*ARG_[0-9]*\b)//;
    }
}

my $exit = 0;

foreach my $name (sort(keys(%name))) {
    if ($name{$name} > 1) {
        my ($macro, $number) = $name =~ m/(\w*)_ARG_(\d*)/;
        print "${prefix}Argument $number of macro $macro is evaluated $name{$name} times.\n";
        $exit = 1;
    }
}

exit($exit) if ($exit);
print "${prefix}No macro that has undesired multiple evaluation of arguments found.\n";
