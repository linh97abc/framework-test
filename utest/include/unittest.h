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

#include "test_assert.h"
#include "test_mock.h"
#include "tc_util.h"
#include "port.h"
namespace testing
{
    // The abstract class that all tests inherit from.
    //
    // In Google Test, a unit test program contains one or many TestSuites, and
    // each TestSuite contains one or many Tests.
    //
    // When you define a test using the TEST macro, you don't need to
    // explicitly derive from Test - the TEST macro automatically does
    // this for you.
    //
    // The only time you derive from Test is when defining a test fixture
    // to be used in a TEST_F.  For example:
    //
    //   class FooTest : public testing::Test {
    //    protected:
    //     void SetUp() override { ... }
    //     void TearDown() override { ... }
    //     ...
    //   };
    //
    //   TEST_F(FooTest, Bar) { ... }
    //   TEST_F(FooTest, Baz) { ... }
    //
    // Test is not copyable.
    class Test
    {
        friend class UnitTest;

    protected:
        const char *name;
        const char *__ts_name;
        int tc_res;
        Test *next;

        //----------------
        Test()
        {
            tc_res = TC_PASS;
            next = nullptr;
        }
        virtual void SetUp(void) {}
        virtual void TearDown(void) {}
        virtual void TestBody(void) = 0;

    public:
        void Run();
        // We disallow copying Tests.
        Test(const Test &) = delete;
        Test &operator=(const Test &) = delete;

        //---------------
        const char *Name() const { return this->name; }
        const char *TS_Name() const { return this->__ts_name; }
        Test *Next() const { return this->next; }
        int GetResult() const { return this->tc_res; }
        bool isSameName(const char *suite_name, const char *tc_name) const;
    };

    class UnitTest
    {
        Test *list;
        UnitTest()
        {
            this->list = nullptr;
        }

    public:
        static UnitTest *GetInstance()
        {
            static UnitTest manager;
            return &manager;
        }

        int Run(const char *suite_name, const char *tc_name);

        int Run() { return this->Run(NULL, NULL); }

        static void AddTest(Test *tc);
    };

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

}

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Entry function.
     *
     */
    int utest_main(const char *suite_name, const char *tc_name);

#ifdef __cplusplus
}
#endif

#define TEST_ID_INFO(ts_name, tc_name) "TEST(" #ts_name ", " #tc_name ")"
#define TEST_CASE_NAME(ts_name, tc_name) _test_##ts_name##_##tc_name
#define TEST_CASE_CLASS_NAME(ts_name, tc_name) _test_##ts_name##_##tc_name##_class

#define __DEFINE_TEST(ts_name, tc_name, parent_class)                  \
    class TEST_CASE_CLASS_NAME(ts_name, tc_name);                      \
    class TEST_CASE_CLASS_NAME(ts_name, tc_name) : public parent_class \
    {                                                                  \
        void TestBody(void) override;                                  \
                                                                       \
    public:                                                            \
        TEST_CASE_CLASS_NAME(ts_name, tc_name)                         \
        (const char *name)                                             \
        {                                                              \
            this->name = name;                                         \
            this->__ts_name = #ts_name;                                \
            testing::UnitTest::AddTest(this);                          \
        }                                                              \
    };                                                                 \
    static TEST_CASE_CLASS_NAME(ts_name, tc_name)                      \
        TEST_CASE_NAME(ts_name, tc_name)(#tc_name);                    \
    void TEST_CASE_CLASS_NAME(ts_name, tc_name)::TestBody(void)

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

#endif /* _TESTSUITE_INCLUDE_UTEST_H_ */
