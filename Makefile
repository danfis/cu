
CC ?= gcc
PYTHON ?= python

PREFIX     ?= /usr/local
INCLUDEDIR ?= include
LIBDIR     ?= lib
BINDIR     ?= bin

CFLAGS = -g -Wall -pedantic

ENABLE_TIMER ?= no

ifeq '$(ENABLE_TIMER)' 'yes'
  CFLAGS += -DCU_ENABLE_TIMER
endif

TARGETS = libcu.a

all: $(TARGETS)

libcu.a: cu.o
	ar cr $@ $^
	ranlib $@

cu.o: cu.c cu.h
	$(CC) $(CFLAGS) -c -o $@ $<

install: libcu.a
	mkdir -p $(PREFIX)/$(INCLUDEDIR)
	mkdir -p $(PREFIX)/$(LIBDIR)
	mkdir -p $(PREFIX)/$(BINDIR)
	cp cu.h $(PREFIX)/$(INCLUDEDIR)/
	cp libcu.a $(PREFIX)/$(LIBDIR)/
	cp cu-check-regressions $(PREFIX)/$(BINDIR)/

deb:
	$(PYTHON) debian/make-changelog.py <CHANGELOG >debian/changelog
	cp BSD-LICENSE debian/copyright
	debuild -us -uc

clean:
	rm -f *.o
	rm -f $(TARGETS)
	$(MAKE) -C testsuites clean

check:
	$(MAKE) -C testsuites

.PHONY: all clean check
