#ifndef FT_MALLOC
# define FT_MALLOC

# include <stddef.h>
# include <unistd.h>

# define TINY_SIZE 128;

void	*g_head = NULL;

enum e_zone_type {
	TINY,
	SMALL,
	LARGE,
};

typedef struct s_alloc_header {
	size_t	size;
	void	*next;
}	t_alloc_header;

typedef struct s_zone_header {
	void				*alloc_head;
	size_t				total_size;
	void				*next_zone;
	enum e_zone_type	zone_type;
}	t_zone_header;

// Public malloc API
void	show_alloc_mem();
void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif