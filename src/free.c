#include "ft_malloc.h"

void	print_zone_size(t_zone_header *zone);

void	double_free() {
	// printf("double_free\n");
	exit(2);
}

void	pointer_not_allocated() {
	// printf("pointer_not_allocated\n");
	exit(1);
}

bool	is_inside_zone(t_list *l_zone, void *ptr) {
	t_zone_header *zone = (t_zone_header *)l_zone->content;
	return (ptr >= (void *)l_zone && ptr < ((void *)l_zone + zone->zone_size));
}

bool	is_linked_alloc(t_list *l_alloc, void *ptr) {
	return (ptr == (void *)l_alloc->content + ALLOC_HEADER_SIZE);
}

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

void	free(void *ptr) {
	t_list	**pl_zone;
	t_list	*l_zone;

	if (ptr == NULL)
		return ;

	// this is the find pointer link part
	// needs its own function for use in realloc
	pl_zone = ft_lstfind_pl(&g_head, ptr, is_inside_zone);
	if (pl_zone == NULL)
		pointer_not_allocated();

	l_zone = *pl_zone;
	t_zone_header *zone = (t_zone_header *)l_zone->content;
	t_list **pl_alloc = ft_lstfind_pl(&zone->alloc_head, ptr, is_linked_alloc);
	if (pl_alloc == NULL) {
		double_free();
	}
	// this is the free part
	*pl_alloc = (*pl_alloc)->next;
	// check if the zone is empty and munmap()
	cleanup_zone_check(pl_zone);
	return ;
}