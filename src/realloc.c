#include "ft_malloc.h"

void	*realloc(void *ptr, size_t size) {
	if (ptr == NULL)
		return (NULL);

	t_list	**pl_zone = find_zone_pl(ptr);

	if (pl_zone == NULL)
		pointer_not_allocated();
	
	// call the realloc strategy
	void	*pointer = realloc_strategy(ptr, *pl_zone, size);

	// check if the zone is empty and munmap() 
	// cleanup_zone_check(pl_zone); already done by free

	return (pointer);
}