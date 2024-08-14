#include "array_strategy.h"
#include "ft_malloc.h"

/**
 * \brief In zone realloc.
 * It can be assumed that the realloc fits inside the spot it is currently in.
*/
void	*realloc_strategy(void *ptr, t_list *l_zone, size_t size)
{
    t_zone_header *zone = (t_zone_header *)l_zone->content;
    t_array_header *array_header = zone->alloc_head;

    uint8_t *alloc_size = ar_ptr_to_size(array_header, ptr);
    
    *alloc_size = size;
    return ptr;
}