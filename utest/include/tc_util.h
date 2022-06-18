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

#define TC_LINE ""
#define TC_NAME(ts_name, tc_name) (ts_name) << "_" << (tc_name)
#define TC_START(ts_name, tc_name) "...\ntest_" << TC_NAME(ts_name, tc_name) << "... "
#define TC_END_RESULT(result) " " << TC_RESULT_TO_STR(result) << TC_LINE
#define TC_END_REPORT(result) \
	TC_LINE "\nPROJECT EXECUTION " << (((result) == TC_PASS) ? "SUCCESSFUL" : "FAILED")

#endif /* _TESTSUITE_INCLUDE_TC_UTIL_H_ */
