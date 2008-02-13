CC = gcc
CFLAGS = -g -Wall -pedantic

TARGETS = libcu.a

all: $(TARGETS)

libcu.a: cu.o
	ar cr $@ $^
	ranlib $@
cu.o: cu.c cu.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: test.c libcu.a
	$(CC) $(CFLAGS) -o $@ $^ -L./ -lcu

check: test
	mkdir -p regressions
	touch regressions/testSuiteName{,2}.{out,err}
	./test
	cd regressions && ../check-regressions

clean:
	rm -f *.o
	rm -f test
	rm -f $(TARGETS)
	rm -f tmp.*
	rm -rf regressions

.PHONY: all clean check
