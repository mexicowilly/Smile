#include "PortRequest.hpp"
#include <smile/TextConverter.hpp>
#include <smile/Exception.hpp>
#include "LogUtil.hpp"
#include <stdexcept>

namespace smile
{

PortRequest::PortRequest(const char* const serviceName)
{
    try
    {
        TextConverter converter(SMILE_DEFAULT_CONVERTER, "US-ASCII");
        m_ascii = converter.convert(reinterpret_cast<const uint8_t*>(serviceName));
    }
    catch (Exception& e)
    {
        throw std::runtime_error(e.what());
    }
}

const char* PortRequest::getName() const
{
    return "port";
}

void PortRequest::send(Connection& connection)
{
    LogUtil::smileLogger().debug("Sending port request...");
    connection.write(&m_ascii[0], m_ascii.size());
}

}
