#include <stdio.h>
#include "cu.h"

/**
 * Definition of test function
 */
TEST(testFunction)
{
    printf("Hello, world!\n");
    assertTrue(0);
}

TEST(testFunction2)
{
    assertEquals(1, 1);
    assertNotEquals(1, 0);
}

TEST(testFunction3)
{
    assertEquals(0, 1);
    assertNotEquals(1, 0);
}

/**
 * Composition of tests into testsuite
 */
TEST_SUITE(testSuiteName)
{
    TEST_ADD(testFunction), /* Add test to testsuite */
    TEST_ADD(testFunction2),
    TEST_ADD(testFunction3),
    TEST_SUITE_CLOSURE /* By this must end all lists of tests */
};

/**
 * Another testsuite.
 */
TEST_SUITE(testSuiteName2)
{
    TEST_ADD(testFunction2),
    TEST_SUITE_CLOSURE
};

int main(int argc, char *argv[])
{
    CU_SET_OUT_PREFIX("regressions/"); /* define prefix for files written
                                           by testsuites */
    RUN(testSuiteName); /* Run testsuite in its own process and stdout is
                           redirected to regressions/tmp.testSuiteName.out
                           and stderr to regressions/tmp.testSuiteName.err
                           according to CU_SET_OUT_PREFIX called before */
    RUN(testSuiteName2);

    PRINT_RESULTS; /* Print results */

    return 0;
}
