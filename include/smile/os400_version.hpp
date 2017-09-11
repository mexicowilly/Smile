#ifndef SMILE_OS400_VERSION_HPP
#define SMILE_OS400_VERSION_HPP

#include <unicode/unistr.h>

#include <cstdint>

namespace smile
{

class os400_version
{
public:
    os400_version();
    os400_version(std::uint32_t packed);
    os400_version(std::uint16_t version, std::uint8_t release, std::uint8_t modification);
    // Not const because we have to null-terminate it
    os400_version(UnicodeString& desc);

    std::uint8_t modification() const;
    std::uint32_t packed() const;
    std::uint8_t release() const;
    std::string to_text() const;
    std::uint16_t version() const;

private:
    std::uint32_t packed_;
};

inline os400_version::os400_version()
    : packed_(0)
{
}

inline os400_version::os400_version(std::uint32_t packed)
    : packed_(packed)
{
}

inline std::uint8_t os400_version::modification() const
{
    return packed_ & 0xff;
}

inline std::uint32_t os400_version::packed() const
{
    return packed_;
}

inline std::uint8_t os400_version::release() const
{
    return (packed_ >> 8) & 0xff;
}

inline std::uint16_t os400_version::version() const
{
    return packed_ >> 16;
}

}

#endif
