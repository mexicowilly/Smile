#include "PortReply.hpp"
#include "ByteOrder.hpp"
#include "LogUtil.hpp"

namespace smile
{

const char* PortReply::getName() const
{
    return "port";
}

int PortReply::getPort() const
{
    if (m_bytes[0] != 0x2b)
        return -1;
    int32_t value;
    memcpy(&value, m_bytes + 1, sizeof(value));
    return N32_TO_H(value);
}

void PortReply::receive(Connection& connection)
{
    LogUtil::smileLogger().debug("Receiving port reply...");
    connection.read(m_bytes, 5);
}

}
