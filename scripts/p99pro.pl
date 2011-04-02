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

no warnings 'portable';  # Support for 64-bit ints required
use English;
use POSIX;
use strict;
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

my $result = GetOptions (
    "include|I=s"        => \@{dirs},      # list of strings
    "define|D=s"        => \@{defs},      # list of strings
    "undef|U=s"        => \@{undefs},    # list of strings
    "dM!"        => \${show},    # flag
    "separator=s"        => \${sep},    # string
    "std|standard=s"        => \${std},    # string
    "graphs=i"        => \${graphs},
    );


## This is a special unstring operator. It is not meant to be used in C
## code but just to implement the unstringing of the arguments of
## _Pragma directives.
my $unstring = "%%";
## This is a special operator that serves to insert line information
## in the output.
my $liner = "??";
## This is a special operator that serves to insert a # character in the output
my $escHash = "\@\@";
## This is a special operator that serves to insert a %: sequence in the output
my $escHash2 = "//";

my %standards = (
    "c99" => "199901",
    );

## holds all defined macros
my %macro = (
    ## The special "macro" _Pragma that we never could do with #define.
    ## The spaces in the expansion are essential such that this is tokenized correctly.
    "_Pragma" => ["X", "$liner $escHash pragma ${unstring}X $liner"],
    ## Mark our presence by setting a special macro.
    "__P99PRO__" => ["1"],
    "__STDC_VERSION__" => [$standards{$std}],
    );

## per include file a hash of macros that were used by that file
my %usedMac;

## Per include file a hash of macros of tokens that are to be
## substituted for that file if the macros used by that file have not
## changed. This is only used when the file generates no side effects,
## that is, it has no active defines of its own.
my %fileHash;

## How many times have we processed this file.
my %fileInc;

## collects all tokens
my @tokensGlobal;

my $output = "";

## did we locally change line numbering?
my $skipedLines = 0;

## integrate the defines that we received on the command line
foreach my $def (@defs) {
    my ($name, $val) = $def =~ m/^([^=]+)=?(.*)/o;
    $val = "" if (!defined($val));
    warn "redefinition of macro $name" if (defined($macro{$name}));
    $macro{$name} = [$val];
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
my $punct = "(?:".join("|", @escPunct).")";
$punct = qr/$punct/;

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
my $isstring = qr/(?:L?"(?:[^"\\\\]++|\\\\.)*+")/;
## a regexp to detect character constants
my $ischar = qr/(?:L?'(?:[^'\\\\]++|\\\\.)*+')/;
## a regexp to detect preprocessor number token
my $isnumber = qr/(?:[.]?[0-9](?:[eEpP][-+]|[.a-zA-Z0-9]+)*)/;
## a regexp to detect preprocessor identifier token
my $isidentifier = qr/(?:[_a-zA-Z][_a-zA-Z0-9]+)/;

my $ishash = qr/(?:[#]|[%][:])/;
my $ishhash = qr/(?:[#][#]|[%][:][%][:])/;

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

sub compactLines($);
sub compList($\@);
sub containedHashtables(\%\%);
sub eqArrays(\@\@);
sub eqHashtables(\%\%);
sub escPre(@);
sub evalExpr($@);
sub expandDefined(\%@);
sub findfile($);
sub flushOut(\@);
sub openfile($;\%);
sub rawtokenize($);
sub readlln($\$);
sub readln($\$);
sub skipcomments($$\$);
sub tokenize($);
sub tokrep($$$\%@);
sub toktrans($$\%);
sub unescPre(@);
sub untokenize(@);

sub toktrans($$\%) {
    my ($inp, $reg, $hash) = @_;
    my %hash = %{$hash};
    my @tokens = map {
        if ($_ && $_ =~ m/$reg/) {
            $hash{$_}
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
        #print STDERR "processing \"$tok\"\n";
        if ($tok eq "(") {
            my @subexp = compList($level + 1, @{$toks});
            $type ||=  $subexp[0];
            push(@list, $subexp[1]);
        } elsif ($tok eq ")") {
            $level = 0;
            last LOOP;
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
sub readln($\$) {
    my ($fd, $lineno) = @_;
    my $line = <$fd>;
    ++${$lineno};
    return 0 if (!defined($line));
    chomp $line;
    my @chunks = split(/([?][?]$trigraph)/o, $line);
    @chunks = map {
        my $c = $_;
        if (m/^[?][?]($trigraph)$/o) {
            $c = $1;
            $c =~ tr@-!'/=()<>@~|^\\#[]{}@;
        }
        $c;
    } @chunks;
    return " ".join("", @chunks);
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
    my @ltok = split(/($isstring|$ischar|$isidentifier|$isnumber)/o, $ltok);
    #print STDERR join("|", @ltok), "\n";
    foreach my $ltok (@ltok) {
        if ($ltok !~ m/^$isstring|$ischar|$isidentifier|$isnumber$/o) {
            my @ltok = split(/\s+/o, $ltok);
            foreach my $ltok (@ltok) {
                my @ltok = split(/($punct)/, $ltok);
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
    my @toks = @_;
    my $ret = shift(@toks);
    my $prev = $ret;
    foreach my $tok (@toks) {
        my $comb = $prev . $tok;
        my $space = $sep;

        ## Don't add spaces before and after newlines
        if ($tok eq "\n") {
            $space = "";
        } elsif ($prev =~ m/\s$/so) {
            $space = "";

            ## imperative, don't create tokens that weren't there before
        } elsif ($comb =~ m/^$punct$/o) {
            $space = "$sep ";
        } elsif ($comb =~ m/^$isnumber$/o) {
            $space = "$sep ";
        } elsif ($comb =~ m/^$isidentifier$/o) {
            $space = "$sep ";

            ## this shouldn't occur, but might be important to
            ## visually mark a clash such as 199901L"my string" where
            ## the L could have been meant for making a long int or
            ## for creating a wide character string.
        } elsif ($tok =~ m/^$isstring|$ischar$/o && $prev =~ m/^$isidentifier|$isnumber$/o) {
            $space = "$sep ";

            ## visual enhancement for some single character tokens
        } elsif ($prev =~ m/^[=,;?:{]$/o) {
            $space = "$sep ";
        } elsif ($tok =~ m/^[=}]$/o) {
            $space = "$sep ";

            ## most two character tokens get spaces surrounding them
        } elsif ($prev =~ m/^$punct$/o && length($prev) > 1 && $prev !~ m/^[-][-]|[+][+]$/o) {
            $space = "$sep ";
        } elsif ($tok =~ m/^$punct$/o && length($tok) > 1 && $tok !~ m/^[-][-]|[+][+]$/o) {
            $space = "$sep ";
        }
        $ret .= $space . $tok;
        $prev = $tok;
    }
    return $ret;
}

sub readlln($\$) {
    my ($fd, $lineno) = @_;
    my $line = readln($fd, ${$lineno});
    while ($line =~ m/[\\]$/o) {
        $line = substr($line, 0, -1);
        $line .= readln($fd, ${$lineno});
        $skipedLines = 1;
    }
    return $line;
}

sub skipcomments($$\$) {
    my $ret = "";
    my ($line, $fd, $lineno) = @_;
    my @raw = rawtokenize($line);
  SCAN:
    while (@raw) {
        my $raw = shift(@raw);
        if ($raw =~ m/^["']/o) {
            $ret .= $raw;
        } else {
            if ($raw =~ m|(.*?)/([*/])(.*)|so) {
                $ret .= $1;
                my $ctyp = $2;
                if ($ctyp eq '/') {
                    @raw = ();
                } else {
                    my $rest = untokenize($3, @raw);
                    while ($rest !~ m|(.*?)[*]/(.*)|so) {
                        if ($fd && ($skipedLines = 1, my $cont = readlln($fd, ${$lineno}))) {
                            $rest .= " " . $cont;
                        } else {
                            warn "unterminated /* comment at end of input";
                            last SCAN;
                        }
                    }
                    my ($dummy, $fwd) = $rest =~ m|(.*?)[*]/(.*)|so;
                    @raw = rawtokenize($fwd);
                }
            } else {
                $ret .= $raw;
            }
        }
    }
    return $ret;
}

sub expandDefined(\%@) {
    my ($used, @toks) = @_;
    my @ret;
    while (@toks) {
        my $tok = shift(@toks);
        if ($tok eq "defined") {
            $tok = shift(@toks);
            if ($tok eq "(") {
                $tok = shift(@toks);
                warn "unbalanced \"defined\" expression near $tok"
                    if (shift(@toks) ne ")");
            }
            #print STDERR "searching for $tok\n";
            my $val = defined($macro{$tok});
            if ($val) {
                $used->{$tok} = $val;
                $tok = 1;
            } else {
                $tok = 0;
            }
        }
        push(@ret, $tok);
    }
    return @ret;
}

## Receives a filename and a reference to he used macros.
## Returns a tuple ($defines, \tokens).
sub openfile($;\%) {
    my ($file, $used) = shift;
    ## keep track of the number of defines in this file and below
    my $defines = 0;
    ## keep track of the tokens produced by this file and below
    my @tokens;
    my $output = "";

    my %used;
    $used = \%used if (!$used);

    if (defined($usedMac{$file})) {
        if (containedHashtables(%{$usedMac{$file}}, %macro)) {
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
    my $lineno = 0;

    ## Install a warning handler.
    my $back = $SIG{__WARN__};
    $SIG{__WARN__} = sub { print STDERR "$file:$lineno: warning: $_[0]"; };
    my @iffound;

    my $fd;
    if (!open($fd, "<$file")) {
        warn "couldn't open $file";
        return (1, []);
    }

    push(@tokens, "$escHash 1 \"$file\"", "\n");
    while (my $line = readlln($fd, $lineno)) {
        my $mult = 0;
        $line = skipcomments($line, $fd, $lineno);
        ############################################################
        ## First check if this is starting a preprocessing directive.
        if ($line =~ m/^\s*$ishash\s*(.*)/o) {
            $line = $1;
            $line =~ s/\s\s+/ /go;
            if ($line =~ m/^(el)?if\s+(.*)/o) {
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
                    my @toks = tokrep(0, $lineno, $file, %used, expandDefined(%used, tokenize($2)));
                    if (compList(0, @toks)) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                }
                #print STDERR "IF $aclevel <= $iflevel : (el)if $line\n";
            } elsif ($line =~ m/^if(n?)def\s+(\w+)/o) {
                if ($aclevel == $iflevel) {
                    my $val = defined($macro{$2});
                    $used{$2} = $val;
                    if ($1 xor $val) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                }
                ++$iflevel;
                #print STDERR "IF $aclevel <= $iflevel : ifdef $line\n";
            } elsif ($line =~ m/^else\s*(.*)/o) {
                warn "garbage at the end of an #else directive: $1" if ($1);
                if ($aclevel == $iflevel) {
                    --$aclevel;
                } elsif ($aclevel == $iflevel - 1
                         && !$iffound[$iflevel]) {
                    ++$aclevel;
                }
                #print STDERR "IF $aclevel <= $iflevel : else $line\n";
            } elsif ($line =~ m/^endif\s*(.*)/o) {
                warn "garbage at the end of an #endif directive: $1" if ($1);
                --$aclevel if ($aclevel == $iflevel);
                --$iflevel;
                pop(@iffound);
                #print STDERR "IF $aclevel <= $iflevel : endif $line\n";
            } elsif ($iflevel == $aclevel) {
                if ($line =~ m/^include\s+(.*)/o) {
                    $line = $1;
                    my $tried;
                  RETRY:
                    my ($type, $name) = $line =~ m/([<"])(.+)[">]/o;
                    if (!$name) {
                        if ($tried) {
                            warn "#include directive incorrect: $line";
                        } else {
                            my @toks = tokrep(0, $lineno, $file, %used, tokenize($line));
                            $line = untokenize(@toks);
                            $tried = 1;
                            goto RETRY;
                        }
                    } else {
                        $output .= flushOut(@tokens);
                        if ($name =~ m{($type|[']|[\\]|/[/*])}) {
                            warn "illegal character sequence $1 in file name: $name";
                        }
                        my ($recdef, $ret) = openfile(findfile($name));
                        $defines += $recdef;
                        $output .= $ret;
                        push(@tokens, "$escHash ".($lineno+2)." \"$file\"", "\n");
                    }
                } elsif ($line =~ m/^define\s+(.*)/o) {
                    $line = $1;
                    my ($name, $rest) = $1 =~ m/^([a-zA-Z_]\w*)(.*)/o;
                    if ($name) {
                        my ($params, $dfn) = $rest =~ m/^\(([^()]*)\)\s*(.*)$/o;
                        $params =~ s/[.][.][.]\s*$/__VA_ARGS__/o
                            if ($params);
                        my $definition =
                            defined($params)
                            ? (($params =~ m/^\s*$/o)
                               ? [ "", $dfn ]
                               : [ split(/,\s*/o, $params), $dfn ])
                            : [ $rest ];
                        # An identifier currently defined as an object-like macro
                        # shall not be redefined by another #define preprocessing
                        # directive unless the second definition is an
                        # object-like macro definition and the two replacement
                        # lists are identical. Likewise, ...
                        if (defined($macro{$name})) {
                            my $sep = int(rand(1000000));
                            my $old = join("|$sep|", @{$macro{$name}});
                            my $new = join("|$sep|", @{$definition});
                            if ($old ne $new) {
                                warn "redefinition of macro $name";
                                $macro{$name} = $definition;
                                ++$defines;
                            }
                        } else {
                            $macro{$name} = $definition;
                            ++$defines;
                        }
                    } else {
                        warn "define directive without a name: $line"
                    }
                } elsif ($line =~ m/^undef\s+([a-zA-Z_]\w*)\s*(.*)/o) {
                    warn "garbage at end of undef for macro $1" if ($2);
                    delete $macro{$1};
                    ++$defines;
                } elsif ($line =~ m/^error\s+(.*)/o) {
                    die "$file:$lineno: error $1";
                } elsif ($line =~ m/^pragma\s+message\s+(.*)/o) {
                    warn "message $1";
                } else {
                    push(@tokens, "# $line");
                }
            }
            ## Ensure that our line numbering stays close to the original
            push(@tokens, "\n");
        }
        ## End of preprocessing directives
        ############################################################
        elsif ($iflevel != $aclevel) {
            ## a blob that is protected by #if
            push(@tokens, "\n");
        } else {
            ## Glue a leading sequence of white space to a preceding one, if any.  This
            ## ensures that indentation stays about the same for the preprocessed file
            if ($line =~ m/^(\s+)(.*)$/o) {
                my $next = $1;
                $line = $2;
                if (@tokens && $tokens[$#tokens] =~ /^\s+$/so) {
                    $tokens[$#tokens] .= $next;
                } else {
                    push(@tokens, $next);
                }
            }
            ## Remaining white space is less significant.
            $line =~ s/\s\s+/ /go;
            ## Trailing white space is no good.
            $line =~ s/\s+$//o;
            if ($skipedLines) {
                push(@tokens, "$escHash $lineno \"$file\"", "\n");
                $skipedLines = 0;
            }
            push(@tokens, tokrep(0, $lineno, $file, %used, escPre(tokenize($line))), "\n");
        }
    }
    warn "unbalanced #if / #endif" if ($iflevel);

    $output .= flushOut(@tokens);
    $output = compactLines($output);

    if ($output =~ m/^\s*(?:#|%:)\s+(\d+).*\n*$/o) {
        ## The file only produced line number information
        $output = "";
    }

    if (!$defines) {
        print STDERR "$file is candidate for content hashing\n";
        if (!defined($usedMac{$file}) || !eqHashtables(%{$usedMac{$file}}, %used)) {
            $usedMac{$file} = \%used;
            $fileHash{$file} = $output;
        }
    }

    $SIG{__WARN__} = $back;

  SHORTCUT:
    if (defined($used)) {
        ## Only update the hash of used macros if we know that the
        ## upper level can use them. Otherwise we can simply empty it.
        if ($defines) {
            %{$used} = ();
        } else {
            my @used = %{$used};
            push(@used, %{$usedMac{$file}})
                if (defined($usedMac{$file}));
            %{$used} = @used;
        }
    }
    print STDERR "$file has been processed\n";
    return ($defines, $output);
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
    my @toks = @_;
    my @ret;
    while (@toks) {
        my $tok = shift(@toks);
        if ($tok eq "$unstring") {
            $tok = shift(@toks);
            $tok =~ s/^L?"(.*)"$/ $1/so;
            $tok =~ s/[\\](["\\])/$1/sog;
        }
        $tok =~ s/\Q$escHash\E/#/go;
        $tok =~ s/\Q$escHash2\E/%:/go;
        push(@ret, $tok);
    }
    return @ret;
}

sub tokrep($$$\%@) {
    my @ret;
    my ($replev, $lineno, $file, $used, @toks) = @_;
  LOOP:
    while (@toks) {
        my $tok;
      DEF:
        while (@toks) {
            $tok = shift(@toks);
            last DEF while (defined($tok) && length($tok));
            last LOOP if (!@toks);
        }
        ++$lineno if ($tok eq "\n");
        my @curr = ($tok);
        if (defined($tok) && defined($macro{$tok})) {
            $used->{$tok} = $macro{$tok};
            my @def = (@{$macro{$tok}});
            my @repl = tokenize(pop(@def));
            #print STDERR "$replev: $#def @def :: ".length($def[0])." :: ".join("|", @repl)."\n";
            my @exp;
            if (@def) {
                ## be careful with those that receive 0 arguments
                if ($def[0] && length($def[0])) {
                    ## a function like macro
                    my $next;
                    if (@toks) {
                        $next = shift(@toks);
                        ++$lineno if ($next eq "\n");
                        push(@curr, $next);
                    }
                    if (defined($next) && ($next eq "(")) {
                        my @args;
                        my $level = 1;
                        my @arg;
                        while ($level && @toks) {
                            my $tok = shift(@toks);
                            ++$lineno if ($tok eq "\n");
                            push(@curr, $tok);
                            if ($tok eq "," && $level == 1) {
                                my @cp = @arg;
                                push(@args, \@cp);
                                @arg = ();
                            } elsif ($tok eq ")") {
                                if ($level != 1) {
                                    push(@arg, $tok);
                                }
                                --$level;
                            } else {
                                if ($tok eq "(") {
                                    ++$level;
                                }
                                push(@arg, $tok);
                            }
                        }
                        push(@args, \@arg);
                        my $args = scalar @args;
                        my $defs = scalar @def;

                        ## Before being substituted, each argument's preprocessing tokens are
                        ## completely macro replaced as if they formed the rest of the
                        ## preprocessing file; no other preprocessing tokens are available.
                        foreach my $arg (@args) {
                            #print STDERR "$replev: argument before: ".join("|", @{$arg})."\n";
                            my @arg = tokrep($replev + 1, $lineno, $file, %{$used}, @{$arg});
                            # warn "level $replev: argument ".join("|", @arg)."++++".join(":", @{$arg})
                            #     if ($args < $defs);
                            ## however, if an argument consists of no preprocessing tokens, the
                            ## parameter is replaced by a placemarker preprocessing token
                            ## instead.
                            @arg = ("") if (!@arg);
                            $arg = \@arg;
                        }

                        if ($args < $defs) {
                            warn "not enough arguments for $tok: $args takes $defs";
                            warn "macro $tok expected arguments are @def.";
                            my $allargs = join("| ",
                                               map {
                                                   @{$_}
                                               } @args);
                            warn "received |$allargs|.";
                            unshift(@toks,
                                    map { @{$_} } @args
                                );
                            @repl = ($tok);
                        } else {
                            my %def;
                            for (my $i = 0; $i <= $#def; ++$i) {
                                $def{$def[$i]} = $i;
                            }
                            if ($def[$#def] eq "__VA_ARGS__") {
                                my @last = splice(@args, $#def);
                                #print STDERR "$replev: arguments @args, starting at $#def: @last\n";
                                my @combined = @{shift(@last)};
                                if (@last) {
                                    push(@combined, map { (",", @{$_}) } @last);
                                }
                                #print STDERR "arguments concated to @combined\n";
                                push(@args, \@combined);
                                # $def{"__VA_ARGS__"} = $def{"..."};
                                # delete $def{"..."};
                                $args = $defs;
                            }
                            if ($args > $defs) {
                                warn "too many arguments for $tok: $args takes $defs";
                                warn "macro $tok expected arguments are @def.";
                                my @allargs = map { @{$_} } @args;
                                my $allargs = join("| ", @allargs);
                                warn "received |$allargs|.";
                                unshift(@toks, @allargs);
                                @repl = ($tok);
                            } else {
                                my @exp;
                                #print STDERR "$replev: tokens before: ".join("|", @repl)."\n";

                                ## Do the stringification
                                while (@repl) {
                                    my $repl = shift(@repl);
                                    if ($repl =~ m/^$ishash$/o) {
                                        my $repl2 = shift(@repl);
                                        if (defined($def{$repl2})) {
                                            #print STDERR "stringifying argument $repl $repl2\n";
                                            my $str = untokenize(@{$args[$def{$repl2}]});

                                            ## White space before the first preprocessing token
                                            ## and after the last preprocessing token composing
                                            ## the argument is deleted.
                                            $str =~ s/^\s*(\w++)\s*$/$1/o;

                                            ## Each occurrence of white space between the
                                            ## argument's preprocessing tokens becomes a single
                                            ## space character in the character string literal.
                                            $str =~ s/^\s+//o;

                                            ## a \ character is inserted before each " and \
                                            ## character of a character constant or string
                                            ## literal
                                            $str =~ s/(["\\])/\\$1/go;

                                            push(@exp, '"'.$str.'"');
                                        } else {
                                            ## Each # preprocessing token in the replacement
                                            ## list for a function-like macro shall be followed
                                            ## by a parameter as the next preprocessing token
                                            ## in the replacement list.
                                            warn "invalid stringify sequence $repl $repl2, not a parameter name";
                                            unshift(@repl, $repl2);
                                            push(@exp, $repl);
                                        }
                                    } elsif (defined($def{$repl})) {
                                        # print STDERR "found argument $repl\n";
                                        push(@exp, @{$args[$def{$repl}]});
                                    } else {
                                        push(@exp, $repl);
                                    }
                                }
                                @repl = @exp;
                                #print STDERR "$replev: tokens after: ".join("|", @repl)."\n";
                            }
                        }
                    } else {
                        ## a is function like macro without ()
                        if (defined($next)) {
                            unshift(@toks, $next);
                            --$lineno if ($next eq "\n");
                            pop(@curr);
                        }
                        @repl = ($tok);
                    }
                } else {
                    my $olineno = $lineno;
                    ## a function like macro that expects 0 arguments
                    while (@toks && $toks[0] =~ m/^\s+$/so) {
                        ++$lineno if $toks[0] =~ m/\n/o;
                        shift(@toks);
                    }
                    if (@toks && ($toks[0] eq "(")) {
                        shift(@toks);
                        while (@toks && $toks[0] =~ m/^\s+$/so) {
                            ++$lineno if $toks[0] =~ m/\n/o;
                            shift(@toks);
                        }
                        if (!@toks) {
                            warn "missing closing parenthesis in call of $tok";
                        } elsif ($toks[0] ne ")") {
                            warn "$tok expects 0 arguments, found $toks[0]";
                            unshift(@toks, "(");
                        } else {
                            shift(@toks);
                        }
                    } else {
                        ## invocation without the parenthesis, ok
                        @repl = ($tok);
                    }
                    $skipedLines = 1 if ($olineno != $lineno);
                }
            } else {
                ## an object like macro
            }
            ## Join on ##
            @exp = ();
            while (@repl) {
                if ($#repl && $repl[1] =~ m/^$ishhash$/o) {
                    if ($#repl > 1) {
                        my @parts = splice(@repl, 0, 3);
                        my $joined = $parts[0] . $parts[2];
                        if ($joined =~ m/^$isstring|$ischar|$isidentifier|$isnumber|$punct$/o) {
                            push(@exp, $joined);
                        } else {
                            warn "'$joined' is not a valid preprocessor token";
                            push(@exp, $parts[0], $parts[2]);
                        }
                    } else {
                        warn "'$repl[0]$repl[1]' at the end of a preprocessing directive";
                        push(@exp, shift(@repl));
                        ## skip the hash hash
                        shift(@repl);
                    }
                } else {
                    push(@exp, shift(@repl));
                }
            }
            #print STDERR "$replev: tokens replacement1: ".join("|", @exp)."\n";
            ## After all parameters in the replacement list have been substituted and # and ##
            ## processing has taken place, all placemarker preprocessing tokens are removed
            @repl = grep { defined($_) && length($_) } @exp;
            #print STDERR "$replev: tokens replacement2: ".join("|", @repl)."\n";

            ## Switch of macro $tok for the recursive call
            my $backup = $macro{$tok};
            delete $macro{$tok};
            @repl = tokrep($replev + 1, $lineno, $file, %{$used}, @exp);
            $macro{$tok} = $backup;
            #@repl = grep { length($_) } @repl;
            #print STDERR "$replev: tokens replacement3: ".join("|", @repl)."\n";
            if (@repl) {
                if (eqArrays(@repl, @curr)) {
                    push(@ret, @repl);
                } else {
                    unshift(@toks, @repl);
                }
            }
        } else {
            if ($tok eq "__LINE__") {
                $tok = "$lineno";
            } elsif ($tok eq "__FILE__") {
                $tok = "\"$file\"";
            } elsif ($tok eq $liner) {
                $tok = "\n# $lineno \"$file\"\n";
            }
            push(@ret, $tok);
        }
    }
    return @ret;
}


sub eqArrays(\@\@) {
    my ($a, $b) = @_;
    return 1 if (!defined($a) && !defined($b));
    return 0 if (defined($a) != defined($b));
    my ($na, $nb) = (scalar @{$a}, scalar @{$b});
    return 0 if ($na != $nb);
    for (my $i = 0; $i < $na; ++$i) {
        return 0 if ($a->[$i] ne $b->[$i]);
    }
    return 1;
}

sub eqHashtables(\%\%) {
    my ($a, $b) = @_;
    return 1 if (!defined($a) && !defined($b));
    return 0 if (defined($a) != defined($b));
    my ($na, $nb) = (scalar keys %{$a}, scalar keys %{$b});
    if ($na != $nb) {
        print STDERR "hash table contents changed $na -> $nb\n";
        return 0;
    }
    return containedHashtables(%{$a}, %{$b});
}


sub containedHashtables(\%\%) {
    my ($a, $b) = @_;
    return 1 if (!defined($a) && !defined($b));
    return 0 if (defined($a) != defined($b));
    foreach my $key (scalar keys %{$a}) {
        if (!defined($a->{$key}) && !defined($a->{$key})) {
        } elsif (defined($a->{$key}) != defined($a->{$key})
                 || ($a->{$key} ne $b->{$key})) {
            print STDERR "value of $key changed\n";
            return 0;
        }
    }
    return 1;
}


sub flushOut(\@) {
    my ($tokens) = @_;
    my @tokens = grep { length($_) } @{$tokens};
    @{$tokens} = ();
    @tokens = unescPre(@tokens);
    my $output = untokenize(@tokens);
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

foreach my $file (@ARGV) {
    my ($defines, $ret) = openfile($file);
    $output .= $ret;
}

if ($show) {
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



print STDOUT $output;

