#include "ft_malloc.h"

static t_zone_type	zone_is_type(size_t size) {
	if (size <= TINY_SIZE)
		return (TINY);
	else if (size <= SMALL_SIZE)
		return (SMALL);
	return (LARGE);
}

// use find_zone to find a suitable zone else use create_zone to create one,
// when that fails return NULL 
void	*malloc(size_t size) {	// possibly already malloc
	t_list		*l_tmp;
	t_zone_type	z_type = zone_is_type(size);

	// create zone directly and append to tail for LARGE zone
	// if (z_type == LARGE)
	// 	return handle_large_zone(size);
	// check if there is room free in an existing zone
	l_tmp = find_zone_by_type(g_head, z_type);
	while (l_tmp != NULL) {
		void	*alloc = create_alloc_strategy((t_zone_header *)l_tmp->content, size);
		if (alloc != NULL)
			return (alloc);
		l_tmp = find_zone_by_type(l_tmp->next, z_type);
	}
	// create zone
	// size not used for non-LARGE zones
	// large zones are always created here
	t_list	*l_new_zone = create_zone(z_type, size);
	if (l_new_zone == NULL)
		return (l_new_zone);

	// append zone to tail
	ft_lstadd_back(&g_head, l_new_zone);
	// need to add alloc too!!
	// create allocation header
	// return pointer to alocation
	void	*alloc = create_alloc_strategy((t_zone_header *)l_new_zone->content, size);
	return (alloc);
}
