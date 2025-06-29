#include "munit.h"
#include "jslib/memory.h"
#include "memory_internal.h"
#include "type.h"

#include <unistd.h>


/**
 **********************************
 * memory.h Tests
 **********************************
 */

/**
 * Tests align rounds x value up to next multiple of ALIGNMENT successfully
 */
static MunitResult test_align_roundsSizeUpToMultipleOfAlignment(const MunitParameter params[], void* fixture) {

	int n = 7;
	int exp = 8;

	int result = align(n, ALIGNMENT);

	munit_assert_int(exp, ==, result);

    return MUNIT_OK;
}

static MunitResult test_align_noRoundUpWhenRequestSizeIsPageSize(const MunitParameter params[], void* fixture) {
	int exp = 8;
	int size = 8;

	int result = align(size, ALIGNMENT);

	munit_assert_int(exp, ==, result);

	return MUNIT_OK;
}

static MunitResult test_allocMemory_returnsPointerToMemoryRegion(const MunitParameter params[], void* fixture) {
	
	// Act
	int* arr = (int*)allocMemory(5 * sizeof(int));
	for(int i = 0; i < 5; i++) {
		arr[i] = i;
	}

	// Assert
	munit_assert_ptr_not_null(arr);
	munit_assert_int(arr[0], ==, 0);
	munit_assert_int(arr[1], ==, 1);
	munit_assert_int(arr[2], ==, 2);
	munit_assert_int(arr[3], ==, 3);
	munit_assert_int(arr[4], ==, 4);

	return MUNIT_OK;
}

static MunitResult test_allocMemory_returnsNullWhenSizeLessThanOne(const MunitParameter params[], void* fixture) {
	// arrange
	int size = 0;

	// act
	int* result = (int*)allocMemory(size * sizeof(int));

	// assert
	munit_assert_ptr_null(result);

	return MUNIT_OK;
}

static MunitResult test_deallocMemory_freesBlockAssociatedWithPointer(const MunitParameter params[], void* fixture) {
	// Arrange
	int* memory = (int*)allocMemory(500 * sizeof(int));
	

	// Act
	deallocMemory(memory);
	memory = NULL;

	// Assert
	munit_assert_int(globalPool.root->free, ==, TRUE);

	return MUNIT_OK;
}

/**
 **********************************
 * memory_internal.h Tests
 **********************************
 */

static MunitResult test_insertBlock_insertsBlocksAtEnd(const MunitParameter params[], void* fixture) {
	// Arrange
	Metadata m_one;
	m_one.next = NULL;
	m_one.size = 50;
	m_one.free = 0;

	Metadata m_two;
	m_two.next = NULL;
	m_two.size = 20;
	m_two.free = 0;

	Metadata m_three;
	m_three.next = NULL;
	m_three.size = 40;
	m_three.free = 1;

	// Act
	int oneResult = insertBlock(&m_one);
	int twoResult = insertBlock(&m_two);
	int threeResult = insertBlock(&m_three);

	// Assert all inserts successfull 
	munit_assert_int(oneResult, ==, 0);
	munit_assert_int(twoResult, ==, 0);
	munit_assert_int(threeResult, ==, 0);

	// Assert m_one and m_two insert
	munit_assert_ptr_not_null(m_one.next);
	munit_assert_int(m_one.next->free, ==, 0);
	munit_assert_int(m_one.next->size, ==, 20);

	// Assert m_two and m_three insert
	munit_assert_ptr_not_null(m_two.next);
	munit_assert_int(m_two.next->size, ==, 40);
	munit_assert_int(m_two.next->free, ==, 1);

	return MUNIT_OK;
}

static MunitResult test_search_returnsFreeBlockClosestToSize(const MunitParameter params[], void* fixture) {
	// Arrange
	Metadata m_one;
	m_one.next = NULL;
	m_one.size = 50;
	m_one.free = 0;

	Metadata m_two;
	m_two.next = NULL;
	m_two.size = 20;
	m_two.free = 0;

	Metadata m_three;
	m_three.next = NULL;
	m_three.size = 40;
	m_three.free = 1;

	insertBlock(&m_one);
	insertBlock(&m_two);
	insertBlock(&m_three);

	size_t size = 20;
	
	// Act
	Metadata* result = search(size);

	// Assert
	munit_assert_int(m_three.size, ==, result->size);
	munit_assert_int(m_three.free, ==, result->free);

	return MUNIT_OK;
}

static MunitResult test_search_returnsNullWhenNoEmptyBlocks(const MunitParameter params[], void* fixture) {
	// Arrange
	Metadata m_one;
	m_one.next = NULL;
	m_one.size = 50;
	m_one.free = 0;

	Metadata m_two;
	m_two.next = NULL;
	m_two.size = 20;
	m_two.free = 0;

	Metadata m_three;
	m_three.next = NULL;
	m_three.size = 40;
	m_three.free = 0;

	insertBlock(&m_one);
	insertBlock(&m_two);
	insertBlock(&m_three);

	// Act
	size_t size = 20;
	Metadata* result = search(size);

	// Assert
	munit_assert_ptr_null(result);

	return MUNIT_OK;
}

static MunitResult test_search_returnsNullWhenNoMemoryPool(const MunitParameter params[], void* fixture) {
	
	// Act
	size_t size = 20;
	Metadata* result = search(size);

	// Assert
	munit_assert_ptr_null(result);

	return MUNIT_OK;
}

static MunitResult test_splitRegion_splitsMemoryRegionAndInsertsBothIntoPool(const MunitParameter params[], void* fixture) {
	// Arrange - Setup pool 
	Metadata* regionOne = (Metadata*)allocMemory(500);
	regionOne -= 1;
	regionOne->free = 1;

	size_t splitSize = 100;
	int expRegionTwoSize = 500 - align(100 + METADATA_SIZE, ALIGNMENT);

	// Act
	Metadata* result = splitRegion(regionOne, splitSize);
	Metadata* regionTwo = result->next;

	// Assert - region one
	munit_assert_ptr_not_null(result);
	munit_assert_int(result->size, ==, 100);
	munit_assert_int(result->free, ==, 1);

	// Assert - region two
	munit_assert_ptr_not_null(regionTwo);
	munit_assert_int(regionTwo->free, ==, 1);
	munit_assert_int(regionTwo->size, ==, expRegionTwoSize);

	return MUNIT_OK;
}

/**
 * TODO: Join is causing infinite loop
 */
static MunitResult test_joinRegion_combinesTwoValidFreeMemoryRegionsIntoOne(const MunitParameter params[], void* fixture) {
	// arrange
	Metadata mOne = {};
	mOne.next = NULL;
	mOne.size = 50;
	mOne.free = TRUE;

	Metadata mTwo = {};
	mTwo.next = &mOne;
	mTwo.size = 20;
	mTwo.free = TRUE;

	Metadata mThree = {};
	mThree.next = &mTwo;
	mThree.size = 40;
	mThree.free = TRUE;

	insertBlock(&mOne);
	insertBlock(&mTwo);
	insertBlock(&mThree);

	int expSize = mThree.size + mTwo.size + METADATA_SIZE;
	void* expNext = &mOne;

	// act
	Metadata* result = joinRegion(&mThree, &mTwo);

	// assert
	munit_assert_int(result->size, ==, expSize);
	munit_assert_ptr_equal(result->next, expNext);

	return MUNIT_OK;

}

/**********************************/

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
		"/test-insertBlock-insertsBlocksAtEnd",
		test_insertBlock_insertsBlocksAtEnd,
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
	{
		"test-search-returnsFreeBlockClosestToSize",
		test_search_returnsFreeBlockClosestToSize,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test-search-returnsNullWhenNoMemoryPool",
		test_search_returnsNullWhenNoMemoryPool,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test-search-returnsNullWhenNoEmptyBlocks",
		test_search_returnsNullWhenNoEmptyBlocks,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test-allocMemory-returnsPointerToMemoryRegion",
		test_allocMemory_returnsPointerToMemoryRegion,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test_splitRegion_splitsMemoryRegionAndInsertsBothIntoPool",
		test_splitRegion_splitsMemoryRegionAndInsertsBothIntoPool,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test-deallocMemory-freesBlockAssociatedWithPointer",
		test_deallocMemory_freesBlockAssociatedWithPointer,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test_allocMemory_returnsNullWhenSizeLessThanOne",
		test_allocMemory_returnsNullWhenSizeLessThanOne,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"test_joinRegion_combinesTwoValidFreeMemoryRegionsIntoOne",
		test_joinRegion_combinesTwoValidFreeMemoryRegionsIntoOne,
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