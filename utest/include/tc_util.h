/* tc_utilities.h - testcase utilities header file */

/*
 * Copyright (c) 2012-2015 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TESTSUITE_INCLUDE_TC_UTIL_H_
#define _TESTSUITE_INCLUDE_TC_UTIL_H_

#include <string.h>
#include <stdio.h>

#ifndef TEST_PRINT
#define TEST_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define STRINGIFY(x) #x
#endif

#define TC_STR_HELPER(x) #x
#define TC_STR(x) TC_STR_HELPER(x)

#ifndef PRINT_LINE
#define PRINT_LINE TEST_PRINT("\n")
#endif

#define FMT_ERROR "%s - %s@%d. "

#define TC_PASS 0
#define TC_FAIL 1
#define TC_SKIP 2

#ifndef TC_PASS_STR
#define TC_PASS_STR "PASS"
#endif
#ifndef TC_FAIL_STR
#define TC_FAIL_STR "FAIL"
#endif
#ifndef TC_SKIP_STR
#define TC_SKIP_STR "SKIP"
#endif

static inline const char *TC_RESULT_TO_STR(int result)
{
	switch (result)
	{
	case TC_PASS:
		return TC_PASS_STR;
	case TC_FAIL:
		return TC_FAIL_STR;
	case TC_SKIP:
		return TC_SKIP_STR;
	default:
		return "?";
	}
}

#ifndef TC_ERROR
#define TC_ERROR(fmt, ...)                                 \
	do                                                     \
	{                                                      \
		TEST_PRINT(FMT_ERROR, "FAIL", __func__, __LINE__); \
		TEST_PRINT(fmt, ##__VA_ARGS__);                    \
	} while (0)
#endif

#ifndef TC_PRINT
#define TC_PRINT(fmt, ...) TEST_PRINT(fmt, ##__VA_ARGS__)
#endif

#ifndef TC_START
#define TC_START(ts_name, tc_name)                             \
	do                                                         \
	{                                                          \
		TEST_PRINT("...\ntest_%s_%s ... \n", ts_name, tc_name); \
	} while (0)
#endif

#ifndef TC_END
#define TC_END(result, fmt, ...) TEST_PRINT(fmt, ##__VA_ARGS__)
#endif

#ifndef Z_TC_END_RESULT
/* prints result and the function name */
#define Z_TC_END_RESULT(result)                          \
	do                                                   \
	{                                                    \
		TC_END(result, ".%s", TC_RESULT_TO_STR(result)); \
		PRINT_LINE;                                      \
	} while (0)
#endif

#ifndef TC_END_RESULT
#define TC_END_RESULT(result) \
	Z_TC_END_RESULT((result))
#endif

#ifndef TC_SUITE_START
#define TC_SUITE_START(name) \
	do                       \
	{                        \
		PRINT_LINE;          \
	} while (0)
#endif

#ifndef TC_SUITE_END
#define TC_SUITE_END(name, result) \
	do                             \
	{                              \
	} while (0)
#endif

#define TC_END_POST(result)

#ifndef TC_END_REPORT
#define TC_END_REPORT(result)                                  \
	do                                                         \
	{                                                          \
		PRINT_LINE;                                            \
		TC_END(result,                                         \
			   "PROJECT EXECUTION %s\n",                       \
			   (result) == TC_PASS ? "SUCCESSFUL" : "FAILED"); \
		TC_END_POST(result);                                   \
	} while (0)
#endif

#endif /* _TESTSUITE_INCLUDE_TC_UTIL_H_ */
