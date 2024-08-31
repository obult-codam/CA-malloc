#include "../src/large.c"
#include "otest.h"

#define BLOCK 256

int main(void)
{
    void *head;
    char chead[BLOCK];
    head = (void *)chead;
    struct s_large_header *header = head;

    setup_large(head, BLOCK);
    TEST(header->end == BLOCK);

    void *alloc;
    alloc = create_large_alloc(head, BLOCK - sizeof(struct s_large_header));
    TEST(alloc != NULL);
    ((char *)alloc)[BLOCK -1 - sizeof(struct s_large_header)] = '0';

    TEST(not_this_large_alloc(head, alloc + 1));
    TEST(!not_this_large_alloc(head, alloc));

    void *prev = alloc;
    alloc = resize_large_alloc(head, alloc, BLOCK / 2);
    TEST(alloc == prev);
    TEST(header->size == BLOCK / 2);

    alloc = create_large_alloc(header, BLOCK);
    TEST(alloc == NULL);

    alloc = create_large_alloc(header, 42);
    size_t size = get_large_alloc_size(head, alloc);
    TEST(size == 42);

    fprintf(stderr, "\n");
}