#ifndef ARRAY_STRATEGY_H
# define ARRAY_STRATEGY_H

# include "ft_malloc.h"
# include <stdint.h>

typedef struct s_array_header {
	size_t	alloc_amount;
	uint8_t	*size_in_use;	// array containing the size of the alloc
	void	*allocs;		// array containing the alloc
} t_array_header;

#define ALLOC_HEADER_SIZE sizeof(t_zone_header) // is this right?
// should this not be sizeof(t_array_header) instead?

/**
 * Utils.
*/
uint8_t *ar_ptr_to_size(t_array_header *array_header, void *ptr);

#endif