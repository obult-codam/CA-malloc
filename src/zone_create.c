#include "ft_malloc.h"

// size only used on LARGE objects
size_t	calculate_zone_size(t_zone_type z_type, size_t size) {
	size_t	zone_sizes[2] = { TINY_SIZE, SMALL_SIZE };
	size_t	zone_size;
	int		page_size = getpagesize();

	if (z_type == LARGE)
		zone_size = ZONE_RESERVED_SIZE + calculate_required_size(size, 1);
	else
		zone_size = ZONE_RESERVED_SIZE + calculate_required_size(zone_sizes[z_type], MIN_ZONE_ALLOC);

	// add space for t_list and zone_header
	zone_size += sizeof(t_zone_header) + sizeof(t_list);

	// rounded up to multiple of getpagesize();
	zone_size = ((zone_size / page_size) + 1) * page_size;
	return zone_size;
}

size_t alloc_size_category(size_t size)
{
	if (size <= TINY_SIZE)
		return TINY_SIZE;
	else if (size <= SMALL_SIZE)
		return SMALL_SIZE;
	else
		return size;
}

// create a zone which is requested by provide_zone
t_list	*create_zone(t_zone_type z_type, size_t alloc_size) {
	size_t	length;
	void	*mapped;

	// calculate the length
	length = calculate_zone_size(z_type, alloc_size);

	mapped = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//  check for failures
	if (mapped == MAP_FAILED)
		return NULL;

	// add a list object at the top?
	t_list *list_header = (t_list *) mapped;
	list_header->next = NULL;
	// why use this as offset? then zone_header comes after list_header part
	list_header->content = (void *)list_header + sizeof(*list_header);

	// initialize the mapped page with headers
	t_zone_header	*header = (t_zone_header *)list_header->content;
	header->alloc_head = NULL;
	header->zone_size = length;
	header->zone_type = z_type;

	// return the newly created zone (list item)
	return (list_header);
}