#include <smile/os400_version.hpp>

#include <unicode/ustdio.h>

#include <stdexcept>
#include <sstream>

namespace smile
{

os400_version::os400_version(std::uint16_t version, std::uint8_t release, std::uint8_t modification)
    : packed_((static_cast<std::uint32_t>(version) << 16) |
              (static_cast<std::uint32_t>(release) << 8) | modification)
{
}

os400_version::os400_version(UnicodeString& desc)
{
    unsigned version;
    unsigned release;
    unsigned modification;
    int count = u_sscanf(desc.getTerminatedBuffer(), "V%uR%uM%u", &version, &release, &modification);
    if (count != 3)
        throw std::runtime_error("Error scanning version descriptor");
    packed_ = (version << 16) | (release << 8) | modification;
}

std::string os400_version::to_text() const
{
    std::ostringstream stream;
    stream << 'V' << static_cast<unsigned>(version())
           << 'R' << static_cast<unsigned>(release())
           << 'M' << static_cast<unsigned>(modification());
    return stream.str();
}

}