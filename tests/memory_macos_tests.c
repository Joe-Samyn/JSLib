#include "munit.h"
#include "jslib/memory.h"
#include "memory_internal.h"

#include <unistd.h>
#include <uuid/uuid.h>


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
 * TODO: This test will need to be fixed once memory region splitting is implemented
 */
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

	deallocMemory(NULL);
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

	// Tear down 
	deallocMemory(NULL);

	return MUNIT_OK;
}

static MunitResult test_search_returnsFreeBlockClosestToSize(const MunitParameter params[], void* fixture) {
	// Arrange
	Metadata m_one;
	uuid_generate(m_one.id);
	m_one.next = NULL;
	m_one.size = 50;
	m_one.free = 0;

	Metadata m_two;
	uuid_generate(m_two.id);
	m_two.next = NULL;
	m_two.size = 20;
	m_two.free = 0;

	Metadata m_three;
	uuid_generate(m_three.id);
	m_three.next = NULL;
	m_three.size = 40;
	m_three.free = 1;

	int oneResult = insertBlock(&m_one);
	int twoResult = insertBlock(&m_two);
	int threeResult = insertBlock(&m_three);

	size_t size = 20;
	
	// Act
	Metadata* result = search(size);

	// Assert
	munit_assert_int(m_three.size, ==, result->size);
	munit_assert_int(m_three.free, ==, result->free);
	munit_assert_int(uuid_compare(result->id, m_three.id), ==, 0);

	// Tear down
	deallocMemory(NULL);

	return MUNIT_OK;
}

static MunitResult test_search_returnsNullWhenNoEmptyBlocks(const MunitParameter params[], void* fixture) {
	// Arrange
	Metadata m_one;
	uuid_generate(m_one.id);
	m_one.next = NULL;
	m_one.size = 50;
	m_one.free = 0;

	Metadata m_two;
	uuid_generate(m_two.id);
	m_two.next = NULL;
	m_two.size = 20;
	m_two.free = 0;

	Metadata m_three;
	uuid_generate(m_three.id);
	m_three.next = NULL;
	m_three.size = 40;
	m_three.free = 0;

	int oneResult = insertBlock(&m_one);
	int twoResult = insertBlock(&m_two);
	int threeResult = insertBlock(&m_three);

	// Act
	size_t size = 20;
	Metadata* result = search(size);

	// Assert
	munit_assert_ptr_null(result);

	// Tear down 
	deallocMemory(NULL);
}

static MunitResult test_search_returnsNullWhenNoMemoryPool(const MunitParameter params[], void* fixture) {
	
	// Act
	size_t size = 20;
	Metadata* result = search(size);

	// Assert
	munit_assert_ptr_null(result);

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
		"/test-alignment-isPageSize",
		test_alignment_isPageSizeOfMachine,
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