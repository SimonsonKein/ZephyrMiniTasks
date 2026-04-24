#include <stdint.h>
#include <string.h>
// Idk how much heap size actually needs to be, but 1KB should be enough for testing
#define HEAP_SIZE 1024
#define MALLOC_ALIGNMENT sizeof(void *)

static uint8_t heap[HEAP_SIZE];

typedef struct block_header_t
{
    size_t size;
    struct block_header_t *next;
    uint8_t is_free;
    uint8_t reserved[3];
} block_header_t;

static block_header_t *head = (block_header_t *)heap;

void my_alloc_init(void)
{
    // printk("Initializing custom allocator with heap size: %d bytes\n", HEAP_SIZE);
    // printk("Allocation alignment: %d bytes\n", MALLOC_ALIGNMENT);
    head->size = HEAP_SIZE - sizeof(block_header_t);
    head->is_free = 1;
    head->next = NULL;
}

void *my_malloc(size_t size)
{
    size = (size + MALLOC_ALIGNMENT - 1) & ~(MALLOC_ALIGNMENT - 1);
    block_header_t *current = head;

    while (current)
    {
        if (current->is_free && current->size >= size)
        {
            if (current->size > size + sizeof(block_header_t) + MALLOC_ALIGNMENT)
            {
                block_header_t *new_block = (block_header_t *)((uint8_t *)current + sizeof(block_header_t) + size);
                new_block->size = current->size - size - sizeof(block_header_t);
                new_block->is_free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }
            current->is_free = 0;
            return (void *)(current + 1);
        }
        current = current->next;
    }
    return NULL;
}

void my_free(void *ptr)
{
    if (!ptr)
    {
        return;
    }
    block_header_t *header = (block_header_t *)ptr - 1;
    header->is_free = 1;

    if (header->next && header->next->is_free)
    {
        header->size += sizeof(block_header_t) + header->next->size;
        header->next = header->next->next;
    }
}

/**
 * @note Here can be error cause of non-enough size for duplication
 */
void *my_realloc(void *ptr, size_t size)
{
    if (!ptr)
    {
        return my_malloc(size);
    }
    block_header_t *header = (block_header_t *)ptr - 1;
    if (header->size >= size)
    {
        return ptr;
    }

    void *new_ptr = my_malloc(size);
    if (new_ptr)
    {
        memcpy(new_ptr, ptr, header->size);
        my_free(ptr);
    }
    return new_ptr;
}