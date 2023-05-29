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
	void		*alloc_head;
	size_t		zone_size;
	t_zone_type	zone_type;
}	t_zone_header;

typedef struct s_alloc_header {
	size_t	size;
}	t_alloc_header;

// ### INTERNAL ###
// 		# zone management
void	cleanup_zone_check(t_list **pl_zone);
t_list	*create_zone(t_zone_type z_type, size_t alloc_size);
t_list	*find_zone_by_type(t_list *start, t_zone_type z_type);
t_list	**find_zone_pl(void *ptr);

// 		# reporting
void	double_free();
void	pointer_not_allocated();


// 		# ll alloc management
t_list	**ll_find_alloc(void *ptr, t_list *l_zone);
void	*ll_create_alloc(t_zone_header *zone, size_t size);
void	ll_cleanup_alloc(void *ptr, t_list *l_zone);
bool	ll_realloc_has_space(t_list *node, size_t size);
void	*realloc_strategy(void *ptr, t_list *l_zone, size_t size);


// ### Public API ###
void	show_alloc_mem();
void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif