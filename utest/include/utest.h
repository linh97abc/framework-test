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

#define CONFIG_TEST_MOCKING

#define CONFIG_X86 1

#include <stdio.h>
#define PRINT printf

#include <test_assert.h>
#include <test_mock.h>
#include <test_deprecated.h>
#include <tc_util.h>

namespace unittest
{
    /**
     * @brief Fail the currently running test.
     *
     * This is the function called from failed assertions and the like. You
     * probably don't need to call it yourself.
     */
    void fail(void);

    /**
     * @brief Pass the currently running test.
     *
     * Normally a test passes just by returning without an assertion failure.
     * However, if the success case for your test involves a fatal fault,
     * you can call this function from k_sys_fatal_error_handler to indicate that
     * the test passed before aborting the thread.
     */
    void pass(void);

    /**
     * @brief Skip the current test.
     */
    void skip(void);

    /**
     * @brief run all test function, must to implement this.
     *
     */
    void RunAllTest(void);
}

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Entry function.
     *
     */
    void utest_main(void);

#ifdef __cplusplus
}
#endif

#define __DEFINE_TEST(ts_name, tc_name, parent_class)                  \
    class TEST_CASE_CLASS_NAME(ts_name, tc_name) : public parent_class \
    {                                                                  \
        void run(void);                                                \
    public:                                                            \
        TEST_CASE_CLASS_NAME(ts_name, tc_name)                         \
        (const char *name) { this->name = name; }                      \
    };                                                                 \
    TEST_CASE_CLASS_NAME(ts_name, tc_name)                             \
    TEST_CASE_NAME(ts_name, tc_name)                                   \
    (TEST_ID_INFO(ts_name, tc_name));                                  \
    void TEST_CASE_CLASS_NAME(ts_name, tc_name)::run(void)

#define TEST(ts_name, tc_name) __DEFINE_TEST(ts_name, tc_name, unittest::TestCase)
#define TEST_F(ts_name, tc_name) __DEFINE_TEST(ts_name, tc_name, ts_name)

#endif /* _TESTSUITE_INCLUDE_UTEST_H_ */
