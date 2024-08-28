#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <new_api.h>

#define CELL_SIZE 128
#define MULTI_CELL 0XFF
#define MAX(a, b) (a < b ? b : a)

struct s_mcell_header {
    size_t  alloc_amount;
    size_t  total_cells;
    size_t  max_size;
    uint8_t *size_in_use;
    void *allocs;
};

/**
 * \brief Calculate the full size strategy needs for internal storage of zone and headers.
*/
size_t calculate_required_size(size_t alloc_size, size_t amount)
{
    uint32_t max_cells = alloc_size / CELL_SIZE + 1;
    return sizeof(struct s_mcell_header) + (((CELL_SIZE + sizeof(uint8_t)) * max_cells) * amount);
}

/**
 * \brief Setup a new zone for allocations upto \param max_alloc_size
 * and
*/
void setup_zone(void *head, size_t max_alloc_size, size_t total_size)
{
    struct s_mcell_header *header = head;
    uint32_t max_cells = max_alloc_size / CELL_SIZE + (max_alloc_size % CELL_SIZE ? 1 : 0);
    uint32_t cif = (max_cells * sizeof(*header->size_in_use)); // storage for size data per (max) alloc.

    header->alloc_amount = (total_size - sizeof(*header)) / (MAX(max_alloc_size + cif, CELL_SIZE + cif));
    header->max_size = max_alloc_size;
    header->total_cells = (total_size - sizeof(*header)) / (CELL_SIZE + sizeof(uint8_t));
    header->size_in_use = (void *)(&header[1]);
    header->allocs = (void *)(&header->size_in_use[header->total_cells]);
    header->allocs += (CELL_SIZE - ((size_t)header->allocs % CELL_SIZE)) % CELL_SIZE;

    // probably redundant because of empy pages.
    for (size_t i =0; i < header->total_cells; i++) {
        header->size_in_use[i] = 0; // could be more efficient. (good memset or set per size_t block)
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

static size_t next_used_cell(uint8_t *size_in_use, size_t cells_left, size_t cells_required)
{
    size_t i = 0;

    if (cells_required > cells_left)
        return cells_required;

    for (; i < cells_required; i++) {
        if (size_in_use[i] != 0)
            return i;
    }
    return 0;
}

static uint8_t calc_cell_size(size_t size)
{
    return size % CELL_SIZE ? size : CELL_SIZE;
}

void *create_alloc(void *head, size_t size)
{
    struct s_mcell_header *header = head;
    uint32_t cell_amount = size / CELL_SIZE;
    size_t unavailable_cells;
    size_t i;

    if (size % CELL_SIZE)
        cell_amount += 1;

    for (i = 0; i < header->total_cells; i++) {
        if (header->size_in_use[i] != 0)
            continue;
       unavailable_cells = next_used_cell(&header->size_in_use[i], header->total_cells - i, cell_amount);
        if (unavailable_cells) {
            i += unavailable_cells;
            continue;
        }
        for (uint32_t j = 0; j + 1 < cell_amount; j++) {
            header->size_in_use[i + j] = MULTI_CELL;
        }
        header->size_in_use[cell_amount + i - 1] = calc_cell_size(size);

        return header->allocs + (i * CELL_SIZE);
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
    struct s_mcell_header *header = head;

    if (ptr == NULL)
        return;

    uint32_t nth_cell = (ptr - header->allocs) / CELL_SIZE;
    while (header->size_in_use[nth_cell] == MULTI_CELL)
    {
        header->size_in_use[nth_cell] = 0;
        nth_cell++;
    }
    header->size_in_use[nth_cell] = 0;
}

/**
 * \brief Check if a zone is empty.
*/
bool zone_is_empty(void *head)
{
    struct s_mcell_header *header = head;

    for (size_t i = 0; i < header->total_cells; i++) {
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
    struct s_mcell_header *header = head;
    uint32_t cell_amount = size / CELL_SIZE + 1;

    if (ptr == NULL)
        return NULL;
    if (size > header->max_size)
        return NULL;

    size_t current_size = get_alloc_size(head, ptr);
    uint32_t current_cell_amount = current_size / CELL_SIZE + (current_size % CELL_SIZE ? 1 : 0);
    uint32_t nth_cell = (ptr - header->allocs) / CELL_SIZE;

    if (cell_amount == current_cell_amount)
        return ptr;
    if (cell_amount < current_cell_amount) {
        header->size_in_use[nth_cell + cell_amount - 1] = calc_cell_size(size);
        for (uint32_t i = 0; i < current_cell_amount - cell_amount; i++) {
            header->size_in_use[nth_cell + cell_amount + i - 1] = 0;
        }
        return ptr;
    }

    // the required space is bigger
    if (next_used_cell(&header->size_in_use[nth_cell + current_cell_amount], header->total_cells - nth_cell - current_cell_amount, cell_amount - current_cell_amount))
        return NULL;

    for (uint32_t i = 0; i < cell_amount - current_cell_amount; i++) {
        header->size_in_use[nth_cell] = MULTI_CELL;
        nth_cell++;
    }
    header->size_in_use[nth_cell] = calc_cell_size(size);
    return ptr;
}

/**
 * \brief Prints out information about the allocations in this zone.
*/
void print_info(void *head)
{
    struct s_mcell_header *header = head;

    for (size_t i = 0; i < header->total_cells; i++) {
        if (header->size_in_use[i] == 0)
            continue;

        void *start = header->allocs + CELL_SIZE * i;
        size_t size = get_alloc_size(head, start);
        i += size / CELL_SIZE + (size % CELL_SIZE ? 1 : 0);
        fprintf(stderr, "%p - %p : %lu bytes\n", start, start + size, size);
    }
}

/**
 * Hex dump
 */
void print_debug(void *head)
{
    struct s_mcell_header *header = head;

    for (size_t i = 0; i < header->total_cells; i++) {
        if (i % 8 == 0)
            fprintf(stderr, "\n");

        if (header->size_in_use[i] == MULTI_CELL)
            fprintf(stderr, "[\033[31m%.2x\033[0m]  ", header->size_in_use[i]);
        else if (header->size_in_use[i] != 0)
            fprintf(stderr, "[\033[91m%.2x\033[0m]  ", header->size_in_use[i]);
        else
            fprintf(stderr, "[\033[36m%.2x\033[0m]  ", header->size_in_use[i]);
    }
    fprintf(stderr, "\n");
}

/**
 * TODO: Look into adding a single_alloc_zone creator Fn.
*/

/* needed for getting the size for a out of zone realloc (need to know how much to copy) */
size_t get_alloc_size(void *head, void *ptr)
{
    struct s_mcell_header *header = head;
    size_t size = 0;

    uint32_t nth_cell = (ptr - header->allocs) / CELL_SIZE;

    while(header->size_in_use[nth_cell] == MULTI_CELL) {
        size += CELL_SIZE;
        nth_cell++;
    }
    return size + header->size_in_use[nth_cell];
}
