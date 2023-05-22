#ifndef FT_MALLOC
# define FT_MALLOC

#pragma once

# include <stddef.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>

# include "libft.h"

# define TINY_SIZE 64
# define SMALL_SIZE 512
# define MIN_ZONE_ALLOC 100

extern t_list *g_head;
extern const size_t LL_NODE_SIZE;
extern const size_t ZONE_HEADER_SIZE;
extern const size_t ALLOC_HEADER_SIZE;

typedef enum e_zone_type {
	TINY,
	SMALL,
	LARGE,
}	t_zone_type;

typedef struct s_zone_header {
	t_list		*alloc_head;
	size_t		zone_size;
	t_zone_type	zone_type;
}	t_zone_header;

typedef struct s_alloc_header {
	size_t	size;
}	t_alloc_header;

// Public malloc API
void	show_alloc_mem();
void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif