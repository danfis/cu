#include <stdio.h>

#include "cu.h"

const char *cu_current_test;
const char *cu_current_test_suite;
int cu_success_test_suites = 0;
int cu_fail_test_suites = 0;
int cu_success_tests = 0;
int cu_fail_tests = 0;
int cu_success_checks = 0;
int cu_fail_checks = 0;

void cu_run(const char *ts_name, cu_test_suite_t *test_suite)
{
    cu_test_suite_t *current = test_suite;
    int fail_tests = cu_fail_tests;
    int fail_checks;

    cu_current_test_suite = ts_name;
    while (current->name != NULL && current->func != NULL){
        fail_checks = cu_fail_checks;
        cu_current_test = current->name;

        (*(current->func))();

        if (cu_fail_checks > fail_checks){
            cu_fail_tests++;
        }else{
            cu_success_tests++;
        }

        current++;
    }

    if (cu_fail_tests > fail_tests){
        cu_fail_test_suites++;
    }else{
        cu_success_test_suites++;
    }
}


void cu_success_assertation(void)
{
    cu_success_checks++;
}

void cu_fail_assertation(const char *file, int line, const char *msg)
{
    cu_fail_checks++;
    fprintf(stderr, "%s:%d (%s::%s) :: %s\n", file, line,
            cu_current_test_suite, cu_current_test, msg);
    fflush(stderr);
}

void cu_print_results(void)
{
    printf("\n");
    printf("==================================================\n");
    printf("|               |  failed  |  succeed  |  total  |\n");
    printf("|------------------------------------------------|\n");
    printf("| assertations: |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_checks, cu_success_checks,
                cu_success_checks+cu_fail_checks);
    printf("| tests:        |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_tests, cu_success_tests,
                cu_success_tests+cu_fail_tests);
    printf("| tests suites: |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_test_suites, cu_success_test_suites,
                cu_success_test_suites+cu_fail_test_suites);
    printf("==================================================\n");
}
