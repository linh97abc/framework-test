/*
 * Copyright (c) 2011-2014, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Misc utilities
 *
 * Misc utilities usable by the kernel and application code.
 */

#ifndef _MACRO_INCLUDE_SYS_UTIL_H_
#define _MACRO_INCLUDE_SYS_UTIL_H_

#include "util_macro.h"

/* needs to be outside _ASMLANGUAGE so 'true' and 'false' can turn
 * into '1' and '0' for asm or linker scripts
 */
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

#ifndef _ASMLANGUAGE

#include <stddef.h>

#if (!defined (__STDC_VERSION__) || __STDC_VERSION__ < 201112L) \
  && (!defined(__cplusplus) || __cplusplus < 201103L)

/*
 * A type with strong alignment requirements, similar to C11 max_align_t. It can
 * be used to force alignment of data structures allocated on the stack or as
 * return * type for heap allocators.
 */
typedef union {
	long long       thelonglong;
	long double     thelongdouble;
	uintmax_t       theuintmax_t;
	size_t          thesize_t;
	uintptr_t       theuintptr_t;
	void            *thepvoid;
	void            (*thepfunc)(void);
} max_align_t;

#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup sys-util Utility Functions
 * @{
 */

/** @brief Cast @p x, a pointer, to an unsigned integer. */
#define POINTER_TO_UINT(x) ((uintptr_t) (x))
/** @brief Cast @p x, an unsigned integer, to a <tt>void*</tt>. */
#define UINT_TO_POINTER(x) ((void *) (uintptr_t) (x))
/** @brief Cast @p x, a pointer, to a signed integer. */
#define POINTER_TO_INT(x)  ((intptr_t) (x))
/** @brief Cast @p x, a signed integer, to a <tt>void*</tt>. */
#define INT_TO_POINTER(x)  ((void *) (intptr_t) (x))

/** Number of bits in a long int. */
#define BITS_PER_LONG	(8 * sizeof(long))

/**
 * @brief Create a contiguous bitmask starting at bit position @p l
 *        and ending at position @p h.
 */
#define GENMASK(h, l) \
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

/** @brief Extract the Least Significant Bit from @p value. */
#define LSB_GET(value) ((value) & -(value))

/**
 * @brief Extract a bitfield element from @p value corresponding to
 *	  the field mask @p mask.
 */
#define FIELD_GET(mask, value)  (((value) & (mask)) / LSB_GET(mask))

/**
 * @brief Prepare a bitfield element using @p value with @p mask representing
 *	  its field position and width. The result should be combined
 *	  with other fields using a logical OR.
 */
#define FIELD_PREP(mask, value) (((value) * LSB_GET(mask)) & (mask))

/** @brief 0 if @p cond is true-ish; causes a compile error otherwise. */
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

#ifndef static_assert
#define static_assert(cond) typedef char __util_static_assert_result_t[1 - 2 * !(cond)]
#endif

/**
 * @brief Number of elements in the given @p array
 *
 * Warning: Do not passing a pointer as @p array.
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))


/**
 * @brief Check if a pointer @p ptr lies within @p array.
 *
 * In C but not C++, this causes a compile error if @p array is not an array
 * (e.g. if @p ptr and @p array are mixed up).
 *
 * @param ptr a pointer
 * @param array an array
 * @return 1 if @p ptr is part of @p array, 0 otherwise
 */
#define PART_OF_ARRAY(array, ptr) \
	((ptr) && ((ptr) >= &array[0] && (ptr) < &array[ARRAY_SIZE(array)]))

/**
 * @brief Get a pointer to a structure containing the element
 *
 * Example:
 *
 *	struct foo {
 *		int bar;
 *	};
 *
 *	struct foo my_foo;
 *	int *ptr = &my_foo.bar;
 *
 *	struct foo *container = CONTAINER_OF(ptr, struct foo, bar);
 *
 * Above, @p container points at @p my_foo.
 *
 * @param ptr pointer to a structure element
 * @param type name of the type that @p ptr is an element of
 * @param field the name of the field within the struct @p ptr points to
 * @return a pointer to the structure that contains @p ptr
 */
#define CONTAINER_OF(ptr, type, field) \
	((type *)(((char *)(ptr)) - offsetof(type, field)))

/**
 * @brief Value of @p x rounded up to the next multiple of @p align,
 *        which must be a power of 2.
 */
#define ROUND_UP(x, align)                                   \
	(((unsigned long)(x) + ((unsigned long)(align) - 1)) & \
	 ~((unsigned long)(align) - 1))

/**
 * @brief Value of @p x rounded down to the previous multiple of @p
 *        align, which must be a power of 2.
 */
#define ROUND_DOWN(x, align)                                 \
	((unsigned long)(x) & ~((unsigned long)(align) - 1))

/** @brief Value of @p x rounded up to the next word boundary. */
#define WB_UP(x) ROUND_UP(x, sizeof(void *))

/** @brief Value of @p x rounded down to the previous word boundary. */
#define WB_DN(x) ROUND_DOWN(x, sizeof(void *))

/**
 * @brief Ceiling function applied to @p numerator / @p divider as a fraction.
 */
#define ceiling_fraction(numerator, divider) \
	(((numerator) + ((divider) - 1)) / (divider))

#ifndef MAX
/**
 * @brief Obtain the maximum of two values.
 *
 * @note Arguments are evaluated twice. Use Z_MAX for a GCC-only, single
 * evaluation version
 *
 * @param a First value.
 * @param b Second value.
 *
 * @returns Maximum value of @p a and @p b.
 */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
/**
 * @brief Obtain the minimum of two values.
 *
 * @note Arguments are evaluated twice. Use Z_MIN for a GCC-only, single
 * evaluation version
 *
 * @param a First value.
 * @param b Second value.
 *
 * @returns Minimum value of @p a and @p b.
 */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef CLAMP
/**
 * @brief Clamp a value to a given range.
 *
 * @note Arguments are evaluated multiple times. Use Z_CLAMP for a GCC-only,
 * single evaluation version.
 *
 * @param val Value to be clamped.
 * @param low Lowest allowed value (inclusive).
 * @param high Highest allowed value (inclusive).
 *
 * @returns Clamped value.
 */
#define CLAMP(val, low, high) (((val) <= (low)) ? (low) : MIN(val, high))
#endif

/**
 * @brief Checks if a value is within range.
 *
 * @note @p val is evaluated twice.
 *
 * @param val Value to be checked.
 * @param min Lower bound (inclusive).
 * @param max Upper bound (inclusive).
 *
 * @retval true If value is within range
 * @retval false If the value is not within range
 */
#define IN_RANGE(val, min, max) ((val) >= (min) && (val) <= (max))

/**
 * @brief Is @p x a power of two?
 * @param x value to check
 * @return true if @p x is a power of two, false otherwise
 */
static inline bool is_power_of_two(unsigned int x)
{
	return (x != 0U) && ((x & (x - 1U)) == 0U);
}

/**
 * @brief Arithmetic shift right
 * @param value value to shift
 * @param shift number of bits to shift
 * @return @p value shifted right by @p shift; opened bit positions are
 *         filled with the sign bit
 */
static inline int64_t arithmetic_shift_right(int64_t value, uint8_t shift)
{
	int64_t sign_ext;

	if (shift == 0U) {
		return value;
	}

	/* extract sign bit */
	sign_ext = (value >> 63) & 1;

	/* make all bits of sign_ext be the same as the value's sign bit */
	sign_ext = -sign_ext;

	/* shift value and fill opened bit positions with sign bit */
	return (value >> shift) | (sign_ext << (64 - shift));
}

/**
 * @brief byte by byte memcpy.
 *
 * Copy `size` bytes of `src` into `dest`. This is guaranteed to be done byte by byte.
 *
 * @param dst Pointer to the destination memory.
 * @param src Pointer to the source of the data.
 * @param size The number of bytes to copy.
 */
static inline void bytecpy(void *dst, const void *src, size_t size)
{
	size_t i;

	for (i = 0; i < size; ++i) {
		((volatile uint8_t *)dst)[i] = ((volatile const uint8_t *)src)[i];
	}
}

/**
 * @brief byte by byte swap.
 *
 * Swap @a size bytes between memory regions @a a and @a b. This is
 * guaranteed to be done byte by byte.
 *
 * @param a Pointer to the the first memory region.
 * @param b Pointer to the the second memory region.
 * @param size The number of bytes to swap.
 */
static inline void byteswp(void *a, void *b, size_t size)
{
	uint8_t t;
	uint8_t *aa = (uint8_t *)a;
	uint8_t *bb = (uint8_t *)b;

	for (; size > 0; --size) {
		t = *aa;
		*aa++ = *bb;
		*bb++ = t;
	}
}

/**
 * @brief      Convert a single character into a hexadecimal nibble.
 *
 * @param c     The character to convert
 * @param x     The address of storage for the converted number.
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
static inline int char2hex(char c, uint8_t *x)
{
	if (c >= '0' && c <= '9') {
		*x = c - '0';
	} else if (c >= 'a' && c <= 'f') {
		*x = c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		*x = c - 'A' + 10;
	} else {
		return -EINVAL;
	}

	return 0;
}

/**
 * @brief      Convert a single hexadecimal nibble into a character.
 *
 * @param c     The number to convert
 * @param x     The address of storage for the converted character.
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
static inline int hex2char(uint8_t x, char *c)
{
	if (x <= 9) {
		*c = x + '0';
	} else  if (x <= 15) {
		*c = x - 10 + 'a';
	} else {
		return -EINVAL;
	}

	return 0;
}

/**
 * @brief Convert a binary coded decimal (BCD 8421) value to binary.
 *
 * @param bcd BCD 8421 value to convert.
 *
 * @return Binary representation of input value.
 */
static inline uint8_t bcd2bin(uint8_t bcd)
{
	return ((10 * (bcd >> 4)) + (bcd & 0x0F));
}

/**
 * @brief Convert a binary value to binary coded decimal (BCD 8421).
 *
 * @param bin Binary value to convert.
 *
 * @return BCD 8421 representation of input value.
 */
static inline uint8_t bin2bcd(uint8_t bin)
{
	return (((bin / 10) << 4) | (bin % 10));
}

#ifdef __cplusplus
}
#endif

#endif /* !_ASMLANGUAGE */

/**
 * @}
 */

#endif /* _MACRO_INCLUDE_SYS_UTIL_H_ */
