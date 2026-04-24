#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>

static int copy_file_bytes(const char *src_path, const char *dst_path)
{
    struct fs_file_t source;
    struct fs_file_t destination;
    uint8_t buffer[128];

    fs_file_t_init(&source);
    fs_file_t_init(&destination);

    int rc = fs_open(&source, src_path, FS_O_READ);
    if (rc < 0)
    {
        return rc;
    }

    rc = fs_open(&destination, dst_path, FS_O_CREATE | FS_O_WRITE | FS_O_TRUNC);
    if (rc < 0)
    {
        fs_close(&source);
        return rc;
    }

    while (1)
    {
        size_t read_bytes = fs_read(&source, buffer, sizeof(buffer));
        if (read_bytes < 0)
        {
            rc = (int)read_bytes;
            break;
        }
        if (read_bytes == 0)
        {
            rc = 0;
            break;
        }

        ssize_t offset = 0;
        while (offset < read_bytes)
        {
            ssize_t wrote = fs_write(&destination, &buffer[offset], (size_t)(read_bytes - offset));
            if (wrote < 0)
            {
                rc = (int)wrote;
                break;
            }
            if (wrote == 0)
            {
                rc = -EIO;
                break;
            }
            offset += wrote;
        }

        if (rc < 0)
        {
            break;
        }
    }

    fs_close(&destination);
    fs_close(&source);
    return rc;
}

int c_file_to_binary(const char *c_path, const char *bin_path)
{
    return copy_file_bytes(c_path, bin_path);
}

static inline char nibble_to_hex(uint8_t nibble)
{
    return (nibble < 10U) ? (char)('0' + nibble) : (char)('A' + (nibble - 10U));
}

int binary_to_hex(const char *bin_path, const char *hex_path)
{
    struct fs_file_t in;
    struct fs_file_t out;
    uint8_t byte;
    char pair[2];

    fs_file_t_init(&in);
    fs_file_t_init(&out);

    int rc = fs_open(&in, bin_path, FS_O_READ);
    if (rc < 0)
    {
        return rc;
    }

    rc = fs_open(&out, hex_path, FS_O_CREATE | FS_O_WRITE | FS_O_TRUNC);
    if (rc < 0)
    {
        fs_close(&in);
        return rc;
    }

    while (1)
    {
        ssize_t read_bytes = fs_read(&in, &byte, 1);
        if (read_bytes < 0)
        {
            rc = (int)read_bytes;
            break;
        }
        if (read_bytes == 0)
        {
            rc = 0;
            break;
        }

        pair[0] = nibble_to_hex((uint8_t)((byte >> 4) & 0x0F));
        pair[1] = nibble_to_hex((uint8_t)(byte & 0x0F));

        ssize_t wrote = fs_write(&out, pair, sizeof(pair));
        if (wrote < 0)
        {
            rc = (int)wrote;
            break;
        }
        if (wrote != (ssize_t)sizeof(pair))
        {
            rc = -EIO;
            break;
        }
    }

    fs_close(&out);
    fs_close(&in);
    return rc;
}

static int hex_nibble_value(int ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    return -1;
}

int hex_to_binary(const char *hex_path, const char *bin_path)
{
    struct fs_file_t in;
    struct fs_file_t out;
    int high_nibble = -1;

    fs_file_t_init(&in);
    fs_file_t_init(&out);

    int rc = fs_open(&in, hex_path, FS_O_READ);
    if (rc < 0)
    {
        return rc;
    }

    rc = fs_open(&out, bin_path, FS_O_CREATE | FS_O_WRITE | FS_O_TRUNC);
    if (rc < 0)
    {
        fs_close(&in);
        return rc;
    }

    while (1)
    {
        uint8_t ch;
        ssize_t read_bytes = fs_read(&in, &ch, 1);
        if (read_bytes < 0)
        {
            rc = (int)read_bytes;
            break;
        }
        if (read_bytes == 0)
        {
            rc = (high_nibble < 0) ? 0 : -EINVAL;
            break;
        }

        if (isspace((unsigned char)ch))
        {
            continue;
        }

        int nibble = hex_nibble_value((int)ch);
        if (nibble < 0)
        {
            rc = -EINVAL;
            break;
        }

        if (high_nibble < 0)
        {
            high_nibble = nibble;
            continue;
        }

        uint8_t byte = (uint8_t)((high_nibble << 4) | nibble);
        ssize_t wrote = fs_write(&out, &byte, 1);
        if (wrote < 0)
        {
            rc = (int)wrote;
            break;
        }
        if (wrote != 1)
        {
            rc = -EIO;
            break;
        }

        high_nibble = -1;
    }

    fs_close(&out);
    fs_close(&in);
    return rc;
}

int binary_to_c_file(const char *bin_path, const char *c_path)
{
    return copy_file_bytes(bin_path, c_path);
}

int printk_file_text(const char *path)
{
    struct fs_file_t file;
    uint8_t buffer[128];

    fs_file_t_init(&file);

    int rc = fs_open(&file, path, FS_O_READ);
    if (rc < 0)
    {
        return rc;
    }

    while (1)
    {
        ssize_t read_bytes = fs_read(&file, buffer, sizeof(buffer) - 1U);
        if (read_bytes < 0)
        {
            rc = (int)read_bytes;
            break;
        }
        if (read_bytes == 0)
        {
            rc = 0;
            break;
        }

        buffer[read_bytes] = '\0';
        printk("%s", buffer);
    }

    fs_close(&file);
    return rc;
}
