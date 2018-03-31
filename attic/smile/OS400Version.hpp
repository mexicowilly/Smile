#if !defined(SMILE_OS400VERSION_HPP__)
#define SMILE_OS400VERSION_HPP__

#include <inttypes.h>
#include <smile/Text.hpp>

namespace smile
{

class OS400Version
{
public:
    OS400Version();
    OS400Version(uint32_t packed);
    OS400Version(uint16_t version, uint8_t release, uint8_t modification);
    OS400Version(const Text& descriptor);
    OS400Version(const OS400Version& version);

    OS400Version& operator= (const OS400Version& version);
    bool operator== (const OS400Version& version) const;
    bool operator!= (const OS400Version& version) const;
    bool operator< (const OS400Version& version) const;
    bool operator<= (const OS400Version& version) const;
    bool operator> (const OS400Version& version) const;
    bool operator>= (const OS400Version& version) const;

    uint8_t getModification() const;
    uint32_t getPacked() const;
    uint8_t getRelease() const;
    uint16_t getVersion() const;
    Text toText() const;

private:
    uint32_t m_packed;
};

inline OS400Version::OS400Version()
    : m_packed(0)
{
}

inline OS400Version::OS400Version(uint32_t packed)
    : m_packed(packed)
{
}

inline OS400Version::OS400Version(const OS400Version& version)
    : m_packed(version.m_packed)
{
}

inline OS400Version& OS400Version::operator= (const OS400Version& version)
{
    m_packed = version.m_packed;
    return *this;
}

inline bool OS400Version::operator== (const OS400Version& version) const
{
    return m_packed == version.m_packed;
}

inline bool OS400Version::operator!= (const OS400Version& version) const
{
    return !operator==(version);
}

inline bool OS400Version::operator< (const OS400Version& version) const
{
    return getPacked() < version.getPacked();
}

inline bool OS400Version::operator<= (const OS400Version& version) const
{
    return getPacked() <= version.getPacked();
}

inline bool OS400Version::operator> (const OS400Version& version) const
{
    return getPacked() > version.getPacked();
}

inline bool OS400Version::operator>= (const OS400Version& version) const
{
    return getPacked() >= version.getPacked();
}

inline uint8_t OS400Version::getModification() const
{
    return m_packed & 0xff;
}

inline uint32_t OS400Version::getPacked() const
{
    return m_packed;
}

inline uint8_t OS400Version::getRelease() const
{
    return (m_packed >> 8) & 0xff;
}

inline uint16_t OS400Version::getVersion() const
{
    return m_packed >> 16;
}

}

#endif
