#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/kernel.h>

#include "custom_allocation.h"

#define APP_HEAP_SIZE 1024
//We can define as many heaps as we want
K_HEAP_DEFINE(app_heap, APP_HEAP_SIZE);

typedef struct alloc_header
{
    size_t size;
} alloc_header_t;

//Helper for multiplication
static int multiply_size(size_t n, size_t size, size_t *out)
{
    if (n != 0 && size > SIZE_MAX / n)
    {
        return -1;
    }

    *out = n * size;
    return 0;
}

void *my_malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    //Allocation in specific heap, also immediately throws NULL if allocation fails
    alloc_header_t *head = k_heap_alloc(&app_heap, sizeof(*head) + size, K_NO_WAIT);
    if (!head)
    {
        //here head = NULL, so we can just return it
        return NULL;
    }

    head->size = size;
    return (void *)(head + 1);
}

void *my_calloc(size_t n, size_t size)
{
    size_t total;

    if (multiply_size(n, size, &total) < 0)
    {
        return NULL;
    }

    void *ptr = my_malloc(total);
    if (!ptr)
    {
        return NULL;
    }

    memset(ptr, 0, total);
    return ptr;
}

void my_free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    alloc_header_t *head = ((alloc_header_t *)ptr) - 1;
    k_heap_free(&app_heap, head);
}

void *my_realloc(void *ptr, size_t size)
{
    if (!ptr)
    {
        return my_malloc(size);
    }

    if (size == 0)
    {
        my_free(ptr);
        return NULL;
    }

    alloc_header_t *old_head = ((alloc_header_t *)ptr) - 1;
    void *new_ptr = my_malloc(size);
    if (!new_ptr)
    {
        return NULL;
    }

    size_t copy_size = old_head->size < size ? old_head->size : size;
    memcpy(new_ptr, ptr, copy_size);
    my_free(ptr);

    return new_ptr;
}
