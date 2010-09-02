
DIRS = ./include ./p99 ./lib ./tests

P99FILES = ./p99/*.h ./tests/p99*.c

P99DISTRI = p99.tgz p99.zip p99-html.tgz p99-html.zip p99-refman.pdf


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

.PHONY : ${P99DISTRI}
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

