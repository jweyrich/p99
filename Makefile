
DIRS = ./include ./p99 ./lib ./tests

P99_SOURCES = ./p99/*.h ./tests/test-p99*.c
P99_DOXY = ./doxy/p99-doxygen

P99FILES =  ${P99_SOURCES} ${P99_DOXY}

ORWL_SOURCES = ./include/*.h ./lib/*.c ./lib/Makefile ./tests/orwl*.c ./tests/Makefile 
#ORWL_DOXY = ./doxy/orwl-doxygen
ORWL_DOXY =

ORWLFILES =  ${ORWL_SOURCES} ${ORWL_DOXY}

ORWLDISTRI = orwl.tgz orwl.zip orwl-html.tgz orwl-html.zip orwl-refman.pdf

CLEAN = ${patsubst %,%/clean,${DIRS}}
DISTCLEAN = ${patsubst %,%/distclean,${DIRS}}
TIDY = ${patsubst %,%/tidy,${DIRS}}

DATE := ${shell date --rfc-3339=date}
P99_RELEASE := p99-${DATE}
P99_ARCHIVE := gforge-p99
P99_PREFIX := ${shell pwd}/${P99_ARCHIVE}/versions/${P99_RELEASE}

P99DISTRI = ${P99_PREFIX}.tgz ${P99_PREFIX}.zip ${P99_PREFIX}-html.tgz ${P99_PREFIX}-html.zip ${P99_PREFIX}-refman.pdf p99-links

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

doxygen : doxygen-p99 doxygen-orwl

doxygen-orwl :
	doxygen

doxygen-p99 :
	doxygen Doxyfile-p99

.PHONY : ${P99DISTRI} ${ORWLDISTRI} 

p99-distribution : ${P99DISTRI}

${P99_PREFIX}.tar : p99/ChangeLog
	git archive -v --format=tar --prefix=${P99_RELEASE}/ HEAD ${P99FILES} -o $@
	tar --label ${P99_RELEASE} ${TAROPT} --transform 's|^|${P99_RELEASE}/|' -rf $@ p99/LICENSE p99/ChangeLog

${P99_PREFIX}.tgz : ${P99_PREFIX}.tar
	gzip -9 $<
	mv $<.gz $@

${P99_PREFIX}.zip : ${P99_PREFIX}.tgz
	tar xzf $<
	zip -r $@ --move ${P99_RELEASE}

${P99_PREFIX}-html.tgz ${P99_PREFIX}-html.zip ${P99_PREFIX}-refman.pdf : ${P99_PREFIX}
	-make -C ${P99_RELEASE}/p99-latex refman.pdf
	-cp ${P99_RELEASE}/p99-latex/refman.pdf ${P99_PREFIX}-refman.pdf
	tar -czf ${P99_PREFIX}-html.tgz ${TAROPT} ${P99_RELEASE}/p99-html
	zip -r ${P99_PREFIX}-html.zip ${P99_RELEASE}/p99-html

p99-links :
	ln -fs  versions/${P99_RELEASE}.tgz ${P99_ARCHIVE}/p99.tgz
	ln -fs  versions/${P99_RELEASE}.zip ${P99_ARCHIVE}/p99.zip
	ln -fs  versions/${P99_RELEASE}-html.tgz ${P99_ARCHIVE}/p99-html.tgz
	ln -fs  versions/${P99_RELEASE}-html.zip ${P99_ARCHIVE}/p99-html.zip
	ln -fs  versions/${P99_RELEASE}-refman.pdf ${P99_ARCHIVE}/p99-refman.pdf

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

p99/ChangeLog :
	git log --since=2010-11-20 -- p99 tests/test-p99-*.c > $@

p99-transfer :
	-rsync -az --no-g --no-p --progress -e 'ssh -ax' ${P99_ARCHIVE}/ ${P99_GFORGE}:${P99_HTDOCS}

p99-html-transfer : ${P99_RELEASE}/p99-html
	-cd ${P99_RELEASE}; rsync -az --no-g --no-p --progress -e 'ssh -ax' p99-html/ ${P99_GFORGE}:${P99_HTDOCS}/p99-html-new
	ssh ${P99_GFORGE} mv ${P99_HTDOCS}/p99-html ${P99_HTDOCS}/p99-html-bak
	ssh ${P99_GFORGE} mv ${P99_HTDOCS}/p99-html-new ${P99_HTDOCS}/p99-html
	ssh ${P99_GFORGE} rm -rf ${P99_HTDOCS}/p99-html-bak

${P99_PREFIX} : ${P99_PREFIX}.tgz
	rm -rf ${P99_PREFIX}
	tar xzf ${P99_PREFIX}.tgz
	cp Doxyfile-p99 ${P99_RELEASE}
	cd ${P99_RELEASE}; doxygen Doxyfile-p99

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

