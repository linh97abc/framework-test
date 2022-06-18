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

namespace unittest
{
	class TestCase
	{
	protected:
		const char *name;

	public:
		virtual void SetUp(void) {}
		virtual void TearDown(void) {}

		virtual void run(void) = 0;
		const char *Name() { return this->name; }
	};
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
/**
 * @brief Define a test case
 *
 * This should be called as an argument to TEST_SUITE.
 *
 * @param ts_name Test suite name
 * @param tc_name Test case name
 */
#define TEST_CASE(ts_name, tc_name) &TEST_CASE_NAME(ts_name, tc_name)

/**
 * @brief Define a test suite
 *
 * This function should be called in the following fashion:
 * ```{.c}
 *      TEST_SUITE(test_suite_name,
 *              TEST_CASE(test_suite_name, test_case_name),
 *              TEST_CASE(test_suite_name, test_case_name)
 *      );
 *
 *      utest_run_test_suite(test_suite_name);
 * ```
 *
 * @param suite Name of the testing suite
 */
#define TEST_SUITE(suite, ...) unittest::TestCase *_test_suite_##suite[] = {__VA_ARGS__, {0}}

/**
 * @}
 */

namespace unittest
{

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
	int run_test_suite(const char *name, unittest::TestCase **suite);
}

/**
 * @brief Run test suite
 *
 * @param suite Name of the testing suite
 */
#define RUN_TEST_SUITE(suite) unittest::run_test_suite(#suite, _test_suite_##suite)

#endif /* _TESTSUITE__TEST_TEST_H_ */
