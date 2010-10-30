## -*- Makefile -*-


COPYCLOSED = "all rights reserved, "
LICENSECLOSED = SHORTLICENCE-closed.txt
COPY = "(C) copyright "
LICENSE = SHORTLICENCE-open.txt
TIDY = ../../parxxl/scripts/findSvnAuthors --type c --copy ${COPY} --lice ${LICENSE} --ofile

p99_generated.h : ../scripts/p99_generate.pl
	../scripts/p99_generate.pl > $@

TAGS : *.h
	etags *.h

clean:
	-$(RM) *~

# for the moment distclean is tha same as clean
distclean : clean

tidy :
	for f in *.h ; do  ${TIDY} $$f ; done
