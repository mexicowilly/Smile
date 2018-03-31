#if !defined(SMILE_BYTEORDER_HPP__)
#define SMILE_BYTEORDER_HPP__

#include "Config.hpp"

#if defined(SMILE_BIG_ENDIAN)

#define H16_TO_N(val) val
#define H32_TO_N(val) val
#define H64_TO_N(val) val
#define N16_TO_H(val) val
#define N32_TO_H(val) val
#define N64_TO_H(val) val

#else

#define H16_TO_N(val) smile::swapInt16(val)
#define H32_TO_N(val) smile::swapInt32(val)
#define H64_TO_N(val) smile::swapInt64(val)
#define N16_TO_H(val) smile::swapInt16(val)
#define N32_TO_H(val) smile::swapInt32(val)
#define N64_TO_H(val) smile::swapInt64(val)

#endif

namespace smile
{

template <class T>
void fastSwap(T& one, T& two)
{
    one ^= two;
    two ^= one;
    one ^= two;
}

static inline uint16_t swapInt16(uint16_t val)
{
    union
    {
        uint16_t    value;
        uint8_t     bytes[2];
    } result;
    result.value = val;
    fastSwap(result.bytes[0], result.bytes[1]);
    return result.value;
}

static inline uint32_t swapInt32(uint32_t val)
{
    union
    {
        uint32_t    value;
        uint8_t     bytes[4];
    } result;
    result.value = val;
    fastSwap(result.bytes[0], result.bytes[3]);
    fastSwap(result.bytes[1], result.bytes[2]);
    return result.value;
}

static inline uint64_t swapInt64(uint64_t val)
{
    union
    {
        uint64_t    value;
        uint8_t     bytes[8];
    } result;
    result.value = val;
    fastSwap(result.bytes[0], result.bytes[7]);
    fastSwap(result.bytes[1], result.bytes[6]);
    fastSwap(result.bytes[2], result.bytes[5]);
    fastSwap(result.bytes[3], result.bytes[4]);
    return result.value;
}

}

#endif
