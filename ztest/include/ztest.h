/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief Zephyr Testsuite
 */

/**
 * @brief Zephyr Tests (ZTest)
 * @defgroup all_tests Zephyr Tests
 * @{
 * @}
 */

#ifndef ZEPHYR_TESTSUITE_INCLUDE_ZTEST_H_
#define ZEPHYR_TESTSUITE_INCLUDE_ZTEST_H_

/**
 * @defgroup ztest Zephyr testing suite
 */

#define CONFIG_ZTEST_ASSERT_VERBOSE 1
#define CONFIG_ZTEST_MOCKING

#define CONFIG_X86 1

#include <stdio.h>
#define PRINT printf

#include <ztest_assert.h>
#include <ztest_mock.h>
#include <ztest_test_deprecated.h>
#include <tc_util.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_main(void);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_TESTSUITE_INCLUDE_ZTEST_H_ */
