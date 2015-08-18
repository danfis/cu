/***
 * CU - C unit testing framework
 * ---------------------------------
 * Copyright (c)2007-2015 Daniel Fiser <danfis@danfis.cz>
 *
 *  This file is part of CU.
 *
 *  Distributed under the OSI-approved BSD License (the "License");
 *  see accompanying file BDS-LICENSE for details or see
 *  <http://www.opensource.org/licenses/bsd-license.php>.
 *
 *  This software is distributed WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the License for more information.
 */

#ifndef _CU_H_
#define _CU_H_

#ifdef CU_ENABLE_TIMER
# include <time.h>
#endif /* CU_ENABLE_TIMER */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***** PUBLIC API *****/
/**
 * Define test
 */
#ifdef __cplusplus
# define TEST(name) \
    extern "C" void name(void)
#else /* __cplusplus */
# define TEST(name) \
    void name(void)
#endif /* __cplusplus */

/**
 * Define testsuite
 */
#define TEST_SUITE(name) \
    cu_test_suite_t test_suite_##name[] =
/**
 * Must be on the end of list of tests.
 */
#define TEST_SUITE_CLOSURE \
    { NULL, NULL }

#define TEST_SUITES \
    cu_test_suites_t cu_test_suites[] =
#define TEST_SUITES_CLOSURE \
    { NULL, NULL }
#define TEST_SUITE_ADD(name) \
    { #name, test_suite_##name }

/**
 * Add test to testsuite
 */
#define TEST_ADD(name) \
    { #name, name }

#define CU_RUN(argc, argv) \
    cu_run(argc, argv)

/**
 * Set prefix for output files. Must contain trailing /.
 */
#define CU_SET_OUT_PREFIX(str) \
    cu_set_out_prefix(str)

/**
 * Enables (disables) output per test instead of a whole testsuite.
 */
#define CU_SET_OUT_PER_TEST(yes) \
    cu_set_out_per_test(yes)

/**
 * Assertions
 * Assertions with suffix 'M' (e.g. assertTrueM) is variation of macro
 * where is possible to specify error message.
 */
#define assertTrueM(a, message) \
    if (a){ \
        cu_success_assertion(); \
    }else{ \
        cu_fail_assertion(__FILE__, __LINE__, message); \
    }
#define assertTrue(a) \
    assertTrueM((a), #a " is not true")

#define assertFalseM(a, message) \
    assertTrueM(!(a), message)
#define assertFalse(a) \
    assertFalseM((a), #a " is not false")

#define assertEqualsM(a,b,message) \
    assertTrueM((a) == (b), message)
#define assertEquals(a,b) \
    assertEqualsM((a), (b), #a " not equals " #b)

#define assertNotEqualsM(a,b,message) \
    assertTrueM((a) != (b), message)
#define assertNotEquals(a,b) \
    assertNotEqualsM((a), (b), #a " equals " #b)
/***** PUBLIC API END *****/


#include <unistd.h>

#define CU_MAX_NAME_LENGTH 30

typedef void (*cu_test_func_t)(void);
typedef struct _cu_test_suite_t {
    const char *name;
    cu_test_func_t func;
} cu_test_suite_t;
typedef struct _cu_test_suites_t {
    const char *name;
    cu_test_suite_t *test_suite;
} cu_test_suites_t;

extern cu_test_suites_t cu_test_suites[];

void cu_run(int argc, char *argv[]);
void cu_success_assertion(void);
void cu_fail_assertion(const char *file, int line, const char *msg);
void cu_set_out_prefix(const char *str);
void cu_set_out_per_test(int yes);

/** Timer **/
#ifdef CU_ENABLE_TIMER

/**
 * Returns value of timer. (as timespec struct)
 */
const struct timespec *cuTimer(void);

/**
 * Starts timer.
 */
void cuTimerStart(void);

/**
 * Stops timer and record elapsed time from last call of cuTimerStart().
 * Returns current value of timer.
 */
const struct timespec *cuTimerStop(void);
#endif /* CU_ENABLE_TIMER */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
