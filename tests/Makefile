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
		orwl_remote_server		\
		orwl_remote_test_exclusive	\
		orwl_remote_test_inclusive	\
		orwl_socket_finish		\
		orwl_socket_test		\
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
		orwl_remote_server.c		\
		orwl_remote_test_exclusive.c	\
		orwl_remote_test_inclusive.c	\
		orwl_socket_finish.c		\
		orwl_socket_test.c		\
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

INCL  	=		# List of *.h
LIBDIR	:= $(join $(dir ${CURDIR}),lib)
################
# Optional add #
################
OPT     += -O0
IPATH   += -I. -I../include -I../p99
OBJOPT  += -std=c99 -Wall -Wstrict-prototypes ${OPT}
EXEOPT  += -lorwl ${OPT}
LPATH   += -L. -Wl,-rpath,${LIBDIR} -L${LIBDIR}
COPY = "all rights reserved, "
LICENSE = SHORTLICENCE-closed.txt
TIDY = ../../parxxl/scripts/findSvnAuthors --type c --copy ${COPY} --lice ${LICENSE} --ofile

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

################
# Dependencies #
################

include Makefile.inc
