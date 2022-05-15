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

#ifndef _TESTSUITE_INCLUDE_utest_H_
#define _TESTSUITE_INCLUDE_utest_H_

/**
 * @defgroup utest testing suite
 */

#define CONFIG_uassert_VERBOSE 1
#define CONFIG_utest_MOCKING

#define CONFIG_X86 1

#include <stdio.h>
#define PRINT printf

#include <test_assert.h>
#include <test_mock.h>
#include <test_deprecated.h>
#include <tc_util.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_main(void);

#ifdef __cplusplus
}
#endif

#endif /* _TESTSUITE_INCLUDE_utest_H_ */
