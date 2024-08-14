#include "ft_malloc.h"
#include "libft.h"
#include <stdio.h>

void	print_zone_size(t_zone_header *zone) {
	char *zones_as_string[] = { "TINY: ", "SMALL:", "LARGE:" };
	printf("%s : %p size: %lu\n", zones_as_string[zone->zone_type], zone - LL_NODE_SIZE, zone->zone_size);
}

void	show_alloc_mem() {
	t_list *l_zone = g_head;

	printf("----------------\n");

	while (l_zone) {
		t_zone_header *zone = (t_zone_header *)l_zone->content;
		// print zone info (type + start location)
		print_zone_size(zone);

		// Strategy dependent
		print_info(zone->alloc_head);

		l_zone = l_zone->next;
	}
}
