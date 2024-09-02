#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

size_t calculate_large_size(size_t size);
void setup_large(void *head, size_t end);
bool not_this_large_alloc(void *head, void *ptr);
void *create_large_alloc(void *head, size_t size);
void *resize_large_alloc(void *head, void *ptr, size_t size);
size_t printf_large_info(void *head);
size_t get_large_alloc_size(void *head, void *ptr);
