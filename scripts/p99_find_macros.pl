#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

use English;

my %name;

open(PIPE, "|c99 -E - ");

my @files = (@ARGV);

# use @ARGV to run over all arguments
while (<>) {
    if (m/^#[ ]*define P99_PASTE/) {
        if (! m/^#[ ]*define P99_PASTE[0-6]\(/) {
            next;
        }
    }
    if (m/^#[ ]*define (P99\w*)\(([^)]*)\)/) {
        my $name = $1;
        my @args = split(",", $2);
        my $args = @args;
        my $va_arg = 0;
        $va_arg = 1 if (@args && $args[$args - 1] =~ m/[.][.][.]/);
        $name{$name} = $args + ($va_arg ? 2 : 0);
    } elsif (m/^[ 	]*P00_DOCUMENT_NUMBER_ARGUMENT\((\w*)[ 	]*,[ 	]*([0-9]*)\)/) {
        print PIPE "#define $1_ARG_$2 2\n";
    }
}

print PIPE << 'HEADER';
#define P00_DOCUMENT_DECLARATION_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N declaration
#define P00_DOCUMENT_MACRO_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N macro
#define P00_DOCUMENT_MULTIPLE_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N multiple
#define P00_DOCUMENT_PERMITTED_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N permitted
#define P00_DOCUMENT_STATEMENT_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N statement
#define P00_DOCUMENT_TYPE_ARGUMENT(MACRO, N) MACRODEFINE MACRO ## _ARG_ ## N type
HEADER

foreach my $file (@files) {
    print PIPE "#include \"${file}\"\n";
}


foreach my $name (sort(keys(%name))) {
    my $str = "$name: $name(";
    for (my $i = 0; $i < $name{$name}; ++$i) {
        $str .= "${name}_ARG_${i}, ";
    }
    $str =~ s/, $//;
    print PIPE "$str)\n";
}
