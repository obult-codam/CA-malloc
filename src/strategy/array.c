#include <new_api.h>
#include <stdint.h>
#include <stdio.h>

struct s_array_header {
	size_t alloc_amount;
	size_t max_size;
	uint16_t *size_in_use;	// array containing the size of the allocs
	void *allocs;	// array containing the allocs
};

/**
 * \brief Calculate the full size strategy needs for internal storage of zone and headers.
*/
size_t calculate_required_size(size_t alloc_size, size_t amount)
{
	return sizeof(struct s_array_header) + (alloc_size + sizeof(uint16_t)) * amount;
}

/**
 * \brief Setup a new zone for allocations upto \param max_alloc_size
 * and
*/
void setup_zone(void *head, size_t max_alloc_size, size_t total_size)
{
	struct s_array_header *header = head;
	header->alloc_amount = (total_size - sizeof(struct s_array_header)) / (max_alloc_size + sizeof(*(header->size_in_use)));

	header->max_size = max_alloc_size;
	header->size_in_use = (void *)(&header[1]);
	header->allocs = (void *)(&header->size_in_use[header->alloc_amount]);
	if (max_alloc_size < 4096)
		header->allocs += (max_alloc_size - ((size_t)header->allocs % max_alloc_size)) % max_alloc_size;

	for (size_t i = 0; i < header->alloc_amount; i++) {
		header->size_in_use[i] = 0;
	}
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
	struct s_array_header *header = head;

	for (u_int32_t i = 0; i < header->alloc_amount; i++) {
		if (header->size_in_use[i] != 0)
			continue;

		header->size_in_use[i] = size;
		return header->allocs + (i * header->max_size);
	}
	return NULL;
}

/**
 * \brief Release an alloc in this zone.
 *
 * \return true when the zone is empty
*/
void cleanup_alloc(void *head, void *ptr)
{
	struct s_array_header *header = head;

	if (ptr == NULL)
		return;

	uint32_t nth_alloc = (ptr - header->allocs) / header->max_size;
	header->size_in_use[nth_alloc] = 0;
}

/**
 * \brief Check if a zone is empty.
*/
bool zone_is_empty(void *head)
{
	struct s_array_header *header = head;

	for (uint32_t i = 0; i < header->alloc_amount; i++) {
		if (header->size_in_use[i] != 0)
			return false;
	}
	return true;
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
	struct s_array_header *header = head;
	if (size > header->max_size)
		return NULL;

	uint32_t nth_alloc = (ptr - header->allocs) / header->max_size;

	header->size_in_use[nth_alloc] = size;
	return ptr;
}

/**
 * \brief Prints out information about the allocations in this zone.
*/
size_t print_info(void *head)
{
	struct s_array_header *header = head;
	size_t total_size = 0;

	for (uint32_t i = 0; i < header->alloc_amount; i++) {
		if (header->size_in_use[i] == 0)
			continue;

		void *start = header->allocs + (header->max_size * i);
		fprintf(stderr, "%p - %p : %u bytes\n", start, start + header->size_in_use[i], header->size_in_use[i]);
		total_size += header->size_in_use[i];
	}
	return total_size;
}

/* This show an allocations spread, not a hex dump! */
void print_debug(void *head)
{
	struct s_array_header *header = head;
	uint32_t size;

	for (uint32_t i = 0; i < header->alloc_amount; i++) {
		if (i % 16 == 0)
			fprintf(stderr, "\n");

		size = header->size_in_use[i];
		if (size == 0)
			fprintf(stderr, "[\033[36m%.2x\033[0m]  ", size);
		else
			fprintf(stderr, "[\033[91m%.2x\033[0m]  ", size);
	}
}

/* needed for getting the size for a out of zone realloc (need to know how much to copy) */
size_t get_alloc_size(void *head, void *ptr)
{
	struct s_array_header *header = head;

	uint32_t nth_alloc = (ptr - header->allocs) / header->max_size;
	return header->size_in_use[nth_alloc];
}
