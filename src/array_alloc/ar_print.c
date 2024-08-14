#include "array_strategy.h"
#include <stdio.h>

void	print_zone_info(void *alloc_head) // add size category string as argument??
{
    t_array_header *ar_header = alloc_head;

    for (uint32_t i = 0; i < ar_header->alloc_amount; i++) {
        if (ar_header->size_in_use[i] != 0) {
            // printf("%p - %p : %lu bytes\n", ar_header->allocs + i * , ar_header->allocs[i] + ar_header->size_in_use[i], ar_header->size_in_use[i]);
            // can do this later lol..
        }
    }
}
