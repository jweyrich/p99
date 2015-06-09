## -*- Makefile -*-


COPYCLOSED = "all rights reserved, "
LICENSECLOSED = SHORTLICENCE-closed.txt
COPY = "(C) copyright "
LICENSE = SHORTLICENCE-open.txt
AUTH = ../scripts/AUTHORS.txt
TIDY = ../scripts/findSvnAuthors
TIDYFLAGS = --scm git --type c --auth ${AUTH} --copy ${COPY} --lice ${LICENSE} --ofile

target : p99_generated.h check

p99_generated.h : ../scripts/p99_generate.pl
	../scripts/p99_generate.pl > $@

check : *.h
	../scripts/p99_find_macros.pl p99_*.h | ../scripts/p99_find_ternary.pl | ../scripts/p99_find_args.pl --prefix="non VM types: "
	../scripts/p99_find_macros.pl p99_*.h | ../scripts/p99_find_ternary.pl --sizeof | ../scripts/p99_find_args.pl --prefix="possible VLA: "

check-vla : *.h
	../scripts/p99_find_macros.pl p99_*.h --sizeof | ../scripts/p99_find_ternary.pl --sizeof | ../scripts/p99_find_args.pl  --prefix="watch for VLA: "

TAGS : *.h
	etags *.h

clean:
	-$(RM) *~

# for the moment distclean is tha same as clean
distclean : clean

tidy :
	for f in *.h p99_special.c ; do  ${TIDY} ${TIDYFLAGS} $$f ; done
