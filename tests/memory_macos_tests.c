#include "munit.h"
#include "jslib/memory.h"

/**
 * Tests align rounds x value up to next multiple of ALIGNMENT successfully
 */
static MunitResult test_align_success(const MunitParameter params[], void* fixture) {

	int n = 7;
	int exp = 8;

	int result = align(n);

	munit_assert_int(exp, ==, result);

    return MUNIT_OK;
}

// TODO: 
// Tests if align returns value passed in when value is already a multiple of ALIGNMENT
// Test changing ALIGNMENT and calling align still rounds number



static MunitTest tests[] = {
	{
		"/test-align-success",
		test_align_success,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	// Required to end array with null terminating entry b/c otherwise munit seg faults
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

const MunitSuite memorySuite = {
    "/memory-tests",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};