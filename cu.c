/***
 * CU - C unit testing framework
 * ---------------------------------
 * Copyright (c)2007 Daniel Fiser <danfis@danfis.cz>
 *
 *
 *  This file is part of CU.
 *
 *  CU is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  CU is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "cu.h"

const char *cu_current_test;
const char *cu_current_test_suite;
int cu_success_test_suites = 0;
int cu_fail_test_suites = 0;
int cu_success_tests = 0;
int cu_fail_tests = 0;
int cu_success_checks = 0;
int cu_fail_checks = 0;

char cu_out_prefix[CU_OUT_PREFIX_LENGTH+1] = "";


/* globally used file descriptor for reading/writing messages */
int fd;

/* indicate if test was failed */
int test_failed;

/* codes of messages */
#define CHECK_FAILED '0'
#define CHECK_SUCCEED '1'
#define TEST_FAILED '2'
#define TEST_SUCCEED '3'
#define TEST_SUITE_FAILED '4'
#define TEST_SUITE_SUCCEED '5'
#define END '6'

/* predefined messages */
#define MSG_CHECK_SUCCEED write(fd, "1\n", 2)
#define MSG_TEST_FAILED write(fd, "2\n", 2)
#define MSG_TEST_SUCCEED write(fd, "3\n", 2)
#define MSG_TEST_SUITE_FAILED write(fd, "4\n", 2)
#define MSG_TEST_SUITE_SUCCEED write(fd, "5\n", 2)
#define MSG_END write(fd, "6\n", 2)

/* length of buffers */
#define BUF_LEN 100


static void redirect_out_err(const char *testName);
static void close_out_err(void);
static void run_test_suite(const char *ts_name, cu_test_suite_t *ts);
static void receive_messages(void);

void cu_run(const char *ts_name, cu_test_suite_t *ts)
{
    int pipefd[2];
    int pid;

    if (pipe(pipefd) == -1){
        perror("Pipe error");
        exit(-1);
    }

    fprintf(stdout, " -> %s [IN PROGESS]\n", ts_name);
    fflush(stdout);

    pid = fork();
    if (pid < 0){
        perror("Fork error");
        exit(-1);
    }

    if (pid == 0){
        /* close read end of pipe */
        close(pipefd[0]);

        fd = pipefd[1];

        /* run testsuite, messages go to fd */
        run_test_suite(ts_name, ts);

        MSG_END;
        close(fd);
    }else{
        /* close write end of pipe */
        close(pipefd[1]);

        fd = pipefd[0];

        /* receive and interpret all messages */
        receive_messages();

        /* wait for children */
        wait(NULL);

        close(fd);

        fprintf(stdout, " -> %s [DONE]\n\n", ts_name);
        fflush(stdout);
    }

}

static void run_test_suite(const char *ts_name, cu_test_suite_t *ts)
{
    int test_suite_failed = 0;

    /* set up current test suite name for later messaging... */
    cu_current_test_suite = ts_name;

    /* redirect stdout and stderr */
    redirect_out_err(cu_current_test_suite);

    while (ts->name != NULL && ts->func != NULL){
        test_failed = 0;

        /* set up name of test for later messaging */
        cu_current_test = ts->name;

        /* run test */
        (*(ts->func))();

        if (test_failed){
            MSG_TEST_FAILED;
            test_suite_failed = 1;
        }else{
            MSG_TEST_SUCCEED;
        }

        ts++; /* next test in test suite */
    }

    if (test_suite_failed){
        MSG_TEST_SUITE_FAILED;
    }else{
        MSG_TEST_SUITE_SUCCEED;
    }

    /* close redirected stdout and stderr */
    close_out_err();
}


static void receive_messages(void)
{
    char buf[BUF_LEN]; /* buffer */
    int buf_read; /* how many chars stored in buf */
    int state = 0; /* 0 - waiting for code, 1 - copy msg to stdout */
    int i;
    int end = 0; /* end of messages? */

    while((buf_read = read(fd, buf, BUF_LEN)) > 0 && !end){
        for (i=0; i < buf_read; i++){

            /* copy msg to stderr */
            if (state == 1){
                fprintf(stdout, "%c", buf[i]);
                fflush(stdout);
            }

            /* reset on '\n' in msg */
            if (buf[i] == '\n'){
                state = 0;
                continue;
            }

            if (state == 0){
                if (buf[i] == CHECK_FAILED){
                    cu_fail_checks++;
                    state = 1;
                }else if (buf[i] == CHECK_SUCCEED){
                    cu_success_checks++;
                }else if (buf[i] == TEST_FAILED){
                    cu_fail_tests++;
                }else if (buf[i] == TEST_SUCCEED){
                    cu_success_checks++;
                }else if (buf[i] == TEST_SUITE_FAILED){
                    cu_fail_test_suites++;
                }else if (buf[i] == TEST_SUITE_SUCCEED){
                    cu_success_test_suites++;
                }else if (buf[i] == END){
                    end = 1;
                    break;
                }
            }
        }
    }
}

void cu_success_assertation(void)
{
    MSG_CHECK_SUCCEED;
}

void cu_fail_assertation(const char *file, int line, const char *msg)
{
    char buf[BUF_LEN];
    int len;

    len = snprintf(buf, BUF_LEN, "%c%s:%d (%s::%s) :: %s\n",
            CHECK_FAILED,
            file, line, cu_current_test_suite, cu_current_test, msg);
    write(fd, buf, len);

    /* enable test_failed flag */
    test_failed = 1;
}

void cu_print_results(void)
{
    fprintf(stdout, "\n");
    fprintf(stdout, "==================================================\n");
    fprintf(stdout, "|               |  failed  |  succeed  |  total  |\n");
    fprintf(stdout, "|------------------------------------------------|\n");
    fprintf(stdout, "| assertations: |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_checks, cu_success_checks,
                cu_success_checks+cu_fail_checks);
    fprintf(stdout, "| tests:        |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_tests, cu_success_tests,
                cu_success_tests+cu_fail_tests);
    fprintf(stdout, "| tests suites: |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_test_suites, cu_success_test_suites,
                cu_success_test_suites+cu_fail_test_suites);
    fprintf(stdout, "==================================================\n");
}

void cu_set_out_prefix(const char *str)
{
    strncpy(cu_out_prefix, str, CU_OUT_PREFIX_LENGTH);
}

static void redirect_out_err(const char *test_name)
{
    char buf[100];

    snprintf(buf, 99, "%stmp.%s.out", cu_out_prefix, test_name);
    if (freopen(buf, "w", stdout) == NULL){
        perror("Redirecting of stdout failed");
        exit(-1);
    }

    snprintf(buf, 99, "%stmp.%s.err", cu_out_prefix, test_name);
    if (freopen(buf, "w", stderr) == NULL){
        perror("Redirecting of stderr failed");
        exit(-1);
    }
}

static void close_out_err(void)
{
    fclose(stdout);
    fclose(stderr);
}
