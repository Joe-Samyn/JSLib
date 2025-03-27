#include "munit.h"
#include <stdio.h>


extern const MunitSuite string_suite;


int main(int argc, char* argv[]) {
	
	MunitSuite suites[] = {
		string_suite,
		{ NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }  // Sentinel entry
	};

	MunitSuite main_suite = {
		"",
		NULL,
		suites,	
		1,
		MUNIT_SUITE_OPTION_NONE
	};

	int result = munit_suite_main(&main_suite, "", argc, argv); 

	return result;
}
