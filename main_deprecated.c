/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <utest.h>
#include <stdint.h>
#include <stdio.h>

TEST_SETUP(sample)
{
	// puts(__func__);
}

TEST_TEARDOWN(sample)
{
	// puts(__func__);
}

TEST(sample, empty)
{
	EXPECT_TRUE(0);
}

TEST(sample, assert)
{
	EXPECT_TRUE(1);
	EXPECT_FALSE(0);
	EXPECT_NULL(NULL);
	EXPECT_NOT_NULL("foo");
	EXPECT_EQ(1, 1);
	EXPECT_EQ_PTR(NULL, NULL);
	utest_fail();
}

TEST(sample, mem_equal)
{
	static const uint32_t expected[4] = {
		0x1234,
		0x5678,
		0x9ABC,
		0xDEF0};
	uint32_t actual[4] = {0};

	memcpy(actual, expected, sizeof(actual));
	EXPECT_MEM_EQ(actual, expected, sizeof(expected));
}

TEST(sample, skip)
{
	utest_skip();
}

TEST_SETUP(suite2)
{
}

TEST_TEARDOWN(suite2)
{
}

TEST(suite2, 001)
{
	EXPECT_FALSE(1, "abc");
}

TEST_SUITE(sample,
		   TEST_CASE(sample, skip),
		   TEST_CASE(sample, empty),
		   TEST_CASE(sample, assert),
		   TEST_CASE(sample, mem_equal));

TEST_SUITE(suite2, TEST_CASE(suite2, 001));

void RunAllTest(void)
{
	RUN_TEST_SUITE(sample);
	RUN_TEST_SUITE(suite2);
}

int main()
{
	utest_main();

	return 0;
}