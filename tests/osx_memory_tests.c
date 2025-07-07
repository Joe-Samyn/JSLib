#include "munit.h"
#include "jslib/memory.h"
#include "memory_internal.h"
#include "type.h"

#include <unistd.h>


/* =========== memory.h Tests ============ */

static MunitResult test_buddyInitGlobal_initializesGlobalBuddyAllocator(const MunitParameter params[], void* fixture) 
{
	// Arrange 
	int size = 10;

	// Act
	int result = buddyInitGlobal(size);

	// Assert
	munit_assert_int(result, ==, SUCCESS);

	return MUNIT_OK;
}

static MunitResult test_buddyInitGlobal_initializesGlobalBuddyAllocatorOnlyOnce(const MunitParameter params[], void* fixture) 
{
	// Arrange 
	int size = 10;
	int expErr = PREV_INIT;

	// Act
	int firstInit = buddyInitGlobal(size);
	int result = buddyInitGlobal(size);

	// Assert
	munit_assert_int(firstInit, ==, SUCCESS);
	munit_assert_int(result, ==, ERROR);
	munit_assert_int(errorCode, ==, expErr);

	return MUNIT_OK;
}

static MunitResult test_buddyInitGlobal_failsWithErrorWhenMaxOrderZeroOrLess(const MunitParameter params[], void* fixture) 
{
	// Arrange 
	int size = 0;
	int expErr = INVAL_ARG;

	// Act
	int result = buddyInitGlobal(size);

	// Assert
	munit_assert_int(result, ==, ERROR);
	munit_assert_int(errorCode, ==, expErr);

	return MUNIT_OK;
}

/* =========== memory_internal.h Tests ============== */

/**********************************/

static MunitTest tests[] = {
	{
		"/test-buddyInitGlobal-initializesGlobalBuddyAllocator",
		test_buddyInitGlobal_initializesGlobalBuddyAllocator,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-buddyInitGlobal-initializesGlobalBuddyAllocatorOnlyOnce",
		test_buddyInitGlobal_initializesGlobalBuddyAllocatorOnlyOnce,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-buddyInitGlobal-failsWithErrorWhenMaxOrderZeroOrLess",
		test_buddyInitGlobal_failsWithErrorWhenMaxOrderZeroOrLess,
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