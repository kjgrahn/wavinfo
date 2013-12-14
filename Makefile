# Makefile
#
# Copyright (c) 2013 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

all: wavinfo
all: wavinfo.1
all: tests

version.c: Makefile mkversion
	./mkversion groblad_{name=Groblad,version=3.3,prefix=$(INSTALLBASE)} $@

libwavinfo.a: riff.o
libwavinfo.a: fmt.o
libwavinfo.a: bext.o
libwavinfo.a: newname.o
libwavinfo.a: basename.o
	$(AR) -r $@ $^

wavinfo: wavinfo.o libwavinfo.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lwavinfo

CFLAGS=-W -Wall -pedantic -ansi -g -Os
CXXFLAGS=-W -Wall -pedantic -std=c++98 -g -Os

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o libwavinfo.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lwavinfo

test/%.o: CPPFLAGS+=-I.

libtest.a: test/test_path.o
libtest.a: test/test_newname.o
	$(AR) -r $@ $^

.PHONY: install
install: all
	install -m555 wavinfo $(INSTALLBASE)/bin/
	install -m644 wavinfo.1 $(INSTALLBASE)/man/man1/

.PHONY: tags
tags: TAGS
TAGS:
	etags *.{c,h,cc}

.PHONY: depend
depend:
	makedepend -- -I. -- -Y -I. *.{c,cc} test/*.cc

.PHONY: clean
clean:
	$(RM) wavinfo tests
	$(RM) test.cc
	$(RM) *.o test/*.o lib*.a
	$(RM) version.c
	$(RM) Makefile.bak

love:
	@echo "not war?"

# DO NOT DELETE

basename.o: basename.h
bext.o: bext.h littleendian.h
fmt.o: fmt.h littleendian.h
newname.o: newname.h
riff.o: riff.h littleendian.h fmt.h
wavinfo.o: riff.h fmt.h bext.h newname.h
test/test_path.o: basename.h
