##
## Makefile
##  
## Made by Jens Gustedt
## Login   <gustedtdamogran.loria.fr>
##
## Started on  Sat Feb 20 21:57:30 2010 Jens Gustedt
## Last update Sat Feb 20 21:57:30 2010 Jens Gustedt
## 

SRC	=					\
		test-p99-block.c		\
		test-p99-cases.c		\
		test-p99-choice.c		\
		test-p99-classification.c	\
		test-p99-compound.c		\
		test-p99-double.c		\
		test-p99-error.c		\
		test-p99-fstruct.c		\
		test-p99-int.c			\
		test-p99-ndim.c			\
		test-p99-pow.c			\
		test-p99-qualifier.c            \
		test-p99-rand.c 		\
		test-p99-thread.c		\
		test-p99-uf.c			\
		test-p99-va-arg.c

ifeq (${NO_TEST_C11},)
SRC	+=	test-p99-c11.c
endif

ASRC	=       				\
		libp99.c			\
		libp99-weak.c


HTML = 						\
c99-conformance-clang-2.9.html			\
c99-conformance-gcc-4.1.html			\
c99-conformance-gcc-4.2.html			\
c99-conformance-gcc-4.3.html			\
c99-conformance-gcc-4.4.html			\
c99-conformance-icc-12.0.0.html			\
c99-conformance-opencc-4.2.4.html		\
c99-conformance-p99-gcc-4.1.html		\
c99-conformance-p99-gcc-4.2.html		\
c99-conformance-p99-gcc-4.3.html		\
c99-conformance-p99-gcc-4.4.html		\
c99-conformance-p99-pcc-0.9.9.html		\
c99-conformance-pcc-0.9.9.html			\
c99-conformance-tcc-0.9.25.html

INCL  	=		# List of *.h
################
# Optional add #
################
OPT     += -O0
IPATH   += -I../p99
OBJOPT  += -Wall -Wstrict-prototypes  -fdata-sections -ffunction-sections ${OPT}
EXEOPT  += ${OPT}
LPATH   += -L.
COPY = "all rights reserved, "
LICENSE = SHORTLICENCE-open.txt
SCRIPTS = ../scripts
AUTH = ${SCRIPTS}/AUTHORS.txt
TIDY = ../../../parxxl/scripts/findSvnAuthors --type c --auth ${AUTH} --copy ${COPY} --lice ${LICENSE} --ofile
CONFORM = ${SCRIPTS}/p99_conformance.pl

ifneq (${POSIX_SEMAPHORES}, -1L)
LDFLAGS := ${LDFLAGS}
endif

LDFLAGS += -Wl,--gc-sections -Wl,--print-gc-sections


#####################
# Macro Definitions #
#####################
CC 	= c99
MAKE 	= make
SHELL	= /bin/sh
OBJS 	= $(SRC:.c=.o)
AOBJS 	= $(ASRC:.c=.o)
ASSEM 	= $(ASRC:.c=.s)
ALIB 	= $(ASRC:.c=.a)
NAME 	= $(SRC:.c=)
RM 	= /bin/rm -f
COMP	= gzip -9v
UNCOMP	= gzip -df
STRIP	= strip

CFLAGS  += $(OBJOPT) $(IPATH)
LDFLAGS := $(EXEOPT) $(LPATH) $(LDFLAGS)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z

# every POSIX platform must provide getconf
XOPEN_VERSION := ${shell getconf _XOPEN_VERSION 2>/dev/null}
GNU_LIBC_VERSION := ${shell getconf GNU_LIBC_VERSION 2>/dev/null}

ifneq (${XOPEN_VERSION},)
CFLAGS += -D_XOPEN_SOURCE=${XOPEN_VERSION}
endif
ifneq (${GNU_LIBC_VERSION},)
CFLAGS += -D_GNU_SOURCE=1
LDFLAGS += -lrt
endif

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME) $(ASSEM) $(ALIB)
% : %.o
	$(CC) $< $(LDFLAGS) -o $@

%.a : %.o
	$(AR) crf $@ $<

test-p99-inline : test-p99-inline.o test-p99-inline-empty.o
	$(CC) $^ $(LDFLAGS) -o $@

depend Makefile.inc: Makefile
	${CC} $(CFLAGS) $(IPATH) -MM $(SRC) $(ASRC) test-p99-inline.c test-p99-inline-empty.c > Makefile.inc
	${CC} $(CFLAGS) $(IPATH) -MM $(SRC) $(ASRC) test-p99-inline.c test-p99-inline-empty.c | sed 's/[.]o/.s/g' >> Makefile.inc
clean:
	-$(RM) $(NAME) $(OBJS) $(AOBJS) $(ASSEM) $(ALIB) *~
distclean: clean
	-$(RM) Makefile.inc
fclean:
	-$(RM) $(NAME)
comp: clean
	$(COMP) $(INCL) $(SRC)
ucomp:
	$(UNCOMP) $(SRC) $(INCL)

.c.Z.c .h.Z.h .c.gz.c .h.gz.h .c.z.c .h.z.h :
	 -$(UNCOMP) $<

.c.o:
	$(CC) $< $(CFLAGS) -c

.c.s:
	$(CC) $< $(CFLAGS) -S


tidy :
	for f in ${SRC} ; do  ${TIDY} $$f ; done

html : ${HTML}

c99-conformance-clang-2.9.html : ${CONFORM}
	${CONFORM} clang 2.9
c99-conformance-gcc-4.1.html : ${CONFORM}
	${CONFORM} gcc-4.1
c99-conformance-gcc-4.2.html : ${CONFORM}
	${CONFORM} gcc-4.2
c99-conformance-gcc-4.3.html : ${CONFORM}
	${CONFORM} gcc-4.3
c99-conformance-gcc-4.4.html : ${CONFORM}
	${CONFORM} gcc-4.4
c99-conformance-gcc-4.5.html : ${CONFORM}
	${CONFORM} gcc-4.5
c99-conformance-icc-12.0.0.html : ${CONFORM}
	${CONFORM} icc 12.0.0
c99-conformance-opencc-4.2.4.html : ${CONFORM}
	${CONFORM} opencc 4.2.4
c99-conformance-p99-gcc-4.1.html : ${CONFORM}
	${CONFORM} ${SCRIPTS}/p99-gcc-4.1
c99-conformance-p99-gcc-4.2.html : ${CONFORM}
	${CONFORM} ${SCRIPTS}/p99-gcc-4.2
c99-conformance-p99-gcc-4.3.html : ${CONFORM}
	${CONFORM} ${SCRIPTS}/p99-gcc-4.3
c99-conformance-p99-gcc-4.4.html : ${CONFORM}
	${CONFORM} ${SCRIPTS}/p99-gcc-4.4
c99-conformance-p99-gcc-4.5.html : ${CONFORM}
	${CONFORM} ${SCRIPTS}/p99-gcc-4.5
c99-conformance-p99-pcc-0.9.9.html : ${CONFORM}
	${CONFORM} ${SCRIPTS}/p99-pcc 0.9.9
c99-conformance-pcc-0.9.9.html : ${CONFORM}
	${CONFORM} pcc 0.9.9
c99-conformance-tcc-0.9.25.html : ${CONFORM}
	${CONFORM} tcc 0.9.25


################
# Dependencies #
################

include Makefile.inc
