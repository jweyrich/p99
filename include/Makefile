## -*- Makefile -*-

COPY = "all rights reserved, "
LICENSE = SHORTLICENCE-closed.txt
TIDY = ../../parxxl/scripts/findSvnAuthors --type c --copy ${COPY} --lice ${LICENSE} --ofile

TAGS : *.h
	etags *.h

clean:
	-$(RM) *~

# for the moment distclean is tha same as clean
distclean : clean


tidy :
	for f in *.h ; do  ${TIDY} $$f ; done
