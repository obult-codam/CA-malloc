#include <stdio.h>
#include "ft_malloc.h"

int	main(void) {
	printf("pagesize: %i\n", getpagesize());
	void *head;
	printf("head: %p\n", head);
	head = malloc(8);
	printf("head: %p\n", head);
	return (0);
}
