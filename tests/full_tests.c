#include "otest.h"
#include <ft_malloc.h>
#include <stdio.h>

#define TS_SMALL 64
#define TS_MEDIUM 512
#define TS_LARGE 1800
#define ALLOC_LOAD 2048

int main(void)
{
	void *pt0, *pt1, *pt2, *pt3, *pt4;
	void *pt_arr[ALLOC_LOAD];

	/**
	 * Malloc
	 */
	MODULE("Malloc");

	pt0 = malloc(TS_SMALL);
	TEST(pt0 != NULL);

	pt1 = malloc(TS_SMALL);


	pt3 = malloc(TS_MEDIUM);
	pt4 = malloc(TS_LARGE);

	/**
	 * Realloc
	 */
	MODULE("Realloc");

	void *tmp;

	pt2 = realloc(pt0, TS_SMALL * 2);
	TEST(pt0 != pt2);

	tmp = realloc(pt4, TS_LARGE * 2);
	pt4 = realloc(tmp, TS_LARGE);
	TEST(pt4 == tmp);

	tmp = realloc(NULL, 42);
	TEST(tmp == NULL);

	tmp = realloc((void *)42, 42);
	TEST(tmp == NULL);

	pt0 = malloc(512);
	tmp = realloc(pt0, 64);
	TEST(tmp != pt0);

	/**
	 * Free
	 */
	MODULE("Free");
	show_alloc_mem();
	show_alloc_mem_ex();
	free(pt1);
	free(pt2);
	free(pt3);
	free(pt4);

	/* because lcov does not free everything we cannot see how a free function path works uless we first get a second page and then free everything on that page */
	for (uint32_t i = 0; i < ALLOC_LOAD; i++) {
		pt_arr[i] = malloc(TS_SMALL);
		if (pt_arr[i])
			((char *)pt_arr[i])[TS_SMALL - 1] = '0';
	}
	// show_alloc_mem();
	TEST(1); // still alive
	for (uint32_t i = 0; i < ALLOC_LOAD; i++)
	{
		free(pt_arr[i]);
	}

	fprintf(stderr, "\nEND tests!\n");
	return 0;
}