#include "munit.h"
#include "jslib/string.h"
#include <stdio.h>


static MunitResult test_str_len(const MunitParameter params[], void* fixture) {

	char* test_str = "Hello, world!\0";
	int exp = 13;

	int result = str_len(test_str);

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
	int result = str_len(str);

	munit_assert_int(result, ==, exp);

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
