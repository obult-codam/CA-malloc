#include <assert.h>
#include "new_api.h"
#include <stdlib.h>

#include "otest.h"

#define MAX_ALLOCS 3
#define MAX_ALLOC_SIZE 8
#define CELL_SIZE 128

void print_debug(void *head);

int main(void)
{
    MODULE("Basics");
    /**
     *  Test zone_internal_size()
     */
    size_t size;

    size = calculate_required_size(MAX_ALLOC_SIZE, MAX_ALLOCS);
    TEST(size > MAX_ALLOCS * MAX_ALLOC_SIZE);

    /**
     * Test setup_zone()
     */
    // fake memory page
    void *head[16384];
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
    TEST(pointer != pointer2);

    char *pointer3;
    pointer3 = create_alloc(head, 8);
    TEST(pointer3 != NULL);
    pointer3[7] = '1'; // last addresable byte in the page.
    print_debug(head);

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

    realloc_ptr1 = resize_alloc(head, NULL, CELL_SIZE * 8 + 42);
    TEST(realloc_ptr1 == NULL);

    void *realloc_ptr2;
    realloc_ptr1 = create_alloc(head, 2);
    realloc_ptr2 = resize_alloc(head, realloc_ptr1, 8);
    TEST(realloc_ptr1 == realloc_ptr2);


    /* To big */
    void *realloc_ptr3;
    realloc_ptr3 = resize_alloc(head, realloc_ptr2, 400);
    TEST(realloc_ptr3 == NULL);

    void *normal_ptr1 = create_alloc(head, 8);
    void *normal_ptr2 = create_alloc(head, 8);
    TEST(normal_ptr1 != NULL && normal_ptr2 != NULL);

    /* Request bigger than allowed realloc in this zone, this req is dubious.. */
    // reason its dubious is becasue it should not be enforced here like in the create alloc but in the upper layer instead. need to chang emaybe.
    void *realloc_ptr4;
    realloc_ptr4 = resize_alloc(head, realloc_ptr2, 10);
    TEST(realloc_ptr4 == NULL);

    cleanup_alloc(head, normal_ptr1);
    void *realloc_ptr5;
    realloc_ptr5 = resize_alloc(head, realloc_ptr1, 8);
    TEST(realloc_ptr1 == realloc_ptr5);

    MODULE("Special realoc case");

    setup_zone(head, CELL_SIZE * 16, CELL_SIZE * 12);
    void *cpt1, *cpt2;
    cpt1 = create_alloc(head, CELL_SIZE * 8);
    TEST(cpt1 != NULL);
    cpt2 = create_alloc(head, CELL_SIZE * 4);
    TEST(cpt2 == NULL);
    print_debug(head);

    setup_zone(head, CELL_SIZE * 16, 16384);

    void *srcp = create_alloc(head, CELL_SIZE * 8);
    void *resized;
    TEST(srcp != NULL);

    resized = resize_alloc(head, srcp, CELL_SIZE * 8 + 42);
    TEST(resized == srcp);

    resized = resize_alloc(head, srcp, CELL_SIZE * 16);
    TEST(resized == srcp);

    setup_zone(head, CELL_SIZE * 12, CELL_SIZE * 32);
    void *pt1, *pt2, *pt3, *pt4;
    pt1 = create_alloc(head, CELL_SIZE * 5);
    pt2 = create_alloc(head, CELL_SIZE * 5);
    pt3 = create_alloc(head, CELL_SIZE * 5);
    print_debug(head);
    cleanup_alloc(head, pt2);
    print_debug(head);

    pt2 = create_alloc(head, CELL_SIZE * 8);
    cleanup_alloc(head, pt2);
    pt4 = resize_alloc(head, pt1, CELL_SIZE * 11);
    TEST(pt4 == NULL);
    print_debug(head);

    fprintf(stderr, "\nEND tests!\n");
    return 0;
}
