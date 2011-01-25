##
## Makefile
##  
## Made by Jens Gustedt
## Login   <gustedtdamogran.loria.fr>
##
## Started on  Fri Feb 19 12:12:22 2010 Jens Gustedt
## Last update Fri Feb 19 12:12:22 2010 Jens Gustedt
## 

include ../scripts/Makefile-getconf-orwl

NAME 	=	liborwl.a liborwl.so	# Name of executable file
SRC	=					\
	orwl_auth_sock_functions.c		\
	orwl_barrier.c				\
	orwl_count.c				\
	orwl_endpoint.c				\
	orwl_handle2.c				\
	orwl_host.c				\
	orwl_int.c				\
	orwl_new.c				\
	orwl_once.c				\
	orwl_posix.c				\
	orwl_rand.c				\
	orwl_register.c				\
	orwl_remote_queue.c			\
	orwl_sem.c				\
	orwl_server.c				\
	orwl_socket.c				\
	orwl_thread.c				\
	orwl_types.c				\
	orwl_wait_queue.c

INCL  	=	# List of *.h
# The global variables that are used and that are external
GLOBVARS=environ stderr stdout stdin in6addr_any
################
# Optional add #
################
IPATH   += -I.  -I../include -I../p99
OPT	+= -O3
OBJOPT  += -std=c99 -Wall -Wstrict-prototypes ${OPT}
EXEOPT  += -lpthread -lm
LPATH   += -L.

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
NM	= nm
NM_UNDEF_FILTER	= sed -n "/^[ ]*U [a-z]/s|.* \([a-zA-Z0-9_]*\).*|\1|g p"
NM_DEF_FILTER	= sed -n "/^[0-9a-fA-F]*[ ]*[TW] [a-z]/s|.* \([a-zA-Z0-9_]*\).*|\1|g p"
CFLAGS  += -fPIC $(OBJOPT) $(IPATH)
LDFLAGS += $(EXEOPT) $(LPATH)
COPY = "all rights reserved, "
LICENSE = SHORTLICENCE-closed.txt
TIDY = ../../parxxl/scripts/findSvnAuthors --type c --copy ${COPY} --lice ${LICENSE} --ofile

include ../scripts/Makefile-DEFINE
include ../scripts/Makefile-__thread
include ../scripts/Makefile-atomic

ifeq (${POSIX_SEMAPHORES},-1L)
$(warning No POSIX semaphores were detected on this system. Replacing them by a wrapper)
else
SRC += semaphore_nointr.c
LDFLAGS += -lrt
endif

ifeq (${ATOMIC_OPS},)
ifeq (${DEFINE__GCC_HAVE_SYNC_COMPARE_AND_SWAP},1)
${warning atomic operations with 'atomic_...' are not supported natively, implementing them with compiler extensions}
SRC += orwl_atomic.c
else
${warning atomic operations with 'atomic_...' are not supported, using a suboptimal counter implementation}
endif
endif


ifeq (${DEFINE__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__},)
LDSHARED := -shared
else
LDSHARED := -dynamiclib
endif

.SUFFIXES: .h.Z .c.Z .h.gz .c.gz .c.z .h.z

##############################
# Basic Compile Instructions #
##############################

all:	$(NAME) external_symbols.o orwl_symbols.o orwl_symbols

${NAME} : ${OBJS}

depend Makefile.inc : Makefile
	${CC} $(OBJOPT) $(IPATH) -MM $(SRC) > Makefile.inc

TAGS : ${SRC}
	etags ${SRC}
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
################
# Dependencies #
################

liborwl.a : liborwl.a(${OBJS})

liborwl.so : liborwl.a # ${OBJS}
	${CC} ${LDSHARED} ${OBJS} $(LDFLAGS) $(TARGET_ARCH) -o `pwd`/liborwl.so

include Makefile.inc

EXTERNAL_SYMBOLS := ${filter-out ${GLOBVARS}, ${sort ${shell ${NM} liborwl.so | ${NM_UNDEF_FILTER}}}}
ORWL_SYMBOLS := ${filter-out ${GLOBVARS}, ${sort ${shell ${NM} liborwl.so | ${NM_DEF_FILTER}}}}

COMMA=,

EXTERNAL_CASTS := ${patsubst %,\t(orwl__FT)%${COMMA}\n,${EXTERNAL_SYMBOLS}}
EXTERNAL_CALLS := ${patsubst %,\t((orwl__FT)%)((void*)0);\n,${EXTERNAL_SYMBOLS}}
EXTERNAL_PROTS := ${patsubst %,\tvoid* %(void*);\n,${EXTERNAL_SYMBOLS}}

external_symbols.c : liborwl.so Makefile
	@printf '/** @file external_symbols.c\n' > external_symbols.c
	@printf ' ** @brief External refferences in the library\n' >> external_symbols.c
	@printf ' **/\n' >> external_symbols.c
	@printf '#include "orwl_posix.h"\n' >> external_symbols.c
	@printf 'typedef void* (*orwl__FT)(void*);\n' >> external_symbols.c
	@printf 'void* (*const external_functions[])(void*) = {\n${EXTERNAL_CASTS}};\n' >> external_symbols.c
	@printf '#ifdef DOXYGEN\n' >> external_symbols.c
	@printf 'void call_externals(void) {\n${EXTERNAL_CALLS}}\n' >> external_symbols.c
	@printf '${EXTERNAL_PROTS}\n' >> external_symbols.c
	@printf '#endif\n' >> external_symbols.c

ORWL_CASTS := ${patsubst %,\t(orwl__FT)%${COMMA}\n,${ORWL_SYMBOLS}}
ORWL_PROTS := ${patsubst %,\tvoid* %(void*);\n,${ORWL_SYMBOLS}}

orwl_symbols.c :
	@printf '/** @file orwl_symbols.c\n' > orwl_symbols.c
	@printf ' ** @brief Orwl refferences in the library\n' >> orwl_symbols.c
	@printf ' **/\n' >> orwl_symbols.c
	@printf '#ifndef DOXYGEN\n' >> orwl_symbols.c
	@printf 'typedef void* (*orwl__FT)(void*);\n' >> orwl_symbols.c
	@printf '${ORWL_PROTS}\n' >> orwl_symbols.c
	@printf 'void* (*const orwl_functions[])(void*) = {\n${ORWL_CASTS}};\n' >> orwl_symbols.c
	@printf 'int main(void) {\n' >> orwl_symbols.c
	@printf 'return sizeof(orwl_functions)/sizeof(orwl_functions[0]);\n}\n' >> orwl_symbols.c
	@printf '#endif\n' >> orwl_symbols.c

orwl_symbols : orwl_symbols.o liborwl.so Makefile
	${CC} -o $@ orwl_symbols.o -L. -Wl,-rpath,. -L. -lorwl
tidy :
	for f in ${SRC} ; do  ${TIDY} $$f ; done
