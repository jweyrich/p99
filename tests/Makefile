##
## Makefile
##  
## Made by Jens Gustedt
## Login   <gustedtdamogran.loria.fr>
##
## Started on  Sat Feb 20 21:57:30 2010 Jens Gustedt
## Last update Sat Feb 20 21:57:30 2010 Jens Gustedt
## 
##############################
# Complete this to make it ! #
##############################
NAME 	=	orwl_wait_list			\
		orwl_remote_test		\
		orwl_socket_test		\
		orwl_socket_finish

SRC	=	orwl_wait_list.c		\
		orwl_remote_test.c		\
		orwl_socket_test.c		\
		orwl_socket_finish.c

INCL  	=		# List of *.h
LIBDIR	:= $(join $(dir ${PWD}),lib)
################
# Optional add #
################
OPT     = -O0
IPATH   = -I. -I../include
OBJOPT  = -std=gnu99 -pedantic -Wall -Wstrict-prototypes ${OPT}
EXEOPT  = -lorwl
LPATH   = -L. -Wl,-rpath,${LIBDIR} -L${LIBDIR}

#####################
# Macro Definitions #
#####################
CC 	= cc
MAKE 	= make
SHELL	= /bin/sh
OBJS 	= $(SRC:.c=.o) 
RM 	= /bin/rm -f 	
COMP	= gzip -9v
UNCOMP	= gzip -df
STRIP	= strip

CFLAGS  = $(OBJOPT) $(IPATH)
LDFLAGS = $(EXEOPT) $(LPATH)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z 

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME)
$(NAME) : ${OBJS}
% : %.o
	$(CC) $< $(LDFLAGS) -o $@
#	$(STRIP) ./$(NAME) # if you debug ,don't strip ...

depend Makefile.inc:
	gcc $(IPATH) -MM $(SRC) > Makefile.inc
clean:
	-$(RM) $(NAME) $(OBJS) *~
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
################
# Dependencies #
################

include Makefile.inc
