#include "munit.h"
#include "jslib/string.h"
#include <stdio.h>
#include <stdlib.h>
 
/****************
 * strLen Tests *
 ****************/
static MunitResult test_str_len(const MunitParameter params[], void* fixture) {

	char* test_str = "Hello, world!\0";
	int exp = 13;

	int result = strLen(test_str);

	munit_assert_int(result, ==, exp);
	
	return MUNIT_OK;
}

static MunitResult test_str_len_long_string(const MunitParameter params[], void* fixture) {
	char str[1000];
	for (int i = 0; i < 999; i++) {
		str[i] = 'c';
	}

	str[999] = '\0';
	
	// Excluding null terminator, length is 999
	int exp = 999;
	int result = strLen(str);

	munit_assert_int(result, ==, exp);
	return MUNIT_OK;
}

/****************
 * strCpy Tests *
 ****************/
// Happy path
static MunitResult test_strCpy_success(const MunitParameter params[], void* fixture) {
	
	// TODO: Needs to be changed to use this libraries memory allocation 
	char* src = "Hello!";
	char* dest = (char*)malloc(sizeof(char) * (str_len(src) + 1));
	
	str_cpy(dest, src);

	munit_assert_string_equal(dest, src);

	return MUNIT_OK;	
}

// dest* is null
static MunitResult test_strCpy_destIsNull(const MunitParameter params[], void* fixture) {

    char* src = "Hello!";
    char* dest = NULL;

    StrResult result = strCpy(dest, 0, src); 
    
    munit_assert_int(result, ==, STR_NULL_PTR);
    
    return MUNIT_OK;
}

// src* is null
static MunitResult test_strCpy_srcIsNull(const MunitParameter params[], void* fixture) 
{
    char* src = NULL;
    char* dest = (char*)malloc(sizeof(char) * 1);

    StrResult result = strCpy(dest, 0, src);

    munit_assert_int(result, ==, STR_NULL_PTR);
    return MUNIT_OK;

}

// src size < dest size and fills dest with terminating 0
static MunitResult test_strCpy_srcSizeLessDestSize(const MunitParameter params[], void* fixture)
{
    return MUNIT_OK;
}

// src size > dest size and dest has no terminating character
static MunitResult test_strCpy_srcSizeMoreDestSize(const MunitParameter params[], void* fixture)
{
    return MUNIT_OK;
}

/*****************
 * strNCpy Tests *
 *****************/
MunitResult test_str_ncpy_n_less_than_length(const MunitParameter params[], void* fixture) {
	// TODO: Needs to be changed to use library memory allocator
	char src[] = "Hello";
	char* dest = (char*)malloc(sizeof(char) * 3);
	
	char* exp = "Hel";

	str_ncpy(dest, src, 3);

	munit_assert_string_equal(dest, exp);

	return MUNIT_OK;
}


/* Define Test Suite */
static MunitTest tests[] = {
	{
		"/test-str-len",
		test_str_len,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-str-len-long-string",
		test_str_len_long_string,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-strCpy-success",
		test_strCpy_success,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
    {
		"/test-strCpy-srcNull",
		test_strCpy_srcIsNull,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
    {
		"/test-strCpy-destIsNull",
		test_strCpy_destIsNull,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-str-ncpy-n-less-than-length",
		test_str_ncpy_n_less_than_length,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	// Required to end array with null terminating entry b/c otherwise munit seg faults
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

const MunitSuite string_suite = {
	"/string-tests",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};
