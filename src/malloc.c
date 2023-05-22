#include "ft_malloc.h"

// size only used on LARGE objects
size_t	calculate_zone_size(t_zone_type z_type, size_t size) {
	size_t	zone_sizes[2] = { TINY_SIZE, SMALL_SIZE };
	size_t	zone_size;
	int		page_size = getpagesize();

	if (z_type == LARGE)
		zone_size = size + sizeof(t_list) + sizeof(t_alloc_header);
	else
		zone_size = MIN_ZONE_ALLOC * (zone_sizes[z_type] + sizeof(t_list) + sizeof(t_alloc_header));

	// add space for t_list and zone_header
	zone_size += sizeof(t_zone_header) + sizeof(t_list);

	// rounded up to multiple of getpagesize();
	zone_size = ((zone_size / page_size) + 1) * page_size;
	return zone_size;
}

// create a zone which is requested by provide_zone
t_list	*create_zone(t_zone_type z_type, size_t alloc_size) {
	size_t	length;
	void	*mapped;

	// calculate the length
	length = calculate_zone_size(z_type, alloc_size);

	mapped = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//  check for failures
	if (mapped == MAP_FAILED)
		return NULL;

	// add a list object at the top?
	t_list *list_header = (t_list *) mapped;
	list_header->next = NULL;
	// why use this as offset? then zone_header comes after list_header part
	list_header->content = list_header + sizeof(*list_header);

	// initialize the mapped page with headers
	t_zone_header	*header = (t_zone_header *)list_header->content;
	header->alloc_head = NULL;
	header->zone_size = length;
	header->zone_type = z_type;

	// return the newly created zone (list item)
	return (list_header);
}

bool	zone_is_tiny(void *param) {
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == TINY);
}

bool	zone_is_small(void *param) {
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == SMALL);
}

bool	zone_is_large(void *param) {
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == LARGE);
}

// find an available zone for this alloc
t_list	*find_zone(t_list *start, t_zone_type z_type) {
	bool	(*zone_type_checks[3])(void *) = {
		zone_is_tiny,
		zone_is_small,
		zone_is_large,
	};
	return (ft_lstfind(start, zone_type_checks[z_type]));
}

t_zone_type zone_type(size_t size) {
	if (size <= TINY_SIZE)
		return (TINY);
	else if (size <= SMALL_SIZE)
		return (SMALL);
	return (LARGE);
}

bool	is_large_enough(void *begin, void *end, size_t requested_size, size_t header_size) {
	return ((end - begin - header_size - requested_size) >= 0);
}

bool	ll_next_has_space(t_list *node, void *data) {
	size_t	requested_size = *(size_t *)data;
	if (node == NULL || node->next == NULL)
		return (true);
	return is_large_enough(node, node->next, requested_size, LL_NODE_SIZE + ALLOC_HEADER_SIZE);
}


void	*create_insert_alloc(t_list **insert, t_list *new, size_t size) {
	t_list	*tail_end = *insert;
	*insert = new;
	new->next = tail_end;
	
	new->content = new + LL_NODE_SIZE;
	t_alloc_header	*new_alloc_header = new->content;
	new_alloc_header->size = size;
	return ((void *)new_alloc_header + ALLOC_HEADER_SIZE);
}

size_t	ll_and_alloc_size(t_list *node) {
	t_alloc_header	*alloc_header = (t_alloc_header *)node->content;
	return (ALLOC_HEADER_SIZE + LL_NODE_SIZE + alloc_header->size);
}

bool	tail_not_enough_space(t_zone_header *zone, t_list *prev, size_t size) {
	// node_size included for the node pointing to the zone
	void *end = zone + zone->zone_size - LL_NODE_SIZE;
	void *begin = prev + ll_and_alloc_size(prev);
	return ((begin + size) > end);
}

void	*ll_alloc_strat(t_zone_header *zone, size_t size) {
	t_list			*l_item;

	l_item = ft_lstfind_l(zone->alloc_head, &size, ll_next_has_space);
	if (l_item == NULL) {
		// return create alloc and save it on alloc_head
		return create_insert_alloc(&zone->alloc_head, (t_list *)(zone + ZONE_HEADER_SIZE), size);
	}
	else if (l_item->next == NULL) {
		// check if there is enough space at tail
		if (tail_not_enough_space(zone, l_item, size))
			return (NULL);
		// return NULL or add at tail and return create alloc and save on tail
		return create_insert_alloc(&l_item->next, l_item + ll_and_alloc_size(l_item), size);
	}
	else {
		// add this alloc inbetween this and next alloc
		return create_insert_alloc(&l_item->next, l_item + ll_and_alloc_size(l_item), size);
	}
}

void	*handle_large_zone(size_t size) {

}

// use find_zone to find a suitable zone else use create_zone to create one,
// when that fails return NULL 
void	*malloc(size_t size) {	// possibly already malloc
	t_list		*l_tmp;
	t_zone_type	z_type = zone_type(size);

	// create zone directly and append to tail for LARGE zone
	// if (z_type == LARGE)
	// 	return handle_large_zone(size);
	// check if there is room free in an existing zone
	l_tmp = find_zone(g_head, z_type);
	while (l_tmp != NULL) {
		void	*alloc = ll_alloc_strat((t_zone_header *)l_tmp->content, size);
		if (alloc != NULL)
			return (alloc);
		l_tmp = find_zone(l_tmp->next, z_type);
	}
	// create zone
	// size not used for non-LARGE zones
	// large zones are always created here
	t_list	*l_new_zone = create_zone(z_type, size);
	if (l_new_zone == NULL)
		return (l_new_zone);

	// append zone to tail
	ft_lstadd_back(&g_head, l_new_zone);

	// create allocation header
	// return pointer to alocation
	return (ll_alloc_strat((t_zone_header *)l_new_zone->content, size));
}
