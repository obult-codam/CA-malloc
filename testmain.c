#include <stdio.h>
#include "ft_malloc.h"

int	main(void) {
	printf("pagesize: %i\n", getpagesize());
	printf("head: %p", g_head);
	return (0);
}
