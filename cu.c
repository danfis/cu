#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cu.h"

const char *cu_current_test;
const char *cu_current_test_suite;
int cu_success_test_suites = 0;
int cu_fail_test_suites = 0;
int cu_success_tests = 0;
int cu_fail_tests = 0;
int cu_success_checks = 0;
int cu_fail_checks = 0;

FILE *cu_stdout;
FILE *cu_stderr;
char cu_out_prefix[CU_OUT_PREFIX_LENGTH+1] = "";

static void cu_set_out_err(const char *testName);
static void cu_reset_out_err(void);

void cu_run(const char *ts_name, cu_test_suite_t *test_suite)
{
    cu_test_suite_t *current = test_suite;
    int fail_tests = cu_fail_tests;
    int fail_checks;

    cu_current_test_suite = ts_name;

    cu_set_out_err(cu_current_test_suite);

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

    cu_reset_out_err();
}


void cu_success_assertation(void)
{
    cu_success_checks++;
}

void cu_fail_assertation(const char *file, int line, const char *msg)
{
    cu_fail_checks++;
    fprintf(cu_stderr, "%s:%d (%s::%s) :: %s\n", file, line,
            cu_current_test_suite, cu_current_test, msg);
    fflush(cu_stderr);
}

void cu_print_results(void)
{
    fprintf(cu_stdout, "\n");
    fprintf(cu_stdout, "==================================================\n");
    fprintf(cu_stdout, "|               |  failed  |  succeed  |  total  |\n");
    fprintf(cu_stdout, "|------------------------------------------------|\n");
    fprintf(cu_stdout, "| assertations: |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_checks, cu_success_checks,
                cu_success_checks+cu_fail_checks);
    fprintf(cu_stdout, "| tests:        |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_tests, cu_success_tests,
                cu_success_tests+cu_fail_tests);
    fprintf(cu_stdout, "| tests suites: |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_test_suites, cu_success_test_suites,
                cu_success_test_suites+cu_fail_test_suites);
    fprintf(cu_stdout, "==================================================\n");
}

void cu_set_out_prefix(const char *str)
{
    strncpy(cu_out_prefix, str, CU_OUT_PREFIX_LENGTH);
}

static void cu_set_out_err(const char *test_name)
{
    char buf[100];

    cu_stdout = stdout;
    cu_stderr = stderr;

    snprintf(buf, 99, "%stmp.%s.out", cu_out_prefix, test_name);
    stdout = fopen(buf, "w");
    if (stdout == NULL){
        fprintf(cu_stderr, "Creating file %s failed! Exiting...\n", buf);
        exit(-1);
    }

    snprintf(buf, 99, "%stmp.%s.err", cu_out_prefix, test_name);
    stderr = fopen(buf, "w");
    if (stderr == NULL){
        fprintf(cu_stderr, "Creating file %s failed! Exiting...\n", buf);
        exit(-1);
    }
}

static void cu_reset_out_err(void)
{
    fclose(stdout);
    fclose(stderr);

    stdout = cu_stdout;
    stderr = cu_stderr;
}
