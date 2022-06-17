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

#ifdef __cplusplus
extern "C"
{
#endif

	struct unit_test
	{
		const char *name;
		void (*test)(void);
		void (*setup)(void);
		void (*teardown)(void);
		uint32_t thread_options;
#ifdef CONFIG_TEST_PRINT_DETAIL_RESULT
		int status;
#endif
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
	int z_utest_run_test_suite(const char *name, struct unit_test *suite);

	/**
	 * @defgroup utest_test_deprecated utest testing macros
	 * @ingroup utest
	 *
	 * This module eases the testing process by providing helpful macros and other
	 * testing structures.
	 *
	 * @{
	 */

	/**
	 * @brief Do nothing, successfully.
	 *
	 * Unit test / setup function / teardown function that does
	 * nothing, successfully. Can be used as a parameter to
	 * utest_unit_test_setup_teardown().
	 */
	static inline void unit_test_noop(void)
	{
	}

#define TEST_ID_INFO(ts_name, tc_name) "TEST(" #ts_name", "#tc_name")"
#define TEST_SETUP_NAME(ts_name) _testsuite_##ts_name##_setup
#define TEST_TEARDOWN_NAME(ts_name) _testsuite_##ts_name##_teardown
#define TEST_CASE_NAME(ts_name, tc_name) _test_##ts_name##_##tc_name

/**
 * @brief Define a test case
 *
 * This should be called as an argument to TEST_SUITE.
 *
 * @param ts_name Test suite name
 * @param tc_name Test case name
 */
#define TEST_CASE(ts_name, tc_name) \
	{                                                       \
		TEST_ID_INFO(ts_name, tc_name), \
		TEST_CASE_NAME(ts_name, tc_name), \
		TEST_SETUP_NAME(ts_name),\
		TEST_TEARDOWN_NAME(ts_name), 0               \
	}

/**
 * @brief Define a test suite
 *
 * This function should be called in the following fashion:
 * ```{.c}
 *      utest_test_suite(test_suite_name,
 *              utest_unit_test(test_function),
 *              utest_unit_test(test_other_function)
 *      );
 *
 *      utest_run_test_suite(test_suite_name);
 * ```
 *
 * @param suite Name of the testing suite
 */
#define TEST_SUITE(suite, ...) \
	static struct unit_test _##suite[] = {__VA_ARGS__, {0}}
/**
 * @brief Run the specified test suite.
 *
 * @param suite Test suite to run.
 */
#define utest_run_test_suite(suite) \
	z_utest_run_test_suite(#suite, _##suite)

	/**
	 * @}
	 */

#ifdef __cplusplus
}
#endif

#endif /* _TESTSUITE__TEST_TEST_H_ */
