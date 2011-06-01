#!/bin/sh -f
#!perl     ##  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*- ##
#          ##  Exec to perl if started from /bin/sh. Do nothing if already executed by perl.            ##
# line 5
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;
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

no warnings 'portable';  # Support for 64-bit ints required
use integer;
use English '-no_match_vars';
use IO::Handle;
use POSIX;
use strict;
use Clone qw(clone);
use Getopt::Long qw(:config no_auto_abbrev no_ignore_case_always auto_version auto_help bundling);

## Variables that hold commandline arguments. They are filled in early
## within a BEGIN construct. Don't assign initial values here. Instead
## use "use constant" with a default value. This is a bit of magic,
## since the BEGIN block that is found below is executed between the
## declaration of the variable and the "use" construct.

## include directories
my @dirs;
## defines
my @defs;
## undefines
my @undefs;
## the standard we are operating with
my $std;
## whether or not to output a list of all (used) #define
my @dM;
## use as a separator to tokens for debugging
my $sep;
## request output with special character sets: without digraphs (0),
## unchanged (1, default), digraphs (2) or trigraphs (3).
my $graphs;
## indicate a hosted environment
my $hosted;
## The number of bits in an uintmax_t. By the standard it must be at
## least 64.
my $ubits;
## The number of bits in an intmax_t. By the standard it must be at
## least 63.
my $sbits;
## A number of bits where to split of large integer constants
my $bsplit;
## Forbid to shortcut expressions of the form ((X)) to (X)
my $noshortcut;
## Mangle universal character names in identifiers
my $mangle;
## Chose an output file, defaults to stdout
my $outfile;
##
my $verbose;
##
my $noline;
##
my $showincludes;

## A file handle to which all output will be pumped.
my $out;

## some variables needed for compatibility
my @mtune;
my @f;
my @W;
my @sdirs;
my $quiet;

my ($directory, $program_name);

BEGIN {

    ## setting help on the command line terminates the program early
    my $help;

    my %options = (
        "I=s"	=> \@{dirs},               # list of strings
        "D=s"        => \@{defs},          # list of strings
        "U=s"		=> \@{undefs},     # list of strings
        "P!"		=> \${noline}, # flag
        "H!"		=> \${showincludes}, # flag
        "d=s"		=> \@{dM},         # flag
        "noshortcut!"	=> \${noshortcut}, # flag
        "mangle!"		=> \${mangle}, # flag
        "hosted!"		=> \${hosted}, # flag
        "help|h!"		=> \${help},   # flag
        "separator=s"	=> \${sep},            # string
        "std|standard=s"	=> \${std},    # string
        "outfile|o=s"	=> \${outfile},        # string
        "graphs=i"		=> \${graphs},
        "ubits|bits=i"	=> \${ubits},
        "sbits=i"		=> \${sbits},
        "bsplit=i"		=> \${bsplit},
        "verbose|v+"	=> \${verbose},
        "f=s"		=> \@{f},
        "W=s"		=> \@{W},
        "isystem|S=s"	=> \@{sdirs},
        "m=s"		=> \@{mtune},
        "quiet!"		=> \${quiet}, # flag
        );

    my %allopts = map {
        s/=[^=]*$//o;
        s/[-+!]$//o;
        map {
            ($_ => 1);
        } split /[|]/;
    } keys %options;
    warn "we know about: ".join(" ", keys %allopts);

    my @argv = (@ARGV);

    ($directory, $program_name) = $PROGRAM_NAME =~ m|^(.*)/([^/]+)$|o;
    print STDERR "called as $program_name: $PROGRAM_NAME @argv\n";
    ## Ensure that all options that are seen as an assignment are treated as such
    @ARGV = map {
        if (m/-([^-=]+)/o) {
            $_ = "-$_" if (defined($allopts{$1}));
        }
        $_;
    } @ARGV;
    print STDERR "replaced for $program_name: $PROGRAM_NAME @ARGV\n";

    my $result = GetOptions (%options);
    print STDERR "called as $program_name: $PROGRAM_NAME @argv\n"
        ;#if ($verbose);

    push(@sdirs, "/usr/include");

    if ($help) {
        print STDERR "$PROGRAM_NAME: [options] file ...\n";
        my $max = 0;
        $max = (length > $max) ? length : $max foreach(keys %options);
        $max += 2;
        foreach (sort keys %options) {
            print STDERR $_.(" " x ($max - length));
            local $LIST_SEPARATOR = ":";
            if (ref($options{$_}) eq "ARRAY") {
                print STDERR "@{$options{$_}}";
            } elsif (ref($options{$_}) eq "SCALAR") {
                if (defined(${$options{$_}})) {
                    print STDERR "${$options{$_}}";
                } else {
                    print STDERR "<undefined>";
                }
            }
            print STDERR "\n";
        }
        exit 0;
    }

    if (!$outfile
        && ($program_name eq "cpp")
        && $#ARGV == 1) {
        my %defs = map {
            m/^([^=]*)=?(.*)$/o;
            $1 => ($2 // "");
        } @defs;
        if (defined($defs{__PCC__})
            && defined($defs{__PCC_MINOR__})
            && defined($defs{__PCC_MINORMINOR__})) {
            warn "assuming we are called as cpp for pcc, writing output to $ARGV[1]";
            #open($out, ">$ARGV[1]") || die "cannot open $ARGV[1] for writing";
            $mangle = 1 if (!defined($mangle));
            $outfile = pop @ARGV;
        }
    }
    if ($outfile) {
        open($out, ">$outfile") || die "cannot open $outfile for writing";
    }
}

use constant STD => $std // "c99";
use constant SEP => $sep // "";
use constant GRAPHS => $graphs // 1;
use constant HOSTED => $hosted // 1;
use constant UBITS => $ubits // 64;
use constant SBITS => $sbits // UBITS - 1;
use constant BSPLIT => $bsplit // 0;
use constant NOLINE => $noline // 0;
use constant SHOWINCLUDES => $showincludes // 0;
use constant NOSHORTCUT => $noshortcut // 0;
use constant MANGLE => $mangle // 0;
use constant VERBOSE => $verbose // 0;
$out //= \*STDOUT;

warn "directories: @dirs, @defs. ".BSPLIT if (VERBOSE);

sub UINTMAX_MAX();
sub INTMAX_MAX();
sub UINTSPLIT_MAX();

BEGIN {
    ## Only use bigint where we can not avoid it, namely when we
    ## really have to compute values in the preprocessor.
    use bigint;
    my $ubig = (1 << UBITS);
    sub UINTMAX_MOD() { $ubig; }
    my $sbig = (1 << SBITS) - 1;
    sub INTMAX_MAX() { $sbig; }
    my $uintsplit_max = (1 << BSPLIT) - 1;
    sub UINTSPLIT_MAX() { $uintsplit_max; }
}

sub splitInt(_) {
    if (BSPLIT) {
        use bigint;
        ## numbers that we will see here will always be without sign.
        my ($_, $spec) = $ARG[0] =~ m/([^uUlL]+)([uUlL]*)/o;
        if (m/^0[xX]/) {
            $_ = hex $_;
        } elsif (m/^0/) {
            $_ = oct $_;
        }
        warn "found $_";
        if ($_ <= UINTSPLIT_MAX) {
            return $_;
        } else {
            my $low = $_ & UINTSPLIT_MAX;
            my $hig = $_ >> BSPLIT;
            warn "found $hig  $low, ".(($hig << BSPLIT)|$low)." ?= $_";
            if ($spec =~ /[uU]/o || $_ > INTMAX_MAX) {
                #return sprintf("(((uintmax_t)0x%XULL << %d) | (uintmax_t)0x%XULL)", $hig, BSPLIT, $low);
                return "(((uintmax_t)${hig}ULL << BSPLIT) | (uintmax_t)${low}ULL)";
            } else {
                return "(((intmax_t)${hig}LL << BSPLIT) | (intmax_t)${low}LL)";
            }
        }
    } else {
        return $_;
    }
}



use constant NEWLINE => ord("\n");

use constant INTERVALOPEN => 1;
my $intervalOpen = chr(INTERVALOPEN);

use constant INTERVALCLOSE => 2;
my $intervalClose = chr(INTERVALCLOSE);

## This is a special unstring operator. It is not meant to be used in C
## code but just to implement the unstringing of the arguments of
## _Pragma directives.
use constant UNSTRING => NEWLINE + 1;
my $unstring = chr(UNSTRING);

## This is a special operator that serves to insert line information
## in the output.
use constant LINER => NEWLINE + 2;
my $liner = chr(LINER);

## This is a special operator that serves to insert a %: sequence in the output
use constant ESCHASH2 => NEWLINE + 3;
my $escHash2 = chr(ESCHASH2);

## This is a special operator that serves to insert a # character in the output
use constant ESCHASH => NEWLINE + 4;
my $escHash = chr(ESCHASH);

use constant COMMA => ord(",");
use constant COLON => ord(":");
use constant QUESTIONMARK => ord("?");
use constant PARENOPEN => ord("(");
use constant PARENCLOSE => ord(")");
use constant SLASH => ord("/");
use constant BSLASH => ord("\\");
use constant HASH => ord("#");
use constant DQUOTE => ord('"');
use constant QUOTE => ord("'");
use constant ELL => ord("L");

my %standards = (
    "c99" => "199901L",
    );

sub compList($\@);
sub compactLines(_);
sub containedHashtables(\%\%);
sub eqArrays(\@\@);
sub eqHashtables(\%\%);
sub escPre(@);
sub evalExpr($@);
sub expandComma(@);
sub expandDefined(\%@);
sub expandPar(@);
sub findfile(_;$@);
sub flushOut(\@);
sub getTokDef(\@);
sub handlePragma(_);
sub joinToks(@);
sub macro(_);
sub macroContained(\%);
sub macroDefine($\@;\@\%);
sub macroHide(_);
sub macroHidden();
sub macroList(;$);
sub macroUndefine($;\@);
sub macroUnhide(_);
sub openfile(_);
sub opRec(\@);
sub parenRec(\@$);
sub printArray(\@;$);
sub rawtokenize($);
sub readlln(_);
sub readln(_);
sub logicalLine($);
sub splitInt(_);
sub substituteArray(\%\@\@@);
sub substituteArrayStringify(\%\@\@@);
sub tokenize(_);
sub tokrep($$$\%@);
sub toktrans($$\%);
sub unescPre(@);
sub untokenize(@);

## Each entry holds a list of usage values for the arguments of the macro.
my %counters;
## Each entry holds a list of used arguments of the macro.
my %positions;
## Each entry holds a hash that gives the position for each macro argument name.
my %argPosition;

## All the potential macros that have been queried and that have not
## been defined, then.
my %undefMacros;

## The amount of join operators in a macro expansion, if any.
my %joins;

## The amount of stringify operators in a macro expansion, if any.
my %stringifies;

my $univ = qr/(?:\\u[[:xdigit:]]{4}|\\U[[:xdigit:]]{8})/;

## a regexp to detect preprocessor identifier token
my $isidentifier = qr/(?:(?:[_[:alpha:]]|$univ)(?:\w|$univ)*+)/;


## holds all defined macros
{
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) =
        localtime(time);
    my @abbr = qw( Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec );

    my %macro = (
        ## The special "macro" _Pragma that we never could do with #define.
        #"_Pragma" => ["X", [$liner, "_Pragma", "X", $liner]],
        ## Mark our presence by setting a special macro.
        "__P99PRO__" => [["1"]],
        ## Provide the macros that are required by the standard. __LINE__ and __FILE__ are
        ## implemented differently.
        "__DATE__" => [[sprintf("\"%s % 2u %u\"", $abbr[$mon], $mday, $year + 1900)]],
        "__TIME__" => [[sprintf("\"%02u:%02u:%02u\"", $hour, $min, $sec)]],
        "__STDC_VERSION__" => [[$standards{STD()}]],
        "__STDC__" => [["1"]],
        "__STDC_HOSTED__" => [[HOSTED()]],
        ## None of these macro names, nor the identifier defined, shall be the subject of a
        ## #define or a #undef preprocessing directive.
        #"defined" => [["(== abuse of keyword defined ==)"]],
        );

    my $predefMacros = scalar keys %macro;

    my %hidden;

    ## return the definition of the macro passed as first argument or undef if it isn't a macro at all
    ## or if it is currently not visible.
    ##
    ## The use of this function is normally dominated by the case of non-macros so all is optimized for
    ## that. In particular in that case the function performs just one hash lookup and one conditional.
    ##
    ## In the case that there is actually a visible definition, there are two cases. The first is that
    ## the macro has already been called once, so just the value is returned. Otherwise some
    ## precomputation for the macro is done.
    sub macro(_) {
        ## values in %macro and %counters are references so they are either undef or non-zero.
        if ($macro{$ARG[0]}) {
            if ($counters{$ARG[0]}) {
                if (!$hidden{$ARG[0]}) {
                    $macro{$ARG[0]};
                } else {
                    undef;
                }
            } else {
                my $name = shift;
                my ($tokDef, %def, %pos) = ($macro{$name});

                ## For each name in the parameter list, hash its
                ## position in that list
                foreach (0 .. $#{$tokDef}-1) {
                    $def{$tokDef->[$_]} = $_;
                }
                $argPosition{$name} = \%def;

                ## For each token in the replacement list, look up if
                ## it is a parameter, hash the positions that are
                ## being used and count its usage.
                map {
                    ++$pos{$_};
                } grep {
                    defined;
                } map {
                    if ($_ eq "#") {
                        ++$stringifies{$name};
                    } elsif ($_ eq "##") {
                        ++$joins{$name};
                    }
                    $def{$_};
                } getTokDef(@{$tokDef});
                $positions{$name} = [sort { $a <=> $b } keys %pos];

                if ($joins{$name}) {
                    my $td = $tokDef->[-1];
                    foreach my $i (0 .. $#{$td}) {
                        if ($td->[$i] eq "##") {
                            ## the parameters that are before or after
                            ## the ## must not be expanded too early
                            $td->[$i-1] = "#".$td->[$i-1] if (defined($def{$td->[$i-1]}));
                            $td->[$i+1] = "#".$td->[$i+1] if (defined($def{$td->[$i+1]}));
                        }
                    }
                }


                ## Linearlize the hash to an array. Macros will only
                ## have at most some hundred arguments so hopefully
                ## lookup will be more efficient when using an array.
                foreach (keys %pos) {
                    $ARG[$_] = $pos{$_};
                }
                $counters{$name} = \@ARG;

                ## Remove the parameters that are not used from the hash.
                foreach (keys %def) {
                    delete $def{$_} if (!$pos{$def{$_}});
                }

                ## return the value
                $tokDef;
            }
        }
    }


    sub macroDefine($\@;\@\%) {
        my ($name, $val, $defines, $used) = @ARG;
        # An identifier currently defined as an object-like macro
        # shall not be redefined by another #define preprocessing
        # directive unless the second definition is an
        # object-like macro definition and the two replacement
        # lists are identical. Likewise, ...
        my $todo = !defined($macro{$name});
        if (!$todo) {
            ## Enforce the tokenized representation of both
            ## definitions.
            getTokDef(@{$macro{$name}});
            getTokDef(@{$val});
            $todo = !eqArrays(@{$macro{$name}}, @{$val});
            if ($todo) {
                warn "redefinition of macro $name:";
                warn "old: ".printArray(@{$macro{$name}});
                warn "new: ".printArray(@{$val});
            } else {
                $used->{$name} = $macro{$name};
            }
        }
        if ($todo) {
            $macro{$name} = $val;
            push(@{$defines}, $name) if (defined($defines));
        }
    }

    sub macroUndefine($;\@) {
        my ($name, $defines) = @ARG;
        if (defined($macro{$name})) {
            delete $macro{$name};
            delete $counters{$name};
            delete $positions{$name};
            delete $argPosition{$name};
            delete $joins{$name};
            delete $stringifies{$name};
            push(@{$defines}, $name)
                if (defined($defines));
        }
    }

    sub macroHide(_) {
        ## Pre-increment to be sure to always return a non-zero value.
        ## This is magic for undef values.
        ++$hidden{$ARG[0]};
    }

    sub macroUnhide(_) {
        ## Post-increment to be sure to always return a non-zero value.
        ## This isn't magic for undef values, but there must always have been an increment before a
        ## decrement.
        $hidden{$ARG[0]}--;
    }

    sub macroHidden() {
        if (wantarray) {
            grep { $hidden{$_} } keys %hidden;
        } else {
            scalar grep { $_ } values %hidden;
        }
    }

    sub macroContained(\%) {
        return containedHashtables(%{$ARG[0]}, %macro)
    }

    sub macroDump($;$) {
        my ($name, $restricted) = @_;
        if (ref $macro{$name}) {
            my @def = @{$macro{$name}};
            my $repl = pop @def;

            if (ref($repl)) {
                $repl = untokenize(@{$repl});
            } elsif ($restricted) {
                return;
            }
            $def[-1] = "..." if (@def && $def[-1] eq "__VA_ARGS__");

            if ($restricted && $undefMacros{$name}) {
                print $out
                    "#undef\t$name ",
                    " " x (30 - length($name)),
                    "/** has been redefined later **/\n";
            }
            if (@def && length($def[0]) && $positions{$name}) {
                my $args = scalar @def;
                $args .= " (or more)" if (@def && $def[-1] eq "...");
                if (@{$positions{$name}} == 0) {
                    if (@def > 1) {
                        print $out "//*****\t$name uses none of its $args arguments\n";
                    } else  {
                        print $out "//*****\t$name doesn't use its $args argument\n";
                    }
                } elsif (@{$positions{$name}} == @def) {
                    if (@{$positions{$name}} == 1 && $def[-1] ne "...") {
                        print $out "//*****\t$name uses its argument\n";
                    } else {
                        print $out "//*****\t$name uses all its $args arguments\n";
                    }
                } else {
                    if (@{$positions{$name}} == 1) {
                        print $out "//*****\t$name uses argument $def[$positions{$name}[0]]\n";
                    } else {
                        print $out "//*****\t$name uses arguments ".
                            join(" ",
                                 map {
                                     $def[$_];
                                 } @{$positions{$name}}
                            )."\n";
                    }
                }
            }
            print $out "//*****\t$name uses the ## operator to join tokens\n" if (defined($joins{$name}));
            print $out "//*****\t$name uses the # operator for stringification\n" if (defined($stringifies{$name}));
            print $out "#define\t$name";
            if (@def) {
                print $out "(", join(", ", @def), ")";
            }
            print $out " $repl\n";
        } else {
            print $out "#undef\t$name\n";
        }
    }

    sub macroState() {
        my $defs = scalar keys %macro;
        $defs -= $predefMacros;
        my @hids = grep { $hidden{$_} } keys %hidden;
        if (!@hids) {
            @hids = ("none", "are");
        } elsif (scalar @hids == 1) {
            push(@hids, "is");
        } else {
            push(@hids, "are");
        }
        warn "/* There are $defs macros, of which @hids hidden. */";
    }

    sub macroList(;$) {
        my ($restricted) = shift;
        my %interesting = ($restricted
                           ? (%undefMacros, %macro)
                           : (%macro));
        foreach my $name (sort keys %interesting) {
            macroDump($name, $restricted);
        }
    }

}

## per include file a hash of macros that were used by that file. This is only set if the
## include file did not define macros by itself.
my %usedMac;

## Per include file a hash of macros of tokens that are to be
## substituted for that file if the macros used by that file have not
## changed. This is only used when the file generates no side effects,
## that is, it has no active defines of its own.
my %fileHash;

## How many times have we processed this file.
my %fileInc;

## did we locally change line numbering?
my $skipedLines = 0;

## has pragma once been called?
my $pragmaOnce;

## integrate the defines that we received on the command line
foreach my $def (@defs) {
    my ($name, $val) = $def =~ m/^([^=]++)=?+(.*+)/o;
    $val //= "";
    my @val = ([$val]);
    macroDefine($name, @val);
}

my @punctPri = (
    ## highest priority, left-to-right
    ["(", ")", "[", "<:", "]", ":>", ".", "->", "++", "--"],
    ##
    ["!", "~"],
    ##
     ["*", "/", "%"],
    ##
    ["+", "-"],
    ##
    ["<<", ">>"],
    ##
    [">", "<", "<=", ">="],
    ##
    ["==", "!="],
    ##
    ["&"],
    ##
    ["^"],
    ##
    ["|"],
    ##
    ["&&"],
    ##
    ["||"],
    ##
    ["?", ":"],
    ##
    ["=", "%=", "&=", "*=", "+=", "-=", "/=", "^=", "|=", "<<=", ">>="],
    ##
    [","],
    ## code punctuation
    [";", "{", "<%", "}", "%>", "..."],
    ## preprocessor
    ["#", "##", "%:", "%:%:"],
    ## internal
    [$liner, $unstring, $escHash, $escHash2, "$escHash$escHash", "$escHash2$escHash2"],
    );

my %punctPri;

{
    my @tmp;
    for (my $i = 0; $i <= $#punctPri; ++$i) {
        my @pairs = map { $_ => $i } @{$punctPri[$i]};
        push(@tmp, @pairs);
    }
    %punctPri = @tmp;
}

## all punctuation tokens. put the longest first, so they will match first.
my @punct = sort {
    length($b) <=> length($a)
} map {
    @{$_}
} @punctPri;

my @escPunct = map { "\Q$_\E" } @punct;

## a regexp for all punctuation operators
my $punctStr = "(?:".join("|", @escPunct).")";
my $punct = qr/$punctStr/;
my $punctSplit = qr/($punctStr)/;
my $punctToken = qr/^$punctStr$/;

my @punct2 = grep { defined } map {
    if (2 > length || m/[+][+]|[-][-]/o) {
        undef;
    } else {
        $_
    }
} @punct;

my @escPunct2 = map { "\Q$_\E" } @punct2;

## a regexp for all punctuation operators with more than two
## characters, excluding ++ and --.
my $punct2Str = "(?:".join("|", @escPunct2).")";
my $punct2 = qr/$punct2Str/;
my $punct2Split = qr/($punct2Str)/;
my $punct2Token = qr/^$punct2Str$/;

## Binary operators that have boolean value as result.
my @logicalOp = map { "\Q$_\E" } (
    ">", "<", "<=", ">=",
    "==", "!=",
    "&&", "||",
);

## a regexp for all binary operators that return bool
my $logicalOpStr = "(?:".join("|", @logicalOp).")";
my $logicalOp = qr/$logicalOpStr/;

## all digraph tokens. put the longest first, so they will match first.
my @digraph = (
    "[%]:[%]:",
    "[%]:",
    "[%][>]",
    ":[>]",
    "[<][%]",
    "[<]:",
    );

my $digraph = "(?:".join("|", @digraph).")";
$digraph = qr/$digraph/;

## all idgraph tokens. put the longest first, so they will match first.
my @idgraph = (
    "[#][#]",
    "[#]",
    "[}]",
    "[\]]",
    "[{]",
    "[\[]",
    );

my $idgraph = "(?:".join("|", @idgraph).")";
$idgraph = qr/$idgraph/;

my %digraph = (
    "%:%:" => "##",
    "%:" => "#",
    "%>" => "}",
    ":>" => "]",
    "<%" => "{",
    "<:" => "[",
    );

my %idgraph = map {
    $digraph{$_} => $_
} keys(%digraph);


## a regexp to detect string literals
my $isstring = qr/(?:L?"(?:[^"\\]|[\\].)*+")/;
my $isstringToken = qr/^(?:L?".*+")$/;
## a regexp to detect character constants
my $ischar = qr/(?:L?'(?:[^'\\]|[\\].)*+')/;
my $ischarToken = qr/(?:L?'.*+')/;
## a regexp to detect preprocessor number token
my $isnumber = qr/(?:[.]?+\d(?:(?:[eEpP][-+])|\w|[.])*+)/;


my $integer =
    qr/^
    (?:
     (?:
      # a number starting with 0 is hex or octal
      # and can never be decimal
      (?:0{1}+
       # if there is 0x must be hex, don't allow the parser to
       # backtrack
       (?:[xX]{1}+
        # there must be at least one digit after the 0x
        [[:xdigit:]]++)
       # an octal constant
       |(?:[0-7]*+))
      # a decimal constant now is forced to start with a non-zero digit
      |(?:[[:digit:]]++))
     # all of this may be followed by an integer suffix
     (?:(?:(?:l{0,2}+|L{0,2}+)[uU]?+)
      |(?:[uU](?:l{0,2}+|L{0,2}+)))?+
    $)/x;

my $ishash = qr/(?:[#]|[%][:])/;
my $ishhash = qr/(?:[#][#]|[%][:][%][:])/;

my $tokenizer = qr/(?:$isstring|$ischar|$isidentifier|$isnumber)/;
my $tokenizerSplit = qr/($isstring|$ischar|$isidentifier|$isnumber)/;
my $tokenizerToken = qr/^(?:$isstring|$ischarToken|$isidentifier|$isnumber)$/;

## regexp that we need when untokenizing
## imperative, don't create tokens that weren't there before
my $clash = qr/^(?:$isnumber|$isidentifier|$punctStr)$/;

my $spaceAfter = qr/(?:
    ## visual enhancement for some single character token
    [=,;?:{]
    |$punct2Token
    |$isstring
    )$/x;

my $spaceBefore = qr/(?:
    ## visual enhancement for some single character token
    [=}]
    |$punct2Token
    |$isstring
    )$/x;

## all third characters that may appear in a trigraph
my $trigraph = "-!'/=()<>";
## all replacements for trigraphs, in the corresponding order
my $onegraph = "~|^\\#[]{}";

my %onegraph;

my @trigraph = split(//, $trigraph);
my @onegraph = split(//, $onegraph);

for (my $i = 0; $i <= $#onegraph; ++$i) {
    $onegraph{$onegraph[$i]} = "??$trigraph[$i]";
}

$trigraph = "(?:[\Q$trigraph\E])";
$onegraph = "(?:[\Q$onegraph\E])";
$trigraph = qr@$trigraph@;
$onegraph = qr@$onegraph@;

sub insertLine(\@$;$) {
    my ($outTok, $file, $fd) = @ARG;
    my $nr = $fd ? $fd->input_line_number() : $NR;
    $nr = 0 if ($nr < 0);
    push(@{$outTok}, "# $nr \"$file\"\n");
}

sub printArray(\@;$) {
    my ($arg, $par) = @ARG;
    $par = "(,)[,]" x 10 if (!$par);
    return
        substr($par, 0, 1).
        join(substr($par, 1, 1)." ",
             map {
                 if (ref($_) eq "ARRAY") {
                     printArray(@{$_}, substr($par, 3));
                 } else {
                     $_;
                 }
             } @{$arg})
        .substr($par, 2, 1);
}


## same as substituteArray but also stringifies arguments where this
## is necessary.
sub substituteArrayStringify(\%\@\@@) {
    my ($def, $args, $pargs) = (shift, shift, shift);
    my @ret;
    my @counts;
    my $_;
    while (@ARG) {
        $_ = shift;
        my $protected;
        if (ord == HASH) {
            if (m/^#\w/o) {
                s/^#//o;
                $protected = 1;
            }
        }
        if (defined($def->{$_})) {
            my $i = $def->{$_};
            push(@ret,
                 $protected
                 ? @{clone($pargs->[$i])}
                 : (
                     ## only clone an argument if and when it is used more than once
                     $counts[$i]
                     ? @{clone($args->[$i])}
                     : @{$args->[$i]}
                 )
                );
            ## The increment operation is magic on undef values.
            ++$counts[$i];
        } elsif ($_ eq "#") {
            if (defined($def->{$ARG[0]})) {
                # stringification should use the unexpanded argument
                $_ = untokenize(@{$pargs->[$def->{$ARG[0]}]});
                # for some reason that I don't understand this shift
                # can't just be put in-place in the previous line.
                shift;

                ## White space before the first preprocessing token
                ## and after the last preprocessing token composing
                ## the argument is deleted.
                s/^\s*(\w++)\s*$/$1/o;

                ## Each occurrence of white space between the
                ## argument's preprocessing tokens becomes a single
                ## space character in the character string literal.
                s/^\s+//o;

                ## a \ character is inserted before each " and \
                ## character of a character constant or string
                ## literal
                s/(["\\])/\\$1/go;

                push(@ret, '"'.$_.'"');
            } else {
                ## Each # preprocessing token in the replacement
                ## list for a function-like macro shall be followed
                ## by a parameter as the next preprocessing token
                ## in the replacement list.
                warn "invalid stringify sequence $_ $ARG[0], not a parameter name";
                push(@ret, $_);
            }
        } else {
            push(@ret, $_);
        }
    }
    @ret;
}

## Perform argument replacement for macros
## $def holds the names of the arguments and gives back there position in the argument list
## $args are the expanded arguments
## $pargs are the unexpanded arguments
##
## $pargs may be empty if the macro in question will not perform a ## operation.
sub substituteArray(\%\@\@@) {
    my ($def, $args, $pargs, @mustClone) = (shift, shift, shift);
    map {
        my $protected;
        if (ord == HASH) {
            if (m/^#\w/o) {
                s/^#//o;
                $protected = 1;
            }
        }
        if (defined($def->{$_})) {
            $_ = $def->{$_};
            if ($protected) {
                @{clone($pargs->[$_])};
            } elsif ($mustClone[$_]) {
                ## only clone an argument if and when it is used more than once
                @{clone($args->[$_])};
            } else {
                $mustClone[$_] = 1;
                @{$args->[$_]};
            }
        } else {
            $_;
        }
    } @ARG;
}

## translate token sequences to digraph or trigraph representation
sub toktrans($$\%) {
    my ($inp, $reg, $hash) = @ARG;
    my @tokens = map {
        if ($_ && $_ =~ m/$reg/) {
            $hash->{$_}
        } else {
            $_
        }
    } split(/($reg)/so, $inp);
    return join("", @tokens);
}

sub handlePragma(_) {
    my $_ = shift;
    s/^L?"(.+)"$/$1/o;
    s/\\(.)/$1/go;
    @ARG = tokenize;
    $_ = shift;
    if ($_ eq "message") {
        warn "message @ARG";
        @ARG = ();
    } elsif ($_ eq "once") {
        $pragmaOnce = 1;
    } elsif ($_ eq "P99") {
        $_ = shift;
        if ($_ eq "dump") {
            $_ = shift;
            if (!m/^["]/) {
                macroDump($_);
            } else {
                if (m/M/) {
                    macroList();
                }
                if (m/U/) {
                    macroList(1);
                }
                if (m/S/) {
                    macroState();
                }
            }
        }
    } else {
        unshift(@ARG, "#pragma ", $_);
    }
    @ARG;
}

local $SIG{__WARN__};

sub evalExprBigint($@) {
    use bigint;
    my $isUn = shift;
    my $back = $SIG{__WARN__};
    $SIG{__WARN__} = sub {
        &{$back}(@ARG);
        print STDERR "   expression is '@ARG'\n";
    };
    my $res = eval("@ARG");
    if ($ARG[0] eq "!"
        || (defined($ARG[1]) && $ARG[1] =~ m/^$logicalOp$/o)) {
        $res = $res ? 1 : 0;
        $isUn = 0;
    }
    if ($isUn && $res < 0) {
        $res %= UINTMAX_MOD
    }
    $SIG{__WARN__} = $back;
    return ($isUn, $res);
}

sub evalExpr($@) {
    my $isUn = shift;
    my $back = $SIG{__WARN__};
    $SIG{__WARN__} = sub {
        &{$back}(@ARG);
        print STDERR "   expression is '@ARG'\n";
    };
    my $res = eval("@ARG");
    if ($ARG[0] eq "!"
        || (defined($ARG[1]) && $ARG[1] =~ m/^$logicalOp$/o)) {
        $res = $res ? 1 : 0;
        $isUn = 0;
    }
    if ($isUn && $res < 0) {
        use bigint;
        $res %= UINTMAX_MOD
    }
    $SIG{__WARN__} = $back;
    return ($isUn, $res);
}

## This function evaluates an expression given in the token list as
## second argument (pointer to array). It is a recursive function, the
## first argument is the recursion level.
##
## The token list should contain a valid expression where the leaf
## nodes are just numbers. Identifiers that are found are simply
## replaced with 0. All numbers that C accepts can be used
## here. Suffixes 'U' or 'L' will be peeled of.
##
## Special care is taken to detect unsigned numbers among the tokens
## and to evaluate the subexpressions to which they contribute with
## the correct modulus. Therefore this function returns a tuple if it
## is evaluated in an array context. The first value states if this is
## an unsigned number (or not) and the second is the result of the
## computation.
sub compList($\@) {
    my ($level, $toks) = @ARG;
    my $type = 0;
    my @list;
  LOOP:
    while (@{$toks}) {
        my $_ = shift(@{$toks});
        if (ord == PARENOPEN) {
            my @subexp = compList($level + 1, @{$toks});
            $type ||=  $subexp[0];
            push(@list, $subexp[1]);
        } elsif (ord == QUESTIONMARK) {
            my @subexp = compList($level + 1, @{$toks});
            ## The type of the controlling expression is irrelevant
            ## for ternary, so we overwrite the previous value.
            $type = $subexp[0];
            push(@list, $_, $subexp[1]);
            $_  = shift(@{$toks});
            if (ord == COLON) {
                my @subexp = compList($level + 1, @{$toks});
                $type ||= $subexp[0];
                push(@list, $_, $subexp[1]);
            } else {
                warn "missing ':' in ternary expression before @{$toks}";
            }
        } elsif (ord == COLON) {
            ## This should be a colon of a ternary expression. Unshift
            ## it such that we may do an error check upwards.
            unshift(@{$toks}, $_);
            $level = 0;
            last LOOP;
        } elsif (ord == PARENCLOSE) {
            $level = 0;
            last LOOP;
        } elsif ((ord) < ord(" ")) {
            next LOOP;
        } else {
            if (m/$integer/o) {
                m/([uU])/o;
                ## a number with an 'U' is unsigned
                $type ||= $1 ? 1 : 0;
                s/[lLuU]//go;
                ## a number that is greater than INTMAX_MAX must be
                ## unsigned. Only check large numbers for that. Large
                ## here is larger than
                ## 0xFFFFFFF (=268435455) for hexadecimal numbers
                ## 999999999              for     decimal numbers
                ## 077777777 (= 16777215) for       octal numbers
                if (length > 9) {
                    use bigint;
                    $type ||= ($_ > INTMAX_MAX) ? 1 : 0;
                }
                push(@list, $_);
            } elsif (m/^\w+/o) {
                push(@list, "0");
            } else {
                push(@list, $_);
            }
        }
    }
    warn "preliminary end of regular expression, missing ')'" if ($level > 0);
    warn "preliminary end of regular expression, missing operand" if (!@list);
    my ($rtype, $res);
    if (length("@list") > 9) {
        ($rtype, $res) = evalExprBigint($type, @list);
    } else {
        ($rtype, $res) = evalExpr($type, @list);
    }
    return wantarray ? ($rtype, $res) : $res;
}

sub transl(_) {
    join(
        "",
        map(
            (0x100 > ord)
            ? $_
            :((0xFFFF < ord)
              ? sprintf("\\U%08X", ord)
              : sprintf("\\u%04X", ord)),
            split(//, $ARG[0])));
}

## read one physical input line and perform the trigraph replacement
sub readln(_) {
    my $_ = readline(shift);
    if (defined) {
        chomp;
        $_ = transl;
        if (m/[?][?]/o) {
            " ".join("",
                     map {
                         if (length == 3
                             && m/^[?][?]($trigraph)$/o) {
                             $_ = $1;
                           tr:-!'/=()<>:~|^\\#[]{}:;
                         }
                         $_;
                     }  split /([?][?]$trigraph)/o
                );
        } else {
            " ".$_;
        }
    }
}

## Scan the list of include directories for a file
sub findfile(_;$@) {
    my ($file, $next) = (shift, shift);
    @ARG = (@dirs, @sdirs) if (!@ARG);
    undef $next if ($next && $next !~ m|/$file$|);
    foreach (@ARG) {
        if ($next) {
            if ($next =~ m|^$_/[^/]+$|) {
                undef $next;
            }
            next;
        }
        my $fname = $_."/".$file;
        return $fname if (-e $fname);
    }
    warn "didn't find $file in @ARG";
    return "./$file";
}

sub tokenize(_) {
    grep {
        length
    } map {
        ## keep it if it is already a "long" token such as strings,
        ## char, identifier or number.
        if (m/$tokenizerToken/o) {
            $_;
        } else {
            ## Other tokens are first split at white space and then
            ## with maximum match rule according to the valid
            ## punctuation tokens.
            map {
                split m/$punctSplit/o;
            } split /\s+/o;
        }
    } split($tokenizerSplit, shift);
}

sub rawtokenize($) {
    @ARG = split(/($isstring|$ischar)/o, shift);
    ## The split only has more than one element if there is a string
    ## literal or character constant in the line. So the inner part of
    ## the while loop will only be executed rarely.
    my @ret = (shift);
    while (my $_ = shift) {
        if (length > 1 || $ret[-1] =~ m/["']$/o) {
            push(@ret, $_);
        } else {
            $ret[-1] .= $_;
        }
    }
    @ret;
}

sub untokenize(@) {
    @ARG = grep { ord != INTERVALOPEN && ord != INTERVALCLOSE } @ARG;
    my $prev = "";
    if (SEP) {
        join("",
             map {
                 ## Don't add spaces before and after newlines
                 if (ord == NEWLINE
                     || $prev =~ m/\s$/so) {
                     $prev = $_;
                     $_;
                 } else {
                     my $comb = $prev . $_;
                     if (
                         $comb =~ $clash
                         || $_ =~ $spaceBefore
                         || $prev =~ $spaceAfter
                         ) {
                         $prev = $_;
                         SEP." ".$_;
                     } else {
                         $prev = $_;
                         SEP.$_;
                     }
                 }
             } @ARG
            );
    } else {
        join("",
             map {
                 if (ord != NEWLINE) {
                     if (MANGLE && m/^$isidentifier$/o && m/$univ/o) {
                         ## Transform the 'u' plus hex sequence to
                         ## uppercase. By that we voluntarily use
                         ## reserved identifiers starting with an
                         ## underscore and a capital letter for cases
                         ## that the input is just one universal
                         ## character. Otherwise this could clash with
                         ## a local variable of the form _u03ba, which
                         ## is allowed by the standard.
                         s/\\(u[[:xdigit:]]{4}|U[[:xdigit:]]{8})/_\U$1\E/go;
                         ## This mangling might still not be enough if
                         ## the symbol starts with a valid sequence of
                         ## characters.
                         $_ = "__".$_ if (!m/^_[A-Z_]/o);
                     }
                     if (BSPLIT && m/^(?:(?:(?:0[xX])[[:xdigit:]]+)|(?:0[0-7]*)|(?:\d+))[uUlL]*$/o) {
                         $_ = splitInt($_);
                         warn "here its $_";
                         $prev = $_;
                     } else {
                         my $comb = $prev . $_;
                         if ($comb =~ $clash) {
                             $prev = $_;
                             " ".$_;
                         } else {
                             if ($_ =~ $spaceBefore) {
                                 $prev = $_;
                                 " ".$_;
                             } else {
                                 if ($prev =~ $spaceAfter) {
                                     $prev = $_;
                                     " ".$_;
                                 } else {
                                     $prev = $_;
                                 }
                             }
                         }
                     }
                 } else {
                     $prev = $_;
                 }
             } @ARG
            );
    }
}

sub readlln(_) {
    my $_ = readln($ARG[0]);
    while (ord(substr($_, -1, 1)) == BSLASH) {
        chop;
        $_ .= readln($ARG[0]);
        $skipedLines = 1;
    }
    $_;
}

sub logicalLine($) {
    my ($fd, $ret, $cont) = (shift, "");
    @ARG = rawtokenize(readlln($fd));
  SCAN:
    while ($_ = shift) {
        if (!m/["']$/o    ## String or char tokens are better
                          ## characterized by their terminating
                          ## character.
            && m|/[*/]|o  ## Find a simple match for a comment start,
                          ## first, and only dissect it if we found
                          ## one.
            ) {
            m|(.*?)/([*/])(.*)|o;
            $ret .= $1;
            if (ord($2) == SLASH) {
                last SCAN;
            } else {
                $_ = untokenize($3, @ARG);
              MATCH:
                while (1) {
                    if (m|[*]/|so) {
                        m|(?:.*?)[*]/(.*)|so;
                        @ARG = rawtokenize($1);
                        last MATCH;
                    } elsif ($fd && ($cont = readlln($fd))) {
                        $skipedLines = 1;
                        $_ .= " " . $cont;
                    } else {
                        warn "unterminated /* comment at end of input";
                        last SCAN;
                    }
                }
            }
        } else {
            $ret .= $_;
        }
    }
    $ret;
}

sub metaTok($) {
    defined($ARG[0])
        && (ord($ARG[0]) == NEWLINE
            || ord($ARG[0]) == INTERVALOPEN
            || ord($ARG[0]) == INTERVALCLOSE);
}

## Test if the token is a callback marker and run the corresponding hide or unhide function.  Returns
## some true value if a callback has been performed and a false value (undef) otherwise.
sub tokCallback(_) {
    if (ord($ARG[0]) < INTERVALCLOSE) {
        macroHide(substr($ARG[0], 1));
    } else {
        macroUnhide(substr($ARG[0], 1));
    }
}

sub expandDefined(\%@) {
    my $used = shift;
    my @outToks;
    while (@ARG) {
        my $_ = shift;
        if ($_ eq "defined") {
            $_ = shift;
            if (ord == PARENOPEN) {
                $_ = shift;
                warn "unbalanced \"defined\" expression near $_"
                    if (ord(shift) != PARENCLOSE);
            }
            my $val = macro;
            if ($val) {
                $used->{$_} = $val;
                $_ = 1;
            } else {
                $undefMacros{$_} = 1;
                $_ = 0;
            }
        }
        push(@outToks, $_);
    }
    return @outToks;
}

my @files;

## Receives a filename and a reference to he used macros.
##
## Returns a triple (defines, output, input) where
## - defines points to a list of macro names that where change by
##   recursively scanning the preprocessing directives in file. A file
##   will be candidate for shortcut replacement if this list is empty,
##   i.e this file didn't change the state of the preprocessor.
## - input is the input after lines protected by #if/#else constructs
##   etc have been removed. This will be the starting point for
##   shortcut replacement if the file will be visited again, defines
##   had been empty and the macros that are used by preprocessor are
##   unchanged.
## - output is the output generated from input by doing macro replacement

sub openfile(_) {
    my $file = shift;
    push(@files, $file);
    warn "opening $file"
        if (VERBOSE);
    ## keep track of the number of defines in this file and below
    my @defines;
    ## keep track of the tokens produced by this file and below
    my $output = "";
    my $input = "";

    my %used;

    if (defined($usedMac{$file})) {
        if (macroContained(%{$usedMac{$file}})) {
            warn "reusing previously read file $file"
                if (VERBOSE);
            if (length($fileHash{$file})) {
                @ARG = map {
                    (tokrep(0, $file, undef, %used, escPre(tokenize)), "\n");
                } split("\n", $fileHash{$file});
            } else {
                @ARG = "\n";
            }
            if (SHOWINCLUDES) {
                print STDERR "+" x $#files, " $file\n";
            }
            goto SHORTCUT;
        } else {
            delete $fileHash{$file};
            delete $usedMac{$file};
        }
    } else {
        ## the increment operator is magic on undef values.
        ++${$fileInc{$file}};
    }

    my $iflevel = 0;
    my $aclevel = 0;

    my $fd;
    if ($file eq "-") {
        $fd = \*STDIN;
    } elsif (!open($fd,  "<:encoding(UTF-8)", "$file")) {
        warn "couldn't open $file";
        pop(@files);
        return ([], []);
    }

    ## Install a warning handler.
    my $back = $SIG{__WARN__};
    $SIG{__WARN__} = sub { print STDERR "$file:".$fd->input_line_number().": warning: $ARG[0]"; };
    my @iffound;

    warn "opening $file"
        if (VERBOSE);

    if (SHOWINCLUDES) {
        print STDERR "." x $#files, " $file\n";
    }


    insertLine(@ARG, $file, $fd) if (!NOLINE);
    while (my $_ = logicalLine($fd)) {
        #print STDERR "line: "."." x $aclevel."!" x ($iflevel - $aclevel)."$_\n";
        ############################################################
        ## First check if this is starting a preprocessing directive.
        if (m/^\s*+$ishash\s*+(.*+)/o) {
            if (scalar macroHidden()) {
                my $str = join(", ", macroHidden());
                warn "Macros hidden at the start of preprocessing directive: $str";
            }
            $skipedLines = 1;
            $_ = $1;
            s/\s\s+/ /go;
            if (m/^(el)?+if\s++(.*+)/o) {
                my $ifcont = $1;
                if ($ifcont) {
                    warn "#elif without preceeding #if"
                        if (!$iflevel);
                    ## this #elsif terminates the preceeding #if or #elsif
                    --$aclevel
                        if ($aclevel == $iflevel);
                } else {
                    ++$iflevel;
                }
                if (($aclevel == ($iflevel - 1))
                    && !($ifcont && $iffound[$iflevel])) {
                    my @expDef =  expandDefined(%used, tokenize($2));
                    my @toks = tokrep(0, $file, $fd, %used, @expDef);
                    push(@toks, ")");
                    my $toks = parenRec(@toks, $fd);
                    opRec(@{$toks});
                    @toks = ($toks);
                    @toks = expandPar(@toks);
                    local $LIST_SEPARATOR = "";
                    if (compList(0, @{toks})) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                }
                #print STDERR "IF $aclevel <= $iflevel : (el)if $_\n";
            } elsif (m/^if(n?+)def\s++(\w++)/o) {
                warn "if$1def for macro $2"
                    if (VERBOSE);
                my $neg = $1 ? 1 : 0;
                my $name = $2;
                if ($aclevel == $iflevel) {
                    my $val = macro($name);
                    $used{$name} = $val;
                    if (!defined($val)) {
                        $undefMacros{$name} = 1;
                    }
                    my $doit = $val ? !$neg : $neg;
                    warn "if$1def for macro $name: $neg => $doit"
                        if (VERBOSE);
                    if ($doit) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                } elsif (VERBOSE) {
                    warn "inactive";
                }
                ++$iflevel;
                #print STDERR "IF $aclevel <= $iflevel : ifdef $_\n";
            } elsif (m/^else\s*+(.*+)/o) {
                warn "garbage at the end of an #else directive: $1" if ($1);
                if ($aclevel == $iflevel) {
                    --$aclevel;
                } elsif ($aclevel == $iflevel - 1
                         && !$iffound[$iflevel]) {
                    ++$aclevel;
                }
                #print STDERR "IF $aclevel <= $iflevel : else $_\n";
            } elsif (m/^endif\s*+(.*+)/o) {
                warn "garbage at the end of an #endif directive: $1" if ($1);
                --$aclevel if ($aclevel == $iflevel);
                --$iflevel;
                pop(@iffound);
                #print STDERR "IF $aclevel <= $iflevel : endif $_\n";
            } elsif ($iflevel == $aclevel) {
                if (m/^include(_next)?\s++(.*+)/o) {
                    my $next = $1;
                    $_ = $2;
                    my $tried;
                  RETRY:
                    my ($type, $name) = m/([<"])(.+)[">]/o;
                    if (!$name) {
                        if ($tried) {
                            warn "#include directive incorrect: $_";
                        } else {
                            my @inToks = tokenize;
                            my @toks = tokrep(0, $file, undef, %used, @inToks);
                            $_ = untokenize(@{toks});
                            $tried = 1;
                            goto RETRY;
                        }
                    } else {
                        $output .= flushOut(@ARG);
                        if ($name =~ m{($type|[']|[\\]|/[/*])}) {
                            warn "illegal character sequence $1 in file name: $name";
                        }
                        my @idirs =
                            ($type eq "<")
                            ? @sdirs
                            : (@dirs, @sdirs);
                        my ($recdef, $outret, $inret) =
                            $next
                            ? openfile(findfile($name, $file, @idirs))
                            : openfile(findfile($name, undef, @idirs));
                        push(@defines, @{$recdef});
                        $output .= $outret;
                        insertLine(@ARG, $file, $fd)  if (!NOLINE);
                        $input .= $inret;
                        $input .= $ARG[-1];
                    }
                } elsif (m/^define\s++(.*+)/o) {
                    $_ = $1;
                    my ($name, $rest) = m/^([a-zA-Z_]\w*+)(.*+)/o;
                    $rest //= "";
                    if ($name) {
                        my ($params, $dfn) = $rest =~ m/^\(([^()]*)\)\s*+(.*+)$/o;
                        $dfn //= "";
                        if ($params) {
                            $params =~ s/[.][.][.]\s*$/__VA_ARGS__/o;
                        }
                        my $definition =
                            defined($params)
                            ? (($params =~ m/^\s*+$/o)
                               ? [ "", $dfn ]
                               : [ split(/,\s*/o, $params), $dfn ])
                            : [ $rest ];
                        macroDefine($name, @{$definition}, @defines, %used);
                    } else {
                        warn "define directive without a name: $_"
                    }
                } elsif (m/^undef\s++([a-zA-Z_]\w*+)\s*+(.*+)/o) {
                    warn "garbage at end of undef for macro $1" if ($2);
                    macroUndefine($1, @defines);
                } elsif (m/^error\s++(.*+)/o) {
                    die "$file:$NR: error $1";
                } elsif (m/^pragma\s++(.*+)/o) {
                    $_ = $1;
                    s/(["\\])/\\$1/go;
                    $_ = "\"$_\"";
                    push(@ARG, handlePragma);
                } else {
                    push(@ARG, "# $_");
                }
            }
            ## Ensure that our line numbering stays close to the original
            push(@ARG, "\n");
        }
        ## End of preprocessing directives
        ############################################################
        elsif ($iflevel != $aclevel) {
            ## a blob that is protected by #if
            push(@ARG, "\n");
        } else {
            ## Glue a leading sequence of white space to a preceding one, if any.  This
            ## ensures that indentation stays about the same for the preprocessed file
            if (m/^(\s++)(.*+)$/o) {
                my $next = $1;
                $_ = $2;
                if (@ARG && $ARG[-1] =~ /^\s++$/so) {
                    $ARG[-1] .= $next;
                } else {
                    push(@ARG, $next);
                }
            }
            ## Trailing white space is no good.
            s/\s+$//o;
            ## Store this input line for eventual reuse when we see
            ## this file again.  We have to do this before macro
            ## replacement since the next time some macro might have
            ## changed and so the expansion will be different.
            $input .= $_."\n";
            if ($skipedLines) {
                insertLine(@ARG, $file, $fd) if (!NOLINE);
                $skipedLines = 0;
                $input .= $ARG[-1];
            }
            ## usage of macros in the C text itself are not accounted.
            my %used;
            push(@ARG, tokrep(0, $file, $fd, %used, escPre(tokenize)), "\n");
        }
    }
    warn "unbalanced #if / #endif" if ($iflevel);

    if ($pragmaOnce) {
        print STDERR "$file is only read once, enforced by #pragma\n";
        my %tmp;
        $usedMac{$file} = \%tmp;
        $fileHash{$file} = "";
        undef $pragmaOnce;
    } elsif (!@defines) {
        ## The file is candidate for content hashing
        if (!defined($usedMac{$file}) || !eqHashtables(%{$usedMac{$file}}, %used)) {
            $usedMac{$file} = \%used;
            if ($input =~ m/^(?:\s*+(?:#|%:)\s++\d++[^\n]++\n*+)*+$/so) {
                ## The file only contained line number information
                $input = "";
            }
            $fileHash{$file} = $input;
            warn "saved stripped contents of $file, length is ".length($input)
                if (VERBOSE);
            if (SHOWINCLUDES) {
                print STDERR "-" x $#files, " $file\n";
            }
        }
    } elsif (${$fileInc{$file}} > 1) {
        my %defines = map { $_ => 1 } @defines;
        @defines = sort keys %defines;
        print STDERR "$file can not be hashed, define changes on: @defines\n"
            if (VERBOSE);
    }

    if (scalar macroHidden()) {
        my $str = join(", ", macroHidden());
        warn "Macros still hidden at the end of input: $str";
    }
    $SIG{__WARN__} = $back;

  SHORTCUT:
    $output .= flushOut(@ARG);

    if ($output =~ m/^(?:\s*+(?:#|%:)\s++\d++[^\n]++\n*+)*+$/so) {
        ## The file only produced line number information
        $output = "";
    }

    pop(@files);
    return (\@defines, $output, $input);
}

sub escPre(@) {
    map {
        if ($_ eq "#") {
            $escHash;
        } elsif ($_ eq "##") {
            $escHash.$escHash;
        } elsif ($_ eq "%:") {
            $escHash2;
        } elsif ($_ eq "%:%:") {
            $escHash2.$escHash2;
        } else {
            $_;
        }
    } @ARG;
}

sub expandPar(@);

## This auxiliary supposes that the list is non empty and that each
## element is a reference to an array of tokens.
sub expandComma(@) {
    @ARG = map {
        (",", expandPar(@{$_}));
    } @ARG;
    splice(@ARG, 1);
}

## Receives a list of tokens that are either plain or of the form
## [[]...] where the outer array represents a parenthesized token
## sequence and the inner parts are token sequences that are separated
## by commas.
sub expandPar(@) {
    map {
        if (ref) {
            ("(", expandComma(@{$_}), ")");
        } else {
            ($_);
        }
    } @ARG;
}

sub unescPre(@) {
    my @outToks;
    @ARG = expandPar(@ARG);
  OUTER:
    while (@ARG) {
        my $_ = shift;
        if (length) {
            if (ord == UNSTRING) {
                $_ = shift;
                print STDERR "unstringing $_\n";
                s/^L?"(.*)"$/ $1/so;
                s/[\\](["\\])/$1/sog;
            } elsif (ord == ESCHASH) {
                $_ = "#".substr($_, 1);
            } elsif (ord == HASH) {
                $_ = "\n#".substr($_, 1);
            } elsif (ord == DQUOTE) {
                s/$escHash/#/go;
                s/\Q$escHash2\E/%:/go;
            } elsif (ord == QUOTE) {
                s/$escHash/#/go;
                s/\Q$escHash2\E/%:/go;
            } elsif (ord ==  ELL) {
                if (m/^L[\"\']/o) {
                    s/$escHash/#/go;
                    s/\Q$escHash2\E/%:/go;
                }
            }
            push(@outToks, $_);
        }
    }
    return @outToks;
}

sub countMeta(\@\%) {
    if (metaTok($ARG[0]->[0])) {
        my $_ = shift @{$ARG[0]};
        if (ord == INTERVALOPEN) {
            ## increment is magic on undef values.
            ++$ARG[1]->{substr($_, 1)};
        } elsif (ord == INTERVALCLOSE) {
            ## decrement isn't magic on undef values.
            $ARG[1]->{substr($_, 1)} = ($ARG[1]->{substr($_, 1)} // 0) - 1;
        } elsif (ord == NEWLINE) {
            ++$ARG[1]->{$_};
        }
        while ($_ = $ARG[0]->[0]) {
            if (ord == INTERVALOPEN) {
                ## increment is magic on undef values.
                ++$ARG[1]->{substr($_, 1)};
            } elsif (ord == INTERVALCLOSE) {
                ## decrement isn't magic on undef values.
                $ARG[1]->{substr($_, 1)} = ($ARG[1]->{substr($_, 1)} // 0) - 1;
            } elsif (ord == NEWLINE) {
                ++$ARG[1]->{$_};
            } else {
                last;
            }
            shift @{$ARG[0]};
        }
        1;
    }
}


sub compactMeta(\%) {
    my $hid = shift;
    $hid->{$_} //= 0 foreach (keys %{$hid}, "\n");
    @ARG = (
        [("\n") x delete $hid->{"\n"}],
        [map { ( $intervalClose.$_) x -$hid->{$_} } grep { $hid->{$_} < 0 } keys %{$hid}],
        [map { ( $intervalOpen.$_ ) x +$hid->{$_} } grep { $hid->{$_} > 0 } keys %{$hid}],
        );
    %{$hid} = ();
    @ARG;
}

sub outputMeta(\%\@) {
    my $hid = shift;
    $hid->{$_} //= 0 foreach (keys %{$hid}, "\n");
    push(@{$ARG[0]},
         ("\n") x delete $hid->{"\n"},
         map {
             if ($hid->{$_} < 0) {
                 ( $intervalClose.$_) x -delete $hid->{$_};
             } else {
                 ( $intervalOpen.$_ ) x +delete $hid->{$_};
             }
         }  keys %{$hid}
        );
}

## Join tokens surrounding a ## directive. These can only be real
## tokens or placeholders (no callback or newline) to we may just use
## normal shift operations.
sub joinToks(@) {
    my (@outToks, %hid, $nl, $cl, $op, $right, $joined, $meta, @ops);
    $meta = countMeta(@ARG, %hid);
    outputMeta(%hid, @outToks) if ($meta);
    while (@ARG) {
        ## This assumes that @ARG doesn't start with a meta token once we come here.
        my ($_, $meta) = (shift, countMeta(@ARG, %hid));
        if (@ARG && $ARG[0] eq "##") {
            $joined = $_;
            @ops = ($_);
          JOIN:
            ## just drop it
            shift;
            ## accumulate
            $meta += countMeta(@ARG, %hid);

            if (@ARG) {
                $joined .= $ARG[0];
                if (!length($joined)
                    || $joined =~ $tokenizerToken
                    || $joined =~ $punctToken
                    ) {
                    $_ = $joined;
                    push(@ops, shift);
                    if (@ARG && $ARG[0] eq "##") {
                        goto JOIN;
                    }
                    if ($#ops > 1) {
                        warn "Multiple join by the ## operator for \"$joined\" (@ops) may be implementation dependent."
                            if (($joined =~ m/\w/o) && ($joined =~ m/[-+]/o));
                    }
                } else {
                    warn "'$joined' is not a valid preprocessor token";
                }
            } else {
                warn "'$_ ##' at the end of a preprocessing directive";
            }
            ## If there are meta tokens in the range between the left and the right token of the
            ## join we move the closing hide intervals behind and the other ones in front.
            ($nl, $cl, $op) = compactMeta(%hid);
            push(@outToks, @{$nl}, @{$op}, $_, @{$cl});
            if (@ARG) {
                $meta = countMeta(@ARG, %hid);
                outputMeta(%hid, @outToks) if ($meta);
            }
        } else {
            push(@outToks, $_) if (length);
            outputMeta(%hid, @outToks) if ($meta);
        }
    }
    ## After all parameters in the replacement list have been substituted and # and ##
    ## processing has taken place, all placemarker preprocessing tokens are removed
    return @outToks;
}

sub getTokDef(\@) {
    if (!ref($ARG[0]->[-1])) {
        ## watch that %: and %:%: are translated to # and ## these are
        ## replaced later, anyhow, so the original spelling is not
        ## relevant for the preprocessor output. So we might as well
        ## speed up the replacement later.
        $ARG[0]->[-1] = [
            map {
                if (m/^$ishash$/o) {
                    "#";
                } elsif (m/^$ishhash$/o) {
                    "##";
                } else {
                    $_;
                }
            } tokenize($ARG[0]->[-1])
            ];
    }
    (@{$ARG[0]->[-1]});
}

## recursively replace macros in a list of tokens
## $level is the recursion level
## $file is the current input file name
## $fd is a file descriptor where we may get more tokens if we need them
## $used is a reference to hash that collects the macros that are used
## $inToks is a reference to the token list that is to be scanned
sub tokrep($$$\%@) {
    my ($level, $file, $fd, $used, $_, @outToks) = (shift, shift, shift, shift);
  LOOP:
    while (defined($_ = shift)) {
        if (INTERVALCLOSE >= ord && length) {
            tokCallback;
            push(@outToks, $_) if ($level);
        } elsif (ref) {
            ## An reference represents an parenthesized expression
            ## that already has been parsed. Do the token replacement
            ## recursively. This will not work if the replacement
            ## produces new parenthesis that are unbalanced on a
            ## particular level. In such a case we have to backtrack.
            my $backtrack;
            my @repl = map {
                my @ret = ([]);
                my $paren = 0;
                if (!$backtrack) {
                    foreach (tokrep($level + 1, $file, $fd, %{$used}, @{$_})) {
                        if (ord == COMMA) {
                            if (!$paren) {
                                $backtrack = 1;
                                last;
                            } else {
                                push(@ret, []);
                            }
                        } elsif (ord == PARENOPEN) {
                            ++$paren;
                            push(@{$ret[-1]}, $_);
                        } elsif (ord == PARENCLOSE) {
                            --$paren;
                            push(@{$ret[-1]}, $_);
                        } else {
                            push(@{$ret[-1]}, $_);
                        }
                    }
                }
                @ret;
            } @{$_};
            if ($backtrack) {
                warn "unbalanced parenthesis when replacing: backtracking";
                @repl = expandPar(($_));
                @repl = tokrep($level + 1, $file, $fd, %{$used}, @repl);
            }
            push(@outToks, \@repl);
        ## For the majority of the tokens that we pass through this, they will not define a macro. So
        ## we hide all the fancy stuff inside the if clause.
        } elsif (defined(macro)) {
            my $tokDef = macro;
            my @repl = getTokDef(@{$tokDef});
            ## The token corresponds to the name of an active macro
            $used->{$_} = $tokDef;
            if ($#{$tokDef}) {
                ## a function like macro
                my @metaToks;
                while ($ARG[0] && metaTok($ARG[0])) {
                    tokCallback($ARG[0]) if (INTERVALCLOSE >= ord($ARG[0]));
                    push(@metaToks, shift);
                }
                if (defined($ARG[0])) {
                    if ($ARG[0] eq "(") {
                        shift;
                        unshift(@ARG, parenRec(@ARG, $fd));
                    }
                }
                if (ref($ARG[0]) eq "ARRAY") {
                    ## insert the meta tokens that we found between the macro name and its parameter
                    ## list
                    push(@outToks, @metaToks) if ($level);

                    my @args = @{(shift)};
                    my $args = scalar @args;
                    my $defs = $#{$tokDef};

                    ## The empty argument is interpreted different. If a macro takes one argument
                    ## it is interpreted as the empty argument. If it takes zero arguments it is
                    ## the empty list.
                    if ($defs == 0) {
                        warn "macro $_ should receive 0 arguments, found @{$args[0]}"
                            if ($args
                                && ref($args[0]) == "ARRAY"
                                && @{$args[0]}
                                && (($args[0]->[0] ne "")
                                    || @{$args[0]} > 1));
                        $args = 0;
                    }

                    ## Before being substituted, each argument's preprocessing tokens are
                    ## completely macro replaced as if they formed the rest of the
                    ## preprocessing file; no other preprocessing tokens are available.
                    ##
                    ## Since this argument processing cannot have a side effect on the preprocessor
                    ## state, we only have to perform this token replacement for those arguments
                    ## that are effectively used. We have the list of the argument positions in the
                    ## hash %positions.
                    my @pos = (@{$positions{$_}});

                    ## The only valid case that the effective number of arguments can be greater
                    ## than in the definition is when the last argument is '...'. Add these
                    ## position s to the list if necessary.
                    if ($args > $defs && $counters{$_}->[$defs-1]) {
                        push(@pos, ($defs ... ($args-1)));
                    }
                    ## There is the following exception of parameter expansion:
                    ##
                    ### A parameter in the replacement list, unless preceded by a # or ##
                    ### preprocessing token or followed by a ## preprocessing token
                    ##
                    ## So if we know that we might need the unexpanded parameters we keep
                    ## them in a separate array.
                    my @pargs;
                    if ($joins{$_} || $stringifies{$_}) {
                        @pargs = (@args);
                        foreach my $i (@pos) {
                            ## however, if an argument consists of no preprocessing tokens, the
                            ## parameter is replaced by a placemarker preprocessing token instead.
                            push(@{$pargs[$i]}, "") if (ref($pargs[$i]) && !@{$pargs[$i]});
                        }
                    }
                    macroHide("_Pragma");
                    foreach my $i (@pos) {
                        $args[$i] = tokrep($level + 1, $file, $fd, %{$used}, @{$args[$i]})
                            if (ref($args[$i]) && @{$args[$i]});
                    }
                    macroUnhide("_Pragma");

                    ## If there is a ... in the identifier-list in the macro definition, then the
                    ## trailing arguments, including any separating comma preprocessing tokens, are
                    ## merged to form a single item: the variable arguments. The number of
                    ## arguments so combined is such that, following merger, the number of
                    ## arguments is one more than the number of parameters in the macro definition
                    ## (excluding the ...).
                    if ($args > $defs && $tokDef->[$defs - 1] eq "__VA_ARGS__") {
                        my @last = splice(@args, $defs-1);
                        my $last = shift(@last);
                        my @combined = ($last && ref($last)) ? @{$last} : ($last);
                        if (@last) {
                            push(@combined,
                                 map {
                                     if (ref eq "ARRAY") {
                                         (",", @{$_});
                                     } else {
                                         (",", $_);
                                     }
                                 } @last);
                        }
                        push(@args, \@combined);
                        $args = $defs;
                    }

                    if ($args == $defs) {
                        @repl = defined($stringifies{$_})
                            ? substituteArrayStringify(%{$argPosition{$_}}, @args, @pargs, @repl)
                            : substituteArray(%{$argPosition{$_}}, @args, @pargs, @repl);
                    } else  {
                        warn "macro $_ argument mismatch, has $args takes $defs";
                        warn "macro $_ definition is @{$tokDef}";
                        my $allargs = printArray(@args, "(,)[|](,)[|](,)[|](,)[|]");
                        warn "macro $_ received |$allargs|.";
                        unshift(@ARG,
                                map { @{$_} } @args
                            );
                        @repl = ($_);
                    }
                } else {
                    ## a is function like macro without ()
                    if ($level) {
                        push(@outToks, $_, @metaToks);
                    } else {
                        push(@outToks, $_);
                    }
                    next LOOP;
                }
            } else {
                ## an object like macro
            }
            ## For both object-like and function-like macro invocations, before the replacement list is
            ## reexamined for more macro names to replace, each instance of a ## preprocessing token in
            ## the replacement list (not from an argument) is deleted and the preceding preprocessing
            ## token is concatenated with the following preprocessing token.
            @repl = joinToks(@repl) if ($joins{$_});

            ## Then, the resulting preprocessing token sequence is rescanned, along with all subsequent
            ## preprocessing tokens of the source file, for more macro names to replace.

            if (@repl) {
                if ($#repl == 0 && length($repl[0]) == 0) {
                    push(@outToks,  "");
                } else {
                    ## If the name of the macro being replaced is found during this scan of the replacement
                    ## list (not including the rest of the source file's preprocessing tokens), it is not
                    ## replaced.
                    macroHide;
                    push(@outToks,  "$intervalOpen$_") if ($level);
                    unshift(@ARG, @repl, "$intervalClose$_");
                }
            }
        } else {
            ## handle the pseudo macros
            if ($_ eq "__LINE__") {
                ## This is the number of this line. We already
                ## advanced the counter.
                push(@outToks, $NR);
            } elsif ($_ eq "__FILE__") {
                push(@outToks, "\"$file\"");
            } elsif ($_ eq "_Pragma") {
                push(@outToks, handlePragma(shift));
            } elsif ($_ eq $liner) {
                ## This is the number of the following line
                insertLine(@outToks, $file) if (!NOLINE);
            } else {
                ## any other non macro token
                push(@outToks, $_);
            }
        }
    }
    #print STDERR "$level: returning ".printArray(@outToks)."\n";
    if (!$level && scalar macroHidden()) {
        my $str = join(", ", macroHidden());
        warn "Macros still hidden at the end of token replacement: $str";
    }
    return wantarray ? @outToks : \@outToks;
}


sub eqArrays(\@\@) {
    my ($a, $b) = @ARG;
    return 1 if (!defined($a) && !defined($b));
    return 0 if (defined($a) != defined($b));
    my ($na, $nb) = (scalar @{$a}, scalar @{$b});
    return 0 if ($na != $nb);
    for (my $i = 0; $i < $na; ++$i) {
        if (ref($a->[$i]) eq "ARRAY" && ref($b->[$i]) eq "ARRAY") {
            return 0 if (!eqArrays(@{$a->[$i]}, @{$b->[$i]}));
        } else {
            return 0 if ($a->[$i] ne $b->[$i]);
        }
    }
    return 1;
}

sub eqHashtables(\%\%) {
    my ($a, $b) = @ARG;
    return 1 if (!defined($a) && !defined($b));
    return 0 if (defined($a) != defined($b));
    return 0 if (scalar keys %{$a} != scalar keys %{$b});
    return containedHashtables(%{$a}, %{$b});
}


sub containedHashtables(\%\%) {
    my ($a, $b) = @ARG;
    ## a null reference is considered empty and is contained in any
    ## other hash
    return 1 if (!defined($a));
    ## an empty hash can only enclose another empty hash and nothing else
    return 0 if (!defined($b));
    ## to be a superset the cardinality must not be smaller
    return 0 if (scalar keys %{$a} > scalar keys %{$b});
    foreach my $key (keys %{$a}) {
        next if (!defined($a->{$key}) && !defined($b->{$key}));
        if (!defined($a->{$key})) {
            warn "$key has changed? value is undefined";
            return 0;
        }
        if (!defined($b->{$key}) || ($a->{$key} ne $b->{$key})) {
            warn "$key has changed: $a->{$key} ne $b->{$key}";
            warn "$key has changed: @{$a->{$key}}"
                if (ref($a->{$key}));
            warn "$key has changed: @{$b->{$key}}"
                if (ref($b->{$key}));
            return 0;
        }
    }
    return 1;
}


sub flushOut(\@) {
    my ($outTok) = @ARG;
    my $output = untokenize(unescPre(@{$outTok}));
    @{$outTok} = ();
    if (!GRAPHS) {
        $output = toktrans($output, $digraph, %digraph);
    } elsif (GRAPHS == 2) {
        $output = toktrans($output, $idgraph, %idgraph);
    } elsif (GRAPHS == 3) {
        $output = toktrans($output, $onegraph, %onegraph);
    }
    return compactLines($output);
}

sub compactLines(_) {
    @ARG = split(/\n/, shift, -1);
    my @lineS;
    while (@ARG) {
        my $_;
        while (defined($ARG[0]) && $ARG[0] =~ m/^\s*(?:#|%:)\s*(\d+)/o) {
            my $ln = $1;
            $_ = shift;
            while (defined($ARG[0]) && $ARG[0] =~ m/^\s*$/o) {
                ++$ln;
                shift;
            }
            s/^\s*(#|%:)\s*(?:\d+)(.*)$/$1 $ln$2/;
        }
        if (defined($_)) {
            push(@lineS, $_);
        } else {
            push(@lineS, shift);
        }
    }
    my $ln = 0;
    @ARG = map {
        if (m/^\s*$/o) {
            ++$ln;
            ();
        } elsif (m/^\s*(?:#|%:)\s*\d+/o) {
            $ln = 0;
            ($_);
        } else {
            my @tmp = (("") x $ln, $_);
            $ln = 0;
            (@tmp);
        }
    } @lineS;
    my $last;
    @ARG = map {
        if (m/^\s*(?:#|%:)\s*\d+/o) {
            $last = $_;
            ();
        } else {
            if ($last) {
                my @tmp = ("", $last, $_);
                undef $last;
                @tmp;
            } else {
                ($_);
            }
        }
    } @ARG;
    join("\n", @ARG, "");
}

## Recursively parse a parenthesis expression.
## This supposes that the opening parenthesis has already been seen.
## It returns an array of arrays:
## '(' 'a' ',' 'b' '(' 'c' ',' 'd' ')' ')'
## is transformed into
## [['a'], ['b', [['c'], ['d']]]]
sub parenRec(\@$) {
    my @args = ([]);
    my %hid;
  LOOP:
    while (@{$ARG[0]}) {
        my $_ = shift(@{$ARG[0]});
        if (ord == PARENOPEN) {
            $_ =  parenRec(@{$ARG[0]}, $ARG[1]);
            if (
                !NOSHORTCUT
                ## this parenthesis expression has no arguments
                ## separated by commas
                && $#{$_} == 0
                ## the token list in that part consists of exactly one
                ## element
                && $#{$_->[0]} == 0
                ## that element is again a parenthesis expression
                && ref($_->[0]->[0])) {
                $_ = $_->[0]->[0];
            }
            push(@{$args[-1]}, $_);
        } elsif (ord == PARENCLOSE) {
            if (%hid) {
                warn "parenthesis with unbalanced hide/unhide: ", join(", ", keys %hid), "\n";
            }
            return \@args;
        } elsif (ord == COMMA) {
            push(@args, []);
            ## The hiding intervals must be split along with commas.
            foreach (keys %hid) {
                if ($hid{$_} > 0) {
                    push(@{$args[-2]}, ($intervalClose.$_) x +$hid{$_});
                    push(@{$args[-1]}, ($intervalOpen.$_) x +$hid{$_});
                } else {
                    push(@{$args[-2]}, ($intervalClose.$_) x -$hid{$_});
                }
            }
        } elsif (ord == INTERVALOPEN) {
            my $name = substr($_, 1);
            ## increment of undef is magic
            ++$hid{$name};
            push(@{$args[-1]}, $_);
        } elsif (ord == INTERVALCLOSE) {
            my $name = substr($_, 1);
            ## decrement of undef is not magic
            $hid{$name} = ($hid{$name} // 0) - 1;
            delete $hid{$name} if (!$hid{$name});
            push(@{$args[-1]}, $_);
        } else {
            push(@{$args[-1]}, $_);
        }
    }
    if ($ARG[1]) {
        $skipedLines = 1;
        warn "expression may extend over several lines"
            if (VERBOSE);
        my %used;
        my @toks = tokrep(0, "", $ARG[1], %used, escPre(tokenize(logicalLine($ARG[1]))));
        if (@toks) {
            push(@{$ARG[0]}, @toks, "\n");
            goto LOOP;
        }
    }
    warn "unbalanced parenthesis";
    return \@args;
}


sub opRec(\@) {
    my $toks = shift;
    if ($#{$toks} > 0) {
        warn "comma expression in preprocessor directive";
        @{$toks} = ($toks->[-1]);
    }
    if (!NOSHORTCUT) {
        while ($#{$toks->[0]} == 0 && ref($toks->[0]->[0])) {
            $toks = $toks->[0]->[0];
        }
    }
    my $oplev = 0;
    my $npref = 1;
    my $pref = 0;
    my @proc =
        map {
            if (ref) {
                map {
                    if (defined($punctPri{$_})) {
                        if (!$npref) {
                            my $lev = $punctPri{$_};
                            $npref = 1;
                            $oplev = $lev if ($lev > $oplev);
                            ((")") x $lev, $_, ("(") x $lev);
                        } else {
                            ++$pref;
                            ($_, ("("));
                        }
                    } else {
                        $npref = 0;
                        if (ref) {
                            opRec(@{$_});
                        } else {
                            $_;
                        }
                    }
                } @{$_};
            } else {
                $_;
            }
    } @{$toks};
    push(@proc, (")") x ($oplev + $pref));
    unshift(@proc, ("(") x $oplev);
    #print STDERR "expr @proc\n";
    @{$toks} = (\@proc);
    return $toks;
}

foreach (@ARGV) {
    my ($defines, $ret) = openfile;
    print $out $ret;
}

my $dM = join("", @dM);
macroList() if ($dM =~ m/M/o);
macroList(1) if ($dM =~ m/U/o);
