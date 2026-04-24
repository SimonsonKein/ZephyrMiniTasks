#ifndef FILE_CONVERTER_H
#define FILE_CONVERTER_H

int c_file_to_binary(const char *c_path, const char *bin_path);
int binary_to_hex(const char *bin_path, const char *hex_path);
int hex_to_binary(const char *hex_path, const char *bin_path);
int binary_to_c_file(const char *bin_path, const char *c_path);
int printk_file_text(const char *path);

#endif // FILE_CONVERTER_H
