


#include <iostream>

int main()
{
    system("clear");

    std::cout << std::endl << "Tamanhos dos tipos C++" << std::endl << std::endl;

    std::cout << "...........Tipos assinados..........."    << std::endl;
    std::cout << "char          : " << sizeof(char)             << " byte,  ou : " << sizeof(char)              * 8 << " bits." << std::endl;
    std::cout << "short         : " << sizeof(short)            << " bytes, ou : " << sizeof(short)             * 8 << " bits." << std::endl;
    std::cout << "long          : " << sizeof(long)             << " bytes, ou : " << sizeof(long)              * 8 << " bits." << std::endl;
    std::cout << "int           : " << sizeof(int)              << " bytes, ou : " << sizeof(int)               * 8 << " bits." << std::endl;
    std::cout << "int8_t        : " << sizeof(int8_t)           << " byte,  ou : " << sizeof(int8_t)            * 8 << " bits." << std::endl;
    std::cout << "int16_t       : " << sizeof(int16_t)          << " bytes, ou : " << sizeof(int16_t)           * 8 << " bits." << std::endl;
    std::cout << "int32_t       : " << sizeof(int32_t)          << " bytes, ou : " << sizeof(int32_t)           * 8 << " bits." << std::endl;
    std::cout << "int64_t       : " << sizeof(int64_t)          << " bytes, ou : " << sizeof(int64_t)           * 8 << " bits." << std::endl;

    std::cout << std::endl;
    std::cout << ".........Tipos não assinados........." << std::endl;
    std::cout << "uchar         : " << sizeof(unsigned char)    << " byte,  ou : " << sizeof(unsigned char)     * 8 << " bits." << std::endl;
    std::cout << "ushort        : " << sizeof(unsigned short)   << " bytes, ou : " << sizeof(unsigned short)    * 8 << " bits." << std::endl;
    std::cout << "ulong         : " << sizeof(unsigned long)    << " bytes, ou : " << sizeof(unsigned long)     * 8 << " bits." << std::endl;
    std::cout << "uint          : " << sizeof(unsigned int)     << " bytes, ou : " << sizeof(unsigned int)      * 8 << " bits." << std::endl;
    std::cout << "uint8_t       : " << sizeof(uint8_t)          << " byte,  ou : " << sizeof(uint8_t)           * 8 << " bits." << std::endl;
    std::cout << "uint16_t      : " << sizeof(uint16_t)         << " bytes, ou : " << sizeof(uint16_t)          * 8 << " bits." << std::endl;
    std::cout << "uint32_t      : " << sizeof(uint32_t)         << " bytes, ou : " << sizeof(uint32_t)          * 8 << " bits." << std::endl;
    std::cout << "uint64_t      : " << sizeof(uint64_t)         << " bytes, ou : " << sizeof(uint64_t)          * 8 << " bits." << std::endl;

    std::cout << std::endl << std::endl;

    return 0;
}
