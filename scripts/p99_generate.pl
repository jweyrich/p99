#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- perl -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#

my $maxnumber = 81;

# these are the normative keywords in C99
my @keywords_C99
    = sort qw( _Bool _Complex _Imaginary auto break case char
         const continue default do double else enum extern float for goto if
         inline int long register restrict return short signed sizeof static
         struct switch typedef union unsigned void volatile while );

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
        fprintf
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
        fscanf
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
        atof
        atoi
        atol
        bsearch
        div
        ldiv
        exit
        free
        getenv
        ldiv
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

my @token_C99 = sort(@keywords_C99, @typedefs_C99, @functions_C99);

print "/* This file is automat";
print "ically generated, do not chan";
print "ge manually. */\n";
print "\n";
print "/** \@file                                                               \n";
print " ** \@brief automatically generated macros to handle variadic macros.    \n";
print " **                                                                      \n";
print " ** You should probably never include this file directly but through     \n";
print " ** other P99 header files that define macros that are of direct use.  */\n";
print "\n";
print "/** \@brief The maximal number of arguments the P99 macros can handle.   \n";
print " **                                                                      \n";
print " ** This limit applies also to the integer arithmetic that is            \n";
print " ** performed by the macros in \@ref preprocessor_arithmetic           */\n";
print "#define P99_MAX_NUMBER $maxnumber\n\n";


print "#define P99__ARG(";
for (my $i = 1; $i <= $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print "\t_$i,";
    } else {
        print "\\\n\t_$i,";
    }
}
print "\\\n\t...) _$maxnumber\n";

for (my $m = 1; $m < 5; ++$m) {
    print "#define P99__NARG_$m(...) P99__ARG(__VA_ARGS__, ";
    for (my $i = ($maxnumber - 1); $i >= 0; --$i) {
        my $val = ($i % $m) ? "P99__INV($m)" : ($i / $m);
        if ($i % 8 != 7) {
            print "\t$val,";
        } else {
            print "\\\n\t$val,";
        }
    }
    print ")\n";
}

print "/** \@ingroup basic_list_operations                                                 */\n";
print "/** \@brief Determine of the argument list has a comma, i.e at least two arguments. */\n";
print "#define P99_HAS_COMMA(...) P99__ARG(__VA_ARGS__,\\\n";
for (my $i = 2; $i < $maxnumber; ++$i) {
    if ($i % 8 != 1) {
        print "\t1,";
    } else {
        print "\\\n\t1,";
    }
}
print "\t0, ...)\n";


for (my $arg = 2; $arg < $maxnumber; ++$arg) {
    my $arg1 = ${arg} - 1;
    print "#define P99__SKP${arg}(_0, ...) P99__SKP${arg1}(__VA_ARGS__)\n";
}

for (my $arg = 2; $arg < $maxnumber; ++$arg) {
    my $arg1 = ${arg} - 1;
    print "#define P99__PRE${arg}(_0, ...) _0, P99__PRE${arg1}(__VA_ARGS__)\n";
}

print "#define P99__ASCENDING() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t$i,";
    } else {
        print "\\\n\t$i,";
    }
}
print STDOUT "\n";

print "#define P99__ALL_ZEROES() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t0,";
    } else {
        print "\\\n\t0,";
    }
}
print STDOUT "\n";

print "#define P99__ALL_ONES() ";
for (my $i = 0; $i < $maxnumber; ++$i) {
    if ($i % 8 != 0) {
        print "\t1,";
    } else {
        print "\\\n\t1,";
    }
}
print STDOUT "\n";

for (my $m = 7; $m < $maxnumber; ++$m) {
    my $m1 = $m - 1;
    print "/** \@brief Paste $m arguments at their boundary.*/\n";
    print "#define P99_PASTE$m(...) P99__PASTE$m(P99_LAST(__VA_ARGS__), P99_ALLBUTLAST(__VA_ARGS__))\n";
    print "#define P99__PASTE$m(L, ...) P99_PASTE2(P99_PASTE${m1}(__VA_ARGS__), L)\n";
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


print "#define P99__IS_${_}_EQ_${_}(...) ,\n"
    foreach (0.. $maxnumber, @token_C99);
print "/*! \@brief Test if the argument consists of exactly the token \@c ${_} */\
#define P99_IS_EQ_${_}(...) P99_TOK_EQ(${_}, __VA_ARGS__)\n"
    foreach (0.. $maxnumber, @token_C99);

print <<'PREPRO3';
/**
 ** @}
 **/
PREPRO3


printf "#define P99__DEC_PRED_%d %d\n", $_ + 1, $_
    foreach (0.. $maxnumber);
printf "#define P99__DEC_PRED_minus_%d minus_%d\n", $_, $_ + 1
    foreach (0.. $maxnumber);
printf "#define P99__minus_minus_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define P99__IS_%d_GE_0 ,\n", $_
    foreach (0.. $maxnumber);
printf "#define P99__dec_eval_%d %d\n", $_, $_
    foreach (0.. $maxnumber);
printf "#define P99__dec_eval_minus_%d %d\n", $_, -$_
    foreach (0.. $maxnumber);
print "#define P99_DUPL${_}(...) __VA_ARGS__, P99_DUPL", ($_ - 1), "(__VA_ARGS__)\n"
    foreach (2 .. $maxnumber);


for (my $i = 2; $i < $maxnumber; ++$i) {
    my $i1 = $i - 1;
    print "#define P99__FOR${i}(NAME, OP, FUNC, ...) \\\n",
    "\tOP(NAME, $i1, P99__FOR${i1}(NAME, OP, FUNC, P99_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, P99_LAST(__VA_ARGS__), $i1))\n";
}

print << 'MAXHEAD';

/* The preprocessor always computes with the precision of uintmax_t */
/* so for the preprocessor this is equivalent to UINITMAX_MAX       */
#define P99__UNSIGNED_MAX ~0u

#define P99__S0 0x01
#define P99__S1 0x02
#define P99__S2 0x04
#define P99__S3 0x08
#define P99__S4 0x10
#define P99__S5 0x20
#define P99__S6 0x40



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
    my @maxval = ( 0xFFFFFFFF, 0, 0, 0 );

    my @B0 = ( 0xAAAAAAAA, 0, 0, 0);
    my @B1 = ( 0xCCCCCCCC, 0, 0, 0);
    my @B2 = ( 0xF0F0F0F0, 0, 0, 0);
    my @B3 = ( 0xFF00FF00, 0, 0, 0);
    my @B4 = ( 0xFFFF0000, 0, 0, 0);
    my @B5 = ( 0x00000000, 0, 0, 0);
    my @B6 = ( 0x00000000, 0, 0, 0);

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
        # $B5[$pos] = ($B5[$pos] << 1) + 1
        #     if ($pos % 2);
        # $B6[$pos] = ($B6[$pos] << 1) + 1
        #     if ($width >= 64);
    }

    sub printout($) {
        my ($width) = @_;
        printf "#if P99__UNSIGNED_MAX == 0x%s\n", printnumber(@maxval);
        printf "#define P99_UINTMAX_BIT %u\n", $width;
        printf "#define P99_UINTMAX_MAX UINTMAX_C(0x%s)\n", printnumber(@maxval);
        printf "#define P99__B0 UINTMAX_C(0x%s)\n", printnumber(@B0);
        printf "#define P99__B1 UINTMAX_C(0x%s)\n", printnumber(@B1);
        printf "#define P99__B2 UINTMAX_C(0x%s)\n", printnumber(@B2);
        printf "#define P99__B3 UINTMAX_C(0x%s)\n", printnumber(@B3);
        printf "#define P99__B4 UINTMAX_C(0x%s)\n", printnumber(@B4);
        printf "#define P99__B5 UINTMAX_C(0x%s)\n", printnumber(@B5);
        printf "#define P99__B6 UINTMAX_C(0x%s)\n", printnumber(@B6);
    }

    for (my $i = 32; $i < 64; ++$i) {
        printout($i);
        advance(1, $i);
        print "#else\n";
    }
    for (my $i = 64; $i < 96; ++$i) {
        printout($i);
        advance(2, $i);
        print "#else\n";
    }
    for (my $i = 96; $i < 129; ++$i) {
        printout($i);
        advance(3, $i);
        if ($i < 128) {
            print "#else\n";
        } else {
            print "#endif /* P99__UNSIGNED_MAX */\n" x (129 - 32);
        }
    }
}

my @groups =
    [ "preprocessor_macros",
      [ "meta_programming",
        [ "preprocessor_conditionals" ],
        [ "preprocessor_for" ],
        [ "preprocessor_blocks" ],
        [ "double_constants" ],
      ],
      [ "list_processing",
        [ "basic_list_operations" ],
        [ "statement_lists" ],
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
      ],
    ];

sub printGroups(@);

sub printGroups(@) {
    my ($name, @rest) = (@{$_[0]});
    print "\n//! \@addtogroup $name\n";
    print "//! \@{\n\n";
    print "\n#define P99__$name\n";
    foreach my $arr (@rest) {
        printGroups($arr);
    }
    print "\n//! \@}\n";
}

printGroups(@groups);
