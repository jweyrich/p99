
DIRS = ./p99 ./tests

P99_SOURCES = ./p99/*.h ./p99/p99_special.c ./tests/test-p99*.c tests/test-p99-*.h
P99_DOXY = ./doxy/p99-doxygen

# The first commit after the first release
REL1 = 4efb830e54f1b157a6127e5e7c2f4b34e298691e

P99FILES =  ${P99_SOURCES} ${P99_DOXY}

CLEAN = ${patsubst %,%/clean,${DIRS}}
DISTCLEAN = ${patsubst %,%/distclean,${DIRS}}
TIDY = ${patsubst %,%/tidy,${DIRS}}

ifndef DATE
DATE := ${shell date --rfc-3339=date}
endif
P99_RELEASE := p99-${DATE}
P99_ARCHIVE := gforge-p99
P99_VERSIONS := ${shell pwd}/${P99_ARCHIVE}/versions
P99_PREFIX := ${P99_VERSIONS}/${P99_RELEASE}

P99DISTRI = ${P99_PREFIX}.tgz ${P99_PREFIX}.zip ${P99_PREFIX}-html.tgz ${P99_PREFIX}-html.zip ${P99_PREFIX}-refman.pdf p99-links

P99_GFORGE = shell.gforge.inria.fr
P99_HTDOCS = /home/groups/p99/htdocs

TAROPT := --dereference --owner=root --group=root

.PHONY : target clean ${DIRS} doxygen p99/ChangeLog ${CLEAN} ${DISTCLEAN} ${TIDY}

all : ${DIRS}

clean : ${CLEAN}
	-$(RM) doxygen.log *~

distclean : ${DISTCLEAN}
	-$(RM) doxygen.log *~

tidy : ${TIDY}

tests :
	$(MAKE) -C $@

doxygen : doxygen-p99

doxygen-p99 :
	doxygen Doxyfile-p99

.PHONY : ${P99DISTRI}

p99-distribution : ${P99_VERSIONS} ${P99DISTRI}

${P99_PREFIX}.tar : p99/ChangeLog
	git archive -v --format=tar --prefix=${P99_RELEASE}/ -o $@ HEAD ${P99FILES}
	tar ${TAROPT} --transform 's|^|${P99_RELEASE}/|' -rf $@ p99/LICENSE p99/ChangeLog

${P99_PREFIX}.tgz : ${P99_PREFIX}.tar
	gzip -9 $<
	mv $<.gz $@

${P99_PREFIX}.zip : ${P99_PREFIX}.tgz
	tar xzf $<
	zip -r $@ --move ${P99_RELEASE}

${P99_PREFIX}-html.tgz ${P99_PREFIX}-html.zip ${P99_PREFIX}-refman.pdf : ${P99_PREFIX}
	-cp greekUTF8.sty ${P99_RELEASE}/p99-latex/
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

${P99_VERSIONS} :
	install -d ${P99_VERSIONS}

p99/ChangeLog : commits Makefile
	git show -C -M --find-copies-harder --name-only `cat commits` | grep -vi orwl > $@

commits : p99/*.h tests/test-p99-*.c tests/test-p99-*.h Makefile
#	insert a line for the first release commit such that we can cut off, later
	git log --pretty=format:"%at CUTCUTCUT%n" ${REL1}^..${REL1} > .commits
#	collect the blobs for all source files
#	prefix them with the time stamp such that we can sort them later
	for f in $^ ; do                                                    \
	git log -C -M --follow --pretty=format:"%at %H%n" $$f >> .commits ; \
	done
#	sort all blobs uniquely and cut off at the chosen line
	sort -u -r .commits | sed -n 's/.* //; /CUTCUTCUT/q; p' > commits
	@rm .commits

all-commits : p99/*.h tests/test-p99-*.c tests/test-p99-*.h Makefile Doxyfile-p99 gforge-p99/* scripts/* doxy/*
#	insert a line for the first release commit such that we can cut off, later
	echo > .commits
#	collect the blobs for all source files
#	prefix them with the time stamp such that we can sort them later
	for f in $^ ; do                                                    \
	git log -C -M --follow --pretty=format:"%at %H%n" $$f >> .commits ; \
	done
#	sort all blobs uniquely and cut off at the chosen line
	sort -u -r .commits | sed 's/.* //' > all-commits
	@rm .commits


p99-transfer :
	-rsync -az --no-g --no-p --chmod=ug+rw --progress -e 'ssh -ax' ${P99_ARCHIVE}/ ${P99_GFORGE}:${P99_HTDOCS}

p99-html-transfer : ${P99_RELEASE}/p99-html
	-cd ${P99_RELEASE}; rsync -az --no-g --no-p --chmod=ug+rw --progress -e 'ssh -ax' p99-html/ ${P99_GFORGE}:${P99_HTDOCS}/p99-html-new
	-ssh ${P99_GFORGE} mv ${P99_HTDOCS}/p99-html ${P99_HTDOCS}/p99-html-bak
	-ssh ${P99_GFORGE} chmod -R ug+rw ${P99_HTDOCS}/p99-html-new
	-ssh ${P99_GFORGE} mv ${P99_HTDOCS}/p99-html-new ${P99_HTDOCS}/p99-html
	-ssh ${P99_GFORGE} rm -rf ${P99_HTDOCS}/p99-html-bak

${P99_PREFIX} : ${P99_PREFIX}.tgz
	rm -rf ${P99_PREFIX}
	tar xzf ${P99_PREFIX}.tgz
	cp Doxyfile-p99 ${P99_RELEASE}
	cd ${P99_RELEASE}; doxygen Doxyfile-p99

./p99 :
	$(MAKE) -C $@

./tests/clean :
	$(MAKE) -C tests clean

./tests/distclean :
	$(MAKE) -C tests distclean

./tests/tidy :
	$(MAKE) -C tests tidy
