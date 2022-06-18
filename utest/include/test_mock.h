/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief mocking support
 */

#ifndef _TESTSUITE__TEST_MOCK_H_
#define _TESTSUITE__TEST_MOCK_H_
#include "tc_util.h"
#include <stdint.h>
/**
 * @defgroup utest_mock utest mocking support
 * @ingroup utest
 *
 * This module provides simple mocking functions for unit testing. These
 * need CONFIG_TEST_MOCKING=y.
 *
 * @{
 */

/**
 * @brief Tell function @a func to expect the value @a value for @a param
 *
 * When using utest_check_expected_value(), tell that the value of @a param
 * should be @a value. The value will internally be stored as an `uintptr_t`.
 *
 * @param func Function in question
 * @param param Parameter for which the value should be set
 * @param value Value for @a param
 */
#define utest_expect_value(func, param, value)                      \
	testing::mock::expect_value(STRINGIFY(func), STRINGIFY(param), \
								 (uintptr_t)(value))

/**
 * @brief If @a param doesn't match the value set by utest_expect_value(),
 * fail the test
 *
 * This will first check that does @a param have a value to be expected, and
 * then checks whether the value of the parameter is equal to the expected
 * value. If either of these checks fail, the current test will fail. This
 * must be called from the called function.
 *
 * @param param Parameter to check
 */
#define utest_check_expected_value(param)                      \
	testing::mock::expected_value(__func__, STRINGIFY(param), \
								   (uintptr_t)(param))

/**
 * @brief Tell function @a func to expect the data @a data for @a param
 *
 * When using utest_check_expected_data(), the data pointed to by
 * @a param should be same @a data in this function. Only data pointer is stored
 * by this function, so it must still be valid when utest_check_expected_data is
 * called.
 *
 * @param func Function in question
 * @param param Parameter for which the data should be set
 * @param data pointer for the data for parameter @a param
 */
#define utest_expect_data(func, param, data) \
	testing::mock::expect_data(STRINGIFY(func), STRINGIFY(param), (void *)(data))

/**
 * @brief If data pointed by @a param don't match the data set by
 * utest_expect_data(), fail the test
 *
 * This will first check that @a param is expected to be null or non-null and
 * then check whether the data pointed by parameter is equal to expected data.
 * If either of these checks fail, the current test will fail. This
 * must be called from the called function.
 *
 * @param param Parameter to check
 * @param length Length of the data to compare
 */
#define utest_check_expected_data(param, length)                    \
	testing::mock::check_expected_data(__func__, STRINGIFY(param), \
										(void *)(param), (length))

/**
 * @brief Tell function @a func to return the data @a data for @a param
 *
 * When using utest_return_data(), the data pointed to by @a param should be
 * same @a data in this function. Only data pointer is stored by this function,
 * so it must still be valid when utest_copy_return_data is called.
 *
 * @param func Function in question
 * @param param Parameter for which the data should be set
 * @param data pointer for the data for parameter @a param
 */
#define utest_return_data(func, param, data) \
	testing::mock::return_data(STRINGIFY(func), STRINGIFY(param), (void *)(data))

/**
 * @brief Copy the data set by utest_return_data to the memory pointed by
 * @a param
 *
 * This will first check that @a param is not null and then copy the data.
 * This must be called from the called function.
 *
 * @param param Parameter to return data for
 * @param length Length of the data to return
 */
#define utest_copy_return_data(param, length)                    \
	testing::mock::copy_return_data(__func__, STRINGIFY(param), \
									 (void *)(param), (length))
/**
 * @brief Tell @a func that it should return @a value
 *
 * @param func Function that should return @a value
 * @param value Value to return from @a func
 */
#define utest_returns_value(func, value) \
	testing::mock::returns_value(STRINGIFY(func), (uintptr_t)(value))

/**
 * @brief Get the return value for current function
 *
 * The return value must have been set previously with utest_returns_value().
 * If no return value exists, the current test will fail.
 *
 * @returns The value the current function should return
 */
#define utest_get_return_value() testing::mock::get_return_value(__func__)

/**
 * @brief Get the return value as a pointer for current function
 *
 * The return value must have been set previously with utest_returns_value().
 * If no return value exists, the current test will fail.
 *
 * @returns The value the current function should return as a `void *`
 */
#define utest_get_return_value_ptr() \
	((void *)testing::mock::get_return_value(__func__))

/**
 * @}
 */

#ifdef CONFIG_TEST_MOCKING

namespace testing
{
	namespace mock
	{
		int __init(void);
		int __cleanup(void);

		void expect_value(const char *fn, const char *name, uintptr_t value);
		void check_expected_value(const char *fn, const char *param,
								  uintptr_t value);

		void expect_data(const char *fn, const char *name, void *val);
		void check_expected_data(const char *fn, const char *name, void *data,
								 uint32_t length);

		void return_data(const char *fn, const char *name, void *val);
		void copy_return_data(const char *fn, const char *name, void *data,
							  uint32_t length);

		void returns_value(const char *fn, uintptr_t value);
		uintptr_t get_return_value(const char *fn);
	}

}

#else /* !CONFIG_TEST_MOCKING */
namespace testing
{
	namespace mock
	{
		static inline int __init(void){return 0};
		static inline int __cleanup(void){return 0};
	}

}

#endif /* CONFIG_TEST_MOCKING */

#endif /* _TESTSUITE__TEST_MOCK_H_ */
