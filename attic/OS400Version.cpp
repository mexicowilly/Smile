#include <smile/OS400Version.hpp>
#include <smile/Exception.hpp>
#include <sstream>

namespace smile
{

OS400Version::OS400Version(uint16_t version, uint8_t release, uint8_t modification)
    : m_packed((static_cast<uint32_t>(version) << 16) |
               (static_cast<uint32_t>(release) << 8) |
               modification)
{
}

OS400Version::OS400Version(const Text& descriptor)
{
    unsigned version;
    unsigned release;
    unsigned modification;
    std::string converted = descriptor.convertToDefault();
    int count = sscanf(converted.c_str(), "V%uR%uM%u", &version, &release, &modification);
    if (count != 3)
        throw InvalidStateException(__FILE__, __LINE__, "Error scanning version descriptor");
    m_packed = (version << 16) | (release << 8) | modification;
}

Text OS400Version::toText() const
{
    std::ostringstream stream;
    stream << 'V' << static_cast<unsigned>(getVersion())
           << 'R' << static_cast<unsigned>(getRelease())
           << 'M' << static_cast<unsigned>(getModification());
    return Text(stream.str().c_str());
}

}
