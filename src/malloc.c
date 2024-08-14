#include "ft_malloc.h"
#include "new_api.h"

#define ZONE_REST_SIZE LL_NODE_SIZE + ZONE_HEADER_SIZE

t_list *g_head;

t_zone_type	zone_is_type(size_t size) {
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
		void *alloc = create_alloc(((t_zone_header *)l_tmp->content)->alloc_head, size);
		if (alloc != NULL)
			return (alloc);
		l_tmp = find_zone_by_type(l_tmp->next, z_type);
	}
	// create zone
	// size not used for non-LARGE zones
	// large zones are always created here
	t_list	*l_new_zone = create_zone(z_type, size);
	if (l_new_zone == NULL)
		return (NULL);

	// append zone to tail
	ft_lstadd_back(&g_head, l_new_zone);

	t_zone_header *zone_header = l_new_zone->content;
	void *head = zone_header->alloc_head;

	// setup_zone (from api)
	setup_zone(head, alloc_size_category(z_type), zone_header->zone_size - ZONE_REST_SIZE);

	// create_alloc (from api)
	void *alloc = create_alloc(head, size);

	return (alloc);
}
