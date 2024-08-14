#ifndef ALLOC_INTERNAL_H
# define ALLOC_INTERNAL_H

# include "ft_malloc.h"

typedef struct s_alloc_header {
	size_t	size;
}	t_alloc_header;

struct s_head_info {
	size_t zone_size;
	t_list *list_head;
};

#define ALLOC_HEADER_SIZE sizeof(t_zone_header)

// 		# ll alloc management
t_list	**ll_find_alloc(void *ptr, t_list *l_zone);
bool	ll_realloc_has_space(t_list *node, size_t size);

#endif