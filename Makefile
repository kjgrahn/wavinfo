# Makefile
#
# Copyright (c) 2013 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

all: wavinfo
all: wavinfo.1
all: tests

libwavinfo.a: riff.o
libwavinfo.a: fmt.o
libwavinfo.a: bext.o
libwavinfo.a: newname.o
libwavinfo.a: basename.o
libwavinfo.a: mv.o
	$(AR) -r $@ $^

wavinfo: wavinfo.o libwavinfo.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lwavinfo

CXXFLAGS=-W -Wall -pedantic -std=c++11 -g -Os

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

test.cc: libtest.a
	orchis -o$@ $^

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

.PHONY: clean
clean:
	$(RM) wavinfo tests
	$(RM) test.cc
	$(RM) *.o test/*.o lib*.a
	$(RM) -r dep

love:
	@echo "not war?"

$(shell mkdir -p dep/test)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
