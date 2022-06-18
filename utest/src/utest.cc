/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/unittest.h"

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
		testing::Message() << "Test " << TC_NAME(test->TS_Name(), test->Name()) << " failed: Unused mock parameter values\n";
		ret = TC_FAIL;
	}
	else if (!ret && mock_status == 2)
	{
		testing::Message() << "Test" << TC_NAME(test->TS_Name(), test->Name()) << "failed: Unused mock return values\n";
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

	testing::Message() << TC_START(test->TS_Name(), test->Name());

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
		testing::Message() << TC_END_RESULT(TC_SKIP);
	}
	else
	{
		testing::Message() << TC_END_RESULT(ret);
	}

	return ret;
}

/* End Porting */

static inline bool isInRunLists(const testing::Test *tc, const char *suite_name, const char *tc_name)
{
	if ((suite_name != nullptr) && strcmp(tc->TS_Name(), suite_name))
	{
		return false;
	}
	if ((tc_name != nullptr) && strcmp(tc->Name(), tc_name))
	{
		return false;
	}

	return true;
}

static int RunTestSuite(testing::Test *ts, const char *suite_name, const char *tc_name)
{
	testing::Test *it = ts;

	int fail = 0;
	unsigned int test_num = 0;

	if (test_status < 0)
	{
		return test_status;
	}

	while (it)
	{
		if (isInRunLists(it, suite_name, tc_name))
		{
			if (run_test(it) == TC_FAIL)
			{
				fail++;
			}

			test_num++;

			if (fail && FAIL_FAST)
			{
				break;
			}
		}

		it = it->Next();
	}

	test_status = (test_status || fail) ? 1 : 0;

	return fail;
}

static void RunAllTest(const char *suite_name, const char *tc_name)
{
	testing::BaseTestManager *allTest = testing::BaseTestManager::getAllTest();

	while (allTest)
	{
		testing::Test *ts = allTest->GetTestSuite();

		RunTestSuite(ts, suite_name, tc_name);
		allTest = allTest->Next();
	}
}

static void end_report(void)
{
	if (test_status)
	{
		testing::Message() << TC_END_REPORT(TC_FAIL);
	}
	else
	{
		testing::Message() << TC_END_REPORT(TC_PASS);
	}
}

void utest_main(const char *suite_name, const char *tc_name)
{
	if (!testing::mock::__init())
	{
		RunAllTest(suite_name, tc_name);
	}

	end_report();

	DO_END_TEST();
}
