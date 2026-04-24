#ifndef CUSTOM_ALLOCATION_H
#define CUSTOM_ALLOCATION_H

#include <stdint.h>
#include <stddef.h>

void my_alloc_init(void);

void *my_malloc(size_t size);
void *my_calloc(size_t n, size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);

#endif // CUSTOM_ALLOCATION_H