#include "ft_malloc.h"

size_t	min_sizet(size_t one, size_t two) {
	if (one < two)
		return one;
	return two;
}

void	*out_of_zone_realloc(void *ptr, size_t prev_size, size_t new_size) {
	void	*new_alloc = malloc(new_size);
	ft_memcpy(new_alloc, ptr, min_sizet(prev_size, new_size));
	free(ptr);
	return (new_alloc);
}

void	*realloc(void *ptr, size_t size) {
	if (ptr == NULL)
		return (NULL);

	t_list	**pl_zone = find_zone_pl(ptr);

	if (pl_zone == NULL)
		pointer_not_allocated();

	
	// call the realloc strategy
	if (((t_zone_header *)((*pl_zone)->content))->zone_type == zone_is_type(size))
		return (realloc_strategy(ptr, *pl_zone, size));

	return (out_of_zone_realloc(ptr, size, size)); // inefficient because it will copy the total size of new alloc
}