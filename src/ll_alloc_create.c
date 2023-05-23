#include "ft_malloc.h"

bool	is_large_enough(void *begin, void *end, size_t requested_size) {
	return ((end - begin - requested_size) >= 0);
}

// 

bool	ll_next_has_space(t_list *node, void *data) {
	size_t	requested_size = *(size_t *)data;
	if (node == NULL || node->next == NULL)
		return (true);
	size_t	alloc_size = ((t_alloc_header *)node->content)->size;
	void	*start = (void *)node + alloc_size + LL_NODE_SIZE + ALLOC_HEADER_SIZE;
	return (is_large_enough(start, node->next, requested_size + LL_NODE_SIZE + ALLOC_HEADER_SIZE));
}

bool	ll_realoc_has_space(t_list *node, size_t size) {
	if (node->next == NULL)
		return (true);

	void	*start = (void *)node;
	return (is_large_enough(start, node->next, size + LL_NODE_SIZE + ALLOC_HEADER_SIZE));
}

void	*create_insert_alloc(t_list **insert, t_list *new, size_t size) {
	t_list	*tail_end = *insert;
	*insert = new;
	new->next = tail_end;
	
	new->content = ((void *)new) + LL_NODE_SIZE;
	t_alloc_header	*new_alloc_header = new->content;
	new_alloc_header->size = size;
	return (((void *)new_alloc_header) + ALLOC_HEADER_SIZE);
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

void	*ll_create_alloc(t_zone_header *zone, size_t size) {
	t_list			*l_item;

	l_item = ft_lstfind_l(zone->alloc_head, &size, ll_next_has_space);
	if (l_item == NULL) {
		// return create alloc and save it on alloc_head
		return create_insert_alloc(&zone->alloc_head, (t_list *)((void *)zone + ZONE_HEADER_SIZE), size);
	}
	else if (l_item->next == NULL) {
		// check if there is enough space at tail
		if (tail_not_enough_space(zone, l_item, size))
			return (NULL);
		// return NULL or add at tail and return create alloc and save on tail
		return create_insert_alloc(&l_item->next, (void *)l_item + ll_and_alloc_size(l_item), size);
	}
	else {
		// add this alloc inbetween this and next alloc
		return create_insert_alloc(&l_item->next, (void *)l_item + ll_and_alloc_size(l_item), size);
	}
}
