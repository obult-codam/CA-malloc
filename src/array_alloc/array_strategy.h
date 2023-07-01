#ifndef ARRAY_STRATEGY_H
# define ARRAY_STRATEGY_H

# include "ft_malloc.h"

typedef struct s_array_header {
	size_t	alloc_amount;
	uint8_t	*size_in_use;	// array containing the size of the alloc
	void	*allocs;		// array containing the alloc
};

// #define ALLOC_HEADER_SIZE sizeof(t_zone_header)

#endif