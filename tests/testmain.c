#include <stdio.h>
#include "ft_malloc.h"
#include "libft.h"

int	main(void) {
	// printf("pagesize: %i\n", getpagesize());
	void	*ptr = malloc(16);
	// printf("tiny: %p\n", ptr);
	// printf("tiny: %p\n", malloc(16));
	// printf("small: %p\n", malloc(512));
	// printf("small : %p\n", malloc(512));
	// printf("small : %p\n", malloc(512));
	// printf("small : %p\n", malloc(512));

	// printf("large : %p\n", malloc(2048));
	// printf("large : %p\n", malloc(2048));
	// void *tofree = malloc(2048);
	// printf("large : %p\n", tofree);
	// printf("large : %p\n", malloc(2048));
	// printf("large : %p\n", malloc(2048));
	for (int i = 0; i < 200; i++)
	{
		malloc(16);
		printf("i: %i\n", i);
	}

	show_alloc_mem();
	ptr = realloc(ptr, 4096);

	// free(tofree);
	show_alloc_mem();
	printf("%lu\n", sizeof(t_zone_header));
	return (0);
}
