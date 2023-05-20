#include "ft_malloc.h"

size_t	calculate_zone_size(t_zone_type z_type) {
	// return zone_size
	// 100+ * (z_type->max_size + sizeof(t_list)) + sizeof(t_zone_header) + sizeof(t_list)
	// rounded up to multiple of getpagesize();
}

// create a zone which is requested by provide_zone
void	*create_zone(t_zone_type z_type) {
	size_t	length;
	void	*mapped;

	// calculate the length
	length = calculatezone_size(z_type);

	mapped = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//  check for failures
	if (mapped == MAP_FAILED)
		return NULL;

	// initialize the mapped page with headers
	t_zone_header	*header;
	header = (t_zone_header *)mapped;
	header->alloc_head = NULL;
	header->zone_size = length;
	header->zone_type = z_type;

	// return the newly created zone
	return (mapped);
}

// find an available zone for this alloc
void	*find_zone();

// use find_zone to find a suitable zone else use create_zoen to create one,
// when that fails return NULL 
void	*provide_zone();

void	*malloc(size_t size) {
	// alloc size == tiny small or large

}