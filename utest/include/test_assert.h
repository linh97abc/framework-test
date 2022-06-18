/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief testing framework assertion macros
 */

#ifndef _TESTSUITE_ASSERT_H__
#define _TESTSUITE_ASSERT_H__

#include <stdint.h>

#ifdef __cplusplus

#include <iostream>
#include <string.h>

namespace unittest
{
	void fail(void);
	class AssertionResult
	{
	public:
		bool success_;
		AssertionResult(bool v)
		{
			success_ = v;
		}

		~AssertionResult()
		{
			if (!success_)
			{
				std::cout << std::endl;
				fail();
			}
		}

		operator bool() const { return success_; } // NOLINT

		std::ostream &Log()
		{
			return std::cout << "[Assert Fail]  ";
		}
	};

	static inline bool isAlmostEqual(double a, double b, double delta)
	{
		if (delta < 0)
		{
			delta = -delta;
		}

		return (a >= (b - delta)) && (a <= (b + delta));
	}
};

#endif

#ifdef __INTEL_COMPILER
#define TEST_AMBIGUOUS_ELSE_BLOCKER_
#else
#define TEST_AMBIGUOUS_ELSE_BLOCKER_ \
	switch (0)                       \
	case 0:                          \
	default: // NOLINT
#endif

#define EXPECT(cond)                                       \
	TEST_AMBIGUOUS_ELSE_BLOCKER_                           \
	if (unittest::AssertionResult __utest_ar = bool(cond)) \
	{                                                      \
	}                                                      \
	else                                                   \
		__utest_ar.Log()                                   \
			<< __FILE__ << ":" << __LINE__ << ": "

/**
 * @brief Assert that this function call won't be reached
 */
#define EXPECT_UNREACHABLE() EXPECT(0) << "Reached unreachable code" \
										  "\n"

/**
 * @brief Assert that @a cond is true
 * @param cond Condition to check
 */
#define EXPECT_TRUE(cond) EXPECT(cond) << #cond " is false" \
												"\n"

/**
 * @brief Assert that @a cond is false
 * @param cond Condition to check
 */
#define EXPECT_FALSE(cond) EXPECT(!(cond)) << #cond " is true" \
													"\n"

/**
 * @brief Assert that @a ptr is NULL
 * @param ptr Pointer to compare
 */
#define EXPECT_NULL(ptr) EXPECT((ptr) == NULL) << #ptr " is not NULL" \
													   "\n"

/**
 * @brief Assert that @a ptr is not NULL
 * @param ptr Pointer to compare
 */
#define EXPECT_NOT_NULL(ptr) EXPECT((ptr) != NULL) << #ptr " is NULL" \
														   "\n"

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_EQ(a, b) EXPECT((a) == (b)) << #a " != " #b "\nv1: " << (a) << "\nv2: " << (b) << "\n"

/**
 * @brief Assert that @a a does not equal @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_NE(a, b) EXPECT((a) != (b)) << #a " == " #b "\nvalue: " << (a) << "\n"

/**
 * @brief Tests that @a a < @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_LT(a, b) EXPECT((a) < (b)) << #a " >= " #b "\nv1: " << (a) << "\nv2: " << (b) << "\n"

/**
 * @brief Tests that @a a <= @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_LE(a, b) EXPECT((a) <= (b)) << #a " > " #b "\nv1: " << (a) << "\nv2: " << (b) << "\n"

/**
 * @brief Tests that @a a > @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_GT(a, b) EXPECT((a) > (b)) << #a " <= " #b "\nv1: " << (a) << "\nv2: " << (b) << "\n"

/**
 * @brief Tests that @a a >= @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_GE(a, b) EXPECT((a) > (b)) << #a " < " #b "\nv1: " << (a) << "\nv2: " << (b) << "\n"

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b will be converted to `void *` before comparing.
 *
 * @param a Value to compare
 * @param b Value to compare
 */
#define EXPECT_EQ_PTR(a, b) EXPECT((void *)(a) == (void *)(b)) \
								<< #a " != " #b "\nv1: "       \
								<< (uintptr_t)(a)              \
								<< "\nv2: "                    \
								<< (uintptr_t)(b) << "\n"

/**
 * @brief Assert that @a a is within @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param delta Delta
 */
#define EXPECT_ALMOST_EQ(a, b, delta) \
	EXPECT(unittest::isAlmostEqual(a, b, delta)) << #a " not within " #b " +/- " #delta "\nv1: " << (a) << "\nv2: " << (b) << "\n"

/**
 * @brief Assert that @a a is relative equal @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param delta Delta
 */
#define EXPECT_RELATIVE_EQ(a, b, delta) EXPECT_ALMOST_EQ(a, b, (b) * (delta))

/**
 * @brief Assert that 2 memory buffers have the same contents
 *
 * This macro calls the final memory comparison assertion macro.
 * Using double expansion allows providing some arguments by macros that
 * would expand to more than one values (ANSI-C99 defines that all the macro
 * arguments have to be expanded before macro call).
 *
 * @param buf Buffer to compare
 * @param exp Buffer with expected contents
 * @param size Size of buffers
 */
#define EXPECT_MEM_EQ(buf, exp, size) \
	EXPECT(memcmp(buf, exp, size) == 0) << #buf " not equal to " #exp "\n"
	/**
	 * @}
	 */

#endif /* _TESTSUITE_ASSERT_H__ */
