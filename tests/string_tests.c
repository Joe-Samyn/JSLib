#include "munit.h"
#include "jslib/jsstr.h"


static MunitResult test_add_numbers(const MunitParameter params[], void* fixture) {
	
	int result = add(1, 3);
	munit_assert_int(result, ==, 4);

}


/* Define Test Suite */
MunitTests tests[] = {
	{
		"/test-add-numbers",
		test_add_numbers,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	}
};

static const MunitSuite string_suite = {
	"/string-tests",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};
