/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2021 Google LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_TESTSUITE_INCLUDE_ZTEST_TEST_H_
#define ZEPHYR_TESTSUITE_INCLUDE_ZTEST_TEST_H_


#include <ztest_test_deprecated.h>

#ifdef __cplusplus
extern "C" {
#endif

void z_test_1cpu_start(void);
void z_test_1cpu_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_TESTSUITE_INCLUDE_ZTEST_TEST_H_ */
