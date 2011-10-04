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
		orwl_remote_test_inclusive	\
		orwl_finish		 	\
		orwl_posix_print		\
		orwl_socket_test		\
		orwl_benchmark_kernel23		\
		orwl_benchmark_synthetic	\
		orwl_benchmark_timer

SRC	=					\
		orwl_local_test_inclusive.c	\
		orwl_remote_server.c		\
		orwl_remote_test_inclusive.c	\
		orwl_socket_finish.c		\
		orwl_socket_test.c		\
		orwl_benchmark_kernel23.c	\
		orwl_benchmark_synthetic.c	\
		orwl_benchmark_timer.c

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

depend Makefile.inc:
	${CC} $(CFLAGS) $(IPATH) -MM $(SRC) > Makefile.inc
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

################
# Dependencies #
################

include Makefile.inc
