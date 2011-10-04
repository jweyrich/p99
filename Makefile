
DIRS = ./include ./lib ./tests

ORWL_SOURCES = ./include/*.h ./lib/*.c ./lib/Makefile ./tests/orwl*.c ./tests/Makefile 
#ORWL_DOXY = ./doxy/orwl-doxygen
ORWL_DOXY =

ORWLFILES =  ${ORWL_SOURCES} ${ORWL_DOXY}

ORWLDISTRI = orwl.tgz orwl.zip orwl-html.tgz orwl-html.zip orwl-refman.pdf

CLEAN = ${patsubst %,%/clean,${DIRS}}
DISTCLEAN = ${patsubst %,%/distclean,${DIRS}}
TIDY = ${patsubst %,%/tidy,${DIRS}}

ifndef DATE
DATE := ${shell date --rfc-3339=date}
endif
P99_GFORGE = shell.gforge.inria.fr
P99_HTDOCS = /home/groups/p99/htdocs

TAROPT := --dereference --owner=root --group=root

.PHONY : target clean ${DIRS} doxygen ${CLEAN} ${DISTCLEAN} ${TIDY}

all : ${DIRS}

clean : ${CLEAN}
	-$(RM) doxygen.log *~

distclean : ${DISTCLEAN}
	-$(RM) doxygen.log *~

tidy : ${TIDY}

tests : lib
	$(MAKE) -C $@

lib : ./include
	$(MAKE) -C $@

doxygen : doxygen-orwl

doxygen-orwl :
	doxygen

.PHONY : ${ORWLDISTRI}

orwl-distribution : ${ORWLDISTRI}

orwl.tgz :
	git archive --format=tar HEAD ${ORWLFILES} | gzip -9 > $@

orwl.zip :
	git archive --format=zip -9 -o $@ HEAD ${ORWLFILES}

orwl-html.tgz orwl-html.zip orwl-refman.pdf : doxygen-orwl
	make -C orwl-latex refman.pdf
	cp orwl-latex/refman.pdf orwl-refman.pdf
	tar -czf orwl-html.tgz orwl-html
	zip -q -r orwl-html orwl-html/

./include :

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

./tests/tidy :
	$(MAKE) -C tests tidy

./lib/tidy :
	$(MAKE) -C lib tidy

./include/tidy :
	$(MAKE) -C include tidy

