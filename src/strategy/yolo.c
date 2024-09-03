#include <new_api.h>
#include <stdint.h>
#include <stdio.h>

/**
 * The most space and time efficient strategy,
 * until it isn't.
 */

struct s_yolo_header {
	size_t total_size; // offset from *head(er)
	size_t alloc_count;
	void *next_alloc;
};

/**
 * \brief Calculate the full size strategy needs for internal storage of zone and headers.
*/
size_t calculate_required_size(size_t alloc_size, size_t amount)
{
	return sizeof(struct s_yolo_header) + alloc_size * amount;
}

/**
 * \brief Setup a new zone for allocations upto \param max_alloc_size
 * and
*/
void setup_zone(void *head, size_t max_alloc_size, size_t total_size)
{
	struct s_yolo_header *header = head;
	(void)max_alloc_size;

	header->alloc_count = 0;
	header->next_alloc = (void *)(&header[1]);
	header->total_size = total_size;
}

/**
 * \brief Request a new allocation in a zone.
 *
 * \param head Start of alloc reserved memory of this zone.
 * \param size The size this alloc requires.
 *
 * \returns NULL on not enough space in zone.
*/
void *create_alloc(void *head, size_t size)
{
	struct s_yolo_header *header = head;
	void *ptr;

	if ((header->next_alloc + size) > (head + header->total_size))
		return NULL;

	header->alloc_count++;
	ptr = header->next_alloc;
	header->next_alloc += size;
	return ptr;
}

/**
 * \brief Release an alloc in this zone.
 *
 * \return true when the zone is empty
*/
void cleanup_alloc(void *head, void *ptr)
{
	struct s_yolo_header *header = head;

	if (ptr > head + header->total_size || ptr < head)
		return;

	header->alloc_count--;
}

/**
 * \brief Check if a zone is empty.
*/
bool zone_is_empty(void *head)
{
	struct s_yolo_header *header = head;
	return header->alloc_count == 0;
}

/**
 * \brief Attempt to resize the allocation inside the zone.
 *
 * \param head Points to start of alloc reserved memory in zone.
 * \param ptr Pointer of memory part to resize.
 * \param size Size required.
 *
 * \return NULL when failed to resize within the zone.
 * On succes the new or original pointer is returned.
*/
void *resize_alloc(void *head, void *ptr, size_t size)
{
	(void)head;
	(void)ptr;
	(void)size;
	return NULL;
}

/**
 * \brief Prints out information about the allocations in this zone.
*/
size_t print_info(void *head)
{
	struct s_yolo_header *header = head;
	size_t size_used = (size_t)header->next_alloc - (size_t)&header[1];

	printf("YOLO -> alloc count: %lu, combined size : %lu\n", header->alloc_count, size_used);
	return size_used;
}

/* Do a hex dump instead of allocation spread.*/
void print_debug(void *head)
{
	struct s_yolo_header *header = head;
	size_t size_used = (size_t)header->next_alloc - (size_t)&header[1];
	void *base = (void *)&header[1];
	size_t dump;

	for (size_t i = 0; i < size_used / 8; i++)
	{
		if (i % 8 == 0)
			fprintf(stderr, "\n");

		dump = *(size_t *)&base[i * 8];

		if (dump)
			fprintf(stderr, "[\033[91m%.8zx\033[0m]  ", dump);
		else
			fprintf(stderr, "[\033[36m%.8zx\033[0m]  ", dump);
	}
}

/**
 * TODO: Look into adding a single_alloc_zone creator Fn.
*/

/* needed for getting the size for a out of zone realloc (need to know how much to copy) */
size_t get_alloc_size(void *head, void *ptr)
{
	(void)head;
	(void)ptr;
	return 0;
}

/**
 * Because of the 42 print function requirement we have to keep track of exact alloc sizes.
 * This could be leveraged to create a function which can return how many bytes are alloced for a pointer.
*/