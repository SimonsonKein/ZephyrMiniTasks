#include <stdint.h>
#include <stdio.h>

uint16_t swap_16(uint16_t value)
{
    return (value >> 8) | (value << 8);
}

uint16_t bytes_to_le16(uint8_t *bytes)
{
    return (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8);
}

uint16_t bytes_to_be16(uint8_t *bytes)
{
    return ((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1];
}

void le16_to_bytes(uint16_t value, uint8_t *bytes)
{
    bytes[0] = value & 0xFF;        
    bytes[1] = (value >> 8) & 0xFF; 
}

void be16_to_bytes(uint16_t value, uint8_t *bytes)
{
    bytes[0] = (value >> 8) & 0xFF; 
    bytes[1] = value & 0xFF;       
}

uint32_t swap_32(uint32_t value)
{
    return ((value) << 24) |
           ((value & 0xFF00) << 8)  |
           ((value & 0xFF0000) >> 8)  |
           (value  >> 24);
}

uint32_t bytes_to_le32(uint8_t *bytes)
{
    return (uint32_t)bytes[0]       |
           ((uint32_t)bytes[1] << 8) |
           ((uint32_t)bytes[2] << 16)|
           ((uint32_t)bytes[3] << 24);
}

uint32_t bytes_to_be32(uint8_t *bytes)
{
    return ((uint32_t)bytes[0] << 24)|
           ((uint32_t)bytes[1] << 16)|
           ((uint32_t)bytes[2] << 8) |
           (uint32_t)bytes[3];
}

void le32_to_bytes(uint32_t value, uint8_t *bytes)
{
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24);
}

void be32_to_bytes(uint32_t value, uint8_t *bytes)
{
    bytes[0] = (value >> 24);
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

void print_bytes(const char *label, uint8_t *bytes, int count)
{
    printf("%s: ", label);
    for (int i = 0; i < count; i++) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

//Generated tests
int main(void)
{
    printf("=== 16-BIT CONVERSION ===\n");
    uint16_t val16 = 0x1234;
    printf("Original value: 0x%04X\n", val16);
    
    uint8_t bytes16[2];
    le16_to_bytes(val16, bytes16);
    print_bytes("Little-endian bytes", bytes16, 2);
    
    be16_to_bytes(val16, bytes16);
    print_bytes("Big-endian bytes", bytes16, 2);
    
    uint16_t swapped16 = swap_16(val16);
    printf("Swapped: 0x%04X\n\n", swapped16);
    
    printf("=== 32-BIT CONVERSION ===\n");
    uint32_t val32 = 0x12345678;
    printf("Original value: 0x%08X\n", val32);
    
    uint8_t bytes32[4];
    le32_to_bytes(val32, bytes32);
    print_bytes("Little-endian bytes", bytes32, 4);
    
    be32_to_bytes(val32, bytes32);
    print_bytes("Big-endian bytes", bytes32, 4);
    
    uint32_t swapped32 = swap_32(val32);
    printf("Swapped: 0x%08X\n\n", swapped32);
    
    printf("=== READING FROM BUFFER ===\n");
    uint8_t buffer_le[] = {0x78, 0x56, 0x34, 0x12};
    uint8_t buffer_be[] = {0x12, 0x34, 0x56, 0x78};
    
    printf("LE buffer: ");
    print_bytes("", buffer_le, 4);
    printf("Read as LE: 0x%08X\n", bytes_to_le32(buffer_le));
    
    printf("BE buffer: ");
    print_bytes("", buffer_be, 4);
    printf("Read as BE: 0x%08X\n", bytes_to_be32(buffer_be));
    
    return 0;
}