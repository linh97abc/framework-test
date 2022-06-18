#pragma once

namespace unittest
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

    class TestCase;

    class TestSuite
    {
        friend class TestCase;

    private:
        TestCase *list;
        void AddTestCase(TestCase *tc)
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

    class TestCase
    {
        friend class TestSuite;

    private:
        TestCase *next;
        virtual void run(void) = 0;

    public:
        TestCase(TestSuite &suite)
        {
            suite.AddTestCase(this);
        }
    };

    template <class T>
    class _TestCase : public TestCase
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

#define TEST_SUITE(suite_name) class TestSuite_##suite_name : public unittest::TestSuite
#define TEST_CASE(suite_name, tc_name)                                    \
    class TestCase_##suite_name##_##tc_name                               \
        : public unittest::_TestCase<TestSuite_##suite_name>              \
    {                                                                     \
    public:                                                               \
        TestCase_##suite_name##_##tc_name(TestSuite &suite)               \
            : unittest::_TestCase<TestSuite_##suite_name>(suite) {}       \
                                                                          \
    private:                                                              \
        void run(void);                                                   \
    };                                                                    \
    TestCase_##suite_name##_##tc_name _testcase_##suite_name##_##tc_name; \
    void TestCase_##suite_name##_##tc_name::run(void)