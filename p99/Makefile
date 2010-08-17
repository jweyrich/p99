## -*- Makefile -*-

p99_generated.h : ../scripts/p99_generate.pl
	../scripts/p99_generate.pl > $@

TAGS : *.h
	etags *.h

clean:
	-$(RM) *~

# for the moment distclean is tha same as clean
distclean : clean
