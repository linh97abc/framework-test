/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <utest.h>
#include <stdint.h>
#include <stdio.h>

static void test_empty_test(void)
{
}

static void test_assert_tests(void)
{
	uassert_true(1, NULL);
	uassert_false(0, NULL);
	uassert_is_null(NULL, NULL);
	uassert_not_null("foo", NULL);
	uassert_equal(1, 1, NULL);
	uassert_equal_ptr(NULL, NULL, NULL);
	utest_fail();
}

static void test_assert_mem_equal(void)
{
	static const uint32_t expected[4] = {
		0x1234,
		0x5678,
		0x9ABC,
		0xDEF0};
	uint32_t actual[4] = {0};

	memcpy(actual, expected, sizeof(actual));
	uassert_mem_equal(actual, expected, sizeof(expected), NULL);
}

static void test_skip_setup()
{
	puts(__func__);
}

static void test_skip_teardown()
{
	puts(__func__);
}

static void test_skip(void)
{
	utest_skip();
}

void test_main(void)
{
	utest_test_suite(framework_tests,
					 utest_unit_test_setup_teardown(test_skip, test_skip_setup, test_skip_teardown),
					 utest_unit_test(test_empty_test),
					 utest_unit_test(test_assert_tests),
					 utest_unit_test(test_assert_mem_equal));

	utest_run_test_suite(framework_tests);
}
