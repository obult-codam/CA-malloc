#include "ft_malloc.h"
#include "new_api.h"
#include "large_alloc.h"
#include <stdio.h>

/**
 * This is just as the regular core but uses "Thread Local storage"
 * to make the malloc thread safe.
 *
 * This way of making malloc thread safe will require the programmer to free in
 * the same thread as the object is allocated.
 *
 * This is faster than using a mutex.
 */

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
#define ZONE_REST_SIZE (LL_NODE_SIZE + ZONE_HEADER_SIZE)
#define MIN(a, b) (a < b ? a : b)

/**
 * Static functions
 */
static t_list **find_zone_pl(void *ptr);
static t_list *find_zone_by_type(t_list *start, t_zone_type z_type);
static t_list *create_zone(t_zone_type z_type, size_t alloc_size);
static size_t alloc_size_category(size_t size);

/* Global starting point for mem map */
/* This global is unique for every thread
 * Each thread will have its own set of memory zones.
 */
__thread t_list *g_head;

/**
 * FREE
 */

static void cleanup_zone(t_list **pl_zone, size_t size) {
	void *to_unmap = (void *)*pl_zone;

	// remove the zone
	*pl_zone = (*pl_zone)->next;

	munmap(to_unmap, size);
}

static bool check_last_of_type(t_zone_type type)
{
	t_list			*tmp = g_head;
	t_zone_header	*zone;
	int 			count = 0;

	while (tmp != NULL) {
		zone = (t_zone_header *)tmp->content;
		if (zone->zone_type == type) {
			count++;
			if (count == 2)
				return false;
		}
		tmp = tmp->next;
	}
	return true;
}

// implemented for Linked-list strategy only
void free(void *ptr)
{
	if (ptr == NULL)
		return ;

	t_list	**pl_zone = find_zone_pl(ptr);
	if (pl_zone == NULL || *pl_zone == NULL) {
		pointer_not_allocated(ptr);
		return;
	}

	t_zone_header *zone_header = (*pl_zone)->content;
	void *head = zone_header->alloc_head;

	// call the cleanup strategy
	if (zone_header->zone_type == LARGE) {
		if (not_this_large_alloc(zone_header->alloc_head, ptr))
			return;

		cleanup_zone(pl_zone, zone_header->zone_size);
		return;
	}

	cleanup_alloc(head, ptr);

	/* Check that the zone is still in use. */
	if (!zone_is_empty(head))
		return;

	if (!check_last_of_type(zone_header->zone_type))
	{
		cleanup_zone(pl_zone, zone_header->zone_size);
	}
}

/**
 * MALLOC
 */

static t_zone_type	zone_is_type(size_t size)
{
	if (size <= TINY_SIZE)
		return (TINY);
	else if (size <= SMALL_SIZE)
		return (SMALL);
	return (LARGE);
}

static void *handle_large_zone(size_t size)
{
	t_list	*l_new_zone = create_zone(LARGE, size);
	if (l_new_zone == NULL) {
		return (NULL); // LCOV_EXCL_LINE
	}

	// append zone to tail
	ft_lstadd_back(&g_head, l_new_zone);

	t_zone_header *zone_header = l_new_zone->content;
	zone_header->alloc_head = &zone_header[1];
	void *head = zone_header->alloc_head;

	// setup_zone (from api)
	setup_large(head, zone_header->zone_size - ZONE_REST_SIZE);
	return create_large_alloc(zone_header->alloc_head, size);
}

void *malloc(size_t size)
{
	t_list		*l_tmp;
	t_zone_type	z_type = zone_is_type(size);


	// create zone directly and append to tail for LARGE zone
	if (z_type == LARGE)
		return handle_large_zone(size);

	// check if there is room free in an existing zone
	l_tmp = find_zone_by_type(g_head, z_type);
	while (l_tmp != NULL) {
		void *alloc = create_alloc(((t_zone_header *)l_tmp->content)->alloc_head, size);
		if (alloc != NULL) {
			return alloc;
		}
		l_tmp = find_zone_by_type(l_tmp->next, z_type);
	}
	// create zone
	// size not used for non-LARGE zones
	// large zones are always created here
	t_list	*l_new_zone = create_zone(z_type, size);
	if (l_new_zone == NULL) {
		return (NULL); // LCOV_EXCL_LINE
	}

	// append zone to tail
	ft_lstadd_back(&g_head, l_new_zone);

	t_zone_header *zone_header = l_new_zone->content;
	zone_header->alloc_head = &zone_header[1];
	void *head = zone_header->alloc_head;

	// setup_zone (from api)
	setup_zone(head, alloc_size_category(size), zone_header->zone_size - ZONE_REST_SIZE);

	// create_alloc (from api)
	void *alloc = create_alloc(head, size);

	return (alloc);
}

/**
 * REALLOC
 */

static void *out_of_zone_realloc(void *ptr, size_t prev_size, size_t new_size)
{
	void	*new_alloc = malloc(new_size);
	ft_memcpy(new_alloc, ptr, MIN(prev_size, new_size));
	free(ptr);
	return (new_alloc);
}

void *realloc(void *ptr, size_t size)
{
	if (ptr == NULL)
		return (NULL);

	t_list	**pl_zone = find_zone_pl(ptr);

	if (pl_zone == NULL) {
		pointer_not_allocated(ptr);
		return NULL;
	}

	/**
	 * Call the realloc zone management api.
	*/
	void *realloc;
	t_zone_header *zone_header = (*pl_zone)->content;
	void *head = zone_header->alloc_head;
	if (zone_header->zone_type == zone_is_type(size))
	{
		if (zone_header->zone_type == LARGE)
			realloc = resize_large_alloc(head, ptr, size);
		else
			realloc = resize_alloc(head, ptr, size);
		if (realloc != NULL)
			return realloc;
	}

	uint32_t old_size;
	if (zone_header->zone_type == LARGE)
		old_size = get_large_alloc_size(head, ptr);
	else
		old_size = get_alloc_size(head, ptr);
	if (old_size == 0)
		return NULL; // allows for some exotic variants.

	return (out_of_zone_realloc(ptr, old_size, size));
}

/**
 * REPORTING
 */
void double_free(void *address)
{
	fprintf(stderr, "double free at : %p\n", address);
}

void pointer_not_allocated(void *ptr)
{
	fprintf(stderr, "pointer_not_allocated: %p\n", ptr);
}

/**
 * SHOW_ALLOC_MEM
 */

void print_zone_size(t_zone_header *zone)
{
	char *zones_as_string[] = { "TINY: ", "SMALL:", "LARGE:" };
	fprintf(stderr, "%s : %p size: %lu\n", zones_as_string[zone->zone_type], zone - LL_NODE_SIZE, zone->zone_size);
}

void show_alloc_mem()
{
	t_list *l_zone = g_head;
	size_t total_size = 0;

	fprintf(stderr, "----------------\n");

	while (l_zone) {
		t_zone_header *zone = (t_zone_header *)l_zone->content;

		// print zone info (type + start location)
		print_zone_size(zone);

		// Strategy dependent
		if (zone->zone_type == LARGE)
			total_size += printf_large_info(zone->alloc_head);
		else
			total_size += print_info(zone->alloc_head);

		l_zone = l_zone->next;
	}
	fprintf(stderr, "Total : %lu bytes\n", total_size);
}

// size only used on LARGE objects
static size_t calculate_zone_size(t_zone_type z_type, size_t size)
{
	size_t	zone_sizes[2] = { TINY_SIZE, SMALL_SIZE };
	size_t	zone_size;
	int		page_size = getpagesize();

	if (z_type == LARGE)
		zone_size = ZONE_RESERVED_SIZE + calculate_large_size(size);
	else
		zone_size = ZONE_RESERVED_SIZE + calculate_required_size(zone_sizes[z_type], MIN_ZONE_ALLOC);

	// add space for t_list and zone_header
	zone_size += sizeof(t_zone_header) + sizeof(t_list);

	// rounded up to multiple of getpagesize();
	zone_size = ((zone_size / page_size) + 1) * page_size;
	return zone_size;
}

static size_t alloc_size_category(size_t size)
{
	if (size <= TINY_SIZE)
		return TINY_SIZE;
	else if (size <= SMALL_SIZE)
		return SMALL_SIZE;
	else
		return size;
}

// create a zone which is requested by provide_zone
static t_list *create_zone(t_zone_type z_type, size_t alloc_size)
{
	size_t	length;
	void	*mapped;

	// calculate the length
	length = calculate_zone_size(z_type, alloc_size);

	mapped = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//  check for failures
	if (mapped == MAP_FAILED)
		return NULL; // LCOV_EXCL_LINE

	// add a list object at the top?
	t_list *list_header = (t_list *) mapped;
	list_header->next = NULL;
	// why use this as offset? then zone_header comes after list_header part
	list_header->content = (void *)list_header + sizeof(*list_header);

	// initialize the mapped page with headers
	t_zone_header	*header = (t_zone_header *)list_header->content;
	header->alloc_head = NULL;
	header->zone_size = length;
	header->zone_type = z_type;

	// return the newly created zone (list item)
	return (list_header);
}

static bool zone_is_tiny(void *param)
{
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == TINY);
}

static bool zone_is_small(void *param)
{
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == SMALL);
}

static bool zone_is_large(void *param)
{
	t_zone_header	*zone = (t_zone_header *)param;
	return (zone->zone_type == LARGE);
}

static bool is_inside_zone(t_list *l_zone, void *ptr)
{
	t_zone_header *zone = (t_zone_header *)l_zone->content;
	return (ptr >= (void *)l_zone && ptr < ((void *)l_zone + zone->zone_size));
}

// find an available zone for this alloc
static t_list *find_zone_by_type(t_list *start, t_zone_type z_type)
{
	bool (*zone_type_checks[3])(void *) = {
		zone_is_tiny,
		zone_is_small,
		zone_is_large,
	};
	return (ft_lstfind(start, zone_type_checks[z_type]));
}

static t_list **find_zone_pl(void *ptr)
{
	return ft_lstfind_pl(&g_head, ptr, is_inside_zone);
}
