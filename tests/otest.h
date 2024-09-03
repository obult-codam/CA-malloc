#pragma once
#include <stdio.h>

// Print the result of the test.
#define TEST(result) do {				   \
	if (result)							 \
		fprintf(stderr, "[\033[32mOK\033[0m]  ");	\
	else									\
		fprintf(stderr, "[\033[31mKO\033[0m]  ");	\
} while (0);

// Print helper for tester
#define MODULE(module_name) fprintf(stderr, "\n%s :\n", module_name)
