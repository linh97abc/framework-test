/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <test.h>
#include <string.h>
#include <stdio.h>

struct parameter {
	struct parameter *next;
	const char *fn;
	const char *name;
	uintptr_t value;
};

#include <stdlib.h>
#include <stdarg.h>

/* ------------- PORT -----------------*/
static void free_parameter(struct parameter *param)
{
	free(param);
}

static struct parameter *alloc_parameter(void)
{
	struct parameter *param;

	param = calloc(1, sizeof(struct parameter));
	if (!param) {
		PRINT("Failed to allocate mock parameter\n");
		utest_test_fail();
	}

	return param;
}
/* -------------------------------------*/

void z_init_mock(void)
{
}


static struct parameter *find_and_delete_value(struct parameter *param,
					       const char *fn, const char *name)
{
	struct parameter *value;

	if (!param->next) {
		return NULL;
	}

	if (strcmp(param->next->name, name) || strcmp(param->next->fn, fn)) {
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
	while (param->next) {
		param = param->next;
	}

	/* Append to end of linked list */
	value->next = param->next;
	param->next = value;
}

static struct parameter parameter_list = { NULL, "", "", 0 };
static struct parameter return_value_list = { NULL, "", "", 0 };

void z_utest_expect_value(const char *fn, const char *name, uintptr_t val)
{
	insert_value(&parameter_list, fn, name, val);
}

void z_utest_check_expected_value(const char *fn, const char *name,
				  uintptr_t val)
{
	struct parameter *param;
	uintptr_t expected;

	param = find_and_delete_value(&parameter_list, fn, name);
	if (!param) {
		PRINT("Failed to find parameter %s for %s\n", name, fn);
		utest_test_fail();
	}

	expected = param->value;
	free_parameter(param);

	if (expected != val) {
		/* We need to cast these values since the toolchain doesn't
		 * provide inttypes.h
		 */
		PRINT("%s:%s received wrong value: Got %lu, expected %lu\n", fn,
		      name, (unsigned long)val, (unsigned long)expected);
		utest_test_fail();
	}
}

void z_utest_expect_data(const char *fn, const char *name, void *val)
{
	insert_value(&parameter_list, fn, name, (uintptr_t)val);
}

void z_utest_check_expected_data(const char *fn, const char *name, void *data,
				 uint32_t length)
{
	struct parameter *param;
	void *expected;

	param = find_and_delete_value(&parameter_list, fn, name);
	if (!param) {
		PRINT("Failed to find parameter %s for %s\n", name, fn);
		/* No return from this function but for coverity reasons
		 * put a return after to avoid the warning of a null
		 * dereference of param below.
		 */
		utest_test_fail();
		return;
	}

	expected = (void *)param->value;
	free_parameter(param);

	if (expected == NULL && data != NULL) {
		PRINT("%s:%s received null pointer\n", fn, name);
		utest_test_fail();
	} else if (data == NULL && expected != NULL) {
		PRINT("%s:%s received data while expected null pointer\n", fn,
		      name);
		utest_test_fail();
	} else if (data != NULL) {
		if (memcmp(data, expected, length) != 0) {
			PRINT("%s:%s data provided don't match\n", fn, name);
			utest_test_fail();
		}
	}
}

void z_utest_return_data(const char *fn, const char *name, void *val)
{
	insert_value(&parameter_list, fn, name, (uintptr_t)val);
}

void z_utest_copy_return_data(const char *fn, const char *name, void *data,
			      uint32_t length)
{
	struct parameter *param;
	void *return_data;

	if (data == NULL) {
		PRINT("%s:%s received null pointer\n", fn, name);
		utest_test_fail();
		return;
	}

	param = find_and_delete_value(&parameter_list, fn, name);
	if (!param) {
		PRINT("Failed to find parameter %s for %s\n", name, fn);
		memset(data, 0, length);
		utest_test_fail();
	} else {
		return_data = (void *)param->value;
		free_parameter(param);
		memcpy(data, return_data, length);
	}
}

void z_utest_returns_value(const char *fn, uintptr_t value)
{
	insert_value(&return_value_list, fn, "", value);
}

uintptr_t z_utest_get_return_value(const char *fn)
{
	uintptr_t value;
	struct parameter *param =
		find_and_delete_value(&return_value_list, fn, "");

	if (!param) {
		PRINT("Failed to find return value for function %s\n", fn);
		utest_test_fail();
	}

	value = param->value;
	free_parameter(param);

	return value;
}

static void free_param_list(struct parameter *param)
{
	struct parameter *next;

	while (param) {
		next = param->next;
		free_parameter(param);
		param = next;
	}
}

int z_cleanup_mock(void)
{
	int fail = 0;

	if (parameter_list.next) {
		PRINT("Parameter not used by mock: %s:%s\n",
		      parameter_list.next->fn,
		      parameter_list.next->name);
		fail = 1;
	}
	if (return_value_list.next) {
		PRINT("Return value no used by mock: %s\n",
		      return_value_list.next->fn);
		fail = 2;
	}

	free_param_list(parameter_list.next);
	free_param_list(return_value_list.next);

	parameter_list.next = NULL;
	return_value_list.next = NULL;

	return fail;
}
