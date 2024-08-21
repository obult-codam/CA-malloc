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

extern t_list *g_head;

typedef enum e_zone_type {
	TINY = 0,
	SMALL,
	LARGE,
}	t_zone_type;

typedef struct s_zone_header {
	void		*alloc_head;
	size_t		zone_size;
	t_zone_type	zone_type;
}	t_zone_header;

#define LL_NODE_SIZE sizeof(t_list)
#define ZONE_HEADER_SIZE sizeof(t_zone_header)
#define ZONE_RESERVED_SIZE ZONE_HEADER_SIZE + LL_NODE_SIZE

#define PTR_AFTER(object) ((void *)object + sizeof(*object))


// ### INTERNAL ###
// 		# zone management
void	cleanup_zone_check(t_list **pl_zone);
t_list	*create_zone(t_zone_type z_type, size_t alloc_size);
t_list	*find_zone_by_type(t_list *start, t_zone_type z_type);
t_list	**find_zone_pl(void *ptr);
t_zone_type	zone_is_type(size_t size);
size_t	alloc_size_category(size_t size);

// 		# extra sauce
void	*out_of_zone_realloc(void *ptr, size_t prev_size, size_t new_size);

// 		# reporting
void	double_free(); // move internally in file.
void	pointer_not_allocated(void *ptr); // move internally in file.

// 		# Strategy API
void	*create_alloc_strategy(t_zone_header *zone, size_t size);
void	cleanup_alloc_strategy(void *ptr, t_list *l_zone);
void	*realloc_strategy(void *ptr, t_list *l_zone, size_t size);
void	print_zone_info(void *alloc_head);
size_t	alloc_header_size();

// ### Public API ###
void	show_alloc_mem();
void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif