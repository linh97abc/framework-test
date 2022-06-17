/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief Testsuite
 */

/**
 * @brief Tests (utest)
 * @defgroup all_tests Tests
 * @{
 * @}
 */

#ifndef _TESTSUITE_INCLUDE_UTEST_H_
#define _TESTSUITE_INCLUDE_UTEST_H_

/**
 * @defgroup utest testing suite
 */

#define CONFIG_uassert_VERBOSE 1
#define CONFIG_utest_MOCKING

#define CONFIG_X86 1

#include <stdio.h>
#define PRINT printf

#include <test_assert.h>
#include <test_mock.h>
#include <test_deprecated.h>
#include <tc_util.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Entry function.
 *
 */
void utest_main(void);

/**
 * @brief Fail the currently running test.
 *
 * This is the function called from failed assertions and the like. You
 * probably don't need to call it yourself.
 */
void utest_fail(void);

/**
 * @brief Pass the currently running test.
 *
 * Normally a test passes just by returning without an assertion failure.
 * However, if the success case for your test involves a fatal fault,
 * you can call this function from k_sys_fatal_error_handler to indicate that
 * the test passed before aborting the thread.
 */
void utest_pass(void);

/**
 * @brief Skip the current test.
 */
void utest_skip(void);

#define TEST(ts_name, tc_name) void TEST_CASE_NAME(ts_name, tc_name)(void)

#define TEST_SETUP(ts_name) void TEST_SETUP_NAME(ts_name)(void)

#define TEST_TEARDOWN(ts_name) void TEST_TEARDOWN_NAME(ts_name)(void)

#ifdef __cplusplus
}
#endif

#endif /* _TESTSUITE_INCLUDE_UTEST_H_ */
