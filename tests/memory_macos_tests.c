#include "munit.h"
#include "jslib/memory.h"
#include "memory_internal.h"

#include <unistd.h>


/**
 **********************************
 * memory.h Tests
 **********************************
 */
static MunitResult test_alignment_isPageSizeOfMachine(const MunitParameter params[], void* fixture) {
	int exp = sysconf(_SC_PAGE_SIZE);
	int result = ALIGNMENT;
	munit_assert_int(exp, ==, result);

	return MUNIT_OK;
}

/**
 * Tests align rounds x value up to next multiple of ALIGNMENT successfully
 */
static MunitResult test_align_roundsSizeUpToMultipleOfAlignment(const MunitParameter params[], void* fixture) {

	int n = 7;
	int exp = 16384;

	int result = align(n);

	munit_assert_int(exp, ==, result);

    return MUNIT_OK;
}

static MunitResult test_align_noRoundUpWhenRequestSizeIsPageSize(const MunitParameter params[], void* fixture) {
	int exp = sysconf(_SC_PAGE_SIZE);
	int size = exp - METADATA_SIZE;

	int result = align(size);

	munit_assert_int(exp, ==, result);

	return MUNIT_OK;
}

/**
 **********************************
 * memory_internal.h Tests
 **********************************
 */

static MunitTest tests[] = {
	{
		"/test-align-success",
		test_align_roundsSizeUpToMultipleOfAlignment,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-alignment-isPageSize",
		test_alignment_isPageSizeOfMachine,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-align-noRondUpWhenRequestSizeIsPageSize",
		test_align_noRoundUpWhenRequestSizeIsPageSize,
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