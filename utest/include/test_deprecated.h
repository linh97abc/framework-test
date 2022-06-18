/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief testing framework _test_deprecated.
 */

#ifndef _TESTSUITE__TEST_TEST_H_
#define _TESTSUITE__TEST_TEST_H_

#include <stdbool.h>
#include <stdint.h>
#include <tc_util.h>

namespace testing
{
	class Test
	{
	protected:
		const char *name;

	public:
		virtual void SetUp(void) {}
		virtual void TearDown(void) {}

		virtual void run(void) = 0;
		const char *Name() { return this->name; }
	};

	/**
	 * @brief Run a test suite.
	 *
	 * Internal implementation. Do not call directly. This will run the full test suite along with some
	 * checks for fast failures and initialization.
	 *
	 * @param name The name of the suite to run.
	 * @param suite Pointer to the first unit test.
	 * @return Negative value if the test suite never ran; otherwise, return the number of failures.
	 */
	int run_test_suite(const char *name, testing::Test **suite);
};

	/**
	 * @defgroup utest_test_deprecated utest testing macros
	 * @ingroup utest
	 *
	 * This module eases the testing process by providing helpful macros and other
	 * testing structures.
	 *
	 * @{
	 */

#define TEST_ID_INFO(ts_name, tc_name) "TEST(" #ts_name ", " #tc_name ")"
#define TEST_CASE_NAME(ts_name, tc_name) _test_##ts_name##_##tc_name
#define TEST_CASE_CLASS_NAME(ts_name, tc_name) _test_##ts_name##_##tc_name##_class

#endif /* _TESTSUITE__TEST_TEST_H_ */
