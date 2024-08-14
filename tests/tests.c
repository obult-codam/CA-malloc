#include <assert.h>
#include "new_api.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_ALLOCS 3
#define MAX_ALLOC_SIZE 8

#define TEST(result) do {                   \
    if (result)                             \
        printf("[\033[32mOK\033[0m]  ");    \
    else                                    \
        printf("[\033[31mKO\033[0m]  ");    \
} while (0);

#define MODULE(module_name) printf("\n%s :\n", module_name)

int main(void)
{
    MODULE("Basics");
    /**
     *  Test zone_internal_size()
     */
    size_t size;

    size = calculate_required_size(MAX_ALLOC_SIZE, MAX_ALLOCS);
    printf("size == %u\n", size);
    TEST(size > MAX_ALLOCS * MAX_ALLOC_SIZE);

    /**
     * Test setup_zone()
     */
    // fake memory page
    void *head = malloc(size);
    setup_zone(head, MAX_ALLOC_SIZE, size);

    /* Zone is empty. */
    TEST(zone_is_empty(head));

    MODULE("Create alloc");
    /**
     * Test create_alloc()
     */
    void *pointer;
    pointer = create_alloc(head, 8);
    TEST(pointer != NULL);

    /* Second alloc (not first) */
    void *pointer2;
    pointer2 = create_alloc(head, 8);
    TEST(pointer2 != NULL);

    char *pointer3;
    pointer3 = create_alloc(head, 8);
    TEST(pointer3 != NULL);
    pointer3[7] = '1'; // last addresable byte in the page.

    // this should return NULL
    void *pointer4;
    pointer4 = create_alloc(head, 8);
    TEST(pointer4 == NULL);

    MODULE("cleanup");

    // delete the second alloc
    cleanup_alloc(head, pointer2);
    // TEST() test if head head somehow points to 3 // need to include the file for that but that is not possible for a general test..

    /* Add another alloc for inbetween, should work now */
    void *pointer5;
    pointer5 = create_alloc(head, 8);
    TEST(pointer5 == pointer2);

    cleanup_alloc(head, pointer);

    TEST(!zone_is_empty(head));

    cleanup_alloc(head, pointer2);
    cleanup_alloc(head, pointer3);
    cleanup_alloc(head, pointer4);
    TEST(zone_is_empty(head));

    MODULE("Realloc");

    void *realloc_ptr1;
    void *realloc_ptr2;
    realloc_ptr1 = create_alloc(head, 2);
    realloc_ptr2 = resize_alloc(head, realloc_ptr1, 8);
    TEST(realloc_ptr1 == realloc_ptr2);



    void *realloc_ptr3;
    realloc_ptr3 = resize_alloc(head, realloc_ptr2, 100);
    TEST(realloc_ptr3 == NULL);

    void *normal_ptr1 = create_alloc(head, 8);
    void *normal_ptr2 = create_alloc(head, 8);
    TEST(normal_ptr1 != NULL && normal_ptr2 != NULL);

    void *realloc_ptr4;
    realloc_ptr4 = resize_alloc(head, realloc_ptr2, 10);
    TEST(realloc_ptr4 == NULL);

    cleanup_alloc(head, normal_ptr1);
    void *realloc_ptr5;
    realloc_ptr5 = resize_alloc(head, realloc_ptr2, 16); // illegal but not this functions task to check that.
    TEST(realloc_ptr2 == realloc_ptr5);

    printf("\nEND tests!\n");
    return 0;
}