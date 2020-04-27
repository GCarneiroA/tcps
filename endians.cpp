
#include <stdint.h>
#include <stdio.h>


int main(void) {
    uint32_t word = 0x0A0B0C0D;     // Um número inteiro de 32 bits não assinado
    char *pointer = (char*) &word;  // Um ponteiro para o primeiro octeto da palavra

    for (int i = 0; i < 4; i++) {
        printf("%02x ", (unsigned int)pointer[i]);
    }

    puts("");
}


static uint32_t changeEndianness32(uint32_t value)
{
    uint32_t result = 0;
    result |= (value & 0x000000FF) << 24;
    result |= (value & 0x0000FF00) << 8;
    result |= (value & 0x00FF0000) << 8;
    result |= (value & 0xFF000000) << 24;
    return result;
}


static uint64_t changeEndianness64(uint64_t value)
{
    return (uint64_t)(changeEndianness32(value & 0xFFFFFFFF)) << 32 | changeEndianness32(value >> 32);
}






/*
#include <stdlib.h>
#include <iostream>

union {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
} u = { .u64 = 0x4A };


int main()
{
    puts(u.u8 == u.u16 && u.u8 == u.u32 && u.u8 == u.u64 ? "true" : "false");
    std::cout << std::endl;

    printf("u8 : %d\n", u.u8);
    
    std::cout << "u8 : " << u.u8 << std::endl;
    std::cout << "u16: " << u.u16 << std::endl;
    std::cout << "u32: " << u.u32 << std::endl;
    std::cout << "u64: " << u.u64 << std::endl;


    return 0;
}
*/