#include <stddef.h>
#include <libft.h>
#include "ft_malloc.h"

struct s_ll_zone_header {
    size_t total_size;
    t_list *head;
};

struct s_alloc_header {
    t_list *next;
    size_t size;
};

#define SIZE_ALLOC_HEADER sizeof(struct s_alloc_header)

/**
 * Structure of the zone is:
 * zone_header
 * alloc->alloc->alloc
 *
 * Where each alloc contians a linked-list item and a size (size_t)
 * which will contain the size of the actual alloced size by the user.
 *
 * The pointer of the linked-list is NULL
 * or the next link containing an alloc.
*/

/**
 * \brief Call to find required size for x times the allocation under a strategy including its headers.
*/
size_t calculate_required_size(size_t alloc_size, size_t amount)
{
    return (
        (alloc_size + sizeof(t_list) + SIZE_ALLOC_HEADER) * amount
        + sizeof(struct s_ll_zone_header)
    );
}

/**
 * \brief Setup a new zone for allocations upto \param max_alloc_size
 * and
*/
void setup_zone(void *head, size_t max_alloc_size, size_t total_size)
{
    (void)max_alloc_size;   // Not required for this strategy.
    struct s_ll_zone_header *header = head;
    header->total_size = total_size;
}

/* ----- Create alloc ----- */

// Check create insert_alloc
static void *add_alloc(t_list **list, size_t size)
{


    return NULL;
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
    struct s_ll_zone_header *header = head;

    // check if the next node is NULL add it there if it is (first node).
    if (header->head == NULL) {
        header->head = &header[1];
        return add_alloc(&header->head, size);
    }

    // check if there is a next node and  while there is evaluate if there is room inbetween.
    t_list *tmp;
    for (tmp = header->head; tmp != NULL; tmp = tmp->next) {
        // if last node check if there is enough room till the end according to header(-total_size).
        if (tmp->next == NULL) {
            break ;
        }
    }
    // check if there is enough room till the end according to header(-total_size).

}

/**
 * \brief Release an alloc in this zone.
*/
void cleanup_alloc(void *head, void *ptr)
{
    struct s_ll_zone_header *header = head;
    t_list *prev = header->head;
    for (t_list *tmp = header->head; tmp != NULL; tmp = tmp->next) {
        if (tmp->content == ptr) {
            if (prev == header->head)
                prev = tmp->next;
            else
                prev->next = tmp->next;

            return;
        }
        prev = tmp;
    }
}

/**
 * \brief Check if a zone is empty.
*/
bool zone_is_empty(void *head)
{
    return (((struct s_ll_zone_header *)head)->head == NULL);
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
    return NULL;
}

/**
 * \brief Prints out information about the allocations in this zone.
*/
void print_info(void *head)
{

}

/**
 * \brief Returns the size of the alloc header used for calculations in zone creation.
*/
size_t alloc_header_size()
{
    return 0;
}

/**
 * TODO: Look into adding a single_alloc_zone creator Fn.
*/

/**
 * Because of the 42 print function requirement we have to keep track of exact alloc sizes.
 * This could be leveraged to create a function which can return how many bytes are alloced for a pointer.
*/