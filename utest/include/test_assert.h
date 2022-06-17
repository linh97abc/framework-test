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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

	void utest_fail(void);

	static inline void __utest_assert_log(
		const char *msg, ...)
	{

		va_list vargs;
		va_start(vargs, msg);
		vprintf(msg, vargs);
		puts("");
		va_end(vargs);
		utest_fail();
	}

	/**
	 * @defgroup utest assertion macros
	 * @ingroup utest
	 *
	 * This module provides assertions when using utest.
	 *
	 * @{
	 */

#define __TEST_ASSERT(cond, default_msg, msg, ...)                  \
	do                                                              \
	{                                                               \
		if (!(cond))                                                \
		{                                                           \
			PRINT("\n    %s:%d: " default_msg, __FILE__, __LINE__); \
			__utest_assert_log(msg, ##__VA_ARGS__);                 \
		}                                                           \
	} while (0)

/**
 * @brief Fail the test, if @a cond is false
 *
 * You probably don't need to call this macro directly. You should
 * instead use uassert_{condition} macros below.
 *
 * Note that when CONFIG_MULTITHREADING=n macro returns from the function. It is
 * then expected that in that case utest asserts will be used only in the
 * context of the test function.
 *
 * @param cond Condition to check
 * @param msg Optional, can be NULL. Message to print if @a cond is false.
 * @param default_msg Message to print if @a cond is false
 */
#define _TEST_ASSERT(cond, default_msg, ...) __TEST_ASSERT(cond, default_msg, "" __VA_ARGS__)

/**
 * @brief Assert that this function call won't be reached
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_UNREACHABLE(...) _TEST_ASSERT(0, "Reached unreachable code", ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is true
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_TRUE(cond, ...) _TEST_ASSERT(cond, #cond " is false", ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is false
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_FALSE(cond, ...) _TEST_ASSERT(!(cond), #cond " is true", ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_NULL(ptr, ...) _TEST_ASSERT((ptr) == NULL, \
										   #ptr " is not NULL", ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is not NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_NOT_NULL(ptr, ...) _TEST_ASSERT((ptr) != NULL, \
											   #ptr " is NULL", ##__VA_ARGS__)

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_EQ(a, b, ...)   \
	_TEST_ASSERT((a) == (b),   \
				 #a " != " #b, \
				 ##__VA_ARGS__)

/**
 * @brief Assert that @a a does not equal @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_NE(a, b, ...)   \
	_TEST_ASSERT((a) != (b),   \
				 #a " == " #b, \
				 ##__VA_ARGS__)

// /**
//  * @brief Assert that number @a a equals @a b
//  *
//  * @a a and @a b won't be converted and will be compared directly.
//  *
//  * @param a Value to compare
//  * @param b Value to compare
//  * @param msg Optional message to print if the assertion fails
//  */
// #define EXPECT_EQ_INT(a, b, ...)                               \
// 	_TEST_ASSERT((a) == (b),                                    \
// 				__TEST_DEFAULT_MSG(                            \
// 					#a " != " #b "\n" #a ": %ld\n" #b ": %ld", \
// 					(a),                                       \
// 					(b)),                                      \
// 				##__VA_ARGS__)

// /**
//  * @brief Assert that number @a a not equals @a b
//  *
//  * @a a and @a b won't be converted and will be compared directly.
//  *
//  * @param a Value to compare
//  * @param b Value to compare
//  * @param msg Optional message to print if the assertion fails
//  */
// #define EXPECT_NE_INT(a, b, ...)                               \
// 	_TEST_ASSERT((a) == (b),                                    \
// 				__TEST_DEFAULT_MSG(                            \
// 					#a " == " #b "\n" #a ": %ld\n" #b ": %ld", \
// 					(a),                                       \
// 					(b)),                                      \
// 				##__VA_ARGS__)

/**
 * @brief Tests that @a a < @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_LT(a, b, ...)   \
	_TEST_ASSERT((a) < (b),    \
				 #a " >= " #b, \
				 ##__VA_ARGS__)

/**
 * @brief Tests that @a a <= @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_LE(a, b, ...)  \
	_TEST_ASSERT((a) <= (b),  \
				 #a " > " #b, \
				 ##__VA_ARGS__)

/**
 * @brief Tests that @a a > @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_GT(a, b, ...)   \
	_TEST_ASSERT((a) > (b),    \
				 #a " <= " #b, \
				 ##__VA_ARGS__)

/**
 * @brief Tests that @a a >= @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_GE(a, b, ...)  \
	_TEST_ASSERT((a) >= (b),  \
				 #a " < " #b, \
				 ##__VA_ARGS__)

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b will be converted to `void *` before comparing.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_EQ_PTR(a, b, ...)             \
	_TEST_ASSERT((void *)(a) == (void *)(b), \
				 #a " != " #b,               \
				 ##__VA_ARGS__)

	static inline bool __isAlmostEqual(double a, double b, double delta)
	{
		if (delta < 0)
		{
			delta = -delta;
		}

		return (a >= (b - delta)) && (a <= (b + delta));
	}

/**
 * @brief Assert that @a a is within @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param delta Delta
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_ALMOST_EQ(a, b, delta, ...)            \
	_TEST_ASSERT(__isAlmostEqual(a, b, delta),        \
				 #a " not within " #b " +/- " #delta, \
				 ##__VA_ARGS__)

/**
 * @brief Assert that @a a is relative equal @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param delta Delta
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_RELATIVE_EQ(a, b, delta, ...) EXPECT_ALMOST_EQ(a, b, (b) * (delta), ##__VA_ARGS__)

/**
 * @brief Assert that 2 memory buffers have the same contents
 *
 * This macro calls the final memory comparison assertion macro.
 * Using double expansion allows providing some arguments by macros that
 * would expand to more than one values (ANSI-C99 defines that all the macro
 * arguments have to be expanded before macro call).
 *
 * @param ... Arguments, see @ref __expect_mem_equal__
 *            for real arguments accepted.
 */
#define EXPECT_MEM_EQ(...) \
	__expect_mem_equal__(__VA_ARGS__)

/**
 * @brief Internal assert that 2 memory buffers have the same contents
 *
 * @note This is internal macro, to be used as a second expansion.
 *       See @ref EXPECT_MEM_EQ.
 *
 * @param buf Buffer to compare
 * @param exp Buffer with expected contents
 * @param size Size of buffers
 * @param msg Optional message to print if the assertion fails
 */
#define __expect_mem_equal__(buf, exp, size, ...) \
	_TEST_ASSERT(memcmp(buf, exp, size) == 0,     \
				 #buf " not equal to " #exp,      \
				 ##__VA_ARGS__)

	/**
	 * @}
	 */

#ifdef __cplusplus
}
#endif

#endif /* _TESTSUITE_ASSERT_H__ */
