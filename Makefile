
DIRS = ./include ./p99 ./lib ./tests

P99_SOURCES = ./p99/*.h ./tests/p99*.c
P99_DOXY = ./doxy/p99-doxygen

P99FILES =  ${P99_SOURCES} ${P99_DOXY}

P99DISTRI = p99.tgz p99.zip p99-html.tgz p99-html.zip p99-refman.pdf

ORWL_SOURCES = ./include/*.h ./lib/*.c ./lib/Makefile ./tests/orwl*.c ./tests/Makefile 
#ORWL_DOXY = ./doxy/orwl-doxygen
ORWL_DOXY =

ORWLFILES =  ${ORWL_SOURCES} ${ORWL_DOXY}

ORWLDISTRI = orwl.tgz orwl.zip orwl-html.tgz orwl-html.zip orwl-refman.pdf

CLEAN = ${patsubst %,%/clean,${DIRS}}
DISTCLEAN = ${patsubst %,%/distclean,${DIRS}}
TIDY = ${patsubst %,%/tidy,${DIRS}}

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

doxygen : doxygen-p99 doxygen-orwl

doxygen-orwl :
	doxygen

doxygen-p99 :
	doxygen Doxyfile-p99

.PHONY : ${P99DISTRI} ${ORWLDISTRI}

p99-distribution : ${P99DISTRI}

p99.tgz :
	git archive --format=tar HEAD ${P99FILES} | gzip -9 > $@

p99.zip :
	git archive --format=zip -9 -o $@ HEAD ${P99FILES}

p99-html.tgz p99-html.zip p99-refman.pdf : doxygen-p99
	make -C p99-latex refman.pdf
	cp p99-latex/refman.pdf p99-refman.pdf
	tar -czf p99-html.tgz p99-html
	zip -q -r p99-html p99-html/

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

./tests/tidy :
	$(MAKE) -C tests tidy

./lib/tidy :
	$(MAKE) -C lib tidy

./include/tidy :
	$(MAKE) -C include tidy

