#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#
# Except of parts copied from previous work and as explicitly stated below,
# the author and copyright holder for this work is
# all rights reserved,  2010-2011 Jens Gustedt, INRIA, France
#
# This file is part of the P99 project. You received this file as as
# part of a confidential agreement and you may generally not
# redistribute it and/or modify it, unless under the terms as given in
# the file LICENSE.  It is distributed without any warranty; without
# even the implied warranty of merchantability or fitness for a
# particular purpose.
#

my $maxnumber = 159;

my @qualpar_C99
    = sort qw(const restrict volatile);

my @intpar_C99
    = sort qw(char int long short signed unsigned);

my @realpar2_C99
    = sort qw(double float);

my @realpar_C99
    = sort(qw(long), @realpar2_C99);

my @complpar_C99
    = qw(_Complex _Imaginary);

my @floatpar_C99
    = sort(@complpar_C99, @realpar_C99);

my @typepar_C99
    = sort(qw(_Bool void), @intpar_C99, @realpar2_C99, @complpar_C99, @qualpar_C99);


# these are the normative keywords in C99
my @keywords_C99
    = sort(qw(auto break case continue default do else enum extern for goto if
              inline register return sizeof static
              struct switch typedef union while),
           @typepar_C99);

# these are macros that are usually defined by C99
my @macros_C99
    = sort qw(
    bool true false
    NULL
    and and_eq
    bitand bitor
    compl
    not not_eq
    or or_eq
    xor xor_eq
    offsetof
);

# these are typedefs that are usually defined by C99
my @typedefs_C99
    = sort qw(
    clock_t
    div_t
    double_t
    fenv_t
    fexcept_t
    float_t
    fpos_t
    imaxdiv_t
    intmax_t
    intptr_t
    ldiv_t
    lldiv_t
    mbstate_t
    off_t
    ptrdiff_t
    sig_atomic_t
    size_t
    ssize_t
    time_t
    uintmax_t
    uintptr_t
    wchar_t
    wctrans_t
    wctype_t
    wint_t
    );

push(@typedefs_C99, "uint${_}_t", "int${_}_t", "uintleast${_}_t", "intleast${_}_t", "uintfast${_}_t", "intfast${_}_t")
    foreach (8, 16, 32, 64);

@typedefs_C99 = sort @typedefs_C99;


# these are functions and functional macros that are usually defined by C99
my @functions_C99 = sort(
    # assert.h
    qw(
        assert
    ),

    # complex.h
    qw(
        cabs
        cacos
        cacosh
        carg
        casin
        casinh
        catan
        catanh
        ccos
        ccosh
        cexp
        cimag
        clog
        conj
        cpow
        cproj
        creal
        csin
        csinh
        csqrt
        ctan
        ctanh
    ),

    # ctype.h
    qw(
        isalnum
        isalpha
        isblank
        iscntrl
        isdigit
        isgraph
        islower
        isprint
        ispunct
        isspace
        isupper
        isxdigit
        tolower
        toupper
    ),

    # inttypes.h
    qw(
        imaxabs
        imaxdiv
        strtoimax
        strtoumax
        wcstoimax
        wcstoumax
    ),

    # locale.h
    qw(
        localeconv
        setlocale
    ),

    # math.h
    qw(
        acos
        asin
        atan
        atan2
        atof
        ceil
        cos
        cosh
        exp
        fabs
        floor
        frexp
        ldexp
        log
        log10
        modf
        pow
        sin
        sinh
        sqrt
        tan
        tanh
    ),

    # netdb.h
    qw(
        getaddrinfo
        getnameinfo
    ),

    # setjmp.h
    qw(
        longjmp
        setjmp
    ),

    # signal.h
    qw(
        raise
    ),

    # stdarg.h
    qw(
        va_arg
        va_copy
        va_end
        va_start
    ),

    # stddef.h
    qw(
        offsetof
    ),

    # stdio.h
    qw(
        clearerr
        fclose
        feof
        ferror
        fflush
        fgetc
        fgetpos
        fgets
        fopen
        freopen
        fdopen
        fprintf
        fputc
        fputs
        fread
        fscanf
        fseek
        fsetpos
        ftell
        fwrite
        getc
        getchar
        gets
        perror
        printf
        sprintf
        snprintf
        putc
        putchar
        fputchar
        puts
        remove
        rename
        rewind
        scanf
        sscanf
        vfscanf
        vscanf
        vsscanf
        setbuf
        setvbuf
        tmpfile
        tmpnam
        ungetc
        vprintf
        vfprintf
        vsprintf
    ),

    # stdlib.h
    qw(
        _Exit
        abort
        abs
        labs
        atexit
        atoi
        atol
        bsearch
        div
        ldiv
        exit
        free
        getenv
        malloc
        calloc
        realloc
        qsort
        rand
        srand
        strtod
        strtol
        strtoul
        system
    ),

    # string.h
    qw(
        memchr
        memcmp
        memcpy
        memmove
        memset
        strcat
        strncat
        strchr
        strcmp
        strncmp
        strcoll
        strcpy
        strncpy
        strcspn
        strerror
        strlen
        strpbrk
        strrchr
        strspn
        strstr
        strtok
        strxfrm
    ),

    # time.h
    qw(
        asctime
        clock
        ctime
        difftime
        gmtime
        localtime
        mktime
        strftime
        time
    ),

    # wchar.h
    qw(
        btowc
        fgetwc
        fgetws
        fputwc
        fputws
        fwide
        fwprintf
        fwscanf
        getwc
        getwchar
        mbrlen
        mbrtowc
        mbsinit
        mbsrtowcs
        putwc
        putwchar
        swprintf
        swscanf
        ungetwc
        vfwprintf
        vswprintf
        vwprintf
        wcrtomb
        wcscat
        wcschr
        wcscmp
        wcscoll
        wcscpy
        wcscspn
        wcsftime
        wcslen
        wcsncat
        wcsncmp
        wcsncpy
        wcspbrk
        wcsrchr
        wcsrtombs
        wcsspn
        wcsstr
        wcstod
        wcstok
        wcstol
        wcstoul
        wcsxfrm
        wctob
        wmemchr
        wmemcmp
        wmemcpy
        wmemmove
        wmemset
        wprintf
        wscanf
    ),

    # wctype.h
    qw(
        iswalnum
        iswalpha
        iswcntrl
        iswctype
        iswdigit
        iswgraph
        iswlower
        iswprint
        iswpunct
        iswspace
        iswupper
        iswxdigit
        towctrans
        towlower
        towupper
        wctrans
        wctype
    )
    );

## this list here is not thought to be alphabetic. In particular
## compilers that are faked by other compilers should come at the end.
my @compilers = (
    "armcc" => "__ARMCC_VERSION",
    "borland" => "__BORLANDC__",
    "comeau" => "__COMO__",
    "cray" => "_CRAYC",
    "dec" => "__DECC_VER",
    "diab" => "__DCC__",
    "hp" => "__HP_cc",
    "ibm" => "__IBMC__",
    "intel" => "__INTEL_COMPILER",
    "kai" => "__KCC",
    "lcc" => "__LCC__",
    "metrowerks" => "__MWERKS__",
    "microsoft" => "_MSC_VER",
    "open64" => "__OPEN64__",
    "pcc" => "__PCC__",
    "portland" => "__PGI",
    "sgi" => "__sgi",
    "sun" => "__SUNPRO_C",
    "tinyc" => "__TINYC__",
    "watcom" => "__WATCOMC__",
    "apple" => "__apple_build_version__",
## apple must come before clang, because it is badly faking it
    "clang" => "__clang__",
## put gcc last, since clang in turn pretends to be gcc
    "gnu" => "__GNUC__",
    );

my @token_C99 = sort(@keywords_C99, @typedefs_C99, @functions_C99);

foreach my $i (8..63) {
    my $num = (1 << $i);
    push(@token_C99, $num)
        if ($num > $maxnumber);
}

my $fileid = '$Id$';

my ($FILEID) = $fileid =~ m/[\$]Id:[ ]*([^\$ ]+)/;

$FILEID //= "";

my $FILEDATE = `date -R`;
chomp $FILEDATE;

print "/*  -*- C -*- */\n/***********************************************\nautomat";
print "ically generated, do not chan";
print "ge manually.\n***********************************************/\n";
print "\n";
print "\n#ifndef P99_GENERATED_H\n#define P99_GENERATED_H\n\n";

my $maxnumber1 = $maxnumber - 1;

print << "DOCU";
/** \@file
 ** \@brief automatically generated macros to handle variadic macros.
 **
 ** You should probably never include this file directly but through
 ** other P99 header files that define macros that are of direct use.
 **/

/** \@brief The version of the P99 collection indicated by the commit date.
 **
 ** This should appear as an RFC2822 style date as you encounter e.g in emails.
 ** If this doesn't contain a date but a bizarre `format' with \$ signs
 ** you have a version that is directly taken from the git repostitory.
 **/
#define P99_VERSION_DATE "\$Format:\%cd\$"

/** \@brief The version of the P99 collection indicated by a unique commit ID.
 **
 ** This should appear as a long string of hexadecimal digits.
 ** If this contains but a bizarre "format" with \$ signs
 ** you have a version that is directly taken from the git repostitory.
 **/
#define P99_VERSION_ID "\$Format:\%H\$"

#define P00_VERSION_DATE "$FILEDATE"
#define P00_VERSION_ID "$FILEID"

/** \@brief The maximal number of arguments the P99 macros can handle.
 **
 ** This limit applies also to the integer arithmetic that is
 ** performed by the macros in \@ref preprocessor_arithmetic.
 **/
#define P99_MAX_NUMBER $maxnumber1
DOCU

print "#define P00_ARG(";
for (my $i = 1; $i <= $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print " _$i,";
    } else {
        print "\\\n _$i,";
    }
}
print "\\\n ...) _$maxnumber\n";

for (my $m = 1; $m < 5; ++$m) {
    print "#define P00_NARG_$m(...) P00_ARG(__VA_ARGS__, ";
    for (my $i = ($maxnumber - 1); $i >= 0; --$i) {
        my $val = ($i % $m) ? "P00_INV($m)" : ($i / $m);
        if ($i % 8 != 7) {
            print " $val,";
        } else {
            print "\\\n $val,";
        }
    }
    print ")\n";
}

print "/** \@ingroup basic_list_operations                                                 */\n";
print "/** \@brief Determine of the argument list has a comma, i.e at least two arguments. */\n";
print "#define P99_HAS_COMMA(...) P00_ARG(__VA_ARGS__,\\\n";
for (my $i = 2; $i < $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print " 1,";
    } else {
        print "\\\n 1,";
    }
}
print " 0, 0)\n";


for (my $arg = 2; $arg < $maxnumber; ++$arg) {
    my $arg1 = ${arg} - 1;
    print "#define P00_SKP${arg}(_0, ...) P00_SKP${arg1}(__VA_ARGS__)\n";
}

for (my $arg = 2; $arg < $maxnumber; ++$arg) {
    my $arg1 = ${arg} - 1;
    print "#define P00_PRE${arg}(_0, ...) _0, P00_PRE${arg1}(__VA_ARGS__)\n";
}

print "#define P00_ASCENDING() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print " $i,";
    } else {
        print "\\\n $i,";
    }
}
print STDOUT "\n";

print "#define P00_DESCENDING() ";
for (my $i = $maxnumber-1; $i >= 0; --$i) {
    if ($i % 8 != 0) {
        print " $i,";
    } else {
        print "\\\n $i,";
    }
}
print STDOUT "\n";

print "#define P00_ALL_ZEROES() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print " 0,";
    } else {
        print "\\\n 0,";
    }
}
print STDOUT "\n";

print "#define P00_ALL_ONES() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print " 1,";
    } else {
        print "\\\n 1,";
    }
}
print STDOUT "\n";

for (my $m = 7; $m < $maxnumber; ++$m) {
    my $m1 = $m - 1;
    print "/** \@brief Paste $m arguments at their boundary.*/\n";
    print "#define P99_PASTE$m(...) P00_PASTE$m(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))\n";
    print "#define P00_PASTE$m(L, ...) P99_PASTE2(P99_PASTE${m1}(__VA_ARGS__), L)\n";
}

for (my $m = 3; $m < $maxnumber; ++$m) {
    my $m1 = $m - 1;
    print "/** \@brief Paste $m partial identifiers separated by an underscore.*/\n";
    print "#define P99_PASTID$m(...) P00_PASTID$m(P99_ALLBUTLAST(__VA_ARGS__), P99_LAST(__VA_ARGS__))\n";
    print "#define P00_PASTID$m(S, L) P99_PASTID2(P99_PASTID${m1}(S), L)\n";
}

print <<"PREPRO1";
/**
 ** \@addtogroup preprocessor_text
 **
 ** \@brief These macros allow the handling of small texts and tokens.
 **
 ** For the following tokens, the macro ::P99_TOK_EQ will work.
 ** - decimal numbers: 0 ... $maxnumber
 ** - keywords:
PREPRO1

print " **     \@c ${_}\n"
    foreach (@keywords_C99);

print <<'PREPRO5';
 ** - typedefs:
PREPRO5

print " **     \@c ${_}\n"
    foreach (@typedefs_C99);

print <<'PREPRO6';
 ** - functions or functional macros:
PREPRO6

print " **     \@c ${_}\n"
    foreach (@functions_C99);

print <<'PREPRO4';
 **
 ** @warning Non-functional macros cannot be captured by this mechanism since they will
 ** be expanded before we may actually handle them. This concerns in particular
 ** the following tokens that are defined to be macros:
PREPRO4

print " **     \@c ${_}\n"
    foreach (@macros_C99);

print <<'PREPRO2';
 ** @{
 **/
PREPRO2


print "#define P00_IS_${_}_EQ_${_}(...) ,\n"
    foreach (0.. $maxnumber, @token_C99);
print "#define P00_TOK_${_}_STARTS_${_} ,\n"
    foreach (0.. $maxnumber, @token_C99);
print "/*! \@brief Test if the argument consists of exactly the token \@c ${_} */ \
#define P99_IS_EQ_${_}(...) P99_TOK_EQ(${_}, __VA_ARGS__)\n"
    foreach (0.. $maxnumber, @token_C99);
print "#define P00_EAT_${_} \n"
    foreach (0.. $maxnumber, @token_C99);
print "#define P00_TOKEN_${_} ,\n"
    foreach (0.. $maxnumber, @token_C99);
print "#define P00_QUALIFIER_${_} ,\n"
    foreach (@qualpar_C99);


print "#define P00_VOID_void ,\n";
print "#define P00_BOOL__Bool ,\n";
print "#define P00_BOOL_bool ,\n";
print "#define P00_INTEGER_${_} ,\n"
    foreach (@intpar_C99);
print "#define P00_REAL_${_} ,\n"
    foreach (@realpar_C99);
print "#define P00_FLOAT_${_} ,\n"
    foreach (@floatpar_C99);
print "#define P00_TYPE_${_} ,\n"
    foreach (@typepar_C99);


print "#define P00_VOID_QUAL_${_} ,\n"
    foreach ("void", @qualpar_C99);
print "#define P00_BOOL_QUAL_${_} ,\n"
    foreach ("_Bool", "bool", @qualpar_C99);
print "#define P00_INTEGER_QUAL_${_} ,\n"
    foreach (@intpar_C99, @qualpar_C99);
print "#define P00_REAL_QUAL_${_} ,\n"
    foreach (@realpar_C99, @qualpar_C99);
print "#define P00_FLOAT_QUAL_${_} ,\n"
    foreach (@floatpar_C99, @qualpar_C99);
print "#define P00_TYPE_QUAL_${_} ,\n"
    foreach (@typepar_C99);

print "#define P00_KEYWORD_${_} ,\n"
    foreach (@keywords_C99);
print "#define P00_RESERVED_${_} ,\n"
    foreach (@token_C99);

foreach my $kind (
    # the three different type qualifiers
    "CONST", "VOLATILE", "RESTRICT",
    # the unqualified type classifiers
    "BOOL", "VOID", "INTEGER", "REAL", "FLOAT", "TYPE",
    # the qualified type classifiers
    "BOOL_QUAL", "VOID_QUAL", "INTEGER_QUAL", "REAL_QUAL", "FLOAT_QUAL", "TYPE_QUAL",
    # general keyword and reserved words
    "KEYWORD", "RESERVED"
    ) {
    print << "KINDS";
#define P00_${kind}7(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(8)(7)
#define P00_${kind}6(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}7(P00_EAT_FIRST(SEQ)))(6)
#define P00_${kind}5(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}6(P00_EAT_FIRST(SEQ)))(5)
#define P00_${kind}4(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}5(P00_EAT_FIRST(SEQ)))(4)
#define P00_${kind}3(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}4(P00_EAT_FIRST(SEQ)))(3)
#define P00_${kind}2(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}3(P00_EAT_FIRST(SEQ)))(2)
#define P00_${kind}1(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}2(P00_EAT_FIRST(SEQ)))(1)
#define P00_${kind}_CLASSIFY_7(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ) P00_${kind}_CLASSIFY_6(P00_EAT_FIRST(SEQ))
#define P00_${kind}_CLASSIFY_6(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ) P00_${kind}_CLASSIFY_5(P00_EAT_FIRST(SEQ))
#define P00_${kind}_CLASSIFY_5(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ) P00_${kind}_CLASSIFY_4(P00_EAT_FIRST(SEQ))
#define P00_${kind}_CLASSIFY_4(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ) P00_${kind}_CLASSIFY_3(P00_EAT_FIRST(SEQ))
#define P00_${kind}_CLASSIFY_3(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ) P00_${kind}_CLASSIFY_2(P00_EAT_FIRST(SEQ))
#define P00_${kind}_CLASSIFY_2(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ) P00_${kind}_CLASSIFY_1(P00_EAT_FIRST(SEQ))
#define P00_${kind}_CLASSIFY_1(SEQ) P99_PASTE2(P00_${kind}_CLASSIFY_, SEQ)
#define P00_${kind}_CLASSIFY___(CODE) P99_IF_ELSE(P00_${kind}_VALIDATE(CODE))(P99_PASTE2(P00_${kind}_CLASSIFY__, CODE))(INVALID_${kind}_TYPE_EXPRESSION[CODE])
#define P00_${kind}_CLASSIFY__(N, ...) P00_${kind}_CLASSIFY___(P00_NARG(__VA_ARGS__))
#define P00_${kind}_CLASSIFY_(N, SEQ) P99_IF_ELSE(P99_IS_EQ_0(N))()(P00_${kind}_CLASSIFY__(N, P99_PASTE2(P00_${kind}_CLASSIFY_, N)(SEQ)))
/*! \@brief Classify the tokens in sequence \@a SEQ that all are of kind ${kind} */
#define P00_${kind}_CLASSIFY(SEQ) P00_${kind}_CLASSIFY_(P99_${kind}_QUAL_LEN(SEQ), SEQ)
/*! \@brief Check if \@a CODE corresponds to a valid combination of tokens of kind ${kind} */
#define P00_${kind}_VALIDATE(CODE) P99_IS_EQ_2(P99_NARG(P99_PASTE2(P00_${kind}_VALIDATE__, CODE)))
/*! \@brief Count the number of tokens in sequence \@a SEQ that all are of kind ${kind} */
#define P99_${kind}_LEN(SEQ) P99_IF_ELSE(P99_IS_${kind}_TOK(SEQ))(P00_${kind}1(P00_EAT_FIRST(SEQ)))(0)
/*! \@brief Test if the token sequence \@a SEQ starts with a token of kind ${kind} */
#define P99_IS_${kind}_TOK(SEQ) P99_HAS_COMMA(P99_PASTE2(P00_${kind}_, SEQ))
KINDS
}

# const is the only keyword that classifies as const qualifier
my @constClass = ("const");
# several occurrences just map to const again
my %constClass = (
    1 =>	"",
    2 =>	"const",
    3 =>	"const",
    4 =>	"const",
    5 =>	"const",
    6 =>	"const",
    7 =>	"const",
    );

# volatile is the only keyword that classifies as volatile qualifier
my @volatileClass = ("volatile");
# several occurrences just map to volatile again
my %volatileClass = (
    1 =>	"",
    2 =>	"volatile",
    3 =>	"volatile",
    4 =>	"volatile",
    5 =>	"volatile",
    6 =>	"volatile",
    7 =>	"volatile",
    );

# void is the only keyword that classifies as void type
my @voidClass = ("void");
# only allowed once
my %voidClass = (
    1 =>	"",
    2 =>	"v",
    );

# _Bool is the only keyword that classifies as _Bool type
# bool is only a macro and will be replaced, anyhow
my @boolClass = ("_Bool");
# only allowed once
my %boolClass = (
    1 =>	"",
    2 =>	"b",
    );

# several keyword that classify as integer types
my @integerClass = ("long", "", "short", "char", "signed", "unsigned", "int");
# they are allowed only once but long which might appear twice
my %integerClass = (
    2 => "l",
    3 => "ll",
    5 => "h",
    9 => "c",

    17 => "i",
    18 => "l",
    19 => "ll",
    21 => "h",
    25 => "hh",

    33 => "u",
    34 => "ul",
    35 => "ull",
    37 => "uh",
    41 => "uhh",

    65 => "i",
    66 => "l",
    67 => "ll",
    69 => "h",

    81 => "i",
    82 => "l",
    83 => "ll",
    85 => "h",

    97 => "u",
    98 => "ul",
    99 => "ull",
    101 => "uh",
    );

# several keyword that classify as float types
my @floatClass = ("long", "", "double", "", "float", "_Complex");
# they are allowed only once but long which might appear twice
my %floatClass = (
    5 => "d",
    6 => "ld",
    17 => "f",
    37 => "dc",
    38 => "ldc",
    49 => "fc",
    );

sub classify ($\@\%) {
    my ($Class, $toks, $keys) = (@_);
    my @toks = @$toks;
    my %keys = %{$keys};
    my $mult = 1;
    my $prev = "";
    foreach my $tok (@toks) {
        if ($tok) {
            print "#define P00_${Class}_CLASSIFY_${tok} /* ${mult} */ " . "," x $mult . "\n";
        } else {
            print STDERR "found empty token\n";
        }
        $mult *= 2;
    }
    foreach my $key (sort {$a <=> $b} keys %keys) {
        print "#define P00_${Class}_CLASSIFY__${key} " . $keys{$key}  . "\n";
        print "#define P00_${Class}_VALIDATE__${key} ,\n";
    }
}

classify("CONST", @constClass, %constClass);
classify("VOLATILE", @volatileClass, %volatileClass);
classify("VOID", @voidClass, %voidClass);
classify("BOOL", @boolClass, %boolClass);
classify("INTEGER", @integerClass, %integerClass);
classify("FLOAT", @floatClass, %floatClass);


my %builtinType = (
    "b" => "_Bool",
    "c" => "char",
    "h" => "signed short",
    "hh" => "signed char",
    "i" => "signed",
    "" => "signed",
    "l" => "signed long",
    "ll" => "signed long long",
    "u" => "unsigned",
    "uh" => "unsigned short",
    "uhh" => "unsigned char",
    "uc" => "unsigned char",
    "ul" => "unsigned long",
    "ull" => "unsigned long long",
    "j" => "intmax_t",
    "uj" => "uintmax_t",
    "uz" => "size_t",
    "t" => "ptrdiff_t",
    "i8" => "int_least8_t",
    "i16" => "int_least16_t",
    "i32" => "int_least32_t",
    "i64" => "int_least64_t",
    "i128" => "int_least128_t",
    "u8" => "uint_least8_t",
    "u16" => "uint_least16_t",
    "u32" => "uint_least32_t",
    "u64" => "uint_least64_t",
    "u128" => "uint_least128_t",
    "v"	=> "void",
    "d" => "double",
    "ld" => "long double",
    "f" => "float",
    "dc" => "double _Complex",
    "ldc" => "long double _Complex",
    "fc" => "float _Complex",
    );

my %builtinMax = (
    "b" => "1",
    "c" => "CHAR_MAX",
    "h" => "SHRT_MAX",
    "hh" => "SCHAR_MAX",
    "i" => "INT_MAX",
    "" => "INT_MAX",
    "l" => "LONG_MAX",
    "ll" => "LLONG_MAX",
    "u" => "UINT_MAX",
    "uh" => "USHRT_MAX",
    "uhh" => "UCHAR_MAX",
    "uc" => "UCHAR_MAX",
    "ul" => "ULONG_MAX",
    "ull" => "ULLONG_MAX",
    "j" => "INTMAX_MAX",
    "uj" => "UINTMAX_MAX",
    "uz" => "SIZE_MAX",
    "t" => "PTRDIFF_MAX",
    "j" => "INTMAX_MAX",
    "i8" => "INT8_MAX",
    "i16" => "INT16_MAX",
    "i32" => "INT32_MAX",
    "i64" => "INT64_MAX",
    "i128" => "INT128_MAX",
    "u8" => "UINT8_MAX",
    "u16" => "UINT16_MAX",
    "u32" => "UINT32_MAX",
    "u64" => "UINT64_MAX",
    "u128" => "UINT128_MAX",
    );

my %builtinMin = (
    "b" => "0",
    "c" => "CHAR_MIN",
    "h" => "SHRT_MIN",
    "hh" => "SCHAR_MIN",
    "i" => "INT_MIN",
    "" => "INT_MIN",
    "l" => "LONG_MIN",
    "ll" => "LLONG_MIN",
    "u" => "0",
    "uh" => "0",
    "uhh" => "0",
    "uc" => "0",
    "ul" => "0UL",
    "ull" => "0ULL",
    "j" => "INTMAX_MIN",
    "uj" => "0ULL",
    "uz" => "0",
    "t" => "PTRDIFF_MIN",
    "i8" => "INT8_MIN",
    "i16" => "INT16_MIN",
    "i32" => "INT32_MIN",
    "i64" => "INT64_MIN",
    "i128" => "INT128_MIN",
    "u8" => "UINT8_C(0)",
    "u16" => "UINT16_C(0)",
    "u32" => "UINT32_C(0)",
    "u64" => "UINT64_C(0)",
    "u128" => "UINT128_C(0)",
    );



my %integerRank = (
    "b" => 0,
    "c" => 1,
    "h" => 2,
    "hh" => 1,
    "i" => 3,
    "" => 3,
    "l" => 4,
    "ll" => 5,
    "u" => 3,
    "uh" => 2,
    "uhh" => 1,
    "ul" => 4,
    "ull" => 5,
    );

sub rank($) {
    my ($code) = @_;
    return
        defined($integerRank{$code})
        ? "$integerRank{$code}"
        : "";
}


my %integerSigned = (
    "b" => 0,
    "h" => 1,
    "hh" => 1,
    "i" => 1,
    "" => 1,
    "l" => 1,
    "ll" => 1,
    "u" => 0,
    "uh" => 0,
    "uhh" => 0,
    "ul" => 0,
    "ull" => 0,
    );

sub isSigned($) {
    my ($code) = @_;
    return
        defined($integerSigned{$code})
        ? "$integerSigned{$code}"
        : "";
}

my %integer2unsigned = (
    "0" => "b",
    "1" => "uhh",
    "2" => "uh",
    "3" => "u",
    "4" => "ul",
    "5" => "ull",
    "b" => "b",
    "c" => "uhh",
    "h" => "uh",
    "hh" => "uhh",
    "i" => "u",
    "" => "u",
    "l" => "ul",
    "ll" => "ull",
    "u" => "u",
    "uh" => "uh",
    "uhh" => "uhh",
    "ul" => "ul",
    "ull" => "ull",
    );

sub unsignedOf($) {
    my ($code) = @_;
    return
        defined($integer2unsigned{$code})
        ? "$integer2unsigned{$code}"
        : "";
}



my %integer2signed = (
    "1" => "hh",
    "2" => "h",
    "3" => "i",
    "4" => "l",
    "5" => "ll",
    "c" => "hh",
    "h" => "h",
    "hh" => "hh",
    "i" => "i",
    "" => "i",
    "ll" => "ll",
    "l" => "l",
    "u" => "i",
    "uh" => "h",
    "uhh" => "hh",
    "ul" => "l",
    "ull" => "ll",
    );

sub signedOf($) {
    my ($code) = @_;
    return
        defined($integer2signed{$code})
        ? "$integer2signed{$code}"
        : "";
}

sub printHash($\%) {
    my ($pref, $keys) = (@_);
    my %keys = %{$keys};
    print "#define P99_${pref}(...) P99_PASTE2(P00_, P99_PASTE(${pref}_, __VA_ARGS__))\n";
    print "#define P00_${pref}_${_} " . $keys{${_}} . "\n"
        foreach sort keys(%keys);
}

my $types = scalar keys %builtinType;
my %builtinTypeRev = reverse(%builtinType);

print << "BUILTIN0";
/**
 ** \@brief Find a builtin type according to a code such as `ull' or `ld'
 **
 ** E.g ::P99_BUILTIN_TYPE(ull) should expand to \@c unsigned \@c long
 ** \@c long. The complete list for the $types builtin types (+ required typedefs):
 ** <table>
 ** <tr><th>code</th><th>type</th><th>is signed</th><th>signed</th><th>unsigned</th><th>integer rank</th></tr>
BUILTIN0

foreach my $type (sort keys %builtinTypeRev) {
    my $code = $builtinTypeRev{$type};
    my $rank = rank($code);
    my $isSigned = isSigned($code);
    my $signed = signedOf($code);
    my $unsigned = unsignedOf($code);
    print " ** <tr><td>${code}</td><td><code>$type</code></td><td>${isSigned}</td><td>${signed}</td><td>${unsigned}</td><td>${rank}</td></tr>\n";
}

print << 'BUILTIN1';
 ** </table>
 **/
BUILTIN1

printHash("BUILTIN_TYPE", %builtinType);
printHash("BUILTIN_MAX", %builtinMax);
printHash("BUILTIN_MIN", %builtinMin);
printHash("INTEGER_RANK", %integerRank);
printHash("INTEGER_SIGN", %integerSigned);
printHash("INTEGER_SIGNED", %integer2signed);
printHash("INTEGER_UNSIGNED", %integer2unsigned);

print <<'PREPRO3';
/**
 ** @}
 **/
PREPRO3


printf "#define P00_PRED_%d %d\n", $_ + 1, $_
    foreach (0.. $maxnumber);
printf "#define P00_PRED_minus_%d minus_%d\n", $_, $_ + 1
    foreach (0.. $maxnumber);
printf "#define P00_minus_minus_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define P00_IS_%d_GE_0 ,\n", $_
    foreach (0.. $maxnumber);
printf "#define P00_dec_eval_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define P00_dec_eval_minus_%d %d\n", $_, -$_
    foreach (0.. $maxnumber);
print "#define P00_DUPL_${_}(...) __VA_ARGS__, P00_DUPL_", ($_ - 1), "(__VA_ARGS__)\n"
    foreach (2 .. $maxnumber);
print "#define P00_REP_${_}(...) P00_REP_", ($_ - 1), "(__VA_ARGS__)__VA_ARGS__\n"
    foreach (2 .. $maxnumber);
print "#define P00_BRACKET_${_}(X, ...) [X]P00_BRACKET_", ($_ - 1), "(__VA_ARGS__)\n"
    foreach (2 .. $maxnumber);
print "#define P00_BRACE_${_}(X, ...) {X}, P00_BRACE_", ($_ - 1), "(__VA_ARGS__)\n"
    foreach (2 .. $maxnumber);
print "#define P00_PAREN_${_}(X, ...) (X)P00_PAREN_", ($_ - 1), "(__VA_ARGS__)\n"
    foreach (2 .. $maxnumber);
print "#define P00_MAP_${_}(MAC, SEP, X, ...) MAC(X)P00_ROBUST SEP P00_MAP_", ($_ - 1), "(MAC, SEP, __VA_ARGS__)\n"
    foreach (2 .. $maxnumber);


for (my $i = 2; $i < $maxnumber; ++$i) {
    my $i1 = $i - 1;
    print "#define P00_FOR${i}(NAME, OP, FUNC, ...) \\\n",
    " OP(NAME, $i1, P00_FOR${i1}(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), $i1))\n";
}

print << 'MAXHEAD';

/* The preprocessor always computes with the precision of uintmax_t */
/* so for the preprocessor this is equivalent to UINITMAX_MAX       */
#define P00_UNSIGNED_MAX ~0u

#define P00_S0 0x01
#define P00_S1 0x02
#define P00_S2 0x04
#define P00_S3 0x08
#define P00_S4 0x10
#define P00_S5 0x20
#define P00_S6 0x40



/* This has to be such ugly #if/#else to ensure that the            */
/* preprocessor never sees a constant that is too large.            */
MAXHEAD

sub printnumber {
    my @arg = @_;
    my $ret = "";
    my $started = 0;
    while (@arg) {
        my $n = pop @arg;
        if ($started) {
            if ($n > 0xFFFFFFF) {
                $ret .= sprintf("%X", $n);
            } else {
                $ret .= sprintf("%08X", $n);
            }
        } elsif ($n) {
            $started = 1;
            $ret .= sprintf("%X", $n);
        }
    }
    $ret = "0" if (!$started);
    $ret;
}

{
    my @maxval = ( 0xFFFFFFFF, 0xFFFFFFFF, 0, 0 );

    my @B0 = ( 0xAAAAAAAA, 0xAAAAAAAA, 0, 0);
    my @B1 = ( 0xCCCCCCCC, 0xCCCCCCCC, 0, 0);
    my @B2 = ( 0xF0F0F0F0, 0xF0F0F0F0, 0, 0);
    my @B3 = ( 0xFF00FF00, 0xFF00FF00, 0, 0);
    my @B4 = ( 0xFFFF0000, 0xFFFF0000, 0, 0);
    my @B5 = ( 0x00000000, 0xFFFFFFFF, 0, 0);
    my @B6 = ( 0x00000000, 0x00000000, 0, 0);

    sub advance($$) {
        my ($pos, $width) = @_;
        my $relwidth = $width % 32;
        $maxval[$pos] = ($maxval[$pos] << 1) + 1;
        $B0[$pos] |= (!!($width & 0x01) << $relwidth);
        $B1[$pos] |= (!!($width & 0x02) << $relwidth);
        $B2[$pos] |= (!!($width & 0x04) << $relwidth);
        $B3[$pos] |= (!!($width & 0x08) << $relwidth);
        $B4[$pos] |= (!!($width & 0x10) << $relwidth);
        $B5[$pos] |= (!!($width & 0x20) << $relwidth);
        $B6[$pos] |= (!!($width & 0x40) << $relwidth);
    }

    sub printout($) {
        my ($width) = @_;
        print "#ifndef P99_UINTMAX_MAX\n";
        printf "# define P00_MASK_%u 0x%s\n", $width, printnumber(@maxval);
        printf "# if P00_UNSIGNED_MAX == 0x%s\n", printnumber(@maxval);
        printf "#  define P99_UINTMAX_WIDTH %u\n", $width;
        printf "#  define P99_UINTMAX_MAX 0x%sU\n", printnumber(@maxval);
        printf "#  define P00_B0 0x%sU\n", printnumber(@B0);
        printf "#  define P00_B1 0x%sU\n", printnumber(@B1);
        printf "#  define P00_B2 0x%sU\n", printnumber(@B2);
        printf "#  define P00_B3 0x%sU\n", printnumber(@B3);
        printf "#  define P00_B4 0x%sU\n", printnumber(@B4);
        printf "#  define P00_B5 0x%sU\n", printnumber(@B5);
        printf "#  define P00_B6 0x%sU\n", printnumber(@B6);
        print "# endif /* P00_UNSIGNED_MAX */\n";
        print "#endif /* P99_UINTMAX_MAX */\n";
    }

    for (my $i = 64; $i < 96; ++$i) {
        printout($i);
        advance(2, $i);
    }
    for (my $i = 96; $i < 129; ++$i) {
        printout($i);
        advance(3, $i);
    }
}

{
    sub printmask($$) {
        my ($n, $val) = @_;
        printf "#define P00_MASK_%d 0x%X\n", $n, $val;
    }

    my $val = 0;
    for (my $n = 0; $n < 64; ++$n) {
        printmask($n, $val);
        $val = 2 * $val + 1;
    }

}


my @groups =
    [ "P99",
      [ "compiler",
        [ "compiler_utilities" ],
        [ "C11",
          [ "C11_keywords" ],
          [ "C11_types" ],
          [ "generic",
            [ "C11_types",
              [ "type_generic" ]
            ],
            [ "qualifiers" ],
            [ "tgprint" ]
          ],
          [ "atomic" ],
          [ "threads" ],
          [ "C11_library" ],
        ],
      ],
      [ "library_support",
        ["futex"],
        ["library_wrappers"],
        ["posix_default"],
        ["callbacks"],
        ["startup_hooks"],
      ],
      [ "preprocessor_macros",
        [ "meta_programming",
          [ "preprocessor_conditionals" ],
          [ "preprocessor_for" ],
          [ "preprocessor_blocks",
            ["try"],
            ["validity"],
          ],
          [ "double_constants" ],
          [ "classification" ],
          [ "list_processing",
            [ "basic_list_operations" ],
            [ "statement_lists" ],
          ],
        ],
        [ "preprocessor_operators",
          [ "preprocessor_logic" ],
          [ "preprocessor_arithmetic" ],
          [ "preprocessor_text" ],
        ],
        [ "code_augmenting",
          [ "preprocessor_allocation" ],
          [ "preprocessor_initialization" ],
          [ "default_arguments" ],
          [ "variadic" ],
          [ "VLA" ],
          [ "types",
            [ "typenames",
              [ "one_token" ],
              [ "bool_specials" ],
            ],
          ],
          [ "stringconversion" ],
          [ "integers" ],
        ],
      ]
    ];

sub printGroups(@);

sub printGroups(@) {
    my ($name, @rest) = (@{$_[0]});
    print "\n//! \@addtogroup $name\n";
    print "//! \@{\n\n";
    print "\n#define P00_$name\n";
    foreach my $arr (@rest) {
        printGroups($arr);
    }
    print "\n//! \@}\n";
}

printGroups(@groups);

sub printCompilers(@);

print << "COMPILERS";

/* This long list of compilers does not mean that we tested P99, nor
   does it even imply that there is a C99 mode for them. We just list
   compilers and detection macros for them for completeness. The
   information for that detection was wildly collected from the web.
   They are listed in alphabetic order, and their numbering is
   nothing that is supposed to stay fixed, reliable or anything. */
COMPILERS

sub printCompilers(@) {
    my @compilers = (@_);
    my %compilers = (@_);
    my %numbers = ();
    my $num = 1;
    foreach my $comp (sort(keys %compilers)) {
        print "#define P99_COMPILER_\U${comp}\E ${num}U\n";
        print "#define P00_COMPILER_PRAGMA_\U${comp}\E(...)\n";
        $numbers{$comp} = $num;
        $num *= 2;
    }
    my $start = "if";
    while (my ($comp, $mac) = (splice(@compilers, 0, 2))) {
        print "#$start defined($mac)\n";
        print "# define P99_COMPILER $numbers{$comp}U\n";
        print "# define P99_COMPILER_VERSION \"${comp} \"\n";
        print "# undef P00_COMPILER_PRAGMA_\U${comp}\E\n";
        print "# define P00_COMPILER_PRAGMA_\U${comp}\E(...) _Pragma(__VA_ARGS__)\n";
        $start = "elif";
    }
    print "#else\n";
    print "# define P99_COMPILER 0\n";
    print "#endif\n";
}

printCompilers(@compilers);

print "\n#endif /* P99_GENERATED_H */\n";
