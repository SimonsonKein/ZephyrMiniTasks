#include <stdint.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include "custom_allocation.h"
#include "file_converter.h"

int main(void)
{

    printk("Custom allocator initialized.\n");

    int *my_array = (int *)my_malloc(5 * sizeof(int));

    if (my_array == NULL)
    {
        printk("Failed to allocate memory!\n");
        return -1;
    }

    for (int i = 0; i < 5; i++)
    {
        my_array[i] = i * 10;
    }

    int *new_ptr = (int *)my_realloc(my_array, 10 * sizeof(int));
    if (new_ptr != NULL)
    {
        my_array = new_ptr;
    }

    my_free(my_array);
    printk("Memory freed.\n");
    return 0;
}
