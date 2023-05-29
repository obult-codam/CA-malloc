#include "ft_malloc.h"

// implemented for Linked-list strategy only
void	free(void *ptr) {
	if (ptr == NULL)
		return ;

	t_list	**pl_zone = find_zone_pl(ptr);
	if (pl_zone == NULL)
		pointer_not_allocated();
		
	// call the cleanup strategy
	cleanup_alloc_strategy(ptr, *pl_zone);

	// check if the zone is empty and munmap()
	cleanup_zone_check(pl_zone);
}