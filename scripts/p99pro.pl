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
use Getopt::Long;
Getopt::Long::Configure ("bundling");

## Variables that hold commandline arguments.
## include directories
my @dirs = ("/usr/include");
## defines
my @defs;
## undefines
my @undefs;
## the standard we are operating with
my $std = "c99";
## whether or not to output a list of all #define
my $show;
## use as a separator to tokens for debugging
my $sep = "";
## request output with special character sets: without digraphs (0),
## unchanged (1, default), digraphs (2) or trigraphs (3).
my $graphs = 1;
## indicate a hosted environment
my $hosted = 1;

my $result = GetOptions (
    "include|I=s"        => \@{dirs},      # list of strings
    "define|D=s"        => \@{defs},      # list of strings
    "undef|U=s"        => \@{undefs},    # list of strings
    "dM!"        => \${show},    # flag
    "hosted!"        => \${hosted},    # flag
    "separator=s"        => \${sep},    # string
    "std|standard=s"        => \${std},    # string
    "graphs=i"        => \${graphs},
    );


use constant NEWLINE => ord("\n");

use constant INTERVALOPEN => NEWLINE + 1;
my $intervalOpen = chr(INTERVALOPEN);

use constant INTERVALCLOSE => NEWLINE + 2;
my $intervalClose = chr(INTERVALCLOSE);

## This is a special unstring operator. It is not meant to be used in C
## code but just to implement the unstringing of the arguments of
## _Pragma directives.
use constant UNSTRING => NEWLINE + 3;
my $unstring = chr(UNSTRING);

## This is a special operator that serves to insert line information
## in the output.
use constant LINER => NEWLINE + 4;
my $liner = chr(LINER);

## This is a special operator that serves to insert a %: sequence in the output
use constant ESCHASH2 => NEWLINE + 5;
my $escHash2 = chr(ESCHASH2);

## This is a special operator that serves to insert a # character in the output
use constant ESCHASH => NEWLINE + 6;
my $escHash = chr(ESCHASH);

use constant COMMA => ord(",");
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
sub compactLines($);
sub containedHashtables(\%\%);
sub eqArrays(\@\@);
sub eqHashtables(\%\%);
sub escPre(@);
sub evalExpr($@);
sub expandDefined(\%@);
sub findfile($);
sub flushOut(\@);
sub getTokDef(\@);
sub isNL($);
sub joinToks(@);
sub macro($);
sub macroContained(\%);
sub macroDefine($\@;\@);
sub macroHide($);
sub macroHidden();
sub macroList();
sub macroUndefine($;\@);
sub macroUnhide($);
sub openfile($;\%);
sub parenRec(\@);
sub printArray(\@;$);
sub rawtokenize($);
sub readlln($);
sub readln($);
sub skipcomments($$);
sub substituteArray(\%\@@);
sub tokenize($);
sub tokrep($$\%@);
sub toktrans($$\%);
sub unescPre(@);
sub untokenize(@);

## Each entry holds a list of usage values for the arguments of the macro.
my %counters;
## Each entry holds a list of used arguments of the macro.
my %positions;
## Each entry holds a hash that gives the position for each macro argument name.
my %argPosition;

## holds all defined macros
{
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) =
        localtime(time);
    my @abbr = qw( Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec );

    my %macro = (
        ## The special "macro" _Pragma that we never could do with #define.
        ## The spaces in the expansion are essential such that this is tokenized correctly.
        "_Pragma" => ["X", [$liner, "#pragma", $unstring, "X", $liner]],
        ## Mark our presence by setting a special macro.
        "__P99PRO__" => ["1"],
        ## Provide the macros that are required by the standard. __LINE__ and __FILE__ are
        ## implemented differently.
        "__DATE__" => [sprintf("%s % 2u %u", $abbr[$mon], $mday, $year + 1900)],
        "__TIME__" => [sprintf("%02u:%02u:%02u", $hour, $min, $sec)],
        "__STDC_VERSION__" => [$standards{$std}],
        "__STDC__" => ["1"],
        "__STDC_HOSTED__" => ["$hosted"],
        ## None of these macro names, nor the identifier defined, shall be the subject of a
        ## #define or a #undef preprocessing directive.
        "defined" => ["(== abuse of keyword defined ==)"],
        );

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
    sub macro($) {
        ## values in %macro and %counters are references so they are either undef or non-zero.
        if ($macro{$_[0]}) {
            if ($counters{$_[0]}) {
                if (!$hidden{$_[0]}) {
                    $macro{$_[0]};
                } else {
                    undef;
                }
            } else {
                ## A macro can only be hidden if it has been evaluated at least once. So in this path
                ## we don't need to question hidden. In the contrary we can initialize it so we don't
                ## have to question if it is defined hereafter.
                $hidden{$_[0]} = 0;
                my $tokDef = $macro{$_[0]};
                my %def;
                for (my $i = 0; $i < $#{$tokDef}; ++$i) {
                    $def{$tokDef->[$i]} = $i;
                }
                $argPosition{$_[0]} = \%def;
                my @counts;
                my %pos;
                my @repl = getTokDef(@{$tokDef});
                foreach my $repl (@repl) {
                    if (defined($def{$repl})) {
                        my $i = $def{$repl};
                        $counts[$i] = ($counts[$i] // 0) + 1;
                        $pos{$i} = 1;
                    }
                }
                $counters{$_[0]} = \@counts;
                my @pos = sort { $a <=> $b } keys %pos;
                $positions{$_[0]} = \@pos;
                $tokDef;
            }
        }
    }


    sub macroDefine($\@;\@) {
        my ($name, $val, $defines) = @_;
        # An identifier currently defined as an object-like macro
        # shall not be redefined by another #define preprocessing
        # directive unless the second definition is an
        # object-like macro definition and the two replacement
        # lists are identical. Likewise, ...
        my $todo = !defined($macro{$name});
        if (!$todo) {
            my $sep = int(rand(1000000));
            my $old = join("|$sep|", @{$macro{$name}});
            my $new = join("|$sep|", @{$val});
            $todo = $old ne $new;
            warn "redefinition of macro $name"
                if ($todo);
        }
        if ($todo) {
            $macro{$name} = $val;
            push(@{$defines}, $name) if (defined($defines));
        }
    }

    sub macroUndefine($;\@) {
        my ($name, $defines) = @_;
        if (defined($macro{$name})) {
            delete $macro{$name};
            delete $counters{$name};
            delete $positions{$name};
            delete $argPosition{$name};
            push(@{$defines}, $name)
                if (defined($defines));
        }
    }

    sub macroHide($) {
        ## Pre-increment to be sure to always return a non-zero value.
        ## This is magic for undef values.
        ++$hidden{$_[0]};
    }

    sub macroUnhide($) {
        ## Post-increment to be sure to always return a non-zero value.
        ## This isn't magic for undef values, but there must always have been an increment before a
        ## decrement.
        $hidden{$_[0]}--;
    }

    sub macroHidden() {
        if (wantarray) {
            grep { $hidden{$_} } keys %hidden;
        } else {
            scalar grep { $_ } values %hidden;
        }
    }

    sub macroContained(\%) {
        my ($hash) = @_;
        return containedHashtables(%{$hash}, %macro)
    }

    sub macroList() {
        foreach my $name (sort keys %macro) {
            print STDERR "#define $name";
            my @def = @{$macro{$name}};
            my $repl = pop @def;
            if (@def) {
                print STDERR "(", join(", ", @def), ")";
            }
            print STDERR " $repl\n";
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


my $output = "";

## did we locally change line numbering?
my $skipedLines = 0;

## integrate the defines that we received on the command line
foreach my $def (@defs) {
    my ($name, $val) = $def =~ m/^([^=]++)=?+(.*+)/o;
    $val //= "";
    my @val = ($val);
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
        my @pairs = map { $_ => $i } $punctPri[$i];
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
my $isstringToken = qr/(?:L?".*")/;
## a regexp to detect character constants
my $ischar = qr/(?:L?'(?:[^'\\]|[\\].)*+')/;
my $ischarToken = qr/(?:L?'.*')/;
## a regexp to detect preprocessor number token
my $isnumber = qr/(?:[.]?+[0-9](?:[eEpP][-+]|[.a-zA-Z0-9]++)*+)/;
## a regexp to detect preprocessor identifier token
my $isidentifier = qr/(?:[_a-zA-Z][_a-zA-Z0-9]*+)/;

my $ishash = qr/(?:[#]|[%][:])/;
my $ishhash = qr/(?:[#][#]|[%][:][%][:])/;

my $tokenizer = qr/(?:$isstring|$ischar|$isidentifier|$isnumber)/;
my $tokenizerSplit = qr/($isstring|$ischar|$isidentifier|$isnumber)/;
my $tokenizerToken = qr/^$isstringToken|$ischarToken|$isidentifier|$isnumber$/;

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
    my ($outTok, $file, $fd) = @_;
    push(@{$outTok},
         "# ",
         $fd ? $fd->input_line_number() : $NR,
         " \"$file\"\n");
}

sub isNL($) {
    ord($_[0]) == NEWLINE;
}


sub printArray(\@;$) {
    my ($arg, $par) = @_;
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

sub substituteArray(\%\@@) {
    my ($def, $args) = (shift, shift);
    my @ret;
    my @counts;
    local $_;
    while (@_) {
        $_ = shift;
        if (defined($def->{$_})) {
            my $i = $def->{$_};
            push(@ret,
                 ## only clone an argument if and when it is used more than once
                 $counts[$i]
                 ? @{clone($args->[$i])}
                 : @{$args->[$i]}
                );
            ## The increment operation is magic on undef values.
            ++$counts[$i];
        } elsif ($_ eq "#") {
            if (defined($def->{$_[0]})) {
                #print STDERR "stringifying argument $_ $_[0]\n";
                $_ = untokenize(@{$args->[$def->{shift}]});

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
                warn "invalid stringify sequence $_ $_[0], not a parameter name";
                push(@ret, $_);
            }
        } else {
            push(@ret, $_);
        }
    }
    @ret;
}

## translate token sequences to digraph or trigraph representation
sub toktrans($$\%) {
    my ($inp, $reg, $hash) = @_;
    my @tokens = map {
        if ($_ && $_ =~ m/$reg/) {
            $hash->{$_}
        } else {
            $_
        }
    } split(/($reg)/so, $inp);
    return join("", @tokens);
}

local $SIG{__WARN__};

sub evalExpr($@) {
    my ($isUn, @list) = @_;
    my $back = $SIG{__WARN__};
    $SIG{__WARN__} = sub {
        &{$back}(@_);
        print STDERR "   expression is '@list'\n";
    };
    my $res = eval("@list");
    $res = (ULONG_MAX - -$res +1) while ($isUn && $res < 0);
    if (defined($res) && length($res) == 0) {
        $res = $res ? 1 : 0;
    }
    $SIG{__WARN__} = $back;
    return $res;
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
    my ($level, $toks) = @_;
    #print STDERR "compList, $level: @{$toks}\n";
    my $type = 0;
    my @logic;
    my @list;
  LOOP:
    while (@{$toks}) {
        my $tok = shift(@{$toks});
        #print STDERR "list @list, processing \"$tok\"\n";
        if ($tok eq "(") {
            my @subexp = compList($level + 1, @{$toks});
            $type ||=  $subexp[0];
            push(@list, $subexp[1]);
        } elsif ($tok eq ")") {
            $level = 0;
            last LOOP;
        } elsif (ord($tok) < ord(" ")) {
            next LOOP;
        } else {
            if ($tok =~ m/^(?:0[xX]?[0-9a-fA-F]|[0-9]+)[lL]*([uU]*)[lL]*$/o) {
                ## a number with an 'U' is unsigned
                $type ||= $1 ? 1 : 0;
                $tok =~ s/[lLuU]//go;
                ## a number that is greater than INTMAX_MAX must be
                ## unsigned
                $type ||= ($tok > LONG_MAX) ? 1 : 0;
                #print STDERR "processing number \"$tok\"\n";
                push(@list, $tok);
            } elsif ($tok =~ m/^\w+/o) {
                push(@list, "0");
            } elsif ($tok =~ m/^&&|[|][|]$/o) {
                my $back = $SIG{__WARN__};
                $SIG{__WARN__} = sub {
                    &{$back}(@_);
                    print STDERR "   expression is '@list'\n";
                };
                my $res = evalExpr($type, splice(@list));
                $SIG{__WARN__} = $back;
                $res = $res ? 1 : 0;
                push(@logic, $res, $tok);
                $type = 0;
            } else {
                push(@list, $tok);
            }
        }
    }
    warn "preliminary end of regular expression, missing ')'" if ($level > 0);
    warn "preliminary end of regular expression, missing operand" if (!@list);
    my $res = evalExpr($type, @list);
    if (@logic) {
        $res = $res ? 1 : 0;
        push(@logic, $res);
        my $back = $SIG{__WARN__};
        $SIG{__WARN__} = sub {
            &{$back}(@_);
            print STDERR "   expression is '@logic'\n";
        };
        $res = eval("@logic");
        $SIG{__WARN__} = $back;
        $type = 0;
    }
    #print STDERR "compList $level: result is $res\n";
    return wantarray ? ($type, $res) : $res;
}

## read one physical input line and perform the trigraph replacement
sub readln($) {
    my ($fd) = @_;
    local $_ = <$fd>;
    return 0 if (!defined);
    chomp;
    " ".join("",
             map {
                 if (length == 3
                     && m/^[?][?]($trigraph)$/o) {
                     $_ = $1;
                     tr@-!'/=()<>@~|^\\#[]{}@;
                 } else {
                     $_;
                 }
             }  split /([?][?]$trigraph)/o
        );
}

## Scan the list of include directories for a file
sub findfile($) {
    my $name = shift;
    foreach my $dir (@dirs) {
        my $fname = "$dir/$name";
        return $fname if (-e $fname);
    }
    return "./$name";
}

sub tokenize($) {
    my $ltok = shift;
    my @ret;
    ## first find strings
    #print STDERR "ltok $ltok\n";
    my @ltok = split($tokenizerSplit, $ltok);
    #print STDERR join("|", @ltok), "\n";
    foreach my $ltok (@ltok) {
        if ($ltok !~ $tokenizerToken) {
            my @ltok = split(/\s+/o, $ltok);
            foreach my $ltok (@ltok) {
                my @ltok = split($punctSplit, $ltok);
                push(@ret, @ltok);
            }
        } else {
            push(@ret, $ltok);
        }
    }
    return grep { length($_) } @ret;
}

sub rawtokenize($) {
    my @ret;
    my $ltok = shift;
    my @toks = split(/($isstring|$ischar)/o, $ltok);
    while (@toks) {
        my $toks = "";
      NONSTR:
        while (@toks) {
            my $tok = shift(@toks);
            if (length($tok) > 1) {
                push(@ret, $toks) if (length($toks));
                push(@ret, $tok);
                last NONSTR;
            }
            $toks .= $tok;
        }
    }
    return @ret;
}

sub untokenize(@) {
    @_ = grep { ord != INTERVALOPEN && ord != INTERVALCLOSE } @_;
    my $ret = shift;
    my $prev = $ret;
    foreach (@_) {
        my $space = $sep;
        my $comb = $prev . $_;
        ## Don't add spaces before and after newlines
        if (ord == NEWLINE) {
            $space = "";
        } elsif ($prev =~ m/\s$/so) {
            $space = "";

            ## imperative, don't create tokens that weren't there before
        } elsif ($comb =~ $punctToken) {
            $space = "$sep ";
        } elsif ($comb =~ m/^$isnumber$/o) {
            $space = "$sep ";
        } elsif ($comb =~ m/^$isidentifier$/o) {
            $space = "$sep ";

        } elsif (m/$punctToken/o && length > 1 && !m/^[-][-]|[+][+]$/o) {
            $space = "$sep ";
        } elsif (m/^[=}]$/o) {
            $space = "$sep ";

            ## this shouldn't occur, but might be important to
            ## visually mark a clash such as 199901L"my string" where
            ## the L could have been meant for making a long int or
            ## for creating a wide character string.
        } elsif (m/^$isstring|$ischar$/o && $prev =~ m/^$isidentifier|$isnumber$/o) {
            $space = "$sep ";

            ## most two character tokens get spaces surrounding them
        } elsif (length($prev) > 1 && $prev =~ $punctToken && $prev !~ m/^[-][-]|[+][+]$/o) {
            $space = "$sep ";

            ## visual enhancement for some single character tokens
        } elsif ($prev =~ m/^[=,;?:{]$/o) {
            $space = "$sep ";
        }
        $ret .= $space . $_;
        #print STDERR "untokenize step: $ret\n";
        $prev = $_;
    }
    #print STDERR "untokenize final: $ret\n";
    return $ret;
}

sub readlln($) {
    my $line = readln($_[0]);
    while (ord(substr($line, -1, 1)) == BSLASH) {
        $line = substr($line, 0, -1);
        $line .= readln($_[0]);
        $skipedLines = 1;
    }
    return $line;
}

sub skipcomments($$) {
    my $ret = "";
    my @raw = rawtokenize(shift);
    local $_;
  SCAN:
    while (@raw) {
        $_ = shift(@raw);
        if (m/^["']/o) {
            $ret .= $_;
        } else {
            if (m|(.*?)/([*/])(.*)|o) {
                $ret .= $1;
                if (ord($2) == SLASH) {
                    last SCAN;
                } else {
                    $_ = untokenize($3, @raw);
                    while (!m|(.*?)[*]/(.*)|o) {
                        if ($_[0] && ($skipedLines = 1, my $cont = readlln($_[0]))) {
                            $_ .= " " . $cont;
                        } else {
                            warn "unterminated /* comment at end of input";
                            last SCAN;
                        }
                    }
                    my ($fwd) = m|(?:.*?)[*]/(.*)|so;
                    @raw = rawtokenize($fwd);
                }
            } else {
                $ret .= $_;
            }
        }
    }
    $ret;
}

sub metaTok($) {
    ord($_[0]) >= NEWLINE
        && ord($_[0]) <= INTERVALCLOSE;
}

## Test if the token is a callback marker and run the corresponding hide or unhide function.  Returns
## some true value if a callback has been performed and a false value (undef) otherwise.
sub tokCallback($) {
    if (ord($_[0]) == INTERVALOPEN) {
        macroHide(substr($_[0], 1));
    } elsif (ord($_[0]) == INTERVALCLOSE) {
        macroUnhide(substr($_[0], 1));
    }
}

sub expandDefined(\%@) {
    my ($used, @inToks) = @_;
    my @outToks;
    while (@inToks) {
        my $tok = shift(@inToks);
        if ($tok eq "defined") {
            $tok = shift(@inToks);
            if ($tok eq "(") {
                $tok = shift(@inToks);
                warn "unbalanced \"defined\" expression near $tok"
                    if (shift(@inToks) ne ")");
            }
            #print STDERR "searching for $tok\n";
            my $val = macro($tok);
            if ($val) {
                $used->{$tok} = $val;
                $tok = 1;
            } else {
                $tok = 0;
            }
        }
        push(@outToks, $tok);
    }
    return @outToks;
}

## Receives a filename and a reference to he used macros.
## Returns a tuple ($defines, \tokens).
sub openfile($;\%) {
    my ($file, $used) = shift;
    ## keep track of the number of defines in this file and below
    my @defines;
    ## keep track of the tokens produced by this file and below
    my @outTok;
    my $output = "";

    my %used;
    $used = \%used if (!$used);

    if (defined($usedMac{$file})) {
        if (macroContained(%{$usedMac{$file}})) {
            print STDERR "reusing contents of $file\n";
            $output .= $fileHash{$file};
            goto SHORTCUT;
        } else {
            print STDERR "context for $file has changed, not reusing content.\n";
            delete $fileHash{$file};
            delete $usedMac{$file};
        }
    } else {
        if (!defined($fileInc{$file})) {
            my $dummy = 1;
            $fileInc{$file} = \$dummy;
        } else {
            ++${$fileInc{$file}};
        }
        print STDERR "$file must be processed (${$fileInc{$file}})\n";
    }

    my $iflevel = 0;
    my $aclevel = 0;

    my $fd;
    if (!open($fd, "<$file")) {
        warn "couldn't open $file";
        return ([], []);
    }

    ## Install a warning handler.
    my $back = $SIG{__WARN__};
    $SIG{__WARN__} = sub { print STDERR "$file:".$fd->input_line_number().": warning: $_[0]"; };
    my @iffound;

    push(@outTok, "# 1 \"$file\"\n");
    while (my $line = readlln($fd)) {
        my $mult = 0;
        $line = skipcomments($line, $fd);
        #print STDERR "line: "."." x $aclevel."!" x ($iflevel - $aclevel)."$line\n";
        ############################################################
        ## First check if this is starting a preprocessing directive.
        if ($line =~ m/^\s*+$ishash\s*+(.*+)/o) {
            if (scalar macroHidden()) {
                my $str = join(", ", macroHidden());
                warn "Macros hidden at the start of preprocessing directive: $str";
            }
            $skipedLines = 1;
            $line = $1;
            $line =~ s/\s\s+/ /go;
            if ($line =~ m/^(el)?+if\s++(.*+)/o) {
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
                    my @toks = tokrep(0, $file, %used, @expDef);
                    if (compList(0, @{toks})) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                }
                #print STDERR "IF $aclevel <= $iflevel : (el)if $line\n";
            } elsif ($line =~ m/^if(n?+)def\s++(\w++)/o) {
                if ($aclevel == $iflevel) {
                    my $val = macro($2);
                    $used{$2} = $val;
                    if ($1 xor $val) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                }
                ++$iflevel;
                #print STDERR "IF $aclevel <= $iflevel : ifdef $line\n";
            } elsif ($line =~ m/^else\s*+(.*+)/o) {
                warn "garbage at the end of an #else directive: $1" if ($1);
                if ($aclevel == $iflevel) {
                    --$aclevel;
                } elsif ($aclevel == $iflevel - 1
                         && !$iffound[$iflevel]) {
                    ++$aclevel;
                }
                #print STDERR "IF $aclevel <= $iflevel : else $line\n";
            } elsif ($line =~ m/^endif\s*+(.*+)/o) {
                warn "garbage at the end of an #endif directive: $1" if ($1);
                --$aclevel if ($aclevel == $iflevel);
                --$iflevel;
                pop(@iffound);
                #print STDERR "IF $aclevel <= $iflevel : endif $line\n";
            } elsif ($iflevel == $aclevel) {
                if ($line =~ m/^include\s++(.*+)/o) {
                    $line = $1;
                    my $tried;
                  RETRY:
                    my ($type, $name) = $line =~ m/([<"])(.+)[">]/o;
                    if (!$name) {
                        if ($tried) {
                            warn "#include directive incorrect: $line";
                        } else {
                            my @inToks = tokenize($line);
                            my @toks = tokrep(0, $file, %used, @inToks);
                            $line = untokenize(@{toks});
                            $tried = 1;
                            goto RETRY;
                        }
                    } else {
                        $output .= flushOut(@outTok);
                        if ($name =~ m{($type|[']|[\\]|/[/*])}) {
                            warn "illegal character sequence $1 in file name: $name";
                        }
                        my ($recdef, $ret) = openfile(findfile($name));
                        push(@defines, @{$recdef});
                        $output .= $ret;
                        insertLine(@outTok, $file, $fd);
                    }
                } elsif ($line =~ m/^define\s++(.*+)/o) {
                    $line = $1;
                    my ($name, $rest) = $1 =~ m/^([a-zA-Z_]\w*+)(.*+)/o;
                    $rest //= "";
                    if ($name) {
                        my ($params, $dfn) = $rest =~ m/^\(([^()]*)\)\s*+(.*+)$/o;
                        $dfn //= "";
                        if ($params) {
                            #print STDERR "params before $params\n";
                            $params =~ s/[.][.][.]\s*$/__VA_ARGS__/o;
                            #print STDERR "params after $params\n";
                        }
                        my $definition =
                            defined($params)
                            ? (($params =~ m/^\s*+$/o)
                               ? [ "", $dfn ]
                               : [ split(/,\s*/o, $params), $dfn ])
                            : [ $rest ];
                        macroDefine($name, @{$definition}, @defines);
                    } else {
                        warn "define directive without a name: $line"
                    }
                } elsif ($line =~ m/^undef\s++([a-zA-Z_]\w*+)\s*+(.*+)/o) {
                    warn "garbage at end of undef for macro $1" if ($2);
                    macroUndefine($1, @defines);
                } elsif ($line =~ m/^error\s++(.*+)/o) {
                    die "$file:$NR: error $1";
                } elsif ($line =~ m/^pragma\s++message\s+(.*+)/o) {
                    warn "message $1";
                } else {
                    push(@outTok, "# $line");
                }
            }
            ## Ensure that our line numbering stays close to the original
            push(@outTok, "\n");
        }
        ## End of preprocessing directives
        ############################################################
        elsif ($iflevel != $aclevel) {
            ## a blob that is protected by #if
            push(@outTok, "\n");
        } else {
            ## Glue a leading sequence of white space to a preceding one, if any.  This
            ## ensures that indentation stays about the same for the preprocessed file
            if ($line =~ m/^(\s++)(.*+)$/o) {
                my $next = $1;
                $line = $2;
                if (@outTok && $outTok[-1] =~ /^\s++$/so) {
                    $outTok[-1] .= $next;
                } else {
                    push(@outTok, $next);
                }
            }
            ## Trailing white space is no good.
            $line =~ s/\s+$//o;
            if ($skipedLines) {
                insertLine(@outTok, $file, $fd);
                $skipedLines = 0;
            }
            my @escPre = escPre(tokenize($line));
            push(@outTok, tokrep(0, $file, %used, @escPre), "\n");
        }
    }
    warn "unbalanced #if / #endif" if ($iflevel);

    $output .= flushOut(@outTok);
    $output = compactLines($output);

    if ($output =~ m/^\s*(?:#|%:)\s+(\d+).*\n*$/o) {
        ## The file only produced line number information
        $output = "";
    }

    if (!@defines) {
        print STDERR "$file is candidate for content hashing\n";
        if (!defined($usedMac{$file}) || !eqHashtables(%{$usedMac{$file}}, %used)) {
            $usedMac{$file} = \%used;
            $fileHash{$file} = $output;
        }
    } elsif (${$fileInc{$file}} > 1) {
        print STDERR "$file can not be hashed, define changes on: @defines\n";
    }

    if (scalar macroHidden()) {
        my $str = join(", ", macroHidden());
        warn "Macros still hidden at the end of input: $str";
    }
    $SIG{__WARN__} = $back;

  SHORTCUT:
    if (defined($used)) {
        ## Only update the hash of used macros if we know that the
        ## upper level can use them. Otherwise we can simply empty it.
        if (@defines) {
            %{$used} = ();
        } else {
            my @used = %{$used};
            push(@used, %{$usedMac{$file}})
                if (defined($usedMac{$file}));
            %{$used} = @used;
        }
    }
    print STDERR "$file has been processed\n";
    return (\@defines, $output);
}

sub escPre(@) {
    my @toks = @_;
    my @ret = ();
    while (@toks) {
        my $tok = shift(@toks);
        if ($tok eq "#") {
            $tok = $escHash;
        } elsif ($tok eq "##") {
            $tok = $escHash.$escHash;
        } elsif ($tok eq "%:") {
            $tok = $escHash2;
        } elsif ($tok eq "%:%:") {
            $tok = $escHash2.$escHash2;
        }
        push(@ret, $tok);
    }
    return @ret;
}


sub unescPre(@) {
    my @outToks;
    local $_;
  OUTER:
    while (@_) {
        $_ = shift;
        if (ref) {
            foreach (@{$_}) {
                unshift(@_, (ref($_) ? @{$_} : ($_)));
            }
            next OUTER;
        } elsif (!length) {
            next OUTER;
        }
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
    return @outToks;
}

sub compactMeta(@) {
    my %hid;
    foreach (@_) {
        if (ord == INTERVALOPEN) {
            $_ = substr($_, 1);
            ## increment is magic on undef values.
            ++$hid{$_};
            undef $_;
        } elsif (ord == INTERVALCLOSE) {
            $_ = substr($_, 1);
            ## decrement isn't magic on undef values.
            $hid{$_} = ($hid{$_} // 0) - 1;
            undef $_;
        }
    }
    (
     [grep { defined } @_],
     [map { ( $intervalClose.$_) x -$hid{$_} } grep { $hid{$_} < 0 } keys %hid],
     [map { ( $intervalOpen.$_ ) x +$hid{$_} } grep { $hid{$_} > 0 } keys %hid],
    );
}


## Join tokens surrounding a ## directive. These can only be real
## tokens or placeholders (no callback or newline) to we may just use
## normal shift operations.
sub joinToks(@) {
    my @outToks;
    while (@_) {
        my $left = shift;
        if (!metaTok($left)) {
            my @met;
            push (@met, shift) while (@_ && metaTok($_[0]));
            if (@_) {
                if (defined($_[0]) && $_[0] eq "##") {
                    ## just drop it
                    shift;
                    push (@met, shift) while (@_ && metaTok($_[0]));
                    my $right = shift;

                    ## If there are meta tokens in the range between the left and the right token of the
                    ## join we move the closing hide intervals behind and the other ones in front.
                    my ($met, $cl, $op) = compactMeta(@met);

                    if (defined($right)) {
                        my $joined = $left . $right;
                        if (!length($joined) || $joined =~ m/^$tokenizer|$punct$/o) {
                            push(@outToks, @{$met}, @{$op}, $joined, @{$cl});
                        } else {
                            warn "'$joined' is not a valid preprocessor token";
                            push(@outToks, @{$met}, $left, $right, @{$cl}, @{$op});
                        }
                    } else {
                        warn "'$left ##' at the end of a preprocessing directive";
                        push(@outToks, @{$met}, $left, @{$cl}, @{$op}) if (defined($left) && length($left));
                    }
                    next;
                }
            }
            push(@outToks, $left) if (length($left));
            if (@met) {
                my ($met, $cl, $op) = compactMeta(@met);
                push(@outToks, @{$met}, @{$cl}, @{$op});
            }
        } else {
            push(@outToks, $left) if (length($left));
        }
    }
    ## After all parameters in the replacement list have been substituted and # and ##
    ## processing has taken place, all placemarker preprocessing tokens are removed
    return @outToks;
}

sub getTokDef(\@) {
    if (ref($_[0]->[-1]) ne "ARRAY") {
        ## watch that %: and %:%: are translated to # and ## these are
        ## replaced later, anyhow, so the original spelling is not
        ## relevant for the preprocessor output. So we might as well
        ## speed up the replacement later.
        $_[0]->[-1] = [
            map {
                if (m/^$ishash$/o) {
                    "#";
                } elsif (m/^$ishhash$/o) {
                    "##";
                } else {
                    $_;
                }
            } tokenize($_[0]->[-1])
            ];
    }
    (@{$_[0]->[-1]});
}

## recursively replace macros in a list of tokens
## $level is the recursion level
## $file is the current input file name
## $used is a reference to hash that collects the macros that are used
## $inToks is a reference to the token list that is to be scanned
sub tokrep($$\%@) {
    my @outToks;
    my ($level, $file, $used) = (shift, shift, shift);
    @_ = grep { defined } @_;
  LOOP:
    while (@_) {
        if (tokCallback($_[0])) {
            push(@outToks, shift);
            next LOOP;
        }
        ## For the majority of the tokens that we pass through this, they will not define a macro. So
        ## we hide all the fancy stuff inside the if clause.
        if (defined(macro($_[0]))) {
            my $tok = shift;
            my $tokDef = macro($tok);
            my @repl = getTokDef(@{$tokDef});
            ## The token corresponds to the name of an active macro
            $used->{$tok} = $tokDef;
            if ($#{$tokDef}) {
                ## a function like macro
                my @metaToks;
                while (defined($_[0]) && metaTok($_[0])) {
                    tokCallback($_[0]);
                    push(@metaToks, shift);
                    #$next = $_[0];
                }
                if (defined($_[0])) {
                    if ($_[0] eq "(") {
                        shift;
                        unshift(@_, parenRec(@_));
                    }
                }
                if (ref($_[0]) eq "ARRAY") {
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
                        warn "macro $tok should receive 0 arguments, found @{$args[0]}"
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
                    my @pos = (@{$positions{$tok}});

                    ## The only valid case that the effective number of arguments can be greater
                    ## than in the definition is when the last argument is '...'. Add these
                    ## position s to the list if necessary.
                    if ($args > $defs && $counters{$tok}->[$defs-1]) {
                        push(@pos, ($defs ... ($args-1)));
                    }
                    macroHide("_Pragma");
                    foreach my $i (@pos) {
                        if (ref($args[$i])) {
                            if (@{$args[$i]}) {
                                $args[$i] = tokrep($level + 1, $file, %{$used}, @{$args[$i]});
                                if (@{$args[$i]}) {
                                    #$args[$i] = \@reparg;
                                    next;
                                }
                            }
                            ## however, if an argument consists of no preprocessing tokens, the
                            ## parameter is replaced by a placemarker preprocessing token instead.
                            $args[$i] = [""];
                        }
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
                        my @combined = $last && ref($last) ? @{$last} : ($last);
                        if (@last) {
                            push(@combined,
                                 map {
                                     if (ref($_) eq "ARRAY") {
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
                        @repl = substituteArray(%{$argPosition{$tok}}, @args, @repl);
                    } else  {
                        warn "macro $tok argument mismatch, has $args takes $defs";
                        warn "macro $tok definition is @{$tokDef}";
                        my $allargs = printArray(@args, "(,)[|](,)[|](,)[|](,)[|]");
                        warn "macro $tok received |$allargs|.";
                        unshift(@_,
                                map { @{$_} } @args
                            );
                        @repl = ($tok);
                    }
                } else {
                    ## a is function like macro without ()
                    if ($level) {
                        push(@outToks, $tok, @metaToks);
                    } else {
                        push(@outToks, $tok);
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
            @repl = joinToks(@repl);

            ## Then, the resulting preprocessing token sequence is rescanned, along with all subsequent
            ## preprocessing tokens of the source file, for more macro names to replace.

            if (@repl) {
                if ($#repl == 0 && length($repl[0]) == 0) {
                    push(@outToks,  "");
                } else {
                    ## If the name of the macro being replaced is found during this scan of the replacement
                    ## list (not including the rest of the source file’s preprocessing tokens), it is not
                    ## replaced.
                    tokCallback("$intervalOpen$tok");
                    push(@outToks,  "$intervalOpen$tok") if ($level);
                    unshift(@_, @repl, "$intervalClose$tok");
                }
            }
        } else {
            ## handle the pseudo macros
            if ($_[0] eq "__LINE__") {
                ## This is the number of this line. We already
                ## advanced the counter.
                push(@outToks, $NR);
            } elsif ($_[0] eq "__FILE__") {
                push(@outToks, "\"$file\"");
            } elsif ($_[0] eq "_Pragma") {
                push(@outToks, "#pragma");
            } elsif ($_[0] eq $liner) {
                ## This is the number of the following line
                insertLine(@outToks, $file);
            } else {
                ## any other non macro token
                push(@outToks, $_[0]);
            }
            shift;
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
    my ($a, $b) = @_;
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
    my ($a, $b) = @_;
    return 1 if (!defined($a) && !defined($b));
    return 0 if (defined($a) != defined($b));
    return 0 if (scalar keys %{$a} != scalar keys %{$b});
    return containedHashtables(%{$a}, %{$b});
}


sub containedHashtables(\%\%) {
    my ($a, $b) = @_;
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
    my ($outTok) = @_;
    my $output = untokenize(unescPre(@{$outTok}));
    if (!$graphs) {
        $output = toktrans($output, $digraph, %digraph);
    } elsif ($graphs == 2) {
        $output = toktrans($output, $idgraph, %idgraph);
    } elsif ($graphs == 3) {
        $output = toktrans($output, $onegraph, %onegraph);
    }
    return compactLines($output);
}

sub compactLines($) {
    my ($output) = @_;
    my @lines = split(/\n/, $output);

    $output = "";

    my @lineS;
    while (@lines) {
        my $lastTok;
        while (defined($lines[0]) && $lines[0] =~ m/^\s*(?:#|%:)\s+(\d+)/o) {
            my $ln = $1;
            $lastTok = shift(@lines);
            while (@lines && defined($lines[0]) && $lines[0] =~ m/^\s*$/o) {
                ++$ln;
                shift(@lines);
            }
            --$ln;
            $lastTok =~ s/^\s*(#|%:)\s+(?:\d+)(.*)$/$1 $ln$2/;
        }
        push(@lineS, $lastTok) if ($lastTok);
        push(@lineS, shift(@lines)) if (@lines);
    }

    $output = join("\n", @lineS, "");
    return $output;
}

## Recursively parse a parenthesis expression.
## This supposes that the opening parenthesis has already been seen.
## It returns an array of arrays:
## '(' 'a' ',' 'b' '(' 'c' ',' 'd' ')' ')'
## is transformed into
## [['a'], ['b', [['c'], ['d']]]]
sub parenRec(\@) {
    my @args = ([]);
    my %hid;
    local $_;
    while (@{$_[0]}) {
        $_ = shift(@{$_[0]});
        if (ord == PARENOPEN) {
            push(@{$args[-1]}, parenRec(@{$_[0]}));
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
    warn "unbalanced parenthesis";
    return \@args;
}

foreach my $file (@ARGV) {
    my ($defines, $ret) = openfile($file);
    $output .= $ret;
}

macroList() if ($show);



print STDOUT $output;

