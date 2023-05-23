#include "ft_malloc.h"

bool	zone_is_tiny(void *param) {
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == TINY);
}

bool	zone_is_small(void *param) {
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == SMALL);
}

bool	zone_is_large(void *param) {
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == LARGE);
}

bool	is_inside_zone(t_list *l_zone, void *ptr) {
	t_zone_header *zone = (t_zone_header *)l_zone->content;
	return (ptr >= (void *)l_zone && ptr < ((void *)l_zone + zone->zone_size));
}

// find an available zone for this alloc
t_list	*find_zone_by_type(t_list *start, t_zone_type z_type) {
	bool	(*zone_type_checks[3])(void *) = {
		zone_is_tiny,
		zone_is_small,
		zone_is_large,
	};
	return (ft_lstfind(start, zone_type_checks[z_type]));
}

t_list	**find_zone_pl(void *ptr) {
	t_list	**pl_zone;

	return ft_lstfind_pl(&g_head, ptr, is_inside_zone);
}
