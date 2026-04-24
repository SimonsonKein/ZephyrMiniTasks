/* #include <stdint.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>


int c_file_to_binary(const char *c_path, const char *bin_path);
int binary_to_hex(const char *bin_path, const char *hex_path);
int hex_to_binary(const char *hex_path, const char *bin_path);
int binary_to_c_file(const char *bin_path, const char *c_path);


int main(void)
{

    my_alloc_init();
    printk("Custom allocator initialized.\n");

    // 2. MALLOC: Request memory for an array of integers
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

    int rc1 = c_file_to_binary("/lfs/input.c", "/lfs/output.bin");
    int rc2 = binary_to_hex("/lfs/output.bin", "/lfs/output.hex");
    int rc3 = hex_to_binary("/lfs/output.hex", "/lfs/output_from_hex.bin");
    int rc4 = binary_to_c_file("/lfs/output_from_hex.bin", "/lfs/output_restored.c");

    printk("Convert results: c->bin=%d, bin->hex=%d, hex->bin=%d, bin->c=%d\n",
           rc1, rc2, rc3, rc4);

    return 0;
}
 */