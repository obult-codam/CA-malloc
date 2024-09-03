#include <assert.h>
#include "new_api.h"
#include <stdlib.h>

#include "otest.h"

#define MAX_ALLOCS 3
#define MAX_ALLOC_SIZE 8

int main(void)
{
	MODULE("Basics");
	/**
	 *  Test zone_internal_size()
	 */
	size_t size;

	size = calculate_required_size(MAX_ALLOC_SIZE, MAX_ALLOCS);
	TEST(size == MAX_ALLOCS * MAX_ALLOC_SIZE + (sizeof(size_t) * 2) + sizeof(void *));

	/**
	 * Test setup_zone()
	 */
	// fake memory page
	void *head[4096];
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

	// this should return NULL
	void *pointer4;
	pointer4 = create_alloc(head, 1);
	TEST(pointer4 == NULL);

	MODULE("cleanup");

	// delete the second alloc
	cleanup_alloc(head, pointer2);
	// TEST() test if head head somehow points to 3 // need to include the file for that but that is not possible for a general test..

	/* Add another alloc for inbetween, should work now */
	void *pointer5;
	pointer5 = create_alloc(head, 8);
	TEST(pointer5 != pointer2); // special case

	cleanup_alloc(head, pointer);

	TEST(!zone_is_empty(head));

	cleanup_alloc(head, pointer5);
	cleanup_alloc(head, pointer3);
	cleanup_alloc(head, pointer4);
	TEST(zone_is_empty(head));

	MODULE("Realloc");

	TEST(NULL == resize_alloc(head, pointer, 42));

	fprintf(stderr, "\nEND tests!\n");
	return 0;
}