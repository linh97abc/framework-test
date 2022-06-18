/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <unittest.h>
#include <stdio.h>

enum Test_phase
{
	TEST_PHASE_SETUP,
	TEST_PHASE_TEST,
	TEST_PHASE_TEARDOWN,
	TEST_PHASE_FRAMEWORK
};

static enum Test_phase phase = TEST_PHASE_FRAMEWORK;

static int test_status;

static int cleanup_test(testing::Test *test)
{
	int ret = TC_PASS;
	int mock_status;

	mock_status = testing::mock::__cleanup();

	if (!ret && mock_status == 1)
	{
		PRINT("Test %s failed: Unused mock parameter values\n",
			  test->Name());
		ret = TC_FAIL;
	}
	else if (!ret && mock_status == 2)
	{
		PRINT("Test %s failed: Unused mock return values\n",
			  test->Name());
		ret = TC_FAIL;
	}
	else
	{
		;
	}

	return ret;
}

static void run_test_functions(testing::Test *test)
{
	phase = TEST_PHASE_SETUP;
	test->SetUp();
	phase = TEST_PHASE_TEST;
	test->run();
}

/* Start Porting */
static void DO_END_TEST() {}

/* Static code analysis tool can raise a violation that the standard header
 * <setjmp.h> shall not be used.
 *
 * setjmp is using in a test code, not in a runtime code, it is acceptable.
 * It is a deliberate deviation.
 */
#include <setjmp.h> /* parasoft-suppress MISRAC2012-RULE_21_4-a MISRAC2012-RULE_21_4-b*/
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define FAIL_FAST 0

static jmp_buf test_fail;
static jmp_buf test_skip;
static jmp_buf test_pass;

namespace testing
{
	void fail(void)
	{
		longjmp(test_fail, 1);
	}

	void skip(void)
	{
		longjmp(test_skip, 1);
	}

	void pass(void)
	{
		longjmp(test_pass, 1);
	}
}

static int run_test(testing::Test *test)
{
	int ret = TC_PASS;
	int skip = 0;

	TC_START(test->Name());

	if (setjmp(test_fail))
	{
		ret = TC_FAIL;
		goto out;
	}

	if (setjmp(test_skip))
	{
		skip = 1;
		ret = TC_SKIP;
		goto out;
	}

	if (setjmp(test_pass))
	{
		ret = TC_PASS;
		goto out;
	}

	run_test_functions(test);
out:
	phase = TEST_PHASE_TEARDOWN;
	test->TearDown();
	phase = TEST_PHASE_FRAMEWORK;

	if (cleanup_test(test) != TC_PASS)
	{
		ret = TC_FAIL;
	}

	if (ret == TC_SKIP)
	{
		Z_TC_END_RESULT(TC_SKIP);
	}
	else
	{
		Z_TC_END_RESULT(ret);
	}

	return ret;
}

/* End Porting */

int testing::run_test_suite(const char *name, testing::Test **suite)
{
	int fail = 0;
	unsigned int test_num = 0;

	if (test_status < 0)
	{
		return test_status;
	}

	TC_SUITE_START(name);
	while (suite[test_num])
	{
		if (run_test(suite[test_num]) == TC_FAIL)
		{
			fail++;
		}

		test_num++;

		if (fail && FAIL_FAST)
		{
			break;
		}
	}

	TC_SUITE_END(name, (fail > 0 ? TC_FAIL : TC_PASS));

	test_status = (test_status || fail) ? 1 : 0;

	return fail;
}

static void end_report(void)
{
	if (test_status)
	{
		TC_END_REPORT(TC_FAIL);
	}
	else
	{
		TC_END_REPORT(TC_PASS);
	}
}

void utest_main(void)
{
	if (!testing::mock::__init())
	{
		testing::RunAllTest();
	}

	end_report();

	DO_END_TEST();
}
