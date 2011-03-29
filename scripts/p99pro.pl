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
use Getopt::Long;
Getopt::Long::Configure ("bundling");

my @dirs = ("/usr/include");

my @defs;
my @undefs;
my $show;
my $sep = "";

my $result = GetOptions (
    "include|I=s"        => \@{dirs},      # list of strings
    "define|D=s"        => \@{defs},      # list of strings
    "undef|U=s"        => \@{undefs},    # list of strings
    "dM!"        => \${show},    # flag
    "separator=s"        => \${sep},    # string
    );


## This is a special unstring operator. It is not meant to be used in C
## code but just to implement the unstringing of the arguments of
## _Pragma directives.
my $unstring = "%%";
## This is a special operator that serves to insert line information
## in the output.
my $liner = "^^";

## holds all defined macros
my %macro = (
    ## The special "macro" _Pragma that we never could do with #define.
    ## The spaces in the expansion are essential such that this is tokenized correctly.
    "_Pragma" => ["X", "$liner #pragma ${unstring}X $liner"],
    ## Mark our presence by setting a special macro.
    "__P99PRO__" => ["1"],
    );

## collects all tokens
my @tokens;

## did we locally change line numbering?
my $skipedLines = 0;

foreach my $def (@defs) {
    my ($name, $val) = $def =~ m/^([^=]+)=?(.*)/o;
    $val = "" if (!defined($val));
    warn "redefinition of macro $name" if (defined($macro{$name}));
    $macro{$name} = [$val];
}


## all punctuation tokens. put the longest first, so they will match first.
my @punct = (
    "[%]:[%]:",

    "[.][.][.]",
    "[<][<]=",
    "[>][>]=",

    "[!]=",
    "[#][#]",
    "[%]:",
    "[%]=",
    "[%][>]",
    "[&][&]",
    "[&]=",
    "[*]=",
    "[+][+]",
    "[+]=",
    "[-][-]",
    "[-]=",
    "[-][>]",
    "[/]=",
    ":[>]",
    "[<][%]",
    "[<]:",
    "[<][<]",
    "[<]=",
    "==",
    "[>]=",
    "[>][>]",
    "^=",
    "[|]=",
    "[|][|]",
    $liner,
    $unstring,

    "[!]",
    "[#]",
    "[%]",
    "[&]",
    "[(]",
    "[)]",
    "[*]",
    "[+]",
    "[,]",
    "[-]",
    "[.]",
    "[/]",
    ":",
    "[;]",
    "[<]",
    "=",
    "[>]",
    "[?]",
    "^",
    "[{]",
    "[|]",
    "[}]",
    "[~]",
    "[\\[]",
    "[\\]]",
    );

## a regexp for all punctuation operators
my $punct = join("|", @punct);
## a regexp to detect string literals
my $isstring = '"(?:[^"\\\\]++|\\\\.)*+"';
## a regexp to detect character constants
my $ischar = "'(?:[^'\\\\]++|\\\\.)*+'";


## all third characters that may appear in a trigraph
my $trigraph = "-!'/=()<>";
## all replacements for trigraphs, in the corresponding order
my $onegraph = "~|^\\#[]{}";

sub readln($\$);
sub openfile($);
sub findfile($);
sub tokrep($$$@);
sub compList($\@);

my $big = ~0;
my $big2 = $big >> 1;

sub evalExpr($@) {
    my ($isUn, @list) = @_;
    my $res = eval("@list");
    $res = ($big - -$res +1) if ($isUn && $res < 0);
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
## and to evaluate the subexpressions to which they contribute with the
## correct modulus. Therefore this function returns a tuple. The first
## value states if this is an unsigned number (or not) and the second
## is the result of the computation.
sub compList($\@) {
    my ($level, $toks) = @_;
    #print STDERR "compList, $level: @{$toks}\n";
    my $isUn = 0;
    my @logic;
    my @list;
    while (@{$toks}) {
        my $tok = shift(@{$toks});
        #print STDERR "processing \"$tok\"\n";
        if ($tok eq "(") {
            my @subexp = compList($level + 1, @{$toks});
            $isUn ||=  $subexp[0];
            push(@list, $subexp[1]);
        } elsif ($tok eq ")") {
            goto FINISH;
        } else {
            if ($tok =~ m/^(0x)?[0-9]+[lL]*([uU]*)[lL]*$/o) {
                ## a number with an 'U' is unsigned
                $isUn ||= (defined($2) && length($2)) ? 1 : 0;
                $tok =~ s/[lLuU]//go;
                ## a number that is greater than INTMAX_MAX must be
                ## unsigned
                $isUn ||= ($tok > $big2) ? 1 : 0;
                #print STDERR "processing number \"$tok\"\n";
                push(@list, $tok);
            } elsif ($tok =~ m/^\w+/o) {
                push(@list, "0");
            } elsif ($tok =~ m/^&&|[|][|]$/o) {
                my $res = evalExpr($isUn, @list);
                $res = $res ? 1 : 0;
                push(@logic, $res, $tok);
                @list = ();
                $isUn = 0;
            } else {
                push(@list, $tok);
            }
        }
    }
    warn "preliminary end of regular expression, missing ')'" if ($level > 0);
  FINISH:
    warn "preliminary end of regular expression, missing operand" if (!@list);
    my $res = evalExpr($isUn, @list);
    if (@logic) {
        $res = $res ? 1 : 0;
        push(@logic, $res);
        $res = eval("@logic");
        $isUn = 0;
    }
    #print STDERR "compList $level: result is $res\n";
    return ($isUn, $res);
}

sub readln($\$) {
    my ($fd, $lineno) = @_;
    my $line = <$fd>;
    ++${$lineno};
    return 0 if (!defined($line));
    chomp $line;
    my $ret = " ";
    while ($line =~ m/^([^?]*)[?](.*)/o) {
        $ret .= $1;
        $line = $2;
        if ($line =~ m/^[?]([$trigraph])(.*)/o) {
            my $c = $1;
            $line = $2;
            $c =~ tr:-!'/=()<>:~|^\\#[]{}:;
            $ret .= $c;
        } else {
            $ret .= "?";
        }
    }
    $ret .= $line;
    return $ret;
}

sub findfile($) {
    my $name = shift;
    foreach my $dir (@dirs) {
        my $fname = "$dir/$name";
        print STDERR "looking for $fname\n";
        return $fname if (-e $fname);
    }
    return "./$name";
}

sub tokenize($) {
    my $ltok = shift;
    my @ret;
    ## first find strings
    my @ltok = split(/($isstring|$ischar)/, $ltok);
    foreach my $ltok (@ltok) {
        if ($ltok !~ m/^(["']).*\1$/o) {
            my @ltok = split(/[ \t]+/o, $ltok);
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
    my @toks = split(/($isstring|$ischar)/, $ltok);
    while (@toks) {
        my $toks = "";
      NONSTR:
        while (@toks) {
            my $tok = shift(@toks);
            if (length($tok) > 1) {
                #print STDERR "long: |$tok|\n";
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
        if ($tok eq "\n") {
            $ret .= $tok;
        } elsif ($prev =~ m/^($punct)$/o && $tok =~ m/^($punct)$/o && $comb =~ m/^($punct)$/o) {
            $ret .= " " . $tok;
        } elsif ($prev =~ m/\w$/o && $tok =~ m/^\w/o) {
            $ret .= " " . $tok;
        } else {
            $ret .= $sep . $tok;
        }
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
        #print STDERR "raw: @raw\n";
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
        if ($line =~ m/^[ \t]*(#|%:)[ \t]*(.*)/o) {
            $line = $2;
            $line =~ s/[ \t][ \t]+/ /go;
            #print STDERR "found directive $line\n";
            if ($line =~ m/^(el)?if[ \t]+(.*)/o) {
                my $ifcont = (defined($1) && (length($1) == 2)) ? 1 : 0;
                my $possible = 0;
                #print STDERR "possible: $possible, $ifcont\n";
                if ($ifcont) {
                    if ($aclevel == $iflevel) {
                        --$aclevel;
                    } elsif ($aclevel == $iflevel - 1
                             && !$iffound[$iflevel]) {
                        $possible = 1;
                    }
                } else {
                    if ($aclevel == $iflevel) {
                        $possible = 1;
                    }
                    $iffound[$iflevel + 1] = 0;
                }
                #print STDERR "possible: $possible\n";
                if ($possible) {
                    #print STDERR "eval: $2\n";
                    my @toks = tokrep(0, $lineno, $file, expandDefined(tokenize($2)));
                    #print STDERR "eval: @toks\n";
                    my @copy = (@toks);
                    my @res2 = compList(0, @copy);
                    my $res2 = $res2[1];
                    #print STDERR "eval: @toks  => $res2\n";
                    if ($res2) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    } else {
                        $iffound[$aclevel] = 0;
                    }
                }
                ++$iflevel if (!$ifcont);
                #print STDERR "IF $aclevel <= $iflevel : (el)if $line\n";
            } elsif ($line =~ m/^if(n?)def[ \t]+(\w+)/o) {
                if ($aclevel == $iflevel) {
                    my $ndef = length($1);
                    my $def = defined($macro{$2});
                    $def = $ndef ? !$def : $def;
                    if ($def) {
                        ++$aclevel;
                        $iffound[$aclevel] = 1;
                    } else {
                        $iffound[$aclevel] = 0;
                    }
                }
                ++$iflevel;
                #print STDERR "IF $aclevel <= $iflevel : ifdef $line\n";
            } elsif ($line =~ m/^else(.*)/o) {
                if ($aclevel == $iflevel) {
                    --$aclevel;
                } elsif ($aclevel == $iflevel - 1
                         && !$iffound[$iflevel]) {
                    ++$aclevel;
                    $iffound[$aclevel] = 1;
                }
                #print STDERR "IF $aclevel <= $iflevel : else $line\n";
            } elsif ($line =~ m/^endif[ \t]*(.*)/o) {
                --$aclevel if ($aclevel == $iflevel);
                --$iflevel;
                warn "garbage at the end of an #else directive: $1" if (length($1));
                #print STDERR "IF $aclevel <= $iflevel : endif $line\n";
            } elsif ($iflevel == $aclevel) {
                if ($line =~ m/^include[ \t]+(.*)/o) {
                    my @toks = tokrep(0, $lineno, $file, expandDefined(tokenize($1)));
                    $line = untokenize(@toks);
                    my $name;
                    if ($line =~ m/<(.+)>/o) {
                        $name = $1;
                    } elsif ($line =~ m/"(.+)"/o) {
                        $name = $1;
                    }
                    if (defined($name)) {
                        $name = findfile($name);
                        print STDERR "including file $name\n";
                        openfile($name);
                        print STDERR "end including file $name\n";
                    }
                } elsif ($line =~ m/^define[ \t]+(.*)/o) {
                    $line = $1;
                    #print STDERR "found define directive $line\n";
                    if ($line =~ m/^([a-zA-Z_]\w*)(.*)/o) {
                        $name = $1;
                        warn "redefinition of macro $name" if (defined($macro{$name}));
                        $line = $2;
                        #print STDERR "found define directive for $name to $line\n";
                        if ($line =~ m/^\([ \t]*([^()]*)[ \t]*\)[ \t]*(.*)/o) {
                            $params = $1;
                            $line = $2;
                            #print STDERR "found define directive for $name|$params| to $line\n";
                            $macro{$name} = [ split(/,[ \t]*/, $params), $line ];
                        } else {
                            $macro{$name} = [ $line ];
                        }
                    }
                } elsif ($line =~ m/^undef[ \t]+([a-zA-Z_]\w*)[ \t]*(.*)/o) {
                    my $name = $1;
                    warn "garbage at end of undef for macro $name" if (length($2));
                    delete $macro{$name};
                } elsif ($line =~ m/^error[ \t]+(.*)/o) {
                    die "$file:$lineno: Error $1";
                } elsif ($line =~ m/^pragma[ \t]+message[ \t]+(.*)/o) {
                    warn "$file:$lineno: message $1";
                } else {
                    push(@tokens, "#$line", "\n");
                }
            }
        }
        ## End of preprocessing directives
        ############################################################
        elsif ($iflevel != $aclevel) {
            ## a blob that is protected by #if
        } else {
            $line =~ s/[ \t][ \t]+/ /go;
            if ($skipedLines) {
                push(@tokens, "# $lineno \"$file\"\n");
                $skipedLines = 0;
            }
            push(@tokens, tokrep(0, $lineno, $file, escPre(tokenize($line))), "\n");
        }
    }
    warn "unbalanced #if / #endif" if ($iflevel);
}

sub escPre(@) {
    my @toks = @_;
    my @ret = ();
    while (@toks) {
        my $tok = shift(@toks);
        if ($tok eq "#") {
            $tok = "\20";
        } elsif ($tok eq "##") {
            $tok = "\21";
        } elsif ($tok eq "%:") {
            $tok = "\22";
        } elsif ($tok eq "%:%:") {
            $tok = "\23";
        }
        push(@ret, $tok);
    }
    return @ret;
}

sub unescPre(@) {
    my @toks = @_;
    my @ret = ();
    while (@toks) {
        my $tok = shift(@toks);
        if ($tok eq "$unstring") {
            #print STDERR "unesc @toks\n";
            $tok = shift(@toks);
            $tok =~ s/^L?"(.*)"$/$1/so;
            $tok =~ s/[\\](["\\])/$1/sog;
        }
        $tok =~ s/\20/#/go;
        $tok =~ s/\21/##/go;
        $tok =~ s/\22/%:/go;
        $tok =~ s/\23/%:%:/go;
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
                                if ($repl =~ m/^[#]$|^[%][:]$/o) {
                                    my $repl2 = shift(@repl);
                                    if (defined($def{$repl2})) {
                                        #print STDERR "stringifying argument $repl $repl2\n";
                                        my $str = untokenize(@{$args[$def{$repl2}]});
                                        $str =~ s/^[ \t]+//o;
                                        $str =~ s/[ \t]+$//o;
                                        $str =~ s/(["\\])/\\$1/go;
                                        push(@exp, '"'.$str.'"');
                                    } else {
                                        #print STDERR "not stringifying argument $repl $repl2\n";
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
            @exp = (shift(@repl));
            while (@repl) {
                my $repl = shift @repl;
                if ($repl =~ m/^##|%:%:$/o) {
                    $repl = shift @repl;
                    #print STDERR "token concated $exp[$#exp] and $repl\n";
                    $exp[$#exp] .= $repl;
                } else {
                    # print STDERR "token $repl\n";
                    push(@exp, $repl);
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

print STDOUT untokenize(@tokens);

