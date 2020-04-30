
#include "byteorder.h"

int main()
{
    unsigned int a = 0x12345678;
    unsigned int b = swap_bytes(a);
    printf("%08x\n", a);
    printf("%08x\n", b);


    unsigned short c = 0xabcd;
    unsigned short d = swap_bytes(c);
    printf("%04x\n", c);
    printf("%04x\n", d);

    return 0;
}
