##
## Makefile
##  
## Made by Jens Gustedt
## Login   <gustedtdamogran.loria.fr>
##
## Started on  Sat Feb 20 21:57:30 2010 Jens Gustedt
## Last update Sat Feb 20 21:57:30 2010 Jens Gustedt
## 

include ../scripts/Makefile-getconf-orwl

NAME 	=					\
		orwl_local_test_inclusive	\
		orwl_remote_server		\
		orwl_remote_test_exclusive	\
		orwl_remote_test_inclusive	\
		orwl_socket_finish		\
		orwl_socket_test		\
		orwl_benchmark_kernel23		\
		orwl_benchmark_synthetic	\
		orwl_wait_list			\
		test-p99-block			\
		test-p99-cases			\
		test-p99-choice			\
		test-p99-classification		\
		test-p99-compound		\
		test-p99-double			\
		test-p99-int			\
		test-p99-ndim			\
		test-p99-pow			\
		test-p99-va-arg

SRC	=					\
		orwl_local_test_inclusive.c	\
		orwl_remote_server.c		\
		orwl_remote_test_exclusive.c	\
		orwl_remote_test_inclusive.c	\
		orwl_socket_finish.c		\
		orwl_socket_test.c		\
		orwl_benchmark_kernel23.c	\
		orwl_benchmark_synthetic.c	\
		orwl_wait_list.c		\
		test-p99-block.c		\
		test-p99-cases.c		\
		test-p99-choice.c		\
		test-p99-classification.c	\
		test-p99-compound.c		\
		test-p99-double.c		\
		test-p99-int.c			\
		test-p99-ndim.c			\
		test-p99-pow.c			\
		test-p99-va-arg.c

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
LIBDIR	:= $(join $(dir ${CURDIR}),lib)
################
# Optional add #
################
OPT     += -O0
IPATH   += -I. -I../include -I../p99
OBJOPT  += -Wall -Wstrict-prototypes ${OPT}
EXEOPT  += -lorwl ${OPT}
LPATH   += -L. -Wl,-rpath,${LIBDIR} -L${LIBDIR}
COPY = "all rights reserved, "
LICENSE = SHORTLICENCE-closed.txt
SCRIPTS = ../scripts
AUTH = ${SCRIPTS}/AUTHORS.txt
TIDY = ../../parxxl/scripts/findSvnAuthors --type c --auth ${AUTH} --copy ${COPY} --lice ${LICENSE} --ofile
CONFORM = ${SCRIPTS}/p99_conformance.pl

ifneq (${POSIX_SEMAPHORES}, -1L)
LDFLAGS += -lrt
endif

#####################
# Macro Definitions #
#####################
CC 	= c99
MAKE 	= make
SHELL	= /bin/sh
OBJS 	= $(SRC:.c=.o)
RM 	= /bin/rm -f
COMP	= gzip -9v
UNCOMP	= gzip -df
STRIP	= strip

CFLAGS  += $(OBJOPT) $(IPATH)
LDFLAGS += $(EXEOPT) $(LPATH)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z

include ../scripts/Makefile-DEFINE
include ../scripts/Makefile-__thread
include ../scripts/Makefile-atomic
include ../scripts/Makefile-complex
include ../scripts/Makefile-tgmath
include ../scripts/Makefile-iso646

ifeq (${GETTIMING},on)
CFLAGS += -DGETTIMING
endif

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME)
% : %.o
	$(CC) $< $(LDFLAGS) -o $@
#	$(STRIP) ./$(NAME) # if you debug ,don't strip ...

test-p99-inline : test-p99-inline.o test-p99-inline-empty.o
	$(CC) $^ $(LDFLAGS) -o $@

depend Makefile.inc:
	${CC} $(CFLAGS) $(IPATH) -MM $(SRC)  test-p99-inline.c test-p99-inline-empty.c > Makefile.inc
clean:
	-$(RM) $(NAME) $(OBJS) *~
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
	$(CC) $(CFLAGS) -c $<


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
