#include <stdio.h>
#include "ft_malloc.h"

int	main(void) {
	printf("pagesize: %i\n", getpagesize());
	char *head;
	printf("head: %p\n", head);
	head = (char *)malloc(8);
	printf("head: %p\n", head);

	void	*small = malloc(512);
	printf("small : %p\n", small);

		void	*large = malloc(4096);	// will use atleast two pages
	printf("large : %p\n", large);


	// this is meant to cause a segf
	// int i = 0;
	// while (1) {
	// 	head[i] = 'i';
	// 	// printf("nr: %i\n", i);
	// 	i++;
	// }
	return (0);
}
