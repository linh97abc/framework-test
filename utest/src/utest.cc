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

static int cleanup_test(const testing::Test *test)
{
	int ret = TC_PASS;
	int mock_status;

	mock_status = testing::mock::__cleanup();

	if (!ret && mock_status == 1)
	{
		testing::Message() << "Test " << TC_NAME(test->TS_Name(), test->Name()) << " failed: Unused mock parameter values";
		ret = TC_FAIL;
	}
	else if (!ret && mock_status == 2)
	{
		testing::Message() << "Test" << TC_NAME(test->TS_Name(), test->Name()) << "failed: Unused mock return values";
		ret = TC_FAIL;
	}
	else
	{
		;
	}

	return ret;
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

void testing::Test::Run()
{
	testing::Message() << TC_START(this->TS_Name(), this->Name());

	if (setjmp(test_fail))
	{
		this->tc_res = TC_FAIL;
		goto out;
	}

	if (setjmp(test_skip))
	{
		this->tc_res = TC_SKIP;
		goto out;
	}

	if (setjmp(test_pass))
	{
		this->tc_res = TC_PASS;
		goto out;
	}

	phase = TEST_PHASE_SETUP;
	this->SetUp();
	phase = TEST_PHASE_TEST;
	this->TestBody();
out:
	phase = TEST_PHASE_TEARDOWN;
	this->TearDown();
	phase = TEST_PHASE_FRAMEWORK;

	if (cleanup_test(this) != TC_PASS)
	{
		this->tc_res = TC_FAIL;
	}

	if (this->tc_res == TC_SKIP)
	{
		testing::Message() << TC_END_RESULT(TC_SKIP);
	}
	else
	{
		testing::Message() << TC_END_RESULT(this->tc_res);
	}
}

/* End Porting */

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
		if (it->isSameName(suite_name, tc_name))
		{
			it->Run();
			if (it->GetResult() == TC_FAIL)
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

bool testing::Test::isSameName(const char *suite_name, const char *tc_name) const
{
	if ((suite_name != nullptr) && strcmp(this->__ts_name, suite_name))
	{
		return false;
	}
	if ((tc_name != nullptr) && strcmp(this->name, tc_name))
	{
		return false;
	}

	return true;
}

int testing::UnitTest::Run(const char *suite_name, const char *tc_name)
{
	int fail;

	if (!testing::mock::__init())
	{
		fail = RunTestSuite(this->list, suite_name, tc_name);
	}

	end_report();

	DO_END_TEST();

	return fail;
}

void testing::UnitTest::AddTest(testing::Test *tc)
{
	UnitTest *manager = GetInstance();

	if (manager->list)
	{
		Test *it = manager->list;
		while (it->next)
		{
			it = it->next;
		}
		it->next = tc;
	}
	else
	{
		manager->list = tc;
	}

	tc->next = nullptr;
}

int utest_main(const char *suite_name, const char *tc_name)
{
	return testing::UnitTest::GetInstance()->Run(suite_name, tc_name);
}
