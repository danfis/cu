# CU

CU is simple unit testing framework for handling automated tests in C.

CU provides a simple interface for defining unit tests using macros.
Each test suite runs in a separate process - test suites does not influence
each other and any failure (such as segfault) does not break up the whole test.
CU also provides script for regression tests based on output of test suites.

## License
CU is licensed under OSI-approved 3-clause BSD License, text of license
is distributed along with source code in BSD-LICENSE file.
Each file should include license notice, the rest should be considered as
licensed under 3-clause BSD License.

## Tutorial

### 1. Prepare CU
Create directory for your tests, say, tests:
```sh
  $ mkdir tests && cd tests
```

Obtain source codes of CU, unzip it, rename the directory and compile it:
```sh
  $ wget https://github.com/danfis/cu/archive/master.zip
  $ unzip master.zip
  $ mv cu-master cu
  $ make -C cu
```

As last thing, create directory for files containing outputs from testsuites - call this directory for exampleregressions:
```sh
  $ mkdir regressions
```

### 2. Define Tests
Tests will be written in file, say, t1.c:

1. First include CU macro definitions:
```cpp
  #include "cu/cu.h"
```

2. Then test functions can be defined:
```cpp
  #include <stdio.h>

  TEST(test1) {
      assertTrue(1);
      printf("Hello from test1\n");
      assertEquals(1, 1);
  }

  TEST(test2) {
      assertFalse(0);
      assertNotEquals(1, 2);
      fprintf(stderr, "Hello from test2\n");
  }

  TEST(test3) {
      assertFalse(1);
  }
```

### 3. Create Test Suite From Tests
Test functions are organized into test suites:

1. Create header file t1.h with definition of T1 testsuite consisting of tests from file t1.c.
```cpp
  #include "cu/cu.h"

  // Declarations of tests
  TEST(test1);
  TEST(test2);
  TEST(test3);

  // Collect tests into test suite
  TEST_SUITE(T1) {
      TEST_ADD(test1),
      TEST_ADD(test2),
      TEST_ADD(test3),
      TEST_SUITE_CLOSURE
  };
```

2. Then C file test.c with main function can be created:
```cpp
  // Include definition of T1 test suite
  #include "t1.h"

  // This struct contains all test suites
  TEST_SUITES {
      TEST_SUITE_ADD(T1), // add T1 test suite
      TEST_SUITES_CLOSURE
  };

  int main(int argc, char *argv[])
  {
      // Set up directory where are stored files with outputs from test
      // suites
      CU_SET_OUT_PREFIX("regressions/");

      // Run all test suites
      CU_RUN(argc, argv);

      return 0;
  }
```

### 3. Compile & Run
Now, everything is prepared and all tests can be compiled, linked and run:

1. We can use this Makefile (download it from here):
```make
  CC ?= gcc
  CFLAGS = -g -Wall -pedantic --std=gnu99
  LDFLAGS = -Lcu/ -lcu

  OBJS = t1.o test.o

  all: test

  test: $(OBJS)
     $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

  %.o: %.c %.h
     $(CC) $(CFLAGS) -c -o $@ $<
  %.o: %.c
     $(CC) $(CFLAGS) -c -o $@ $<

  clean:
     rm -f *.o test

  .PHONY: all clean
```

2. and simply type
```sh
  $ make
```

3. After compilation, a binary test should emerge. Simply call this binary to run all tests:
```sh
  $ ./test
```

4. Output should look like this:
```
   -> T1 [IN PROGESS]
      --> Running test1...
      --> Running test2...
      --> Running test3...
  t1.c:17 (T1::test3) :: 1 is not false
   -> T1 [DONE]


  ==================================================
  |               |  failed  |  succeed  |  total  |
  |------------------------------------------------|
  | assertations: |       1  |        4  |      5  |
  | tests:        |       1  |        2  |      3  |
  | tests suites: |       1  |        0  |      1  |
  ==================================================
```

### 5. Check Regressions
While running tests stdout and stderr are redirected to files.
Each test suite has its own files stored in
<PREFIX>tmp.<TSNAME>.{out,err} where <PREFIX> is prefix defined
using *CU_SET_OUT_PREFIX* macro and <TSNAME> is name of test suite.

In our case, it means that test suite T1 has its stdout stored
in regressions/tmp.T1.out and stderr in regressions/tmp.T1.err.
If you look there you will see that tmp.T1.out contains:
```
  Hello from test1
```

and tmp.T1.err contains:
```
  Hello from test2
```

Why is it so should be obvious from t1.c source code.

CU provides the python script ***check-regressions*** which checks
all files generated from test suites for regressions. This
script accepts several options, to see them all type:
```sh
  $ ./cu/check-regressions --help
```

And back to example.

If you run ***check-regressions*** on directory regressions:
```sh
  $ ./cu/check-regressions regressions
```

You get this output:
```
  Processing directory 'regressions/':

  === Can't compare tmp.T1.err T1.err, bacause T1.err does not exist!
  === Can't compare tmp.T1.out T1.out, bacause T1.out does not exist!
```

This means that script has nothing to compare with. So create some output files:
```sh
  $ touch regressions/T1.{out,err}
```

And run ***check-regressions*** again:
```
  Processing directory 'regressions/':

  Comparing tmp.T1.err and T1.err  [ FAILED ]
   --> Diff has 2 lines
   --> Diff:
     | 1d0
     | < Hello from test2
  Comparing tmp.T1.out and T1.out  [ FAILED ]
   --> Diff has 2 lines
   --> Diff:
     | 1d0
     | < Hello from test1
```

Now, you can see diff between T1.{out,err} files which are taken as
base files and newly generated files tmp.T1.{out,err}.

So, fixing this problem is easy:
```sh
  $ cp regressions/tmp.T1.out regressions/T1.out
  $ cp regressions/tmp.T1.err regressions/T1.err
```

Run check-regressions again:
```
  Processing directory 'regressions/':

  Comparing tmp.T1.err and T1.err  [ OK ]
  Comparing tmp.T1.out and T1.out  [ OK ]
```
