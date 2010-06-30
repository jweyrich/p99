
DIRS = ./include ./lib ./tests

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

doxygen :
	doxygen

./include :
	$(MAKE) -C $@

./tests/clean :
	$(MAKE) -C tests clean

./lib/clean :
	$(MAKE) -C lib clean

./include/clean :
	$(MAKE) -C include clean

./tests/distclean :
	$(MAKE) -C tests distclean

./lib/distclean :
	$(MAKE) -C lib distclean

./include/distclean :
	$(MAKE) -C include distclean

