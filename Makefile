
DIRS = ./include ./p99 ./lib ./tests

CLEAN = ${patsubst %,%/clean,${DIRS}}
DISTCLEAN = ${patsubst %,%/distclean,${DIRS}}

.PHONY : target clean ${DIRS} doxygen ${CLEAN} ${DISTCLEAN}

all : ${DIRS}

clean : ${CLEAN}
	-$(RM) doxygen.log *~

distclean : ${DISTCLEAN}
	-$(RM) doxygen.log *~

tests : lib
	$(MAKE) -C $@

lib : ./include
	$(MAKE) -C $@

doxygen : doxygen-p99 doxygen-orwl

doxygen-orwl :
	doxygen

doxygen-p99 :
	doxygen Doxyfile-p99

./include :

./p99 :
	$(MAKE) -C $@

./tests/clean :
	$(MAKE) -C tests clean

./lib/clean :
	$(MAKE) -C lib clean

./include/clean :
	$(MAKE) -C include clean

./p99/clean :
	$(MAKE) -C p99 clean

./tests/distclean :
	$(MAKE) -C tests distclean

./lib/distclean :
	$(MAKE) -C lib distclean

./include/distclean :
	$(MAKE) -C include distclean

