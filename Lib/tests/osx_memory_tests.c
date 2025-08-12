#include "memory_internal.h"
#include "munit.h"
#include "type.h"
#include "jslib/memory.h"

#include <math.h> // TODO: Write own log function
#include <unistd.h>

/* =========== memory.h Tests ============ */

static MunitResult test_buddyInitGlobal_initializesGlobalBuddyAllocator(const MunitParameter params[], void *fixture)
{
	// Arrange
	int size = 10;

	// Act
	int result = buddyInitGlobal(size);

	// Assert
	munit_assert_int(result, ==, SUCCESS);

	return MUNIT_OK;
}

static MunitResult test_buddyInitGlobal_initializesGlobalBuddyAllocatorOnlyOnce(const MunitParameter params[], void *fixture)
{
	// Arrange
	int size = 10;

	// Act
	int firstInit = buddyInitGlobal(size);
	int result = buddyInitGlobal(size);

	// Assert
	munit_assert_int(firstInit, ==, SUCCESS);
	munit_assert_int(result, ==, PREV_INIT);

	return MUNIT_OK;
}

static MunitResult test_buddyInitGlobal_failsWithErrorWhenMaxOrderZeroOrLess(const MunitParameter params[], void *fixture)
{
	// Arrange
	int size = 0;
	int expErr = INVAL_ARG;

	// Act
	int result = buddyInitGlobal(size);

	// Assert
	munit_assert_int(result, ==, expErr);

	return MUNIT_OK;
}

static MunitResult test_buddyAlloc_returnsPtrToMemoryWithProperSize(const MunitParameter params[], void *fixture)
{
	// Arrange
	int initSize = 5;
	buddyInitGlobal(initSize);

	int size = 10;
	int expSize = 20;

	// Act
	void *result = buddyAlloc(size);
	// TODO: Probably a good idea to move this into a function called getHeader() because we perform this operation a lot in tests
	struct Header *header = (struct Header*)(result - HEADER_SIZE);

	// Assert
	munit_assert_ptr_not_null(result);
	munit_assert_int(header->size, ==, expSize);
	munit_assert_int(header->free, ==, FALSE);

	return MUNIT_OK;
}

static MunitResult test_buddyAlloc_returnsNullAndSetsErrorWhenInvalidSize(const MunitParameter params[], void *fixture)
{
	// Arrange
	int initSize = 5;
	buddyInitGlobal(initSize);

	int size = 0;

	// Act
	void *result = buddyAlloc(size);

	// Assert
	munit_assert_ptr_null(result);
	munit_assert_int(errorCode, ==, INVAL_ARG);

	return MUNIT_OK;
}

static MunitResult test_buddyFree_freesMemoryRegion(const MunitParameter params[], void* fixture) 
{
	// Arrange
	size_t order = 4;
	size_t memSize = 32;
	int s = sysconf(_SC_PAGE_SIZE);
	size_t expectedOrder = log2(s);
	buddyInitGlobal(order);

	// Act
	void* memory = buddyAlloc(memSize);
	buddyFree(memory);

	struct Header* freeMemory = (memory - HEADER_SIZE);
	struct Header* root = globalBuddyAllocator.start;

	// Assert
	munit_assert_int(freeMemory->free, ==, TRUE);
	munit_assert_int(root->order, ==, expectedOrder);

	return MUNIT_OK;
}

/* =========== memory_internal.h Tests ============== */

/* =========== Setup & Tear Down Functions =============== */
static void tearDownGlobalBuddyAlloc(void *fixture)
{
	globalBuddyAllocator = (struct BuddyAllocator){0};
}

/**********************************/

static MunitTest tests[] = {
	{"/test-buddyInitGlobal-initializesGlobalBuddyAllocator",
	 test_buddyInitGlobal_initializesGlobalBuddyAllocator,
	 NULL,
	 tearDownGlobalBuddyAlloc,
	 MUNIT_TEST_OPTION_NONE,
	 NULL},
	{"/test-buddyInitGlobal-initializesGlobalBuddyAllocatorOnlyOnce",
	 test_buddyInitGlobal_initializesGlobalBuddyAllocatorOnlyOnce,
	 NULL,
	 tearDownGlobalBuddyAlloc,
	 MUNIT_TEST_OPTION_NONE,
	 NULL},
	{"/test-buddyInitGlobal-failsWithErrorWhenMaxOrderZeroOrLess",
	 test_buddyInitGlobal_failsWithErrorWhenMaxOrderZeroOrLess,
	 NULL,
	 tearDownGlobalBuddyAlloc,
	 MUNIT_TEST_OPTION_NONE,
	 NULL

	},
	{"/test-buddyAlloc-returnsPtrToMemoryWithProperSize",
	 test_buddyAlloc_returnsPtrToMemoryWithProperSize,
	 NULL,
	 tearDownGlobalBuddyAlloc,
	 MUNIT_TEST_OPTION_NONE,
	 NULL},
	{"/test-buddyAlloc-returnsNullAndSetsErrorWhenInvalidSize",
	 test_buddyAlloc_returnsNullAndSetsErrorWhenInvalidSize,
	 NULL,
	 tearDownGlobalBuddyAlloc,
	 MUNIT_TEST_OPTION_NONE,
	 NULL},
	{"/test-buddyFree-freesMemoryRegion",
	 test_buddyFree_freesMemoryRegion,
	 NULL,
	 tearDownGlobalBuddyAlloc,
	 MUNIT_TEST_OPTION_NONE,
	 NULL
	},
	// Required to end array with null terminating entry b/c otherwise munit seg faults
	{NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

const MunitSuite memorySuite = {
	"/memory-tests",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE};