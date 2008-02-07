CC = gcc
CFLAGS += -Wall -pedantic -O2 -pipe
LDFLAGS =

TARGETS = libcu.a

all: $(TARGETS)

libcu.a: cu.o
	ar cr $@ $^
	ranlib $@
cu.o: cu.c cu.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

test: test.c cu.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f test
	rm -f $(TARGETS)
	rm -f tmp.*

.PHONY: all clean
