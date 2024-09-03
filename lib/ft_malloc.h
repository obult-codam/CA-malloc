#ifndef FT_MALLOC
# define FT_MALLOC

# include <stddef.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>
#include "new_api.h"

# include "libft.h"

# define TINY_SIZE 64
# define SMALL_SIZE 512
# define MIN_ZONE_ALLOC 8

// ### Public API ###
void	show_alloc_mem();
void	show_alloc_mem_ex();
void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif