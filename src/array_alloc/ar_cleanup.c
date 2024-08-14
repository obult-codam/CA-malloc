#include "ft_malloc.h"
#include "array_strategy.h"

/**
 * \brief Clean up the array alloc.
*/
void	cleanup_alloc_strategy(void *ptr, t_list *l_zone)
{
    t_array_header *array_header =  ((t_zone_header *)l_zone->content)->alloc_head;

    uint8_t *alloc_size = ar_ptr_to_size(array_header, ptr);

    // can use an if statement here to check if it is already 0 and call double free() if it is.

    *alloc_size = 0;
}
