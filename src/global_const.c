#include "ft_malloc.h"

t_list	*g_head = NULL;
const size_t LL_NODE_SIZE = sizeof(t_list);
const size_t ZONE_HEADER_SIZE = sizeof(t_zone_header);
const size_t ALLOC_HEADER_SIZE = sizeof(t_alloc_header);
