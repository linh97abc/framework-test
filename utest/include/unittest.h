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

namespace testing
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
                                                                       \
    public:                                                            \
        TEST_CASE_CLASS_NAME(ts_name, tc_name)                         \
        (const char *name) { this->name = name; }                      \
    };                                                                 \
    TEST_CASE_CLASS_NAME(ts_name, tc_name)                             \
    TEST_CASE_NAME(ts_name, tc_name)                                   \
    (TEST_ID_INFO(ts_name, tc_name));                                  \
    void TEST_CASE_CLASS_NAME(ts_name, tc_name)::run(void)

// Defines a test.
//
// The first parameter is the name of the test suite, and the second
// parameter is the name of the test within the test suite.
//
// The convention is to end the test suite name with "Test".  For
// example, a test suite for the Foo class can be named FooTest.
//
// Test code should appear between braces after an invocation of
// this macro.  Example:
//
//   TEST(FooTest, InitializesCorrectly) {
//     Foo foo;
//     EXPECT_TRUE(foo.StatusIsOK());
//   }

// Note that we call GetTestTypeId() instead of GetTypeId<
// ::testing::Test>() here to get the type ID of testing::Test.  This
// is to work around a suspected linker bug when using Google Test as
// a framework on Mac OS X.  The bug causes GetTypeId<
// ::testing::Test>() to return different values depending on whether
// the call is from the Google Test framework itself or from user test
// code.  GetTestTypeId() is guaranteed to always return the same
// value, as it always calls GetTypeId<>() from the Google Test
// framework.
#define TEST(ts_name, tc_name) __DEFINE_TEST(ts_name, tc_name, testing::Test)

// Defines a test that uses a test fixture.
//
// The first parameter is the name of the test fixture class, which
// also doubles as the test suite name.  The second parameter is the
// name of the test within the test suite.
//
// A test fixture class must be declared earlier.  The user should put
// the test code between braces after using this macro.  Example:
//
//   class FooTest : public testing::Test {
//    protected:
//     void SetUp() override { b_.AddElement(3); }
//
//     Foo a_;
//     Foo b_;
//   };
//
//   TEST_F(FooTest, InitializesCorrectly) {
//     EXPECT_TRUE(a_.StatusIsOK());
//   }
//
//   TEST_F(FooTest, ReturnsElementCountCorrectly) {
//     EXPECT_EQ(a_.size(), 0);
//     EXPECT_EQ(b_.size(), 1);
//   }
#define TEST_F(ts_name, tc_name) __DEFINE_TEST(ts_name, tc_name, ts_name)

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
#define TEST_SUITE(suite, ...) testing::Test *_test_suite_##suite[] = {__VA_ARGS__, {0}}

/**
 * @brief Run test suite
 *
 * @param suite Name of the testing suite
 */
#define RUN_TEST_SUITE(suite) testing::run_test_suite(#suite, _test_suite_##suite)

#endif /* _TESTSUITE_INCLUDE_UTEST_H_ */
