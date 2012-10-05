
CC ?= gcc
PYTHON ?= python

CFLAGS = -g -Wall -pedantic

ENABLE_TIMER ?= no

ifeq '$(ENABLE_TIMER)' 'yes'
  CFLAGS += -DCU_ENABLE_TIMER
endif

TARGETS = libcu.a

TEST_OBJS = test.o test2.o

all: $(TARGETS)

libcu.a: cu.o
	ar cr $@ $^
	ranlib $@

cu.o: cu.c cu.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_OBJS) libcu.a
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) -L./ -lcu
test-segfault: test-segfault.c libcu.a
	$(CC) $(CFLAGS) -o $@ $^ -L./ -lcu

check: test test-segfault
	mkdir -p regressions
	touch regressions/testSuiteName.out
	touch regressions/testSuiteName.err
	touch regressions/testSuiteName2.out
	touch regressions/testSuiteName2.err
	touch regressions/testSuiteSegfault.out
	touch regressions/testSuiteSegfault.err
	touch regressions/testSuiteTest2.out
	touch regressions/testSuiteTest2.err
	-./test
	-cd regressions && $(PYTHON) ../check-regressions
	@echo ""
	@echo "======= SEGFAULT: ========="
	@echo ""
	-./test-segfault

clean:
	rm -f *.o
	rm -f test
	rm -f test-segfault
	rm -f $(TARGETS)
	rm -f tmp.*
	rm -rf regressions

.PHONY: all clean check
