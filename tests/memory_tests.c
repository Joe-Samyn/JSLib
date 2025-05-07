#include "munit.h"
#include "memory.h"

/**
 * Tests align rounds x value up to next multiple of ALIGNMENT successfully
 */
static MunitResult test_align_success(const MunitParameter params[], void* fixture) {

    return MUNIT_OK;
}

static MunitTest tests[] = {

};

static MunitSuite memorySuite = {
    "/memory-tests",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};