#include "ft_malloc.h"

void	cleanup_zone(t_list **pl_zone, size_t size) {
	void	*to_unmap = (void *)*pl_zone;

	// remove the zone
	*pl_zone = (*pl_zone)->next;
	
	munmap(to_unmap, size);
}

bool	is_one_zone_of_type(t_zone_type type) {
	t_list			*tmp = g_head;
	t_zone_header	*zone;
	int 			count = 0;

	while (tmp != NULL) {
		zone = (t_zone_header *)tmp->content;
		if (zone->zone_type == type) {
			count++;
			if (count == 2)
				return false;
		}
		tmp = tmp->next;
	}
	return true;
}

void	cleanup_zone_check(t_list **pl_zone) {
	t_zone_header	*zone = (t_zone_header *)(*pl_zone)->content;
	// check if there are allocations, if there are then this zone does not need clearing
	if (zone->alloc_head != NULL)
		return ;

	// check if the block is of type large, yes means it needs to be unmapped;
	if (zone->zone_type == LARGE) {
		cleanup_zone(pl_zone, zone->zone_size);
	}
	// check if this is the only block available of this size;
	else if (is_one_zone_of_type(zone->zone_type) == false) {
		cleanup_zone(pl_zone, zone->zone_size);
	}
}
