## -*- Makefile -*-


COPY = "all rights reserved, "
LICENSE = SHORTLICENCE-closed.txt
COPYOPEN = "(C) copyright "
LICENSEOPEN = SHORTLICENCE-open.txt
AUTH = ../scripts/AUTHORS.txt
TIDY = ../../parxxl/scripts/findSvnAuthors --type perl --auth ${AUTH} --copy ${COPY} --lice ${LICENSE} --ofile

target : tidy

TAGS : *.h
	etags *.h

clean:
	-$(RM) *~

# for the moment distclean is tha same as clean
distclean : clean

tidy :
	for f in *.pl ; do  ${TIDY} $$f ; done
