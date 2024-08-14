#include "ft_malloc.h"
#include "array_strategy.h"

static void init_array_header(t_array_header *array_header, size_t alloc_amount)
{
	array_header->alloc_amount = alloc_amount;
	array_header->size_in_use = PTR_AFTER(array_header);
	array_header->allocs = array_header->size_in_use + alloc_amount; // Here the multiplying effect of + is correct.
}

void setup_zone_strategy(void *header, size_t object_size, size_t zone_size)
{

}

/**
 * \brief Check for and return open spot in the zone.
 * \return NULL when no available subzone is found.
*/
void	*create_alloc_strategy(t_zone_header *zone, size_t size)
{
	t_array_header *array_header = zone->alloc_head;

	// set up the zone if it is not used yet.
	if (array_header == NULL) {
		zone->alloc_head = PTR_AFTER(zone);

		// alloc amount is the amount one alloc will be?
		// this does not seem right.
		// size_t alloc_amount = (zone->zone_size) / (sizeof(*array_header->size_in_use) + ar_ptr_to_size());
		// it should be the zone size devided by the max size of an alloc plus the size of the header which is uint8_t
		size_t alloc_amount = (zone->zone_size) / (alloc_size_category(size) + sizeof(uint8_t));
		// if it can get here when it is a large alloc we have got a problem... (because it will create the size * amount of allocs instead of one block of size)

		init_array_header(&array_header, alloc_amount);
	}

	// find an open spot in the zone. (this seems right)
	uint32_t i;
	for (i = 0; i < array_header->alloc_amount; i++) {
		if (array_header->size_in_use[i] == 0)
		{
			array_header->size_in_use[i] = (uint8_t)size;
			return array_header->allocs + i * alloc_size_category(size);
		}
	}
	return (NULL);
}
