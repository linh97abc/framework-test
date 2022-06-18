/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/unittest.h"

#ifdef CONFIG_TEST_MOCKING
#include <string.h>

struct parameter
{
	struct parameter *next;
	const char *fn;
	const char *name;
	uintptr_t value;
};

#include <stdlib.h>

/* ------------- PORT -----------------*/
#ifdef __ucosii__
#include <ucos_ii.h>

#define MAX_NUM_OF_PARAM_ON_HEAP 1024
static OS_MEM *p_param_mem;
static void *__param_memarea[(MAX_NUM_OF_PARAM_ON_HEAP + 1) * sizeof(struct parameter) / sizeof(void *)];

static void free_parameter(struct parameter *param)
{
	INT8U err = OSMemPut(p_param_mem, (void *)param);
	EXPECT_EQ(err, OS_ERR_NONE) << "Failed to free parameter";
}

static struct parameter *alloc_parameter(void)
{
	struct parameter *param;
	INT8U err;

	param = (struct parameter *)OSMemGet(p_param_mem, &err);
	EXPECT_EQ(err, OS_ERR_NONE) << "Failed to free parameter";
	EXPECT_NOT_NULL(param) << "Failed to allocate mock parameter";

	return param;
}

int testing::mock::__init(void)
{
	INT8U err;
	p_param_mem = OSMemCreate((void *)__param_memarea, MAX_NUM_OF_PARAM_ON_HEAP, sizeof(struct parameter), &err);

	if (err != OS_ERR_NONE)
	{
		testing::AssertionResult res = true;
		testing::Message() << "Failed to init parameter memory region, error: " << err;

		return -1;
	}

	return 0;
}
#else

static void free_parameter(struct parameter *param)
{
	free(param);
}

static struct parameter *alloc_parameter(void)
{
	struct parameter *param;

	param = (struct parameter *)calloc(1, sizeof(struct parameter));
	EXPECT_NOT_NULL(param) << "Failed to allocate mock parameter";

	return param;
}

int testing::mock::__init(void)
{
	return 0;
}

#endif

/* -------------------------------------*/

static struct parameter *find_and_delete_value(struct parameter *param,
											   const char *fn, const char *name)
{
	struct parameter *value;

	if (!param->next)
	{
		return NULL;
	}

	if (strcmp(param->next->name, name) || strcmp(param->next->fn, fn))
	{
		return find_and_delete_value(param->next, fn, name);
	}

	value = param->next;
	param->next = param->next->next;
	value->next = NULL;

	return value;
}

static void insert_value(struct parameter *param, const char *fn,
						 const char *name, uintptr_t val)
{
	struct parameter *value;

	value = alloc_parameter();
	value->fn = fn;
	value->name = name;
	value->value = val;

	/* Seek to end of linked list to ensure correct discovery order in find_and_delete_value */
	while (param->next)
	{
		param = param->next;
	}

	/* Append to end of linked list */
	value->next = param->next;
	param->next = value;
}

static struct parameter parameter_list = {NULL, "", "", 0};
static struct parameter return_value_list = {NULL, "", "", 0};

void testing::mock::expect_value(const char *fn, const char *name, uintptr_t val)
{
	insert_value(&parameter_list, fn, name, val);
}

void testing::mock::check_expected_value(const char *fn, const char *name,
										 uintptr_t val)
{
	struct parameter *param;
	uintptr_t expected;

	param = find_and_delete_value(&parameter_list, fn, name);
	EXPECT_NOT_NULL(param) << "Failed to find parameter " << name << " for " << fn;

	expected = param->value;
	free_parameter(param);

	EXPECT_EQ(expected, val) << fn << ":" << name << " received wrong value";
}

void testing::mock::expect_data(const char *fn, const char *name, void *val)
{
	insert_value(&parameter_list, fn, name, (uintptr_t)val);
}

void testing::mock::check_expected_data(const char *fn, const char *name, void *data,
										uint32_t length)
{
	struct parameter *param;
	void *expected;

	param = find_and_delete_value(&parameter_list, fn, name);
	EXPECT_NOT_NULL(param) << "Failed to find parameter " << name << " for " << fn;

	expected = (void *)param->value;
	free_parameter(param);

	EXPECT((expected != NULL) || (data == NULL)) << fn << ":" << name << " received null pointer";
	EXPECT((expected == NULL) || (data != NULL)) << fn << ":" << name << " received data while expected null pointer";
	EXPECT((data == NULL) || (memcmp(data, expected, length) == 0)) << fn << ":" << name << " data provided don't match";
}

void testing::mock::return_data(const char *fn, const char *name, void *val)
{
	insert_value(&parameter_list, fn, name, (uintptr_t)val);
}

void testing::mock::copy_return_data(const char *fn, const char *name, void *data,
									 uint32_t length)
{
	struct parameter *param;
	void *return_data;

	EXPECT_NOT_NULL(data) << fn << ":" << name << " received null pointer";

	param = find_and_delete_value(&parameter_list, fn, name);
	EXPECT_NOT_NULL(param) << "Failed to find parameter " << name << " for " << fn;

	return_data = (void *)param->value;
	free_parameter(param);
	memcpy(data, return_data, length);
}

void testing::mock::returns_value(const char *fn, uintptr_t value)
{
	insert_value(&return_value_list, fn, "", value);
}

uintptr_t testing::mock::get_return_value(const char *fn)
{
	uintptr_t value;
	struct parameter *param =
		find_and_delete_value(&return_value_list, fn, "");

	EXPECT_NOT_NULL(param) << "Failed to find return value for function " << fn;

	value = param->value;
	free_parameter(param);

	return value;
}

static void free_param_list(struct parameter *param)
{
	struct parameter *next;

	while (param)
	{
		next = param->next;
		free_parameter(param);
		param = next;
	}
}

int testing::mock::__cleanup(void)
{
	int fail = 0;

	if (parameter_list.next)
	{
		Message() << "Parameter not used by mock: " << parameter_list.next->fn << ":" << parameter_list.next->name;
		fail = 1;
	}
	if (return_value_list.next)
	{
		Message() << "Return value no used by mock: " << return_value_list.next->fn;
		fail = 2;
	}

	free_param_list(parameter_list.next);
	free_param_list(return_value_list.next);

	parameter_list.next = NULL;
	return_value_list.next = NULL;

	return fail;
}

#endif
