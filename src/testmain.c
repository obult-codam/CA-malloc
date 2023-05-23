#include <stdio.h>
#include "ft_malloc.h"
#include "libft.h"

int	main(void) {
	printf("pagesize: %i\n", getpagesize());

	printf("tiny: %p\n", malloc(16));
	printf("tiny: %p\n", malloc(16));
	printf("small: %p\n", malloc(512));
	printf("small : %p\n", malloc(512));
	printf("small : %p\n", malloc(512));
	printf("small : %p\n", malloc(512));

	printf("large : %p\n", malloc(2048));
	printf("large : %p\n", malloc(2048));
	printf("large : %p\n", malloc(2048));
	printf("large : %p\n", malloc(2048));
	printf("large : %p\n", malloc(2048));

	show_alloc_mem();
	return (0);
}
