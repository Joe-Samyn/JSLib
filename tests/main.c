#include <stdio.h>

#define mu_assert(message, test) do { if (!test) return message } while(0)
#define mu_run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while(0)


int tests_run = 0;

char* sample_test() {
	return 0;
}

/**
 * Runs all automated tests
 * @returns 0 if all tests run successfully; otherwise error message of failed test.
 */
char* all_tests() {
	mu_run_test(sample_test);	
}

int main() {

	char *result = "Success";

	if (result) {
		printf(result);
	}
	else {
		printf("%d Tests completed successfully!.");
	}

	return 0;
}
