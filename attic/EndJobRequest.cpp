#include "EndJobRequest.hpp"
#include "CentralService.hpp"
#include "RemoteCommandService.hpp"

namespace
{

const uint16_t DATABASE_SERVICE = 0xe004;

}

namespace smile
{

EndJobRequest::EndJobRequest(uint16_t serverId)
    : PacketRequest((serverId == DATABASE_SERVICE) ? 40 : 20)
{
    m_packet.setServerId(serverId);
    switch (serverId)
    {
    case CentralService::IDENTIFIER:
        m_packet.setRequestReplyId(0x1400);
        break;
    case RemoteCommandService::IDENTIFIER:
        m_packet.setRequestReplyId(0x1004);
        break;
    case DATABASE_SERVICE:
        m_packet.setRequestReplyId(0x1fff);
        m_packet.setTemplateLength(20);
        break;
    }
}

const char* EndJobRequest::getName() const
{
    return "end job";
}

}
