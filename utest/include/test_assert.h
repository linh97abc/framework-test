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
extern "C" {
#endif

void utest_fail(void);

static inline bool __utest_assert(bool cond,
			    const char *default_msg,
			    const char *file,
			    int line, const char *func,
			    const char *msg, ...)
{
	if (cond == false) {
		va_list vargs;

		va_start(vargs, msg);
		PRINT("\n    Assertion failed at %s:%d: %s: %s\n",
		      file, line, func, default_msg);
		vprintf(msg, vargs);
		printf("\n");
		va_end(vargs);
		utest_fail();
		return false;
	}

	return true;
}


/**
 * @defgroup uuassert utest assertion macros
 * @ingroup utest
 *
 * This module provides assertions when using utest.
 *
 * @{
 */

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
#define TEST_ASSERT(cond, default_msg, msg, ...) do { \
	__utest_assert(cond, msg ? ("(" default_msg ")") : (default_msg), \
			     __FILE__, __LINE__, __func__, \
			     msg ? msg : "", ##__VA_ARGS__); \
} while (0)

/**
 * @brief Assert that this function call won't be reached
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_UNREACHABLE(msg, ...) TEST_ASSERT(0, "Reached unreachable code", \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is true
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_TRUE(cond, msg, ...) TEST_ASSERT(cond, #cond " is false", \
					     msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is false
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_FALSE(cond, msg, ...) TEST_ASSERT(!(cond), #cond " is true", \
					      msg, ##__VA_ARGS__)


/**
 * @brief Assert that @a ptr is NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_NULL(ptr, msg, ...) TEST_ASSERT((ptr) == NULL,	    \
					       #ptr " is not NULL", \
					       msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is not NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_NOT_NULL(ptr, msg, ...) TEST_ASSERT((ptr) != NULL,	      \
						#ptr " is NULL", msg, \
						##__VA_ARGS__)

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_EQ(a, b, msg, ...) TEST_ASSERT((a) == (b),	      \
					      #a " not equal to " #b, \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a does not equal @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_NE(a, b, msg, ...) TEST_ASSERT((a) != (b),	      \
						  #a " equal to " #b, \
						  msg, ##__VA_ARGS__)

/**
 * @brief Tests that @a a < @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_LT(a, b, msg, ...) TEST_ASSERT((a) < (b),	      \
						  #a " >= " #b, \
						  msg, ##__VA_ARGS__)

/**
 * @brief Tests that @a a <= @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_LE(a, b, msg, ...) TEST_ASSERT((a) <= (b),	      \
						  #a " > " #b, \
						  msg, ##__VA_ARGS__)

/**
 * @brief Tests that @a a > @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_GT(a, b, msg, ...) TEST_ASSERT((a) > (b),	      \
						  #a " <= " #b, \
						  msg, ##__VA_ARGS__)

/**
 * @brief Tests that @a a >= @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_GE(a, b, msg, ...) TEST_ASSERT((a) >= (b),	      \
						  #a " < " #b, \
						  msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b will be converted to `void *` before comparing.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_EQ_PTR(a, b, msg, ...)			    \
	TEST_ASSERT((void *)(a) == (void *)(b), #a " not equal to " #b, \
		msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a is within @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param delta Delta
 * @param msg Optional message to print if the assertion fails
 */
#define EXPECT_ALMOST_EQ(a, b, delta, msg, ...)			     \
	TEST_ASSERT(((a) >= ((b) - (delta))) && ((a) <= ((b) + (delta))),	     \
		#a " not within " #b " +/- " #delta,		     \
		msg, ##__VA_ARGS__)


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
#define __expect_mem_equal__(buf, exp, size, msg, ...)                    \
	TEST_ASSERT(memcmp(buf, exp, size) == 0, #buf " not equal to " #exp, \
	msg, ##__VA_ARGS__)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _TESTSUITE_ASSERT_H__ */
