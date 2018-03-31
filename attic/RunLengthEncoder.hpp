#if !defined(SMILE_RUNLENGTHENCODER_HPP__)
#define SMILE_RUNLENGTHENCODER_HPP__

#include <inttypes.h>
#include <vector>

namespace smile
{

class RunLengthEncoder
{
public:
    static const uint8_t DEFAULT_ESCAPE = 0x1b;

    static std::vector<uint8_t> decode(const std::vector<uint8_t>& bytes, uint8_t escape = DEFAULT_ESCAPE);
    static std::vector<uint8_t> encode(const std::vector<uint8_t>& bytes, uint8_t escape = DEFAULT_ESCAPE);
};

}

#endif
