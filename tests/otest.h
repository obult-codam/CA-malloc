#pragma once
#include <stdio.h>

// Print the result of the test.
#define TEST(result) do {                   \
    if (result)                             \
        printf("[\033[32mOK\033[0m]  ");    \
    else                                    \
        printf("[\033[31mKO\033[0m]  ");    \
} while (0);

// Print helper for tester
#define MODULE(module_name) printf("\n%s :\n", module_name)
