#include "ft_malloc.h"

// size only used on LARGE objects
size_t	calculate_zone_size(t_zone_type z_type, size_t size) {
	size_t	zone_sizes[2] = { TINY_SIZE, SMALL_SIZE };
	size_t	zone_size;
	int		page_size = getpagesize();

	if (z_type == LARGE)
		zone_size = size + sizeof(t_list) + sizeof(t_alloc_header);
	else
		zone_size = MIN_ZONE_ALLOC * (zone_sizes[z_type] + sizeof(t_list) + sizeof(t_alloc_header));

	// add space for t_list and zone_header
	zone_size += sizeof(t_zone_header) + sizeof(t_list);

	// rounded up to multiple of getpagesize();
	zone_size = ((zone_size / page_size) + 1) * page_size;
	return zone_size;
}

// create a zone which is requested by provide_zone
t_list	*create_zone(t_zone_type z_type, size_t alloc_size) {
	size_t	length;
	void	*mapped;

	// calculate the length
	length = calculatezone_size(z_type, alloc_size);

	mapped = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//  check for failures
	if (mapped == MAP_FAILED)
		return NULL;

	// add a list object at the top?
	t_list *list_header = (t_list *) mapped;
	list_header->next = NULL;
	// why use this as offset? then zone_header comes after list_header part
	list_header->content = list_header + sizeof(*list_header);

	// initialize the mapped page with headers
	t_zone_header	*header = (t_zone_header *)list_header->content;
	header->alloc_head = NULL;
	header->zone_size = length;
	header->zone_type = z_type;

	// return the newly created zone (list item)
	return (list_header);
}

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

// find an available zone for this alloc
t_list	*find_zone(t_list *start, t_zone_type z_type) {
	bool	(*zone_type_checks[3])(void *) = {
		zone_is_tiny,
		zone_is_small,
		zone_is_large,
	};
	return (ft_lstfind(start, zone_type_checks[z_type]));
}

t_zone_type zone_type(size_t size) {
	if (size <= TINY_SIZE)
		return (TINY);
	else if (size <= SMALL_SIZE)
		return (SMALL);
	return (LARGE);
}

// use find_zone to find a suitable zone else use create_zone to create one,
// when that fails return NULL 
void	*provide_zone(size_t size) {	// possibly already malloc
	t_list	*tmp;

	// create zone directly and append to tail for LARGE zone
	// while not tail
	while (tmp = find_zone(g_head, zone_type(size))) {
		
	}
		// find zone
		// if zone has available space
			// create allocation header
			// return pointer to allocation
	// create zone
	// append zone (to tail by find zone?)
	// create allocation header
	// return pointer to alocation
}

void	*malloc(size_t size) {
	// alloc size == tiny small or large

}