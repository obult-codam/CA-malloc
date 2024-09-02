#pragma once

#include <stddef.h>
#include <stdbool.h>

/**
 * \brief Calculate the full size strategy needs for internal storage of zone and headers.
*/
size_t calculate_required_size(size_t alloc_size, size_t amount);

/**
 * \brief Setup a new zone for allocations upto \param max_alloc_size
 * and
*/
void setup_zone(void *head, size_t max_alloc_size, size_t total_size);

/**
 * \brief Request a new allocation in a zone.
 *
 * \param head Start of alloc reserved memory of this zone.
 * \param size The size this alloc requires.
 *
 * \returns NULL on not enough space in zone.
*/
void *create_alloc(void *head, size_t size);

/**
 * \brief Release an alloc in this zone.
 *
 * \return true when the zone is empty
*/
void cleanup_alloc(void *head, void *ptr);

/**
 * \brief Check if a zone is empty.
*/
bool zone_is_empty(void *head);

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
void *resize_alloc(void *head, void *ptr, size_t size);

/**
 * \brief Prints out information about the allocations in this zone.
*/
size_t print_info(void *head);
void print_debug(void *head);

/**
 * TODO: Look into adding a single_alloc_zone creator Fn.
*/

/* needed for getting the size for a out of zone realloc (need to know how much to copy) */
size_t get_alloc_size(void *head, void *ptr);

/**
 * Because of the 42 print function requirement we have to keep track of exact alloc sizes.
 * This could be leveraged to create a function which can return how many bytes are alloced for a pointer.
*/