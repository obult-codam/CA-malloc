#include "ft_malloc.h"
#include "new_api.h"

void	cleanup_zone(t_list **pl_zone, size_t size) {
	void	*to_unmap = (void *)*pl_zone;

	// remove the zone
	*pl_zone = (*pl_zone)->next;

	munmap(to_unmap, size);
}

bool check_last_of_type(t_zone_type type)
{
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

// implemented for Linked-list strategy only
void	free(void *ptr) {
	if (ptr == NULL)
		return ;

	t_list	**pl_zone = find_zone_pl(ptr);
	if (pl_zone == NULL) {
		pointer_not_allocated();
		return;
	}

	t_zone_header *zone_header = (*pl_zone)->content;
	void *head = zone_header->alloc_head;

	// call the cleanup strategy
	cleanup_alloc(head, ptr);

	/* Check that the zone is still in use. */
	if (!zone_is_empty(head))
		return;

	if (zone_header->zone_type == LARGE) {
		cleanup_zone(pl_zone, zone_header->zone_size);
	} else if (!check_last_of_type(zone_header->zone_type)) {
		cleanup_zone(pl_zone, zone_header->zone_size);
	}
}