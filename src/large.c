#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

struct s_large_header {
	size_t size;
	size_t end;
};

size_t calculate_large_size(size_t size)
{
	return size + sizeof(struct s_large_header);
}

void setup_large(void *head, size_t end)
{
	struct s_large_header *header = head;

	header->end = end;
}

bool not_this_large_alloc(void *head, void *ptr)
{
	struct s_large_header *header = head;

	return (void *)&header[1] != ptr;
}

void *create_large_alloc(void *head, size_t size)
{
	struct s_large_header *header = head;

	// Turned off for optimization
	// if (size > (header->end - sizeof(*header)))
	//	 return NULL;

	header->size = size;
	return (void *)&header[1];
}

void *resize_large_alloc(void *head, void *ptr, size_t size)
{
	struct s_large_header *header = head;

	if (ptr != (void *)&header[1])
		return NULL;

	if (size <= header->end - sizeof(*header)) {
		header->size = size;
		return ptr;
	}
	return NULL;
}

size_t printf_large_info(void *head)
{
	struct s_large_header *header = head;
	void *start = (void *)&header[1];

	fprintf(stderr, "%p - %p : %lu bytes\n", start, start + header->size, header->size);
	return header->size;
}

size_t get_large_alloc_size(void *head, void *ptr)
{
	struct s_large_header *header = head;

	if (ptr != (void *)&header[1])
		return 0;

	return header->size;
}
