#include "ft_malloc.h"
#include "linked_alloc.h"
#include <stdint.h>

void	*realloc_strategy(void *ptr, t_list *l_zone, size_t size) {
	t_list	**pl_alloc = ll_find_alloc(ptr, l_zone);
	t_alloc_header *alloc = (t_alloc_header *)(*pl_alloc)->content;

	// check if the alloc can be done in place
	if (ll_realloc_has_space(*pl_alloc, size) == true) {
		alloc->size = size;
		return (ptr);
	}

	return (out_of_zone_realloc(ptr, alloc->size, size));
}

void *resize_alloc(void *head, void *ptr, size_t size)
{

}