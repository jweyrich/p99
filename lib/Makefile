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
SRC	=					\
	orwl_auth_sock_functions.c		\
	orwl_endpoint.c				\
	orwl_enum.c				\
	orwl_handle2.c				\
	orwl_host.c				\
	orwl_int.c				\
	orwl_new.c				\
	orwl_once.c				\
	orwl_posix_default.c			\
	orwl_rand.c				\
	orwl_register.c				\
	orwl_remote_queue.c			\
	orwl_server.c				\
	orwl_socket.c				\
	orwl_thread.c				\
	orwl_types.c				\
	orwl_wait_queue.c
INCL  	=	# List of *.h
# The global variables that are used and that are external
GLOBVARS=stderr stdout stdin in6addr_any
################
# Optional add #
################
IPATH   = -I.  -I../include
OPT	= -O3
OBJOPT  = -std=gnu99 -pedantic -Wall -Wstrict-prototypes ${OPT}
EXEOPT  = -lpthread -lm
LPATH   = -L.

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
NM	= nm
NM_UNDEF_FILTER	= sed -n "/^[ ]*U [a-z]/s|.* \([a-z0-9_]*\).*|\1|g p"
CFLAGS  = -fPIC $(OBJOPT) $(IPATH)
LDFLAGS = $(EXEOPT) $(LPATH)

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME) external_symbols.o

${NAME} : ${OBJS}

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

liborwl.so : liborwl.a # ${OBJS}
	${CC} --shared  ${OBJS} $(LDFLAGS) $(TARGET_ARCH) -o liborwl.so

include Makefile.inc

SYMBOLS=${filter-out ${GLOBVARS}, ${sort ${shell ${NM} liborwl.so | ${NM_UNDEF_FILTER}}}}

COMMA=,

CASTS=${patsubst %,\t(_FT)%${COMMA}\n,${SYMBOLS}}
CALLS=${patsubst %,\t((_FT)%)(NULL);\n,${SYMBOLS}}
PROTS=${patsubst %,\tvoid* %(void*);\n,${SYMBOLS}}

external_symbols.c : liborwl.so Makefile
	@printf '/** @file external_symbols.c\n' > external_symbols.c
	@printf ' ** @brief External refferences in the library\n' >> external_symbols.c
	@printf ' **/\n' >> external_symbols.c
	@printf '#include "orwl_posix_default.h"\n' >> external_symbols.c
	@printf 'typedef void* (*_FT)(void*);\n' >> external_symbols.c
	@printf 'void* (*const external_functions[])(void*) = {\n${CASTS}};\n' >> external_symbols.c
	@printf '#ifdef DOXYGEN\n' >> external_symbols.c
	@printf 'void call_externals(void) {\n${CALLS}}\n' >> external_symbols.c
	@printf '${PROTS}\n' >> external_symbols.c
	@printf '#endif\n' >> external_symbols.c
