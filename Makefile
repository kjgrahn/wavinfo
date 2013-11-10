# Makefile
#
# Copyright (c) 2013 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

all: wavinfo
all: wavinfo.1

version.c: Makefile mkversion
	./mkversion groblad_{name=Groblad,version=3.3,prefix=$(INSTALLBASE)} $@

libwavinfo.a: riff.o
	$(AR) -r $@ $^

wavinfo: wavinfo.o libwavinfo.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lwavinfo

CFLAGS=-W -Wall -pedantic -ansi -g -Os
CXXFLAGS=-W -Wall -pedantic -std=c++98 -g -Os

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
	makedepend -- $(CFLAGS) -- -Y -I. *.{c,cc}

.PHONY: clean
clean:
	$(RM) wavinfo
	$(RM) *.o lib*.a
	$(RM) version.c
	$(RM) Makefile.bak

love:
	@echo "not war?"

# DO NOT DELETE

riff.o: riff.h littleendian.h
wavinfo.o: riff.h
