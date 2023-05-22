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

	show_alloc_mem();

	free(large);

	show_alloc_mem();


	return (0);
}
