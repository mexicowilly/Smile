#ifndef SMILE_OS400_VERSION_HPP
#define SMILE_OS400_VERSION_HPP

#include <boost/operators.hpp>

#include <string>
#include <cstdint>

namespace smile
{

class os400_version : public boost::totally_ordered<os400_version>
{
public:
    os400_version();
    os400_version(std::uint32_t packed);
    os400_version(std::uint16_t version, std::uint8_t release, std::uint8_t modification);
    os400_version(const std::string& desc);

    bool operator== (const os400_version& other) const;
    bool operator< (const os400_version& other) const;

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

inline bool os400_version::operator== (const os400_version& other) const
{
    return packed_ == other.packed_;
}

inline bool os400_version::operator< (const os400_version& other) const
{
    return packed_ < other.packed_;
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
