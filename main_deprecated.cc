/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <unittest.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

TEST(sample, empty)
{
	EXPECT_TRUE(0);
}

TEST(sample, eq_ptr)
{
	int x1, x2;
	EXPECT_EQ_PTR(&x1, &x2) << "eq ptr";
}

TEST(sample, number)
{
	EXPECT_DOUBLE_EQ(1, 1);
	EXPECT_DOUBLE_EQ(1, 1.05, 0.1);
	EXPECT_DOUBLE_EQ(-1, -1.05, 0.1);
	EXPECT_DOUBLE_EQ(-1, -1.2, 0.1);
}

TEST(sample, assert)
{
	int x = +0;
	EXPECT_TRUE(1);
	EXPECT_FALSE(0);
	EXPECT_NULL(NULL);
	EXPECT_NOT_NULL("abc");
	EXPECT_EQ(1, x) << "test eq fail";
	EXPECT_EQ_PTR(NULL, NULL);
	testing::fail();
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
	testing::skip();
}

class suite2 : public testing::Test
{
	void SetUp()
	{
		puts(__func__);
	}

	void TearDown()
	{
		puts(__func__);
	}
};

TEST_F(suite2, 001)
{
	EXPECT_EQ(1, 21) << "ancd";
}

int main()
{
	utest_main("suit1", "001");
	utest_main(NULL, NULL);

	return 0;
}