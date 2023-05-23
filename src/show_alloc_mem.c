#include "ft_malloc.h"
#include "libft.h"
#include <stdio.h>

void	print_zone_size(t_zone_header *zone) {
	char *zones_as_string[] = { "TINY: ", "SMALL:", "LARGE:" };
	printf("%s : %p size: %lu\n", zones_as_string[zone->zone_type], zone - LL_NODE_SIZE, zone->zone_size);
}

void print_alloc_info(void *v_alloc) {
	t_alloc_header *alloc = (t_alloc_header *)v_alloc;
	void	*start = alloc + ALLOC_HEADER_SIZE;
	void	*end = start + alloc->size;
	printf("%p - %p : %lu bytes\n", start, end, alloc->size);
}

void	show_alloc_mem() {
	t_list *l_zone = g_head;

	while (l_zone) {
		t_zone_header *zone = (t_zone_header *)l_zone->content;
		// print zone info (type + start location)
		print_zone_size(zone);

		// go over and print all alocations (lstiter?)
		ft_lstiter(zone->alloc_head, print_alloc_info);

		l_zone = l_zone->next;
	}
}
