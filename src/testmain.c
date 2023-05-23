#include <stdio.h>
#include "ft_malloc.h"
#include "libft.h"

int	main(void) {
	printf("pagesize: %i\n", getpagesize());
	char *head;
	printf("head: %p\n", head);
	head = (char *)malloc(8);
	ft_bzero(head, 8);
	printf("head: %p\n", head);

	void	*small = malloc(512);
	ft_bzero(small, 512);
	printf("small : %p\n", small);

	void	*large = malloc(4096);	// will use atleast two pages
	printf("large : %p\n", large);
	ft_bzero(large, 4096);

	void *tiny = malloc(16);
	// third malloc in small region causes problem
	// void *tiny2 = malloc(4);
// causes infinite loop
	// int i = 3;
	// while (i--) {
	// 	malloc(8);
	// }

	show_alloc_mem();

	free(large);
	free(small);
	printf("\n\n");

	show_alloc_mem();
	// insert new blocks in right spot in LL

	return (0);
}
