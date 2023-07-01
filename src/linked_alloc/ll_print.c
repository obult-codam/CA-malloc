#include "ft_malloc.h"
#include "libft.h"
#include "linked_alloc.h"
#include <stdio.h>

void print_alloc_info(void *v_alloc) {
	t_alloc_header *alloc = (t_alloc_header *)v_alloc;
	void	*start = alloc + ALLOC_HEADER_SIZE;
	void	*end = start + alloc->size;
	printf("%p - %p : %lu bytes\n", start, end, alloc->size);
}

void	print_zone_info(void *alloc_head)
{
	if (alloc_head == NULL)
		return ;
	ft_lstiter((t_list *)alloc_head, print_alloc_info);
}