#include "PacketRequest.hpp"
#include "LogUtil.hpp"
#include <assert.h>

namespace smile
{

PacketRequest::PacketRequest(size_t size)
    : m_packet(size)
{
    assert(size >= 20);
}

PacketRequest::PacketRequest(const PacketRequest& request)
    : m_packet(request.m_packet)
{
}

PacketRequest& PacketRequest::operator= (const PacketRequest& request)
{
    if (&request != this)
        m_packet = request.m_packet;
    return *this;
}

void PacketRequest::send(Connection& connection)
{
    SMILE_LOG_DEBUG(LogUtil::smileLogger(), "Sending " << getName() <<
        " request...");
    m_packet.write(connection);
}

}
