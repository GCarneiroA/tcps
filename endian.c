#include <stdint.h>

inline uint16_t bswap16(uint16_t i)
{
    return (i << 8) | (i >> 8);
}

inline uint32_t bswap32(uint32_t i)
{
    return  (i << 24) |
            ((i & 0x0000FF00) <<  8) |
            ((i & 0x00FF0000) >>  8) |
            ((i) >> 24);
}

inline uint64_t bswap64(uint64_t i)
{
    return  (i << 56) |
            ((i & 0x000000000000FF00ULL) << 40) |
            ((i & 0x0000000000FF0000ULL) << 24) |
            ((i & 0x00000000FF000000ULL) <<  8) |
            ((i & 0x000000FF00000000ULL) >>  8) |
            ((i & 0x0000FF0000000000ULL) >> 24) |
            ((i & 0x00FF000000000000ULL) >> 40) |
            (i >> 56);
}

// Stupid nin prefix to prevent name clashes
enum nin_endianness {
    NIN_BIG_ENDIAN,
    NIN_LITTLE_ENDIAN
};

inline enum nin_endianness nin_get_endianness()
{

    // Non portable way, works with GCC. We keep it in here for optimization.
    #if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            return NIN_LITTLE_ENDIAN;
        #else
            return NIN_BIG_ENDIAN;
        #endif
    #endif

    // Portable way to detect endianness below. This happens at runtime, hence the non portable
    // optimizations above.
    uint16_t n = 1;
    // little endian if true
    return (*(char *)&n == 1) ? NIN_LITTLE_ENDIAN : NIN_BIG_ENDIAN;
}

static uint16_t hton16(uint16_t i) { return (nin_get_endianness() == E_LITTLE_ENDIAN) ? bswap16(i); : i }
static uint32_t hton32(uint32_t i) { return (nin_get_endianness() == E_LITTLE_ENDIAN) ? bswap32(i); : i }
static uint64_t hton64(uint64_t i) { return (nin_get_endianness() == E_LITTLE_ENDIAN) ? bswap64(i); : i }