#include "munit.h"
#include "jslib/jsstr.h"
#include <stdio.h>



static MunitResult test_add_numbers(const MunitParameter params[], void* fixture) {
	
	int result = add(1, 3);
	munit_assert_int(result, ==, 4);

	return MUNIT_OK;

}


/* Define Test Suite */
static MunitTest tests[] = {
	{
		"/test-add-numbers",
		test_add_numbers,
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
