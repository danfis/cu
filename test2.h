#ifndef _TEST2_H_
#define _TEST2_H_

/**
 * Declaration of test function (implemented in test2.c)
 */
TEST(testTest2Function);

/**
 * Definition of all tests in this test suite
 */
TEST_SUITE(testSuiteTest2) {
    TEST_ADD(testTest2Function),
    TEST_SUITE_CLOSURE
};

#endif
