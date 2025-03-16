#include "munit.h"

extern const MunitSuite string_suite;

static const MunitSuite main_suite = {
	"",
	NULL,
	(MunitSuite[]) { string_suite },
	1,
	MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
	return munit_suite_main(&main_suite, NULL, argc, argv); 
}
