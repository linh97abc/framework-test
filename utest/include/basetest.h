#pragma once

namespace testing
{
    template <class T>
    class GenInst
    {
    private:
        /* data */
    public:
        static T inst;
        GenInst(/* args */) = default;
    };

    template <class T>
    T GenInst<T>::inst; 

    class Test;

    class TestSuite
    {
        friend class Test;

    private:
        Test *list;
        void AddTestCase(Test *tc)
        {
            tc->next = this->list;
            this->list = tc;
        }

    public:
        void SetUp(void){};
        void TearDown(void){};
        TestSuite(/* args */)
        {
            list = nullptr;
        }

        ~TestSuite() = default;
    };

    class Test
    {
        friend class TestSuite;

    private:
        Test *next;
        virtual void run(void) = 0;

    public:
        Test(TestSuite &suite)
        {
            suite.AddTestCase(this);
        }
    };

    template <class T>
    class _TestCase : public Test
    {
    private:
        virtual void run(void) = 0;

    public:
        _TestCase()
        {
            GenInst<T>::inst.AddTestCase(this);
        }
    };

};

#define TEST_SUITE(suite_name) class TestSuite_##suite_name : public testing::TestSuite
#define TEST_CASE(suite_name, tc_name)                                    \
    class TestCase_##suite_name##_##tc_name                               \
        : public testing::_TestCase<TestSuite_##suite_name>              \
    {                                                                     \
    public:                                                               \
        TestCase_##suite_name##_##tc_name(TestSuite &suite)               \
            : testing::_TestCase<TestSuite_##suite_name>(suite) {}       \
                                                                          \
    private:                                                              \
        void run(void);                                                   \
    };                                                                    \
    TestCase_##suite_name##_##tc_name _testcase_##suite_name##_##tc_name; \
    void TestCase_##suite_name##_##tc_name::run(void)