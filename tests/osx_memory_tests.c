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