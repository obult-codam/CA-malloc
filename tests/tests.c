#include <assert.h>
#include "new_api.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_ALLOCS 64
#define MAX_ALLOC_SIZE 64

#define TEST(result) do {                   \
    if (result)                             \
        printf("[\033[32mOK\033[0m]  ");    \
    else                                    \
        printf("[\033[31mKO\033[0m]  ");    \
} while (0);

#define MODULE(module_name) printf("\n%s :\n", module_name)

int main(void)
{
    MODULE("Setup");
    /**
     *  Test zone_internal_size()
     */
    size_t size;

    size = calculate_required_size(MAX_ALLOC_SIZE, MAX_ALLOCS);
    TEST(size > 64 * 64);

    /**
     * Test setup_zone()
     */
    // fake memory page
    void *head = malloc(size);
    setup_zone(head, MAX_ALLOC_SIZE, size);

    MODULE("Create alloc");
    /**
     * Test create_alloc()
     */
    void *pointer;
    pointer = create_alloc(head, 8);
    TEST(pointer != NULL);
    // assert();




    printf("\nEND tests!\n");
    return 0;
}