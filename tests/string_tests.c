#include "munit.h"
#include "jslib/string.h"
#include <stdio.h>
#include <stdlib.h>
 
/****************
 * strLen Tests *
 ****************/

// Success case, str is not null & contains null terminating character

// str == NULL

// str has no null terminating and reaches max length

// maxLength = NULL

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
	char src[] = "Hello!";
	int destSize = strLen(src);
    char dest[destSize];

	strCpy(dest, destSize, src);

	munit_assert_string_equal(dest, src);

	return MUNIT_OK;	
}

// dest* is null
static MunitResult test_strCpy_destIsNull(const MunitParameter params[], void* fixture) {

    char src[] = "Hello!";
    char* dest = NULL;

    StrResult result = strCpy(dest, 0, src); 
    
    munit_assert_int(result, ==, STR_NULL_PTR);
    
    return MUNIT_OK;
}

// src* is null
static MunitResult test_strCpy_srcIsNull(const MunitParameter params[], void* fixture) 
{
    char* src = NULL;
    char destSize = 1;
    char dest[destSize];

    StrResult result = strCpy(dest, 0, src);

    munit_assert_int(result, ==, STR_NULL_PTR);
    return MUNIT_OK;

}

// src size < dest size and fills dest with terminating 0
static MunitResult test_strCpy_srcSizeLessDestSize(const MunitParameter params[], void* fixture)
{
    char* src = "he";
    int destSize = 4;
    char dest[destSize];

    StrResult result = strCpy(dest, destSize, src);
    
    munit_assert_int(dest[3], ==, '\0');
    munit_assert_int(dest[2], ==, '\0');
    munit_assert_int(result, ==, STR_OK);

    return MUNIT_OK;
}

// src size > dest size and dest has no terminating character
static MunitResult test_strCpy_srcSizeMoreDestSize(const MunitParameter params[], void* fixture)
{
    char src[] = "Hello!";
    char destSize = 3;
    char dest[destSize];

    StrResult result = strCpy(dest, destSize, src);

    munit_assert_int(dest[destSize - 1], ==, 'l');
    munit_assert_int(result, ==, STR_MISSING_TERM);

    return MUNIT_OK;
}

/*****************
 * strNCpy Tests *
 *****************/
// Case 1: Success path, dest is not null, destSize == maxChars == lenth of src + 1, returns STR_OK

// Case 2: destSize < maxChars

// Case 3: maxChars < 0

// Case 4: maxChars == 0

// Case 5: dest == NULL

// Case 6: src == NULL

// Case 7: destSize == maxChars but both less than length of source, should result in no null term 

// Case 8: destSize == maxChars but both > length of src, fills dest with null term until maxChars is reached 

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
		"/test-strCpy-srcSizeLessDestSize",
		test_strCpy_srcSizeLessDestSize,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
    {
		"/test-strCpy-srcSizeMoreDestSize",
		test_strCpy_srcSizeMoreDestSize,
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
