#include "ft_malloc.h"
#include "linked_alloc.h"

// can make it return bool to call double free ethod in te free or realloc
void	cleanup_alloc_strategy(void *ptr, t_list *l_zone) {
	t_list	**pl_alloc = ll_find_alloc(ptr, l_zone);
	if (pl_alloc == NULL) {
		double_free();
	}

	// this is the free part
	*pl_alloc = (*pl_alloc)->next;
}

void cleanup_alloc(void *head, void *ptr)
{
		
}