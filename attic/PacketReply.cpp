#include "PacketReply.hpp"
#include "LogUtil.hpp"
#include <assert.h>

namespace smile
{

PacketReply::PacketReply(const AccessPacket& packet)
    : m_packet(packet)
{
}

PacketReply::PacketReply(const PacketReply& reply)
    : m_packet(reply.m_packet)
{
}

PacketReply& PacketReply::operator= (const PacketReply& reply)
{
    if (&reply != this)
        m_packet = reply.m_packet;
    return *this;
}

int PacketReply::findOffset(unsigned start, uint16_t code) const
{
    for (int offset = start;
         offset < static_cast<int>(m_packet.getSize());
         offset += m_packet.getInt32(offset))
    {
        if (m_packet.getInt16(offset + 4) == code)
            return offset;
    }
    return -1;
}

const char* PacketReply::getName() const
{
    return "packet";
}

uint32_t PacketReply::getReturnCode() const
{
    return m_packet.getInt32(20);
}

void PacketReply::receive(Connection& connection)
{
    SMILE_LOG_DEBUG(LogUtil::smileLogger(), "Receiving " << getName() <<
        " reply...");
    m_packet.read(connection);
}

}
