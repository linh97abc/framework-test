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
#include <tc_util.h>
#include <stdint.h>
/**
 * @defgroup utest_mock utest mocking support
 * @ingroup utest
 *
 * This module provides simple mocking functions for unit testing. These
 * need CONFIG_utest_MOCKING=y.
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
#define utest_expect_value(func, param, value)                                 \
	z_utest_expect_value(STRINGIFY(func), STRINGIFY(param),                \
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
#define utest_check_expected_value(param)                                      \
	z_utest_check_expected_value(__func__, STRINGIFY(param),               \
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
#define utest_expect_data(func, param, data)                                   \
	z_utest_expect_data(STRINGIFY(func), STRINGIFY(param), (void *)(data))

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
#define utest_check_expected_data(param, length)                               \
	z_utest_check_expected_data(__func__, STRINGIFY(param),                \
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
#define utest_return_data(func, param, data)                                   \
	z_utest_return_data(STRINGIFY(func), STRINGIFY(param), (void *)(data))

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
#define utest_copy_return_data(param, length)                                  \
	z_utest_copy_return_data(__func__, STRINGIFY(param),                   \
				 (void *)(param), (length))
/**
 * @brief Tell @a func that it should return @a value
 *
 * @param func Function that should return @a value
 * @param value Value to return from @a func
 */
#define utest_returns_value(func, value)                                       \
	z_utest_returns_value(STRINGIFY(func), (uintptr_t)(value))

/**
 * @brief Get the return value for current function
 *
 * The return value must have been set previously with utest_returns_value().
 * If no return value exists, the current test will fail.
 *
 * @returns The value the current function should return
 */
#define utest_get_return_value() z_utest_get_return_value(__func__)

/**
 * @brief Get the return value as a pointer for current function
 *
 * The return value must have been set previously with utest_returns_value().
 * If no return value exists, the current test will fail.
 *
 * @returns The value the current function should return as a `void *`
 */
#define utest_get_return_value_ptr()                                           \
	((void *)z_utest_get_return_value(__func__))

/**
 * @}
 */

#ifdef CONFIG_utest_MOCKING

#ifdef __cplusplus
extern "C" {
#endif

void z_init_mock(void);
int z_cleanup_mock(void);

void z_utest_expect_value(const char *fn, const char *name, uintptr_t value);
void z_utest_check_expected_value(const char *fn, const char *param,
				  uintptr_t value);

void z_utest_expect_data(const char *fn, const char *name, void *val);
void z_utest_check_expected_data(const char *fn, const char *name, void *data,
				 uint32_t length);

void z_utest_return_data(const char *fn, const char *name, void *val);
void z_utest_copy_return_data(const char *fn, const char *name, void *data,
			      uint32_t length);

void z_utest_returns_value(const char *fn, uintptr_t value);
uintptr_t z_utest_get_return_value(const char *fn);

#ifdef __cplusplus
}
#endif

#else /* !CONFIG_utest_MOCKING */

#define z_init_mock()
#define z_cleanup_mock() 0

#endif /* CONFIG_utest_MOCKING */

#endif /* _TESTSUITE__TEST_MOCK_H_ */
