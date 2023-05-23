#include "ft_malloc.h"

void	*ll_realloc(void *ptr, t_list *l_zone, size_t size) {
	t_list	**pl_alloc = ll_find_alloc(ptr, l_zone);

	// check if the alloc can be done in place
	if (ll_realloc_has_space(*pl_alloc, size) == true) {
		t_alloc_header *alloc = (t_alloc_header *)(*pl_alloc)->content;
		alloc->size = size;
		return (ptr);
	}

	// need to move it to another place and copy the content then remove the current alloc
}