##
## Makefile
##  
## Made by Jens Gustedt
## Login   <gustedtdamogran.loria.fr>
##
## Started on  Fri Feb 19 12:12:22 2010 Jens Gustedt
## Last update Fri Feb 19 12:12:22 2010 Jens Gustedt
## 
##############################
# Complete this to make it ! #
##############################
NAME 	=	liborwl.a liborwl.so	# Name of executable file
SRC	=	orwl_wait_queue.c	orwl_enum.c # List of *.c
INCL  	=	# List of *.h
################
# Optional add #
################
IPATH   = -I.  -I../include         # path of include file
OBJOPT  = -std=gnu99 -pedantic -O2 -Wall -Wstrict-prototypes       # option for obj
EXEOPT  = -std=gnu99 -pedantic -O2 -Wall -Wstrict-prototypes       # option for exe (-lefence ...)
LPATH   = -L.           # path for librairies ... 

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

CFLAGS  = -fPIC $(OBJOPT) $(IPATH)
LDFLAGS = $(EXEOPT) $(LPATH)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z 

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME)
#$(NAME): $(OBJS) $(SRC) $(INCL)  
#	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME) 
#	$(STRIP) ./$(NAME) # if you debug ,don't strip ...

depend Makefile.inc :
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

liborwl.a : liborwl.a(${OBJS})

liborwl.so : ${OBJS}
	${CC} --shared $(LDFLAGS) $(TARGET_ARCH) -o liborwl.so ${OBJS}

include Makefile.inc
