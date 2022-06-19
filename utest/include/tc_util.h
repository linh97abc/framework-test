/* tc_utilities.h - testcase utilities header file */

/*
 * Copyright (c) 2012-2015 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TESTSUITE_INCLUDE_TC_UTIL_H_
#define _TESTSUITE_INCLUDE_TC_UTIL_H_

#define STRINGIFY(x) #x
#define TC_STR_HELPER(x) #x
#define TC_STR(x) TC_STR_HELPER(x)

#define TC_PASS 0
#define TC_FAIL 1
#define TC_SKIP 2

#define CONFIG_TC_COLOR

#ifdef CONFIG_TC_COLOR
#define TC_COLOR_RED "\e[1;31m"
#define TC_COLOR_GREEN "\e[1;32m"
#define TC_COLOR_YELLOW "\e[1;33m"
#define TC_COLOR_BLUE "\e[1;34m"
#define TC_COLOR_END "\e[1;39m"

#else
#define TC_COLOR_RED ""
#define TC_COLOR_GREEN ""
#define TC_COLOR_YELLOW ""
#define TC_COLOR_BLUE ""
#define TC_COLOR_END ""

#endif

#ifndef TC_PASS_STR
#define TC_PASS_STR \
	TC_COLOR_GREEN  \
	"PASS" TC_COLOR_END
#endif
#ifndef TC_FAIL_STR
#define TC_FAIL_STR \
	TC_COLOR_RED    \
	"FAIL" TC_COLOR_END
#endif
#ifndef TC_SKIP_STR
#define TC_SKIP_STR \
	TC_COLOR_YELLOW \
	"SKIP" TC_COLOR_END
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

#define TC_LINE ""
#define TC_NAME(ts_name, tc_name) (ts_name) << "_" << (tc_name)
#define TC_START(ts_name, tc_name) "...\n" TC_COLOR_BLUE "test_" << TC_NAME(ts_name, tc_name) << "... " TC_COLOR_END
#define TC_END_RESULT(result) " " << TC_RESULT_TO_STR(result) << TC_LINE
#define TC_END_REPORT(result)                                          \
	TC_LINE << (((result) == TC_PASS) ? TC_COLOR_GREEN : TC_COLOR_RED) \
			<< "\nPROJECT EXECUTION "                                  \
			<< (((result) == TC_PASS) ? "SUCCESSFUL" : "FAILED")       \
			<< TC_COLOR_END

#endif /* _TESTSUITE_INCLUDE_TC_UTIL_H_ */
