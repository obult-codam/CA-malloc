#include "ft_malloc.h"
#include "linked_alloc.h"

bool	is_linked_alloc(t_list *l_alloc, void *ptr) {
	return (ptr == (void *)l_alloc->content + ALLOC_HEADER_SIZE);
}

t_list	**ll_find_alloc(void *ptr, t_list *l_zone) {
	t_zone_header *zone = (t_zone_header *)l_zone->content;
	return (ft_lstfind_pl((t_list **)&zone->alloc_head, ptr, is_linked_alloc));
}
