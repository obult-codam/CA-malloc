#include "array_strategy.h"

/**
 * \brief Translate the pointer to a pointer to the size of this alloc.
 * \return A poitner to the size of the alloc referenced by \param ptr.
*/
uint8_t *ar_ptr_to_size(t_array_header *array_header, void *ptr)
{
    return array_header->allocs - ptr + array_header->size_in_use;
}
