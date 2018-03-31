#include "LogUtil.hpp"

namespace smile
{

namespace LogUtil
{

Logger& smileLogger()
{
    static std::string name("smile");
    return Logger::getLogger(name);
}

std::string bytes(const std::vector<uint8_t>& bytes)
{
    std::ostringstream stream;
    stream.setf(std::ios::hex, std::ios::basefield);
    for (std::vector<uint8_t>::const_iterator itor = bytes.begin();
         itor != bytes.end();
         itor++)
    {
        stream << static_cast<unsigned>(*itor) << " ";
    }
    return stream.str();
}

}

}
