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

## holds all defined macros
my %macro = (
    ## The special "macro" _Pragma that we never could do with #define.
    ## The spaces in the expansion are essential such that this is tokenized correctly.
    "_Pragma" => ["X", "$liner $escHash pragma ${unstring}X $liner"],
    ## Mark our presence by setting a special macro.
    "__P99PRO__" => ["1"],
    );

## collects all tokens
my @tokens;

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

sub compList($\@);
sub escPre(@);
sub evalExpr($@);
sub expandDefined(@);
sub findfile($);
sub openfile($);
sub rawtokenize($);
sub readlln($\$);
sub readln($\$);
sub skipcomments($$\$);
sub tokenize($);
sub tokrep($$$@);
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

sub evalExpr($@) {
    my ($isUn, @list) = @_;
    my $res = eval("@list");
    $res = (ULONG_MAX - -$res +1) while ($isUn && $res < 0);
    if (defined($res) && length($res) == 0) {
        $res = $res ? 1 : 0;
    }
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
                my $res = evalExpr($type, splice(@list));
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
        $res = eval("@logic");
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

sub expandDefined(@) {
    my @toks = @_;
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
            $tok = defined($macro{$tok}) ? 1 : 0;
        }
        push(@ret, $tok);
    }
    return @ret;
}

sub openfile($) {
    my $file = shift;
    my $fd;
    print STDERR "including file $file\n";
    if (!open($fd, "<$file")) {
        warn "couldn't open $file";
        return ();
    }
    my $iflevel = 0;
    my $aclevel = 0;
    my $lineno = 0;
    my @iffound;
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
                    my @toks = tokrep(0, $lineno, $file, expandDefined(tokenize($2)));
                    if (compList(0, @toks)) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    }
                }
                #print STDERR "IF $aclevel <= $iflevel : (el)if $line\n";
            } elsif ($line =~ m/^if(n?)def\s+(\w+)/o) {
                if ($aclevel == $iflevel) {
                    if ($1 xor defined($macro{$2})) {
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
                    my ($name) = $line =~ m/[<"](.+)[">]/o;
                    if (!$name) {
                        if ($tried) {
                            warn "#include directive incorrect: $line";
                        } else {
                            my @toks = tokrep(0, $lineno, $file, expandDefined(tokenize($line)));
                            $line = untokenize(@toks);
                            $tried = 1;
                            goto RETRY;
                        }
                    } else {
                        openfile(findfile($name));
                    }
                } elsif ($line =~ m/^define\s+(.*)/o) {
                    my ($name, $rest) = $1 =~ m/^([a-zA-Z_]\w*)(.*)/o;
                    if ($name) {
                        warn "redefinition of macro $name" if (defined($macro{$name}));
                        #print STDERR "found define directive for $name to $rest\n";
                        my ($params, $dfn) = $rest =~ m/^\(\s*([^()]*)\s*\)\s*(.*)/o;
                        $macro{$name} =
                            $params
                            ? [ split(/,\s*/o, $params), $dfn ]
                            : [ $rest ];
                    }
                } elsif ($line =~ m/^undef\s+([a-zA-Z_]\w*)\s*(.*)/o) {
                    warn "garbage at end of undef for macro $1" if ($2);
                    delete $macro{$1};
                } elsif ($line =~ m/^error\s+(.*)/o) {
                    die "$file:$lineno: error $1";
                } elsif ($line =~ m/^pragma\s+message\s+(.*)/o) {
                    warn "$file:$lineno: message $1";
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
                push(@tokens, "$escHash $lineno \"$file\"\n");
                $skipedLines = 0;
            }
            push(@tokens, tokrep(0, $lineno, $file, escPre(tokenize($line))), "\n");
        }
    }
    warn "unbalanced #if / #endif" if ($iflevel);
    print STDERR "end including file $file\n";
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

sub tokrep($$$@) {
    my @ret;
    my ($replev, $lineno, $file, @toks) = @_;
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
            my @def = (@{$macro{$tok}});
            my @repl = tokenize(pop(@def));
            #print STDERR "$replev: tokens orig: ".join("|", @repl)." @def\n";
            my @exp;
            if (@def) {
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
                        my @arg = tokrep($replev + 1, $lineno, $file, @{$arg});
                        #print STDERR "$replev: argument after: ".join("|", @arg)."\n";
                        ## however, if an argument consists of no preprocessing tokens, the
                        ## parameter is replaced by a placemarker preprocessing token
                        ## instead.
                        @arg = ("") if (!@arg);
                        $arg = \@arg;
                    }

                    if ($args < $defs) {
                        warn "macro $tok called with $args arguments, takes $defs";
                        unshift(@toks,
                                map { @{$_} } @args
                            );
                        @repl = ($tok);
                    } else {
                        my %def;
                        for (my $i = 0; $i <= $#def; ++$i) {
                            $def{$def[$i]} = $i;
                        }
                        if ($def[$#def] eq "...") {
                            my @last = splice(@args, $#def);
                            #print STDERR "$replev: arguments @args, starting at $#def: @last\n";
                            my @combined = @{shift(@last)};
                            if (@last) {
                                push(@combined, map { (",", @{$_}) } @last);
                            }
                            #print STDERR "arguments concated to @combined\n";
                            push(@args, \@combined);
                            $def{"__VA_ARGS__"} = $def{"..."};
                            delete $def{"..."};
                            $args = $defs;
                        }
                        if ($args > $defs) {
                            warn "macro $tok called with $args arguments, takes $defs";
                            unshift(@toks,
                                    map { @{$_} } @args
                                );
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
                                        ## argument’s preprocessing tokens becomes a single
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
                ## an object like macro
            }
            ## Join on ##
            @exp = (); #(shift(@repl));
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
            @repl = tokrep($replev + 1, $lineno, $file, @exp);
            $macro{$tok} = $backup;
            #@repl = grep { length($_) } @repl;
            #print STDERR "$replev: tokens replacement3: ".join("|", @repl)."\n";
            if (@repl) {
                if (untokenize(@repl) eq untokenize(@curr)) {
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


foreach my $file (@ARGV) {
    openfile($file);
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

@tokens = grep { length($_) } @tokens;
@tokens = unescPre(@tokens);
my $output = untokenize(@tokens);
if (!$graphs) {
    $output = toktrans($output, $digraph, %digraph);
} elsif ($graphs == 2) {
    $output = toktrans($output, $idgraph, %idgraph);
} elsif ($graphs == 3) {
    $output = toktrans($output, $onegraph, %onegraph);
}

print STDOUT $output;

