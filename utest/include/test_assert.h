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

static inline bool z_uassert(bool cond,
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
#if CONFIG_uassert_VERBOSE == 2
	else {
		PRINT("\n   Assertion succeeded at %s:%d (%s)\n",
		      file, line, func);
	}
#endif
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
#define uassert(cond, default_msg, msg, ...) do { \
	bool _ret = z_uassert(cond, msg ? ("(" default_msg ")") : (default_msg), \
			     __FILE__, __LINE__, __func__, \
			     msg ? msg : "", ##__VA_ARGS__); \
} while (0)

/**
 * @brief Assert that this function call won't be reached
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_unreachable(msg, ...) uassert(0, "Reached unreachable code", \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is true
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_true(cond, msg, ...) uassert(cond, #cond " is false", \
					     msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is false
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_false(cond, msg, ...) uassert(!(cond), #cond " is true", \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is 0 (success)
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_ok(cond, msg, ...) uassert(!(cond), #cond " is non-zero", \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_is_null(ptr, msg, ...) uassert((ptr) == NULL,	    \
					       #ptr " is not NULL", \
					       msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is not NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_not_null(ptr, msg, ...) uassert((ptr) != NULL,	      \
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
#define uassert_equal(a, b, msg, ...) uassert((a) == (b),	      \
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
#define uassert_not_equal(a, b, msg, ...) uassert((a) != (b),	      \
						  #a " equal to " #b, \
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
#define uassert_equal_ptr(a, b, msg, ...)			    \
	uassert((void *)(a) == (void *)(b), #a " not equal to " #b, \
		msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a is within @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param d Delta
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_within(a, b, d, msg, ...)			     \
	uassert(((a) >= ((b) - (d))) && ((a) <= ((b) + (d))),	     \
		#a " not within " #b " +/- " #d,		     \
		msg, ##__VA_ARGS__)

/**
 * @brief Assert that 2 memory buffers have the same contents
 *
 * This macro calls the final memory comparison assertion macro.
 * Using double expansion allows providing some arguments by macros that
 * would expand to more than one values (ANSI-C99 defines that all the macro
 * arguments have to be expanded before macro call).
 *
 * @param ... Arguments, see @ref uassert_mem_equal__
 *            for real arguments accepted.
 */
#define uassert_mem_equal(...) \
	uassert_mem_equal__(__VA_ARGS__)

/**
 * @brief Internal assert that 2 memory buffers have the same contents
 *
 * @note This is internal macro, to be used as a second expansion.
 *       See @ref uassert_mem_equal.
 *
 * @param buf Buffer to compare
 * @param exp Buffer with expected contents
 * @param size Size of buffers
 * @param msg Optional message to print if the assertion fails
 */
#define uassert_mem_equal__(buf, exp, size, msg, ...)                    \
	uassert(memcmp(buf, exp, size) == 0, #buf " not equal to " #exp, \
	msg, ##__VA_ARGS__)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _TESTSUITE_ASSERT_H__ */
