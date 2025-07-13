#include "munit.h"
#include "jslib/memory.h"
#include "memory_internal.h"
#include "type.h"

#include <unistd.h>
#include <math.h>	// TODO: Write own log function 


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

static MunitResult test_buddyAlloc_returnsPtrToMemoryWithProperSize(const MunitParameter params[], void* fixture)
{
	// Arrange
	int initSize = 5;
	buddyInitGlobal(initSize);

	int size = 10;
	int expSize = 40;

	// Act
	void* result = buddyAlloc(size);
	struct Header* header = ((struct Header*)(result - HEADER_SIZE));
	int resultSize = header->size;

	// Assert
	munit_assert_ptr_not_null(result);
	munit_assert_int(resultSize, ==, expSize);

	return MUNIT_OK;
}

static MunitResult test_buddyAlloc_returnsNullAndSetsErrorWhenInvalidSize(const MunitParameter params[], void* fixture)
{
	// Arrange
	int initSize = 5;
	buddyInitGlobal(initSize);

	int size = 0;

	// Act
	void* result = buddyAlloc(size);

	// Assert
	munit_assert_ptr_null(result);
	munit_assert_int(errorCode, ==, INVAL_ARG);

	return MUNIT_OK;
}

// TODO: This test wil
static MunitResult test_buddyAlloc_splitsMemoryCorrectNumberOfTimes(const MunitParameter params[], void* fixture)
{
	// Arrange
	int initSize = 5;
	buddyInitGlobal(initSize);

	int size = 10;
	int expMemorySize = 40;
	int expNodes = (int)floor(log2(sysconf(_SC_PAGE_SIZE) / expMemorySize));

	// Act
	void* memory = buddyAlloc(size);
	int nodes = 0;
	struct Header* node = globalBuddyAllocator.root;
	while (node->next)
	{
		nodes++;
		node = node->next;
	}

	// Assert
	munit_assert_int(nodes, ==, expNodes);

	return MUNIT_OK;
}

/* =========== memory_internal.h Tests ============== */

/* =========== Setup & Tear Down Functions =============== */
static void tearDownGlobalBuddyAlloc(void* fixture) 
{
	globalBuddyAllocator = (struct BuddyAllocator){0};
}

/**********************************/

static MunitTest tests[] = {
	{
		"/test-buddyInitGlobal-initializesGlobalBuddyAllocator",
		test_buddyInitGlobal_initializesGlobalBuddyAllocator,
		NULL,
		tearDownGlobalBuddyAlloc,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-buddyInitGlobal-initializesGlobalBuddyAllocatorOnlyOnce",
		test_buddyInitGlobal_initializesGlobalBuddyAllocatorOnlyOnce,
		NULL,
		tearDownGlobalBuddyAlloc,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-buddyInitGlobal-failsWithErrorWhenMaxOrderZeroOrLess",
		test_buddyInitGlobal_failsWithErrorWhenMaxOrderZeroOrLess,
		NULL,
		tearDownGlobalBuddyAlloc,
		MUNIT_TEST_OPTION_NONE,
		NULL

	},
	{
		"/test-buddyAlloc-returnsPtrToMemoryWithProperSize",
		test_buddyAlloc_returnsPtrToMemoryWithProperSize,
		NULL,
		tearDownGlobalBuddyAlloc,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-buddyAlloc-returnsNullAndSetsErrorWhenInvalidSize",
		test_buddyAlloc_returnsNullAndSetsErrorWhenInvalidSize,
		NULL,
		tearDownGlobalBuddyAlloc,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/test-buddyAlloc-splitsMemoryCorrectNumberOfTimes",
		test_buddyAlloc_splitsMemoryCorrectNumberOfTimes,
		NULL,
		tearDownGlobalBuddyAlloc,
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