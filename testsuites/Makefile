CC ?= gcc
PYTHON ?= python

TEST_OBJS = test.o test2.o

all: test test-segfault
	mkdir -p regressions
	touch regressions/testSuiteName.out
	touch regressions/testSuiteName.err
	touch regressions/testSuiteName2.out
	touch regressions/testSuiteName2.err
	touch regressions/testSuiteSegfault.testFunction.out
	touch regressions/testSuiteSegfault.testFunction.err
	touch regressions/testSuiteSegfault.testPrint.out
	touch regressions/testSuiteSegfault.testPrint.err
	touch regressions/testSuiteTest2.out
	touch regressions/testSuiteTest2.err
	-./test
	-cd regressions && $(PYTHON) ../../cu-check-regressions
	@echo ""
	@echo "======= SEGFAULT: ========="
	@echo ""
	-./test-segfault
	-cd regressions && $(PYTHON) ../../cu-check-regressions

test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) -L../ -lcu

test-segfault: test-segfault.c
	$(CC) $(CFLAGS) -o $@ $^ -L../ -lcu

clean:
	rm -f *.o
	rm -f test
	rm -f test-segfault
	rm -f tmp.*
	rm -rf regressions


.PHONY: all clean
