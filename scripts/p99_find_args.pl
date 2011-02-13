#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

use English;

my %name;

open(PIPE, "c99 -E -xc - |");

while (<PIPE>) {
    s/"[^"]"//go;
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
        print "Argument $number of macro $macro is evaluated $name{$name} times.\n";
        $exit = 1;
    }
}

exit($exit) if ($exit);
print "No macro that has undesired multiple evaluation of arguments found.\n";
